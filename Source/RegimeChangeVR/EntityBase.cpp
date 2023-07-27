// Fill out your copyright notice in the Description page of Project Settings.


#include "EntityBase.h"

// Sets default values
AEntityBase::AEntityBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(FName("DigiHealthComponent"));
}

// Called when the game starts or when spawned
void AEntityBase::BeginPlay()
{
	Super::BeginPlay();
	HealthComponent->EventOnDied.AddUniqueDynamic(this, &AEntityBase::OnDiedEvent);
}

// Called every frame
void AEntityBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (HealthComponent->bIsAlive)
	{
		UpdateHitBonesInterpolation();
	}
}

void AEntityBase::AddHitBone(FName HitBoneName, FVector Impulse)
{
	const bool bLeftLeg = HitBoneName == FName("DEF_LegJoint_L_001") || HitBoneName == FName("DEF_LegJoint_L_002") || HitBoneName == FName("DEF_LegJoint_L_003") || HitBoneName == FName("DEF_LegJoint_L_004");
	const bool bRightLeg = HitBoneName == FName("DEF_LegJoint_R_001") || HitBoneName == FName("DEF_LegJoint_R_002") || HitBoneName == FName("DEF_LegJoint_R_003") || HitBoneName == FName("DEF_LegJoint_R_004");
	const bool bRightArm = HitBoneName == FName("DEF_Arm_R_001") || HitBoneName == FName("DEF_Arm_R_002");
	const bool bLeftArm = HitBoneName == FName("DEF_Arm_L_001") || HitBoneName == FName("DEF_Arm_L_002");
	const bool bNeck = HitBoneName == FName("DEF_Neck_001") || HitBoneName == FName("DEF_Neck_002") || HitBoneName == FName("DEF_Spine_002");

	const bool bBoneCanHaveImpulseAppliedWithoutRuiningCharactersOrientation =
		HitBoneName != FName("DEF_Spine_001") && HitBoneName != FName("DEF_Spine_002") &&
		HitBoneName != FName("DEF_PelvisBase_001") && HitBoneName != FName("ExoSkeleton");

	if (bNeck || bLeftLeg || bRightLeg || bRightArm || bLeftArm) // LOL
	{
		if (HitBonesData.Contains(HitBoneName)) 
		{
			return;
		}
		else
		{
			HitBonesData.Add(TTuple<FName, float>(HitBoneName, 1.0f));
			GetMesh()->SetAllBodiesBelowSimulatePhysics(HitBoneName, true, true);
			GetMesh()->SetAllBodiesBelowPhysicsBlendWeight(HitBoneName, 1.0f, false, true);
			GetMesh()->SetAllBodiesBelowLinearVelocity(HitBoneName, FVector(0, 0, 0), true);
			GetMesh()->SetAllPhysicsAngularVelocityInDegrees(FVector(0, 0, 0));
			GetMesh()->AddImpulseToAllBodiesBelow(Impulse, HitBoneName, false, true);
		}
	}

	
}

void AEntityBase::UpdateHitBonesInterpolation()
{
	for (TTuple<FName, float>& KVP : HitBonesData)
	{
		if (KVP.Value > 0.0f)
		{
			KVP.Value -= 0.01f;
			GetMesh()->SetAllBodiesBelowPhysicsBlendWeight(KVP.Key, KVP.Value, false, true);
		}
		else
		{
			GetMesh()->SetAllBodiesBelowSimulatePhysics(KVP.Key, false, false);
			NextBonesToClear.Add(KVP.Key);

			//FTimerDelegate Timer;
			//Timer.BindUFunction(this, FName("ClearHitBone"), EVariantTypes::Custom);
			//GetWorld()->GetTimerManager().SetTimerForNextTick(Timer);
		}
	}
	ClearHitBone();
}

void AEntityBase::ClearHitBone()
{
	if (NextBonesToClear.Num() > 0)
	{
		for (FName& Bone : NextBonesToClear)
		{
			HitBonesData.Remove(Bone);
		}
		NextBonesToClear.Empty();
	}
}

void AEntityBase::OnDiedEvent()
{
	GetMesh()->SetAllBodiesCollisionObjectType(ECollisionChannel::ECC_PhysicsBody);
	GetMesh()->SetAllBodiesSimulatePhysics(true);
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetAllBodiesPhysicsBlendWeight(1.0f);
}

// Called to bind functionality to input
void AEntityBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

