# Working implementation of detecting VS Code dark mode from settings files

from pathlib import Path
from typing import Optional
import warnings
import matplotlib.pyplot as plt
import json
import re
import os

def init_plotting():
    # Hide numerical warnings and permit large datasets
    warnings.filterwarnings('ignore')

    try:
        # Altair:
        #  - hide IPC warnings from large datasets
        #  - enable dark mode if VS Code is in dark mode
        import altair as alt
        alt.data_transformers.disable_max_rows()

        if "VSCODE_PID" in os.environ:
            dark_mode = vscode_query_dark_mode()
            theme = "dark" if dark_mode else "light"

            @alt.theme.register("transparent", enable=True)
            def _() -> alt.theme.ThemeConfig:
                return {
                    "config": {
                        "view": {"continuousWidth": 300, "continuousHeight": 300 },
                        "background": "transparent",
                        "axis": {
                            "gridOpacity": 0.2,
                            "gridColor": '#888888',
                            "gridDash": [1, 2],
                        }
                    },
                    "usermeta" : {
                        "embedOptions" : {
                            "theme": theme
                        }
                    }
                }

            # In VS Code, make the background of the output cell transparent, to avoid a white
            # box around the plot in dark mode.
            from IPython.display import display, HTML
            display(HTML("""
            <style>
                .cell-output-ipywidget-background { background-color: transparent !important; }
            </style>
            """))
    except ImportError:
        pass

    # Matplotlib:
    #  - set transparent backgrounds for figures and axes
    #  - set higher DPI for better clarity
    plt.ioff()
    plt.rcParams['figure.facecolor'] = 'none'
    plt.rcParams['axes.facecolor'] = '#00000055'
    plt.rcParams['savefig.transparent'] = True
    plt.rcParams['figure.dpi'] = 200
    plt.rcParams['grid.linestyle'] = ':'
    plt.rcParams['grid.alpha'] = 0.3
        

def vscode_query_dark_mode() -> Optional[bool]:
    """
    Check VS Code settings when running in VS Code.
    Based on code from https://github.com/OpenMined/jupyter-dark-detect, but:
      - defaults to dark mode if no setting is found
      - falls back to re-based parsing as VS Code settings files can be malformed JSON
        with trailing commas
    """
    if "VSCODE_PID" not in os.environ:
        return None

    # Check VS Code settings files
    vscode_settings_paths = [
        Path.cwd() / ".vscode" / "settings.json",
        Path.home() / ".config" / "Code" / "User" / "settings.json",
        Path.home() / "Library" / "Application Support" / "Code" / "User" / "settings.json",  # macOS
        Path.home() / "AppData" / "Roaming" / "Code" / "User" / "settings.json",  # Windows
    ]

    for settings_path in vscode_settings_paths:
        if settings_path.exists():
            with open(settings_path, "r") as f:
                try:
                    settings = json.load(f)
                    theme = settings.get("workbench.colorTheme", "").lower()
                except:
                    pattern = re.compile(r'"workbench\.colorTheme"\s*:\s*"([^"]+)"')
                    content = settings_path.read_text()
                    match = pattern.search(content)
                    
                    theme = None
                    if match:
                        theme = match.group(1).lower()

                if theme:                    
                    if "dark" in theme:
                        return True
                    elif "light" in theme:
                        return False
    
    return True