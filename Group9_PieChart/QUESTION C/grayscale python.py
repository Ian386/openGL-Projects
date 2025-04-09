# Import necessary libraries
import tkinter as tk
from tkinter import font as tkfont
import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import numpy as np

# --------------------------
# DATA CONFIGURATION SECTION
# --------------------------

# Define survey data about fruit preferences
fruits = ['Avocado', 'Orange', 'Banana', 'Kiwifruit', 'Mango', 'Grapes']
people = [36, 41, 19, 28, 30, 16]  # Number of votes per fruit
total = sum(people)  # Total number of participants
percentages = [round((count / total) * 100, 1) for count in people]  # Calculate percentages

# Original RGB colors for each fruit (normalized 0-1 values)
original_colors = [
    (0.82, 0.81, 0.41),   # Avocado (light green)
    (0.93, 0.55, 0.14),   # Orange (orange)
    (1.0, 0.87, 0.35),    # Banana (yellow)
    (0.43, 0.51, 0.04),   # Kiwifruit (dark green)
    (1.0, 0.51, 0.26),    # Mango (orange-red)
    (0.44, 0.18, 0.66)    # Grapes (purple)
]

# Convert colors to grayscale using luminance formula: 0.299*R + 0.587*G + 0.114*B
# Multiply by 3 to create RGB tuple (gray, gray, gray) for matplotlib compatibility
fruit_colors = [(0.299*r + 0.587*g + 0.114*b,) * 3 for r, g, b in original_colors]

# --------------------------
# PIE CHART CREATION FUNCTION
# --------------------------

def create_pie_chart():
    """Create and configure the pie chart with labels and styling"""
    
    # Initialize figure with black background
    fig = plt.figure(figsize=(8, 8), dpi=100, facecolor='black')
    ax = fig.add_subplot(111)
    ax.set_facecolor('black')  # Set plot area background

    # Create pie chart with custom parameters
    wedges, texts = ax.pie(
        people,
        colors=fruit_colors,
        startangle=90,  # Rotate chart to start at top
        wedgeprops={
            'linewidth': 1,  # Border thickness
            'edgecolor': 'white'  # Border color
        },
        radius=0.9  # Reduce pie size to leave space for labels
    )

    # Configure label styling parameters
    bbox_props = dict(
        boxstyle="round,pad=0.2",  # Rounded label background
        fc="black",  # Background color
        ec="white",  # Border color
        lw=0.8  # Border line width
    )
    
    # Annotation configuration dictionary
    kw = dict(
        arrowprops=dict(
            arrowstyle="-",  # Simple line arrow
            color='white',  # Line color
            linewidth=0.8  # Line thickness
        ),
        bbox=bbox_props,
        zorder=0,  # Drawing order
        va="center",  # Vertical text alignment
        ha="center"  # Horizontal text alignment
    )

    # --------------------------
    # LABEL POSITIONING SYSTEM
    # --------------------------
    for i, wedge in enumerate(wedges):
        # Calculate middle angle of current wedge
        ang = (wedge.theta2 - wedge.theta1)/2. + wedge.theta1
        
        # Convert angle to radians for trigonometric calculations
        rad = np.deg2rad(ang)
        
        # Calculate coordinates for label positioning
        y = np.sin(rad)
        x = np.cos(rad)
        
        # Configure label positioning
        label_distance = 1.05  # 5% outside pie radius
        ax.annotate(
            f"{fruits[i]}\n({percentages[i]}%)",  # Label text
            xy=(x*0.95, y*0.95),  # Line start point (95% radius)
            xytext=(x*label_distance, y*label_distance),  # Label position
            color='white',  # Text color
            fontsize=10,  # Font size
            **kw  # Apply preconfigured styles
        )

    # Final chart formatting
    ax.axis('equal')  # Ensure pie remains circular
    ax.set_title("Grayscale Implementation", 
                color='white',  # Title color
                pad=25,  # Padding below title
                fontweight='bold')  # Title font weight
    return fig

# --------------------------
# GUI APPLICATION CLASS
# --------------------------

class PieChartApp:
    """Tkinter application wrapper for the pie chart visualization"""
    
    def __init__(self, root):
        """Initialize the GUI window and components"""
        self.root = root
        self.root.title("Grayscale Fruit Preferences")  # Window title
        self.root.geometry("900x900")  # Window dimensions
        self.root.configure(bg='black')  # Window background color

        # Create and embed the pie chart
        fig = create_pie_chart()
        self.canvas = FigureCanvasTkAgg(fig, master=self.root)
        self.canvas.draw()
        self.canvas.get_tk_widget().pack(expand=True, fill=tk.BOTH)  # Responsive layout

        # Configure close button
        close_button = tk.Button(
            self.root,
            text="Close",
            command=self.root.quit,  # Exit command
            font=tkfont.Font(size=11, weight='bold'),  # Font styling
            bg='#404040',  # Background color
            fg='white',  # Text color
            activebackground='#606060',  # Clicked state color
            relief='flat'  # Remove 3D button effect
        )
        close_button.pack(pady=15, ipadx=15, ipady=3)  # Positioning and padding

# --------------------------
# MAIN EXECUTION
# --------------------------

def main():
    """Entry point of the application"""
    root = tk.Tk()  # Create root window
    app = PieChartApp(root)  # Initialize application
    root.mainloop()  # Start GUI event loop

if __name__ == "__main__":
    main()  # Run the application when executed directly