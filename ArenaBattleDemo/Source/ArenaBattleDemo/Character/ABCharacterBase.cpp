// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ABCharacterBase.h"
#include "ABCharacterControlData.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ABComboActionData.h"

// Sets default values
AABCharacterBase::AABCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UABCharacterControlData> ShoulderDataRef(TEXT("/Game/ArenaBattle/CharacterControl/ABC_Shouler.ABC_Shouler"));
	if (ShoulderDataRef.Object)
	{
		CharacterControlManager.Add(ECharacterControlType::Shoulder, ShoulderDataRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UABCharacterControlData> QuarterDataRef(TEXT("/Game/ArenaBattle/CharacterControl/ABC_Quater.ABC_Quater"));
	if (QuarterDataRef.Object)
	{
		CharacterControlManager.Add(ECharacterControlType::Quarter, QuarterDataRef.Object);
	}

}

void AABCharacterBase::SetCharacterControlData(const UABCharacterControlData* InCharacterControlData)
{
	// Pawn.
	bUseControllerRotationYaw = InCharacterControlData->bUseControllerRotationYaw;

	// CharacterMovement.
	GetCharacterMovement()->bOrientRotationToMovement = InCharacterControlData->bOrientRotationToMovement;
	GetCharacterMovement()->bUseControllerDesiredRotation = InCharacterControlData->bUseControllerDesiredRotation;
	GetCharacterMovement()->RotationRate = InCharacterControlData->RotationRate;

}

void AABCharacterBase::ProcessComboCommand()
{
	// ���� ��� ���� �޺� Ȯ��.
	if (CurrentCombo == 0)
	{
		// �޺� �׼� ���� ó���� ����.
		ComboActionBegin();
		return;
	}

	// �޺��� �������� ���� �ش� �����ֿ� �´� Ÿ�̸Ӹ� Ȱ��.
	// �޺� Ÿ�̸� �ڵ��� ��ȿ���� �ʴٸ�,
	// �̹� �޺� �ߵ��� �ɷȰų� Ÿ�̹��� ��ģ ���.
	// �� ���� �޺� ó���� �ʿ����� ����.
	if (!ComboTimerHandle.IsValid())
	{
		HasNextComboCommand = false;
	}
	else
	{
		HasNextComboCommand = true;
	}
}

void AABCharacterBase::ComboActionBegin()
{
	// �޺� ���¸� 1�� ����.
	CurrentCombo = 1;

	// �̵� ��Ȱ��ȭ.
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	// ��Ÿ�� ���.
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		const float AttackSpeedRate = 1.0f;
		AnimInstance->Montage_Play(ComboActionMontage, AttackSpeedRate);

		// ��Ÿ�� ����� ���۵Ǹ�, ����� ����� �� ȣ��Ǵ� ��������Ʈ�� ���.
		FOnMontageEnded EndDelegate;
		EndDelegate.BindUObject(this, &AABCharacterBase::ComboActionEnd);
		AnimInstance->Montage_SetEndDelegate(EndDelegate, ComboActionMontage);

		// ComboActionBegin�Լ�.
		// �޺� Ȯ���� ���� Ÿ�̸� ����.
		ComboTimerHandle.Invalidate();
		SetComboCheckTimer();
	}

}

void AABCharacterBase::ComboActionEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	// ��ȿ�� �˻�.
	ensure(CurrentCombo != 0);

	// �޺� �ʱ�ȭ.
	CurrentCombo = 0;

	// ĳ���� �����Ʈ ������Ʈ ��� ����.
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void AABCharacterBase::SetComboCheckTimer()
{
	// ���� ������� �޺��� �ε���.
	int32 ComboIndex = CurrentCombo - 1;

	// �޺� �ε��� �� ����.
	ensure(ComboActionData->EffectiveFrameCount.IsValidIndex(ComboIndex));

	// �޺� �ð� ���(Ȯ��).
	const float AttackSpeedRate = 1.0f;
	float ComboEffectiveTime = (ComboActionData->EffectiveFrameCount[ComboIndex] / ComboActionData->FrameRate) / AttackSpeedRate;

	// Ÿ�̸� ����.
	if (ComboEffectiveTime > 0.0f)
	{
		// Ÿ�̸� ����.
		// ù��°: ������ Ÿ�̸� �ڵ�.
		// �ι�°: Ÿ�̸ӿ� ������ �ð��� ��� ������ �� ����� �Լ��� ����.
		// ����°: Ÿ�̸ӿ� ������ ������ �Լ� ������.
		// �׹�°: Ÿ�̸� �ð�.
		// �ټ���°: �ݺ�����.
		GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &AABCharacterBase::ComboCheck, ComboEffectiveTime, false);
	}

}

void AABCharacterBase::ComboCheck()
{
	// Ÿ�̸� �ڵ� ��ȿȭ(�ʱ�ȭ).
	ComboTimerHandle.Invalidate();

	// ������ ���� �Է��� ���Դ��� Ȯ��.
	if (HasNextComboCommand)
	{
		// ��Ÿ�� ���� ó��.
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			// ���� �޺� �ε��� ����.
			CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, ComboActionData->MaxComboCount);

			// ������ ������ �̸� ����(��: ComboAttack2).
			FName NextSection = *FString::Printf(TEXT("%s%d"), *ComboActionData->MontageSectionNamePrefix, CurrentCombo);

			// ���� ����.
			AnimInstance->Montage_JumpToSection(NextSection, ComboActionMontage);

			// ���� �޺� ������ ���� Ÿ�̸� ����.
			SetComboCheckTimer();

			// �޺� ���� �Է� �÷��� �ʱ�ȭ.
			HasNextComboCommand = false;
		}
	}
}


