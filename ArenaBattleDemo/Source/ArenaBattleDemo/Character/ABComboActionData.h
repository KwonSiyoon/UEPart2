// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ABComboActionData.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLEDEMO_API UABComboActionData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UABComboActionData();

public:
	// ��Ÿ�� ���� �̸� ���λ� (ComboAttack).
	UPROPERTY(EditAnywhere, Category = Name)
	FString MontageSectionNamePrefix;

	// �ִ� �޺� ���� (4��)
	UPROPERTY(EditAnywhere, Category = ComboData)
	uint8 MaxComboCount;

	// ������ ��� �ӵ�.
	// �ִϸ��̼� �ּ��� �⺻ ��� �ӵ� ���� ������.
	UPROPERTY(EditAnywhere, Category = ComboData)
	float FrameRate;

	// �Է��� ������ ���Դ����� Ȯ���ϴ� ������.
	// ���� �ִϸ��̼� �ּ� ���� ����.
	UPROPERTY(EditAnywhere, Category = ComboData)
	TArray<float> EffectiveFrameCount;

};
