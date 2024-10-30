import pandas as pd
import matplotlib.pyplot as plt
import datetime
import json


# Load data from JSON file
def load_data():
    try:
        with open("expenses.json", "r") as file:
            return json.load(file)
    except FileNotFoundError:
        return []


# Save data to JSON file
def save_data(data):
    with open("expenses.json", "w") as file:
        json.dump(data, file)


# Add new expense entry
def add_expense(data, date, category, description, amount):
    data.append({"Date": date, "Category": category, "Description": description, "Amount": amount})
    save_data(data)
    print("Expense added successfully!")


# Set monthly budget for a category
def set_budget(budgets):
    category = input("Enter category to set budget for (e.g., Food, Bills, Entertainment): ")
    amount = float(input(f"Enter budget amount for {category}: "))
    budgets[category] = amount
    save_budget(budgets)
    print(f"Budget of {amount} set for {category}")


# Load budgets from JSON file
def load_budget():
    try:
        with open("budgets.json", "r") as file:
            return json.load(file)
    except FileNotFoundError:
        return {}


# Save budgets to JSON file
def save_budget(budgets):
    with open("budgets.json", "w") as file:
        json.dump(budgets, file)


# Check if spending exceeds budget
def check_budget(data, budgets):
    df = pd.DataFrame(data)
    category_totals = df.groupby("Category")["Amount"].sum()

    for category, total_spent in category_totals.items():
        if category in budgets and total_spent > budgets[category]:
            print(f"Warning: Spending in {category} exceeds budget by {total_spent - budgets[category]:.2f}")


# Visualize expenses by month and by category
def plot_expenses(data):
    df = pd.DataFrame(data)
    df['Date'] = pd.to_datetime(df['Date'])

    # Monthly total spending
    monthly_totals = df.groupby(df['Date'].dt.to_period("M"))["Amount"].sum()
    monthly_totals.plot(kind="bar", title="Monthly Expenses", color="skyblue")
    plt.ylabel("Amount Spent")
    plt.show()

    # Spending by category
    category_totals = df.groupby("Category")["Amount"].sum()
    category_totals.plot(kind="pie", title="Spending by Category", autopct="%1.1f%%", startangle=140)
    plt.show()


# View all expenses
def view_expenses(data):
    if not data:
        print("No expenses recorded.")
    else:
        df = pd.DataFrame(data)
        print("\nAll Recorded Expenses:")
        print(df.to_string(index=False))


# Main interface
def main():
    data = load_data()
    budgets = load_budget()

    while True:
        print("\n--- Personal Expense Tracker ---")
        print("1. Add Expense\n2. Set Budget\n3. View All Expenses\n4. Show Analysis\n5. Check Budget\n6. Exit")
        choice = input("Enter choice: ")

        if choice == "1":
            date = input("Enter date (YYYY-MM-DD): ")
            category = input("Enter category (e.g., Food, Bills, Entertainment): ")
            description = input("Enter description (e.g., Grocery shopping): ")
            amount = float(input("Enter amount: "))
            add_expense(data, date, category, description, amount)

        elif choice == "2":
            set_budget(budgets)

        elif choice == "3":
            view_expenses(data)

        elif choice == "4":
            plot_expenses(data)

        elif choice == "5":
            check_budget(data, budgets)

        elif choice == "6":
            print("Exiting Personal Expense Tracker. Goodbye!")
            break

        else:
            print("Invalid choice. Please select a valid option.")


if __name__ == "__main__":
    main()
