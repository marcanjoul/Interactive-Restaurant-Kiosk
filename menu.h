#ifndef MENU_H
#define MENU_H

#define NAME_LEN 50
#define DESC_LEN 1000
#define MAX_MENU_ITEMS 100

//Global Variables
int totalItems;

// Structs
struct MenuItem
{
    char category[20];             
    char name[NAME_LEN + 1];        
    char description[DESC_LEN + 1]; 
    double price;                   
};

// Global variables
struct MenuItem menu[MAX_MENU_ITEMS];
struct MenuItem beverages[MAX_MENU_ITEMS];
struct MenuItem mainDishes[MAX_MENU_ITEMS];
struct MenuItem appetizers[MAX_MENU_ITEMS];
struct MenuItem dessert[MAX_MENU_ITEMS];

// Function prototypes
void readCSV();
int searchCSV(const struct MenuItem menu[], int n, struct MenuItem category[], char *type);
void printMenu(struct MenuItem appetizers[], int appetizerCount, struct MenuItem mainDishes[], int mainDishCount, struct MenuItem dessert[], int dessertCount, struct MenuItem beverages[], int beverageCount);

#endif // MENU_H