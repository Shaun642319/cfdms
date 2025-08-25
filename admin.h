#ifndef ADMIN_H
#define ADMIN_H

typedef struct {
    char id[20];
    char firstName[50];
    char lastName[50];
    char username[50];
    char email[50];
    char password[20];
} Admin;

// Main dashboard
void adminMenu(Admin *a);

// Management
void viewAllStudents();
void viewAllRestaurants();
void viewAllDeliveries();

// Reports
void systemSalesReport();
void systemDeliveryReport();

#endif
