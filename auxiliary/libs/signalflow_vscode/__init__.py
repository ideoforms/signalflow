"""
SignalFlow Status - Python package for updating VS Code status panel
"""

import threading
import requests
import atexit
import socket
import time
import json

from signalflow import AudioGraph, amplitude_to_db
from typing import Optional, Literal

class SignalFlowStatus:
    def __init__(self, host: str = "localhost", port: int = 8765):
        self.base_url = f"http://{host}:{port}"
        self.session = requests.Session()
        
    def update(self, key: str, value: str, status_type: Literal["info", "warning", "error", "success"] = "info") -> bool:
        """Update a status item in the VS Code panel"""
        try:
            response = self.session.post(
                f"{self.base_url}/status",
                json={
                    "key": key,
                    "value": value,
                    "type": status_type
                },
                timeout=1
            )
            return response.status_code == 200
        except requests.exceptions.RequestException:
            return False
    
    def info(self, key: str, value: str) -> bool:
        """Update with info status"""
        return self.update(key, value, "info")
    
    def success(self, key: str, value: str) -> bool:
        """Update with success status"""
        return self.update(key, value, "success")
    
    def warning(self, key: str, value: str) -> bool:
        """Update with warning status"""
        return self.update(key, value, "warning")
    
    def error(self, key: str, value: str) -> bool:
        """Update with error status"""
        return self.update(key, value, "error")
    
    def clear(self) -> bool:
        """Clear all status items"""
        try:
            response = self.session.delete(f"{self.base_url}/status", timeout=1)
            return response.status_code == 200
        except requests.exceptions.RequestException:
            return False
    
    def get_all(self) -> Optional[dict]:
        """Get all current status items"""
        try:
            response = self.session.get(f"{self.base_url}/status", timeout=1)
            if response.status_code == 200:
                return response.json()
            return None
        except requests.exceptions.RequestException:
            return None

# Global instance for convenience
status = SignalFlowStatus()

# Convenience functions
def update_status(key: str, value: str, status_type: Literal["info", "warning", "error", "success"] = "info") -> bool:
    """Update a status item (convenience function)"""
    return status.update(key, value, status_type)

def info(key: str, value: str) -> bool:
    """Update with info status (convenience function)"""
    return status.info(key, value)

def success(key: str, value: str) -> bool:
    """Update with success status (convenience function)"""
    return status.success(key, value)

def warning(key: str, value: str) -> bool:
    """Update with warning status (convenience function)"""
    return status.warning(key, value)

def error(key: str, value: str) -> bool:
    """Update with error status (convenience function)"""
    return status.error(key, value)

def clear_all() -> bool:
    """Clear all status items (convenience function)"""
    return status.clear()

def clear_audio_graph() -> bool:
    """Clear the AudioGraph singleton"""
    try:
        # Import AudioGraph and clear the shared graph
        graph = AudioGraph.get_shared_graph()
        
        # Check if graph exists before clearing
        if graph is not None:
            graph.clear()
            # Also update status to reflect the clear action
            info("AudioGraph", "Cleared")
            return True
        else:
            info("AudioGraph", "No active graph to clear")
            return True
    except ImportError:
        error("AudioGraph", "SignalFlow not available")
        return False
    except Exception as e:
        error("AudioGraph", f"Clear failed: {str(e)}")
        return False

# RPC Listener for remote clear commands
class SignalFlowRPCListener:
    def __init__(self, port: int = 8766):
        self.port = port
        self.running = False
        self.server_thread = None
        self.tcp_server = None
        
    def start_listener(self):
        """Start the RPC listener in a background thread"""
        if self.running:
            return
        
        # Clear any existing status before starting listener
        clear_all()
            
        self.running = True
        self.server_thread = threading.Thread(target=self._run_tcp_server, daemon=True)
        self.server_thread.start()
        
    def stop_listener(self):
        """Stop the RPC listener"""
        self.running = False
        if self.tcp_server:
            self.tcp_server.close()
            
    def _run_tcp_server(self):
        """Run TCP socket server"""
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
                    
        except Exception as e:
            pass
            
    def _handle_connection(self, conn):
        """Handle incoming RPC connection"""
        try:
            data = conn.recv(1024).decode('utf-8')
            
            if data.strip() == 'clear':
                result = clear_audio_graph()
                response = json.dumps({"success": result, "message": "AudioGraph cleared"})
                conn.send(response.encode('utf-8'))
            else:
                response = json.dumps({"success": False, "message": "Unknown command"})
                conn.send(response.encode('utf-8'))
        except Exception as e:
            error_response = json.dumps({"success": False, "message": str(e)})
            conn.send(error_response.encode('utf-8'))
        finally:
            conn.close()

# Global RPC listener instance
_rpc_listener = SignalFlowRPCListener()

def start_signalflow_listener():
    """Start the SignalFlow RPC listener for remote clear commands
    
    Call this function at the start of your notebook to enable cmd+. functionality:
    
    import signalflow_status
    signalflow_status.start_signalflow_listener()
    """
    _rpc_listener.start_listener()

    thread = threading.Thread(target=run_signalflow_status_display_thread, daemon=True)
    thread.start()
    
def stop_signalflow_listener():
    """Stop the SignalFlow RPC listener"""
    _rpc_listener.stop_listener()

def run_signalflow_status_display_thread():
    while _rpc_listener.running:
        graph = AudioGraph.get_shared_graph()
        if graph:
            if graph.has_raised_audio_thread_error:
                error("AudioGraph", "Error in audio thread")
            else:
                success("AudioGraph", "Running")
                cpu_usage_percent = graph.cpu_usage * 100
                if cpu_usage_percent > 60:
                    warning("CPU", "%.1f%%" % cpu_usage_percent)
                else:
                    info("CPU", "%.1f%%" % cpu_usage_percent)
                memory_usage_mb = graph.memory_usage / (1024 * 1024)
                info("Memory", "%.1f MB" % memory_usage_mb)
                level_db = amplitude_to_db(graph.output_level_peak)
                if graph.output_level_peak > 1.0:
                    error("Level", "%.1fdB (clipping!)" % level_db)
                else:
                    level_str = "%.1f" % level_db if level_db > -180 else "-"
                    info("Level", "%sdB" % level_str)
                info("Patches", str(graph.patch_count))
                info("Nodes", str(graph.node_count))
        else:
            info("AudioGraph", "Not created")
        time.sleep(0.5)


# Auto-cleanup on exit
atexit.register(stop_signalflow_listener)

# Example usage and testing
if __name__ == "__main__":
    # Test the connection
    print("Testing SignalFlow Status connection...")
    
    # Update various status types
    success("Connection", "Established")
    info("Model", "Loading...")
    time.sleep(1)
    success("Model", "Loaded successfully")
    
    warning("Memory", "Usage at 80%")
    time.sleep(1)
    
    error("Database", "Connection failed")
    time.sleep(1)
    
    success("Database", "Reconnected")
    
    # Show current status
    current = status.get_all()
    if current:
        print("\nCurrent status:")
        for key, data in current.items():
            print(f"  {key}: {data['value']} ({data['type']})")
    
    print("\nTest complete!")

class NotebookAudioGraph (AudioGraph):
    def __init__(self, **kwargs):
        super().__init__(**kwargs)
        start_signalflow_listener()

start_signalflow_listener()
