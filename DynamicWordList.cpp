//
// Created by Gülce Gül on 26.01.2022.
//

#include <iostream>

class DynamicWordList { // a custom words to hold wordArray as string and their repetition counts as integers
private:
    int capacity;
    std::string *wordArray;
    int currentCapacity;
    int *repetitionArray;
public:
    DynamicWordList() {
        currentCapacity = 0;
        capacity = 5;
        wordArray = new std::string[capacity];
        repetitionArray = new int[capacity];
    }

    int getCurrentCapacity(){
        return currentCapacity;
    }

    std::string getWord(int position) { // function to get word at given position
        return wordArray[position];
    }

    int getRepetition(int position) { // function to get repetition number of word at given position
        return repetitionArray[position];
    }

    void addWord(std::string wordToAdd,
                 int repetitionOfWord) { // adds given word to array and increases used size (current position) by 1
        wordArray[currentCapacity] = wordToAdd;
        repetitionArray[currentCapacity] = repetitionOfWord;
        currentCapacity++;
        if (currentCapacity == capacity - 1) {
            capacity = capacity*2;

            std::string* wordsTemp = new std::string[capacity];
            std::copy(wordArray, wordArray + std::min(capacity/2, capacity), wordsTemp);
            delete[] wordArray;
            wordArray = wordsTemp;

            int* repetitionsTemp = new int[capacity];
            std::copy(repetitionArray, repetitionArray + std::min(capacity/2, capacity), repetitionsTemp);
            delete[] repetitionArray;
            repetitionArray = repetitionsTemp;
        }
    }

    void increaseRepetition(int position, int amount) {
        repetitionArray[position] = repetitionArray[position] + amount;
    }

    void resetRepetition(int position) { // sets repetition count of word at given position to 0
        repetitionArray[position] = 0;
    }

    ~DynamicWordList() { // to free memory after our job is done
        delete[] wordArray;
        delete[] repetitionArray;
    }
};
