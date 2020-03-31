// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWordList.h"
#include <time.h>
#include <random>

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();

    SetValidWords(WordList);

    srand(time(NULL));

    PrintLine(TEXT("Welcome to the Bull Cow game!\n(Push TAB to access Terminal)"));

    InitGame();
}

void UBullCowCartridge::SetValidWords(const TArray<FString>& Words) {
    for(FString Word : Words){
        if(IsIsogram(Word) && Word.Len() >= 4 && Word.Len() <= 8) {
            ValidWords.Emplace(Word);
        }
    }
}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
    if(bGameOver){
        ClearScreen();
        InitGame();
        bGameOver = false;
    } else {
        ProcessGuess(Input.ToLower());
    }
}

void UBullCowCartridge::InitGame()
{
    HiddenWord = ValidWords[rand() % ValidWords.Num()];
    Lives = std::ceil(HiddenWord.Len() * 1.5);
    PrintLine(TEXT("Guess the %i letter word!"), HiddenWord.Len());
    PrintLine(TEXT("Type in your guess and \npress enter to continue...\n"));
}

void UBullCowCartridge::ProcessGuess(const FString& Guess){
    if (Guess == HiddenWord)
    {
        PrintLine(TEXT("You win!"));
        PrintLine(TEXT("/\\  /\\"));
        PrintLine(TEXT("~  ~"));
        PrintLine(TEXT(" O O"));
        PrintLine(TEXT("  00"));
        PrintLine(TEXT(" \\_/ \t moo!"));

        bGameOver = true;
        return;
    }

    if (Guess.Len() != HiddenWord.Len())
    {
        //Return size wrong
        PrintLine(TEXT("Lengths are not the same. It has %i characters."), HiddenWord.Len());
        return;
    }
    
    if (!IsIsogram(Guess))
    {
        PrintLine("Not an isogram, try again and ensure there are no repeating letters.");
        return;
    }

    --Lives;
    if (Lives <= 0)
    {
        PrintLine(TEXT("Out of lives! The word was %s"), *HiddenWord);
        ValidWords.Remove(HiddenWord);
        bGameOver = true;
    }
    else
    {
        CheckBullCows(Guess);
        PrintLine(TEXT("You have %i lives left."), Lives);
    }
    
}

void UBullCowCartridge::CheckBullCows(const FString& Guess)
{
    int32 Bulls = 0;
    int32 Cows = 0;

    for(int i = 0; i < Guess.Len(); i++){
        char PlayerChar = Guess[i];
        int32 HiddenIndexChar;
        HiddenWord.FindChar(PlayerChar, HiddenIndexChar);
        if(HiddenIndexChar != INDEX_NONE && HiddenIndexChar == i){
            ++Bulls;
        } else if(HiddenIndexChar != INDEX_NONE) {
            ++Cows;
        }
    }

    PrintLine(TEXT("There are %i Bulls and %i Cows."), Bulls, Cows);
}

bool UBullCowCartridge::IsIsogram(const FString& CheckWord){
    for(int i = 0; i < CheckWord.Len() - 1; i++) {
        char Character = CheckWord[i];
        for(int j = i + 1; j < CheckWord.Len(); j++)
        {
            if(Character == CheckWord[j]){
                return false;
            }
        }
    }
    return true;
}


