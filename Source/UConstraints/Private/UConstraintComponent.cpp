// Fill out your copyright notice in the Description page of Project Settings.

#include "UConstraintComponent.h"


// Sets default values for this component's properties
UConstraintComponent::UConstraintComponent()
{
  // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
  // off to improve performance if you don't need them.
  PrimaryComponentTick.bCanEverTick = true;
  // ModularConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(FName(GetName() + TEXT("_ModularConstraint")));
  // FixedConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(FName(GetName() + TEXT("_FixedConstraint")));

  // FixedConstraint->ResetRelativeTransform();
  // ModularConstraint->ResetRelativeTransform();

  // ...
}

void UConstraintComponent::Init(UPhysicsConstraintComponent* InFixedConstraint,
                                UPhysicsConstraintComponent* InModularConstraint,
                                UObject* InPrimaryTrigger,
                                UObject* InTrigger)
{
  if(UPrimitiveComponent* Temp = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent()))
    {
     Temp->SetGenerateOverlapEvents(true);
    }
  Trigger = InTrigger;
  SetConstraints(InFixedConstraint, InModularConstraint);
}

void UConstraintComponent::SetConstraints(UPhysicsConstraintComponent* InFixedConstraint,
                                          UPhysicsConstraintComponent* InModularConstraint)
{
  FixedConstraint = InFixedConstraint;
  if(FixedConstraint)
    {
      FixedConstraint->ConstraintInstance.SetLinearXLimit(ELinearConstraintMotion::LCM_Locked, 0);
      FixedConstraint->ConstraintInstance.SetLinearYLimit(ELinearConstraintMotion::LCM_Locked, 0);
      FixedConstraint->ConstraintInstance.SetLinearZLimit(ELinearConstraintMotion::LCM_Locked, 0);
      FixedConstraint->ConstraintInstance.SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 0);
      FixedConstraint->ConstraintInstance.SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0);
      FixedConstraint->ConstraintInstance.SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 0);
      FixedConstraint->SetLinearBreakable(bFixedConstraintBreakable, FixedConstraintBreakLimit);
    }
  ModularConstraint = InModularConstraint;
}

void UConstraintComponent::OnComponentCreated()
{
  Super::OnComponentCreated();
}

void UConstraintComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
  Super::OnComponentDestroyed(bDestroyingHierarchy);
}

// Called when the game starts
void UConstraintComponent::BeginPlay()
{
  Super::BeginPlay();

  if(ConstraintType && Trigger)
    {
      ConstraintType->SetConstraints(FixedConstraint, ModularConstraint);
      ConstraintType->SetupPrimaryCondition(PrimaryTrigger);
      ConstraintType->SetupSecondaryCondition(Trigger);
    }
  else
    {
      UE_LOG(LogTemp, Error, TEXT("[%s]: ConstraintType or Trigger missing"));
    }
}


// Called every frame
void UConstraintComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
  Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

  // ...
}

UBoxTriggerConstraintComponent::UBoxTriggerConstraintComponent()
{
  // BoxComponent = CreateDefaultSubobject<UBoxComponent>(FName(GetName() + TEXT("_BoxComponent")));
  // Trigger = BoxComponent;
}

void UBoxTriggerConstraintComponent::OnComponentCreated()
{
  Super::OnComponentCreated();
  // BoxComponent->SetupAttachment(GetOwner()->GetRootComponent());
}

void UBoxTriggerConstraintComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
  Super::OnComponentDestroyed(bDestroyingHierarchy);
  // if(BoxComponent)
  //   {
  //     if(BoxComponent->HasBeenCreated())
  //       {
  //         BoxComponent->DestroyComponent(false);
  //       }
  //   }
}

// void UBoxTriggerConstraintComponent::Init(UPhysicsConstraintComponent* InFixedConstraint,
//                                           UPhysicsConstraintComponent* InModularConstraint,
//                                           UObject* InTrigger)
// {
//   Super::Init(InFixedConstraint, InModularConstraint, InTrigger);
// }
