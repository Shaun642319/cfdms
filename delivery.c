#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "delivery.h"

#define ORDER_FILE "data/orders.txt"

// ==========================================================
// Delivery Menu
// ==========================================================
void deliveryMenu(Delivery *d) {
    int choice;

    while (1) {
        printf("\n========================================\n");
        printf("     DELIVERY DASHBOARD - %s %s\n", d->firstName, d->lastName);
        printf("========================================\n");
        printf("| 1. View Assigned Orders              |\n");
        printf("| 2. Update Delivery Status            |\n");
        printf("| 3. Back to Main Menu                 |\n");
        printf("========================================\n");
        printf(">> ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: viewAssignedOrders(d); break;
            case 2: updateDeliveryStatus(d); break;
            case 3: return;
            default: printf("❌ Invalid option. Please try again.\n");
        }
    }
}


// ==========================================================
// View Assigned Orders
// ==========================================================
void viewAssignedOrders(Delivery *d) {
    FILE *fp = fopen(ORDER_FILE, "r");
    if (!fp) {
        printf("❌ Could not open orders.txt\n");
        return;
    }

    char orderId[20], studentId[20], restaurantId[20], itemId[20], deliveryId[20];
    int quantity;
    double price;
    char status[20];
    int found = 0;

    printf("\n--- Assigned Orders ---\n");
    while (fscanf(fp, "%19[^,],%19[^,],%19[^,],%19[^,],%d,%lf,%19[^,],%19[^\n]\n",
                  orderId, studentId, restaurantId, itemId, &quantity, &price,
                  status, deliveryId) == 8) {
        if (strcmp(deliveryId, d->id) == 0) {
            printf("[%s] Student: %s | Item: %s | Qty: %d | Total: %.2f | Status: %s\n",
                   orderId, studentId, itemId, quantity, price, status);
            found = 1;
        }
    }

    if (!found) {
        printf("No orders assigned.\n");
    }

    fclose(fp);
}

// ==========================================================
// Update Delivery Status
// ==========================================================
// ==========================================================
// Update Delivery Status (with menu-style selection)
// ==========================================================
void updateDeliveryStatus(Delivery *d) {
    FILE *fp = fopen(ORDER_FILE, "r");
    if (!fp) {
        printf("❌ Could not open %s\n", ORDER_FILE);
        return;
    }

    char orderIds[100][20];
    int count = 0;

    char orderId[20], studentId[20], restaurantId[20], itemId[20], deliveryId[20];
    int quantity;
    double price;
    char status[20];

    printf("\n--- Orders Assigned to %s %s ---\n", d->firstName, d->lastName);
    while (fscanf(fp, "%19[^,],%19[^,],%19[^,],%19[^,],%d,%lf,%19[^,],%19[^\n]\n",
                  orderId, studentId, restaurantId, itemId,
                  &quantity, &price, status, deliveryId) == 8) {
        if (strcmp(deliveryId, d->id) == 0) {
            count++;
            strcpy(orderIds[count - 1], orderId);
            printf("%d. Order %s | Student: %s | Item: %s | Qty: %d | Total: %.2f | Status: %s\n",
                   count, orderId, studentId, itemId, quantity, price, status);
        }
    }
    fclose(fp);

    if (count == 0) {
        printf("No orders assigned to you.\n");
        return;
    }

    // Choose order by index
    int choice;
    printf("Enter order number to update: ");
    scanf("%d", &choice);
    if (choice < 1 || choice > count) {
        printf("❌ Invalid choice.\n");
        return;
    }
    char targetOrder[20];
    strcpy(targetOrder, orderIds[choice - 1]);

    // Rewrite file with updated status
    fp = fopen(ORDER_FILE, "r");
    FILE *temp = fopen("data/orders_temp.txt", "w");
    if (!fp || !temp) {
        printf("❌ File error.\n");
        return;
    }

    int updated = 0;
    while (fscanf(fp, "%19[^,],%19[^,],%19[^,],%19[^,],%d,%lf,%19[^,],%19[^\n]\n",
                  orderId, studentId, restaurantId, itemId,
                  &quantity, &price, status, deliveryId) == 8) {

        if (strcmp(orderId, targetOrder) == 0 && strcmp(deliveryId, d->id) == 0) {
            printf("\nCurrent Status: %s\n", status);

            // Menu-style status selection
            int statusChoice;
            printf("Select new status:\n");
            printf("1. OutForDelivery\n");
            printf("2. Delivered\n");
            printf(">> ");
            scanf("%d", &statusChoice);

            char newStatus[20];
            if (statusChoice == 1) strcpy(newStatus, "OutForDelivery");
            else if (statusChoice == 2) strcpy(newStatus, "Delivered");
            else {
                printf("❌ Invalid choice.\n");
                fclose(fp);
                fclose(temp);
                remove("data/orders_temp.txt");
                return;
            }

            fprintf(temp, "%s,%s,%s,%s,%d,%.2f,%s,%s\n",
                    orderId, studentId, restaurantId, itemId,
                    quantity, price, newStatus, deliveryId);

            updated = 1;
            printf("✅ Order %s updated to %s!\n", orderId, newStatus);

            if (strcmp(newStatus, "Delivered") == 0) {
                printf("📦 Order %s delivered successfully by %s %s (ID: %s)\n",
                       orderId, d->firstName, d->lastName, d->id);
            }
        } else {
            fprintf(temp, "%s,%s,%s,%s,%d,%.2f,%s,%s\n",
                    orderId, studentId, restaurantId, itemId,
                    quantity, price, status, deliveryId);
        }
    }

    fclose(fp);
    fclose(temp);

    remove(ORDER_FILE);
    rename("data/orders_temp.txt", ORDER_FILE);

    if (!updated) {
        printf("❌ Order not found or not assigned to you.\n");
    }
}
