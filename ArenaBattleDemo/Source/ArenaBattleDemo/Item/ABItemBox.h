// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ABItemBox.generated.h"

UCLASS()
class ARENABATTLEDEMO_API AABItemBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AABItemBox();

protected:
	// �ڽ� ������Ʈ�� ������ ��������Ʈ�� ����� �Լ�.
	// OnComponentBeginOverlap ��������Ʈ�� ���̳������� �����Ǿ� �ֱ� ������
	// UFUNCTION ��ũ�θ� �����ؾ� ��.
	UFUNCTION()
	void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	// ��ƼŬ ��� ���� �� ����Ǵ� ��������Ʈ�� ����� �Լ�.
	UFUNCTION()
	void OnEffectFinished(class UParticleSystemComponent* PSystem);


protected:
	// ������ �浹�� ����� �ڽ� ������Ʈ.
	UPROPERTY(VisibleAnywhere, Category = Box)
	TObjectPtr<class UBoxComponent> Trigger;

	// ������ �ڽ��� ������ �޽� ������Ʈ.
	UPROPERTY(VisibleAnywhere, Category = Box)
	TObjectPtr<class UStaticMeshComponent> Mesh;

	// �ڽ��� ��ȣ�ۿ����� �� ������ ��ƼŬ ȿ�� ������Ʈ.
	UPROPERTY(VisibleAnywhere, Category = Box)
	TObjectPtr<class UParticleSystemComponent> Effect;

	// ������ ����.
	UPROPERTY(EditAnywhere, Category = Item)
	TObjectPtr<class UABItemData> Item;

};
