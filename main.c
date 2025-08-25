#include <stdio.h>
#include <stdbool.h>
#include "utils.h"

int main() {
    int option, option2;
    bool running = true;

    while (running) {
        // Main Menu
        printf("\n========================================\n");
        printf("        CAMPUS FOOD DELIVERY SYSTEM     \n");
        printf("========================================\n");
        printf("| 1. Student                            |\n");
        printf("| 2. Restaurant                         |\n");
        printf("| 3. Delivery Personnel                 |\n");
        printf("| 4. Admin                              |\n");
        printf("| 5. Exit                               |\n");
        printf("========================================\n");
        printf(">> ");
        scanf("%d", &option);

        if (option < 1 || option > 5) {
            while (getchar() != '\n'); 
            printf("❌ Invalid input, please select a valid option.\n");
            continue;
        } 
        if (option == 5) {
            printf("👋 Goodbye! Thank you for using Campus Food Delivery.\n"); 
            running = false;
            break;
        } 
        if (option == 4) {
            login("Admin");  // Admin skips signup
            continue;
        }

        // Sign In / Sign Up Menu
        printf("\n========================================\n");
        printf("               SIGN IN / SIGN UP        \n");
        printf("========================================\n");
        printf("| 1. Login                              |\n");
        printf("| 2. Register                           |\n");
        printf("========================================\n");
        printf(">> ");
        scanf("%d", &option2);

        if (option2 < 1 || option2 > 2) {
            while (getchar() != '\n');
            printf("❌ Invalid input, please enter a number.\n");
            continue;
        }

        switch (option) {
            case 1: (option2 == 2) ? signUp("Student") : login("Student"); break;
            case 2: (option2 == 2) ? signUp("Restaurant") : login("Restaurant"); break;
            case 3: (option2 == 2) ? signUp("Delivery") : login("Delivery"); break;
            default: printf("❌ Invalid option, please try again.\n");
        }
    }

    return 0;
}
