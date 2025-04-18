// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ABHpBarWidget.h"
#include "Components/ProgressBar.h"
#include "Interface/ABCharacterWidgetInterface.h"

UABHpBarWidget::UABHpBarWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// �Ϻη� ��ȿ���� ���� ���� �����ϱ� ���� �� ����.
	MaxHp = -1.0f;
}

void UABHpBarWidget::UpdateHpBar(float NewCurrentHp)
{
	// MaxHp ���� ����� �����ƴ��� Ȯ��.
	ensure(MaxHp > 0.0f);

	// ���α׷��� �� ���� ������Ʈ.
	if (HpProgressBar)
	{
		HpProgressBar->SetPercent(NewCurrentHp / MaxHp);
	}
}

void UABHpBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// �� �Լ��� ȣ��Ǹ�, UI�� ���� ��κ��� ���� �ʱ�ȭ �Ǿ��ٰ� ������ �� ����.
	// ���� ���� ������ ���� �̸����� �̻�.
	HpProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PbHpBar")));
	ensure(HpProgressBar);

	// ĳ���Ϳ� �� ����(����)�� ����.
	// �������� ���ϱ� ���� �������̽��� ���� ��ȸ�� ���� (������ ����).
	IABCharacterWidgetInterface* CharacterWidget = Cast<IABCharacterWidgetInterface>(OwningActor);
	if (CharacterWidget)
	{
		// �������̽��� ���� ĳ���Ϳ� �� ����(����) ����.
		CharacterWidget->SetupCharacterWidget(this);
	}

}
