// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "ConstraintType.h"
#include "UConstraintComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UCONSTRAINTS_API UConstraintComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UConstraintComponent();

        virtual void OnComponentCreated() override;
        virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;

        UFUNCTION(BlueprintCallable)
        virtual void Init(UPhysicsConstraintComponent* InFixedConstraint,
                          UPhysicsConstraintComponent* InModularConstraint,
                          UObject* InPrimaryTrigger,
                          UObject* InTrigger);

        // Called when the game starts
	virtual void BeginPlay() override;

protected:
        void SetConstraints(UPhysicsConstraintComponent* InFixedConstraint,
                            UPhysicsConstraintComponent* InModularConstraint);
public:
        UPROPERTY(BlueprintReadWrite, EditAnywhere)
          UPhysicsConstraintComponent* ModularConstraint;

        UPROPERTY(BlueprintReadWrite, EditAnywhere)
          UPhysicsConstraintComponent* FixedConstraint;

        UPROPERTY(BlueprintReadWrite, EditAnywhere, Instanced)
          UConstraintType* ConstraintType;

        UPROPERTY(BlueprintReadWrite)
          UObject* PrimaryTrigger;

        UPROPERTY(BlueprintReadWrite)
          UObject* Trigger;

        UPROPERTY(EditAnywhere)
          bool bFixedConstraintBreakable = true;

        UPROPERTY(EditAnywhere)
          float FixedConstraintBreakLimit = 10000.;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UCONSTRAINTS_API UBoxTriggerConstraintComponent : public UConstraintComponent
{
  GENERATED_BODY()

public:

    UBoxTriggerConstraintComponent();

  virtual void OnComponentCreated() override;
  virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;

  // virtual void Init(UPhysicsConstraintComponent* InFixedConstraint,
  //                   UPhysicsConstraintComponent* InModularConstraint,
  //                   UObject* InTrigger);
private:

  UPROPERTY()
    UBoxComponent* BoxComponent;
};
