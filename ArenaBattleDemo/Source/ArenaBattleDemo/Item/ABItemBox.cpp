// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ABItemBox.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Physics/ABCollision.h"
#include "Interface/ABCharacterItemInterface.h"
#include "Engine/AssetManager.h"
#include "ABItemData.h"

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

	//// 트리거가 발생하는 다이나믹 델리게이트에 함수 등록.
	//Trigger->OnComponentBeginOverlap.AddDynamic(this, &AABItemBox::OnOverlapBegin);

	Mesh->SetCollisionProfileName(TEXT("NoCollision"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BoxMeshRef(TEXT("/Game/ArenaBattle/Environment/Props/SM_Env_Breakables_Box1.SM_Env_Breakables_Box1"));
	if (BoxMeshRef.Object)
	{
		Mesh->SetStaticMesh(BoxMeshRef.Object);
	}
	// 매시 컴포넌트 위치 조정.
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

void AABItemBox::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// PrimaryAssetId 목록을 가져오기 위해 애셋 매니저 싱글톤 참조.
	UAssetManager& Manager = UAssetManager::Get();

	// 애셋 목록 가져오기.
	TArray<FPrimaryAssetId> Assets;
	Manager.GetPrimaryAssetIdList(TEXT("ABItemData"), Assets);

	// 제대로 가져왔는지 검사.
	ensure(Assets.Num() > 0);

	// 랜덤으로 인덱스 선택.
	int32 RandomIndex = FMath::RandRange(0, Assets.Num() - 1);
	
	UE_LOG(LogTemp, Log, TEXT("Assets.Num() : %d"), Assets.Num());

	// 선택된 인덱스를 사용해 애셋 참조.
	FSoftObjectPtr AssetPtr(Manager.GetPrimaryAssetPath(Assets[RandomIndex]));

	// 애셋이 로드가 안된 경우 로드.
	if (AssetPtr.IsPending())
	{
		AssetPtr.LoadSynchronous();
	}

	// 로드한 애셋을 아이템으로 설정.
	Item = Cast<UABItemData>(AssetPtr.Get());

	// 제대로 설정됐는지 확인.
	ensure(Item);

	// 트리거가 발생하는 다이나믹 델리게이트에 함수 등록.
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AABItemBox::OnOverlapBegin);

}

void AABItemBox::OnOverlapBegin(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{

	// 꽝 상자도 있다고 가정.
	// Item이 nullptr이면 꽝.
	if (!Item)
	{
		Destroy();
		return;
	}

	// 아이템이 있으면 
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


