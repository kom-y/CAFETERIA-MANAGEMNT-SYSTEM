import tkinter as tk
from tkinter import ttk, messagebox
from datetime import datetime


class CafeApp:
    def __init__(self, root):
        self.root = root
        self.root.title("Cafe Management")
        self.root.geometry("800x600")
        self.root.configure(bg="#f0f0f5")  # Light gray background

        # Apply styles
        self.setup_styles()

        # Menu and Orders data
        self.menu = {
            1: {"name": "Chicken Burger", "price": 150},
            2: {"name": "Zinger Burger", "price": 250},
            3: {"name": "French Fries", "price": 50},
            4: {"name": "Juice", "price": 100},
            5: {"name": "Coffee", "price": 80},
        }
        self.orders = []

        # UI setup
        self.setup_ui()

    def setup_styles(self):
        style = ttk.Style()
        style.theme_use("clam")  # Modern theme
        style.configure("TLabel", background="#f0f0f5", font=("Arial", 12))
        style.configure("TButton", font=("Arial", 11), padding=5, background="#4caf50", foreground="white")
        style.map("TButton", background=[("active", "#45a049")])  # Active button color
        style.configure("Treeview", font=("Arial", 11), rowheight=25)
        style.configure("Treeview.Heading", font=("Arial Bold", 12), background="#4caf50", foreground="white")

    def setup_ui(self):
        # Menu Frame
        menu_frame = tk.LabelFrame(self.root, text="Menu", padx=10, pady=10, bg="#ffffff", font=("Arial", 12, "bold"))
        menu_frame.pack(fill="x", padx=10, pady=5)

        self.menu_table = ttk.Treeview(menu_frame, columns=("Item No", "Name", "Price"), show="headings", height=5)
        self.menu_table.heading("Item No", text="Item No")
        self.menu_table.heading("Name", text="Name")
        self.menu_table.heading("Price", text="Price")
        self.menu_table.column("Item No", width=80, anchor="center")
        self.menu_table.column("Name", width=200, anchor="center")
        self.menu_table.column("Price", width=100, anchor="center")
        self.menu_table.pack(fill="x", padx=5, pady=5)

        for item_no, details in self.menu.items():
            self.menu_table.insert("", "end", values=(item_no, details["name"], details["price"]))

        # Order Frame
        order_frame = tk.LabelFrame(self.root, text="Orders", padx=10, pady=10, bg="#ffffff", font=("Arial", 12, "bold"))
        order_frame.pack(fill="x", padx=10, pady=5)

        self.order_table = ttk.Treeview(order_frame, columns=("Item No", "Name", "Quantity", "Total Price"), show="headings", height=5)
        self.order_table.heading("Item No", text="Item No")
        self.order_table.heading("Name", text="Name")
        self.order_table.heading("Quantity", text="Quantity")
        self.order_table.heading("Total Price", text="Total Price")
        self.order_table.column("Item No", width=80, anchor="center")
        self.order_table.column("Name", width=200, anchor="center")
        self.order_table.column("Quantity", width=100, anchor="center")
        self.order_table.column("Total Price", width=120, anchor="center")
        self.order_table.pack(fill="x", padx=5, pady=5)

        # Controls
        control_frame = tk.Frame(self.root, bg="#f0f0f5")
        control_frame.pack(fill="x", padx=10, pady=5)

        ttk.Label(control_frame, text="Item No:").grid(row=0, column=0, padx=5, pady=5)
        self.item_no_var = tk.StringVar()
        ttk.Entry(control_frame, textvariable=self.item_no_var, width=10).grid(row=0, column=1, padx=5, pady=5)

        ttk.Label(control_frame, text="Quantity:").grid(row=0, column=2, padx=5, pady=5)
        self.quantity_var = tk.StringVar()
        ttk.Entry(control_frame, textvariable=self.quantity_var, width=10).grid(row=0, column=3, padx=5, pady=5)

        ttk.Button(control_frame, text="Place Order", command=self.place_order).grid(row=0, column=4, padx=5, pady=5)
        ttk.Button(control_frame, text="Modify Order", command=self.modify_order).grid(row=0, column=5, padx=5, pady=5)
        ttk.Button(control_frame, text="Delete Order", command=self.delete_order).grid(row=0, column=6, padx=5, pady=5)
        ttk.Button(control_frame, text="Generate Bill", command=self.generate_bill).grid(row=0, column=7, padx=5, pady=5)

        # Total Label
        self.total_label = ttk.Label(self.root, text="Total: 0", font=("Arial", 16))
        self.total_label.pack(pady=5)

    def place_order(self):
        try:
            item_no = int(self.item_no_var.get())
            quantity = int(self.quantity_var.get())

            if item_no not in self.menu:
                messagebox.showerror("Error", "Invalid Item No!")
                return

            item = self.menu[item_no]
            total_price = item["price"] * quantity
            self.orders.append({"item_no": item_no, "name": item["name"], "quantity": quantity, "total_price": total_price})

            self.refresh_order_table()
            self.update_total()
            messagebox.showinfo("Success", f"Order placed: {item['name']} x {quantity}")
        except ValueError:
            messagebox.showerror("Error", "Please enter valid item number and quantity!")

    def modify_order(self):
        try:
            item_no = int(self.item_no_var.get())
            quantity = int(self.quantity_var.get())

            for order in self.orders:
                if order["item_no"] == item_no:
                    order["quantity"] = quantity
                    order["total_price"] = self.menu[item_no]["price"] * quantity
                    self.refresh_order_table()
                    self.update_total()
                    messagebox.showinfo("Success", f"Order modified: Item No {item_no} now has quantity {quantity}")
                    return

            messagebox.showerror("Error", "Order not found!")
        except ValueError:
            messagebox.showerror("Error", "Please enter valid item number and quantity!")

    def delete_order(self):
        try:
            item_no = int(self.item_no_var.get())
            self.orders = [order for order in self.orders if order["item_no"] != item_no]
            self.refresh_order_table()
            self.update_total()
            messagebox.showinfo("Success", f"Order deleted: Item No {item_no}")
        except ValueError:
            messagebox.showerror("Error", "Please enter a valid item number!")

    def generate_bill(self):
        if not self.orders:
            messagebox.showerror("Error", "No orders to generate bill!")
            return

        # Display the receipt in a new window
        receipt_window = tk.Toplevel(self.root)
        receipt_window.title("Bill Receipt")
        receipt_window.geometry("400x400")
        receipt_window.configure(bg="#ffffff")

        receipt_text = tk.Text(receipt_window, wrap="word", font=("Arial", 12))
        receipt_text.pack(expand=True, fill="both", padx=10, pady=10)

        now = datetime.now()
        receipt_content = f"Bill Receipt - {now.strftime('%Y-%m-%d %H:%M:%S')}\n"
        receipt_content += "=" * 30 + "\n"

        total = 0
        for order in self.orders:
            total_price = order["total_price"]
            receipt_content += f"{order['item_no']} - {order['name']} - {order['quantity']} x {self.menu[order['item_no']]['price']} = ${total_price}\n"
            total += total_price

        receipt_content += "=" * 30 + "\n"
        receipt_content += f"Total: ${total}\n"

        receipt_text.insert("1.0", receipt_content)
        receipt_text.config(state="disabled")

        # Clear orders and update UI
        self.orders.clear()
        self.refresh_order_table()
        self.update_total()

    def refresh_order_table(self):
        for row in self.order_table.get_children():
            self.order_table.delete(row)

        for order in self.orders:
            self.order_table.insert("", "end", values=(order["item_no"], order["name"], order["quantity"], order["total_price"]))

    def update_total(self):
        total = sum(order["total_price"] for order in self.orders)
        self.total_label.config(text=f"Total: {total}")


# Main Application
if __name__ == "__main__":
    root = tk.Tk()
    app = CafeApp(root)
    root.mainloop()
