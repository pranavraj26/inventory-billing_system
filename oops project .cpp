
#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
using namespace std;

class Product {
public:
    int id, stock;
    string name;
    float price;

    void input() {
        cout << "Enter Product ID: ";
        cin >> id;
        cout << "Enter Product Name: ";
        cin >> name;
        cout << "Enter Price: ";
        cin >> price;
        cout << "Enter Quantity: ";
        cin >> stock;
    }

    void display() {
        cout << left << setw(10) << id
             << setw(15) << name
             << setw(12) << price
             << stock << endl;
    }
};

class Inventory {
public:
    vector<Product> products;

    void load() {
        ifstream file("inventory.txt");
        Product p;
        while (file >> p.id >> p.name >> p.price >> p.stock)
            products.push_back(p);
        file.close();
    }

    void save() {
        ofstream file("inventory.txt");
        for (auto p : products)
            file << p.id << " " << p.name << " "
                 << p.price << " " << p.stock << endl;
        file.close();
    }

    void addProduct() {
        Product p;
        p.input();

        if (findProduct(p.id) != -1) {
            cout << "Product ID already exists!\n";
            return;
        }

        products.push_back(p);
        save();
        cout << "Product added successfully!\n";
    }

    int findProduct(int id) {
        for (int i = 0; i < products.size(); i++)
            if (products[i].id == id)
                return i;
        return -1;
    }

    void displayInventory() {
        cout << "\nID        Name           Price       Stock\n";
        for (auto p : products)
            p.display();
    }

    void searchProduct() {
        int id;
        cout << "Enter Product ID: ";
        cin >> id;

        int index = findProduct(id);

        if (index == -1)
            cout << "Product not found!\n";
        else
            products[index].display();
    }
};

class BillingSystem {
    Inventory inventory;

public:
    BillingSystem() {
        inventory.load();
    }

    void generateBill() {
        int id, quantity;
        float total = 0;
        vector<pair<int, int>> bill;

        char choice = 'y';

        while (choice == 'y' || choice == 'Y') {
            inventory.displayInventory();

            cout << "\nEnter Product ID: ";
            cin >> id;

            int index = inventory.findProduct(id);

            if (index == -1) {
                cout << "Product not found!\n";
                continue;
            }

            cout << "Enter Quantity: ";
            cin >> quantity;

            if (quantity <= 0 ||
                quantity > inventory.products[index].stock) {
                cout << "Insufficient stock!\n";
                continue;
            }

            bill.push_back({index, quantity});

            total += inventory.products[index].price * quantity;

            cout << "Add another product? (y/n): ";
            cin >> choice;
        }

        if (bill.empty()) {
            cout << "No items in bill.\n";
            return;
        }

        cout << "\n========== BILL ==========\n";
        cout << "Total Amount: Rs. " << total << endl;

        char payment;
        cout << "Confirm Payment? (y/n): ";
        cin >> payment;

        if (payment == 'y' || payment == 'Y') {
            for (auto item : bill)
                inventory.products[item.first].stock -= item.second;

            inventory.save();
            cout << "Payment Successful!\n";
            cout << "Inventory Updated Successfully!\n";
        }
        else {
            cout << "Payment Cancelled!\n";
            cout << "Inventory remains unchanged.\n";
        }
    }

    void menu() {
        int choice;

        do {
            cout << "\n===== INVENTORY MANAGEMENT SYSTEM =====\n";
            cout << "1. Add Product\n";
            cout << "2. View Inventory\n";
            cout << "3. Search Product\n";
            cout << "4. Generate Bill\n";
            cout << "5. Exit\n";
            cout << "Enter Choice: ";
            cin >> choice;

            switch (choice) {
                case 1:
                    inventory.addProduct();
                    break;

                case 2:
                    inventory.displayInventory();
                    break;

                case 3:
                    inventory.searchProduct();
                    break;

                case 4:
                    generateBill();
                    break;

                case 5:
                    cout << "Thank You!\n";
                    break;

                default:
                    cout << "Invalid Choice!\n";
            }

        } while (choice != 5);
    }
};

int main() {
    BillingSystem system;
    system.menu();
    return 0;
}
