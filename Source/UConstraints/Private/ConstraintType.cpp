// Fill out your copyright notice in the Description page of Project Settings.


#include "ConstraintType.h"
#include "Engine/StaticMeshActor.h"
#include "UConstraintComponent.h"

void UConstraintType::SetConstraints(UPhysicsConstraintComponent* InFixedConstraint,
                                     UPhysicsConstraintComponent* InModularConstraint)
{
  FixedConstraint = InFixedConstraint;
  ModularConstraint = InModularConstraint;

}

UNeedleConstraint::UNeedleConstraint()
{
  // BoxComponent = CreateDefaultSubobject<UBoxComponent>(FName(GetName() + TEXT("_BoxComponent")));
  // UObject* TempOuter = GetOuter();
  // if(TempOuter)
  //   {
  //     UActorComponent* Constraint = Cast<UActorComponent>(TempOuter);
  //     Owner = Cast<UConstraintComponent>(Constraint);
  //     if (Constraint && Owner)
  //       {
  //         UE_LOG(LogTemp, Error, TEXT("Constructor NeedleConstraint Name: %s"),  *Constraint->GetOwner()->GetName());
  //         ActorMesh = Cast<UPrimitiveComponent>(Constraint->GetOwner()->GetRootComponent());
  //         if(ActorMesh)
  //           {
  //             // BoxComponent->SetupAttachment(Cast<USceneComponent>(ActorMesh));
  //             BoxComponent->SetupAttachment(ActorMesh);
  //           }



// #if WITH_EDITOR
//           GEditor->EditorUpdateComponents();
// #endif // WITH_EDITOR
    //     }
    // }
}


void UNeedleConstraint::OnPrimaryAreaBeginOverlap(class UPrimitiveComponent* HitComp,
                                                  class AActor* OtherActor,
                                                  class UPrimitiveComponent* OtherComp,
                                                  int32 OtherBodyIndex,
                                                  bool bFromSweep,
                                                  const FHitResult& SweepResult)
{
  UE_LOG(LogTemp, Error, TEXT("PrimaryOverlapBegin with %s, HitComp %s, OtherComp %s"), *OtherActor->GetName(), *HitComp->GetName(), *OtherComp->GetName());

  if(OtherComp == MeshComponent->GetOwner()->GetRootComponent())
    {
      UE_LOG(LogTemp, Error, TEXT("Overlap of %s with %s has same root comp %s"), *OtherComp->GetName(), *HitComp->GetName(), *MeshComponent->GetOwner()->GetRootComponent()->GetName());
    }


  if(ModularConstraint && MeshComponent)
    {
      if(OtherComp == MeshComponent->GetOwner()->GetRootComponent())
        {
          return;
        }
      bConnected = true;
      ConnectedComp = OtherComp;

      ModularConstraint->ConstraintActor1 = MeshComponent->GetOwner();
      ModularConstraint->ConstraintActor2 = OtherActor;
      ModularConstraint->SetConstrainedComponents(MeshComponent,
                                                  NAME_None,
                                                  OtherComp,
                                                  NAME_None);
    }
}

void UNeedleConstraint::OnPrimaryAreaEndOverlap(class UPrimitiveComponent* HitComp,
                                                class AActor* OtherActor,
                                                class UPrimitiveComponent* OtherComp,
                                                int32 OtherBodyIndex)
{
  UE_LOG(LogTemp, Error, TEXT("PrimaryOverlapEnd HitComp %s, OtherComp %s"), *HitComp->GetName(), *OtherComp->GetName());
  if(ModularConstraint && (OtherComp == ConnectedComp)  && bConnected)
  {

    bConnected = false;
    ModularConstraint->BreakConstraint();
    FConstrainComponentPropName Name;
    Name.ComponentName = FName(TEXT("None"));
    ModularConstraint->ComponentName1 = Name;
    ModularConstraint->ComponentName2 = Name;
    ModularConstraint->ConstraintActor2 = nullptr;
  }
}

void UNeedleConstraint::OnSecondaryAreaBeginOverlap(class UPrimitiveComponent* HitComp,
                                                    class AActor* OtherActor,
                                                    class UPrimitiveComponent* OtherComp,
                                                    int32 OtherBodyIndex,
                                                    bool bFromSweep,
                                                    const FHitResult& SweepResult)
{
  UE_LOG(LogTemp, Error, TEXT("SecondaryOverlapBegin with %s"), *OtherActor->GetName());

  if(FixedConstraint && MeshComponent)
    {
      if(OtherComp == MeshComponent->GetOwner()->GetRootComponent())
        {
          return;
        }

      if(bConnected && (OtherComp != ConnectedComp))
        {
          UE_LOG(LogTemp, Error, TEXT("[%s] Already connected to %s. Cannot connect to %s"), *FString(__FUNCTION__), *ConnectedComp->GetName(),*OtherComp->GetName());
          return;
        }

      FixedConstraint->ConstraintActor1 = MeshComponent->GetOwner();
      FixedConstraint->ConstraintActor2 = OtherActor;
      FixedConstraint->SetConstrainedComponents(MeshComponent,
                                                NAME_None,
                                                OtherComp,
                                                NAME_None);

    }
};

        // Function called when an item leaves the fixation overlap area
void UNeedleConstraint::OnSecondaryAreaEndOverlap(class UPrimitiveComponent* HitComp,
                                                  class AActor* OtherActor,
                                                  class UPrimitiveComponent* OtherComp,
                                                  int32 OtherBodyIndex)
{

  UE_LOG(LogTemp, Error, TEXT("SecondaryOverlapEnd"));
  if(FixedConstraint)
    {
      if(bConnected && (OtherComp != ConnectedComp))
        {
          UE_LOG(LogTemp, Error, TEXT("[%s]  %s is not the connected comp %s. Ignore EndOverlap"), *FString(__FUNCTION__), *OtherComp->GetName(), *ConnectedComp->GetName());
          return;
        }
      FixedConstraint->BreakConstraint();
      FConstrainComponentPropName Name;
      Name.ComponentName = FName(TEXT("None"));
      FixedConstraint->ComponentName1 = Name;
      FixedConstraint->ComponentName2 = Name;
      FixedConstraint->ConstraintActor2 = nullptr;
    }

}

void UNeedleConstraint::SetupPrimaryCondition(UObject* InPrimaryTrigger)
{
  PrimaryTrigger = Cast<UPrimitiveComponent>(InPrimaryTrigger);
  if(PrimaryTrigger)
    {
      UE_LOG(LogTemp, Error, TEXT("SetupPrimarycondition"));
      PrimaryTrigger->OnComponentBeginOverlap.AddDynamic(this, &UNeedleConstraint::OnPrimaryAreaBeginOverlap);
      PrimaryTrigger->OnComponentEndOverlap.AddDynamic(this, &UNeedleConstraint::OnPrimaryAreaEndOverlap);
    }
  else
    {
      UE_LOG(LogTemp, Error, TEXT("[%s]: Trigger needs to be of Type UPrimitiveComponent got %s"), *GetName(), *PrimaryTrigger->GetClass()->GetName());
    }
}

void UNeedleConstraint::SetupSecondaryCondition(UObject* Trigger)
{

  if(!Trigger)
    {
      UE_LOG(LogTemp, Error, TEXT("[%s]: No Trigger Component set"), *GetName());
      return;
    }

  if(UPrimitiveComponent* TriggerComponent = Cast<UPrimitiveComponent>(Trigger))
    {
      UE_LOG(LogTemp, Error, TEXT("SetupSecondarycondition"));
      TriggerComponent->OnComponentBeginOverlap.AddDynamic(this, &UNeedleConstraint::OnSecondaryAreaBeginOverlap);
      TriggerComponent->OnComponentEndOverlap.AddDynamic(this, &UNeedleConstraint::OnSecondaryAreaEndOverlap);
    }
  else
    {
      UE_LOG(LogTemp, Error, TEXT("[%s]: Trigger needs to be of Type UPrimitiveComponent got %s"), *GetName(), *Trigger->GetClass()->GetName());
    }
}
