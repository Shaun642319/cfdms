#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "order.h"

#define ORDER_FILE "data/orders.txt"
#define MENU_FILE "data/menu.txt"
#define STUDENT_FILE "data/student.txt"

static void saveStudentBalance(Student *s);

// ==========================================================
// Place Order (Student side with balance check)
// ==========================================================
void placeOrderFromStudent(Student *s, const char *restaurantId) {
    FILE *menuFp = fopen(MENU_FILE, "r");
    if (!menuFp) {
        printf("❌ Could not open menu.txt\n");
        return;
    }

    // Temporary arrays to hold items for selection
    char itemIds[100][20], itemNames[100][50];
    double prices[100];
    int stocks[100];
    int count = 0;

    char itemId[20], rId[20], itemName[50];
    double price;
    int stock;

    printf("\n--- Available Items ---\n");
    while (fscanf(menuFp, "%19[^,],%19[^,],%49[^,],%lf,%d\n",
                  itemId, rId, itemName, &price, &stock) == 5) {
        if (strcmp(rId, restaurantId) == 0 && stock > 0) {
            // Save into arrays for later reference
            strcpy(itemIds[count], itemId);
            strcpy(itemNames[count], itemName);
            prices[count] = price;
            stocks[count] = stock;

            printf("%d. %s - $%.2f (Stock: %d)\n",
                   count + 1, itemName, price, stock);
            count++;
        }
    }
    fclose(menuFp);

    if (count == 0) {
        printf("❌ No available items for this restaurant.\n");
        return;
    }

    int choice, quantity;
    printf("Enter menu number to order: ");
    scanf("%d", &choice);

    if (choice < 1 || choice > count) {
        printf("❌ Invalid choice.\n");
        return;
    }

    printf("Enter quantity: ");
    scanf("%d", &quantity);

    if (quantity > stocks[choice - 1]) {
        printf("❌ Not enough stock.\n");
        return;
    }

    double totalPrice = prices[choice - 1] * quantity;

    // Check balance
    if (s->balance < totalPrice) {
        printf("❌ Insufficient balance. You have $%.2f, need $%.2f.\n",
               s->balance, totalPrice);
        return;
    }

    // Deduct balance
    s->balance -= totalPrice;
    saveStudentBalance(s);

    // Generate Order ID
    FILE *orderFp = fopen(ORDER_FILE, "r");
    int orderCount = 0;
    char buffer[256];
    if (orderFp) {
        while (fgets(buffer, sizeof(buffer), orderFp)) orderCount++;
        fclose(orderFp);
    }
    char orderId[20];
    sprintf(orderId, "O%03d", orderCount + 1);

    // Save order
    orderFp = fopen(ORDER_FILE, "a");
    fprintf(orderFp, "%s,%s,%s,%s,%d,%.2f,Pending,NA\n",
            orderId, s->id, restaurantId, itemIds[choice - 1],
            quantity, totalPrice);
    fclose(orderFp);

    printf("✅ Order %s placed successfully! %s x%d | Total: $%.2f\n",
           orderId, itemNames[choice - 1], quantity, totalPrice);
    printf("💰 New Balance: $%.2f\n", s->balance);
}

// ==========================================================
// Save Student Balance after order
// ==========================================================
static void saveStudentBalance(Student *s) {
    FILE *fp = fopen(STUDENT_FILE, "r");
    FILE *temp = fopen("data/student_temp.txt", "w");

    if (!fp || !temp) {
        printf("❌ Error updating balance file.\n");
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

    remove(STUDENT_FILE);
    rename("data/student_temp.txt", STUDENT_FILE);
}

// ==========================================================
// View Student Order History
// ==========================================================
void viewStudentOrderHistory(const char *studentId) {
    FILE *fp = fopen(ORDER_FILE, "r");
    if (!fp) {
        printf("❌ Could not open orders.txt\n");
        return;
    }

    char orderId[20], sId[20], restaurantId[20], itemId[20], deliveryId[20];
    int quantity;
    double price;
    char status[20];
    int found = 0;

    printf("\n--- Order History for Student %s ---\n", studentId);
    while (fscanf(fp, "%19[^,],%19[^,],%19[^,],%19[^,],%d,%lf,%19[^,],%19[^\n]\n",
                  orderId, sId, restaurantId, itemId,
                  &quantity, &price, status, deliveryId) == 8) {
        if (strcmp(sId, studentId) == 0) {
            printf("[%s] Restaurant: %s | Item: %s | Qty: %d | Total: %.2f | Status: %s\n",
                   orderId, restaurantId, itemId, quantity, price, status);
            found = 1;
        }
    }

    if (!found) {
        printf("No orders found for this student.\n");
    }

    fclose(fp);
}
