// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Console/Cartridge.h"
#include "BullCowCartridge.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BULLCOWGAME_API UBullCowCartridge : public UCartridge
{
	GENERATED_BODY()

	public:
	virtual void BeginPlay() override;
	virtual void OnInput(const FString& Input) override;
	void InitGame();
	void ProcessGuess(const FString& Guess);
	bool IsIsogram(const FString& PlayerInput);
	void CheckBullCows(const FString& Guess);
	void SetValidWords(const TArray<FString>& Words);

	// Your declarations go below!
	private:
	TArray<FString> ValidWords;
	FString HiddenWord;
	int32 Lives;
	bool bGameOver;
};
