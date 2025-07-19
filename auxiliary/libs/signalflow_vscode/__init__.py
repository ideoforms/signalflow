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
        self.is_started = False

    def start(self) -> bool:
        """Activate the SignalFlow Status panel in VS Code"""
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
            try:
                import inspect
                frame = inspect.currentframe()
                while frame:
                    frame = frame.f_back
                    if frame and '__vsc_ipynb_file__' in frame.f_globals:
                        notebook_path = frame.f_globals['__vsc_ipynb_file__']
                        notebook_name = os.path.basename(notebook_path)
                        print(f"Could not connect to SignalFlow VS Code extension on port {self.port} for notebook '{notebook_name}'.")
                        print("This could be because:")
                        print("1. The VS Code extension is not installed or enabled")
                        print("2. Another VS Code instance is using the same notebook name (port conflict)")
                        print("3. The extension failed to start the server")
                        print(f"Try renaming the notebook or closing other VS Code instances with notebooks named '{notebook_name}'")
                        return False
            except:
                pass
            print("Could not connect to SignalFlow Visual Studio Code extension. Please check that it is installed and enabled.")
            return False

    def update(self, key: str, value: str, status_type: Literal["info", "warning", "error", "success"] = "info") -> bool:
        if not self.is_started:
            return False
        try:
            response = self.session.post(f"{self.base_url}/status",
                                         json={"key": key, "value": value, "type": status_type},
                                         timeout=1)
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
        try:
            response = self.session.delete(f"{self.base_url}/status", timeout=1)
            return response.status_code == 200
        except:
            return False

    def flash_cell(self, cell_id: str, flash_text: str = None) -> bool:
        """Flash a specific notebook cell in VS Code"""
        try:
            payload = {"cellId": cell_id}
            if flash_text:
                payload["flashText"] = flash_text

            response = self.session.post(
                f"{self.base_url}/flash-cell",
                json=payload,
                timeout=1
            )
            return response.status_code == 200
        except Exception as e:
            print(f"Error flashing cell: {e}")
            return False

# Create global instance
status = SignalFlowStatus()

def clear_all() -> bool:
    return status.clear()

def start() -> bool:
    """Start the SignalFlow Status panel in VS Code"""
    return status.start()

def clear_audio_graph():
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
                clear_audio_graph()
                response = json.dumps({"success": True, "message": "AudioGraph cleared"})
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

        time.sleep(0.5)

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
        self.notebook_path = notebook_path or self._get_current_notebook_path()
        self.line_to_cell_map = {}
        self._parse_notebook()

    def _get_current_notebook_path(self) -> str:
        """Get the current notebook path from VS Code variables"""
        try:
            import inspect
            frame = inspect.currentframe()
            while frame:
                frame = frame.f_back
                if frame and '__vsc_ipynb_file__' in frame.f_globals:
                    return frame.f_globals['__vsc_ipynb_file__']
        except:
            pass
        return None

    def _parse_notebook(self):
        """Parse the notebook JSON to create line-to-cell mapping"""
        if not self.notebook_path or not os.path.exists(self.notebook_path):
            return

        try:
            with open(self.notebook_path, 'r', encoding='utf-8') as f:
                notebook = json.load(f)

            current_line = 1
            for cell in notebook.get('cells', []):
                cell_id = cell.get('id', 'unknown')
                cell_type = cell.get('cell_type', 'unknown')
                source = cell.get('source', [])

                # Calculate line range for this cell
                if isinstance(source, list):
                    line_count = len(source)
                else:
                    line_count = source.count('\n') + 1 if source else 1

                # Map each line in this cell to the cell ID
                for line_offset in range(line_count):
                    self.line_to_cell_map[current_line + line_offset] = {
                        'cell_id': cell_id,
                        'cell_type': cell_type,
                        'line_in_cell': line_offset + 1
                    }

                current_line += line_count

        except Exception as e:
            print(f"Error parsing notebook: {e}")

    def get_cell_from_current_frame(self) -> dict:
        """Get cell information for the current execution frame"""
        try:
            import inspect
            # Look for the frame that's executing user code (module __main__)
            stack = inspect.stack()
            for frame_info in stack[1:]:  # Skip current function
                frame = frame_info.frame
                if '__vsc_ipynb_file__' in frame.f_globals:
                    module_name = frame.f_globals.get('__name__', '')
                    filename = frame.f_code.co_filename

                    # Check if this is user code execution in a temp file
                    if module_name == '__main__' and 'ipykernel_' in filename:
                        # Try to get source code context from the frame
                        try:
                            import linecache
                            # Get lines of context around the execution point
                            context_lines = []
                            start_line = max(1, frame_info.lineno - 5)
                            end_line = frame_info.lineno + 5

                            for line_num in range(start_line, end_line + 1):
                                line = linecache.getline(filename, line_num)
                                if line.strip():  # Only include non-empty lines
                                    context_lines.append(line.strip())

                            # If we don't have much context, try to get more from the temp file
                            if len(context_lines) < 3:
                                try:
                                    with open(filename, 'r', encoding='utf-8') as f:
                                        all_lines = f.readlines()
                                    context_lines = [line.strip() for line in all_lines if line.strip()]
                                except:
                                    pass

                            if context_lines:
                                # Create a code snippet from the context
                                context_code = '\n'.join(context_lines)
                                # Try to match this code against cells
                                return self._match_content_to_cell(context_code)

                        except Exception as e:
                            # We can't get source context
                            pass


            print("No valid cell found in current frame")
            return None
        except Exception as e:
            print(f"Error getting cell: {e}")
            return None

    def _match_content_to_cell(self, code_snippet: str) -> dict:
        """Match code snippet to a cell in the notebook using exact line matching"""
        try:
            if not self.notebook_path or not os.path.exists(self.notebook_path):
                return None

            # Re-parse notebook (it should be saved after cell execution)
            self._parse_notebook()

            # Extract individual lines from the snippet for exact matching
            snippet_lines = [line.strip() for line in code_snippet.split('\n') if line.strip()]

            with open(self.notebook_path, 'r', encoding='utf-8') as f:
                notebook = json.load(f)

            # Look for exact line matches (perfect for track() calls with unique identifiers)
            for cell in notebook.get('cells', []):
                if cell.get('cell_type') == 'code':
                    cell_source = ''.join(cell.get('source', []))
                    cell_lines = [line.strip() for line in cell_source.split('\n') if line.strip()]

                    # Check if any snippet line appears exactly in this cell
                    for snippet_line in snippet_lines:
                        if snippet_line in cell_lines:
                            # print(f"Found matching cell for snippet line '{snippet_line} ' in cell ID {cell.get('id', 'unknown')}")
                            return {
                                'cell_id': cell.get('id', 'unknown'),
                                'cell_type': cell.get('cell_type', 'code'),
                                'line_in_cell': 1
                            }

            print("No matching cell found for code snippet")
            return None
        except Exception as e:
            print("Error matching content to cell:", e)
            return None






# Test function
def test_cell_mapping():
    """Test the cell mapping functionality"""
    mapper = NotebookCellMapper()
    if mapper.notebook_path:
        print("=== Current Cell Detection Test ===")
        cell_info = mapper.get_cell_from_current_frame()
        if cell_info:
            print(f"This line is in cell: {cell_info['cell_id']}")
        else:
            print("Could not determine current cell")
    else:
        print("No notebook path found - not running in VS Code notebook?")

def flash_cell(cell_id: str, flash_text: str = None):
    """Flash a specific notebook cell in VS Code"""
    return status.flash_cell(cell_id, flash_text)

def vscode_get_this_cell_id():
    """Get the ID of the currently executing cell"""
    mapper = NotebookCellMapper()
    cell_info = mapper.get_cell_from_current_frame()
    if cell_info:
        return cell_info['cell_id']
    else:
        return None

def vscode_flash_cell_id(cell_id: str, flash_text: str = None):
    """Flash a specific cell by its ID"""
    return status.flash_cell(cell_id, flash_text)

def which_cell(flash_text: str = None):
    """Simple function to determine which cell is calling this and flash it"""
    mapper = NotebookCellMapper()
    cell_info = mapper.get_cell_from_current_frame()
    if cell_info:
        cell_id = cell_info['cell_id']
        print(f"Called from cell: {cell_id}")
        # Flash the cell with optional custom text
        flash_cell(cell_id, flash_text)
        return cell_id
    else:
        print("Could not determine calling cell")
        return None

start()
clear_all()
start_signalflow_listener()