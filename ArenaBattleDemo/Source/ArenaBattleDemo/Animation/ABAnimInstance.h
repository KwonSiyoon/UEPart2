// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ABAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLEDEMO_API UABAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:	// Functions.
	UABAnimInstance();

	// �ִϸ��̼��� �ʱ�ȭ�� �� ȣ��.
	virtual void NativeInitializeAnimation() override;
	// �ִϸ��̼� ������Ʈ �Լ�.
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:	// Property.

	// �ִ� �ν��Ͻ��� �����ϴ� ĳ���� ���� ����.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	TObjectPtr<class ACharacter> Owner;

	// ĳ���� �����Ʈ ������Ʈ ���� ����.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	TObjectPtr<class UCharacterMovementComponent> Movement;

	// ĳ������ ���� �̵� �ӵ��� ������ ����.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	FVector Velocity;
	
	// ĳ������ �������� �̵� �ӵ��� ������ ����.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	float GroundSpeed;

	// ĳ���Ͱ� Idle���� ���θ� Ȯ���� ����.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	uint8 bIsIdle : 1;

	// ĳ���Ͱ� �̵��ϴ���(�����ִ���) ���θ� �Ǵ��ϴµ� ����� ���� ��.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	float MovingThreshold;

	// ĳ���Ͱ� ���߿��� ���������� ���θ� Ȯ���ϴµ� ����� ���� ��.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	uint8 bIsFalling : 1;

	// ĳ���Ͱ� ���������� ���θ� Ȯ���ϴµ� ����� ���� ��.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	uint8 bIsJumping : 1;

	// ĳ���Ͱ� �����ϴ��� ���θ� Ȯ���ϴµ� ����� ���� ��.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	float JumpingThreshold;


};
