// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ABCharacterStatComponent.generated.h"

// ��������Ʈ ����.
// ü�°��� 0�� �Ǿ��� �� ������ ��������Ʈ.
DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);

// ü�� ������ �߻��� �� ������ ��������Ʈ.
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChangedDelegate, float /*CurrentHp*/);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARENABATTLEDEMO_API UABCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UABCharacterStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:		// Getter.
	FORCEINLINE float GetMaxHp() { return MaxHp; }
	FORCEINLINE float GetCurrentHp() { return CurrentHp; }
	
	// ����� ���� �Լ�.
	float ApplyDamage(float InDamage);

protected:
	// Hp�� ������� �� ������ �Լ�.
	void SetHp(float NewHp);

public:
	// ü���� ��� �������� �� ����Ǵ� ��������Ʈ.
	FOnHpZeroDelegate OnHpZero;

	// ü�� ���� ��������Ʈ.
	FOnHpChangedDelegate OnHpChanged;

protected:	// ����.
	// �ִ� ü�� ��.
	UPROPERTY(VisibleInstanceOnly, Category = Stat)
	float MaxHp;

	// ���� ü�� ��.
	// Transient: ���� ü�� ���� ������ ������ ������ �ٲ�� ��.
	// ���� ��ũ�� ��������� ������ �ʿ䰡 ���� �� ����.
	// �̷� ���� Transient�� ���� ����.
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentHp;

};
