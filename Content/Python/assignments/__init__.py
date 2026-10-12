# Makes `assignments` a package. Keep this file light: it runs on first `import assignments`.
import importlib
import sys

import unreal


def reload_all():
    """Reload every already-imported assignments.m* module so edits apply without restarting the editor."""
    # list() snapshot: reloading can mutate sys.modules while we iterate.
    for name, module in sorted(list(sys.modules.items())):
        if name.startswith("assignments.m"):
            importlib.reload(module)
            unreal.log(f"[reload_all] Reloaded {name}")
