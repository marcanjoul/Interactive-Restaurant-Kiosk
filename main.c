#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "menu.h"
#include "order.h"

// Function prototypes
char yesOrNo(const char *prompt);
void processPayment(double subtotal);

//Main function
int main()
{
    srand(time(NULL)); // Seed the random number generator once at the start of the program
    readCSV();         // Read from CSV file containing the menu

    // Find matching items from CSV to menu categories
    appetizerCount = searchCSV(menu, totalItems, appetizers, "APPETIZERS");
    beverageCount = searchCSV(menu, totalItems, beverages, "BEVERAGES");
    mainDishCount = searchCSV(menu, totalItems, mainDishes, "MAIN DISHES");
    dessertCount = searchCSV(menu, totalItems, dessert, "DESSERT");

    // Order struct
    struct OrderNode *order = NULL;

    // Delay order printing for some real-life effect
    srand(time(NULL));

    // Introduction
    printf("\n");
    printf("╔════════════════════════════════════════════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                                                                                                                ║\n");
    printf("║                                  Welcome to the Self-Ordering Restaurant Kiosk!                                ║\n");
    printf("║                                                                                                                ║\n");
    printf("╠════════════════════════════════════════════════════════════════════════════════════════════════════════════════╣\n");
    printf("║                           Order your meal, customize your experience, and pay with ease                        ║\n");
    printf("║                                 — all through our interactive digital kiosk system.                            ║\n");
    printf("╠════════════════════════════════════════════════════════════════════════════════════════════════════════════════╣\n");
    printf("║                                                                                                                ║\n");
    printf("║                                !You have 5 chances to attempt the required input!                              ║\n");
    printf("║                                                                                                                ║\n");
    printf("╚════════════════════════════════════════════════════════════════════════════════════════════════════════════════╝\n");
    printf("\nPress Enter to view the menu and begin your order...");
    getchar();
    printf("Loading menu...\n");

    printMenu(appetizers, appetizerCount, mainDishes, mainDishCount, dessert, dessertCount, beverages, beverageCount);

    printf("Browse the menu above. Press Enter when you're ready to begin placing your order...");
    getchar(); // Wait for actual Enter key press

    // Start the full order process
    int totalAdded = 0; // Tracks the total number of items added to the order
    printf("\n");
    totalAdded += takeOrder(appetizers, appetizerCount, "appetizer", "Appetizer Choice", &order); // Take appetizer order
    printf("\n");
    totalAdded += takeOrder(mainDishes, mainDishCount, "main dish", "Main Dish Choice", &order); // Take main dish order
    printf("\n");
    totalAdded += takeOrder(dessert, dessertCount, "dessert", "Dessert Choice", &order); // Take dessert order
    printf("\n");
    totalAdded += takeOrder(beverages, beverageCount, "beverage", "Beverage Choice", &order); // Take beverage order
    if (totalAdded == 0)
    {
        printf("\nYou did not order anything. Order has been cancelled.\n");
        return 0;
    }

    char confirm = yesOrNo("\nProceed with your order?");
    if (confirm == 'n' || confirm == 'N')
    {
        printf("Your order has been cancelled.\n");
        freeOrder(order);
        return 0;
    }

    // Display initial receipt
    sleep(1);                           // Simulated processing delay for realism
    double subtotal = viewOrder(order); // Shows current order and returns subtotal

    // Allow modifications
    modifyOrder(&order);

    // Recalculate subtotal if modifications were appplied
    subtotal = recalculateSubtotal(order);

    // Proceed to payment
    processPayment(subtotal);

    // Free memory used for the linked list
    freeOrder(order); 

    return 0;
}



// Function that prompts the user with a yes/no question and validates their input.
// Returns 'y' or 'n'. If the user fails 5 times, defaults to 'n'.
char yesOrNo(const char *prompt)
{
    char response;

    for (int i = 0; i < MAX_ATTEMPTS; i++)
    {
        // Display the prompt and get a single character
        printf("%s (y = yes, n = no): ", prompt);
        scanf(" %c", &response);  // leading space skips any leftover newline

        // Accept valid responses
        if (response == 'y' || response == 'Y' || response == 'n' || response == 'N')
        {
            return response;
        }

        // Invalid response — prompt again
        printf("Oops! That wasn't a valid option. Try again.\n\n");
    }

    // Too many failed attempts, fail-safe fallback
    printf("Invalid input after %d attempts. Assuming 'n'...\n", MAX_ATTEMPTS);
    return 'n';
}



// Function that processes the payment
void processPayment(double subtotal)
{
    double tax = subtotal * TAX_RATE; // Calculate tax based on subtotal
    double tip;                       // Tip amount
    double total;                     // Total amount to be paid
    int tipChoice;                    // User's choice for tip
    char paymentType;                 // Payment method (cash or card)
    double amountPaid;                // Amount paid by the user
    int orderNumber = generateOrderNumber(); // Generate a random order number

    // Prompt user for tip
    printf("\n\nWould you like to leave a tip?\n");
    printf("1. 10%%\n2. 15%%\n3. 20%%\n4. Custom Amount\n5. No Tip\n\n");

    // Allow up to MAX_ATTEMPTS for valid tip input
    for (int i = 0; i < MAX_ATTEMPTS; i++)
    {
        printf("Choice: ");
        if (scanf("%d", &tipChoice) != 1)
        {
            printf("Invalid input. Please enter a number between 1 and 5.\n\n");
            while (getchar() != '\n')
                ; // Clear buffer
            continue;
        }

        // Determine tip based on user choice
        switch (tipChoice)
        {
        case 1:
            tip = subtotal * 0.10; // 10% tip
            break;
        case 2:
            tip = subtotal * 0.15; // 15% tip
            break;
        case 3:
            tip = subtotal * 0.20; // 20% tip
            break;
        case 4:
            // Custom tip amount
            printf("Enter custom tip amount: $");
            if (scanf("%lf", &tip) == 1 && tip >= 0.0)
                break;
            else
            {
                printf("Invalid amount. Try again.\n");
                while (getchar() != '\n')
                    ;
                continue;
            }
        case 5:
            tip = 0.0; // No tip
            break;
        default:
            printf("Invalid choice. Please select a number between 1 and 5.\n\n");
            continue;
        }
        break; // Valid input
    }
    printf("Invalid input after %d attempts. Assuming 'No Tip'...\n", MAX_ATTEMPTS);

    // Calculate total amount
    total = subtotal + tax + tip;

    // Display payment summary
    printf("\nSubtotal: $%.2f\n", subtotal);
    printf("Tax (8.5%%): $%.2f\n", tax);
    printf("Tip: $%.2f\n", tip);
    printf("Total: $%.2f\n", total);

    // Prompt user for payment method
    for (int i = 0; i < MAX_ATTEMPTS; i++)
    {
        printf("\nWould you like to pay with cash or card? (c = cash, d = debit/credit): ");
        scanf(" %c", &paymentType);

        if (paymentType == 'c' || paymentType == 'C')
        {
            // Handle cash payment
            for (int j = 0; j < MAX_ATTEMPTS; j++)
            {
                printf("Enter cash amount: $");
                if (scanf("%lf", &amountPaid) != 1)
                {
                    printf("Invalid input. Please enter a digit\n\n");
                    while (getchar() != '\n')
                        ;
                    continue;
                }

                if (amountPaid >= total)
                {
                    // Calculate and display change
                    printf("Your change is $%.2f. Thank you, hope to see you soon! :)\n", amountPaid - total);
                    return;
                }
                else
                {
                    // Insufficient cash provided
                    printf("That amount is not enough. Please provide at least $%.2f.\n\n", total);
                }
            }
            break;
        }
        else if (paymentType == 'd' || paymentType == 'D')
        {
            // Handle card payment
            printf("Processing card...\n\n");
            sleep(1); // Simulate processing delay
            printf("\n======================================\n");
            printf("   Thank you for dining with us!\n");
            printf("           Your order is \n\t\t#%d\n", orderNumber);
            printf("======================================\n");
            return;
        }
        else
        {
            // Invalid payment method
            printf("Invalid option. Try again.\n");
        }
    }

    // If no valid payment method is provided
    printf("\nNo payment method. Transaction canceled...\n");
}

