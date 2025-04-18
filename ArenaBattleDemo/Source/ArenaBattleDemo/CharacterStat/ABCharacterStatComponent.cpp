// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/ABCharacterStatComponent.h"
#include "ABCharacterStatComponent.h"

// Sets default values for this component's properties
UABCharacterStatComponent::UABCharacterStatComponent()
{
	MaxHp = 200.0f;
	CurrentHp = MaxHp;
}


// Called when the game starts
void UABCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ������ ���۵Ǹ� ü���� ���� ä���.
	SetHp(MaxHp);
}

float UABCharacterStatComponent::ApplyDamage(float InDamage)
{
	// ������ ó��.
	const float PrevHp = CurrentHp;

	// ������ ���� ��.
	// ������ ���� ���޵� �������� ������ �� ����.
	// ������ ���� 0 ����.
	//const float ActualDamage = FMath::Clamp<float>(InDamage, 0, InDamage);
	const float ActualDamage = FMath::Max<float>(0.0f, InDamage);


	// ������ ���.
	//CurrentHp = FMath::Clamp<float>(PrevHp - ActualDamage, 0.0f, MaxHp);
	SetHp(PrevHp - ActualDamage);

	// �׾����� (ü���� ��� �����ߴ���) Ȯ��.
	if (CurrentHp <= KINDA_SMALL_NUMBER)
	{
		// �̺�Ʈ ����.
		OnHpZero.Broadcast();
	}

	return ActualDamage;
}

void UABCharacterStatComponent::SetHp(float NewHp)
{
	// ���� ü�� ������Ʈ.
	CurrentHp = FMath::Clamp<float>(NewHp, 0.0f, MaxHp);

	// ü�� ���� �̺�Ʈ ����.
	OnHpChanged.Broadcast(CurrentHp);
}



