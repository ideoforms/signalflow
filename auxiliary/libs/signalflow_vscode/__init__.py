"""
Python package for interacting with the SignalFlow Visual Studio Code extension.

Overview:
 - When enabled, the extension starts a local HTTP server on a port derived from the notebook name.
 - This package creates a corresponding server, which can be used for RPC calls in the Python code.

"""

import linecache
import threading
import requests
import inspect
import atexit
import socket
import time
import json
import os

import urllib.parse
from signalflow import AudioGraph, amplitude_to_db
from typing import Optional, Literal

signalflow_vscode_debug = False

def debug_log(message: str):
    if signalflow_vscode_debug:
        print("signalflow_vscode: %s" % message)

def debug():
    global signalflow_vscode_debug
    signalflow_vscode_debug = True
    debug_log("SignalFlow extension debugging enabled.")
    debug_log("Ports: %d (status), %d (RPC)" % (get_notebook_port(), get_notebook_port() + 1))

def get_current_notebook_path() -> Optional[str]:
    frame = inspect.currentframe()
    while frame:
        frame = frame.f_back
        if frame and '__vsc_ipynb_file__' in frame.f_globals:
            notebook_path = frame.f_globals['__vsc_ipynb_file__']
            # Handle URL decoding if the path contains %20 etc.
            decoded_path = urllib.parse.unquote(notebook_path)
            return decoded_path

            return notebook_path
    raise RuntimeError("Could not determine notebook path from VS Code variables")

def get_notebook_port() -> int:
    """
    Get the port for the current notebook based a deterministic hash function.
    Notebook path is derived from VS Code's __vsc_ipynb_file__ variable.
    """
    notebook_path = get_current_notebook_path()
    basename = os.path.basename(notebook_path)

    # Calculate hash using the same algorithm as the VS Code extension
    hash_obj = 0
    for char in basename:
        hash_obj = ((hash_obj << 5) - hash_obj) + ord(char)
        hash_obj = ((hash_obj & 0xFFFFFFFF) ^ 0x80000000) - 0x80000000

    remainder = hash_obj % 1000
    if hash_obj < 0 and remainder != 0:
        remainder = remainder - 1000
    return 8765 + abs(remainder)

class SignalFlowStatus:
    def __init__(self, host: str = "localhost"):
        self.port = get_notebook_port()
        self.base_url = f"http://{host}:{self.port}"
        self.session = requests.Session()
        self.is_started = False

    def start(self) -> bool:
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
status = SignalFlowStatus()

def clear_status() -> bool:
    return status.clear()

def start() -> bool:
    """Start the SignalFlow Status panel in VS Code"""
    return status.start()

def stop_playback():
    """Clear the AudioGraph singleton"""
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



# RPC Listener for remote clear commands
class SignalFlowRPCListener:
    def __init__(self, port: Optional[int] = None, notebook_path: str = None):
        if port is None:
            port = get_notebook_port() + 1
        self.port = port
        self.notebook_path = notebook_path or get_current_notebook_path()
        self.running = False
        self.server_thread = None
        self.tcp_server = None

    def start_listener(self):
        if self.running:
            return
        self.running = True
        self.server_thread = threading.Thread(target=self._run_tcp_server, daemon=True)
        self.server_thread.start()
        # print("SignalFlow RPC Listener started on port", self.port)
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
                stop_playback()
                response = json.dumps({"success": True, "message": "Playback stopped"})
                conn.send(response.encode('utf-8'))
            elif command.startswith('stop-tracks:'):
                # Extract cell ID and stop tracks in that cell
                cell_id = command.split(':', 1)[1]
                success, message = self._stop_tracks_in_cell(cell_id)
                if success:
                    response = json.dumps({"success": True, "message": message})
                else:
                    response = json.dumps({"success": False, "message": message})
                conn.send(response.encode('utf-8'))
            else:
                response = json.dumps({"success": False, "message": "Unknown command"})
                conn.send(response.encode('utf-8'))
        except:
            error_response = json.dumps({"success": False, "message": "Error"})
            conn.send(error_response.encode('utf-8'))
        finally:
            conn.close()

    def _stop_tracks_in_cell(self, cell_id: str) -> tuple[bool, str]:
        """Stop all tracks defined in a specific cell"""
        # Use the stored notebook path if available, otherwise try to get it
        notebook_path = self.notebook_path
        if not notebook_path:
            # Fallback to using NotebookCellMapper to get the path
            mapper = NotebookCellMapper()
            notebook_path = mapper.notebook_path
        
        if not notebook_path or not os.path.exists(notebook_path):
            return False, f"Notebook path not found or doesn't exist: {notebook_path}"

        with open(notebook_path, 'r', encoding='utf-8') as f:
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

            stopped_count = 0
            total_tracks = len(timeline.tracks)
            
            for track_name in track_names:
                # Find tracks with matching names
                for track in timeline.tracks:
                    track_attr_name = getattr(track, 'name', None)
                    if track_attr_name == track_name:
                        track.stop()
                        stopped_count += 1

            if stopped_count > 0:
                return True, f"Stopped {stopped_count} track(s) in cell {cell_id}"
            else:
                return True, f"Found track definitions ({track_names}) but no matching active tracks"
                
        except (ImportError, ModuleNotFoundError) as e:
            return False, f"isobar not available: {e}"


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
    """Start the SignalFlow RPC listener for remote commands"""
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

# Cleanup on exit
def _cleanup():
    global _rpc_listener
    if _rpc_listener:
        _rpc_listener.stop_listener()

atexit.register(_cleanup)

# Cell flashing class
class NotebookCellMapper:
    """Class to map execution context to notebook cell IDs"""

    def __init__(self, notebook_path: str = None):
        self.notebook_path = notebook_path or get_current_notebook_path()
        self.line_to_cell_map = {}

    def get_cell_from_current_frame(self) -> dict:
        """
        Get cell information for the current execution frame.
        """
        try:
            # Look for the frame that's executing user code (module __main__)
            stack = inspect.stack()
            for frame_info in stack[1:]:  # Skip current function
                frame = frame_info.frame
                if '__vsc_ipynb_file__' in frame.f_globals:
                    module_name = frame.f_globals.get('__name__', '')
                    filename = frame.f_code.co_filename

                    # Check if this is user code execution in a temp file
                    if module_name == '__main__' and 'ipykernel_' in filename:
                        # Get lines of context around the execution point
                        line = linecache.getline(filename, frame_info.lineno).strip()                       
                        return self._match_content_to_cell(line)


            print("No valid cell found in current frame")
            return None
        except Exception as e:
            print(f"Error getting cell: {e}")
            return None

    def _match_content_to_cell(self, line: str) -> dict:
        """
        Match code snippet to a cell in the notebook using exact line matching.
        """
        with open(self.notebook_path, 'r', encoding='utf-8') as f:
            notebook = json.load(f)

        # Look for exact line matches (perfect for track() calls with unique identifiers)
        for cell in notebook.get('cells', []):
            if cell.get('cell_type') == 'code':
                cell_source = ''.join(cell.get('source', []))
                cell_lines = [line.strip() for line in cell_source.split('\n') if line.strip()]

                # Check if any snippet line appears exactly in this cell
                if line in cell_lines:
                    return cell.get('id')

        print("No matching cell found for code snippet")
        return None


def flash_cell(cell_id: str, flash_text: str = None):
    """
    Flash a specific notebook cell in VS Code.
    """
    return status.flash_cell(cell_id, flash_text)

def vscode_get_this_cell_id():
    """
    Get the ID of the currently executing cell.
    """
    mapper = NotebookCellMapper()
    return mapper.get_cell_from_current_frame()

def vscode_flash_cell_id(cell_id: str, flash_text: str = None):
    """
    Flash a specific cell by its ID.
    """
    return status.flash_cell(cell_id, flash_text)

def which_cell(flash_text: str = None):
    """
    Simple function to determine which cell is calling this and flash it.
    """
    mapper = NotebookCellMapper()
    cell_id = mapper.get_cell_from_current_frame()
    print(f"Called from cell: {cell_id}")
    # Flash the cell with optional custom text
    flash_cell(cell_id, flash_text)

start()
clear_status()
start_signalflow_listener()