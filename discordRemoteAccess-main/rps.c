#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    int playerChoice, computerChoice;
    char playAgain;
    
    // Seed the random number generator
    srand(time(NULL));
    
    do {
        // Get player's choice
        printf("\nRock Paper Scissors Game!\n");
        printf("Enter your choice:\n");
        printf("1 for Rock\n");
        printf("2 for Paper\n");
        printf("3 for Scissors\n");
        printf("Choice: ");
        scanf("%d", &playerChoice);
        
        // Validate input
        while (playerChoice < 1 || playerChoice > 3) {
            printf("Invalid choice. Please enter 1, 2, or 3: ");
            scanf("%d", &playerChoice);
        }
        
        // Generate computer's choice (1-3)
        computerChoice = (rand() % 3) + 1;
        
        // Display choices
        printf("\nYou chose: ");
        switch(playerChoice) {
            case 1: printf("Rock\n"); break;
            case 2: printf("Paper\n"); break;
            case 3: printf("Scissors\n"); break;
        }
        
        printf("Computer chose: ");
        switch(computerChoice) {
            case 1: printf("Rock\n"); break;
            case 2: printf("Paper\n"); break;
            case 3: printf("Scissors\n"); break;
        }
        
        // Determine winner
        if (playerChoice == computerChoice) {
            printf("\nIt's a tie!\n");
        } else if ((playerChoice == 1 && computerChoice == 3) ||
                   (playerChoice == 2 && computerChoice == 1) ||
                   (playerChoice == 3 && computerChoice == 2)) {
            printf("\nYou win!\n");
        } else {
            printf("\nComputer wins!\n");
        }
        
        // Ask to play again
        printf("\nWould you like to play again? (y/n): ");
        scanf(" %c", &playAgain);
        
    } while (playAgain == 'y' || playAgain == 'Y');
    
    printf("\nThanks for playing!\n");
    return 0;
}
