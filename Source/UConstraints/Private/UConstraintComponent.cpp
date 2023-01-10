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

  UE_LOG(LogTemp, Error, TEXT("[%s] %s: CallInit"), *FString(__FUNCTION__), *GetName());
  if(UPrimitiveComponent* Temp = Cast<UPrimitiveComponent>(InPrimaryTrigger))
    {
     Temp->SetGenerateOverlapEvents(true);
    }
  if(UPrimitiveComponent* Temp = Cast<UPrimitiveComponent>(InTrigger))
    {
     Temp->SetGenerateOverlapEvents(true);
    }
  PrimaryTrigger = InPrimaryTrigger;
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
  UE_LOG(LogTemp, Error, TEXT("[%s] %s: Component Created"), *FString(__FUNCTION__), *GetName());
}

void UConstraintComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
  Super::OnComponentDestroyed(bDestroyingHierarchy);
}

// Called when the game starts
void UConstraintComponent::BeginPlay()
{
  Super::BeginPlay();

  UE_LOG(LogTemp, Error, TEXT("[%s] %s: BeginPlay"), *FString(__FUNCTION__), *GetName());
  if(ConstraintType && Trigger)
    {
      ConstraintType->MeshComponent = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
      ConstraintType->SetConstraints(FixedConstraint, ModularConstraint);
      ConstraintType->SetupPrimaryCondition(PrimaryTrigger);
      ConstraintType->SetupSecondaryCondition(Trigger);
    }
  else
    {
      UE_LOG(LogTemp, Error, TEXT("[%s] %s: ConstraintType or Trigger missing"), *FString(__FUNCTION__), *GetName());
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
