// Fill out your copyright notice in the Description page of Project Settings.


#include "ABGameMode.h"
//#include "Player/ABPlayerController.h"

AABGameMode::AABGameMode()
{
	// Class�� ������ ���� �ڿ� '_C'�� �ٿ��ش�.
	// Asset�� �����ִ� Class�� ������ �� _C�� �ٿ� ��������� Class�� �����´ٰ� ����� �������� ����.
	//static ConstructorHelpers::FClassFinder<APawn> DefaultCharacterRef(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter.BP_ThirdPersonCharacter_C"));
	static ConstructorHelpers::FClassFinder<APawn> DefaultCharacterRef(TEXT("/Script/ArenaBattleDemo.ABCharacterPlayer"));


	// GameMode�� Default Pawn Class ����.
	if (DefaultCharacterRef.Class)
	{
		DefaultPawnClass = DefaultCharacterRef.Class;
	}

	// GameMode�� PlayerController Class ����.
	// header�� include�ؼ� class�� �������� ���.
	//PlayerControllerClass = AABPlayerController::StaticClass();
	// ConstructorHelpers::FClassFinder�� ������ �� ���� : header�� include ������ �ʾƵ� �����ϴ�.
	// header�� include���� ������ ������ �ӵ��� �� ������ �� �� �ִ�. 
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Script/ArenaBattleDemo.ABPlayerController"));
	if (PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}

}
