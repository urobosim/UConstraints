// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Components/BoxComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "ConstraintType.generated.h"

/**
 *
 */
UCLASS(Abstract, editinlinenew)
class UCONSTRAINTS_API UConstraintType : public UObject
{
	GENERATED_BODY()

public:

          virtual void SetupPrimaryCondition(UObject* PrimaryTrigger){};
          virtual void SetupSecondaryCondition(UObject* SecundaryTrigger){};

          void SetConstraints(UPhysicsConstraintComponent* InFixedConstraint,
                              UPhysicsConstraintComponent* InModularConstraint);


          UPROPERTY()
            UPrimitiveComponent* MeshComponent = nullptr;

protected:

        UPROPERTY()
          UPhysicsConstraintComponent* ModularConstraint = nullptr;

        UPROPERTY()
          UPhysicsConstraintComponent* FixedConstraint = nullptr;

};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), DefaultToInstanced)
class UCONSTRAINTS_API  UNeedleConstraint: public UConstraintType
{

	GENERATED_BODY()

public:

          UNeedleConstraint();

          // UPROPERTY(EditAnywhere)
          // UBoxComponent* BoxComponent;

        virtual void SetupPrimaryCondition(UObject* InPrimaryTrigger) override;
        virtual void SetupSecondaryCondition(UObject* SecundaryTrigger) override;
 protected:

          UPROPERTY()
            UPrimitiveComponent* PrimaryTrigger = nullptr;

          UPROPERTY(EditAnywhere)
            bool bConnected = false;

          UPROPERTY()
            UPrimitiveComponent* ConnectedComp = nullptr;

	UFUNCTION()
	virtual void OnPrimaryAreaBeginOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor,
	                                             class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	                                             bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnPrimaryAreaEndOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor,
	                                           class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	virtual void OnSecondaryAreaBeginOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor,
	                                             class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                               bool bFromSweep, const FHitResult& SweepResult);

	// Function called when an item leaves the fixation overlap area
	UFUNCTION()
	virtual void OnSecondaryAreaEndOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor,
                                             class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
        UPROPERTY()
          UStaticMeshComponent* OtherComponent = nullptr;
};
