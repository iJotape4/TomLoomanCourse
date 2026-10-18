#include "SharedGameplayTags.h"

namespace SharedGameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG(Action_Attack, "Action.Attack")
	UE_DEFINE_GAMEPLAY_TAG(Attribute_Health, "Attribute.Health")
	UE_DEFINE_GAMEPLAY_TAG(Attribute_HealthMax, "Attribute.HealthMax")
	UE_DEFINE_GAMEPLAY_TAG(Attribute_MoveSpeed, "Attribute.MoveSpeed")
	UE_DEFINE_GAMEPLAY_TAG(Attribute_Rage, "Attribute.Rage")
	UE_DEFINE_GAMEPLAY_TAG(Attribute_RageMax, "Attribute.RageMax")
	
	
	UE_DEFINE_GAMEPLAY_TAG(Action_MagicProjectile, "Action.Attack.MagicProjectile")
	UE_DEFINE_GAMEPLAY_TAG(Action_GravityProjectile, "Action.Attack.GravityProjectile")
	UE_DEFINE_GAMEPLAY_TAG(Action_TeleportProjectile, "Action.Attack.TeleportProjectile")
	UE_DEFINE_GAMEPLAY_TAG(Action_Sprint, "Action.Sprint")
	
	UE_DEFINE_GAMEPLAY_TAG(StatusEffect_Sprinting, "StatusEffect.Sprinting")
}
