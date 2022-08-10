#include <iostream>
#include <fstream>
#include <algorithm> // used for converting uppercase letters to lowercase
#include <chrono>
#include "DynamicWordList.cpp"
#include<sstream> // for int to string conversion only

int main() {
    auto beginningTime = std::chrono::high_resolution_clock::now(); // starting timer
    std::ifstream inputFile;
    std::ofstream outputFile;
    static std::string tempString;
    static char tempChar;

    //###############################     STOPWORDS

    std::cout << "Reading stopwords..." << std::endl;
    inputFile.open("stopwords.txt");
    DynamicWordList dynamicStopwordsList = *new DynamicWordList;
    while (inputFile >> tempString) {
        dynamicStopwordsList.addWord(tempString, 0);
    }
    for (int i = 0; i <= 100; i++) {
        dynamicStopwordsList.addWord(std::to_string(i), 0);
    }
    //std::cout << dynamicStopwordsList.getCurrentCapacity() << std::endl;
    inputFile.close();

    //#######################################

    //####################################### clear text

    std::cout << "Removing punctuation and re-writing..." << std::endl;
    inputFile.open("PublicationsDataSet - Copy.txt");
    outputFile.open("edited.txt");
    while (inputFile.get(tempChar)) {
        if (tempChar == '"') {
            tempChar = ' ';
        }
        outputFile << tempChar;
    }
    inputFile.close();
    outputFile.close();

    inputFile.open("edited.txt");
    outputFile.open("edited2.txt");
    while (inputFile >> tempString) { //remove special characters on last and first letters
        while (tempString.length() > 1 && (tempString[0] == ':' || tempString[0] == '(')) {
            tempString.erase(0, 1);
        }
        while (tempString.length() > 1 && (tempString[tempString.length() - 1] == '.' || tempString[tempString.length() - 1] == ',' ||
                                           tempString[tempString.length() - 1] == '\\' || tempString[tempString.length() - 1] == ')' ||
                                           tempString[tempString.length() - 1] == '@' || tempString[tempString.length() - 1] == '-' ||
                                           tempString[tempString.length() - 1] == '(')) {
            tempString.pop_back();
        }
        // std::cout << tempString << std::endl;
        outputFile << tempString + " ";
    }
    inputFile.close();
    outputFile.close();

    //#######################################

    //####################################### from "unigramCount" to "}"

    DynamicWordList dynamicWordList = *new DynamicWordList;
    std::cout << "Reading arranged file and counting words..." << std::endl;
    int count = 0;
    inputFile.open("edited2.txt");
    while (inputFile >> tempString && !inputFile.eof()) {
        if (tempString == "unigramCount") {
            count++;
            if (count % 300 == 0) std::cout << "..." << std::endl;
            //std::cout << tempString << count << std::endl;
            inputFile >> tempString; // iterate until end of unigramCount object
            //std::cout << "{ - " << tempString << std::endl;
            while (tempString.find("}}") == std::string::npos) {
                bool isStopword = false;
                bool isRepeated = false;
                static int repetition = 0;

                inputFile >> tempString; // take the word

                transform(tempString.begin(), tempString.end(), tempString.begin(), ::tolower);

                inputFile >> repetition; // take its repetition

                if (inputFile.fail()) inputFile.clear();

                //std::cout << tempString << " : " << repetition << std::endl;

                for (int i = 0; i < dynamicStopwordsList.getCurrentCapacity(); i++) { // check if the word is a stopword
                    if (dynamicStopwordsList.getWord(i) == tempString || tempString == "=" || tempString == "+" ||
                        tempString == "&" || tempString == "," || tempString == ")" || tempString == "." || tempString == "\\"
                        || tempString == "&t" || tempString == "{keys:[{t:" || tempString == "-" || tempString == "{t:") {
                        //std::cout << "stopword: " << tempString << std::endl;
                        isStopword = true;
                        break;
                    }
                }

                if (!isStopword) { // if not a stopword
                    for (int i = 0; i <= dynamicWordList.getCurrentCapacity(); i++) {
                        if (dynamicWordList.getWord(i)[0] == tempString[0]) { // check if it's already added
                            if (dynamicWordList.getWord(i).length() == tempString.length()) {
                                if (dynamicWordList.getWord(i) == tempString) {
                                    isRepeated = true;
                                    //std::cout << "repeated word: " << tempString << std::endl;
                                    dynamicWordList.increaseRepetition(i, repetition); // if it's added sum repetitions
                                    break;
                                }
                            }
                        }
                    }

                    if (!isRepeated) { // if it's not added
                        dynamicWordList.addWord(tempString, repetition);
                        //std::cout << "new word: " << tempString << std::endl;
                    }
                }
            }
        }
    }

    //#######################################

    //#######################################

    int indexOfLargest = 0;
    std::cout << "\n \n##### TOP 10 REPEATED WORDS ###### \n" << std::endl;
    for (int j = 0; j < 10; j++) { // loop to print top 10 repeated words
        for (int i = 0; i < dynamicWordList.getCurrentCapacity(); i++) {
            if (dynamicWordList.getRepetition(i) > dynamicWordList.getRepetition(indexOfLargest)) {
                indexOfLargest = i;
            }
        }
        std::cout << dynamicWordList.getWord(indexOfLargest) << " - " << dynamicWordList.getRepetition(indexOfLargest)
                  << std::endl;
        dynamicWordList.resetRepetition(indexOfLargest);
    }
    std::cout << std::endl;

    //#######################################

    //#######################################

    auto endingTime = std::chrono::high_resolution_clock::now(); // stopping timer
    std::chrono::duration<double> elapsedTime = endingTime - beginningTime; // calculate elapsed time
    std::cout << "Total Elapsed Time: " << (int)(elapsedTime.count() / 60) << " minutes "
              << (int)elapsedTime.count() % 60 << " seconds" << std::endl;
    return 0;
}
