# Stock-Market-Analysis-Tool
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Structure for a stock node
typedef struct Stock {
    char symbol[10];
    float price;
    int volume;
    char sector[20];
    struct Stock* left;
    struct Stock* right;
} Stock;
// Function to create a new node
Stock* createNode(const char *symbol, float price, int volume, const char *sector) {
    Stock* newNode = (Stock*)malloc(sizeof(Stock));
    strcpy(newNode->symbol, symbol);
    newNode->price = price;
    newNode->volume = volume;
    strcpy(newNode->sector, sector);
    newNode->left = newNode->right = NULL;
    return newNode;
}

// Function to insert a stock by price
Stock* insertStock(Stock* root, const char *symbol, float price, int volume, const char *sector) {
    if (root == NULL) return createNode(symbol, price, volume, sector);
    if (price < root->price) root->left = insertStock(root->left, symbol, price, volume, sector);
    else if (price > root->price) root->right = insertStock(root->right, symbol, price, volume, sector);
    // If price is equal, you could decide to insert in right subtree or ignore duplicates
    else printf("Stock with price %.2f already exists. Ignored duplicate.\n", price);
    return root;
}

// Search entire BST for a stock by symbol
Stock* searchStock(Stock* root, const char *symbol) {
    if (root == NULL) return NULL;
    if (strcmp(root->symbol, symbol) == 0) return root;
    Stock* leftResult = searchStock(root->left, symbol);
    if (leftResult != NULL) return leftResult;
    return searchStock(root->right, symbol);
}

// Helper: delete a node by price in BST (standard BST deletion)
Stock* deleteByPrice(Stock* root, float price) {
    if (root == NULL) return NULL;

    if (price < root->price) {
        root->left = deleteByPrice(root->left, price);
    } else if (price > root->price) {
        root->right = deleteByPrice(root->right, price);
    } else {
        // Node found
        if (root->left == NULL) {
            Stock* temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            Stock* temp = root->left;
            free(root);
            return temp;
        } else {
            // Node with two children
            Stock* minRight = root->right;
            while (minRight->left != NULL)
                minRight = minRight->left;

            // Copy successor's data
            strcpy(root->symbol, minRight->symbol);
            root->price = minRight->price;
            root->volume = minRight->volume;
            strcpy(root->sector, minRight->sector);

            root->right = deleteByPrice(root->right, minRight->price);
        }
    }
    return root;
}

// Modified deleteStock: search node by symbol, then delete by price
Stock* deleteStock(Stock* root, const char *symbol) {
    if (root == NULL) return NULL;

    // Traverse left and right first
    root->left = deleteStock(root->left, symbol);
    root->right = deleteStock(root->right, symbol);

    // Check current node
    if (strcmp(root->symbol, symbol) == 0) {
        root = deleteByPrice(root, root->price);
    }

    return root;
}

// Display stocks in-order (sorted by price)
void displayStocks(Stock* root) {
    if (root != NULL) {
        displayStocks(root->left);
        printf("%s: ₹%.2f\n", root->symbol, root->price);
        displayStocks(root->right);
    }
}

// Free all nodes to avoid memory leaks
void freeStocks(Stock* root) {
    if (root != NULL) {
        freeStocks(root->left);
        freeStocks(root->right);
        free(root);
    }
}

// Main Function
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
I have created stock market analysis tool.Using data structure In C programming language.
