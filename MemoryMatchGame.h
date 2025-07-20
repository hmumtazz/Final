#ifndef MEMORYMATCHGAME_H
#define MEMORYMATCHGAME_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <thread>
#include <iomanip>

using namespace std;

class MemoryMatchGame {
private:
    string theme50Words[50];
    string** answerArray;
    string** gamePlayDisplayArray;
    bool** matchedPairs;
    
    int gridSize;
    int delaySeconds;
    string themeName;
    string faceTermName;
    
    int totalPairs;
    int matchedCount;
    
    chrono::steady_clock::time_point startTime;
    chrono::steady_clock::time_point endTime;
    
public:
    MemoryMatchGame();
    ~MemoryMatchGame();
    
    void start();
    
private:
    void selectTheme();
    void selectDifficulty();
    void selectSpeed();
    
    void loadTheme(const string& filename);
    void shuffleThemeArray();
    void populateAnswerGrid();
    
    void allocateArrays();
    void deallocateArrays();
    
    void displayGrid();
    void displayGridWithRevealed(int row1, int col1, int row2, int col2);
    void clearScreen();
    
    void playGame();
    bool selectSquare(int& row, int& col, int squareNumber);
    bool checkMatch(int row1, int col1, int row2, int col2);
    bool isGameComplete();
    
    void delay(int seconds);
    void displayTimer();
    int getValidInput(int min, int max);
};

#endif
