# M1 — Project banner. Logged on editor start by init_unreal.py; rerun with `assignments.m01_banner.main()`.
import json

import unreal
import course_utils

ASSETS_PATH = "/Game/ActionRoguelike"


def main():
    engine_version = unreal.SystemLibrary.get_engine_version()

    project_directory = unreal.SystemLibrary.get_project_directory()
    project_name = project_directory.split('/')[-2]

    assetslist = unreal.EditorAssetLibrary.list_assets(ASSETS_PATH)

    uprojectfile = project_directory + project_name + ".uproject"
    with open(uprojectfile) as f:
        json_data = json.load(f)
    active_plugins = json_data["Plugins"]

    unreal.log_warning(f"Engine version: {engine_version}")
    unreal.log_warning(f"Project name: {project_name}")
    course_utils.print_box("Enabled plugins =", active_plugins)
    course_utils.print_box(f"Assets in {ASSETS_PATH} = ", assetslist)