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

PackagePaths = ["/Game/ActionRoguelike", "/Game/Core"]
AssetsDict = {}

def IsAssetDeadWeight(asset_path):
    options = unreal.AssetRegistryDependencyOptions(
        include_hard_package_references=True, 
        include_soft_package_references=True)
    
    refs = unreal.AssetRegistryHelpers.get_asset_registry().get_referencers(asset_path, options)
    bIsDeadWeight = len(refs) == 0
    if(bIsDeadWeight):
        unreal.log_error("Dead weight asset found: " + str(asset_path))
    else:
        unreal.log("Asset " + str(asset_path) + " is used by " + str(refs))
    
    return bIsDeadWeight

def CheckDuplicate(asset_name, incoming_asset_path):
    unreal.log_error("Duplicate asset found: " + str(asset_name))
    
    bIsStoredAssetDeadWeight = IsAssetDeadWeight(AssetsDict.get(asset_name))
    bIsIncomingAssetDeadWeight =  IsAssetDeadWeight(incoming_asset_path)
    
    if(not bIsStoredAssetDeadWeight and not bIsIncomingAssetDeadWeight):
        unreal.log_error("Both asset copies are being used, check assets usage for \n" + str(AssetsDict.get(asset_name)) + "\n " + str(incoming_asset_path) + "\n")
    
    if bIsStoredAssetDeadWeight and not bIsIncomingAssetDeadWeight:
            AssetsDict[asset_name] = incoming_asset_path

def main():
    
    unreal.log("Starting naming audit...")
    for PackagePath in PackagePaths:
        asset_reg = unreal.AssetRegistryHelpers.get_asset_registry()
        
        ar_filter = unreal.ARFilter(
            package_paths=[PackagePath],
            #class_names=["Blueprint"],
            recursive_paths=True
        )
        assets = asset_reg.get_assets(ar_filter)
        for i in range(len(assets)):
            asset_name = assets[i].asset_name
            asset_path = assets[i].package_name
            asset_type = assets[i].asset_class_path.asset_name
            
            if(asset_name not in AssetsDict):
                AssetsDict[asset_name] = asset_path
            else:
                CheckDuplicate(asset_name, asset_path)
       
            if str(asset_type) not in TableKey:
                unreal.log_warning("Asset type  " +str(asset_type) + " has undefined prefix "+str(asset_path))
                continue
    
            expected_Prefix = TableKey.get(str(asset_type))
                
            if(str(asset_name).count("_")<=0):
                unreal.log_warning(f"Asset {str(asset_path)} has no Prefix")
                continue
                
            asset_Prefix = str(asset_name).split("_")[0]
            
            if(asset_Prefix != expected_Prefix):
                unreal.log_warning(f"Incorrect name Prefix found in: {str(asset_path)}"
                                   f"  Expected: {expected_Prefix}")
                
main()