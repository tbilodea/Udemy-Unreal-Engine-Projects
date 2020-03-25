#include <iostream>
#include <time.h>
#include <vector>

/*
Simple C++ guessing game that scales in level, asking player to figure out 3 number combination
*/

bool PlayAtDifficulty(int Difficulty) {
    srand(time(NULL));

    const int FirstNumber = (rand() % Difficulty) + 1;
    const int SecondNumber = (rand() % Difficulty) + 1;
    const int ThirdNumber = (rand() % Difficulty) + 1;

    const int GuessSum = FirstNumber + SecondNumber + ThirdNumber;
    const int GuessProduct = FirstNumber * SecondNumber * ThirdNumber;

    int FirstPlayerGuess, SecondPlayerGuess, ThirdPlayerGuess;
    
    std::cout << "\nLevel " << Difficulty << " requires 3 numbers.\n\n";
    std::cout << "The numbers add to " << GuessSum << "\n";
    std::cout << "The numbers multiply to " << GuessProduct << "\n\n";
    std::cout << "Figure out the numbers and input them Dear Sentient Bean (separated by spaces of course!):\n";

    std::cin >> FirstPlayerGuess >> SecondPlayerGuess >> ThirdPlayerGuess;
    
    const int PlayerSum = FirstPlayerGuess + SecondPlayerGuess + ThirdPlayerGuess;
    const int PlayerProduct = FirstPlayerGuess * SecondPlayerGuess * ThirdPlayerGuess;

    return PlayerSum == GuessSum && PlayerProduct == GuessProduct;
}


int main(){
    //Welcome messages
    std::cout << "The ship needs you to input the codes to verify bean sentience.\n";
    std::cout << std::endl;
    std::cout << "Enter the correct codes to start the engines!\n";

    int Difficulty = 1;
    const int MaxDifficulty = 10;

    while(Difficulty <= MaxDifficulty){
        bool PlayerRight = PlayAtDifficulty(Difficulty);

        if(PlayerRight){
            std::cout << "Correct bean!\n";
        } else {
            std::cout << "Uhhh that is incorrect. You are not a bean. Locking you out of the ship.\n";
            return 0;
        }

        std::cin.clear();
        std::cin.ignore();
        ++Difficulty;
    }

    std::cout << "Ship started, let's go bean friend!\n";
    return 0;
}
