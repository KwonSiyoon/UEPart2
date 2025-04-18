// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ABWidgetComponent.h"
#include "ABUserWidget.h"

void UABWidgetComponent::InitWidget()
{
	// InitWodget �Լ��� ȣ��Ǹ� WidgetClass ������ �������� ���� �ν��Ͻ��� ������.
	// ���ο��� CreateWidget �Լ��� ����.
	Super::InitWidget();

	// Super::InitWidget(); ȣ�� ���Ŀ��� ���� �ν��Ͻ��� �ִٰ� Ȯ���� �� ����.
	UABUserWidget* ABUserWidget = Cast<UABUserWidget>(GetWidget());
	if (ABUserWidget)
	{
		ABUserWidget->SetOwningActor(GetOwner());
	}


}
