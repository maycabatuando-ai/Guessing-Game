#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LENGTH 15
#define MAX_STACK 10
#define MAX_QUEUE 10

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

int main() {
    int mainChoice;
    char continueChoice;

    printLine('=', 40);
    printf("            GUESSING GAME SYSTEM       \n");
    printf("  (Arrays, Stack, Queue | Sort, Search)\n");
    printLine('=', 40);

    do {
        printf("\n");
        printLine('-', 20);
        printf("     MAIN MENU\n");
        printLine('-', 20);
        printf("1. Play NUMBER Guessing Game\n");
        printf("2. Play WORD Guessing Game\n");
        printf("3. EXIT\n");
        printLine('-', 20);
        printf("Enter your choice: ");
        scanf("%d", &mainChoice);
        while(getchar() != '\n');

        switch(mainChoice) {
            case 1: playNumberGame(); break;
            case 2: playWordGame(); break;
            case 3: 
                printf("\n");
                printLine('*', 40);
                printf("    Thank you for playing! Goodbye.\n");
                printLine('*', 40);
                return 0;
            default: 
                printf("\nInvalid Input! Please try again.\n");
        }

        printf("\n");
        printLine('-', 40);
        printf("Do you wish to play another game? (Y/N): ");
        scanf(" %c", &continueChoice);
        printLine('-', 40);

    } while(toupper(continueChoice) == 'Y');

    printf("\n");
    printLine('*', 40);
    printf("        Game Over. See you next time!\n");
    printLine('*', 40);
    return 0;
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
    printf("     NUMBER GUESSING GAME\n");
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
            printf("Please enter a number within range 1-10!\n");
            continue;
        }

        sprintf(tempStr, "%d", userGuess);
        push(tempStr);

        if (userGuess == secretNum) {
            printf("\n CORRECT! The secret number was [%d]\n", secretNum);
            printf(" Total Attempts: %d\n", attempts);
            displayHistory();
            found = 1;
        } 
        else if (userGuess < secretNum) {
            printf("Too LOW! Try higher.\n");
        } 
        else {
            printf("Too HIGH! Try lower.\n");
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
    printf("WORD GUESSING GAME\n");
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
        default: printf("Invalid Category!\n"); return;
    }

    printf("\nCategory Selected: [%s]\n", categoryName);

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
            printf("\nCORRECT! The secret word was [%s]\n", wordList[secretIndex]);
            printf("Total Attempts: %d\n", attempts);
            displayHistory();
            found = 1;
        } 
        else {
            printf("WRONG GUESS! ");
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
    
    printf("Hint: ");
    for(int i = 0; i < len; i++) {
        if(i < revealedLen) {
            printf("%c ", fullWord[i]);
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
    printf("\nYour Guess History: ");
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