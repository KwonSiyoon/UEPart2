// Fill out your copyright notice in the Description page of Project Settings.


#include "ABCharacterPlayer.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "ABCharacterControlData.h"


#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

AABCharacterPlayer::AABCharacterPlayer()
{
	// 컨트롤러의 회전을 받아서 설정하는 모드를 모두 해제.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// 무브먼트 설정.
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 800.0f;

	// 컴포넌트 설정.
	GetCapsuleComponent()->SetCapsuleHalfHeight(88.0f);

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));

	


	// 리소스 설정.
	// Mesh 설정. 
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMesh(TEXT("/Game/Characters/Mannequins/Meshes/SKM_Quinn.SKM_Quinn"));
	if (CharacterMesh.Object)
	{
		// Mesh에 접근해서 설정하는 방법.
		GetMesh()->SetSkeletalMesh(CharacterMesh.Object);
	}

	// Animation Blueprint 설정.
	// Class니까 _C 추가.
	static ConstructorHelpers::FClassFinder<UAnimInstance> CharacterAnim(TEXT("/Game/Characters/Mannequins/Animations/ABP_Quinn.ABP_Quinn_C"));
	if (CharacterAnim.Class)
	{
		GetMesh()->SetAnimClass(CharacterAnim.Class);
	}

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 500.0f;
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	// Input.
	/*static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingContextRef(TEXT("/Game/ArenaBattle/Input/IMC_Default.IMC_Default"));
	if (InputMappingContextRef.Object)
	{
		DefaultMappingContext = InputMappingContextRef.Object;
	}*/

	static ConstructorHelpers::FObjectFinder<UInputAction> ShoulderMoveActionRef(TEXT("/Game/ArenaBattle/Input/Actions/IA_ShoulderMove.IA_ShoulderMove"));
	if (ShoulderMoveActionRef.Object)
	{
		ShoulderMoveAction = ShoulderMoveActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> ShoulderLookActionRef(TEXT("/Game/ArenaBattle/Input/Actions/IA_ShoulderLook.IA_ShoulderLook"));
	if (ShoulderLookActionRef.Object)
	{
		ShoulderLookAction = ShoulderLookActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> QuarterMoveActionRef(TEXT("/Game/ArenaBattle/Input/Actions/IA_QuarterMove.IA_QuarterMove"));
	if (QuarterMoveActionRef.Object)
	{
		QuarterMoveAction = QuarterMoveActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> JumpActionRef(TEXT("/Game/ArenaBattle/Input/Actions/IA_Jump.IA_Jump"));
	if (JumpActionRef.Object)
	{
		JumpAction = JumpActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> ChangeControlActionRef(TEXT("/Game/ArenaBattle/Input/Actions/IA_ChangeControl.IA_ChangeControl"));
	if (ChangeControlActionRef.Object)
	{
		ChangeControlAction = ChangeControlActionRef.Object;
	}

	// 초기 설정.
	CurrentCharacterControlType = ECharacterControlType::Quarter;

}

void AABCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	// 입력 설정.
	SetCharacterControl(CurrentCharacterControlType);

	// Add InputMapping Context to Enhanced Input System.
	// Cast와 CastChecked
	// Cast는 확실하지 않을 때 사용. Cast 실패시 nullptr 반환.
	// CastChecked 확실하거나 꼭 Cast 되어야 할 때 사용. Cast 실패시 Assertion되어 프로그램 break.
	/*APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (auto SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		SubSystem->AddMappingContext(DefaultMappingContext, 0);
	}*/

}

void AABCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	auto EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	// Binding.
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	EnhancedInputComponent->BindAction(ChangeControlAction, ETriggerEvent::Triggered, this, &AABCharacterPlayer::ChangeCharacterControl);
	EnhancedInputComponent->BindAction(ShoulderMoveAction, ETriggerEvent::Triggered, this, &AABCharacterPlayer::ShoulderMove);
	EnhancedInputComponent->BindAction(ShoulderLookAction, ETriggerEvent::Triggered, this, &AABCharacterPlayer::ShoulderLook);
	EnhancedInputComponent->BindAction(QuarterMoveAction, ETriggerEvent::Triggered, this, &AABCharacterPlayer::QuarterMove);
}

void AABCharacterPlayer::SetCharacterControl(ECharacterControlType NewCharacterControlType)
{
	UABCharacterControlData* NewCharacterControl = CharacterControlManager[NewCharacterControlType];
	check(NewCharacterControl);

	SetCharacterControlData(NewCharacterControl);

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (auto SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		SubSystem->ClearAllMappings();
		SubSystem->AddMappingContext(NewCharacterControl->InputMappingContext, 0);
	}

	CurrentCharacterControlType = NewCharacterControlType;

}

void AABCharacterPlayer::SetCharacterControlData(const UABCharacterControlData* InCharacterControlData)
{
	Super::SetCharacterControlData(InCharacterControlData);
	SpringArm->TargetArmLength = InCharacterControlData->TargetArmLength;
	SpringArm->SetRelativeRotation(InCharacterControlData->RelativeRotation);
	SpringArm->bUsePawnControlRotation = InCharacterControlData->bUsePawnControlRotation;
	SpringArm->bInheritPitch = InCharacterControlData->bInheritPitch;
	SpringArm->bInheritYaw = InCharacterControlData->bInheritYaw;
	SpringArm->bInheritRoll = InCharacterControlData->bInheritRoll;
	SpringArm->bDoCollisionTest = InCharacterControlData->bDoCollisionTest;
	
}

void AABCharacterPlayer::ChangeCharacterControl()
{
	// 사용할 캐릭터 컨트롤을 변경하는 함수.
	if (CurrentCharacterControlType == ECharacterControlType::Quarter)
	{
		SetCharacterControl(ECharacterControlType::Shoulder);
	}
	else if (CurrentCharacterControlType == ECharacterControlType::Shoulder)
	{
		SetCharacterControl(ECharacterControlType::Quarter);
	}
}

void AABCharacterPlayer::ShoulderMove(const FInputActionValue& Value)
{
	// 입력 값 읽기.
	FVector2D Movement = Value.Get<FVector2D>();

	// 컨트롤러의 회전 값.
	//FRotator Rotation = GetController()->GetControlRotation();
	FRotator Rotation = GetControlRotation(); // 안에서 GetController도 알아서 해줌.
	FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

	// 방향 구하기.
	FVector ForwardVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	FVector RightVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardVector, Movement.X);
	AddMovementInput(RightVector, Movement.Y);

}

void AABCharacterPlayer::ShoulderLook(const FInputActionValue& Value)
{
	// 입력 값 읽기.
	FVector2D LookVector = Value.Get<FVector2D>();

	// 컨트롤러에 회전 추가.
	AddControllerYawInput(LookVector.X);
	AddControllerPitchInput(LookVector.Y);

}

void AABCharacterPlayer::QuarterMove(const FInputActionValue& Value)
{
	// 입력 값 읽기.
	FVector2D Movement = Value.Get<FVector2D>();
	//float InputSizeSquared = Movement.SizeSquared();
	float MovementvectorSize = 1.0f;
	float MovementVectorSizeSquared = Movement.SizeSquared();

	// 두 방향으로 입력이 들어오면, 이동 방향은 정규화해 크기를 1로 만들고
	// 입력 스케일을 1로 강제 설정.
	if (Movement.SquaredLength() > 1.0f)
	{
		Movement.Normalize();
		MovementvectorSize = 1.0f;
	}
	// 입력이 1 이하이면, 해당 입력을 스케일로 사용하기 위해 값 계산.
	else
	{
		MovementvectorSize = FMath::Sqrt(MovementVectorSizeSquared);
	}

	FVector MoveDirection = FVector(Movement.X, Movement.Y, 0.0f);

	// 캐릭터가 이동하는 방향에 맞게 컨트롤러 회전 설정.
	Controller->SetControlRotation(FRotationMatrix::MakeFromX(MoveDirection).Rotator());

	// 입력에 따른 방향으로 이동하도록 입력 전달.
	AddMovementInput(MoveDirection, MovementvectorSize);
}
