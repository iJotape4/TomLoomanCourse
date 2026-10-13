import csv
import os

import course_utils
from course_utils import ErrorType, GetReferencers

import unreal


def rename_asset(assetName, assetPath, ExpectedPrefix, dry_run=True):
    
    if ExpectedPrefix + "_" in assetName:
        return
    
    #Some assets may have a suffix, we want to rid-off it.
    ExpectedName = ExpectedPrefix + "_" + assetName.removesuffix(ExpectedPrefix)
    assetFinalName =  assetPath.rsplit("/",1)[0] + "/" + ExpectedName
    
    if dry_run:
        print("would rename", assetPath, "to",assetFinalName)
    else:
        if unreal.EditorAssetLibrary.rename_asset(assetPath, assetFinalName):
            course_utils.log(f"Renamed {assetPath} -> {assetFinalName}")
        else:
            course_utils.error(f"FAILED to rename {assetPath} -> {assetFinalName}")
        
def delete_duplicated_asset(asset):
    if unreal.EditorAssetLibrary.delete_asset(asset):
        unreal.log_warning(" deleted " +asset)
    else:
        unreal.log_error("failed to delete " + asset)

def process_duplicated_assets(duplicates, dry_run = True):
    print(duplicates)
    Useful_assets = []
    for name,paths in duplicates.items():
        for path in paths:
            references = len(GetReferencers(path))
            if references <=0:
                if(dry_run):
                    print("it would delete asset in"  , path)
                else:
                    delete_duplicated_asset(path)
            if references >=1:
                Useful_assets.append(paths)
                
    if len(Useful_assets)> 1:
        #unable to determine which asset is the master one , in order to consolidate them.
        unreal.log_warning("Some duplicated assets with references still exist. Check manually")
        for path in Useful_assets:
            unreal.log_warning(path)

def ReadReport():
    report_path = os.path.join(course_utils.reports_dir(), "naming_audit.csv")
    with open(report_path, "r", newline="", encoding="utf-8") as csvfile:
        reader = csv.reader(csvfile)
        header = next(reader)
        
        duplicates = {}
        
        for row in reader:
            errortype = row[header.index(course_utils.Header.ErrorType.value)]

            AssetName = row[header.index(course_utils.Header.AssetName.value)]
            ExpectedPrefix = row[header.index(course_utils.Header.ExpectedPrefix.value)]
            AssetPath = row[header.index(course_utils.Header.AssetPath.value)]
            
            if errortype == ErrorType.MISSING_PREFIX.name or errortype == ErrorType.INVALID_PREFIX.name:
                rename_asset(AssetName, AssetPath, ExpectedPrefix, True)
                
            if errortype == ErrorType.DUPLICATE.name:
                duplicates.setdefault(AssetName,[]).append(AssetPath)
                
        process_duplicated_assets(duplicates , False)
def main():
    ReadReport()
        
main()