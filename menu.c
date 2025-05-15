#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h> // Required for sleep()

#include "menu.h"

// Function that reads from a CSV file containing the menu
void readCSV()
{
    FILE *CSV = fopen("menu.csv", "r");

    if (CSV == NULL)
    {
        printf("File can't be opened.\n");
        return;
    }

    while (fscanf(CSV, " %[^,],%[^,],\"%[^\"]\",%lf\n",
                  menu[totalItems].category,
                  menu[totalItems].name,
                  menu[totalItems].description,
                  &menu[totalItems].price) == 4)
    {
        totalItems++;
        if (totalItems >= MAX_MENU_ITEMS)
            break;
    }

    fclose(CSV);
}

// Function that searches the CSV and places each category in a struct for easier use
int searchCSV(const struct MenuItem menu[], int n, struct MenuItem category[], char *type)
{
    int i;
    int matchCount = 0;
    for (i = 0; i < n; i++)
    {
        if (strcmp(menu[i].category, type) == 0)
        {
            category[matchCount] = menu[i];
            matchCount++;
        }
    }
    return matchCount;
}

/**
 * Prints the menu items categorized into appetizers, main dishes, desserts, and beverages.
 * 
 * "appetizers" Array of appetizer menu items.
 * "appetizerCount" Number of appetizer items in the array.
 * "mainDishes" Array of main dish menu items.
 * "mainDishCount" Number of main dish items in the array.
 * "dessert" Array of dessert menu items.
 * "dessertCount" Number of dessert items in the array.
 * "beverages" Array of beverage menu items.
 * "beverageCount" Number of beverage items in the array.
 */
void printMenu(struct MenuItem appetizers[], int appetizerCount, struct MenuItem mainDishes[], int mainDishCount, struct MenuItem dessert[], int dessertCount, struct MenuItem beverages[], int beverageCount)
{
     // Define a list of menu categories and their item arrays
    struct {
        struct MenuItem *items;     // Pointer to array of items
        int count;                  // Number of items in category
        const char *categoryName;   // Display name of the category
    } categories[] = {
        {appetizers, appetizerCount, "APPETIZERS"},
        {mainDishes, mainDishCount, "MAIN DISHES"},
        {dessert, dessertCount, "DESSERTS"},
        {beverages, beverageCount, "BEVERAGES"}
    };

    // Loop through each category
    for (int c = 0; c < 4; c++)
    {
        sleep(1);  // Simulate loading delay for realism
        printf("  ====================================== %s =====================================\n", categories[c].categoryName);

        // Print each item in the current category
        for (int i = 0; i < categories[c].count; i++)
        {
            printf(" %2d. %-85s $%.2f\n", i + 1, categories[c].items[i].name, categories[c].items[i].price);

            // If description isn't "EMPTY", print it
            if (strcmp(categories[c].items[i].description, "EMPTY") != 0)
            {
                printf("     → %s\n\n", categories[c].items[i].description);
            }
        }

        printf("\n\n");
    }
}