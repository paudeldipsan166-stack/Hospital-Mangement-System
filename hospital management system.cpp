#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

// -------- STRUCTURE --------
struct Patient {
    int id;
    char name[50];
    int age;
    char disease[50];
};

// -------- HELPER: CLEAR BUFFER --------
void clearBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// -------- HIDE PASSWORD --------
void getPassword(char pass[]) {
    int i = 0;
    char ch;
    while (1) {
        ch = getch();
        if (ch == 13 || ch == 10) break; // Enter key
        if (ch == 8) { // Backspace
            if (i > 0) {
                i--;
                printf("\b \b");
            }
        } else if (i < 19) {
            pass[i++] = ch;
            printf("*");
        }
    }
    pass[i] = '\0';
    printf("\n");
}

// -------- REGISTER --------
void registerUser() {
    FILE *fp = fopen("users.dat", "a");
    if (!fp) { printf("File Error!\n"); return; }
    
    char user[30], pass[20];
    printf("\n===== REGISTER =====\n");
    printf("Full Name / Username: ");
    scanf(" %[^\n]s", user); 
    printf("Password: ");
    getPassword(pass);

    // Clean up hidden newline characters
    user[strcspn(user, "\r\n")] = 0;
    pass[strcspn(pass, "\r\n")] = 0;

    fprintf(fp, "%s|%s\n", user, pass);
    fclose(fp);
    printf("\n? Registration Successful! Press any key to go to Login...");
    getch();
}

// -------- LOGIN --------
int loginUser() {
    FILE *fp = fopen("users.dat", "r");
    char inputUser[30], inputPass[20], storedUser[30], storedPass[20];
    int found = 0;

    if (fp == NULL) {
        printf("\nNo users found! Please register first.\n");
        getch();
        return 0;
    }

    printf("\n===== LOGIN =====\n");
    printf("Username: ");
    scanf(" %[^\n]s", inputUser);
    printf("Password: ");
    getPassword(inputPass);

    // Clean input
    inputUser[strcspn(inputUser, "\r\n")] = 0;
    inputPass[strcspn(inputPass, "\r\n")] = 0;

    // Scan using the | separator to handle names with spaces correctly
    while (fscanf(fp, " %[^|]|%s\n", storedUser, storedPass) != EOF) {
        storedUser[strcspn(storedUser, "\r\n")] = 0;
        storedPass[strcspn(storedPass, "\r\n")] = 0;

        if (strcmp(inputUser, storedUser) == 0 && strcmp(inputPass, storedPass) == 0) {
            found = 1;
            break;
        }
    }
    fclose(fp);

    if (found) {
        printf("\n? Login Successful! Loading system...\n");
        return 1;
    } else {
        printf("\n? Invalid Credentials! Press any key to try again.\n");
        getch();
        return 0;
    }
}

// -------- ADD PATIENT --------
void addPatient() {
    system("cls");
    FILE *fp = fopen("patients.dat", "a");
    struct Patient p;

    printf("\n--- ADD PATIENT ---\n");
    printf("ID: "); scanf("%d", &p.id);
    printf("Name: "); scanf(" %[^\n]s", p.name);
    printf("Age: "); scanf("%d", &p.age);
    printf("Disease: "); scanf(" %[^\n]s", p.disease);

    fprintf(fp, "%d|%s|%d|%s\n", p.id, p.name, p.age, p.disease);
    fclose(fp);
    printf("\n? Patient Added! Press any key...");
    getch();
}

// -------- VIEW --------
void viewPatients() {
    system("cls");
    FILE *fp = fopen("patients.dat", "r");
    struct Patient p;

    if (fp == NULL) {
        printf("\n?? No records found!\n");
        getch();
        return;
    }

    printf("\n%-5s %-20s %-5s %-20s", "ID", "Name", "Age", "Disease");
    printf("\n------------------------------------------------------------\n");

    while (fscanf(fp, "%d|%[^|]|%d|%[^\n]\n", &p.id, p.name, &p.age, p.disease) != EOF) {
        printf("%-5d %-20s %-5d %-20s\n", p.id, p.name, p.age, p.disease);
    }
    fclose(fp);
    printf("\nPress any key to return to menu...");
    getch();
}

// -------- SEARCH --------
void searchPatient() {
    system("cls");
    FILE *fp = fopen("patients.dat", "r");
    struct Patient p;
    int id, found = 0;

    if (!fp) { printf("No records found!\n"); getch(); return; }

    printf("Enter ID to search: ");
    scanf("%d", &id);

    while (fscanf(fp, "%d|%[^|]|%d|%[^\n]\n", &p.id, p.name, &p.age, p.disease) != EOF) {
        if (p.id == id) {
            printf("\n?? Record Found:\nID: %d\nName: %s\nAge: %d\nDisease: %s\n", p.id, p.name, p.age, p.disease);
            found = 1;
            break;
        }
    }
    fclose(fp);
    if (!found) printf("? ID %d not found!\n", id);
    printf("\nPress any key...");
    getch();
}

// -------- UPDATE --------
void updatePatient() {
    system("cls");
    FILE *fp = fopen("patients.dat", "r");
    FILE *temp = fopen("temp.dat", "w");
    struct Patient p;
    int id, found = 0;

    if (!fp) { printf("No records found!\n"); getch(); return; }

    printf("Enter ID to update: ");
    scanf("%d", &id);

    while (fscanf(fp, "%d|%[^|]|%d|%[^\n]\n", &p.id, p.name, &p.age, p.disease) != EOF) {
        if (p.id == id) {
            printf("New Name: "); scanf(" %[^\n]s", p.name);
            printf("New Age: "); scanf("%d", &p.age);
            printf("New Disease: "); scanf(" %[^\n]s", p.disease);
            found = 1;
        }
        fprintf(temp, "%d|%s|%d|%s\n", p.id, p.name, p.age, p.disease);
    }

    fclose(fp); fclose(temp);
    remove("patients.dat");
    rename("temp.dat", "patients.dat");

    if (found) printf("? Record Updated!\n");
    else printf("? Not found!\n");
    getch();
}

// -------- DELETE --------
void deletePatient() {
    system("cls");
    FILE *fp = fopen("patients.dat", "r");
    FILE *temp = fopen("temp.dat", "w");
    struct Patient p;
    int id, found = 0;

    if (!fp) { printf("No records found!\n"); getch(); return; }

    printf("Enter ID to delete: ");
    scanf("%d", &id);

    while (fscanf(fp, "%d|%[^|]|%d|%[^\n]\n", &p.id, p.name, &p.age, p.disease) != EOF) {
        if (p.id != id)
            fprintf(temp, "%d|%s|%d|%s\n", p.id, p.name, p.age, p.disease);
        else
            found = 1;
    }

    fclose(fp); fclose(temp);
    remove("patients.dat");
    rename("temp.dat", "patients.dat");

    if (found) printf("? Deleted successfully!\n");
    else printf("? ID not found!\n");
    getch();
}

// -------- MAIN --------
int main() {
    int choice;

    while (1) {
        system("cls");
        printf("\n=== HOSPITAL SYSTEM ACCESS ===\n");
        printf("1. Register\n2. Login\n3. Exit\n");
        printf("Choice: ");
        
        if (scanf(" %d", &choice) != 1) { 
            clearBuffer(); 
            continue; 
        }

        if (choice == 1) {
            registerUser();
        } 
        else if (choice == 2) {
            if (loginUser()) break;
        } 
        else exit(0);
    }

    while (1) {
        system("cls");
        printf("\n===== HOSPITAL MAIN MENU =====\n");
        printf("1. Add Patient\n2. View All\n3. Search Patient\n4. Update Patient\n5. Delete Patient\n6. Exit\n");
        printf("Choice: ");

        if (scanf(" %d", &choice) != 1) { 
            clearBuffer(); 
            continue; 
        }
          switch (choice) {
            case 1: addPatient(); break;
            case 2: viewPatients(); break;
            case 3: searchPatient(); break;
            case 4: updatePatient(); break;
            case 5: deletePatient(); break;
            case 6: exit(0);
            default: printf("Invalid choice! Press any key..."); getch();
        }
    }
    return 0;
}

