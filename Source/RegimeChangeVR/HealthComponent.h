// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDiedEvent);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class REGIMECHANGEVR_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float Health;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float InitHealth = 80.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float MaxHealth = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsAlive = true;

	UFUNCTION()
	void OnDamageInflicted(float DamageAmount);

	UFUNCTION()
	void OnEntityDamaged(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION(BlueprintCallable)
	void OnHealthRestored(float HealthAmount);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE bool GetIsAlive() const { return bIsAlive; }

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintReadWrite)
	FOnDiedEvent EventOnDied;
};
