// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAnimInstance.h"


void UMyAnimInstance::NativeInitializeAnimation()
{
	if (Pawn == nullptr) //find pawn if it doesn't reference to any
	{
		Pawn = TryGetPawnOwner(); //From UAnimInstance
	}
}

void UMyAnimInstance::UpdateAnimation() //updates animation properties
{
	if (Pawn == nullptr) //find pawn if it doesn't reference to any
	{
		Pawn = TryGetPawnOwner(); //From UAnimInstance
	}

	if (Pawn)
	{
		FVector MovementSpeed = Pawn->GetVelocity();
		FVector LateralSpeed = FVector(MovementSpeed.X, MovementSpeed.Y, 0.f);
		Speed = LateralSpeed.Size(); //uses this to change from idle to run
	}
}
