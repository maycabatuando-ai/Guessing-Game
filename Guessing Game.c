#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <unistd.h>

#define MAX_LENGTH 15
#define MAX_STACK 10
#define MAX_QUEUE 10

// Color definitions
#define RESET "\033[0m"
#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define BLUE "\033[1;34m"
#define MAGENTA "\033[1;35m"
#define CYAN "\033[1;36m"
#define WHITE "\033[1;37m"

// --- DATA STRUCTURES GLOBAL DECLARATION ---
char guessStack[MAX_STACK][MAX_LENGTH];
int stackTop = -1;

char hintQueue[MAX_QUEUE][MAX_LENGTH];
int queueFront = 0;
int queueRear = -1;

// Function Prototypes
void playNumberGame();
void playWordGame();
void resetGameData();
void showHint(char *revealedPart, char *fullWord);
void push(char *item);
void displayHistory();
void enqueue(char *item);
void showNextHint(char *fullWord);
void sortNumbers(int arr[], int size);
int linearSearch(int arr[], int size, int key);
void printLine(char ch, int length);
void introAnimation();
void loadingScreen();
void exitAnimation();
void menuAnimation();

int main() {
    int mainChoice;
    char continueChoice;

    introAnimation();

    printLine('=', 40);
    printf(BLUE "            GUESSING GAME SYSTEM       \n" RESET);
    printf(YELLOW "  (Arrays, Stack, Queue | Sort, Search)\n" RESET);
    printLine('=', 40);

    do {
        menuAnimation();
        scanf("%d", &mainChoice);
        while(getchar() != '\n');

        switch(mainChoice) {
            case 1: playNumberGame(); break;
            case 2: playWordGame(); break;
            case 3: 
                exitAnimation();
                return 0;
            default: 
                printf(RED "\nInvalid Input! Please try again.\n" RESET);
        }

        printf("\n");
        printLine('-', 40);
        printf("Do you wish to play another game? (Y/N): ");
        scanf(" %c", &continueChoice);
        printLine('-', 40);

    } while(toupper(continueChoice) == 'Y');

    exitAnimation();
    return 0;
}

// Animation functions
void introAnimation() {
    printf(CYAN);
    printf("\n");
    printLine('*', 50);
    printf("         WELCOME TO THE GUESSING GAME!\n");
    printLine('*', 50);
    printf(RESET);
    usleep(500000); // 0.5 seconds
    printf(YELLOW "Loading game assets...\n" RESET);
    loadingScreen();
    printf(GREEN "Ready to play!\n" RESET);
    usleep(500000);
}

void loadingScreen() {
    printf("[");
    for(int i = 0; i < 20; i++) {
        printf("=");
        fflush(stdout);
        usleep(100000); // 0.1 seconds
    }
    printf("] 100%%\n");
}

void menuAnimation() {
    printf("\n");
    printLine('-', 20);
    printf(GREEN "     MAIN MENU\n" RESET);
    printLine('-', 20);
    
    usleep(200000);
    printf(CYAN "1. Play NUMBER Guessing Game\n" RESET);
    usleep(200000);
    printf(MAGENTA "2. Play WORD Guessing Game\n" RESET);
    usleep(200000);
    printf(RED "3. EXIT\n" RESET);
    usleep(200000);
    printLine('-', 20);
    printf("Enter your choice: ");
}

void exitAnimation() {
    printf(RED);
    printf("\n");
    printLine('*', 50);
    printf("         THANK YOU FOR PLAYING!\n");
    printLine('*', 50);
    printf(RESET);
    usleep(500000);
    printf(MAGENTA "Shutting down...\n" RESET);
    for(int i = 0; i < 5; i++) {
        printf(".");
        fflush(stdout);
        usleep(200000);
    }
    printf("\n" GREEN "Goodbye!\n" RESET);
}

// Helper function to print lines
void printLine(char ch, int length) {
    for(int i = 0; i < length; i++) {
        printf("%c", ch);
    }
    printf("\n");
}


// NUMBER GAME

void playNumberGame() {
    resetGameData();
    int numbers[10] = {5, 2, 9, 1, 7, 3, 10, 4, 8, 6};
    int secretNum, userGuess, attempts = 0;
    char tempStr[10];
    int found = 0;

    printf("\n");
    printLine('=', 30);
    printf(CYAN "     NUMBER GUESSING GAME\n" RESET);
    printLine('=', 30);
    printf("Guess a number between 1 and 10.\n");

    sortNumbers(numbers, 10);
    srand(time(0));
    secretNum = numbers[rand() % 10];

    while (!found) {
        printf("\nEnter your guess: ");
        scanf("%d", &userGuess);
        attempts++;

        if(linearSearch(numbers, 10, userGuess) == -1){
            printf(RED "Please enter a number within range 1-10!\n" RESET);
            continue;
        }

        sprintf(tempStr, "%d", userGuess);
        push(tempStr);

        if (userGuess == secretNum) {
            printf(GREEN "\n CORRECT! The secret number was [%d]\n" RESET, secretNum);
            printf(" Total Attempts: %d\n", attempts);
            displayHistory();
            found = 1;
        } 
        else if (userGuess < secretNum) {
            printf(YELLOW "Too LOW! Try higher.\n" RESET);
        } 
        else {
            printf(YELLOW "Too HIGH! Try lower.\n" RESET);
        }
    }
}

void playWordGame() {
    resetGameData();
    char fruits[10][MAX_LENGTH] = {"apple", "banana", "cherry", "coconut", "grape", 
                                   "mango", "orange", "pineapple", "strawberry", "watermelon"};
    char animals[10][MAX_LENGTH] = {"cat", "dog", "elephant", "tiger", "lion", 
                                    "giraffe", "zebra", "monkey", "penguin", "kangaroo"};
    char countries[10][MAX_LENGTH] = {"philippines", "japan", "canada", "brazil", "france", 
                                       "germany", "india", "australia", "mexico", "italy"};
    char vegetables[10][MAX_LENGTH] = {"carrot", "potato", "onion", "tomato", "cabbage",
                                       "broccoli", "spinach", "garlic", "pepper", "cucumber"};

    char (*wordList)[MAX_LENGTH];
    char categoryName[20];
    int category, wordCount = 0;

    printf("\n");
    printLine('=', 30);
    printf(CYAN "WORD GUESSING GAME\n" RESET);
    printLine('=', 30);
    printf("Select Category:\n");
    printf("1. Fruits\n2. Animals\n3. Countries\n4. Vegetables\n");
    printLine('-', 30);
    printf("Enter choice: ");
    scanf("%d", &category);

    switch(category) {
        case 1: wordList = fruits; wordCount = 10; strcpy(categoryName, "FRUITS"); break;
        case 2: wordList = animals; wordCount = 10; strcpy(categoryName, "ANIMALS"); break;
        case 3: wordList = countries; wordCount = 10; strcpy(categoryName, "COUNTRIES"); break;
        case 4: wordList = vegetables; wordCount = 10; strcpy(categoryName, "VEGETABLES"); break;
        default: printf(RED "Invalid Category!\n" RESET); return;
    }

    printf(GREEN "\nCategory Selected: [%s]\n" RESET, categoryName);

    char userInput[MAX_LENGTH];
    int secretIndex, attempts = 0;
    int found = 0;

    srand(time(0));
    secretIndex = rand() % wordCount;

    char hint1[MAX_LENGTH], hint2[MAX_LENGTH], hint3[MAX_LENGTH];
    strcpy(hint1, wordList[secretIndex]); hint1[1] = '\0';
    strcpy(hint2, wordList[secretIndex]); hint2[2] = '\0';
    strcpy(hint3, wordList[secretIndex]); hint3[3] = '\0';
    enqueue(hint1); enqueue(hint2); enqueue(hint3);

    printf("\nStart Guessing the Word!\n");
    while (!found) {
        printf("\nEnter your guess: ");
        scanf("%s", userInput);
        attempts++;

        push(userInput);

        if (strcmp(userInput, wordList[secretIndex]) == 0) {
            printf(GREEN "\nCORRECT! The secret word was [%s]\n" RESET, wordList[secretIndex]);
            printf("Total Attempts: %d\n", attempts);
            displayHistory();
            found = 1;
        } 
        else {
            printf(RED "WRONG GUESS! " RESET);
            showNextHint(wordList[secretIndex]);
        }
    }
}


// UTILITY FUNCTIONS

void resetGameData() {
    stackTop = -1;
    queueFront = 0;
    queueRear = -1;
}

void showHint(char *revealedPart, char *fullWord) {
    int len = strlen(fullWord);
    int revealedLen = strlen(revealedPart);
    
    printf(YELLOW "Hint: " RESET);
    for(int i = 0; i < len; i++) {
        if(i < revealedLen) {
            printf(GREEN "%c " RESET, fullWord[i]);
        } else {
            printf("_ ");
        }
    }
    printf("\n");
}


// STACK

void push(char *item) {
    if(stackTop < MAX_STACK - 1) {
        stackTop++;
        strcpy(guessStack[stackTop], item);
    }
}

void displayHistory() {
    printf(BLUE "\nYour Guess History: " RESET);
    for(int i = stackTop; i >= 0; i--) {
        printf("[%s] ", guessStack[i]);
    }
    printf("\n");
}


// QUEUE

void enqueue(char *item) {
    if(queueRear < MAX_QUEUE - 1) {
        queueRear++;
        strcpy(hintQueue[queueRear], item);
    }
}

void showNextHint(char *fullWord) {
    if(queueFront <= queueRear) {
        showHint(hintQueue[queueFront], fullWord);
        queueFront++;
    } else {
        printf(" No more hints available!\n");
    }
}


// ALGORITHMS
//ini ang sort ta
void sortNumbers(int arr[], int size) {
    for (int i = 0; i < size-1; i++) {
        for (int j = 0; j < size-i-1; j++) {
            if (arr[j] > arr[j+1]) {
                int temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
}
//ini ang search ta
int linearSearch(int arr[], int size, int key) {
    for (int i = 0; i < size; i++) {
        if (arr[i] == key)
            return i;
    }
    return -1;
}