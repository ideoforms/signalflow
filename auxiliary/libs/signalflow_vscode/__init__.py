"""
SignalFlow Status - Python package for updating VS Code status panel
"""

import threading
import requests
import atexit
import socket
import time
import json
import os

from signalflow import AudioGraph, amplitude_to_db
from typing import Optional, Literal

def get_notebook_port() -> int:
    """Get the port for the current notebook using VS Code variable"""
    try:
        # Try to access VS Code variable from the calling frame
        import inspect
        frame = inspect.currentframe()
        while frame:
            frame = frame.f_back
            if frame and '__vsc_ipynb_file__' in frame.f_globals:
                notebook_path = frame.f_globals['__vsc_ipynb_file__']
                basename = os.path.basename(notebook_path)

                # Calculate hash same as VS Code extension
                hash_obj = 0
                for char in basename:
                    hash_obj = ((hash_obj << 5) - hash_obj) + ord(char)
                    hash_obj = ((hash_obj & 0xFFFFFFFF) ^ 0x80000000) - 0x80000000

                remainder = hash_obj % 1000
                if hash_obj < 0 and remainder != 0:
                    remainder = remainder - 1000
                return 8765 + abs(remainder)
    except:
        pass

    # Fallback to default port
    return 8765

class SignalFlowStatus:
    def __init__(self, host: str = "localhost", port: Optional[int] = None):
        if port is None:
            port = get_notebook_port()
        self.base_url = f"http://{host}:{port}"
        self.port = port
        self.session = requests.Session()

    def update(self, key: str, value: str, status_type: Literal["info", "warning", "error", "success"] = "info") -> bool:
        try:
            response = self.session.post(
                f"{self.base_url}/status",
                json={"key": key, "value": value, "type": status_type},
                timeout=1
            )
            return response.status_code == 200
        except:
            return False

    def info(self, key: str, value: str) -> bool:
        return self.update(key, value, "info")

    def success(self, key: str, value: str) -> bool:
        return self.update(key, value, "success")

    def warning(self, key: str, value: str) -> bool:
        return self.update(key, value, "warning")

    def error(self, key: str, value: str) -> bool:
        return self.update(key, value, "error")

    def clear(self) -> bool:
        try:
            response = self.session.delete(f"{self.base_url}/status", timeout=1)
            return response.status_code == 200
        except:
            return False

# Create global instance
status = SignalFlowStatus()


def clear_audio_graph() -> bool:
    """Clear the AudioGraph singleton"""
    try:
        from signalflow import AudioGraph
        graph = AudioGraph.get_shared_graph()
        if graph is not None:
            graph.clear()
            status.info("AudioGraph", "Cleared")
            return True
        else:
            status.info("AudioGraph", "No active graph to clear")
            return True
    except ImportError:
        status.error("AudioGraph", "SignalFlow not available")
        return False
    except Exception as e:
        status.error("AudioGraph", f"Clear failed: {str(e)}")
        return False

# RPC Listener for remote clear commands
class SignalFlowRPCListener:
    def __init__(self, port: Optional[int] = None):
        if port is None:
            port = get_notebook_port() + 1
        self.port = port
        self.running = False
        self.server_thread = None
        self.tcp_server = None

    def start_listener(self):
        if self.running:
            return
        self.running = True
        self.server_thread = threading.Thread(target=self._run_tcp_server, daemon=True)
        self.server_thread.start()

    def stop_listener(self):
        self.running = False
        if self.tcp_server:
            self.tcp_server.close()

    def _run_tcp_server(self):
        try:
            self.tcp_server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.tcp_server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            self.tcp_server.bind(('localhost', self.port))
            self.tcp_server.listen(1)

            while self.running:
                try:
                    conn, addr = self.tcp_server.accept()
                    threading.Thread(target=self._handle_connection, args=(conn,), daemon=True).start()
                except OSError:
                    break
        except:
            pass

    def _handle_connection(self, conn):
        try:
            data = conn.recv(1024).decode('utf-8')
            if data.strip() == 'clear':
                result = clear_audio_graph()
                response = json.dumps({"success": result, "message": "AudioGraph cleared"})
                conn.send(response.encode('utf-8'))
            else:
                response = json.dumps({"success": False, "message": "Unknown command"})
                conn.send(response.encode('utf-8'))
        except:
            error_response = json.dumps({"success": False, "message": "Error"})
            conn.send(error_response.encode('utf-8'))
        finally:
            conn.close()

# Global RPC listener
_rpc_listener = None

def start_signalflow_listener():
    """Start the SignalFlow RPC listener for remote clear commands"""
    global _rpc_listener
    if _rpc_listener is None:
        _rpc_listener = SignalFlowRPCListener(port=status.port + 1)
    _rpc_listener.start_listener()

    # Start a background thread to display status updates
    thread = threading.Thread(target=run_signalflow_status_display_thread, daemon=True)
    thread.start()

def stop_signalflow_listener():
    global _rpc_listener
    if _rpc_listener:
        _rpc_listener.stop_listener()

def run_signalflow_status_display_thread():
    global _rpc_listener
    while _rpc_listener and _rpc_listener.running:
        graph = AudioGraph.get_shared_graph()
        if graph:
            if graph.has_raised_audio_thread_error:
                status.error("AudioGraph", "Error in audio thread")
            else:
                status.success("AudioGraph", "Running")
                cpu_usage_percent = graph.cpu_usage * 100
                if cpu_usage_percent > 60:
                    status.warning("CPU", "%.1f%%" % cpu_usage_percent)
                else:
                    status.info("CPU", "%.1f%%" % cpu_usage_percent)
                memory_usage_mb = graph.memory_usage / (1024 * 1024)
                status.info("Memory", "%.1f MB" % memory_usage_mb)
                level_db = amplitude_to_db(graph.output_level_peak)
                if graph.output_level_peak > 1.0:
                    status.error("Level", "%.1fdB (clipping!)" % level_db)
                else:
                    level_str = "%.1f" % level_db if level_db > -180 else "-"
                    status.info("Level", "%sdB" % level_str)
                status.info("Patches", str(graph.patch_count))
                status.info("Nodes", str(graph.node_count))
        else:
            status.info("AudioGraph", "Not created")
        time.sleep(0.5)

# Cleanup on exit
def _cleanup():
    global _rpc_listener
    if _rpc_listener:
        _rpc_listener.stop_listener()

atexit.register(_cleanup)

status.clear()
start_signalflow_listener()