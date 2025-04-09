# This program creates a simple GUI application using Tkinter to display a pie chart
# that visualizes youth fruit preferences based on survey data. The chart shows 
# different fruits and their corresponding percentages based on the number of votes 
# each fruit received in a survey. Matplotlib is used to generate the pie chart, 
# and the chart is embedded in the Tkinter window using the FigureCanvasTkAgg widget.

# Author: Group 9

import tkinter as tk
from tkinter import font as tkfont
import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg

# Survey data
fruits = ['Avocado', 'Orange', 'Banana', 'Kiwifruit', 'Mango', 'Grapes']
people = [36, 41, 19, 28, 30, 16]
total = sum(people)  # Total number of responses
percentages = [round((count / total) * 100, 1)
               for count in people]  # Percentage per fruit

# RGB colors for each fruit slice (normalized to 0-1)
fruit_colors = [
    (0.82, 0.81, 0.41),   # Avocado
    (0.93, 0.55, 0.14),   # Orange
    (1.0, 0.87, 0.35),    # Banana
    (0.43, 0.51, 0.04),   # Kiwifruit
    (1.0, 0.51, 0.26),    # Mango
    (0.44, 0.18, 0.66)    # Grapes
]


def create_pie_chart():
    # Create a figure and axis
    fig, ax = plt.subplots(figsize=(6, 6), dpi=100)

    # Create the pie chart
    wedges, texts, autotexts = ax.pie(
        people,
        labels=[f"{f} ({p}%)" for f, p in zip(fruits, percentages)],
        colors=fruit_colors,
        autopct='',
        startangle=90,
        textprops={'fontsize': 10}
    )

    # Equal aspect ratio ensures the pie chart is circular
    ax.axis('equal')

    # Set the title
    ax.set_title("Youth Fruit Preferences in Gachororo", pad=20)

    return fig


class PieChartApp:
    def __init__(self, root):
        self.root = root
        self.root.title("Fruit Preferences Pie Chart")

        # Set window size
        self.root.geometry("800x800")

        # Create the pie chart
        fig = create_pie_chart()

        # Create a canvas to display the pie chart
        self.canvas = FigureCanvasTkAgg(fig, master=self.root)
        self.canvas.draw()
        self.canvas.get_tk_widget().pack(expand=True, fill=tk.BOTH)

        # Close button
        close_button = tk.Button(
            self.root,
            text="Close",
            command=self.root.quit,
            font=tkfont.Font(size=12)
        )
        close_button.pack(pady=10)


def main():
    root = tk.Tk()
    app = PieChartApp(root)
    root.mainloop()


if __name__ == "__main__":
    main()
