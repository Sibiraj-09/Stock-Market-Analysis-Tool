#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Stock {
    char symbol[10];
    float price;
    int volume;
    char sector[20];
    struct Stock* left;
    struct Stock* right;
} Stock;

Stock* createNode(const char *symbol, float price, int volume, const char *sector) {
    Stock* newNode = (Stock*)malloc(sizeof(Stock));
    strcpy(newNode->symbol, symbol);
    newNode->price = price;
    newNode->volume = volume;
    strcpy(newNode->sector, sector);
    newNode->left = newNode->right = NULL;
    return newNode;
}

Stock* insertStock(Stock* root, const char *symbol, float price, int volume, const char *sector) {
    if (root == NULL) return createNode(symbol, price, volume, sector);
    if (price < root->price) root->left = insertStock(root->left, symbol, price, volume, sector);
    else if (price > root->price) root->right = insertStock(root->right, symbol, price, volume, sector);
    else printf("Stock with price %.2f already exists. Ignored duplicate.\n", price);
    return root;
}

Stock* searchStock(Stock* root, const char *symbol) {
    if (root == NULL) return NULL;
    if (strcmp(root->symbol, symbol) == 0) return root;
    Stock* leftResult = searchStock(root->left, symbol);
    if (leftResult != NULL) return leftResult;
    return searchStock(root->right, symbol);
}

Stock* deleteByPrice(Stock* root, float price) {
    if (root == NULL) return NULL;
    if (price < root->price) root->left = deleteByPrice(root->left, price);
    else if (price > root->price) root->right = deleteByPrice(root->right, price);
    else {
        if (root->left == NULL) {
            Stock* temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            Stock* temp = root->left;
            free(root);
            return temp;
        } else {
            Stock* minRight = root->right;
            while (minRight->left != NULL)
                minRight = minRight->left;
            strcpy(root->symbol, minRight->symbol);
            root->price = minRight->price;
            root->volume = minRight->volume;
            strcpy(root->sector, minRight->sector);
            root->right = deleteByPrice(root->right, minRight->price);
        }
    }
    return root;
}

Stock* deleteStock(Stock* root, const char *symbol) {
    if (root == NULL) return NULL;
    root->left = deleteStock(root->left, symbol);
    root->right = deleteStock(root->right, symbol);
    if (strcmp(root->symbol, symbol) == 0) {
        root = deleteByPrice(root, root->price);
    }
    return root;
}

void displayStocks(Stock* root) {
    if (root != NULL) {
        displayStocks(root->left);
        printf("%s: ₹%.2f\n", root->symbol, root->price);
        displayStocks(root->right);
    }
}

void freeStocks(Stock* root) {
    if (root != NULL) {
        freeStocks(root->left);
        freeStocks(root->right);
        free(root);
    }
}

int main() {
    Stock* root = NULL;
    int choice;
    char symbol[10], sector[20];
    float price;
    int volume;

    do {
        printf("\nStock Market Tool\n");
        printf("1. Add Stock\n");
        printf("2. Search Stock\n");
        printf("3. Delete Stock\n");
        printf("4. Display Stocks\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter stock symbol: ");
                scanf("%s", symbol);
                printf("Enter stock price: ");
                scanf("%f", &price);
                printf("Enter stock volume: ");
                scanf("%d", &volume);
                printf("Enter stock sector: ");
                scanf("%s", sector);
                root = insertStock(root, symbol, price, volume, sector);
                break;

            case 2: {
                printf("Enter stock symbol to search: ");
                scanf("%s", symbol);
                Stock* found = searchStock(root, symbol);
                if (found)
                    printf("Stock Found: %s, Price: ₹%.2f, Volume: %d, Sector: %s\n",
                           found->symbol, found->price, found->volume, found->sector);
                else
                    printf("Stock not found.\n");
                break;
            }

            case 3:
                printf("Enter stock symbol to delete: ");
                scanf("%s", symbol);
                root = deleteStock(root, symbol);
                printf("Stock deleted if existed.\n");
                break;

            case 4:
                printf("Stocks (sorted by price):\n");
                displayStocks(root);
                break;

            case 5:
                printf("Exiting...\n");
                break;

            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 5);

    freeStocks(root);
    return 0;
}