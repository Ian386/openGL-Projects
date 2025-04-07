import numpy as np
from OpenGL.GL import *
from OpenGL.GLU import *
from OpenGL.GLUT import *
import math

# Survey data
fruits = ['Avocado', 'Orange', 'Banana', 'Kiwifruit', 'Mango', 'Grapes']
people = [36, 41, 19, 28, 30, 16]
total = sum(people)  # Total number of responses
percentages = [round((count / total) * 100, 1) for count in people]  # Percentage per fruit

# RGB colors for each fruit slice
fruit_colors = [
    (0.82, 0.81, 0.41),   # Avocado 
    (0.93, 0.55, 0.14),   # Orange
    (1.0, 0.87, 0.35),    # Banana 
    (0.43, 0.51, 0.04),   # Kiwifruit 
    (1.0, 0.51, 0.26),    # Mango 
    (0.44, 0.18, 0.66)    # Grapes 
]

def draw_pie_chart():
    glClear(GL_COLOR_BUFFER_BIT)  # Clear the screen
    glLoadIdentity()              # Reset transformations

    # Draw the chart title
    glColor3f(0, 0, 0)  # Black color for text
    glRasterPos2f(-0.9, 0.9)  # Position the title near the top-left
    for c in "Youth Fruit Preferences in Gachororo":
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, ord(c))

    radius = 0.6          # Radius of the pie chart
    angle_start = 0       # Initial angle (in radians)

    # Loop through each fruit category to draw a slice
    for i in range(len(people)):
        # Calculate the angle that this slice will span
        angle_end = angle_start + 2 * math.pi * people[i] / total

        # Begin drawing a filled fan shape (slice)
        glBegin(GL_TRIANGLE_FAN)
        glColor3f(*fruit_colors[i])  # Set the slice color
        glVertex2f(0, 0)  # Center of the pie

        # Draw the edge of the slice from start to end angle
        segments = 50  # Controls smoothness of curve
        for j in range(segments + 1):
            angle = angle_start + (angle_end - angle_start) * j / segments
            glVertex2f(radius * math.cos(angle), radius * math.sin(angle))
        glEnd()

        # Position label near the outer edge of the slice
        mid_angle = (angle_start + angle_end) / 2
        label_x = radius * math.cos(mid_angle)
        label_y = radius * math.sin(mid_angle)

        # Slightly offset the label outward
        offset = 0.12
        label_x += offset * math.cos(mid_angle)
        label_y += offset * math.sin(mid_angle)

        # Draw the label (fruit name + percentage)
        glColor3f(0, 0, 0)  # Label text color
        glRasterPos2f(label_x, label_y)
        label = f"{fruits[i]} ({percentages[i]}%)"
        for c in label:
            glutBitmapCharacter(GLUT_BITMAP_9_BY_15, ord(c))

        angle_start = angle_end  # Update for next slice

    glutSwapBuffers()  # Swap the double buffers (display result)

def init():
    # Set background color to white
    glClearColor(1.0, 1.0, 1.0, 1.0)

    # Set the 2D orthographic projection (no perspective)
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0)

def main():
    glutInit()  # Initialize GLUT
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB)  # Double buffering + RGB color mode
    glutInitWindowSize(800, 800)  # Window size in pixels
    glutCreateWindow(b"Fruit Preferences Pie Chart")  # Create window with title
    glutDisplayFunc(draw_pie_chart)  # Set the display callback
    init()  # Custom initialization
    glutMainLoop()  # Enter the event processing loop

if __name__ == "__main__":
    main()
