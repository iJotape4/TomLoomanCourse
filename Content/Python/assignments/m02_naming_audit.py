import unreal

TableKey = {
    "Blueprint" : "BP",
    "WidgetBlueprint" : "WBP_",
    "Material" : "M_", 
    "MaterialInstanceConstant" : "MI_", 
    "MaterialFunction" : "MF_",
    "SoundCue" : "SC_",
    "BehaviorTree" : "BT_", 
    "BlackboardData" : "BB_",
    "EnvQuery" : "EQS_",
    "CurveFloat" : "Curve_",
    "NiagaraSystem" : "NS_",
    "InputAction" : "IA_",
    "InputMappingContext" : "IMC_"
}

PackagePath = "/Game/ActionRoguelike"

def main():
    
    unreal.log("Starting naming audit...")
    asset_reg = unreal.AssetRegistryHelpers.get_asset_registry()
    
    ar_filter = unreal.ARFilter(
        package_paths=[PackagePath],
        #class_names=["Blueprint"],
        recursive_paths=True
    )
    assets = asset_reg.get_assets(ar_filter)
    for i in range(len(assets)):
        expected_Prefix = TableKey.get(str(assets[i].asset_class_path.asset_name))
        if(expected_Prefix is None):
            unreal.log_warning("No Prefix found for: " + str(assets[i].package_name))
            continue
            
        asset_name = assets[i].asset_name
        asset_Prefix = str(asset_name).split("_")[0]
        
        if(asset_Prefix != expected_Prefix):
            unreal.log_error(f"Incorrect name Prefix found in: {str(assets[i].package_name)}"
                               f"  Expected: {expected_Prefix}")
            
main()