#ifndef RESTAURANT_H
#define RESTAURANT_H

typedef struct {
    char id[20];
    char name[50];
    char username[50];
    char email[50];
    char password[20];
} Restaurant;

// Dashboard
void restaurantMenu(Restaurant *r);

// Menu management
void viewMenuItems(Restaurant *r);
void addMenuItem(Restaurant *r);
void updateMenuItem(Restaurant *r);
void deleteMenuItem(Restaurant *r);

// Orders
void viewOrders(Restaurant *r);
void processOrder(Restaurant *r, const char *targetOrder);

// Reports
void generateSalesReport(Restaurant *r);

#endif
