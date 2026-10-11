// Fill out your copyright notice in the Description page of Project Settings.


#include "TomLoomanCourse/ActionSystem/URogueActionSystemComponent.h"

#include "RogueAction.h"
#include "TomLoomanCourse/SharedGameplayTags.h"

// Sets default values for this component's properties
URogueActionSystemComponent::URogueActionSystemComponent()
{
	bWantsInitializeComponent = true;
	AttributeSetClass = URogueAttributeSet::StaticClass();
}

// Called when the game starts
void URogueActionSystemComponent::BeginPlay()
{
	Super::BeginPlay();
	//Attributes.he = MaxHealth;
	OnBeginPlay.Broadcast(this);
}

void URogueActionSystemComponent::InitializeComponent()
{
	Super::InitializeComponent();
	
	Attributes = NewObject<URogueAttributeSet>(this, AttributeSetClass);
	
	for (TFieldIterator<FStructProperty> PropIt(Attributes->GetClass()); PropIt; ++PropIt)
	{
		FRogueAttribute* FoundAattribute = PropIt->ContainerPtrToValuePtr<FRogueAttribute>(Attributes);
		
		FName AtttributeTagName = FName("Attribute."+PropIt->GetName());
		FGameplayTag AttributeTag = FGameplayTag::RequestGameplayTag(AtttributeTagName);
		CachedAttributes.Add(AttributeTag, FoundAattribute);
	}
	
	for (TSubclassOf<URogueAction> ActionClass : DefaultActions)
		if (ensure(ActionClass))
			GrantAction(ActionClass);
	
}

void URogueActionSystemComponent::StartAction(FGameplayTag InActionName)
{
	for (URogueAction* Action : Actions)
	{
		if (Action && Action->GetActionName() == InActionName)
		{
			if (Action->CanStartAction())
			{
				Action->StartAction();
			}
			
			return;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Action %s not found"), *InActionName.ToString());
}

FGameplayTag URogueActionSystemComponent::GetNextAttackName(FGameplayTag Current) const
{
	TArray<FGameplayTag> Attacks;
	for (const URogueAction* Action : Actions)
	{
		const FGameplayTag Tag = Action ? Action->GetActionName() : FGameplayTag();
		if (Tag.MatchesTag(SharedGameplayTags::Action_Attack))
			Attacks.Add(Tag);
	}

	if (Attacks.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("No attack actions granted"));
		return FGameplayTag::EmptyTag;
	}

	// INDEX_NONE (-1) when Current isn't an attack, so we start at the first one
	const int32 Index = Attacks.IndexOfByKey(Current);
	return Attacks[(Index + 1) % Attacks.Num()];
}


void URogueActionSystemComponent::StopAction(FGameplayTag InActionName)
{
	for (URogueAction* Action : Actions)
	{
		if (Action && Action->GetActionName() == InActionName)
		{
			Action->StopAction();
			return;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Action %s not found"), *InActionName.ToString());
}

URogueAction* URogueActionSystemComponent::GrantAction(const TSubclassOf<URogueAction>& NewActionClass)
{
	URogueAction* NewAction = NewObject<URogueAction>(this, NewActionClass);
	Actions.Add(NewAction);
	return NewAction;
}

void URogueActionSystemComponent::ApplyAttributeChange(FGameplayTag InAttributeTag, float Delta,
	EAttributeModifyType ModifyType /*= Base*/)
{
	FRogueAttribute* FoundAttribute = GetAttribute(InAttributeTag);
	if (!ensureMsgf(FoundAttribute, TEXT("Attribute %s not found on %s"), *InAttributeTag.ToString(), *GetNameSafe(GetOwner())))
		return;
	
	float oldValue = FoundAttribute->GetValue();
	
	switch (ModifyType)
	{
		case Base:
			FoundAttribute->Base += Delta;
			break;
		case Modifier:
			FoundAttribute->Modifier += Delta;
			break;
		case OverrideBase:
			FoundAttribute->Base = Delta;
			break;
			
		default:
			check(false);
		
	}
	
	Attributes->PostAttributeChanged();
	
	if (FOnAttributeChanged* Event = AttributeListeners.Find(InAttributeTag))
	{
		Event->Broadcast(InAttributeTag, FoundAttribute->GetValue(), oldValue);
	}
	
	if (InAttributeTag == SharedGameplayTags::Attribute_Health && FoundAttribute->GetValue() <= 0.0f)
	{
		Death(this->GetOwner());
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Attribute %s modified by %f"), *InAttributeTag.ToString(), Delta);
}

FRogueAttribute* URogueActionSystemComponent::GetAttribute(FGameplayTag InAttributeTag)
{
	FRogueAttribute** FoundAttribute = CachedAttributes.Find(InAttributeTag);
	if (FoundAttribute)
	{
		return *FoundAttribute;
	}
	return nullptr;
}

FOnAttributeChanged& URogueActionSystemComponent::GetAttributeListener(FGameplayTag InAttributeTag)
{ 
	return AttributeListeners.FindOrAdd(InAttributeTag);
}

void URogueActionSystemComponent::Death(AActor* InstigatorActor)
{
	if (!bIsAlive) return;
	
	OnDeath.Broadcast(InstigatorActor);
	UE_LOG(LogTemp, Warning, TEXT("Health of Actor %s has reached Zero"), *GetOwner()->GetActorLabel());
	bIsAlive = false;
}

bool URogueActionSystemComponent::IsAlive() const
{
	return bIsAlive;
}

URogueActionSystemComponent* URogueActionSystemComponent::GetAttributesComponent(AActor* FromActor)
{
	return FromActor ? FromActor->FindComponentByClass<URogueActionSystemComponent>() : nullptr;
}