// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ABCharacterBase.h"
#include "ABCharacterControlData.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ABComboActionData.h"
#include "Physics/ABCollision.h"
#include "Components/CapsuleComponent.h"
#include "Engine/DamageEvents.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
AABCharacterBase::AABCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// ������Ʈ ����.
	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_ABCAPSULE);

	// �޽��� �ݸ����� NoCollision ����(�ַ� ������ ����).
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));


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

	// �޺� �׼� ��Ÿ�� �ּ� ����.
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ComboActionMontageRef(TEXT("/Game/ArenaBattle/Animation/AM_ComboAttack.AM_ComboAttack"));
	if (ComboActionMontageRef.Object)
	{
		ComboActionMontage = ComboActionMontageRef.Object;
	}

	// �޺� �׼� ������ �ּ� ����.
	static ConstructorHelpers::FObjectFinder<UABComboActionData> ComboActionDataRef(TEXT("/Game/ArenaBattle/ComboAction/ABA_ComboAction.ABA_ComboAction"));
	if (ComboActionDataRef.Object)
	{
		ComboActionData = ComboActionDataRef.Object;
	}

	// ���� ��Ÿ�� �ּ� ����.
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRef(TEXT("/Game/ArenaBattle/Animation/AM_Dead.AM_Dead"));
	if (DeadMontageRef.Object)
	{
		DeadMontage = DeadMontageRef.Object;
		DeadMontage->bEnableAutoBlendOut = false;
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

void AABCharacterBase::AttackHitCheck()
{
	// ���� ���� ����.
	//UE_LOG(LogTemp, Log, TEXT("AttackHitCheck !!"));

	// �浹 ���� ���� ���.
	// ĳ���� ���뿡�� �ణ ������(ĸ���� ������ ��ŭ) ����.
	FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();

	// ���� �Ÿ�.
	const float AttackRange = 200.0f;
	FVector End = Start + GetActorForwardVector() * AttackRange;

	// SCENE_QUERY_STAT: �𸮾󿡼� �����ϴ� �м� ���� �±׸� �߰�.
	// �ι�° ����: ������ ������ �浹ü�� ������ �� ����.
	// ����° ����: ������ ���� ���.
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

	// Ʈ���̽��� ����� ��ü�� ������.
	const float AttackRadius = 50.0f;

	// Ʈ���̽��� Ȱ���� �浹 �˻�.
	FHitResult OutHitResult;
	bool HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, CCHANNEL_ABACTION, FCollisionShape::MakeSphere(AttackRadius), Params);

	// �浹 ������ ����� ó��.
	if (HitDetected)
	{
		// ������ ��.
		const float AttackDamage = 30.0f;

		// ������ �̺�Ʈ.
		FDamageEvent DamageEvent;

		// ������ ����.
		OutHitResult.GetActor()->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
	}

	// �浹 �����(�ð������� Ȯ���� �� �ֵ���).
#if ENABLE_DRAW_DEBUG

	// ĸ���� �߽� ��ġ.
	//FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
	FVector CapsuleOrigin = (End + Start) * 0.5f;

	// ĸ�� ���� ���� ��.
	float CapsuleHalfHeight = AttackRange * 0.5f;

	// ǥ���� ����(�� �¾����� ����, �¾����� �ʷ�).
	FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;
	//FMath::RandRange
	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor, false, 5.0f);

#endif

}

float AABCharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	// @Task: ������ �ٷ� �׵��� ó��.
	SetDead();

	return DamageAmount;
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

void AABCharacterBase::SetDead()
{
	// �����Ʈ 
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	// �ݸ��� ����.
	SetActorEnableCollision(false);

	// �״� �ִϸ��̼� ���.
	PlayDeadAnimation();
}

void AABCharacterBase::PlayDeadAnimation()
{
	// ��Ÿ�� ���.
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		// �̹� ������� ��Ÿ�ְ� �ִٸ�, ��� ����.
		AnimInstance->StopAllMontages(0.0f);

		// ���� ��Ÿ�� ���.
		const float PlayRate = 1.0f;
		AnimInstance->Montage_Play(DeadMontage, PlayRate);
	}
}


