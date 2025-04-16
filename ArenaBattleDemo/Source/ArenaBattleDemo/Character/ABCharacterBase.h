// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ABCharacterBase.generated.h"

UENUM()
enum class ECharacterControlType : uint8
{
	Shoulder,
	Quarter
};


UCLASS()
class ARENABATTLEDEMO_API AABCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AABCharacterBase();

	virtual void SetCharacterControlData(const class UABCharacterControlData* InCharacterControlData);

protected:	// Combo Section.
	// �޺� �׼� ó�� �Լ�.
	// ������ ó�� ����� ���� �޺� �׼� ó���� �б�.
	void ProcessComboCommand();

	// �޺� �׼��� ���۵� �� ȣ���� �Լ�.
	void ComboActionBegin();

	// �޺��� ����� �� ȣ��� �Լ�.
	// �ִ� ��Ÿ�ֿ��� �����ϴ� ��������Ʈ�� �Ķ���� ����.
	void ComboActionEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);

	// �޺� Ÿ�̸� ���� �Լ�.
	void SetComboCheckTimer();

	// Ÿ�̸� �ð� ���̿� �Է��� ���Դ��� ���θ� Ȯ���ϴ� �Լ�.
	void ComboCheck();



protected:
	UPROPERTY(EditAnywhere, Category = "CharacterControl", meta = (AllowPrivateAccess = "true"))
	TMap<ECharacterControlType, class UABCharacterControlData*> CharacterControlManager;

	// ���� ��Ÿ�� �ּ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> ComboActionMontage;

	// �޺� ó���� ����� ������ ����.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UABComboActionData> ComboActionData;


	// ���� ������� �޺� �ܰ�.
	// 0 -> �޺� �������� ����. 1/2/3/4 �޺��� ���۵�.
	int32 CurrentCombo = 0;

	// �޺� ���� ���θ� �Ǵ��ϱ� ���� Ÿ�̸� �ڵ�.
	FTimerHandle ComboTimerHandle;

	// �޺� Ÿ�̸� ������ �Է��� ���Դ����� Ȯ���ϴ� �Ҹ��� ����.
	bool HasNextComboCommand = false;

};
