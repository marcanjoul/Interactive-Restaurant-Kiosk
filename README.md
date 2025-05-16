# C-Line-interactive-self-ordering-restaurant-kiosk
# 🍽️ Interactive Restaurant Kiosk

A C-based self-ordering restaurant simulation that mimics a digital kiosk system for customers. This interactive console program allows users to view a structured menu, place orders, update or remove items, and proceed through a realistic payment flow — all within a clean terminal interface.

---

## Features

- 📋 **Full Menu Navigation**: Organized by Appetizers, Main Dishes, Beverages, and Desserts.
- 🔁 **Linked List Order System**: Tracks dynamic orders using singly linked lists.
- 🛠️ **Modify Your Order**:
  - Add new items
  - Update quantities
  - Remove items
- 💳 **Simulated Payment**:
  - Cash or Card
  - Tip selection with custom entry
  - Change calculation
- 🧾 **Itemized Receipt**:
  - Subtotal, tax, tip, and total
  - Auto-cancel if order is empty
- 🔄 **Input Validation**:
  - 5 retry attempts for invalid inputs
  - Graceful fallbacks for skipping or canceling
- 🧠 **Clean Design**:
  - Modular structure (multiple `.c` and `.h` files)
  - Fully commented and maintainable
  - Realistic user prompts

---

## 📁 Project Structure
├── main.c # Main flow: user interface, flow control, payment

├── menu.c # Menu handling: reading CSV, displaying items

├── menu.h # Header for menu functions and constants

├── order.c # Linked list order logic: add, remove, update, view

├── order.h # Header for order functions and struct definitions

├── menu.csv # Menu data file (included in repo)

└── README.md # You're here!

## 🔧 How to Compile & Run

1. **Make sure you have GCC installed**:
   ```bash
   gcc --version
2. **Compile the project**:
   gcc main.c menu.c order.c -o kiosk
3. **Run the program**:
  ./kiosk
Make sure menu.csv is in the same directory!

## 🎯 **Learning Outcomes**

This project demonstrates:
  Proficiency in C programming
  Usage of structures, pointers, and dynamic memory
  Modularization of code into header/source files
  Understanding of linked lists, input validation, and formatted output
  Real-world application design through UI flow and payment logic
  
##🚧 **Limitations & Future Features**

This is a simulation — it doesn't:
  Persist order data
  Support multi-user or networking
  Handle real payment APIs
  Potential Improvements:
  Save receipts to a file
  Add order history or timestamps
  GUI version (CLI → mobile app someday!)



##⭐ **Why This Project?**

  Although not solving a real-world problem directly, this project shines as a portfolio-quality demonstration of low-level logic, modular architecture, and interactive user design — all built in C
  
##👨‍💻 **Author**

  Mark Anjoul
  Computer Science Student · Self-driven coder · Project-based learner
- **Copyright**
  © 2025 Mark Anjoul. This project is for educational and portfolio purposes only.
  Not licensed for commercial use or redistribution.
