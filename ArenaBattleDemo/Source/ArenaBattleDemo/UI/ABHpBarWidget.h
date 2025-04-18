// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ABUserWidget.h"
#include "ABHpBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLEDEMO_API UABHpBarWidget : public UABUserWidget
{
	GENERATED_BODY()
	
public:
	UABHpBarWidget(const FObjectInitializer& ObjectInitializer);
	
	// �ִ� ü�� ���� �Լ�.
	FORCEINLINE void SetMaxHp(float NewMaxHp) { MaxHp = NewMaxHp; }

	// HpBar�� �ۼ�Ʈ�� ������ �� ȣ���� �Լ�.
	void UpdateHpBar(float NewCurrentHp);

protected:
	// UMG�� �ʱ�ȭ�� �� ȣ��Ǵ� �Լ�.
	virtual void NativeConstruct() override;

protected:
	// HP �������� �����ֱ� ���� ����� ���α׷����� ���� ����.
	UPROPERTY()
	TObjectPtr<class UProgressBar> HpProgressBar;

	// �ִ� ü�� ��.
	UPROPERTY()
	float MaxHp;

};
