#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "admin.h"

#define STUDENT_FILE "data/student.txt"
#define RESTAURANT_FILE "data/restaurant.txt"
#define DELIVERY_FILE "data/delivery.txt"
#define ORDER_FILE "data/orders.txt"

// ==========================================================
// Admin Main Menu
// ==========================================================
void adminMenu(Admin *a) {
    int choice;

    while (1) {
        printf("\n========================================\n");
        printf("        ADMIN DASHBOARD - Welcome %s %s\n", a->firstName, a->lastName);
        printf("========================================\n");
        printf("| 1. View All Students                 |\n");
        printf("| 2. View All Restaurants              |\n");
        printf("| 3. View All Delivery Personnel       |\n");
        printf("| 4. Generate System Sales Report      |\n");
        printf("| 5. Generate Delivery Performance     |\n");
        printf("| 6. Back to Main Menu                 |\n");
        printf("========================================\n");
        printf(">> ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: viewAllStudents(); break;
            case 2: viewAllRestaurants(); break;
            case 3: viewAllDeliveries(); break;
            case 4: systemSalesReport(); break;
            case 5: systemDeliveryReport(); break;
            case 6: return;
            default: printf("❌ Invalid option. Please try again.\n");
        }
    }
}


// ==========================================================
// View Students
// ==========================================================
void viewAllStudents() {
    FILE *fp = fopen(STUDENT_FILE, "r");
    if (!fp) {
        printf("❌ Could not open %s\n", STUDENT_FILE);
        return;
    }

    char id[20], first[50], last[50], username[50], email[50], password[20];
    double balance;

    printf("\n--- All Students ---\n");
    while (fscanf(fp, "%19[^,],%49[^,],%49[^,],%49[^,],%49[^,],%19[^,],%lf\n",
                  id, first, last, username, email, password, &balance) == 7) {
        printf("[%s] %s %s | %s | Balance: %.2f\n", id, first, last, email, balance);
    }

    fclose(fp);
}

// ==========================================================
// View Restaurants
// ==========================================================
void viewAllRestaurants() {
    FILE *fp = fopen(RESTAURANT_FILE, "r");
    if (!fp) {
        printf("❌ Could not open %s\n", RESTAURANT_FILE);
        return;
    }

    char id[20], name[50], username[50], email[50], password[20];

    printf("\n--- All Restaurants ---\n");
    while (fscanf(fp, "%19[^,],%49[^,],%49[^,],%49[^,],%19[^\n]\n",
                  id, name, username, email, password) == 5) {
        printf("[%s] %s | %s\n", id, name, email);
    }

    fclose(fp);
}

// ==========================================================
// View Deliveries
// ==========================================================
void viewAllDeliveries() {
    FILE *fp = fopen(DELIVERY_FILE, "r");
    if (!fp) {
        printf("❌ Could not open %s\n", DELIVERY_FILE);
        return;
    }

    char id[20], first[50], last[50], username[50], email[50], password[20];

    printf("\n--- All Delivery Personnel ---\n");
    while (fscanf(fp, "%19[^,],%49[^,],%49[^,],%49[^,],%49[^,],%19[^\n]\n",
                  id, first, last, username, email, password) == 6) {
        printf("[%s] %s %s | %s\n", id, first, last, email);
    }

    fclose(fp);
}

// ==========================================================
// System Sales Report (sum across all restaurants)
// ==========================================================
void systemSalesReport() {
    FILE *fp = fopen("data/orders.txt", "r");
    if (!fp) {
        printf("❌ Could not open orders.txt\n");
        return;
    }

    char orderId[20], studentId[20], restaurantId[20], itemId[20], deliveryId[20];
    int quantity;
    double price, totalRevenue = 0;
    char status[20];
    int delivered = 0;

    printf("\n========================================\n");
    printf("         SYSTEM SALES REPORT            \n");
    printf("========================================\n");
    printf("| %-8s | %-12s | %-8s |\n", "OrderID", "RestaurantID", "Amount");
    printf("----------------------------------------\n");

    while (fscanf(fp, "%19[^,],%19[^,],%19[^,],%19[^,],%d,%lf,%19[^,],%19[^\n]\n",
                  orderId, studentId, restaurantId, itemId,
                  &quantity, &price, status, deliveryId) == 8) {

        if (strcmp(status, "Delivered") == 0) {
            printf("| %-8s | %-12s | $%-7.2f |\n", orderId, restaurantId, price);
            totalRevenue += price;
            delivered++;
        }
    }

    printf("----------------------------------------\n");
    printf(" Total Delivered Orders: %d\n", delivered);
    printf(" Total Revenue: $%.2f\n", totalRevenue);
    printf("========================================\n");

    fclose(fp);
}

// ==========================================================
// Delivery Performance Report
// ==========================================================
void systemDeliveryReport() {
    FILE *fp = fopen("data/orders.txt", "r");
    if (!fp) {
        printf("❌ Could not open %s\n", ORDER_FILE);
        return;
    }

    char orderId[20], studentId[20], restaurantId[20], itemId[20], deliveryId[20];
    int quantity;
    double price;
    char status[20];

    int delivered = 0, outForDelivery = 0, ready = 0, pending = 0, cancelled = 0;

    printf("\n========================================\n");
    printf("       DELIVERY PERFORMANCE REPORT       \n");
    printf("========================================\n");
    printf("| %-8s | %-15s | %-12s |\n", "OrderID", "Status", "DeliveryID");
    printf("------------------------------------------------\n");

    while (fscanf(fp, "%19[^,],%19[^,],%19[^,],%19[^,],%d,%lf,%19[^,],%19[^\n]\n",
                  orderId, studentId, restaurantId, itemId,
                  &quantity, &price, status, deliveryId) == 8) {
        printf("| %-8s | %-15s | %-12s |\n", orderId, status, deliveryId);

        if (strcmp(status, "Delivered") == 0) delivered++;
        else if (strcmp(status, "OutForDelivery") == 0) outForDelivery++;
        else if (strcmp(status, "ReadyForDelivery") == 0) ready++;
        else if (strcmp(status, "Pending") == 0) pending++;
        else if (strcmp(status, "Cancelled") == 0) cancelled++;
    }
    fclose(fp);

    printf("------------------------------------------------\n");
    printf(" Delivered: %d | OutForDelivery: %d | Ready: %d | Pending: %d | Cancelled: %d\n",
           delivered, outForDelivery, ready, pending, cancelled);
    printf("========================================\n");
}
