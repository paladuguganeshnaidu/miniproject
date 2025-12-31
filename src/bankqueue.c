#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct Customer {
    int id;                     
    char name[50];          
    char time[30];           
    char service[30];          
    float balance;              
    struct Customer *next;     
};

struct BankQueue {
    struct Customer *front;
    struct Customer *rear;
    int nextID;               
};

void getCurrentTime(char *buffer, int size) {
    time_t t;
    time(&t);
    struct tm *local = localtime(&t);
    strftime(buffer, size, "%Y-%m-%d %H:%M:%S", local);
}

void initQueue(struct BankQueue *q) {
    q->front = NULL;
    q->rear = NULL;
    q->nextID = 101;
}


int isEmpty(struct BankQueue *q) {
    return (q->front == NULL);
}

void chooseService(char service[], float *balance) {
    int choice;
    printf("\nSelect Service:\n");
    printf("1. Deposit\n");
    printf("2. Withdraw\n");
    printf("3. Balance Inquiry\n");
    printf("4. Loan Inquiry\n");
    printf("Enter choice: ");
    scanf("%d", &choice);
    getchar();
    switch (choice) {
        case 1: strcpy(service, "Deposit"); break;
        case 2: strcpy(service, "Withdraw"); break;
        case 3: strcpy(service, "Balance Inquiry"); break;
        case 4: strcpy(service, "Loan Inquiry"); break;
        default: strcpy(service, "Unknown"); break;
    }
    if (*balance == -1) *balance = 0.0;
}

void addCustomer(struct BankQueue *q) {
    char name[50], service[30];
    float balance = -1.0;
    printf("\nEnter Customer Name: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = 0;
    chooseService(service, &balance);
    struct Customer *newCustomer = (struct Customer *)malloc(sizeof(struct Customer));
    if (!newCustomer) {
        printf("Memory allocation failed!\n");
        return;
    }
    newCustomer->id = q->nextID++;
    strcpy(newCustomer->name, name);
    strcpy(newCustomer->service, service);
    newCustomer->balance = balance;
    getCurrentTime(newCustomer->time, sizeof(newCustomer->time));
    newCustomer->next = NULL;
    if (isEmpty(q)) {
        q->front = q->rear = newCustomer;
    } else {
        q->rear->next = newCustomer;
        q->rear = newCustomer;
    }
    printf("\n✅ Customer added successfully!\n");
    printf("ID: %d | Name: %s | Service: %s | Arrival: %s\n", newCustomer->id, newCustomer->name, newCustomer->service, newCustomer->time);
}

void serveCustomer(struct BankQueue *q) {
    if (isEmpty(q)) {
        printf("\n⚠️  No customers to serve.\n");
        return;
    }
    struct Customer *temp = q->front;
    printf("\n👨‍💼 Serving Customer:\n");
    printf("ID: %d\n", temp->id);
    printf("Name: %s\n", temp->name);
    printf("Service: %s\n", temp->service);
    printf("Arrival: %s\n", temp->time);
    if (strcmp(temp->service, "Deposit") == 0) {
        float amount;
        printf("Enter deposit amount: ");
        scanf("%f", &amount);
        getchar();
        temp->balance += amount;
        printf("Deposit successful! Funds Added: %.2f\n", temp->balance);
    } else if (strcmp(temp->service, "Withdraw") == 0) {
        float amount;
        printf("Enter withdrawal amount: ");
        scanf("%f", &amount);
        getchar();
        if (amount > temp->balance) {
            printf("Insufficient balance! Current Balance: %.2f\n", temp->balance);
        } else {
            temp->balance -= amount;
            printf("Withdrawal successful! Current Balance: %.2f\n", temp->balance);
        }
    } else if (strcmp(temp->service, "Balance Inquiry") == 0) {
        printf("Customer Balance: %.2f\n", temp->balance);
    } else if (strcmp(temp->service, "Loan Inquiry") == 0) {
        printf("Loan services information: Please contact bank officer.\n");
    }
    q->front = q->front->next;
    if (q->front == NULL) q->rear = NULL;
    free(temp);
}

void displayQueue(struct BankQueue *q) {
    if (isEmpty(q)) {
        printf("\n🕒 No customers waiting.\n");
        return;
    }
    printf("\n📋 Current Queue:\n");
    printf("-------------------------------------------------------------------------\n");
    printf("| %-5s | %-20s | %-15s | %-20s |\n", "ID", "Name", "Service", "Arrival Time");
    printf("-------------------------------------------------------------------------\n");
    struct Customer *temp = q->front;
    while (temp != NULL) {
        printf("| %-5d | %-20s | %-15s | %-20s |\n", temp->id, temp->name, temp->service, temp->time);
        temp = temp->next;
    }
    printf("-------------------------------------------------------------------------\n");
}

int main() {
    struct BankQueue bank;
    initQueue(&bank);
    int choice;
    while (1) {
        printf("\n===== 🏦 PROFESSIONAL BANKING QUEUE SYSTEM =====\n");
        printf("1. Add Customer\n");
        printf("2. Serve Customer\n");
        printf("3. Display Waiting Customers\n");
        printf("4. Exit\n");
        printf("----------------------------------------------\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();
        switch (choice) {
            case 1: addCustomer(&bank); break;
            case 2: serveCustomer(&bank); break;
            case 3: displayQueue(&bank); break;
            case 4: printf("Exiting system. Have a nice day!\n"); exit(0);
            default: printf("❌ Invalid choice. Try again.\n");
        }
    }
    return 0;
}
