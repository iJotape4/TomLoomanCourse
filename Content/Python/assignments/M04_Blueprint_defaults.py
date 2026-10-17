import unreal

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

def build_action_row(CDO) -> dict:
    action_name = CDO.get_editor_property("action_name")
    action_name_value = unreal.GameplayTagLibrary.get_tag_name(action_name)
    unreal.log(action_name_value)

    grant_tags = CDO.get_editor_property("grant_tags")
    grant_tags_value = iterate_gameplay_tag_list(grant_tags.gameplay_tags)
    unreal.log(grant_tags_value)

    blocked_tags = CDO.get_editor_property("blocked_tags")
    blocked_tags_value = iterate_gameplay_tag_list(blocked_tags.gameplay_tags)
    unreal.log(blocked_tags_value)

    cdtime = CDO.get_editor_property("activation_cost")
    activation_cost_value = iterate_gameplay_tag_keymap(cdtime)

    return {
        "action_name": action_name_value,
        "grant_tags": grant_tags_value,
        "blocked_tags": blocked_tags_value,
        "activation_cost": activation_cost_value,
    }

def main():
    asset_Path = "/Game/ActionRoguelike/Actions/BP_Action_GravityProjectile"
    action = unreal.EditorAssetLibrary.load_blueprint_class(asset_Path)
    CDO = unreal.get_default_object(action)
    rows = build_action_row(CDO)
    unreal.log(rows)
main()