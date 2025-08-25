#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "restaurant.h"

#define MENU_FILE "data/menu.txt"

// ==========================================================
// Restaurant Main Menu
// ==========================================================
void restaurantMenu(Restaurant *r) {
    int choice;

    while (1) {
        printf("\n========================================\n");
        printf("   RESTAURANT DASHBOARD - %s\n", r->name);
        printf("========================================\n");
        printf("| 1. View Menu Items                   |\n");
        printf("| 2. Add Menu Item                     |\n");
        printf("| 3. Update Menu Item                  |\n");
        printf("| 4. Delete Menu Item                  |\n");
        printf("| 5. View Orders                       |\n");
        printf("| 6. Generate Sales Report             |\n");
        printf("| 7. Back to Main Menu                 |\n");
        printf("========================================\n");
        printf(">> ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: viewMenuItems(r); break;
            case 2: addMenuItem(r); break;
            case 3: updateMenuItem(r); break;
            case 4: deleteMenuItem(r); break;
            case 5: viewOrders(r); break;
            case 6: generateSalesReport(r); break;
            case 7: return;
            default: printf("❌ Invalid option. Please try again.\n");
        }
    }
}

// ==========================================================
// View Menu Items
// ==========================================================
void viewMenuItems(Restaurant *r) {
    FILE *fp = fopen(MENU_FILE, "r");
    if (!fp) {
        printf("❌ Could not open %s\n", MENU_FILE);
        return;
    }

    char itemId[20], restaurantId[20], itemName[50];
    double price;
    int stock;
    int found = 0;

    printf("\n--- Menu Items for %s ---\n", r->name);

    while (fscanf(fp, "%19[^,],%19[^,],%49[^,],%lf,%d\n",
                  itemId, restaurantId, itemName, &price, &stock) == 5) {
        if (strcmp(restaurantId, r->id) == 0) {
            printf("[%s] %s - $%.2f (Stock: %d)\n",
                   itemId, itemName, price, stock);
            found = 1;
        }
    }

    if (!found) {
        printf("No menu items found.\n");
    }

    fclose(fp);
}

// ==========================================================
// Add Menu Item
// ==========================================================
void addMenuItem(Restaurant *r) {
    FILE *fp = fopen(MENU_FILE, "a+");
    if (!fp) {
        printf("❌ Could not open %s\n", MENU_FILE);
        return;
    }

    // Count existing records to generate new ID
    FILE *countFp = fopen(MENU_FILE, "r");
    int count = 0;
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), countFp)) {
        count++;
    }
    fclose(countFp);

    char itemId[20], itemName[50];
    double price;
    int stock;

    sprintf(itemId, "M%03d", count + 1);

    printf("\n--- Add Menu Item ---\n");
    printf("Generated Item ID: %s\n", itemId);
    printf("Enter item name: ");
    scanf(" %[^\n]", itemName);  // allow spaces
    printf("Enter price: ");
    scanf("%lf", &price);
    printf("Enter stock: ");
    scanf("%d", &stock);

    fprintf(fp, "%s,%s,%s,%.2f,%d\n",
            itemId, r->id, itemName, price, stock);

    fclose(fp);

    printf("✅ Item '%s' added successfully!\n", itemName);
}

// ==========================================================
// Placeholders (to be implemented later)
// ==========================================================
void updateMenuItem(Restaurant *r) {
    FILE *fp = fopen(MENU_FILE, "r");
    if (!fp) {
        printf("❌ Could not open %s\n", MENU_FILE);
        return;
    }

    char itemId[20], restaurantId[20], itemName[50];
    double price;
    int stock;

    // Step 1: Show current items
    printf("\n--- Update Menu Item ---\n");
    printf("Your current items:\n");

    int found = 0;
    while (fscanf(fp, "%19[^,],%19[^,],%49[^,],%lf,%d\n",
                  itemId, restaurantId, itemName, &price, &stock) == 5) {
        if (strcmp(restaurantId, r->id) == 0) {
            printf("[%s] %s - $%.2f (Stock: %d)\n",
                   itemId, itemName, price, stock);
            found = 1;
        }
    }
    fclose(fp);

    if (!found) {
        printf("❌ No menu items to update.\n");
        return;
    }

    // Step 2: Ask which item to update
    char targetId[20];
    printf("Enter Item ID to update: ");
    scanf("%19s", targetId);

    // Step 3: Rewrite file with updates
    fp = fopen(MENU_FILE, "r");
    FILE *temp = fopen("data/menu_temp.txt", "w");
    if (!fp || !temp) {
        printf("❌ File error.\n");
        return;
    }

    while (fscanf(fp, "%19[^,],%19[^,],%49[^,],%lf,%d\n",
                  itemId, restaurantId, itemName, &price, &stock) == 5) {
        if (strcmp(restaurantId, r->id) == 0 && strcmp(itemId, targetId) == 0) {
            printf("\nUpdating %s...\n", itemName);

            char newName[50];
            double newPrice;
            int newStock;

            printf("Enter new name (current: %s): ", itemName);
            scanf(" %[^\n]", newName);
            printf("Enter new price (current: %.2f): ", price);
            scanf("%lf", &newPrice);
            printf("Enter new stock (current: %d): ", stock);
            scanf("%d", &newStock);

            fprintf(temp, "%s,%s,%s,%.2f,%d\n",
                    itemId, restaurantId, newName, newPrice, newStock);
            printf("✅ Item updated!\n");
        } else {
            // Write unchanged item
            fprintf(temp, "%s,%s,%s,%.2f,%d\n",
                    itemId, restaurantId, itemName, price, stock);
        }
    }

    fclose(fp);
    fclose(temp);

    remove(MENU_FILE);
    rename("data/menu_temp.txt", MENU_FILE);
}

void deleteMenuItem(Restaurant *r) {
    FILE *fp = fopen(MENU_FILE, "r");
    if (!fp) {
        printf("❌ Could not open %s\n", MENU_FILE);
        return;
    }

    char itemId[20], restaurantId[20], itemName[50];
    double price;
    int stock;

    // Step 1: Show current items
    printf("\n--- Delete Menu Item ---\n");
    printf("Your current items:\n");

    int found = 0;
    while (fscanf(fp, "%19[^,],%19[^,],%49[^,],%lf,%d\n",
                  itemId, restaurantId, itemName, &price, &stock) == 5) {
        if (strcmp(restaurantId, r->id) == 0) {
            printf("[%s] %s - $%.2f (Stock: %d)\n",
                   itemId, itemName, price, stock);
            found = 1;
        }
    }
    fclose(fp);

    if (!found) {
        printf("❌ No menu items to delete.\n");
        return;
    }

    // Step 2: Ask which item to delete
    char targetId[20];
    printf("Enter Item ID to delete: ");
    scanf("%19s", targetId);

    // Step 3: Rewrite file without the target item
    fp = fopen(MENU_FILE, "r");
    FILE *temp = fopen("data/menu_temp.txt", "w");
    if (!fp || !temp) {
        printf("❌ File error.\n");
        return;
    }

    int deleted = 0;
    while (fscanf(fp, "%19[^,],%19[^,],%49[^,],%lf,%d\n",
                  itemId, restaurantId, itemName, &price, &stock) == 5) {
        if (strcmp(restaurantId, r->id) == 0 && strcmp(itemId, targetId) == 0) {
            printf("✅ Deleted item: %s (%s)\n", itemName, itemId);
            deleted = 1;
            // Skip writing this line → effectively deletes it
        } else {
            fprintf(temp, "%s,%s,%s,%.2f,%d\n",
                    itemId, restaurantId, itemName, price, stock);
        }
    }

    fclose(fp);
    fclose(temp);

    remove(MENU_FILE);
    rename("data/menu_temp.txt", MENU_FILE);

    if (!deleted) {
        printf("❌ Item ID not found.\n");
    }
}

void viewOrders(Restaurant *r) {
    FILE *fp = fopen("data/orders.txt", "r");
    if (!fp) {
        printf("❌ Could not open orders.txt\n");
        return;
    }

    char orderId[20], studentId[20], restaurantId[20], itemId[20], deliveryId[20];
    int quantity;
    double price;
    char status[20];

    int found = 0;
    printf("\n--- Orders for %s ---\n", r->name);
    printf("| %-8s | %-10s | %-6s | %-10s | %-12s |\n",
           "OrderID", "Status", "Qty", "Amount", "DriverID");
    printf("------------------------------------------------------------\n");

    while (fscanf(fp,
                  "%19[^,],%19[^,],%19[^,],%19[^,],%d,%lf,%19[^,],%19[^\n]\n",
                  orderId, studentId, restaurantId, itemId,
                  &quantity, &price, status, deliveryId) == 8) {

        if (strcmp(restaurantId, r->id) == 0) {
            printf("| %-8s | %-10s | %-6d | $%-9.2f | %-12s |\n",
                   orderId, status, quantity, price,
                   strcmp(deliveryId, "NA") == 0 ? "Not Assigned" : deliveryId);
            found = 1;
        }
    }

    fclose(fp);

    if (!found) {
        printf("No orders for this restaurant.\n");
        return;
    }

    // Always prompt to process order
    char targetOrder[20];
    printf("\nEnter Order ID to process (or 0 to cancel): ");
    scanf("%19s", targetOrder);

    if (strcmp(targetOrder, "0") != 0) {
        processOrder(r, targetOrder);
    }
}

void processOrder(Restaurant *r, const char *targetOrder) {
    FILE *fp = fopen("data/orders.txt", "r");
    FILE *temp = fopen("data/orders_temp.txt", "w");
    if (!fp || !temp) {
        printf("❌ File error.\n");
        return;
    }

    char orderId[20], studentId[20], restaurantId[20], itemId[20], deliveryId[20];
    int quantity;
    double price;
    char status[20];
    int updated = 0;

    while (fscanf(fp,
                  "%19[^,],%19[^,],%19[^,],%19[^,],%d,%lf,%19[^,],%19[^\n]\n",
                  orderId, studentId, restaurantId, itemId,
                  &quantity, &price, status, deliveryId) == 8) {

        if (strcmp(restaurantId, r->id) == 0 && strcmp(orderId, targetOrder) == 0) {
            printf("\nProcessing Order %s...\n", orderId);

            // --- Status Selection ---
            int statusChoice;
            printf("Select new status:\n");
            printf("1. Pending\n");
            printf("2. ReadyForDelivery\n");
            printf("3. Cancelled\n");
            printf(">> ");
            scanf("%d", &statusChoice);

            char newStatus[20];
            switch (statusChoice) {
                case 1: strcpy(newStatus, "Pending"); break;
                case 2: strcpy(newStatus, "ReadyForDelivery"); break;
                case 3: strcpy(newStatus, "Cancelled"); break;
                default: strcpy(newStatus, "Pending");
            }

            // --- Assign Delivery Driver (only if ReadyForDelivery) ---
            char newDriverId[20];
            strcpy(newDriverId, deliveryId); // keep old one by default

            if (strcmp(newStatus, "ReadyForDelivery") == 0) {
                printf("\n--- Available Delivery Drivers ---\n");
                FILE *df = fopen("data/delivery.txt", "r");
                if (!df) {
                    printf("❌ Could not open delivery.txt\n");
                    fclose(fp);
                    fclose(temp);
                    return;
                }

                char dId[20], first[50], last[50], user[50], email[50], pass[50];
                int driverCount = 0;
                char driverIds[100][20];

                while (fscanf(df, "%19[^,],%49[^,],%49[^,],%49[^,],%49[^,],%49[^\n]\n",
                              dId, first, last, user, email, pass) == 6) {
                    driverCount++;
                    printf("%d. [%s] %s %s (%s)\n", driverCount, dId, first, last, email);
                    strcpy(driverIds[driverCount - 1], dId);
                }
                fclose(df);

                int driverChoice;
                printf("Select Driver (1-%d): ", driverCount);
                scanf("%d", &driverChoice);

                if (driverChoice >= 1 && driverChoice <= driverCount) {
                    strcpy(newDriverId, driverIds[driverChoice - 1]);
                } else {
                    strcpy(newDriverId, "NA");
                }
            }

            // --- Write updated order ---
            fprintf(temp, "%s,%s,%s,%s,%d,%.2f,%s,%s\n",
                    orderId, studentId, restaurantId, itemId,
                    quantity, price, newStatus, newDriverId);

            updated = 1;
            printf("✅ Order %s updated to %s", orderId, newStatus);
            if (strcmp(newStatus, "ReadyForDelivery") == 0) {
                printf(" and assigned to driver %s", newDriverId);
            }
            printf("!\n");

        } else {
            // keep unchanged
            fprintf(temp, "%s,%s,%s,%s,%d,%.2f,%s,%s\n",
                    orderId, studentId, restaurantId, itemId,
                    quantity, price, status, deliveryId);
        }
    }

    fclose(fp);
    fclose(temp);

    remove("data/orders.txt");
    rename("data/orders_temp.txt", "data/orders.txt");

    if (!updated) {
        printf("❌ Order ID not found.\n");
    }
}

void generateSalesReport(Restaurant *r) {
    FILE *fp = fopen("data/orders.txt", "r");
    if (!fp) {
        printf("❌ Could not open orders.txt\n");
        return;
    }

    char orderId[20], studentId[20], restaurantId[20], itemId[20], deliveryId[20];
    int quantity;
    double price, totalRevenue = 0;
    char status[20];
    int delivered = 0, ready = 0, pending = 0, cancelled = 0;

    printf("\n========================================\n");
    printf("       SALES REPORT - %s\n", r->name);
    printf("========================================\n");
    printf("| %-8s | %-15s | %-6s | %-10s |\n", "OrderID", "Status", "Qty", "Amount");
    printf("---------------------------------------------------\n");

    while (fscanf(fp, "%19[^,],%19[^,],%19[^,],%19[^,],%d,%lf,%19[^,],%19[^\n]\n",
                  orderId, studentId, restaurantId, itemId,
                  &quantity, &price, status, deliveryId) == 8) {

        if (strcmp(restaurantId, r->id) == 0) {
            printf("| %-8s | %-15s | %-6d | $%-9.2f |\n", orderId, status, quantity, price);

            if (strcmp(status, "Delivered") == 0) {
                totalRevenue += price;
                delivered++;
            } else if (strcmp(status, "ReadyForDelivery") == 0) {
                ready++;
            } else if (strcmp(status, "Pending") == 0) {
                pending++;
            } else if (strcmp(status, "Cancelled") == 0) {
                cancelled++;
            }
        }
    }

    printf("---------------------------------------------------\n");
    printf(" Delivered: %d | Ready: %d | Pending: %d | Cancelled: %d\n",
           delivered, ready, pending, cancelled);
    printf(" Total Revenue (Delivered only): $%.2f\n", totalRevenue);
    printf("========================================\n");

    fclose(fp);
}
