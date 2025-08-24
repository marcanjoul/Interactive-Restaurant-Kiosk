#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h> // Required for sleep()

#include  "order.h"
#include "menu.h"

//Function that allows the user to input their food of choice from the menu
// Returns 1 if at least one item was added to the order, 0 otherwise
int takeOrder(struct MenuItem menu[], int matchCount, const char *categoryName, const char *choiceLabel, struct OrderNode **order)
{
    int itemNum;
    int choiceNumber = 1;         // Keeps track of item prompt number
    int invalidAttempts = 0;      // Counts invalid inputs
    int itemAdded = 0;            // Flags whether any item was added

    while (1)
    {
        if (invalidAttempts >= MAX_ATTEMPTS)
        {
            printf("\nToo many invalid attempts. Assuming 'skip'...\n");
            break;
        }

        printf("%s #%d (0 to skip): ", choiceLabel, choiceNumber);
        if (scanf("%d", &itemNum) != 1)
        {
            printf("Invalid input. Try again.\n\n");
            while (getchar() != '\n')
                ;
            invalidAttempts++;
            continue;
        }
        if (itemNum == 0)
                    break;  // User chose to skip

                // Check for valid menu index
                if (itemNum >= 1 && itemNum <= matchCount)
                {
                    // Add item to order list
                    *order = addItem(*order, menu[itemNum - 1].name, menu[itemNum - 1].price);
                    printf("Added %s to order!\n\n", menu[itemNum - 1].name);

                    itemAdded = 1;        // At least one item was added
                    invalidAttempts = 0;  // Reset attempt counter
                    choiceNumber++;       // Increment for the next prompt
                }
                else
                {
                    printf("Invalid selection. Try again.\n\n");
                    invalidAttempts++;
                }
        }

        return itemAdded;
}
//Function that prints an order review for the user
double viewOrder(struct OrderNode *head)
{
    int index = 1;
    double subtotal = 0.0;

    sleep(1);
    printf("\n============ Order Review ============\n");
    while (head != NULL)
    {
        head->index = index;
        double itemTotal = head->quantity * head->price;
        printf("%d. %-25s x%d  $%.2f\n\n", head->index, head->name, head->quantity, itemTotal);
        subtotal += itemTotal;
        index++;
        head = head->next;
    }
    double tax = subtotal * TAX_RATE;
    double total = subtotal + tax;

    printf("\nSubtotal:      $%.2f\n", subtotal);
    printf("Tax (8.5%%):    $%.2f\n", tax);
    printf("Total:         $%.2f\n", total);
    printf("======================================\n");

    return subtotal;
}

// Function that removes an item from the order
int removeItem(struct OrderNode **head, int number)
{
    struct OrderNode *cur = *head; // Pointer to traverse the list
    struct OrderNode *prev = NULL; // Pointer to keep track of the previous node

    for (int i = 0; i < MAX_ATTEMPTS; i++) // Allow up to MAX_ATTEMPTS for user input
    {
        while (cur != NULL) // Traverse the linked list
        {
            if (cur->index == number) // Check if the current node matches the item number
            {
                // Handle removal of the head node
                if (prev == NULL)
                {
                    *head = cur->next; // Update the head pointer
                }
                else
                {
                    prev->next = cur->next; // Skip the current node
                }

                printf("\"%s\" has been removed from your receipt.\n", cur->name);
                free(cur); // Free memory of the removed node

                // Renumber the remaining items
                cur = *head;
                int newNumber = 1;
                while (cur != NULL)
                {
                    cur->index = newNumber++; // Update indices
                    cur = cur->next;
                }

                return 1; // Successfully removed the item
            }

            // Move to the next node
            prev = cur;
            cur = cur->next;
        }

        // Prompt user again if no matching item was found
        printf("No item found with number %d. Please try again.\n\n", number);
        printf("Enter the number of the item you want to remove: ");
        if (scanf("%d", &number) != 1) // Validate user input
        {
            while (getchar() != '\n') // Clear the input buffer
                ;
            printf("Invalid input.\n");
        }

        // Reset pointers for another pass
        cur = *head;
        prev = NULL;
    }

    // Exceeded maximum attempts
    printf("Too many invalid attempts. Returning to menu...\n");
    return 0; // Failed to remove the item
    printf("Too many invalid attempts. Returning to menu...\n");
    return 0; // Failed to remove the item
}

//Function to free the removed item's node to avoid memory leaks
void freeOrder(struct OrderNode *head)
{
    struct OrderNode *temp;
    while (head != NULL)
    {
        temp = head;
        head = head->next;
        free(temp);
    }
}

//Function that updates an item's qty
int updateItemQty(struct OrderNode **order)
{
    int number, newQty;
    struct OrderNode *p;

    // Allow the user up to MAX_ATTEMPTS to select an item to update
    for (int i = 0; i < MAX_ATTEMPTS; i++)
    {
        printf("Which item would you like to update? Enter its number from your receipt: ");
        if (scanf("%d", &number) != 1) // Validate user input
        {
            printf("Invalid input. Please enter a digit.\n\n");
            while (getchar() != '\n') // Clear input buffer
                ;
            continue;
        }

        // Search for the item in the order by its index
        p = *order;
        while (p != NULL)
        {
            if (p->index == number) // Item found
            {
                // Allow the user up to MAX_ATTEMPTS to enter a valid quantity
                for (int j = 0; j < MAX_ATTEMPTS; j++)
                {
                    printf("Enter the new quantity for \"%s\": ", p->name);
                    if (scanf("%d", &newQty) != 1) // Validate user input
                    {
                        printf("Invalid input. Please enter a digit.\n\n");
                        while (getchar() != '\n') // Clear input buffer
                            ;
                        continue;
                    }

                    if (newQty < 0) // Quantity cannot be negative
                    {
                        printf("Quantity cannot be negative. Try again.\n\n");
                        continue;
                    }
                    else if (newQty == 0) // Quantity of 0 means removing the item
                    {
                        printf("Quantity for \"%s\" updated to 0. Removing from order...\n", p->name);
                        removeItem(order, number); // Remove the item from the order
                        return 1; // Successfully updated
                    }
                    else // Update the item's quantity
                    {
                        p->quantity = newQty;
                        printf("Quantity for \"%s\" updated to %d.\n", p->name, p->quantity);
                        return 1; // Successfully updated
                    }
                }

                // Too many invalid attempts for quantity
                printf("Invalid input after %d attempts. Returning to menu...\n", MAX_ATTEMPTS);
                return 0;
            }
            p = p->next; // Move to the next item in the order
        }

        // If no item matches the entered number
        printf("No item found with number %d. Try again.\n\n", number);
    }

    // Too many failed attempts to select an item
    printf("Too many failed attempts. Returning to menu...\n");
    return 0;
}

//Function that adds an item to order and returns a node to our linked list
struct OrderNode *addItem(struct OrderNode *head, const char *itemName, double price)
{
    struct OrderNode *current = head;

    // Check if item already exists in the order
    while (current != NULL)
    {
        if (strcmp(current->name, itemName) == 0)
        {
            current->quantity++;
            return head;
        }
        current = current->next;
    }

    // Item not found — create a new node
    struct OrderNode *newNode = malloc(sizeof(struct OrderNode));
    if (!newNode)
    {
        printf("Memory allocation failed.\n");
        exit(1);
    }

    strncpy(newNode->name, itemName, NAME_LEN);
    newNode->name[NAME_LEN] = '\0';
    newNode->quantity = 1;
    newNode->price = price;
    newNode->next = head;

    return newNode;
}


// Function that allows the user to modify their order by removing an item, updating an item's quantity, 
// adding an item, proceeding to payment, or exiting the program
void modifyOrder(struct OrderNode **order)
{
    int updateChoice, categoryChoice;

    while (1)
    {
        // Display modification options
        printf("\nWould you like to:\n");
        printf("1. Remove an item\n");
        printf("2. Update an item's quantity\n");
        printf("3. Add item to order\n");
        printf("4. Proceed to payment\n");
        printf("5. Exit the program\n\n");

        int validInput = 0;
        // Allow the user up to MAX_ATTEMPTS to enter a valid choice
        for (int i = 0; i < MAX_ATTEMPTS; i++)
        {
            printf("Choice: ");
            if (scanf("%d", &updateChoice) != 1)
            {
                printf("Invalid input. Please enter 1, 2, 3, 4, or 5.\n\n");
                while (getchar() != '\n')
                    ; // Flush input buffer
                continue;
            }

            if (updateChoice >= 1 && updateChoice <= 5)
            {
                validInput = 1; // Valid choice entered
                break;
            }

            printf("Invalid choice. Try again.\n\n");
        }

        // If no valid input after MAX_ATTEMPTS, return to the main menu
        if (!validInput)
        {
            printf("Invalid input after %d attempts. Returning to menu...\n", MAX_ATTEMPTS);
            continue;
        }

        int itemNumber;
        struct OrderNode *p;

        // Handle the user's choice
        switch (updateChoice)
        {
        case 1: // Remove an item
            printf("\nEnter the number of the item you want to remove: ");
            scanf("%d", &itemNumber);
            if (removeItem(order, itemNumber)) // Attempt to remove the item
            {
                printf("\nHere’s your updated order...\n");
                viewOrder(*order); // Display the updated order
            }
            break;

        case 2: // Update an item's quantity
            if (updateItemQty(order)) // Attempt to update the quantity
            {
                printf("\nHere’s your updated order...\n");
                viewOrder(*order); // Display the updated order
            }
            break;

        case 3: // Add an item to the order
            printf("From what category would you like to add from?\n");
            printf("1. Appetizers\n");
            printf("2. Main Dishes\n");
            printf("3. Dessert\n");
            printf("4. Beverages\n");

            validInput = 0;
            // Allow the user up to MAX_ATTEMPTS to select a valid category
            for (int i = 0; i < MAX_ATTEMPTS; i++)
            {
                printf("Choice: ");
                if (scanf("%d", &categoryChoice) != 1)
                {
                    printf("Invalid input. Please enter 1, 2, 3, or 4.\n\n");
                    while (getchar() != '\n')
                        ; // Flush input buffer
                    continue;
                }

                if (categoryChoice >= 1 && categoryChoice <= 4)
                {
                    validInput = 1; // Valid category selected
                    break;
                }

                printf("Invalid choice. Try again.\n\n");
            }

            // If no valid input after MAX_ATTEMPTS, return to the main menu
            if (!validInput)
            {
                printf("Invalid input after %d attempts. Returning to menu...\n", MAX_ATTEMPTS);
                continue;
            }

            // Handle adding an item based on the selected category
            switch (categoryChoice)
            {
            case 1:
                takeOrder(appetizers, appetizerCount, "appetizers", "Appetizer Choice", order);
                break;
            case 2:
                takeOrder(mainDishes, mainDishCount, "main dishes", "Main Dish Choice", order);
                break;
            case 3:
                takeOrder(dessert, dessertCount, "desserts", "Dessert Choice", order);
                break;
            case 4:
                takeOrder(beverages, beverageCount, "beverages", "Beverage Choice", order);
                break;
            }

            printf("\nHere’s your updated order...\n");
            viewOrder(*order); // Display the updated order
            break;

        case 4: // Proceed to payment
            return; 

        case 5: // Exit the program
            printf("Exiting the program...\n");
            exit(0);
        }
    }
}

//Function that recalculates subtotal if any modification occured to the order
double recalculateSubtotal(struct OrderNode *head)
{
    double subtotal = 0.0;
    while (head)
    {
        subtotal += head->price * head->quantity;
        head = head->next;
    }
    return subtotal;
}

//Function that generates a random order number that shows after payment
int generateOrderNumber()
{
    return rand() % 101; // 0 to 100 inclusive
}