// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	Health = InitHealth;
	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	
	GetOwner()->OnTakeAnyDamage.AddUniqueDynamic(this, &UHealthComponent::OnEntityDamaged);
	// ...
	
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHealthComponent::OnDamageInflicted(float DamageAmount)
{
	float Damage = FMath::Abs(DamageAmount);

	if (Health - Damage > 0.0f)
	{
		Health -= Damage;
	}
	else
	{
		Health = 0.0f;
		bIsAlive = false;
		EventOnDied.Broadcast();
	}

	EventHealthChanged.Broadcast(Health);
}

void UHealthComponent::OnEntityDamaged(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	OnDamageInflicted(Damage);
}

void UHealthComponent::OnHealthRestored(float HealthAmount)
{
	float InHealth = FMath::Abs(HealthAmount);

	if (Health + InHealth <= MaxHealth)
	{
		Health += InHealth;
	}
	else
	{
		Health = MaxHealth;
	}

	EventHealthChanged.Broadcast(Health);
}


