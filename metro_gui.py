import tkinter as tk
from tkinter import ttk, messagebox
import pymetro


class MetroGUI:
    def __init__(self, root):
        self.root = root
        self.root.title("Метро - Поиск маршрута")
        self.root.geometry("600x500")


        self.metro = pymetro.Metro()
        self.load_data()


        ttk.Label(root, text="Поиск кратчайшего маршрута", font=('Arial', 14)).pack(pady=10)


        frame = ttk.Frame(root, padding=20)
        frame.pack(fill='both', expand=True)


        ttk.Label(frame, text="От станции:").grid(row=0, column=0, sticky='w', pady=5)
        self.from_entry = ttk.Entry(frame, width=30)
        self.from_entry.grid(row=0, column=1, pady=5)


        ttk.Label(frame, text="До станции:").grid(row=1, column=0, sticky='w', pady=5)
        self.to_entry = ttk.Entry(frame, width=30)
        self.to_entry.grid(row=1, column=1, pady=5)


        ttk.Button(frame, text="Найти маршрут", command=self.find_route).grid(row=2, column=0, columnspan=2, pady=20)


        ttk.Label(frame, text="Результат:").grid(row=3, column=0, sticky='nw', pady=5)


        self.result_text = tk.Text(frame, height=15, width=50)
        self.result_text.grid(row=4, column=0, columnspan=2)


        scroll = ttk.Scrollbar(frame, command=self.result_text.yview)
        scroll.grid(row=4, column=2, sticky='ns')
        self.result_text.config(yscrollcommand=scroll.set)


    def load_data(self):
        self.metro.addLine(1, "Red", False)
        self.metro.addUsualStation("A", 100, 1)
        self.metro.addUsualStation("B", 120, 1)
        self.metro.addUsualStation("C", 90, 1)


        self.metro.addStationToLine(1, "A")
        self.metro.addStationToLine(1, "B")
        self.metro.addStationToLine(1, "C")


        self.metro.addSpan(1, "A", "B", 2)
        self.metro.addSpan(1, "B", "C", 3)


    def find_route(self):
        try:
            from_station = self.from_entry.get().strip()
            to_station = self.to_entry.get().strip()


            if not from_station or not to_station:
                raise ValueError("Введите обе станции")


            if from_station == to_station:
                raise ValueError("Станции должны быть разными")


            path, time = self.metro.shortestRoute(from_station, to_station)


            if not path:
                raise ValueError("Маршрут не найден") # проверялось на примере, где линия не связная

            self.result_text.delete(1.0, tk.END)
            self.result_text.insert(tk.END, f"Время в пути: {time} минут\n\n")
            self.result_text.insert(tk.END, "Маршрут:\n")
            for i, station in enumerate(path, 1):
                self.result_text.insert(tk.END, f"{i}. {station}\n")


        except ValueError as e:
            messagebox.showerror("Ошибка ввода", str(e))
        except Exception as e:
            messagebox.showerror("Ошибка", f"Произошла ошибка: {str(e)}")


if __name__ == "__main__":
    root = tk.Tk()
    app = MetroGUI(root)
    root.mainloop()