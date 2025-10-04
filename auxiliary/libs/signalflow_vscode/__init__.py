"""
Python package for interacting with the SignalFlow Visual Studio Code extension.

Overview:
 - When enabled, the extension starts a local HTTP server on a port derived from the notebook name.
 - This package creates a corresponding server, which can be used for RPC calls in the Python code.

"""

from .notebook import NotebookMapper

import threading
import requests
import atexit
import socket
import time
import json

from signalflow import AudioGraph, amplitude_to_db
from typing import Literal


signalflow_vscode_debug = False
notebook_mapper = NotebookMapper()

def debug_log(message: str):
    if signalflow_vscode_debug:
        print("signalflow_vscode: %s" % message)

def debug():
    global signalflow_vscode_debug
    signalflow_vscode_debug = True
    debug_log("SignalFlow extension debugging enabled.")
    debug_log("Ports: %d (status), %d (RPC)" % (notebook_mapper.get_notebook_port(), notebook_mapper.get_notebook_port() + 1))



class SignalFlowStatusClient:
    def __init__(self, host: str = "localhost"):
        self.port = notebook_mapper.get_notebook_port()
        self.base_url = f"http://{host}:{self.port}"
        self.session = requests.Session()
        self.is_started = False

    def display(self) -> bool:
        """
        Begin displaying the SignalFlow Status panel in VS Code.
        """
        try:
            response = self.session.post(f"{self.base_url}/start", timeout=1)
            if response.status_code == 200:
                self.is_started = True
                return True
            else:
                print("Invalid response from SignalFlow Visual Studio Code extension (response code: %d)" % response.status_code)
                return False
            
        except requests.exceptions.ConnectionError:
            # Try to provide more helpful error message
            print(f"Could not connect to SignalFlow VS Code extension on port {self.port}.")
            print("This could be because:")
            print("1. The VS Code extension is not installed or enabled")
            print("2. Another VS Code instance is using the same notebook name (port conflict)")
            print("3. The extension failed to start the server")
            return False

    def update(self,
               key: str,
               value: str,
               status_type: Literal["info", "warning", "error", "success"] = "info") -> bool:
        """
        Display a status update in the SignalFlow Status panel in VS Code.

        Args:
            key (str): The key for the status update.
            value (str): The value for the status update.
            status_type (Literal["info", "warning", "error", "success"], optional): The type of status update. Defaults to "info".
        """
        if not self.is_started:
            return False
        try:
            payload = {"key": key, "value": value, "type": status_type}
            response = self.session.post(f"{self.base_url}/status", json=payload, timeout=1)
            return response.status_code == 200
        except Exception:
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
        if not self.is_started:
            return False
        response = self.session.delete(f"{self.base_url}/status", timeout=1)
        return response.status_code == 200

    def flash_cell(self, cell_id: str, flash_text: str = None) -> bool:
        """
        Flash a specific notebook cell in VS Code.

        Args:
            cell_id (str): The Jupyter identifier of the cell to flash.
        """
        payload = {"cellId": cell_id, "flashText": flash_text}
        response = self.session.post(f"{self.base_url}/flash-cell", json=payload, timeout=1)
        return response.status_code == 200

# Create global instance
status = SignalFlowStatusClient()


def start():
    """
    Display and initialise the SignalFlow Status panel in VS Code.
    """
    status.display()
    status.clear()
    start_signalflow_listener()



# RPC Listener for remote clear commands
class SignalFlowRPCListener:
    def __init__(self):
        notebook_mapper = NotebookMapper()

        self.port = notebook_mapper.get_notebook_port() + 1
        self.notebook_path = notebook_mapper.get_current_notebook_path()
        self.running = False
        self.server_thread = None
        self.tcp_server = None

    def start_listener(self):
        if self.running:
            return
        self.running = True
        self.server_thread = threading.Thread(target=self._run_tcp_server, daemon=True)
        self.server_thread.start()
        debug_log("SignalFlow RPC Listener started on port %d" % self.port)
        status.info("RPC Listener", f"Started on port {self.port}")

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
                    if signalflow_vscode_debug:
                        print("signalflow_vscode: Accepted connection from %s" % str(addr))
                    threading.Thread(target=self._handle_connection, args=(conn,), daemon=True).start()
                except OSError:
                    break
        except Exception as e:
            print("signalflow_vscode: Could not start RPC listener on port %d (%s)" % (self.port, e))

    def _handle_connection(self, conn):
        try:
            data = conn.recv(1024).decode('utf-8')
            command = data.strip()
            if signalflow_vscode_debug:
                print("signalflow_vscode: Received command: %s" % data)

            if command == 'stop-playback':
                self.stop_playback()
                response = json.dumps({"success": True, "message": "Playback stopped"})
                conn.send(response.encode('utf-8'))
            elif command.startswith('stop-tracks:'):
                # Extract cell ID and stop tracks in that cell
                cell_id = command.split(':', 1)[1]
                success, message = self.stop_tracks_in_cell(cell_id)
                response = json.dumps({"success": success, "message": message})
                conn.send(response.encode('utf-8'))
            else:
                response = json.dumps({"success": False, "message": "Unknown command"})
                conn.send(response.encode('utf-8'))
        except:
            error_response = json.dumps({"success": False, "message": "Error"})
            conn.send(error_response.encode('utf-8'))
        finally:
            conn.close()

    def stop_playback(self):
        """
        Clear the AudioGraph and Timeline objects.
        """
        try:
            from signalflow import AudioGraph
            graph = AudioGraph.get_shared_graph()
            if graph is not None:
                graph.clear()
                status.info("AudioGraph", "Cleared")
            else:
                status.info("AudioGraph", "No active graph to clear")
        except (ImportError, ModuleNotFoundError):
            # If SignalFlow is not available, there is no AudioGraph to clear
            pass

        try:
            from isobar import Timeline
            timeline = Timeline.get_shared_timeline()
            if timeline is not None:
                timeline.clear()
                status.info("Timeline", "Cleared")
        except (ImportError, ModuleNotFoundError):
            # If isobar is not available, there is no timeline to clear
            pass

    def stop_tracks_in_cell(self, cell_id: str) -> tuple[bool, str]:
        """
        Stop all isobar tracks defined in a specific cell.
        """

        debug_log(f"Stopping tracks in cell {cell_id}")

        # Use the stored notebook path if available, otherwise try to get it
        with open(self.notebook_path, 'r', encoding='utf-8') as f:
            notebook = json.load(f)

        # Find the cell with the given ID
        target_cell = None
        for cell in notebook.get('cells', []):
            if cell.get('cell_type') == 'code' and cell.get('id') == cell_id:
                target_cell = cell
                break

        if not target_cell:
            return False, f"Cell with ID {cell_id} not found in notebook"

        # Parse track definitions from the cell
        cell_source = ''.join(target_cell.get('source', []))
        track_names = self._extract_track_names(cell_source)

        if not track_names:
            return True, f"No track definitions found in cell {cell_id}"

        # Stop the tracks using isobar
        try:
            from isobar import Timeline
            timeline = Timeline.get_shared_timeline()
            if not timeline:
                return True, "No active timeline to stop tracks in"
            
            for track_name in track_names:
                # Find tracks with matching names
                for track in timeline.tracks:
                    track_attr_name = getattr(track, 'name', None)
                    if track_attr_name == track_name:
                        track.stop()

            return True, f"Stopped tracks in cell {cell_id}"
                
        except (ImportError, ModuleNotFoundError) as e:
            return True, f"isobar not available: {e}"


    def _extract_track_names(self, cell_source: str) -> list:
        """Extract track names from cell source code"""
        import re
        track_names = []
        
        # Look for track("name", ...) patterns
        pattern = r'track\s*\(\s*["\']([^"\']+)["\']'
        matches = re.findall(pattern, cell_source)
        track_names.extend(matches)
        
        # Also look for track(name=...) patterns
        pattern2 = r'track\s*\([^)]*name\s*=\s*["\']([^"\']+)["\']'
        matches2 = re.findall(pattern2, cell_source)
        track_names.extend(matches2)
        
        return track_names

# Global RPC listener
_rpc_listener = None

def start_signalflow_listener():
    """
    Start the SignalFlow RPC listener for remote commands.
    """
    global _rpc_listener
    if _rpc_listener is None:
        _rpc_listener = SignalFlowRPCListener()
    _rpc_listener.start_listener()

    # Start a background thread to display status updates
    thread = threading.Thread(target=run_signalflow_status_display_thread, daemon=True)
    thread.start()

def stop_signalflow_listener():
    global _rpc_listener
    if _rpc_listener:
        _rpc_listener.stop_listener()

atexit.register(stop_signalflow_listener)


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

        # If present, also display isobar timeline status.
        try:
            time.sleep(0.1)
            from isobar import Timeline, Globals
            timeline = Timeline.get_shared_timeline()

            if timeline:
                status.success("Timeline", "Running")
                status.info("Beat", "%.1f" % timeline.current_time)
                status.info("Tracks", str(len(timeline.tracks)))
                status.info("Tempo", f"{timeline.tempo:.1f}")
                status.info("Events/sec", f"{timeline.events_per_second:.1f}")
                for key, value in Globals.dict.items():
                    status.info("Globals → %s" % key, str(value))

        except (ImportError, ModuleNotFoundError):
            pass

        time.sleep(0.25)


# Externally-exposed API functions, called with
# cell_id = signalflow_vscode.get_this_cell_id() 
# signalflow_vscode.flash_cell_id(cell_id, "text to flash")

def flash_cell_id(cell_id: str, flash_text: str = None):
    """
    Flash a specific notebook cell in VS Code.
    """
    return status.flash_cell(cell_id, flash_text)

def get_this_cell_id():
    """
    Get the ID of the currently executing cell.
    """
    mapper = NotebookMapper()
    return mapper.get_cell_from_current_frame()


start()
