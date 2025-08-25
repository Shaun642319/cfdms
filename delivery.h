#ifndef DELIVERY_H
#define DELIVERY_H

typedef struct {
    char id[20];
    char firstName[50];
    char lastName[50];
    char username[50];
    char email[50];
    char password[20];
} Delivery;

// Dashboard
void deliveryMenu(Delivery *d);

// Features
void viewAssignedOrders(Delivery *d);
void updateDeliveryStatus(Delivery *d);

#endif
