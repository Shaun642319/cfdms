#ifndef UTILS_H
#define UTILS_H

// ==========================================================
// File Helpers
// ==========================================================
char ***readFile(const char *filename, int *lineCount, int **valueCounts);
void freeFileData(char ***data, int lineCount, int *valueCounts);

// ==========================================================
// Authentication
// ==========================================================
int login(const char *userType);
void signUp(const char *userType);

// ==========================================================
// Validation & Checks
// ==========================================================
int userExists(const char *username, const char *email, const char *userType);
int validatePassword(const char *password);
int validateEmail(const char *email);

void updateUserCredentials(const char *username, const char *newPassword, const char *userType);

#endif
