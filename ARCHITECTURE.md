# TomLoomanCourse — Architecture Overview

> Analysis of the code as of branch `AI/EnemyMonsters` (commit `ad1cdfc`).
> Engine: Unreal Engine 5.6 · Single runtime module `TomLoomanCourse` · C++ + Blueprint hybrid.
> Note: `README.md` is the upstream course README and lists features (Action System, SaveGame,
> GameplayTags, multiplayer, Asset Manager…) that are **not yet implemented** in this repo.

## 1. High-level approach

The project follows the **standard Unreal Gameplay Framework** with a few clear ideas:

| Principle | How it shows up |
|---|---|
| **C++ base, Blueprint config** | C++ classes define behavior and expose `UPROPERTY(EditAnywhere/EditDefaultsOnly)` knobs. `BP_*` subclasses in `Content/ActionRoguelike` assign meshes, VFX, SFX, projectile classes, input actions, and widget classes. |
| **Composition over inheritance for shared capabilities** | Health/death is in `USAttributesComponent` and interaction in `USInteractionComponent`. Any actor gets the capability by adding the component. |
| **Event-driven communication** | Dynamic multicast delegates (`OnHealthChanged`, `OnDeath`, `OnBeginPlay`) decouple the attribute owner from its listeners (UI, animation, controller, VFX). |
| **Interfaces for interaction** | `ISGameplayInterface::Interact` (BlueprintNativeEvent) lets the interaction component talk to chests and pickups without knowing their types. |
| **Inheritance for families of similar actors** | Abstract bases with virtual hooks (Template Method style): `ASProjectileBase` and `APickableBase`. |
| **Data-driven AI** | Behavior Tree, Blackboard, and EQS assets drive the AI. Custom C++ BT nodes are small, reusable building blocks. |

## 2. Module & folder layout

```
Source/TomLoomanCourse/
├── TomLoomanCourse.Build.cs   Public: Core, CoreUObject, Engine, InputCore, EnhancedInput, Niagara, UMG
│                              Private: EngineCameras, AIModule
├── Public/    Headers for gameplay classes (S* prefix, plus PickableBase/HealthPotion)
├── Private/   .cpp files + some private-only headers (SAnimInstance, SCameraShake,
│              SDamagePopUp_Widget, STargetDummy)
└── AI/        Enemy AI (Rogue* prefix), .h and .cpp side-by-side
```

Content mirrors this split: `Content/Core` (GameMode BP), `Content/ActionRoguelike/{AI, Projectiles,
Interactable, UI, Materials, Audio}`, `Content/Maps/BasicMap` (default + startup map).

**Naming conventions:** three styles coexist:
- `S` prefix (`ASCharacter`, `USAttributesComponent`): the original course convention.
- `Rogue` prefix (`ARogueAIController`, `URogueBTTask_*`): the newer course convention, used in `AI/`.
- No prefix (`APickableBase`, `AHealthPotion`): the user's own additions.

## 3. Core systems

### 3.1 Player
- **`ASCharacter`** (`ACharacter`) owns a SpringArm + Camera, `USInteractionComponent`, and `USAttributesComponent`.
  - Input: **Enhanced Input**. The `UInputAction*` assets live on the character, which binds them in `SetupPlayerInputComponent`.
  - Attack: it plays a montage, then after a 0.2 s timer spawns `CurrentProjectile` from the `Muzzle_01` socket, aimed at a camera line-trace hit point. `SwitchProjectile` cycles through the `Projectiles` array.
  - Reactions: it listens to its own attributes component. `OnHealthChanged` drives the hit-flash material params, and `OnDeath` turns on capsule ragdoll physics.
- **`ASPlayerController`** adds the `UInputMappingContext`s (local player only), creates the `USPlayerHealthBar` widget on possess, and wires the pawn's attribute delegates to the widget. On death it unpossesses.
- **`USAnimInstance`** subscribes to `OnDeath` to flip `bIsAlive`, and the AnimBP plays the death animation.

### 3.2 Attributes (health) — the central hub
`USAttributesComponent` holds `Health`/`MaxHealth`/`bIsAlive` and exposes:
- `ApplyHealthChange(Delta)`: clamps the value, broadcasts `OnHealthChanged`, and calls `Death()` at 0.
- `GetHealthPercent()`, `IsAlive()`, and the static helper `GetAttributesComponent(AActor*)`.

Who listens:

```
                                          ┌──► ASCharacter            (hit flash / ragdoll)
                                          ├──► USAnimInstance         (death anim)
USAttributesComponent                     │
  OnHealthChanged / OnDeath / OnBeginPlay ┼──► ASPlayerController ──► USPlayerHealthBar (UI)
                                          ├──► ARogueAIController     (destroy pawn on death)
                                          └──► ASTargetDummy ───────► USDamagePopUp_Widget
```

Who changes health: `ASMagicProjectile` (damage on overlap), `AHealthPotion` (heal on interact), `URogueBTTask_Heal` (AI self-heal).

### 3.3 Interaction
`USInteractionComponent::PrimaryInteract` sphere-sweeps from the owner's eyes against `ECC_WorldDynamic`. It calls `ISGameplayInterface::Execute_Interact` on the first actor that implements the interface.

Implementers:
- `ASItemChest`: an `FTimeline` driven by a `UCurveFloat` animates the lid, and Niagara plays when it opens.
- `APickableBase` (abstract): plays the pickup VFX, then either destroys itself or goes inactive and respawns on a timer (`bMultiPickable`).
  - `AHealthPotion` heals the instigator.

### 3.4 Projectiles
`ASProjectileBase` (abstract) holds the Sphere (profile `Projectile`), ProjectileMovement, Niagara, and Audio components, plus `Damage`, `LifeTime`, and the impact VFX/SFX. It provides shared impact handling (emitter, sound, camera shake) and virtual hooks:
`OnComponentHit`, `OnComponentBeginOverlap`, `OnTimeElapasedAfterSpawn`, `SpawnEmitter`.

| Subclass | Behavior |
|---|---|
| `ASMagicProjectile` | Damages any actor with attributes on overlap; destroys itself on hit. |
| `ASTeleportProjectile` | Travels for a set time (or until it hits something), then teleports the instigator. |
| `ASGravityProjectile` | Negative radial force (black hole) that destroys physics bodies it overlaps. |

Spawned by `ASCharacter`, `ASShootingMachine` (turret test actor), and `URogueBTTask_RangedAttack`.

### 3.5 World / test actors
`ASExplosiveBarrel` + `USRadialForceComponent` (custom `MakeExplosion`), `ASShootingMachine`,
`ASTargetDummy` (hit flash + floating damage pop-up), `USCameraShake` (legacy shake).

### 3.6 UI (UMG)
The pattern is a **C++ widget base with Blueprint presentation**. `USPlayerHealthBar` receives data through C++ (`SetHealth`) and forwards it to `BlueprintImplementableEvent`s (`OnHealthChanged`, `SetDefaults`) so the WBP handles visuals. `USDamagePopUp_Widget` projects a world position to the screen each tick and removes itself on a timer.

### 3.7 Game mode
`ASGameModeBase` is an empty C++ shell. Configuration lives in `Content/Core/BP_SGameModeBase` (set as `GlobalDefaultGameMode`).

## 4. AI architecture (`AI/`)

```
ARogueAIController ──RunBehaviorTree(MinonRangedBT)──► Blackboard (MinionRangedBB)
       │  BeginPlay: TargetActor = player pawn
       │  OnPossess: bind OnDeath → destroy pawn
       ▼
ARogueAICharacter (ACharacter + USAttributesComponent)   ← BP: MinonRangedBP
```

Custom C++ BT nodes, each a single-purpose node configured from the BT editor:

| Node | Type | Purpose |
|---|---|---|
| `URogueBTService_CheckRangeTo` | Service | Writes `WithinRange` = distance ≤ `MaxAttackRange` && line of sight. |
| `URogueBTTask_RangedAttack` | Task | Spawns `ProjectileClass` from the muzzle socket toward the target, with random spread. |
| `URogueBTTask_Heal` | Task | `ApplyHealthChange(+HealAmount)` on its own pawn. |
| `URogueUBTDecorator_IsLowHealth` | Decorator | Health % ≤ `LowHealthThreshold` gates the flee/heal branch. |
| `URogueEnvQueryContext_TargetActor` | EQS context | Supplies the Blackboard `TargetActor` to EQS queries. |

EQS assets: `EQS_MoveToTarget` (attack positioning) and `EQS_MoveToSafePlace` (flee).
Shared constants live in `Public/RogueGameTypes.h` (`NAME_TargetActor`, custom collision channel macros).

## 5. Development workflow
- Feature branches (`Feature/HealthPotion`, `Feature/GameFeel`, `AI/EnemyMonsters`…) merged into `master` via PRs.
- Commit prefixes: `[Add]`, `[Fix]`, `[Ref]`, `[Update]`, plus "Assignment N" for course assignments.
- Blueprint screenshots are kept in `BlueprintScreenshots/`.

## 6. Observations & improvement opportunities

**Consistency**
- Attribute lookup is done three different ways: `GetAttributesComponent()`, `FindComponentByClass`, and `Cast<>(GetComponentByClass(...))`. Standardizing on the static helper would help.
- Pointers are a mix of raw `UPROPERTY` pointers and `TObjectPtr`. UE5 recommends `TObjectPtr` for members.
- Constants are `#define` macros. A `static const FName` or a namespace would be more idiomatic.
- Three naming prefixes coexist (see §2).
- Some headers sit in `Private/` while others are in `Public/`.

**Likely bugs / risks**
- `ASProjectileBase::OnComponentHit`: `if (ensure(IsPendingKillPending())) return;` fires the ensure on every normal hit. The intent was probably `if (IsPendingKillPending()) return;`.
- `ASPlayerController::OnUnPossess` reads `GetPawn()` *after* `Super::OnUnPossess()`, which has already cleared the pawn, so the delegate is never removed. `HandleOnPawnDeath` also calls `OnUnPossess()` directly instead of `UnPossess()`.
- `ARogueAIController::BeginPlay` uses `check(PlayerPawn)`, which crashes if there is no player pawn yet.
- `ApplyHealthChange` always broadcasts a `nullptr` instigator, so listeners can't know who dealt the damage.
- Leftover state from before the decorator refactor: `ARogueAIController::LowHealthKey/bLowHealth` and `ARogueAICharacter::bIsLowHealth` are unused.

**Performance / scope**
- Most actors and components keep `bCanEverTick = true` with empty `Tick`. They could disable ticking.
- The code assumes single-player throughout (`GetPlayerPawn(this,0)`, `GetFirstPlayerController`) and has no replication.
- Debug draws (`DrawDebugLine/Sphere`) run unconditionally in shipping code paths.
