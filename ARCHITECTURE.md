# TomLoomanCourse — Architecture Overview

> Analysis of the code as of branch `AutomationCourse` (commit `6b6ff52`).
> Engine: Unreal Engine 5.6 · Single runtime module `TomLoomanCourse` · C++ + Blueprint hybrid, plus Python editor scripting.
> Note: `README.md` is the upstream course README. The **Action System** and **GameplayTags** it lists are now
> implemented here. SaveGame, multiplayer, and the Asset Manager are **not yet implemented**.

## 1. High-level approach

The project follows the **standard Unreal Gameplay Framework** with a few clear ideas:

| Principle | How it shows up |
|---|---|
| **C++ base, Blueprint config** | C++ classes define behavior and expose `UPROPERTY(EditAnywhere/EditDefaultsOnly)` knobs. `BP_*` subclasses in `Content/ActionRoguelike` assign meshes, VFX, SFX, projectile classes, input actions, and widget classes. |
| **Composition over inheritance for shared capabilities** | Attributes, actions, and death live in `URogueActionSystemComponent`, and interaction lives in `USInteractionComponent`. Any actor gets the capability by adding the component. |
| **Tag-driven gameplay** | Attributes, actions, and status effects are addressed by `FGameplayTag` (`Attribute.Health`, `Action.Attack.MagicProjectile`, `StatusEffect.Sprinting`) rather than by hard-coded members. |
| **Event-driven communication** | Per-attribute listeners (native and Blueprint) and dynamic multicast delegates (`OnDeath`, `OnBeginPlay`) decouple the attribute owner from its listeners (UI, animation, controller, VFX). |
| **Interfaces for interaction** | `ISGameplayInterface::Interact` (BlueprintNativeEvent) lets the interaction component talk to chests and pickups without knowing their types. |
| **Inheritance for families of similar actors** | Abstract bases with virtual hooks (Template Method style): `ARogueProjectileBase`, `APickableBase`, `URogueAction`, `URogueAttributeSet`. |
| **Data-driven AI** | Behavior Tree, Blackboard, and EQS assets drive the AI. Custom C++ BT nodes are small, reusable building blocks. |

## 2. Module & folder layout

```
Source/TomLoomanCourse/
├── TomLoomanCourse.Build.cs   Public: Core, CoreUObject, Engine, InputCore, EnhancedInput, Niagara, UMG, GameplayTags
│                              Private: EngineCameras, AIModule
├── SharedGameplayTags.h/.cpp  Native tags (Attribute.*, Action.*)
├── ActionSystem/  URogueActionSystemComponent, URogueAction(+_ProjectileAttack), URogueAttributeSet family
├── AI/            Enemy AI (Rogue* prefix), .h and .cpp side-by-side
├── Core/          ASGameModeBase, ISGameplayInterface, URogueGameplayStatics
├── Player/        ASCharacter, ASPlayerController, USInteractionComponent
├── Public/        Headers for world actors, projectiles, pickups, UI
└── Private/       .cpp files + some private-only headers (SAnimInstance, SCameraShake,
                   SDamagePopUp_Widget, STargetDummy)
```

**Plugins** (`TomLoomanCourse.uproject`): PythonScriptPlugin, EditorScriptingUtilities, PythonFoundationPackages, ModelingToolsEditorMode (editor only).

**Content:**
- `Content/Core`: `BP_SGameModeBase`, `BP_SPlayerController`, `BP_SPlayerCharacter`, `Input/`.
- `Content/ActionRoguelike/{AI, Actions, Projectiles, Interactable, UI, Materials, Audio}`.
  - `Actions/` holds `BP_Action_MagicProjectile`, `BP_Action_GravityProjectile`, `BP_Action_TeleportProjectile`, and `BP_Action_Sprint`.
- `Content/Maps/BasicMap` (default + startup map).
- `Content/Python`: editor scripts (see §6).

**Naming conventions:** three styles coexist:
- `S` prefix (`ASCharacter`, `ASPlayerController`): the original course convention.
- `Rogue` prefix (`URogueActionSystemComponent`, `ARogueProjectileBase`, `URogueBTTask_*`): the newer course convention. It is spreading: `SAttributesComponent` and `SProjectileBase` have already been renamed.
- No prefix (`APickableBase`, `AHealthPotion`): the user's own additions.

Renamed classes and properties are kept loadable through `[CoreRedirects]` in `Config/DefaultEngine.ini`.

## 3. Core systems

### 3.1 Player
- **`ASCharacter`** (`ACharacter`) owns a SpringArm + Camera, `USInteractionComponent`, and `URogueActionSystemComponent`.
  - Input: **Enhanced Input**. The `UInputAction*` assets (`Input_Move`, `Input_Look`, `Input_Jump`, `Input_Sprint`, `Input_PrimaryAttack`, `Input_SwitchWeapon`, `Input_Interaction`) live on the character, which binds them in `SetupPlayerInputComponent`.
  - Attacks go through the action system. `SelectedAttackName` holds the current `Action.Attack.*` tag:
    - PrimaryAttack calls `StartAction(SelectedAttackName)`.
    - SwitchWeapon cycles the selection with `GetNextAttackName`.
    - Sprint starts and stops `Action.Sprint`.
  - Reactions: a native listener on `Attribute.Health` drives the hit-flash material params. `OnDeath` turns on capsule ragdoll physics.
- **`ASPlayerController`** adds the `UInputMappingContext`s (local player only) and creates the `USPlayerHealthBar` widget on possess.
  - It binds the widget to `OnBeginPlay` and to a dynamic `Attribute.Health` listener.
  - On unpossess it unbinds them before calling `Super`. On death it unpossesses.
- **`USAnimInstance`** subscribes to `OnDeath` (bound from `ASCharacter`) to flip `bIsAlive`, and the AnimBP plays the death animation.

### 3.2 Action System & Attributes — the central hub
`URogueActionSystemComponent` (in `ActionSystem/`) replaces the old `USAttributesComponent`. It owns three things: an **attribute set**, a list of **actions**, and the active **gameplay tags**.

**Attributes**
- `FRogueAttribute { Base, Modifier, Multiplier }`, where `GetValue() = max((Base + Modifier) * Multiplier, 0)`.
  - `Base` is the persistent value.
  - `Modifier` and `Multiplier` are transient buffs and costs.
- Attribute sets are a UObject hierarchy, picked per component through `AttributeSetClass`:

```
URogueAttributeSet                 (InitializeAttributes / PostAttributeChanged hooks)
└── URogueHealthAttributeSet       Health, HealthMax · clamps Health · fires OnOutOfHealth when crossing 0
    └── URoguePawnAttributeSet     MoveSpeed → pushes into CharacterMovement::MaxWalkSpeed
        ├── URoguePlayerAttributeSet   Rage, RageMax · gains Rage = damage taken × RageGainPerDamage
        └── URogueMonsterAttributeSet
```

- **Reflection-based tag binding:** `InitializeComponent` iterates the set's `FStructProperty`s and caches `"Attribute." + PropertyName` → `FRogueAttribute*`. Every `FRogueAttribute` property therefore needs a registered tag with the matching name (see `SharedGameplayTags`).
- **`ApplyAttributeChange(Tag, Delta, ModifyType)`**, where `ModifyType` ∈ `Base | Modifier | OverrideBase | Multiplier | RemoveMultiplier`:
  1. Mutates the attribute.
  2. Calls `PostAttributeChanged` on the set (clamping, Rage gain; this may nest further changes).
  3. Broadcasts the native listeners (`GetAttributeListener(Tag)`), then the dynamic/Blueprint listeners (`AddAttributeDynamicListener`). Unbound dynamic delegates are cleaned up as it goes.
  4. Defers death until the outermost nested change finishes (`AttributeChangeDepth` + `bPendingDeath`, set by `OnOutOfHealth`). Listeners therefore see the final value before `OnDeath`.
- Read access: `GetAttribute(Tag)` (raw pointer) and `GetAttributeValue(Tag)` (BlueprintCallable).

**Actions**
- `URogueAction` (Blueprintable UObject, outer = the component) is configured in `BP_Action_*` assets:
  - `ActionName` (tag)
  - `GrantTags` / `BlockedTags`
  - `CooldownTime`
  - `ActivationCost` (`TMap<Tag, float>`)
- `CanStartAction` checks the cooldown, the `BlockedTags` against `ActiveGameplayTags`, and the cost against the current attribute values.
- `StartAction` stamps the cooldown, appends the `GrantTags`, and pays costs as negative `Modifier`s. `StopAction` removes the `GrantTags`.
- `URogueAction_ProjectileAttack` plays the montage and the casting VFX/SFX, then spawns `ProjectileClass` from the muzzle socket toward the camera aim point.
- The component grants `DefaultActions` in `InitializeComponent`. `GetNextAttackName` cycles through the actions whose tag matches `Action.Attack`.

**Tags**
- `SharedGameplayTags` declares the native tags: `Attribute.{Health, HealthMax, MoveSpeed, Rage, RageMax}`, `Action.Attack`, `Action.Attack.{MagicProjectile, GravityProjectile, TeleportProjectile}`, and `Action.Sprint`.
- `Config/DefaultGameplayTags.ini` adds `StatusEffect.Attacking` and `StatusEffect.Sprinting`.

**Helpers**
- `URogueActionSystemComponent::GetAttributesComponent(AActor*)`.
- `URogueGameplayStatics::IsFullHealth`.

Who listens:

```
                                         ┌─ native Health listener ──► ASCharacter      (hit flash)
                                         ├─ native Health listener ──► ASTargetDummy ──► USDamagePopUp_Widget
URogueActionSystemComponent              ├─ dynamic Health listener ─► ASPlayerController ──► USPlayerHealthBar (UI)
  attribute listeners / OnDeath /        ├─ OnBeginPlay ─────────────► USPlayerHealthBar::SetDefaults
  OnBeginPlay                            └─ OnDeath ─────────────────► ASCharacter (ragdoll), USAnimInstance,
                                                                       ASPlayerController, ARogueAIController
```

Who changes attributes:
- `ASMagicProjectile` (damage on overlap)
- `ASExplosiveBarrel` / `USRadialForceComponent` (explosion damage)
- `AHealthPotion` (heal on interact)
- `URogueBTTask_Heal` (AI self-heal)
- `URogueAction` costs (Rage, etc.)
- `URoguePlayerAttributeSet` (Rage gain from damage)

### 3.3 Interaction
`USInteractionComponent::PrimaryInteract` sphere-sweeps from the owner's eyes against `ECC_WorldDynamic`. It calls `ISGameplayInterface::Execute_Interact` on the first actor that implements the interface.

Implementers:
- `ASItemChest`: an `FTimeline` driven by a `UCurveFloat` animates the lid, and Niagara plays when it opens.
- `APickableBase` (abstract): plays the pickup VFX, then either destroys itself or goes inactive and respawns on a timer (`bMultiPickable`).
  - `AHealthPotion` heals the instigator.

### 3.4 Projectiles
`ARogueProjectileBase` (abstract; formerly `ASProjectileBase`) holds the Sphere (profile `Projectile`), ProjectileMovement, Niagara, and Audio components, plus `Damage`, `LifeTime`, and the impact VFX/SFX. It provides shared impact handling (emitter, sound, camera shake) and virtual hooks:
`OnComponentHit`, `OnComponentBeginOverlap`, `OnTimeElapasedAfterSpawn`, `SpawnEmitter`.

| Subclass | Behavior |
|---|---|
| `ASMagicProjectile` | Damages any actor with an action system component on overlap; destroys itself on hit. |
| `ASTeleportProjectile` | Travels for a set time (or until it hits something), then teleports the instigator. |
| `ASGravityProjectile` | Negative radial force (black hole) that destroys physics bodies it overlaps. |

Spawned by `URogueAction_ProjectileAttack` (player), `ASShootingMachine` (turret test actor), and `URogueBTTask_RangedAttack` (AI).

### 3.5 World / test actors
`ASExplosiveBarrel` + `USRadialForceComponent` (custom `MakeExplosion`), `ASShootingMachine`,
`ASTargetDummy` (hit flash + floating damage pop-up), `USCameraShake` (legacy shake).

### 3.6 UI (UMG)
The pattern is a **C++ widget base with Blueprint presentation**. `USPlayerHealthBar` receives data through C++ (`SetHealth`, bound as a dynamic attribute listener) and forwards it to `BlueprintImplementableEvent`s (`OnHealthChanged`, `SetDefaults`) so the WBP handles visuals. `USDamagePopUp_Widget` projects a world position to the screen each tick and removes itself on a timer.

### 3.7 Game mode
`ASGameModeBase` (`Core/`) is an empty C++ shell. Configuration lives in `Content/Core/BP_SGameModeBase` (set as `GlobalDefaultGameMode`).

## 4. AI architecture (`AI/`)

```
ARogueAIController ──RunBehaviorTree(MinonRangedBT)──► Blackboard (MinionRangedBB)
       │  BeginPlay: TargetActor = player pawn
       │  OnPossess: bind OnDeath → destroy pawn
       ▼
ARogueAICharacter (ACharacter + URogueActionSystemComponent)   ← BP: MinonRangedBP
```

Custom C++ BT nodes, each a single-purpose node configured from the BT editor:

| Node | Type | Purpose |
|---|---|---|
| `URogueBTService_CheckRangeTo` | Service | Writes `WithinRange` = distance ≤ `MaxAttackRange` && line of sight. |
| `URogueBTTask_RangedAttack` | Task | Spawns `ProjectileClass` from the muzzle socket toward the target, with random spread. |
| `URogueBTTask_Heal` | Task | `ApplyAttributeChange(Attribute.Health, +HealAmount)` on its own pawn. |
| `URogueUBTDecorator_IsLowHealth` | Decorator | Health % ≤ `LowHealthThreshold` gates the flee/heal branch. |
| `URogueEnvQueryContext_TargetActor` | EQS context | Supplies the Blackboard `TargetActor` to EQS queries. |

EQS assets: `EQS_MoveToTarget` (attack positioning) and `EQS_MoveToSafePlace` (flee).
Shared constants live in `Public/RogueGameTypes.h` (`NAME_TargetActor`, custom collision channel macros).

## 5. Development workflow
- Feature branches (`Feature/HealthPotion`, `Feature/GameFeel`, `AI/EnemyMonsters`, `ActionSystem`, `AutomationCourse`…) are merged into `master` via PRs. `Claude/*` branches (e.g. `Claude/CourseArchitectureUpgrade`) hold AI-assisted refactors and get merged into the active feature branch.
- Commit prefixes: `[Add]`, `[Fix]`, `[Ref]`, `[Update]`, `[Move]`, plus "Assignment N" for course assignments.
- Blueprint screenshots are kept in `BlueprintScreenshots/`.
- Python automation study plan: `Docs/PythonAutomation_StudyPlan.md`.

## 6. Editor scripting (Python)
- **Plugins:** PythonScriptPlugin, EditorScriptingUtilities, PythonFoundationPackages.
- **`Content/Python/`** is on the editor's Python path:
  - `init_unreal.py` runs automatically at editor start (it currently logs a banner).
  - `list_tool_menus.py` prints every registered ToolMenus name. It is registered as a startup script through `StartupScripts=list_tool_menus.py`.
  - `assignments/` holds the study-plan exercises.
- **`[/Script/PythonScriptPlugin.PythonScriptPluginSettings]`** in `DefaultEngine.ini`:
  - `bDeveloperMode=True` generates the API stub at `Intermediate/PythonStub/unreal.py`. Point your IDE at it for autocomplete.
  - `bRemoteExecution=True` lets external tools and IDEs run code in the editor (multicast `239.0.0.1:6766`, bound to `127.0.0.1`).
  - `bIsolateInterpreterEnvironment=True`, and `bRunPipInstallOnStartup=True` installs plugin-declared pip dependencies.
- All project UFUNCTION/UPROPERTY members marked as editable or Blueprint-visible are reflected into Python. For example, `unreal.RogueActionSystemComponent.get_attribute_value`, and the `RogueAction` editor properties `action_name`, `cooldown_time`, `activation_cost`.

## 7. Observations & improvement opportunities

**Consistency**
- The component is looked up three different ways: `GetAttributesComponent()`, `FindComponentByClass`, and `Cast<>(GetComponentByClass(...))` (in `HealthPotion`, `SMagicProjectile`, `SRadialForceComponent`, `RogueUBTDecorator_IsLowHealth`). Standardizing on the static helper would help.
- Some members and variables still say "Attributes" (`AttributesComponent`, the `"AttributesComponent"` subobject name) even though the type is now the action system component.
- Pointers are a mix of raw `UPROPERTY` pointers and `TObjectPtr`. UE5 recommends `TObjectPtr` for members.
- Constants are `#define` macros in `RogueGameTypes.h`. A `static const FName` or a namespace would be more idiomatic.
- Three naming prefixes coexist (see §2).
- Some headers sit in `Private/` while others are in `Public/`.
- The comment on `URogueAction::ActivationCost` ("Attacks are the actions the player cycles through…") belongs to another property. It also shows up in the Python docstrings.

**Likely bugs / risks**
- `ARogueProjectileBase::OnComponentHit`: `if (ensure(IsPendingKillPending())) return;` fires the ensure on every normal hit. The intent was probably `if (IsPendingKillPending()) return;`.
- `ARogueAIController::BeginPlay` uses `check(PlayerPawn)`, which crashes if there is no player pawn yet.
- `ApplyAttributeChange` calls `Death(GetOwner())`, so `OnDeath` receives the victim instead of whoever dealt the damage.
- `URoguePawnAttributeSet::ApplyMoveSpeed` casts the owner to `ACharacter` without a null check. Assigning a Pawn/Player/Monster set to a non-Character actor (e.g. `ASTargetDummy`) would crash.
- Attribute tags are derived from property names. Adding an `FRogueAttribute` without registering the matching `Attribute.<Name>` tag fails at runtime in `RequestGameplayTag`.
- Leftover state: `ARogueAIController::LowHealthKey/bLowHealth` and `ARogueAICharacter::bIsLowHealth` are unused. `EventsCopy` in `ApplyAttributeChange` is unused.

**Performance / scope**
- Every attribute change logs at `Warning` level.
- Most actors and components keep `bCanEverTick = true` with empty `Tick`. They could disable ticking.
- The code assumes single-player throughout (`GetPlayerPawn(this,0)`, `GetFirstPlayerController`) and has no replication.
- Debug draws (`DrawDebugLine/Sphere`) run unconditionally in shipping code paths.

**Content hygiene** (these are good targets for Python automation; see the study plan)
- Same-name duplicate Blueprints:
  - `BP_SItemChest` (`ActionRoguelike/` and `ActionRoguelike/Interactable/`)
  - `BP_SMagicProjectile` (`ActionRoguelike/` and `ActionRoguelike/Projectiles/`)
  - `BP_SPlayerCharacter` (`ActionRoguelike/` and `Core/`)
- AI assets use a typo and suffix-style names: `MinonRangedBP`, `MinonControllerBP`, `MinonRangedBT`, `MinionRangedBB`.
- Assets without a type prefix: `ChestOpeningCurve`, `Core/PlayerCharacter`, `Att_Projectiles`.
