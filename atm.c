#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_USERS 100
#define ENCRYPTION_KEY 5

typedef struct {
    char username[30];
    char pin[10];
    float balance;
} User;

void encryptDecrypt(char *str) {
    while (*str) {
        *str ^= ENCRYPTION_KEY;
        str++;
    }
}

int loadUsers(User users[]) {
    FILE *file = fopen("users.txt", "r");
    if (!file) return 0;

    int count = 0;
    while (fscanf(file, "%s %s %f", users[count].username, users[count].pin, &users[count].balance) == 3) {
        encryptDecrypt(users[count].pin); 
        count++;
    }
    fclose(file);
    return count;
}

void saveUsers(User users[], int count) {
    FILE *file = fopen("users.txt", "w");
    for (int i = 0; i < count; i++) {
        char tempPin[10];
        strcpy(tempPin, users[i].pin);
        encryptDecrypt(tempPin); 
        fprintf(file, "%s %s %.2f\n", users[i].username, tempPin, users[i].balance);
    }
    fclose(file);
}

int findUser(User users[], int count, char *username) {
    for (int i = 0; i < count; i++) {
        if (strcmp(users[i].username, username) == 0)
            return i;
    }
    return -1;
}

int login(User users[], int count) {
    char username[30], pin[10];
    printf("\n--- Login ---\n");
    printf("Enter Username: ");
    scanf("%s", username);
    printf("Enter PIN: ");
    scanf("%s", pin);

    for (int i = 0; i < count; i++) {
        if (strcmp(users[i].username, username) == 0 &&
            strcmp(users[i].pin, pin) == 0) {
            printf("Login successful!\n");
            return i;
        }
    }

    printf("Invalid credentials.\n");
    return -1;
}

void checkBalance(User user) {
    printf("\nYour Balance is: Rs %.2f\n", user.balance);
}

void deposit(User *user) {
    float amount;
    printf("\nEnter amount to deposit: Rs ");
    scanf("%f", &amount);
    if (amount > 0) {
        user->balance += amount;
        printf("Deposit successful! New Balance: Rs %.2f\n", user->balance);
    } else {
        printf("Invalid amount.\n");
    }
}

void withdraw(User *user) {
    float amount;
    printf("\nEnter amount to withdraw: Rs ");
    scanf("%f", &amount);
    if (amount > 0 && amount <= user->balance) {
        user->balance -= amount;
        printf("Withdrawal successful! New Balance: Rs %.2f\n", user->balance);
    } else {
        printf("Invalid or insufficient amount.\n");
    }
}

void transfer(User users[], int count, User *sender) {
    char receiverName[30];
    float amount;
    printf("\nEnter receiver's username: ");
    scanf("%s", receiverName);
    int receiverIndex = findUser(users, count, receiverName);

    if (receiverIndex == -1 || strcmp(receiverName, sender->username) == 0) {
        printf("Invalid receiver.\n");
        return;
    }

    printf("Enter amount to transfer: Rs ");
    scanf("%f", &amount);

    if (amount > 0 && sender->balance >= amount) {
        sender->balance -= amount;
        users[receiverIndex].balance += amount;
        printf("Transfer successful! Your New Balance: Rs %.2f\n", sender->balance);
    } else {
        printf("Invalid or insufficient amount.\n");
    }
}

int main() {
    User users[MAX_USERS];
    int userCount = loadUsers(users);

    int loginIndex;
    do {
        loginIndex = login(users, userCount);
    } while (loginIndex == -1);

    int choice;
    do {
        printf("\n--- Main Menu ---\n");
        printf("1. Check Balance\n");
        printf("2. Deposit\n");
        printf("3. Withdraw\n");
        printf("4. Transfer\n");
        printf("5. Exit\n");
        printf("Choose option: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                checkBalance(users[loginIndex]);
                break;
            case 2:
                deposit(&users[loginIndex]);
                break;
            case 3:
                withdraw(&users[loginIndex]);
                break;
            case 4:
                transfer(users, userCount, &users[loginIndex]);
                break;
            case 5:
                printf("Thank you for using Smart ATM!\n");
                break;
            default:
                printf("Invalid option.\n");
        }

        saveUsers(users, userCount); 
    } while (choice != 5);

    return 0;
}