#include "MemoryMatchGame.h"

MemoryMatchGame::MemoryMatchGame() {
    answerArray = nullptr;
    gamePlayDisplayArray = nullptr;
    matchedPairs = nullptr;
    gridSize = 0;
    delaySeconds = 0;
    matchedCount = 0;
    totalPairs = 0;
}

MemoryMatchGame::~MemoryMatchGame() {
    deallocateArrays();
}

void MemoryMatchGame::start() {
    cout << "\n=== MEMORY MATCH GAME ===" << endl;
    
    selectTheme();
    selectDifficulty();
    selectSpeed();
    
    allocateArrays();
    shuffleThemeArray();
    populateAnswerGrid();
    
    startTime = chrono::steady_clock::now();
    playGame();
    endTime = chrono::steady_clock::now();
    
    displayTimer();
}

void MemoryMatchGame::selectTheme() {
    cout << "\nChoose game theme:" << endl;
    cout << "1) Animals" << endl;
    cout << "2) Colors" << endl;
    cout << "3) Elements" << endl;
    cout << "Enter your choice (1-3): ";
    
    int choice = getValidInput(1, 3);
    
    switch(choice) {
        case 1:
            loadTheme("theme1_animals.txt");
            themeName = "Animals";
            faceTermName = "ANIMALS";
            break;
        case 2:
            loadTheme("theme2_colors.txt");
            themeName = "Colors";
            faceTermName = "COLORS";
            break;
        case 3:
            loadTheme("theme3_elements.txt");
            themeName = "Elements";
            faceTermName = "ELEMENTS";
            break;
    }
}

void MemoryMatchGame::selectDifficulty() {
    cout << "\nLevel of Play:" << endl;
    cout << "1) 4 x 4 grid (Easy)" << endl;
    cout << "2) 6 x 6 grid (Moderate)" << endl;
    cout << "3) 8 x 8 grid (Difficult)" << endl;
    cout << "Enter your choice (1-3): ";
    
    int choice = getValidInput(1, 3);
    
    switch(choice) {
        case 1:
            gridSize = 4;
            totalPairs = 8;
            break;
        case 2:
            gridSize = 6;
            totalPairs = 18;
            break;
        case 3:
            gridSize = 8;
            totalPairs = 32;
            break;
    }
}

void MemoryMatchGame::selectSpeed() {
    cout << "\nSpeed of Play:" << endl;
    cout << "1) 2 seconds (Difficult)" << endl;
    cout << "2) 4 seconds (Moderate)" << endl;
    cout << "3) 6 seconds (Easy)" << endl;
    cout << "Enter your choice (1-3): ";
    
    int choice = getValidInput(1, 3);
    
    switch(choice) {
        case 1:
            delaySeconds = 2;
            break;
        case 2:
            delaySeconds = 4;
            break;
        case 3:
            delaySeconds = 6;
            break;
    }
}

void MemoryMatchGame::loadTheme(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cout << "Error opening file: " << filename << endl;
        exit(1);
    }
    
    int i = 0;
    string word;
    while (getline(file, word) && i < 50) {
        theme50Words[i] = word;
        i++;
    }
    
    file.close();
}

void MemoryMatchGame::shuffleThemeArray() {
    random_device rd;
    mt19937 gen(rd());
    shuffle(begin(theme50Words), end(theme50Words), gen);
}

void MemoryMatchGame::populateAnswerGrid() {
    vector<string> selectedWords;
    for (int i = 0; i < totalPairs; i++) {
        selectedWords.push_back(theme50Words[i]);
        selectedWords.push_back(theme50Words[i]);
    }
    
    random_device rd;
    mt19937 gen(rd());
    shuffle(selectedWords.begin(), selectedWords.end(), gen);
    
    int index = 0;
    for (int i = 0; i < gridSize; i++) {
        for (int j = 0; j < gridSize; j++) {
            answerArray[i][j] = selectedWords[index++];
            gamePlayDisplayArray[i][j] = faceTermName;
            matchedPairs[i][j] = false;
        }
    }
}

void MemoryMatchGame::allocateArrays() {
    answerArray = new string*[gridSize];
    gamePlayDisplayArray = new string*[gridSize];
    matchedPairs = new bool*[gridSize];
    
    for (int i = 0; i < gridSize; i++) {
        answerArray[i] = new string[gridSize];
        gamePlayDisplayArray[i] = new string[gridSize];
        matchedPairs[i] = new bool[gridSize];
    }
}

void MemoryMatchGame::deallocateArrays() {
    if (answerArray != nullptr) {
        for (int i = 0; i < gridSize; i++) {
            delete[] answerArray[i];
            delete[] gamePlayDisplayArray[i];
            delete[] matchedPairs[i];
        }
        delete[] answerArray;
        delete[] gamePlayDisplayArray;
        delete[] matchedPairs;
    }
}

void MemoryMatchGame::displayGrid() {
    clearScreen();
    cout << "\n=== MEMORY MATCH: " << themeName << " ===" << endl;
    cout << "Matched: " << matchedCount << "/" << totalPairs << endl << endl;
    
    cout << "    ";
    for (int i = 0; i < gridSize; i++) {
        cout << setw(10) << i + 1;
    }
    cout << endl;
    
    cout << "    ";
    for (int i = 0; i < gridSize; i++) {
        cout << "----------";
    }
    cout << endl;
    
    for (int i = 0; i < gridSize; i++) {
        cout << setw(2) << i + 1 << " |";
        for (int j = 0; j < gridSize; j++) {
            cout << setw(10) << gamePlayDisplayArray[i][j];
        }
        cout << endl;
    }
    cout << endl;
}

void MemoryMatchGame::displayGridWithRevealed(int row1, int col1, int row2, int col2) {
    string temp1 = gamePlayDisplayArray[row1][col1];
    string temp2 = gamePlayDisplayArray[row2][col2];
    
    gamePlayDisplayArray[row1][col1] = answerArray[row1][col1];
    gamePlayDisplayArray[row2][col2] = answerArray[row2][col2];
    
    displayGrid();
    
    gamePlayDisplayArray[row1][col1] = temp1;
    gamePlayDisplayArray[row2][col2] = temp2;
}

void MemoryMatchGame::clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void MemoryMatchGame::playGame() {
    while (!isGameComplete()) {
        displayGrid();
        
        int row1, col1, row2, col2;
        
        cout << "Select FIRST square (row col): ";
        if (!selectSquare(row1, col1, 1)) {
            continue;
        }
        
        gamePlayDisplayArray[row1][col1] = answerArray[row1][col1];
        displayGrid();
        
        cout << "Select SECOND square (row col): ";
        if (!selectSquare(row2, col2, 2)) {
            gamePlayDisplayArray[row1][col1] = faceTermName;
            continue;
        }
        
        if (row1 == row2 && col1 == col2) {
            cout << "You selected the same square twice! Try again." << endl;
            gamePlayDisplayArray[row1][col1] = faceTermName;
            delay(2);
            continue;
        }
        
        displayGridWithRevealed(row1, col1, row2, col2);
        
        if (checkMatch(row1, col1, row2, col2)) {
            cout << "MATCH! Great job!" << endl;
            gamePlayDisplayArray[row1][col1] = answerArray[row1][col1];
            gamePlayDisplayArray[row2][col2] = answerArray[row2][col2];
            matchedPairs[row1][col1] = true;
            matchedPairs[row2][col2] = true;
            matchedCount++;
            delay(2);
        } else {
            cout << "No match. Try again!" << endl;
            delay(delaySeconds);
            gamePlayDisplayArray[row1][col1] = faceTermName;
            gamePlayDisplayArray[row2][col2] = faceTermName;
        }
    }
    
    displayGrid();
    cout << "\nCONGRATULATIONS! You've matched all pairs!" << endl;
}

bool MemoryMatchGame::selectSquare(int& row, int& col, int squareNumber) {
    cin >> row >> col;
    
    if (cin.fail()) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Invalid input! Please enter two numbers." << endl;
        delay(2);
        return false;
    }
    
    row--;
    col--;
    
    if (row < 0 || row >= gridSize || col < 0 || col >= gridSize) {
        cout << "Invalid position! Please select within the grid." << endl;
        delay(2);
        return false;
    }
    
    if (matchedPairs[row][col]) {
        cout << "This square has already been matched!" << endl;
        delay(2);
        return false;
    }
    
    return true;
}

bool MemoryMatchGame::checkMatch(int row1, int col1, int row2, int col2) {
    return answerArray[row1][col1] == answerArray[row2][col2];
}

bool MemoryMatchGame::isGameComplete() {
    return matchedCount == totalPairs;
}

void MemoryMatchGame::delay(int seconds) {
    this_thread::sleep_for(chrono::seconds(seconds));
}

void MemoryMatchGame::displayTimer() {
    auto duration = chrono::duration_cast<chrono::seconds>(endTime - startTime);
    cout << "\nTime taken: " << duration.count() << " seconds" << endl;
}

int MemoryMatchGame::getValidInput(int min, int max) {
    int input;
    while (true) {
        cin >> input;
        if (cin.fail() || input < min || input > max) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input! Please enter a number between " << min << " and " << max << ": ";
        } else {
            return input;
        }
    }
}
