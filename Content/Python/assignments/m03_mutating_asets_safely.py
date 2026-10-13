import csv
import os

import course_utils
from course_utils import ErrorType

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
        

def ReadReport():
    report_path = os.path.join(course_utils.reports_dir(), "naming_audit.csv")
    with open(report_path, "r", newline="", encoding="utf-8") as csvfile:
        reader = csv.reader(csvfile)
        header = next(reader)
        
        for row in reader:
            errortype = row[header.index(course_utils.Header.ErrorType.value)]

            AssetName = row[header.index(course_utils.Header.AssetName.value)]
            ExpectedPrefix = row[header.index(course_utils.Header.ExpectedPrefix.value)]
            AssetPath = row[header.index(course_utils.Header.AssetPath.value)]
            
            
            if errortype == ErrorType.MISSING_PREFIX.name:
                rename_asset(AssetName, AssetPath, ExpectedPrefix, False)

def main():
    ReadReport()
        
main()