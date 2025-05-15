#ifndef ORDER_H
#define ORDER_H

#include "menu.h"

// Macro definitions
#define TAX_RATE 0.085
#define MAX_ATTEMPTS 5

//struct
struct OrderNode
{
    int index;
    char name[NAME_LEN + 1];
    int quantity;
    double price;
    struct OrderNode *next;
};

//Variables
int beverageCount, mainDishCount, appetizerCount, dessertCount;

//Function Prototypes
int takeOrder(struct MenuItem menu[], int matchCount, const char *categoryName, const char *choiceLabel, struct OrderNode **order);
double viewOrder(struct OrderNode *head);
int removeItem(struct OrderNode **head, int number);
void freeOrder(struct OrderNode *head);
int updateItemQty(struct OrderNode **order);
struct OrderNode *addItem(struct OrderNode *head, const char *itemName, double price);
void modifyOrder(struct OrderNode **order);
double recalculateSubtotal(struct OrderNode *head);
int generateOrderNumber();

#endif // ORDER_H