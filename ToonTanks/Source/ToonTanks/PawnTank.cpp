// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnTank.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"

APawnTank::APawnTank()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->AttachTo(RootComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->AttachTo(SpringArm);
}

void APawnTank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &APawnTank::CalculateMovementInput);
	PlayerInputComponent->BindAxis("Turn", this, &APawnTank::CalculateRotateInput);

	//PlayerInputComponent->BindAxis("RotateTurret", this, &APawnTank::RotateTheTurret);

	PlayerInputComponent->BindAction("Fire", EInputEvent::IE_Pressed, this, &APawnTank::Fire);
}

void APawnTank::CalculateMovementInput(float Value)
{
	MoveDirection = FVector(Value * MoveSpeed * GetWorld()->DeltaTimeSeconds, 0, 0);
}

void APawnTank::CalculateRotateInput(float Value)
{
	float RotateAmount = Value * RotateSpeed * GetWorld()->DeltaTimeSeconds;
	FRotator Rotation = FRotator(0, RotateAmount, 0);
	RotationDirection = FQuat(Rotation);
}

void APawnTank::Move()
{
	AddActorLocalOffset(MoveDirection, true);
}

void APawnTank::Rotate()
{
	AddActorLocalRotation(RotationDirection, true);
}

// Called when the game starts or when spawned
void APawnTank::BeginPlay()
{
	Super::BeginPlay();

	PlayerControllerRef = Cast<APlayerController>(GetController());
}

void APawnTank::HandleDestruction()
{
	Super::HandleDestruction();

	// HidePlayer.TODO- Create New Function
}

// Called every frame
void APawnTank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Move();
	Rotate();

	if (PlayerControllerRef)
	{
		PlayerControllerRef->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, TraceHitResult);
		FVector HitLocation = TraceHitResult.ImpactPoint;

		DrawDebugPoint(GetWorld(), HitLocation, 3, FColor::Green, false, 3.f);
		RotateTurret(HitLocation);
	}
}

//void APawnTank::RotateTheTurret(float Value)
//{
//	auto RotationChange = TurretRotateSpeed * Value * GetWorld()->DeltaTimeSeconds;
//	auto Rot = GetTurretMesh()->GetComponentRotation();
//	auto NewRotation = Rot.Yaw + RotationChange;
//	RotateTurret(FRotator(0, NewRotation, 0).Vector());
//	GetTurretMesh()->SetWorldRotation(FRotator(0, NewRotation, 0));
//}