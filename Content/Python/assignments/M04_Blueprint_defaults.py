import course_utils

import unreal

asset_actions_path = "/Game/ActionRoguelike/Actions"
parent_class = unreal.RogueAction.static_class()

def iterate_gameplay_tag_keymap(keymap):
    tags_list = []
    for i in keymap:
        tag = unreal.GameplayTagLibrary.get_tag_name(i)
        tags_list.append(f"{tag}={keymap[i]}")
    return ";".join(tags_list)

def iterate_gameplay_tag_list(tags):
    tags_list = []
    for i in tags:
        tag = unreal.GameplayTagLibrary.get_tag_name(i)
        tags_list.append(str(tag))
    return ";".join(tags_list)

def build_action_row(package_name,CDO) -> dict:
    action_name = CDO.get_editor_property("action_name")
    action_name_value = str(unreal.GameplayTagLibrary.get_tag_name(action_name))
    #unreal.log(action_name_value)

    grant_tags = CDO.get_editor_property("grant_tags")
    grant_tags_value = iterate_gameplay_tag_list(grant_tags.gameplay_tags)
    #unreal.log(grant_tags_value)

    blocked_tags = CDO.get_editor_property("blocked_tags")
    blocked_tags_value = iterate_gameplay_tag_list(blocked_tags.gameplay_tags)
    #unreal.log(blocked_tags_value)
    
    cooldowntime = CDO.get_editor_property("cooldown_time")

    activation_cost_data = CDO.get_editor_property("activation_cost")
    activation_cost_value = iterate_gameplay_tag_keymap(activation_cost_data)

    return {
        "package_name": package_name,
        "action_name": action_name_value,
        "grant_tags": grant_tags_value,
        "blocked_tags": blocked_tags_value,
        "cooldown_time": cooldowntime,
        "activation_cost": activation_cost_value,
    }

def get_blueprintclasses_from_path(assets_path):
    ar_filter = unreal.ARFilter(
        package_paths=[assets_path],
        class_names=["Blueprint"],
        recursive_paths=True
    )
    assets_in_path = unreal.AssetRegistryHelpers.get_asset_registry().get_assets(ar_filter)
    
    asset_datas = []
    for i in assets_in_path:
        package_name = str(i.package_name)
        blueprintclass = unreal.EditorAssetLibrary.load_blueprint_class(package_name)
        if unreal.MathLibrary.class_is_child_of(blueprintclass, parent_class):
            asset_datas.append((package_name,blueprintclass))
            
    return asset_datas

def main():
    action_data_csv_file_name = "balance.csv"
    fieldnames = ["package_name", "action_name", "grant_tags", "blocked_tags", "cooldown_time", "activation_cost"]
    
    asset_datas = get_blueprintclasses_from_path(asset_actions_path)
    rows = []
    for package_name,blueprint_class in asset_datas:
        CDO = unreal.get_default_object(blueprint_class)
        row = build_action_row(package_name, CDO)
        rows.append(row)
        print(row)
        
    course_utils.write_csv(rows, action_data_csv_file_name, fieldnames)
    
main()