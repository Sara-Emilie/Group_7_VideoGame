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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
		float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
		APawn* Pawn;

};
