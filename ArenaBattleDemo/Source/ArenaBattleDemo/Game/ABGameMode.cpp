// Fill out your copyright notice in the Description page of Project Settings.


#include "ABGameMode.h"
#include "Player/ABPlayerController.h"

AABGameMode::AABGameMode()
{
	// Class를 가져올 때는 뒤에 '_C'를 붙여준다.
	// Asset에 속해있는 Class를 가져올 때 _C를 붙여 명시적으로 Class를 가져온다고 해줘야 가져오기 가능.
	//static ConstructorHelpers::FClassFinder<APawn> DefaultCharacterRef(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter.BP_ThirdPersonCharacter_C"));
	static ConstructorHelpers::FClassFinder<APawn> DefaultCharacterRef(TEXT("/Script/ArenaBattleDemo.ABCharacterPlayer"));


	// GameMode의 Default Pawn Class 설정.
	if (DefaultCharacterRef.Class)
	{
		DefaultPawnClass = DefaultCharacterRef.Class;
	}

	// GameMode의 PlayerController Class 설정.
	// header를 include해서 class를 가져오는 방법.
	//PlayerControllerClass = AABPlayerController::StaticClass();
	// ConstructorHelpers::FClassFinder로 가져올 때 장점 : header를 include 해주지 않아도 가능하다.
	// header를 include하지 않으면 컴파일 속도를 더 빠르게 할 수 있다. 
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Script/ArenaBattleDemo.ABPlayerController"));
	if (PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}

	// 기본값 설정.
	ClearScore = 3;
	CurrentScore = 0;
	bIsCleared = false;

}

void AABGameMode::OnPlayerScoreChanged(int32 NewPlayerScore)
{
	// 현재 점수를 새로운 점수로 업데이트.
	CurrentScore = NewPlayerScore;

	// 플레이어 컨트롤러 가져오기.
	AABPlayerController* ABPlayerController = Cast<AABPlayerController>(GetWorld()->GetFirstPlayerController());

	// 형변환 확인.
	if (ABPlayerController)
	{
		ABPlayerController->GameScoreChanged(CurrentScore);
	}

	// 게임 클리어 여부 확인.
	if (CurrentScore >= ClearScore)
	{
		bIsCleared = true;
		// 형변환 확인.
		if (ABPlayerController)
		{
			ABPlayerController->GameClear();
		}
	}
}

void AABGameMode::OnPlayerDead()
{
	// 플레이어 컨트롤러 가져오기.
	AABPlayerController* ABPlayerController = Cast<AABPlayerController>(GetWorld()->GetFirstPlayerController());
	// 형변환 확인.
	if (ABPlayerController)
	{
		ABPlayerController->GameOver();
	}
}

bool AABGameMode::IsGameCleared()
{
	return bIsCleared;
}
