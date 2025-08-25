# Campus Food Delivery Management System (CFDMS)

## 📌 Overview

This project is a **menu-driven console application** built in C to simulate a **Campus Food Delivery Management System (CFDMS)**.
It supports four roles:

* **Students**: Place orders, top-up balance, view order history, update profile.
* **Restaurants**: Manage menus, view/process orders, assign delivery drivers, generate sales reports.
* **Delivery Personnel**: View assigned orders, update delivery status.
* **Admin**: Manage system reports (students, restaurants, deliveries, sales, performance).

All data is stored in **CSV-like `.txt` files** under the `data/` directory.

---

## 📂 Project Structure

```
├── main.c
├── student.c / student.h
├── restaurant.c / restaurant.h
├── delivery.c / delivery.h
├── admin.c / admin.h
├── order.c / order.h
├── utils.c / utils.h
├── Makefile
├── data/
│   ├── users.txt
│   ├── student.txt
│   ├── restaurant.txt
│   ├── delivery.txt
│   ├── admin.txt
│   ├── menu.txt
│   ├── orders.txt
```

---

## ⚙️ Setup Instructions

1. **Clone or download** the project.
2. Ensure you have `gcc` installed:

   ```bash
   gcc --version
   ```
3. Make sure your `data/` folder contains all the required `.txt` files (sample data provided).

---

## ▶️ Build & Run

### 🔹 Using the Makefile (preferred)

Build the full project:

```bash
make all
```

Run the project:

```bash
make run
```

Clean object files & executable:

```bash
make clean
```

Build & run a single file (for testing):

```bash
make student
```

---

### 🔹 Without Makefile (manual compile)

If `make` is not available on your system, you can directly compile and run with `gcc`:

```bash
gcc -Wall -g main.c student.c restaurant.c delivery.c admin.c order.c utils.c -o cfds
```

Run the program:

```bash
./cfds       # On Linux / Git Bash
.\cfds.exe   # On Windows PowerShell / CMD
```

---

## 👥 User Roles & Navigation

### 1. **Student Dashboard**

* View Profile
* Update Profile
* Place Order (browse restaurants, select items, pay with balance)
* Top-up Balance (E-Wallet / Card simulation)
* View Order History

### 2. **Restaurant Dashboard**

* Manage Menu (Add / Update / Delete)
* View Orders & Update Status (assign drivers, mark orders)
* Generate Sales Report

### 3. **Delivery Dashboard**

* View Assigned Orders
* Update Delivery Status (OutForDelivery / Delivered)

### 4. **Admin Dashboard**

* View All Students
* View All Restaurants
* View All Delivery Personnel
* System Sales Report
* Delivery Performance Report

---

## 🗄️ Data Files

* `users.txt` → Stores login credentials for all roles.
* `student.txt` → Student profiles + balances.
* `restaurant.txt` → Restaurant accounts.
* `delivery.txt` → Delivery personnel accounts.
* `admin.txt` → Admin accounts.
* `menu.txt` → Restaurant menu items.
* `orders.txt` → All placed orders (linked to students, restaurants, and deliveries).

---

## 📌 Example Run

```text
========================================
          MAIN MENU
========================================
1. Student Login
2. Restaurant Login
3. Delivery Login
4. Admin Login
5. Exit
>> 1
```

From here, select your role and interact with the system.

---

✨ That’s it! Now you can build, run, and test the **Campus Food Delivery Management System** easily — either with the `make` commands or the direct `gcc` command if `make` isn’t available.

