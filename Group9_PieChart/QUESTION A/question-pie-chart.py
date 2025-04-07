"""
-The program below creates a pie chart using Tkinter's Canvas widget.
-We computed the angle for each slice based on given values and drew each slice as a polygon to ensure
accurate boundaries.
-White radial lines delineate each slice, and labels (with percentage values)are placed just outside the pie. 
-Specific labels (such as "Banana", "Grapes", and "Ovacado") have custom x/y offsets applied to adjust their positions. For "Banana", a newline is inserted
between the fruit name and its percentage value. 
-The overall design uses a black background with white outlines and text for a clear, high-contrast visualization.
"""

import tkinter as tk
import math

# ---------------------------
#   Data and configuration
# ---------------------------
values = [36.0, 41.0, 19.0, 28.0, 30.0, 16.0]               # Data values for each pie slice
labels = ["Ovacado", "Orange", "Banana", "Kiwifruit", "Mangos", "Grapes"]  # Labels for each slice
chartTitle = "Youth Fruit Preferences in Gachororo"           # Title of the chart
total = sum(values)                                           # Total value to compute percentages

# Window and Canvas settings
canvas_width = 600                                            # Width of the canvas/window
canvas_height = 600                                           # Height of the canvas/window
centerX, centerY = canvas_width / 2, canvas_height / 2         # Center coordinates of the pie chart
radius = 200                                                  # Radius of the pie chart
label_offset = 30                                             # Base distance beyond the pie's circumference for labels

# Create the main window and canvas with a black background
root = tk.Tk()
root.title("Fruit Consumption Pie Chart")
canvas = tk.Canvas(root, width=canvas_width, height=canvas_height, bg="black")
canvas.pack()

currentAngle = 0.0  # starting angle in degrees for the first slice

# Optional: Dictionary for label-specific offsets.
# Positive x_offset moves text to the right; negative moves it to the left.
# Positive y_offset moves text down; negative moves it up.
label_offsets = {
    "Kiwifruit": (-20, 0),  # Shift Kiwifruit label to the left by 20 pixels
    "Banana":    (-5, 0),   # Shift Banana label slightly to the left by 5 pixels
    "Grapes":    (25, 0),   # Shift Grapes label to the right by 25 pixels
    "Ovacado":   (30, 0),   # Shift Ovacado label to the right by 30 pixels
}

def point_on_circle(angle_deg):
    """
    Returns the (x, y) coordinates of a point on the circle at a given angle (in degrees).
    This is used to calculate the boundary points of each pie slice.
    """
    angle_rad = math.radians(angle_deg)
    x = centerX + radius * math.cos(angle_rad)
    y = centerY + radius * math.sin(angle_rad)
    return x, y

# Draw each pie slice as a polygon to ensure precise boundaries.
for i, val in enumerate(values):
    slicePercentage = val / total                  # Calculate percentage for the slice
    sliceAngle = slicePercentage * 360.0             # Convert percentage to angle in degrees
    startAngle = currentAngle                        # Starting angle for the current slice
    endAngle = currentAngle + sliceAngle             # Ending angle for the current slice

    # Create a list of points defining the polygon (wedge) for the slice.
    points = [centerX, centerY]                      # Start at the center of the pie
    step = 1.0                                       # Degree step for smoothness of the arc
    angle = startAngle
    while angle < endAngle:
        x, y = point_on_circle(angle)
        points.extend([x, y])
        angle += step
    # Ensure the last point is exactly at the end angle.
    x, y = point_on_circle(endAngle)
    points.extend([x, y])
    
    # Draw the wedge as a filled polygon in black with a white outline.
    canvas.create_polygon(points, fill="black", outline="white")
    
    # Draw radial boundary lines at the start and end angles of the slice.
    start_pt = point_on_circle(startAngle)
    end_pt = point_on_circle(endAngle)
    canvas.create_line(centerX, centerY, start_pt[0], start_pt[1], fill="white")
    canvas.create_line(centerX, centerY, end_pt[0], end_pt[1], fill="white")
    
    # Compute the midpoint angle of the slice for label placement.
    midAngle = startAngle + sliceAngle / 2.0
    midRad = math.radians(midAngle)
    labelX = centerX + (radius + label_offset) * math.cos(midRad)
    labelY = centerY + (radius + label_offset) * math.sin(midRad)
    
    # Apply additional offsets if defined for specific labels.
    if labels[i] in label_offsets:
        offset_x, offset_y = label_offsets[labels[i]]
        labelX += offset_x
        labelY += offset_y
    
    # Build label text, inserting a newline in the "Banana" label after the word.
    pct = slicePercentage * 100
    if labels[i] == "Banana":
        labelText = f"{labels[i]}\n({pct:.1f}%)"   # "Banana" on the first line, percentage on the second
    else:
        labelText = f"{labels[i]} ({pct:.1f}%)"
    
    # Place the label text on the canvas.
    canvas.create_text(labelX, labelY, text=labelText, fill="white", font=("Helvetica", 12))
    
    # Update the current angle to the end of the current slice for the next iteration.
    currentAngle = endAngle

# Draw the overall outer circle (white boundary) for a clean edge.
canvas.create_oval(
    centerX - radius, centerY - radius,
    centerX + radius, centerY + radius,
    outline="white"
)

# Draw the chart title near the top of the canvas.
canvas.create_text(centerX, 50, text=chartTitle, fill="white", font=("Helvetica", 16))

# Start the Tkinter event loop.
root.mainloop()
