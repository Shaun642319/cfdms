#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

#include "utils.h"
#include "student.h"
#include "restaurant.h"
#include "delivery.h"
#include "admin.h"

// Read file
char ***readFile(const char *filename, int *lineCount, int **valueCounts)
{
    FILE *p = fopen(filename, "r");
    if (p == NULL)
    {
        perror("Error opening file");
        return NULL;
    }

    char buffer[256];
    char ***data = NULL; 
    *lineCount = 0;
    *valueCounts = NULL;

    while (fgets(buffer, sizeof(buffer), p))
    {
        buffer[strcspn(buffer, "\n")] = 0; // remove newline

        // grow arrays
        data = realloc(data, (*lineCount + 1) * sizeof(char **));
        *valueCounts = realloc(*valueCounts, (*lineCount + 1) * sizeof(int));

        int valueCount = 0;
        char **values = NULL;

        char *token = strtok(buffer, ",");
        while (token != NULL)
        {
            values = realloc(values, (valueCount + 1) * sizeof(char *));
            values[valueCount] = malloc(strlen(token) + 1);
            strcpy(values[valueCount], token);

            valueCount++;
            token = strtok(NULL, ",");
        }

        data[*lineCount] = values;
        (*valueCounts)[*lineCount] = valueCount;
        (*lineCount)++;
    }
    fclose(p);

    return data;
}

// Free data allocated by readFile
void freeFileData(char ***data, int lineCount, int *valueCounts)
{
    if (!data || !valueCounts)
        return;
    for (int i = 0; i < lineCount; i++)
    {
        for (int j = 0; j < valueCounts[i]; j++)
        {
            free(data[i][j]);
        }
        free(data[i]);
    }
    free(data);
    free(valueCounts);
}

// Universal Login
int login(const char *userType)
{
    int lineCount;
    int *valueCounts;
    char ***data;

    char username[50], password[50];

    printf("\n========================================\n");
    printf("             %s LOGIN                  \n", userType);
    printf("========================================\n");
    printf("Username: ");
    scanf("%49s", username);

    printf("Password: ");
    scanf("%49s", password);

    // Validate credentials from users.txt
    data = readFile("data/users.txt", &lineCount, &valueCounts);
    if (data == NULL)
        return 1;

    int found = 0;
    for (int i = 0; i < lineCount; i++)
    {
        if (strcmp(data[i][0], username) == 0)
        {
            found = 1;
            if (strcmp(data[i][1], password) == 0 && strcmp(data[i][2], userType) == 0)
            {
                printf("🎉 Login successful! Welcome, %s (%s)\n", username, userType);

                // Switch by userType
                if (strcmp(userType, "Student") == 0)
                {
                    int sc, *vc;
                    char ***sData = readFile("data/student.txt", &sc, &vc);
                    for (int j = 0; j < sc; j++)
                    {
                        if (strcmp(sData[j][3], username) == 0)
                        {
                            Student student;
                            strcpy(student.id, sData[j][0]);
                            strcpy(student.firstName, sData[j][1]);
                            strcpy(student.lastName, sData[j][2]);
                            strcpy(student.username, sData[j][3]);
                            strcpy(student.email, sData[j][4]);
                            strcpy(student.password, sData[j][5]);
                            student.balance = atof(sData[j][6]);

                            studentMenu(&student); // pass by reference
                            break;
                        }
                    }
                    freeFileData(sData, sc, vc);
                }
                else if (strcmp(userType, "Restaurant") == 0)
                {
                    int rc, *vc;
                    char ***rData = readFile("data/restaurant.txt", &rc, &vc);
                    for (int j = 0; j < rc; j++)
                    {
                        if (strcmp(rData[j][2], username) == 0)
                        {
                            Restaurant r;
                            strcpy(r.id, rData[j][0]);
                            strcpy(r.name, rData[j][1]);
                            strcpy(r.username, rData[j][2]);
                            strcpy(r.email, rData[j][3]);
                            strcpy(r.password, rData[j][4]);

                            restaurantMenu(&r);
                            break;
                        }
                    }
                    freeFileData(rData, rc, vc);
                }
                else if (strcmp(userType, "Delivery") == 0)
                {
                    int dc, *vc;
                    char ***dData = readFile("data/delivery.txt", &dc, &vc);
                    for (int j = 0; j < dc; j++)
                    {
                        if (strcmp(dData[j][3], username) == 0)
                        {
                            Delivery d;
                            strcpy(d.id, dData[j][0]);
                            strcpy(d.firstName, dData[j][1]);
                            strcpy(d.lastName, dData[j][2]);
                            strcpy(d.username, dData[j][3]);
                            strcpy(d.email, dData[j][4]);
                            strcpy(d.password, dData[j][5]);

                            deliveryMenu(&d);
                            break;
                        }
                    }
                    freeFileData(dData, dc, vc);
                }
                else if (strcmp(userType, "Admin") == 0)
                {
                    int ac, *vc;
                    char ***aData = readFile("data/admin.txt", &ac, &vc);
                    for (int j = 0; j < ac; j++)
                    {
                        if (strcmp(aData[j][3], username) == 0)
                        {
                            Admin a;
                            strcpy(a.id, aData[j][0]);
                            strcpy(a.firstName, aData[j][1]);
                            strcpy(a.lastName, aData[j][2]);
                            strcpy(a.username, aData[j][3]);
                            strcpy(a.email, aData[j][4]);
                            strcpy(a.password, aData[j][5]);

                            adminMenu(&a);
                            break;
                        }
                    }
                    freeFileData(aData, ac, vc);
                }
            }
            else
            {
                printf("❌ Incorrect password or user type\n");
            }
            break;
        }
    }
    if (!found)
    {
        printf("❌ Incorrect username\n");
    }

    // Free users.txt data
    freeFileData(data, lineCount, valueCounts);

    return 0;
}

// Sign Up
void signUp(const char *userType)
{
    char id[20], firstName[50], lastName[50], username[50], email[100], password[50];
    char restaurantName[100];
    char fileName[50];
    char prefix;

    if (strcmp(userType, "Student") == 0)
    {
        strcpy(fileName, "data/student.txt");
        prefix = 'S';
    }
    else if (strcmp(userType, "Restaurant") == 0)
    {
        strcpy(fileName, "data/restaurant.txt");
        prefix = 'R';
    }
    else if (strcmp(userType, "Delivery") == 0)
    {
        strcpy(fileName, "data/delivery.txt");
        prefix = 'D';
    }
    else if (strcmp(userType, "Admin") == 0)
    {
        strcpy(fileName, "data/admin.txt");
        prefix = 'A';
    }
    else
    {
        printf("Unknown user type!\n");
        return;
    }

    // Count existing records
    FILE *fp = fopen(fileName, "r");
    int count = 0;
    if (fp != NULL)
    {
        char buffer[256];
        while (fgets(buffer, sizeof(buffer), fp))
        {
            count++;
        }
        fclose(fp);
    }

    // Generate ID
    sprintf(id, "%c%03d", prefix, count + 1);

    printf("\n========================================\n");
    printf("           %s SIGN UP                  \n", userType);
    printf("========================================\n");
    printf("Generated ID: %s\n", id);

    // Username loop
    while (1)
    {
        printf("Choose a username: ");
        scanf("%49s", username);

        int exists = userExists(username, "", userType);
        if (exists == 1)
        {
            printf("❌ Username already exists. Try another.\n");
        }
        else
        {
            break;
        }
    }

    // Password loop
    while (1)
    {
        printf("Choose a password: ");
        scanf("%49s", password);

        if (!validatePassword(password))
        {
            printf("❌ Password must be at least 6 characters long, contain upper/lowercase, a digit, and a special character.\n");
        }
        else
        {
            break;
        }
    }

    // Email loop
    while (1)
    {
        printf("Enter email: ");
        scanf("%99s", email);

        if (!validateEmail(email))
        {
            printf("❌ Invalid email format. Try again.\n");
            continue;
        }

        int exists = userExists("", email, userType);
        if (exists == 2)
        {
            printf("❌ Email already registered. Try another.\n");
        }
        else
        {
            break;
        }
    }

    // --- Collect role-specific details ---
    if (strcmp(userType, "Student") == 0)
    {
        printf("Enter first name: ");
        scanf("%49s", firstName);
        printf("Enter last name: ");
        scanf("%49s", lastName);

        FILE *fs = fopen(fileName, "a");
        fprintf(fs, "%s,%s,%s,%s,%s,%s,0\n", id, firstName, lastName, username, email, password);
        fclose(fs);
    }
    else if (strcmp(userType, "Restaurant") == 0)
    {
        printf("Enter restaurant name: ");
        scanf(" %[^\n]", restaurantName);

        FILE *fr = fopen(fileName, "a");
        fprintf(fr, "%s,%s,%s,%s,%s\n", id, restaurantName, username, email, password);
        fclose(fr);
    }
    else if (strcmp(userType, "Delivery") == 0)
    {
        printf("Enter first name: ");
        scanf("%49s", firstName);
        printf("Enter last name: ");
        scanf("%49s", lastName);

        FILE *fd = fopen(fileName, "a");
        fprintf(fd, "%s,%s,%s,%s,%s,%s\n", id, firstName, lastName, username, email, password);
        fclose(fd);
    }

    // add credentials to users.txt
    FILE *fu = fopen("data/users.txt", "a");
    fprintf(fu, "%s,%s,%s\n", username, password, userType);
    fclose(fu);

    printf("🎉 %s account created successfully! You may now log in.\n", userType);
}

// Helpers
int userExists(const char *username, const char *email, const char *userType)
{
    FILE *fu = fopen("data/users.txt", "r");
    if (fu != NULL)
    {
        char line[256];
        while (fgets(line, sizeof(line), fu))
        {
            char fileUser[50], filePass[50], fileType[20];
            sscanf(line, "%49[^,],%49[^,],%19[^\n]", fileUser, filePass, fileType);

            if (strcmp(fileUser, username) == 0)
            {
                fclose(fu);
                return 1; // username already taken
            }
        }
        fclose(fu);
    }

    // check email in role-specific file
    char fileName[50];
    if (strcmp(userType, "Student") == 0)
    {
        strcpy(fileName, "data/student.txt");
    }
    else if (strcmp(userType, "Restaurant") == 0)
    {
        strcpy(fileName, "data/restaurant.txt");
    }
    else if (strcmp(userType, "Delivery") == 0)
    {
        strcpy(fileName, "data/delivery.txt");
    }
    else if (strcmp(userType, "Admin") == 0)
    {
        strcpy(fileName, "data/admin.txt");
    }
    else
    {
        return 0; // unknown type
    }

    FILE *fp = fopen(fileName, "r");
    if (fp != NULL)
    {
        char line[256];
        while (fgets(line, sizeof(line), fp))
        {
            char *token;
            char buffer[200];
            char *fileEmail = NULL;

            token = strtok(line, ",");
            while (token != NULL)
            {
                strcpy(buffer, token);
                token = strtok(NULL, ",");
                if (token == NULL)
                {
                    fileEmail = buffer; // second last column is email
                }
            }

            if (fileEmail && strcmp(fileEmail, email) == 0)
            {
                fclose(fp);
                return 2; // email already exists
            }
        }
        fclose(fp);
    }

    return 0; // safe
}

int validatePassword(const char *password)
{
    if (strlen(password) < 6)
        return 0;

    int hasUpper = 0, hasLower = 0, hasDigit = 0, hasSpecial = 0;
    for (int i = 0; password[i]; i++)
    {
        if (isupper(password[i]))
            hasUpper = 1;
        else if (islower(password[i]))
            hasLower = 1;
        else if (isdigit(password[i]))
            hasDigit = 1;
        else
            hasSpecial = 1;
    }

    return (hasUpper && hasLower && hasDigit && hasSpecial);
}

int validateEmail(const char *email)
{
    const char *at = strchr(email, '@');
    if (at == NULL || at == email)
        return 0;

    const char *dot = strrchr(email, '.');
    if (dot == NULL || dot < at)
        return 0;

    if (email[strlen(email) - 1] == '.' || email[strlen(email) - 1] == '@')
        return 0;

    return 1;
}

// Update users.txt password when profile changes
void updateUserCredentials(const char *username, const char *newPassword, const char *userType) {
    FILE *fp = fopen("data/users.txt", "r");
    FILE *temp = fopen("data/users_temp.txt", "w");
    if (!fp || !temp) {
        printf("❌ Error updating users.txt\n");
        return;
    }

    char fileUser[50], filePass[50], fileType[20];
    while (fscanf(fp, "%49[^,],%49[^,],%19[^\n]\n", fileUser, filePass, fileType) == 3) {
        if (strcmp(fileUser, username) == 0 && strcmp(fileType, userType) == 0) {
            fprintf(temp, "%s,%s,%s\n", fileUser, newPassword, fileType);
        } else {
            fprintf(temp, "%s,%s,%s\n", fileUser, filePass, fileType);
        }
    }

    fclose(fp);
    fclose(temp);

    remove("data/users.txt");
    rename("data/users_temp.txt", "data/users.txt");
}
