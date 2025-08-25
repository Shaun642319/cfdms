#ifndef ORDER_H
#define ORDER_H

#include "student.h"

// Student order flow
void placeOrderFromStudent(Student *s, const char *restaurantId);
void viewStudentOrderHistory(const char *studentId);

#endif
