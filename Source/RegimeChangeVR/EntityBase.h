// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HealthComponent.h"
#include "EntityBase.generated.h"

UCLASS()
class REGIMECHANGEVR_API AEntityBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEntityBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TMap <FName, float> HitBonesData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TSet<FName> NextBonesToClear;

	UFUNCTION(BlueprintCallable)
	void AddHitBone(FName HitBoneName, FVector Impulse);

	UFUNCTION()
	void UpdateHitBonesInterpolation();

	UFUNCTION(BlueprintCallable)
	void ClearHitBone();

	UFUNCTION(BlueprintCallable)
		void OnDiedEvent();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
