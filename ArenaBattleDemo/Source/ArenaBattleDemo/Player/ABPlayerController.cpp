// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ABPlayerController.h"

void AABPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// ������ �� ���콺�� ����Ʈ�� �ԷµǾ� �ٷ� ����ǵ��� ����.
	FInputModeGameOnly GameInput;
	// �Է� ��� ����.
	// ����ü�� �־��ش�. FInputModeDataBase�� ����� ����ü.
	SetInputMode(GameInput);
}
