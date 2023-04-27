// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class GROUP_7_API UMyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

		virtual void NativeInitializeAnimation() override; 

public:

	UFUNCTION(BlueprintCallable)
		void UpdateAnimationProperties();

	//A reference the speed of the pawn to change it from idle animation to run
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
		float Speed;

	//Pointer to the Pawn the animation is used on
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
		APawn* Pawn;

};
