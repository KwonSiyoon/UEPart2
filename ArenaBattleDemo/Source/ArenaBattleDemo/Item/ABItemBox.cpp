// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ABItemBox.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Physics/ABCollision.h"
#include "Interface/ABCharacterItemInterface.h"

// Sets default values
AABItemBox::AABItemBox()
{
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Effect"));

	RootComponent = Trigger;
	Mesh->SetupAttachment(Trigger);
	Effect->SetupAttachment(Trigger);

	Trigger->SetCollisionProfileName(CPROFILE_ABTRIGGER);

	Trigger->SetBoxExtent(FVector(40.0f, 42.0f, 30.0f));

	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AABItemBox::OnOverlapBegin);

	Mesh->SetCollisionProfileName(TEXT("NoCollision"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BoxMeshRef(TEXT("/Game/ArenaBattle/Environment/Props/SM_Env_Breakables_Box1.SM_Env_Breakables_Box1"));
	if (BoxMeshRef.Object)
	{
		Mesh->SetStaticMesh(BoxMeshRef.Object);
	}
	Mesh->AddRelativeLocation(FVector(0.0f, -3.5f, -30.0f));

	static ConstructorHelpers::FObjectFinder<UParticleSystem> EffectRef(TEXT("/Game/ArenaBattle/Effect/P_TreasureChest_Open_Mesh.P_TreasureChest_Open_Mesh"));
	if (EffectRef.Object)
	{
		Effect->SetTemplate(EffectRef.Object);

		Effect->bAutoActivate = false;
	}

	//Trigger->OnComponentBeginOverlap
	//Effect->OnSystemFinished

}

void AABItemBox::OnOverlapBegin(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (!Item)
	{
		Destroy();
		return;
	}

	IABCharacterItemInterface* OvelappedPawn
		= Cast<IABCharacterItemInterface>(OtherActor);
	if (OvelappedPawn)
	{
		OvelappedPawn->TakeItem(Item);
	}

	Effect->Activate();

	Mesh->SetHiddenInGame(true);

	SetActorEnableCollision(false);

	Effect->OnSystemFinished.AddDynamic(this, &AABItemBox::OnEffectFinished);
}

void AABItemBox::OnEffectFinished(UParticleSystemComponent* PSystem)
{
	Destroy();
}


