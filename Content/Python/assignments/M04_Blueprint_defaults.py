import csv
import os
from enum import Enum

import course_utils

import unreal


class RogueActionField(str, Enum):
    PACKAGE_NAME = "package_name"
    ACTION_NAME = "action_name"
    GRANT_TAGS = "grant_tags"
    BLOCKED_TAGS = "blocked_tags"
    COOLDOWN_TIME = "cooldown_time"
    ACTIVATION_COST = "activation_cost"
    MUZZLE_SOCKET_NAME = "muzzle_socket_name"
    PROJECTILE_CLASS = "projectile_class"


class ProjectileField(str, Enum):
    PACKAGE_NAME = "package_name"
    DAMAGE = "damage"
    LIFE_TIME = "life_time"


def enum_values(enum_class):
    return [field.value for field in enum_class]

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

    muzzle_socket_name = ""
    projectile_class_path = ""

    if unreal.MathLibrary.class_is_child_of(CDO.get_class(), unreal.RogueAction_ProjectileAttack.static_class()):
        muzzle_socket_name = str(CDO.get_editor_property("muzzle_socket_name"))
        projectile_class = CDO.get_editor_property("projectile_class")
        projectile_class_path = unreal.AssetRegistryHelpers.get_path_name(projectile_class)
    
    return {
        RogueActionField.PACKAGE_NAME.value: package_name,
        RogueActionField.ACTION_NAME.value: action_name_value,
        RogueActionField.GRANT_TAGS.value: grant_tags_value,
        RogueActionField.BLOCKED_TAGS.value: blocked_tags_value,
        RogueActionField.COOLDOWN_TIME.value: cooldowntime,
        RogueActionField.ACTIVATION_COST.value: activation_cost_value,
        RogueActionField.MUZZLE_SOCKET_NAME.value: muzzle_socket_name,
        RogueActionField.PROJECTILE_CLASS.value: projectile_class_path
    }


def build_projectile_row(package_name, CDO) ->dict:
    damage = CDO.get_editor_property("damage")
    life_time = CDO.get_editor_property("life_time")
    
    return {
        ProjectileField.PACKAGE_NAME.value: package_name,
        ProjectileField.DAMAGE.value: damage,
        ProjectileField.LIFE_TIME.value: life_time
    }

def get_blueprintclasses_from_path(assets_path , parent_class):
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


def Export_BlueprintClass_Properties(fieldnames, filename, asset_datas, build_row):
    rows = []
    for package_name,blueprint_class in asset_datas:
        CDO = unreal.get_default_object(blueprint_class)
        row = build_row(package_name, CDO)
        rows.append(row)
        
    course_utils.write_csv(rows, filename, fieldnames)
    
def import_gameplay_tag(tag_name) -> unreal.GameplayTag:
        tag = unreal.GameplayTag()
        tag.import_text(tag_name)
        return tag

def import_gameplay_tag_container(InTags) -> unreal.GameplayTagContainer:
    tag_list = []
    for tag in InTags.split(";"):
        tag = import_gameplay_tag(tag)
        tag_list.append(tag)
    return unreal.GameplayTagContainer(tag_list)

def import_activation_cost(InActivationCost):
    Map = {}
    for tag in InActivationCost.split(";"):
        Pair = tag.split("=")
        if(Pair[0] != ""):
            Map[import_gameplay_tag( Pair[0] ) ]  = float(Pair[1])
    return Map
    
def Import_blueprintClass_Properties(filename):
    config_file = os.path.join(course_utils.reports_dir(), filename)
    with open(config_file, "r", newline="", encoding="utf-8") as csvfile:
        reader = csv.reader(csvfile)
        header = next(reader)
        
        for row in reader:
            #GET
            package_name = row[header.index(RogueActionField.PACKAGE_NAME.value)]
            action_name = row[header.index(RogueActionField.ACTION_NAME.value)]
            grant_tags = row[header.index(RogueActionField.GRANT_TAGS.value)]
            blocked_tags = row[header.index(RogueActionField.BLOCKED_TAGS.value)]
            cooldown_time = row[header.index(RogueActionField.COOLDOWN_TIME.value)]
            activation_cost = row[header.index(RogueActionField.ACTIVATION_COST.value)]
            muzzle_socket_name = row[header.index(RogueActionField.MUZZLE_SOCKET_NAME.value)]
            projectile_class = row[header.index(RogueActionField.PROJECTILE_CLASS.value)]
            
            #TRANSLATE
            action_name = import_gameplay_tag(action_name)
            grant_tags = import_gameplay_tag_container(grant_tags)
            blocked_tags = import_gameplay_tag_container(blocked_tags)
            cooldown_time = float(cooldown_time)
            activation_cost = import_activation_cost(activation_cost)
            projectile_class = unreal.EditorAssetLibrary.load_blueprint_class(projectile_class)
            
            #SET
            blueprintclass = unreal.EditorAssetLibrary.load_blueprint_class(package_name)
            CDO = unreal.get_default_object(blueprintclass)
            CDO.set_editor_property(RogueActionField.ACTION_NAME.value, action_name)
            CDO.set_editor_property(RogueActionField.GRANT_TAGS.value, grant_tags)
            CDO.set_editor_property(RogueActionField.COOLDOWN_TIME.value, cooldown_time)
            CDO.set_editor_property(RogueActionField.BLOCKED_TAGS.value, blocked_tags)
            CDO.set_editor_property(RogueActionField.ACTIVATION_COST.value, activation_cost)

            if unreal.MathLibrary.class_is_child_of(CDO.get_class(), unreal.RogueAction_ProjectileAttack.static_class()):
                CDO.set_editor_property(RogueActionField.MUZZLE_SOCKET_NAME.value, muzzle_socket_name)
                CDO.set_editor_property(RogueActionField.PROJECTILE_CLASS.value, projectile_class)
            
            #SAVE
            unreal.EditorAssetLibrary.save_asset(package_name)
            

def main():
    #Export_Properties()
    Import_blueprintClass_Properties("balance.csv")


def Export_Properties():
    asset_actions_path = "/Game/ActionRoguelike/Actions"
    RogueAction_fieldnames = enum_values(RogueActionField)
    RogueAction_data_csv_file_name = "balance.csv"
    Rogue_action_asset_datas = get_blueprintclasses_from_path(asset_actions_path, unreal.RogueAction.static_class())

    Export_BlueprintClass_Properties(RogueAction_fieldnames, RogueAction_data_csv_file_name, Rogue_action_asset_datas,
                                     build_action_row)

    asset_projectiles_path = "/Game/ActionRoguelike/Projectiles"
    Projectile_fieldnames = enum_values(ProjectileField)
    Projectile_data_csv_file_name = "projectiles.csv"
    Projectile_asset_datas = get_blueprintclasses_from_path(asset_projectiles_path,
                                                            unreal.RogueProjectileBase.static_class())

    Export_BlueprintClass_Properties(Projectile_fieldnames, Projectile_data_csv_file_name, Projectile_asset_datas,
                                     build_projectile_row)


main()