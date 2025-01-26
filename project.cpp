#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class MenuNode {
public:
    string item;
    int price, itemNo;
    MenuNode* next;

    MenuNode(int IN, string I, int p) {
    	itemNo = IN;
    	item= I;
		price= p; 
		next = NULL;
	}

    string getItem() { return item; }
    int getItemNo() { return itemNo; }
    int getPrice() { return price; }
};

class OrderNode {
public:
    int itemNo;
    int quantity;
    OrderNode* left;
    OrderNode* right;

    OrderNode(int itemN, int Quantity) {
		itemNo = itemN;
		quantity =Quantity;
		left = NULL;
		right = NULL;
	}

    int operator*(MenuNode* m) {
        return quantity * m->getPrice();
    }
};

class CafeMenu {
private:
    MenuNode* head;

public:
    CafeMenu() {
		head = NULL;
	}

    void add(int ItemNo, string Item, int Price) {
        MenuNode* newItem = new MenuNode(ItemNo, Item, Price);
        if (!head) {
            head = newItem;
        } else {
            MenuNode* temp = head;
            while (temp->next) {
                temp = temp->next;
            }
            temp->next = newItem;
        }
    }

    void display_menu() {
        MenuNode* temp = head;
        while (temp) {
            cout << temp->getItemNo() << " - " << temp->getItem() << " " << temp->getPrice() << endl;
            temp = temp->next;
        }
    }

    void bubbleSort() {
        bool swapped;
        MenuNode* ptr1;
        MenuNode* lptr = NULL;

        if (!head)
            return;

        do {
            swapped = false;
            ptr1 = head;

            while (ptr1->next != lptr) {
                if (ptr1->getPrice() > ptr1->next->getPrice()) {
                    swap(ptr1->itemNo, ptr1->next->itemNo);
                    swap(ptr1->item, ptr1->next->item);
                    swap(ptr1->price, ptr1->next->price);
                    swapped = true;
                }
                ptr1 = ptr1->next;
            }
            lptr = ptr1;
        } while (swapped);
    }

    MenuNode* searchMenu(int itemNo) {
        MenuNode* temp = head;
        while (temp) {
            if (temp->getItemNo() == itemNo)
                return temp;
            temp = temp->next;
        }
        return NULL;
    }
};

class OrderBST {
private:
    OrderNode* root;

    OrderNode* insertRec(OrderNode* root, int itemNo, int quantity) {
        if (!root) {
            return new OrderNode(itemNo, quantity);
        }
        if (itemNo < root->itemNo) {
            root->left = insertRec(root->left, itemNo, quantity);
        } else if (itemNo > root->itemNo) {
            root->right = insertRec(root->right, itemNo, quantity);
        }
        return root;
    }

    OrderNode* removeRec(OrderNode* root, int itemNo) {
        if (!root)
            return root;

        if (itemNo < root->itemNo) {
            root->left = removeRec(root->left, itemNo);
        } else if (itemNo > root->itemNo) {
            root->right = removeRec(root->right, itemNo);
        } else {
            if (!root->left) {
                OrderNode* temp = root->right;
                delete root;
                return temp;
            } else if (!root->right) {
                OrderNode* temp = root->left;
                delete root;
                return temp;
            }

            OrderNode* temp = minValueNode(root->right);
            root->itemNo = temp->itemNo;
            root->quantity = temp->quantity;
            root->right = removeRec(root->right, temp->itemNo);
        }
        return root;
    }

    OrderNode* minValueNode(OrderNode* node) {
        while (node && node->left)
            node = node->left;
        return node;
    }

    void printOrdersRec(OrderNode* node) {
        if (!node)
            return;
        printOrdersRec(node->left);
        cout << "Item No: " << node->itemNo << " X " << node->quantity << endl;
        printOrdersRec(node->right);
    }

    void traverseAndBill(OrderNode* node, CafeMenu& menu, ofstream& bill, int& total) {
        if (!node)
            return;
        traverseAndBill(node->left, menu, bill, total);

        MenuNode* menuItem = menu.searchMenu(node->itemNo);
        if (menuItem) {
            int cost = node->operator*(menuItem);
            bill << node->itemNo << ": " << menuItem->getItem() << " " << menuItem->getPrice() << " X " << node->quantity << " = " << cost << endl;
            total += cost;
        }

        traverseAndBill(node->right, menu, bill, total);
    }

public:
    OrderBST() {
		root = NULL;
	}

    void insert(int itemNo, int quantity) {
        root = insertRec(root, itemNo, quantity);
    }

    void remove(int itemNo) {
        root = removeRec(root, itemNo);
    }

    void printOrders() {
        printOrdersRec(root);
    }

    OrderNode* getRoot() {
        return root;
    }

    void generateBill(CafeMenu& menu) {
        ofstream bill("Receipt.txt");
        if (!bill) {
            cout << "Cannot create receipt file!" << endl;
            return;
        }

        int total = 0;
        bill << "===============================" << endl;
        bill << "Bill Receipt" << endl;
        bill << "===============================" << endl;

        traverseAndBill(root, menu, bill, total);

        bill << "===============================" << endl;
        bill << "Total: " << total << endl;
        cout << "Bill Receipt generated (check Receipt.txt)" << endl;

        bill.close();
    }
};

class Cafe {
private:
    CafeMenu menu;
    OrderBST orders;

public:
    void addMenuItem(int ItemNo, string Item, int Price) {
        menu.add(ItemNo, Item, Price);
    }

    void displayMenu() {
        menu.display_menu();
    }

    void sortMenu() {
        menu.bubbleSort();
    }

    void placeOrder(int itemNo, int quantity) {
        if (MenuNode* menuItem = menu.searchMenu(itemNo)) {
            orders.insert(itemNo, quantity);
            cout << "Order placed: " << menuItem->getItem() << " X " << quantity << endl;
        } else {
            cout << "Item not found in the menu!" << endl;
        }
    }

    void removeOrder(int itemNo) {
        orders.remove(itemNo);
        cout << "Item removed from order." << endl;
    }

    void showOrders() {
        cout << "<<<<< Current Orders >>>>>" << endl;
        orders.printOrders();
    }

    void generateReceipt() {
        orders.generateBill(menu);
    }
};

int main() {
    Cafe cafe;
    cafe.addMenuItem(1, "Chicken Burger", 150);
    cafe.addMenuItem(2, "Zinger Burger", 250);
    cafe.addMenuItem(3, "French Fries", 50);
    cafe.addMenuItem(4, "Juice", 100);
    cafe.addMenuItem(5, "Coffee", 80);
   
    cout << "<<<<< Cafeteria Menu >>>>>" << endl;
	cafe.displayMenu();
	
	char choice;
	do {
	    cout << "\nDo you want to place an order? (y/n): ";
	    cin >> choice;
	
	    if (choice == 'y') {
	        int itemNo, quantity;
	        cout << "Enter item No: ";
	        cin >> itemNo;
	        cout << "Enter quantity: ";
	        cin >> quantity;
	        cafe.placeOrder(itemNo, quantity);
			
	        char modifyChoice;
	        do {
	            cout << "Do you want to modify your order? (m) Modify, (f) Finalize, (c) Cancel: ";
	            cin >> modifyChoice;
	
	            if (modifyChoice == 'm') {
	        		
	        		cout << "\nDo you want to view the sorted menu? (y/n): ";
		            char viewChoice;
		            cin >> viewChoice;
		
		            if (viewChoice == 'y') {
		                cout << "\n<<<<< Sorted Cafeteria Menu (by Price) >>>>>" << endl;
		                cafe.sortMenu();
		                cafe.displayMenu();
		            }
	        		
			        int option;
	                cout << "Choose an option: 1) Add Order  2) Remove Order: ";
	                cin >> option;
	
	                if (option == 1) {
	                    int itemNo, quantity;
	                    cout << "Enter item No to add: ";
	                    cin >> itemNo;
	                    cout << "Enter quantity: ";
	                    cin >> quantity;
	                    cafe.placeOrder(itemNo, quantity);
	                } else if (option == 2) {
	                    int itemNo;
	                    cout << "Enter item No to remove: ";
	                    cin >> itemNo;
	                    cafe.removeOrder(itemNo);
	                } else {
	                    cout << "Invalid option!" << endl;
	                }
	
	
	            } else if (modifyChoice == 'f') {
	                cout << "Finalizing order..." << endl;
	                cafe.generateReceipt();
	                break;
	            } else if (modifyChoice == 'c') {
	                cout << "Order has been canceled." << endl;
	                break;
	            } else {
	                cout << "Invalid choice!" << endl;
	            }
	        } while (modifyChoice != 'f' && modifyChoice != 'c');
	
	        if (modifyChoice == 'f' || modifyChoice == 'c') {
	            break;
	        }
	    } else if (choice != 'n') {
	        cout << "Invalid choice!" << endl;
	    }
	} while (choice != 'n');


    return 0;
}
