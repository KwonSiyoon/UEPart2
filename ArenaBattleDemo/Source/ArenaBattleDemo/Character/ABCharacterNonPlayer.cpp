// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ABCharacterNonPlayer.h"

AABCharacterNonPlayer::AABCharacterNonPlayer()
{
}

void AABCharacterNonPlayer::SetDead()
{
	Super::SetDead();

	// Ÿ�̸Ӹ� ����� ���� ����.
	FTimerHandle DeadTimerHandle;

	// Ÿ�̸� ����.
	GetWorld()->GetTimerManager().SetTimer(
		DeadTimerHandle, 
		FTimerDelegate::CreateLambda([&]() 
			{
				// ���� ����.
				Destroy();
			}
		),
		DeadEventDelayTime,		// Ÿ�̸� ���� �ð�.
		false);		// �ݺ� ���� ����(�ݺ� ����).

}
