#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define constants for account types
typedef enum {
    SAVINGS,
    CURRENT,
    FIXED_DEPOSIT
} AccountType;

// Structure for holding customer information
struct Customer {
    char customerName[50];
    char password[20];
};

// Structure for holding transaction information
struct Transaction {
    int transactionID;
    char transactionType[20];
    float amount;
    struct Transaction *next;
};

// Structure for holding account information
struct Account {
    int accountNumber;
    struct Customer customerInfo;
    float balance;
    AccountType type;
    struct Transaction *transactionHistory;
    struct Account *left;
    struct Account *right;
};

// Global variable for the root of the account tree
struct Account *root = NULL;

// Function to collect customer information
struct Customer collectCustomerInfo() {
    struct Customer newCustomer;

    printf("Enter customer name: ");
    scanf("%s", newCustomer.customerName);

    // Add password input
    printf("Enter password for the account: ");
    scanf("%s", newCustomer.password);

    return newCustomer;
}

// Function to create a new account
struct Account *createAccount(struct Account *node) {
    struct Account *newAccount = (struct Account *)malloc(sizeof(struct Account));
    if (newAccount == NULL) {
        printf("Memory allocation failed.\n");
        return node;
    }

    struct Customer newCustomer = collectCustomerInfo();

    newAccount->accountNumber = rand() % 1000 + 1;
    newAccount->customerInfo = newCustomer;
    newAccount->balance = 0.0;
    newAccount->type = SAVINGS; // Default account type
    newAccount->transactionHistory = NULL;
    newAccount->left = NULL;
    newAccount->right = NULL;

    if (node == NULL) {
        return newAccount;
    } else {
        if (strcmp(newAccount->customerInfo.customerName, node->customerInfo.customerName) < 0) {
            node->left = createAccount(node->left);
        } else {
            node->right = createAccount(node->right);
        }
        return node;
    }
}

// Function to deposit money into an account
void deposit(struct Account *account, float amount) {
    if (account == NULL) {
        printf("Account not found.\n");
        return;
    }

    account->balance += amount;

    struct Transaction *newTransaction = (struct Transaction *)malloc(sizeof(struct Transaction));
    if (newTransaction == NULL) {
        printf("Memory allocation failed for transaction.\n");
        return;
    }

    newTransaction->transactionID = rand() % 1000 + 1;
    strcpy(newTransaction->transactionType, "Deposit");
    newTransaction->amount = amount;
    newTransaction->next = account->transactionHistory;
    account->transactionHistory = newTransaction;

    printf("Deposit successful. Updated balance: %.2f\n", account->balance);
}

// Function to withdraw money from an account
void withdraw(struct Account *account, float amount) {
    if (account == NULL) {
        printf("Account not found.\n");
        return;
    }

    if (amount > account->balance) {
        printf("Insufficient funds.\n");
        return;
    }

    account->balance -= amount;

    struct Transaction *newTransaction = (struct Transaction *)malloc(sizeof(struct Transaction));
    if (newTransaction == NULL) {
        printf("Memory allocation failed for transaction.\n");
        return;
    }

    newTransaction->transactionID = rand() % 1000 + 1;
    strcpy(newTransaction->transactionType, "Withdrawal");
    newTransaction->amount = amount;
    newTransaction->next = account->transactionHistory;
    account->transactionHistory = newTransaction;

    printf("Withdrawal successful. Updated balance: %.2f\n", account->balance);
}

// Function to display the transaction history of an account
void displayTransactionHistory(struct Account *account) {
    if (account == NULL) {
        printf("Account not found.\n");
        return;
    }

    struct Transaction *currentTransaction = account->transactionHistory;
    printf("Transaction History for Account Number: %d\n", account->accountNumber);
    printf("----------------------------------------\n");

    printf("Deposits:\n");
    while (currentTransaction != NULL) {
        if (strcmp(currentTransaction->transactionType, "Deposit") == 0) {
            printf("Transaction ID: %d, Type: %s, Amount: %.2f\n", currentTransaction->transactionID,
                   currentTransaction->transactionType, currentTransaction->amount);
        }
        currentTransaction = currentTransaction->next;
    }

    currentTransaction = account->transactionHistory;

    printf("\nWithdrawals:\n");
    while (currentTransaction != NULL) {
        if (strcmp(currentTransaction->transactionType, "Withdrawal") == 0) {
            printf("Transaction ID: %d, Type: %s, Amount: %.2f\n", currentTransaction->transactionID,
                   currentTransaction->transactionType, currentTransaction->amount);
        }
        currentTransaction = currentTransaction->next;
    }

    printf("\nBalance: %.2f\n", account->balance);
}

// Function to calculate interest for an account
void calculateInterest(struct Account *account) {
    if (account == NULL) {
        printf("Account not found.\n");
        return;
    }

    float principalAmount, interestRate;
    int numberOfYears;

    printf("Enter principal amount: ");
    scanf("%f", &principalAmount);

    printf("Enter rate of interest: ");
    scanf("%f", &interestRate);

    printf("Enter number of years: ");
    scanf("%d", &numberOfYears);

    float totalAccruedAmount = principalAmount * (1 + (interestRate / 100) * numberOfYears);
    float interestEarned = totalAccruedAmount - principalAmount;

    printf("Interest: %.2f\n", interestEarned);
}

// Function to free memory allocated for accounts and transactions
void freeAccount(struct Account *account) {
    if (account == NULL) {
        return;
    }

    freeAccount(account->left);
    freeAccount(account->right);

    struct Transaction *currentTransaction = account->transactionHistory;
    while (currentTransaction != NULL) {
        struct Transaction *temp = currentTransaction;
        currentTransaction = currentTransaction->next;
        free(temp);
    }

    free(account);
}

// Function to handle an invalid choice in the main menu
void handleInvalidChoice() {
    printf("Invalid choice. Please try again.\n");
}

int main() {
    printf("Bank Management System\n");

    int choice;
    do {
        printf("\n1. Create Account\n");
        printf("2. Deposit\n");
        printf("3. Withdraw\n");
        printf("4. Display Transaction History\n");
        printf("5. Calculate Interest\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                // Create Account
                root = createAccount(root);
                break;
            case 2: {
                // Deposit
                float amount;
                printf("Enter amount to deposit: ");
                scanf("%f", &amount);
                deposit(root, amount);
                break;
            }
            case 3: {
                // Withdraw
                float amount;
                printf("Enter amount to withdraw: ");
                scanf("%f", &amount);
                withdraw(root, amount);
                break;
            }
            case 4:
                // Display transaction history
                displayTransactionHistory(root);
                break;
            case 5:
                // Calculate interest
                calculateInterest(root);
                break;
            default:
                // Invalid choice
                handleInvalidChoice();
                break;
        }
    } while (choice != 6);

    // Free allocated memory before exiting
    freeAccount(root);

    return 0;
}
