import csv
import os
from collections import Counter

import course_utils
from course_utils import ErrorType
import unreal

PackagePaths = ["/Game/ActionRoguelike", "/Game/Core"]
AssetsDict = {}
ReportedDuplicates = set()

ReportRows = []

def GetClassInfo(asset_path):
    asset_datas = unreal.AssetRegistryHelpers.get_asset_registry().get_assets_by_package_name(asset_path)
    if not asset_datas:
        return "", ""

    asset_data = asset_datas[0]
    asset_class = str(asset_data.asset_class_path.asset_name)

    # Only Blueprint-based assets have this tag, e.g. "/Script/CoreUObject.Class'/Script/Engine.Actor'"
    parent_class = str(asset_data.get_tag_value("ParentClass") or "")
    if "'" in parent_class:
        parent_class = parent_class.split("'")[1]
    parent_class = parent_class.split(".")[-1]

    return asset_class, parent_class

def AddReportRow(asset_name, error_type, expected_prefix, asset_path, references="", redirects_to=""):
    asset_class, parent_class = GetClassInfo(asset_path)
    class_info = f"{asset_class}/{parent_class}" if parent_class else asset_class
    ReportRows.append([str(asset_name), class_info, error_type.name, expected_prefix, str(asset_path), references, redirects_to])

def WriteReport():
    report_path = os.path.join(course_utils.reports_dir(), "naming_audit.csv")
    with open(report_path, "w", newline="", encoding="utf-8") as csvfile:
        writer = csv.writer(csvfile)
        writer.writerow(course_utils.ReportHeader)
        writer.writerows(ReportRows)

def PrintSummary():
    error_column = course_utils.ReportHeader.index("ErrorType")
    counts = Counter(row[error_column] for row in ReportRows)
    name_width = max(len(error_type.name) for error_type in ErrorType)

    rows = [f"{error_type.name.ljust(name_width)}  {counts[error_type.name]}" for error_type in ErrorType]
    rows.append(f"{'TOTAL'.ljust(name_width)}  {len(ReportRows)}")
    course_utils.print_box("Naming audit summary", rows, numbered=False, sort=False)

def GetReferencers(asset_path):
    options = unreal.AssetRegistryDependencyOptions(
        include_hard_package_references=True,
        include_soft_package_references=True)

    return unreal.AssetRegistryHelpers.get_asset_registry().get_referencers(asset_path, options) or []

def GetRedirectorTarget(redirector_path):
    # A redirector package's only hard dependency is the asset it points to
    options = unreal.AssetRegistryDependencyOptions(
        include_hard_package_references=True,
        include_soft_package_references=False)
    targets = unreal.AssetRegistryHelpers.get_asset_registry().get_dependencies(redirector_path, options) or []
    return "; ".join(str(target) for target in targets)

def IsAssetDeadWeight(asset_path):
    refs = GetReferencers(asset_path)
    return len(refs) == 0

def CheckDuplicate(asset_name, incoming_asset_path, expected_prefix):
    # Report both copies; the set stops the stored copy being re-reported on a 3rd+ duplicate
    for path in (AssetsDict.get(asset_name), incoming_asset_path):
        if path not in ReportedDuplicates:
            ReportedDuplicates.add(path)
            references = "; ".join(str(ref) for ref in GetReferencers(path))
            AddReportRow(asset_name, ErrorType.DUPLICATE, expected_prefix, path, references)

    bIsStoredAssetDeadWeight = IsAssetDeadWeight(AssetsDict.get(asset_name))
    bIsIncomingAssetDeadWeight =  IsAssetDeadWeight(incoming_asset_path)
    
    if bIsStoredAssetDeadWeight and not bIsIncomingAssetDeadWeight:
            AssetsDict[asset_name] = incoming_asset_path

def main():
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
            
            if str(asset_type) == "ObjectRedirector":
                references = "; ".join(str(ref) for ref in GetReferencers(asset_path))
                AddReportRow(asset_name, ErrorType.REDIRECTOR, "N/A", asset_path, references, GetRedirectorTarget(asset_path))
                continue
            
            # Empty when the type has no entry in TableKey
            expected_Prefix = course_utils.TableKey.get(str(asset_type), "")

            if(asset_name not in AssetsDict):
                AssetsDict[asset_name] = asset_path
            else:
                CheckDuplicate(asset_name, asset_path, expected_Prefix)

            if str(asset_type) not in course_utils.TableKey:
                AddReportRow(asset_name, ErrorType.UNDEFINED_PREFIX, expected_Prefix, asset_path)
                continue

            if(str(asset_name).count("_")<=0):
                AddReportRow(asset_name, ErrorType.MISSING_PREFIX, expected_Prefix, asset_path)
                continue
                
            asset_Prefix = str(asset_name).split("_")[0]
            
            if(asset_Prefix != expected_Prefix):
                AddReportRow(asset_name, ErrorType.INVALID_PREFIX, expected_Prefix, asset_path)
                continue

    WriteReport()
    PrintSummary()

main()