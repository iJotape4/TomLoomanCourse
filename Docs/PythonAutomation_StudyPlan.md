# Python Automation for Unreal Engine 5.6 — Study Plan

> Audience: fluent Python developer, comfortable with UE C++ (this project).
> Goal: go from "I can run a script in the Output Log" to "my project has an editor toolbelt, validators, and a headless CI report", all built on **this** project's assets and systems.
> Reference architecture: [`ARCHITECTURE.md`](../ARCHITECTURE.md). Solutions go in [`Content/Python/assignments/`](../Content/Python/assignments/README.md).

## How to use this plan

- Modules are ordered from easier to harder. Each one only relies on the modules before it.
- Every module has two parts:
  - **Concepts / Key APIs / Reading**: general, valid for any UE project.
  - **Assignment**: applies the concepts to TomLoomanCourse, with a *Done when* checklist and an optional *Stretch* goal.
- Work on a branch and commit after each assignment (`[Add] Python M02 - naming auditor`). Assignments M3+ change assets, so git is your undo button.
- Keep the API stub open: `Intermediate/PythonStub/unreal.py`. `bDeveloperMode=True` already generates it. Grep it before reaching for the docs. Every C++ class in this project is in there (`class RogueActionSystemComponent`, `class RogueAction`…).

**Suggested pace:** about 1 module per week for Tiers 1–3. Tier 4 modules take about 2 weeks each. That is roughly 12–14 weeks in total.

| Tier | Modules | Theme |
|---|---|---|
| 1 — Foundations | M1, M2 | Run code, read the project |
| 2 — Editing content & levels | M3, M4, M5 | Change assets, Blueprint defaults, levels |
| 3 — Building tools | M6, M7, M8 | Menus, Editor Utility Widgets, C++ ⇄ Python |
| 4 — Pipelines & automation | M9, M10 | PIE inspection, validation, headless CI |

---

## Tier 1 — Foundations

### M1. Environment & the `unreal` module

**Concepts**
- How the Python plugin embeds CPython. Everything reflected (UCLASS/USTRUCT/UENUM/UFUNCTION/UPROPERTY) is exposed in `unreal` with snake_case names.
- Three ways to run code: the Output Log in `Python` mode, the `py <file or expression>` console command, and **File → Execute Python Script**.
- Startup: any `init_unreal.py` on the Python path runs at editor start. The `StartupScripts` setting adds more scripts.
- Path: `Content/Python` (project), `Plugins/*/Content/Python`, plus `Additional Paths` from settings.
- Stub file: `Intermediate/PythonStub/unreal.py`, used for IDE autocomplete and type hints.
- Remote execution: the editor listens on UDP multicast, and IDE plugins or `remote_execution.py` send commands to it.
- Logging: `unreal.log`, `unreal.log_warning`, and `unreal.log_error` go to the Output Log with category `LogPython`.
- Reloading: `importlib.reload` and module caching gotchas (`init_unreal.py` runs only once).

**Key APIs**
- `unreal.SystemLibrary.get_engine_version()`
- `unreal.SystemLibrary.get_project_directory()`
- `unreal.Paths`
- `unreal.EditorAssetLibrary.list_assets`
- `unreal.PluginBlueprintLibrary.get_enabled_plugin_names()`

**Reading**
- Epic docs: *Scripting the Unreal Editor Using Python*.
- Epic docs: *Python API reference (5.6)*.
- `Engine/Plugins/Experimental/PythonScriptPlugin/Content/Python/remote_execution.py`

**Assignment — "Project Banner & Dev Loop"**

Set up a comfortable dev loop before writing real tools.
1. Extend `Content/Python/init_unreal.py` to log a banner with:
   - the engine version
   - the project name
   - the enabled project plugins (filtered to the four in `TomLoomanCourse.uproject`)
   - the asset count per subfolder of `/Game/ActionRoguelike`
2. Configure VS Code or PyCharm to use the stub (`python.analysis.extraPaths`). Confirm that `unreal.RogueActionSystemComponent` autocompletes.
3. Run a script **from your IDE** through remote execution. `bRemoteExecution=True` is already set. Use the VS Code "Unreal Engine Python" extension, or write a tiny client around `remote_execution.py`.
4. Create `assignments/__init__.py` with a `reload_all()` helper that reloads every `assignments.m*` module. You will use it constantly.

*Done when:* the banner prints on editor start, autocomplete works, and editing a module then calling `reload_all()` picks up the change without restarting the editor.

*Stretch:* move the existing `list_tool_menus.py` from `StartupScripts` to an on-demand function. It runs on every startup today. Is that what you want?

---

### M2. Querying content: Asset Registry & EditorAssetLibrary (read-only)

**Concepts**
- The Asset Registry indexes every asset **without loading it**. Querying it is cheap, while loading assets is expensive.
- `AssetData` gives you the package name, asset name, class path, and registry tags.
- Filters: `ARFilter(package_paths=…, class_paths=…, recursive_paths=True, recursive_classes=True)`.
- Blueprints: the asset class is `Blueprint`. The *generated* class and parent class live in the registry tags (`GeneratedClass`, `ParentClass`).
- Dependencies vs. referencers (`get_dependencies` / `get_referencers` with `AssetRegistryDependencyOptions`).
- `EditorAssetLibrary` is the convenience layer (`list_assets`, `does_asset_exist`, `find_asset_data`).

**Key APIs**
- `unreal.AssetRegistryHelpers.get_asset_registry()`
- `unreal.ARFilter`
- `unreal.AssetData`, `asset_data.get_tag_value(...)`
- `unreal.TopLevelAssetPath`
- `unreal.EditorAssetLibrary`

**Reading**
- Epic docs: *Asset Registry*.
- Epic's recommended asset naming conventions (also Allar's *UE Style Guide*).

**Assignment — "Naming Convention Auditor"** → `assignments/m02_naming_audit.py`

Write an auditor that scans `/Game/ActionRoguelike` and `/Game/Core` **using only the Asset Registry** (no `load_asset`).
- Define a prefix table keyed by class. Examples:
  - `Blueprint`→`BP_`
  - `WidgetBlueprint`→`WBP_`
  - `Material`→`M_`, `MaterialInstanceConstant`→`MI_`, `MaterialFunction`→`MF_`
  - `SoundCue`→`SC_`
  - `BehaviorTree`→`BT_`, `BlackboardData`→`BB_`, `EnvQuery`→`EQS_`
  - `CurveFloat`→`Curve_`
  - `NiagaraSystem`→`NS_`
  - `InputAction`→`IA_`, `InputMappingContext`→`IMC_`
- Report each asset's path, class, expected prefix, and the problem (`missing prefix`, `wrong prefix`, `duplicate name`).
- Write the report as CSV to `Saved/Reports/naming_audit.csv`, and print a summary table to the log.

*Done when:* the report flags at least these real issues in the project today:
- `MinonRangedBP`, `MinonControllerBP`, `MinonRangedBT`, `MinionRangedBB` (typo + suffix style)
- `ChestOpeningCurve`, `Core/PlayerCharacter`
- same-name duplicates: `BP_SItemChest`, `BP_SMagicProjectile`, `BP_SPlayerCharacter`

*Stretch:* for every duplicate, list its **referencers**, so you know which copy is actually used by `BasicMap` and which is dead weight. Also report Blueprints whose **parent class** is a project C++ class (read the `ParentClass` tag). This gives you a map of "which BP configures which C++ class".

---

## Tier 2 — Editing content & levels

### M3. Mutating assets safely

**Concepts**
- Rename/move creates **redirectors**. Fixing them up rewrites the referencers and deletes the redirectors.
- Deleting is safe only when nothing references the asset. Check the referencers first.
- `ScopedEditorTransaction` makes a block of changes a single undo step. It works for object and actor edits; asset renames have their own handling.
- `ScopedSlowTask` gives a progress dialog and cancel button for long operations.
- Design for safety: a dry run by default, a printed plan before applying, and an idempotent script (running it twice is harmless).
- Source control: assets are binary, so commit before running bulk operations.

**Key APIs**
- `unreal.EditorAssetLibrary.rename_asset / duplicate_asset / delete_asset / save_directory`
- `unreal.AssetToolsHelpers.get_asset_tools().rename_assets([AssetRenameData…])`
- `unreal.EditorAssetSubsystem.consolidate_assets`: replaces every reference to the duplicates with the kept asset
- `unreal.AssetRegistryHelpers.is_redirector`
- `unreal.ScopedSlowTask`, `unreal.ScopedEditorTransaction`

> **Heads-up (verified against this project's 5.6 stub):** redirector fix-up (`IAssetTools::FixupReferencers`) is **not** exposed to Python. Choose one workaround and write down why:
> - the content browser's *Update Redirector References* on the folder
> - the `ResavePackages -fixupredirects` commandlet (headless)
> - a C++ wrapper later in M8

**Assignment — "Content Cleanup"** → `assignments/m03_content_cleanup.py`

Using the output of M2 as input:
1. Rename the AI assets to the convention:
   - `MinonRangedBP` → `BP_MinionRanged`
   - `MinonControllerBP` → `BP_MinionController`
   - `MinonRangedBT` → `BT_MinionRanged`
   - `MinionRangedBB` → `BB_MinionRanged`
   - `ChestOpeningCurve` → `Curve_ChestOpening`
2. Move the loose Blueprints at the root of `/Game/ActionRoguelike` into sensible subfolders.
3. Find the redirectors left in the touched folders (`is_redirector`) and fix them up (see the heads-up above). Afterwards, re-run the scan and confirm it finds zero.
4. For each duplicate pair, delete the copy that has **zero referencers**. If both are referenced, `consolidate_assets` into the one inside the proper subfolder, then delete the other.

Requirements:
- `apply=False` by default (dry run prints the plan).
- Show a `ScopedSlowTask` progress bar that respects cancel.
- Log a final summary.

*Done when:* the M2 auditor reports zero issues for the AI folder, and `BasicMap` still runs in PIE with the minion attacking, fleeing, and healing. Also confirm `ARogueAIController` still finds `MinonRangedBT`. It is assigned in a Blueprint, so check that the reference followed the rename.

*Stretch:* make the rename table a JSON file so the script is reusable.

---

### M4. Blueprint defaults (CDO) as data

**Concepts**
- A Blueprint asset is not the class. `load_blueprint_class` gives the generated class, and `get_default_object` gives its **CDO** (the defaults you see in the Details panel).
- `get_editor_property` / `set_editor_property` work on any reflected property that is editable or Blueprint-visible. C++ `protected` does not matter. `EditDefaultsOnly` is reachable; `Transient`-only members like `CooldownUntil` are not.
- Type mapping: `unreal.GameplayTag`, `unreal.GameplayTagContainer`, `unreal.Map`, `unreal.Array`, struct copies vs. references (mutate a copy, then set it back).
- Components on a Blueprint: the defaults live on **subobject templates**, not on the CDO's runtime components. Use `SubobjectDataSubsystem` to reach them.
- Mark dirty and save: `EditorAssetLibrary.save_loaded_asset`.

**Key APIs**
- `unreal.EditorAssetLibrary.load_blueprint_class`
- `unreal.get_default_object`
- `obj.get_editor_property(...)`
- `unreal.GameplayTagLibrary.get_tag_name`
- `StructBase.export_text()` / `import_text()`
- `unreal.SubobjectDataSubsystem`

**Assignment — "Action Balance Sheet"** → `assignments/m04_balance_sheet.py`

Turn your gameplay tuning into a spreadsheet.
1. **Export:**
   - Every `BP_Action_*` in `/Game/ActionRoguelike/Actions`: `action_name`, `cooldown_time`, `activation_cost` (as `Attribute.Rage=5;…`), `grant_tags`, `blocked_tags`. For `RogueAction_ProjectileAttack` children, also add `ProjectileClass` and `MuzzleSocketName`.
   - Every projectile BP in `/Game/ActionRoguelike/Projectiles`: `Damage` and `LifeTime`.
   - Output goes to `Saved/Reports/balance.csv`.
2. **Import:** read the edited CSV back, print a **diff** (old → new) for each changed cell, then apply and save only the changed assets.

*Challenge:* `unreal.GameplayTag` has a read-only `tag_name` and no "from string" constructor.
- Find a way to build `Attribute.Rage` from a string. Hint: `StructBase.import_text('(TagName="Attribute.Rage")')`.
- Validate that the tag actually exists; an unknown name silently produces an empty tag.
- Write down the friction. M8 fixes it properly in C++.

*Done when:* changing the Gravity projectile's Rage cost in the CSV, importing, and starting PIE shows the new cost. The `Not enough Attribute.Rage` log from `URogueAction::CanStartAction` shows up at the new threshold.

*Stretch:* also export the **player's attribute defaults**. That means `AttributeSetClass` on the `RogueActionSystemComponent` template inside `BP_SPlayerCharacter`, and its `Health`, `HealthMax`, `MoveSpeed`, `Rage`, `RageMax`, and `RageGainPerDamage` base values.

---

### M5. Level / world scripting

**Concepts**
- Editor subsystems replace the deprecated `EditorLevelLibrary`:
  - `EditorActorSubsystem` for actors
  - `LevelEditorSubsystem` for loading and saving levels
  - `UnrealEditorSubsystem` for the editor world
- Spawning from a class or asset, setting labels, outliner folders (`set_folder_path`), and actor tags.
- Math types: `unreal.Vector`, `unreal.Rotator`, `unreal.Transform`, `unreal.MathLibrary`.
- Traces in the editor world: `SystemLibrary.line_trace_single` with the editor world as the context object.
- Navigation queries: `NavigationSystemV1.project_point_to_navigation`.
- Wrapping level edits in a single `ScopedEditorTransaction` so Ctrl+Z undoes the whole operation.

**Key APIs**
- `unreal.get_editor_subsystem(unreal.EditorActorSubsystem)`
- `spawn_actor_from_class`, `get_all_level_actors`, `set_selected_level_actors`, `destroy_actors`
- `unreal.SystemLibrary.line_trace_single`
- `unreal.NavigationSystemV1`

**Assignment — "Arena Populator & Level Lint"** → `assignments/m05_arena.py`
1. **Populator:** in `BasicMap`, spawn `BP_HealthPotion`, `BP_SExplosiveBarrel`, and `BP_STargetDummy` inside a box volume (or the bounds of a selected actor).
   - Choose the layout with a parameter: `grid` or `poisson` (Poisson-disc, minimum distance between props).
   - Snap each actor to the floor with a downward trace, and skip points that don't hit.
   - Put everything under the outliner folder `Generated/Arena` with the actor tag `Generated`.
   - Make it one undo step. A `clear()` function removes everything tagged `Generated`.
2. **Lint:** report (and select) problem actors:
   - minion BPs whose location doesn't project onto the NavMesh
   - health potions overlapping another actor's bounds
   - any actor below the world's `KillZ`

*Done when:* Ctrl+Z after populating removes everything in one step, and the lint selects a minion you deliberately placed off the NavMesh.

*Stretch:* use a seeded RNG and store the seed as an actor tag, so a layout can be reproduced exactly.

---

## Tier 3 — Building tools

### M6. Extending the editor UI with ToolMenus

**Concepts**
- ToolMenus is the data-driven menu system behind the main menu, toolbars, and context menus.
- Menus are found by name. Your existing `list_tool_menus.py` already dumps them.
- Entries: a simple `ToolMenuEntry` with a Python command string, or a **`ToolMenuEntryScript` subclass** (`@unreal.uclass`) that overrides `execute`, `can_execute`, `get_label`, and so on.
- Context menus get a `ToolMenuContext`. Content browser entries can read the selected assets or folders from it.
- Idempotency: register with stable names and remove or overwrite on reload. Call `refresh_all_widgets()` afterwards.

**Key APIs**
- `unreal.ToolMenus.get()`, `find_menu`, `extend_menu`, `add_sub_menu`, `add_section`, `add_menu_entry_object`
- `unreal.ToolMenuEntryScript`, `unreal.ToolMenuContext`
- `unreal.EditorUtilityLibrary.get_selected_assets()`, `get_selected_folder_paths()`

**Assignment — "Rogue Tools menu"** → `assignments/m06_menus.py`
1. Add a top-level **Rogue Tools** menu to `LevelEditor.MainMenu` with these entries:
   - Run Naming Audit (M2)
   - Export Balance Sheet / Import Balance Sheet (M4)
   - Populate Arena / Clear Arena / Lint Level (M5)
   - Reload Python Tools (`reload_all` from M1)
2. Content browser **folder** context menu: "Audit naming here", which runs M2 scoped to the clicked folder.
3. Content browser **asset** context menu: "Export to balance sheet". It is only enabled (`can_execute`) when every selected asset is a Blueprint whose parent is `RogueAction`.
4. Register everything from `init_unreal.py`. Running the registration twice must not duplicate entries.

*Done when:* all entries work, and "Reload Python Tools" followed by re-registration leaves exactly one copy of each entry.

*Stretch:* add a toolbar button with an icon from `EditorStyle` / `AppStyle`.

---

### M7. Python-defined UClasses & Editor Utility Widgets

**Concepts**
- Python can **define** reflected types:
  - `@unreal.uclass()` (subclass a UClass)
  - `@unreal.ustruct()`, `@unreal.uenum()`
  - `@unreal.ufunction(...)` (expose methods; `static=True`, `params=[…]`, `ret=…`, `meta=dict(Category=…)`)
  - `@unreal.uproperty(...)`
- A Python `BlueprintFunctionLibrary` subclass makes its static ufunctions show up as **Blueprint nodes**. That is the bridge into Editor Utility Widgets (EUW).
- Lifetime: Python-defined classes exist only while Python is loaded and the module has been imported. Import them from `init_unreal.py`.
- Binding delegates to Python callables: `delegate.add_callable(fn)`.

**Key APIs**
- `unreal.uclass`, `unreal.ufunction`, `unreal.uproperty`
- `unreal.BlueprintFunctionLibrary`
- `unreal.EditorUtilitySubsystem.spawn_and_register_tab`

**Assignment — "Action Tuner" Editor Utility Widget** → `assignments/m07_action_tuner.py` + `EUW_ActionTuner` asset
1. Write a Python BFL `RogueToolsLibrary` with static ufunctions. Reuse the M4 code.
   - `list_actions() -> Array[str]`
   - `get_action_cooldown(path) -> float`
   - `set_action_cooldown(path, value)`
   - `get_action_cost(path, attribute) -> float`
   - `set_action_cost(path, attribute, value)`
2. Build `EUW_ActionTuner` (Content/Developers or `/Game/Editor/`). It shows a list of actions, and selecting one shows editable cooldown and Rage/Health cost fields that call your library.
3. Add a **"Grant to player"** button. It appends the selected action class to `DefaultActions` on the `RogueActionSystemComponent` template of `BP_SPlayerCharacter`, using M4's subobject knowledge. It must not add duplicates.
4. Add a Rogue Tools menu entry (M6) that opens the widget with `spawn_and_register_tab`.

*Done when:* you can tune the Teleport cooldown from the widget, press Play, and feel the difference, without opening the action Blueprint.

---

### M8. Bridging C++ and Python

**Concepts**
- Some things Python cannot do (or should not do): private or non-reflected data, hot loops, engine APIs without reflection.
- The fix is a small **editor module** that exposes `BlueprintCallable` C++ functions. These are automatically available in Python after a rebuild.
- Editor modules: `Type: "Editor"` in `.uproject`, a separate `Build.cs`, dependencies on `UnrealEd` and friends. They are never shipped.
- Script metadata:
  - `meta=(ScriptMethod)` turns a static `Foo(UMyObj* Obj)` into `obj.foo()` in Python
  - `ScriptName` renames the function for Python
  - `ScriptConstant`, `DeprecatedFunction`
- The stub regenerates on the next editor start. Check your new functions in it.

**Key APIs (C++ side)**
- `UBlueprintFunctionLibrary`
- `TFieldIterator<FStructProperty>`
- `FGameplayTag::RequestGameplayTag(Name, /*ErrorIfNotFound*/ false)`
- `UGameplayTagsManager::Get().RequestAllGameplayTags`

**Assignment — "Attribute & Tag Validator"** → new module `Source/TomLoomanCourseEditor/` + `assignments/m08_tag_validator.py`
1. Create the `TomLoomanCourseEditor` module and register it in `TomLoomanCourse.uproject` and `TomLoomanCourseEditor.Target.cs`.
2. Add a `URogueEditorLibrary` with:
   - `MakeGameplayTag(FName TagName, bool& bValid) -> FGameplayTag`. This replaces the M4 `import_text` workaround.
   - `GetAttributePropertyNames(TSubclassOf<URogueAttributeSet> SetClass) -> TArray<FName>`. It mirrors the `TFieldIterator` loop in `URogueActionSystemComponent::InitializeComponent` (`ActionSystem/URogueActionSystemComponent.cpp`).
   - `GetAllRegisteredTags(FString Prefix) -> TArray<FName>`.
3. In Python, validate the following:
   - Every `FRogueAttribute` property of **every** `URogueAttributeSet` subclass has a registered `Attribute.<PropertyName>` tag. This is exactly what the runtime relies on.
   - Every `BP_Action_*` has a non-empty `ActionName`, and every `ActivationCost` key is a registered `Attribute.*` tag.
   - Every `Action.Attack.*` tag declared in `SharedGameplayTags` has at least one action asset using it.

*Done when:* the validator passes on the current project. Then add `FRogueAttribute Armor;` to `URoguePawnAttributeSet` **without** registering a tag: the validator must report it before you ever press Play. Revert afterwards.

*Stretch:* wrap `IAssetTools::FixupReferencers` as `FixupRedirectorsInFolder(FString Path)` and replace the M3 workaround with it. Also add `meta=(ScriptMethod)` to a helper so you can write `action_cdo.get_cost_map_as_strings()` in Python.

---

## Tier 4 — Pipelines & automation

### M9. Inspecting PIE & async editor work

**Concepts**
- The editor world vs. the **PIE world**: `UnrealEditorSubsystem.get_editor_world()` vs. `get_game_world()`.
- Python runs on the game thread, so a `while` loop freezes the editor. Use `unreal.register_slate_post_tick_callback(fn)` (and unregister it) for periodic work, and accumulate delta time yourself.
- Calling gameplay `BlueprintCallable` functions on live PIE objects from Python: reading attributes, applying changes.
- Detecting PIE start and end: poll `get_game_world()` in your tick callback, or bind to editor delegates where they are exposed.

**Key APIs**
- `unreal.UnrealEditorSubsystem.get_game_world`
- `unreal.GameplayStatics.get_all_actors_of_class`, `get_player_pawn`
- `actor.get_component_by_class(unreal.RogueActionSystemComponent)`
- `RogueActionSystemComponent.get_attribute_value`, `apply_attribute_change`, `is_alive`
- `unreal.register_slate_post_tick_callback` / `unregister_slate_post_tick_callback`

**Assignment — "Live Attribute Monitor"** → `assignments/m09_pie_monitor.py`
1. `start_monitor(interval=0.5)` registers a tick callback. While PIE is running, every `interval` seconds it samples:
   - the player's `Attribute.Health`, `Attribute.Rage`, and `Attribute.MoveSpeed`
   - every `ARogueAICharacter`'s Health and `is_alive()`
2. Log only **changes** (not every sample).
3. When PIE ends, write `Saved/Reports/pie_timeline_<timestamp>.csv` and reset. The monitor stays registered across multiple PIE sessions until `stop_monitor()`.

*Done when:* a PIE session where you take damage, cast the Gravity projectile (Rage cost), sprint, and kill a minion produces a CSV that shows Rage rising with damage taken and dropping on cast, MoveSpeed changing while sprinting, and the minion's `is_alive` flipping.

*Stretch:* add Rogue Tools → **Cheats** entries that act on the PIE player through `apply_attribute_change`:
- *Fill Rage* (`OverrideBase`)
- *Double Speed* (`Multiplier`, 2.0)
- *Reset Speed* (`RemoveMultiplier`)

Watch how the multiplier semantics in `ApplyAttributeChange` behave.

---

### M10. Validation, headless runs & CI (capstone)

**Concepts**
- **Data Validation:** subclass `unreal.EditorValidatorBase` in Python (`@unreal.uclass`) and override `can_validate_asset` and `validate_loaded_asset`. Report with `asset_fails` / `asset_warning`. It runs on save, on *Validate Assets* in the content browser, and in commandlets.
- **Headless editor:**
  - `UnrealEditor-Cmd.exe <project>.uproject -run=pythonscript -script="path/to/script.py" -unattended -nullrhi -nosplash -stdout`
  - Alternatively, `-ExecutePythonScript=` on a full editor.
- Exit codes and parsing the log.
- CI: a git `pre-push` hook or a GitHub Actions self-hosted runner (it needs the engine installed). Reports go to Markdown or JUnit XML.

**Key APIs**
- `unreal.EditorValidatorBase`
- `unreal.EditorValidatorSubsystem` (`validate_assets_with_settings`)
- everything from M2, M4, and M8

**Assignment — "Rogue Content Pipeline"** → `assignments/m10_validators.py`, `assignments/ci_report.py`, `.githooks/pre-push`
1. **Validator** `RogueActionValidator` for Blueprints whose parent is `RogueAction`:
   - error if `ActionName` is empty or not a registered tag (reuse the M8 helper)
   - error if a `RogueAction_ProjectileAttack` child has no `ProjectileClass` or `AttackMontage`
   - warning if `ActivationCost` keys aren't `Attribute.*`, or if `CooldownTime < 0`

   Register it from `init_unreal.py`. Break an action on purpose and check that saving it shows the error.
2. **Headless report** `ci_report.py` runs:
   - the naming audit (M2)
   - the tag validator (M8)
   - the Data Validation pass over `/Game/ActionRoguelike` (step 1)
   - the balance export (M4)

   It writes `Saved/Reports/report.md` (sections, counts, tables) and **exits non-zero** when there are errors.
3. A `pre-push` git hook (`git config core.hooksPath .githooks`) runs the headless report and blocks the push on failure.

*Done when:* a clean push takes under a few minutes, and a push with a broken `BP_Action_*` is blocked with a readable reason.

*Stretch:* run it on GitHub Actions with a self-hosted Windows runner, and upload `report.md` as a build artifact or job summary.

---

## Progress checklist

- [ ] M1 — Project Banner & Dev Loop
- [ ] M2 — Naming Convention Auditor
- [ ] M3 — Content Cleanup
- [ ] M4 — Action Balance Sheet
- [ ] M5 — Arena Populator & Level Lint
- [ ] M6 — Rogue Tools menu
- [ ] M7 — Action Tuner EUW
- [ ] M8 — Attribute & Tag Validator (C++ editor module)
- [ ] M9 — Live Attribute Monitor
- [ ] M10 — Rogue Content Pipeline (capstone)

## References
- Epic: *Scripting the Unreal Editor Using Python* and the *Python API reference* (select 5.6).
- Epic: *Extending the Editor UI with ToolMenus*, *Editor Utility Widgets*, *Data Validation*.
- Local stub: `Intermediate/PythonStub/unreal.py`. This is the source of truth for your engine version and project classes.
- Engine sources worth reading:
  - `Engine/Plugins/Experimental/PythonScriptPlugin/Content/Python/` (`remote_execution.py`, `debugger_*`)
  - `Engine/Plugins/Editor/DataValidation/`
