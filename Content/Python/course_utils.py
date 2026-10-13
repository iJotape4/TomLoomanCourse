# Shared helpers for the project's editor Python scripts.
# Content/Python is on the editor's sys.path, so any script (including ones in
# subpackages like assignments/) can simply do:
#
#     import course_utils
#     course_utils.log("Hello")
#
# Keep this module free of side effects at import time.
import importlib
import os
import sys
from enum import Enum

import unreal

TableKey = {
    "Blueprint" : "BP",
    "WidgetBlueprint" : "WBP",
    "Material" : "M",
    "MaterialInstanceConstant" : "MI",
    "MaterialFunction" : "MF",
    "SoundCue" : "SC",
    "BehaviorTree" : "BT",
    "BlackboardData" : "BB",
    "EnvQuery" : "EQS",
    "CurveFloat" : "Curve",
    "NiagaraSystem" : "NS",
    "InputAction" : "IA",
    "InputMappingContext" : "IMC"
}

class Header(Enum):
    AssetName = "AssetName"
    ClassParentClass = "Class/ParentClass"
    ErrorType = "ErrorType"
    ExpectedPrefix = "ExpectedPrefix"
    AssetPath = "AssetPath"
    References = "References"
    RedirectsTo = "RedirectsTo"

class ErrorType(Enum):
    UNDEFINED_PREFIX = 1
    INVALID_PREFIX = 2
    MISSING_PREFIX = 3
    DUPLICATE = 4
    REDIRECTOR= 5

ReportHeader = [Header.AssetName.value, Header.ClassParentClass.value, Header.ErrorType.value, Header.ExpectedPrefix.value, Header.AssetPath.value, Header.References.value, Header.RedirectsTo.value]

LOG_PREFIX = "[Python]"


# --- Logging -----------------------------------------------------------------

def log(message):
    unreal.log(f"{LOG_PREFIX} {message}")


def warn(message):
    unreal.log_warning(f"{LOG_PREFIX} {message}")


def error(message):
    unreal.log_error(f"{LOG_PREFIX} {message}")


# --- Formatting --------------------------------------------------------------

def print_box(title, rows, numbered=True, sort=True):
    """Print rows inside an ASCII box with a centered title, e.g. for listings in the Output Log."""
    rows = [str(row) for row in rows]
    if sort:
        rows = sorted(rows)

    title = "%s (%d)" % (title, len(rows))
    index_width = len(str(len(rows)))
    lines = ["%s. %s" % (str(i).rjust(index_width), row) for i, row in enumerate(rows, start=1)] if numbered else rows
    width = max([len(title)] + [len(line) for line in lines])

    print("+" + "=" * (width + 2) + "+")
    print("| " + title.center(width) + " |")
    print("+" + "=" * (width + 2) + "+")

    for line in lines:
        print("| " + line.ljust(width) + " |")

    print("+" + "-" * (width + 2) + "+")


# --- Paths -------------------------------------------------------------------

def project_dir():
    """Absolute path to the project root (folder containing the .uproject)."""
    return os.path.abspath(unreal.Paths.project_dir())


def project_name():
    return os.path.basename(os.path.normpath(project_dir()))


def saved_dir(*parts):
    return os.path.join(os.path.abspath(unreal.Paths.project_saved_dir()), *parts)


def reports_dir():
    """Saved/Reports, created on demand. Git-ignored along with Saved/."""
    path = saved_dir("Reports")
    os.makedirs(path, exist_ok=True)
    return path

def GetReferencers(asset_path):
    options = unreal.AssetRegistryDependencyOptions(
        include_hard_package_references=True,
        include_soft_package_references=True)

    return unreal.AssetRegistryHelpers.get_asset_registry().get_referencers(asset_path, options) or []

# --- Assets ------------------------------------------------------------------

def selected_assets():
    return unreal.EditorUtilityLibrary.get_selected_assets()


def list_assets(path="/Game", recursive=True):
    """Asset object paths under a content folder."""
    return unreal.EditorAssetLibrary.list_assets(path, recursive=recursive, include_folder=False)


# --- Development -------------------------------------------------------------

def reload_modules(prefix):
    """Reload every already-imported module whose name starts with prefix. Returns the reloaded names."""
    names = sorted(name for name in sys.modules if name == prefix or name.startswith(prefix + "."))
    for name in names:
        importlib.reload(sys.modules[name])
    return names
