import linecache
import inspect
import json
import os

import urllib.parse
from typing import Optional


class NotebookMapper:
    """
    Class to query the current Jupyter notebook path and map code execution lines to notebook cells.
    """

    def __init__(self):
        self.notebook_path = self.get_current_notebook_path()
        self.line_to_cell_map = {}
    
    def get_current_notebook_path(self) -> Optional[str]:
        frame = inspect.currentframe()
        while frame:
            frame = frame.f_back
            if frame and '__vsc_ipynb_file__' in frame.f_globals:
                notebook_path = frame.f_globals['__vsc_ipynb_file__']
                # Handle URL decoding if the path contains %20 etc.
                decoded_path = urllib.parse.unquote(notebook_path)
                return decoded_path

        raise RuntimeError("Could not determine notebook path from VS Code variables")

    def get_notebook_port(self) -> int:
        """
        Get the port for the current notebook based a deterministic hash function.
        Notebook path is derived from VS Code's __vsc_ipynb_file__ variable.
        """
        notebook_path = self.notebook_path
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

    def get_cell_from_current_frame(self) -> str:
        """
        Get cell ID for the current execution frame.
        """
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

    def _match_content_to_cell(self, line: str) -> str:
        """
        Return the ID of the notebook cell that contains the given line of code.
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
