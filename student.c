#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "student.h"
#include "order.h"  
#include "utils.h"

// Forward declarations of helpers
static void saveStudentUpdate(Student *s);

void studentMenu(Student *loggedInStudent) {
    int choice;

    while (1) {
        printf("\n========================================\n");
        printf("     STUDENT DASHBOARD - %s %s\n", loggedInStudent->firstName, loggedInStudent->lastName);
        printf("========================================\n");
        printf("| 1. View Profile                      |\n");
        printf("| 2. Update Profile                    |\n");
        printf("| 3. Place Order                       |\n");
        printf("| 4. View Order History                |\n");
        printf("| 5. Top-Up Balance                    |\n");
        printf("| 6. Back to Main Menu                 |\n");
        printf("========================================\n");
        printf(">> ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: viewProfile(loggedInStudent); break;
            case 2: updateProfile(loggedInStudent); break;
            case 3: placeOrder(loggedInStudent); break;
            case 4: viewOrderHistory(loggedInStudent); break;
            case 5: topUpBalance(loggedInStudent); break;
            case 6: return;
            default: printf("❌ Invalid option. Please try again.\n");
        }
    }
}

// View Profile
void viewProfile(Student *s) {
    printf("\n--- Student Profile ---\n");
    printf("ID: %s\n", s->id);
    printf("Name: %s %s\n", s->firstName, s->lastName);
    printf("Username: %s\n", s->username);
    printf("Email: %s\n", s->email);
    printf("Balance: %.2f\n", s->balance);
}

// Update Profile 
void updateProfile(Student *s) {
    int choice;
    char newValue[100];

    printf("\n--- Update Profile ---\n");
    printf("1. Update First Name\n");
    printf("2. Update Last Name\n");
    printf("3. Update Email\n");
    printf("4. Update Password\n");
    printf(">> ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            printf("Enter new first name: ");
            scanf("%s", newValue);
            strcpy(s->firstName, newValue);
            break;
        case 2:
            printf("Enter new last name: ");
            scanf("%s", newValue);
            strcpy(s->lastName, newValue);
            break;
        case 3:
            printf("Enter new email: ");
            scanf("%s", newValue);
            strcpy(s->email, newValue);
            break;
        case 4:
            printf("Enter new password: ");
            scanf("%s", newValue);
            strcpy(s->password, newValue);
            updateUserCredentials(s->username, s->password, "Student");
            break;
        default:
            printf("Invalid option.\n");
            return;
    }

    // save changes to file
    saveStudentUpdate(s);
    printf("✅ Profile updated successfully!\n");
}

// Save Student Update to students.txt
// (rewrite entire file, replacing this student's record)
static void saveStudentUpdate(Student *s) {
    FILE *fp = fopen("data/student.txt", "r");
    FILE *temp = fopen("data/student_temp.txt", "w");

    if (!fp || !temp) {
        printf("Error opening file.\n");
        return;
    }

    Student tempStudent;
    while (fscanf(fp, "%19[^,],%49[^,],%49[^,],%49[^,],%49[^,],%19[^,],%lf\n",
                  tempStudent.id, tempStudent.firstName, tempStudent.lastName,
                  tempStudent.username, tempStudent.email,
                  tempStudent.password, &tempStudent.balance) == 7) {

        if (strcmp(tempStudent.id, s->id) == 0) {
            fprintf(temp, "%s,%s,%s,%s,%s,%s,%.2f\n",
                    s->id, s->firstName, s->lastName,
                    s->username, s->email, s->password, s->balance);
        } else {
            fprintf(temp, "%s,%s,%s,%s,%s,%s,%.2f\n",
                    tempStudent.id, tempStudent.firstName, tempStudent.lastName,
                    tempStudent.username, tempStudent.email,
                    tempStudent.password, tempStudent.balance);
        }
    }

    fclose(fp);
    fclose(temp);

    remove("data/student.txt");
    rename("data/student_temp.txt", "data/student.txt");
}

// Place Order 
void placeOrder(Student *s) {
    printf("\n--- Place Order ---\n");
    printf("Welcome %s!\n", s->firstName);

    FILE *fp = fopen("data/restaurant.txt", "r");
    if (!fp) {
        printf("❌ Could not open restaurant list.\n");
        return;
    }

    char id[20], name[100], username[50], email[100], password[50];
    char restaurantIds[100][20];  
    int count = 0;

    printf("\n--- Available Restaurants ---\n");
    while (fscanf(fp, "%19[^,],%99[^,],%49[^,],%99[^,],%49[^\n]\n",
                  id, name, username, email, password) == 5) {
        printf("%d. [%s] %s\n", count + 1, id, name);
        strcpy(restaurantIds[count], id);
        count++;
    }
    fclose(fp);

    if (count == 0) {
        printf("No restaurants available at the moment.\n");
        return;
    }

    int choice;
    printf("\nEnter the number of the restaurant: ");
    scanf("%d", &choice);

    if (choice < 1 || choice > count) {
        printf("❌ Invalid choice.\n");
        return;
    }

    // map choice to restaurant ID
    char selectedId[20];
    strcpy(selectedId, restaurantIds[choice - 1]);

    // pass to order system
    placeOrderFromStudent(s, selectedId);
}


// View Order History 
void viewOrderHistory(Student *s) {
    viewStudentOrderHistory(s->id);  
}

void topUpBalance(Student *s) {
    double amount;
    int method;

    printf("\n--- Top-Up Balance ---\n");
    printf("Enter amount to top-up: $");
    scanf("%lf", &amount);

    if (amount <= 0) {
        printf("❌ Invalid amount.\n");
        return;
    }

    printf("Select payment method:\n");
    printf("1. E-Wallet\n");
    printf("2. Debit/Credit Card\n");
    printf(">> ");
    scanf("%d", &method);

    switch (method) {
        case 1:
            printf("Processing E-Wallet payment...\n");
            break;
        case 2:
            printf("Processing Card payment...\n");
            break;
        default:
            printf("❌ Invalid payment method.\n");
            return;
    }

    s->balance += amount;
    saveStudentUpdate(s);  
    printf("✅ Top-up successful! New Balance: $%.2f\n", s->balance);
}
