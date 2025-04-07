from OpenGL.GL import *
from OpenGL.GLUT import *
from OpenGL.GLU import *
import math
import sys

# Window dimensions
WINDOW_WIDTH = 600
WINDOW_HEIGHT = 600

# Chart data configuration
values = [36.0, 41.0, 19.0, 28.0, 30.0, 16.0]
labels = [
    "Avocado",
    "Orange",
    "Banana",
    "Kiwifruit",
    "Mangos",
    "Grapes"
]

# Grayscale colors (R=G=B) using luminance-preserving conversion
grayscale_colors = [
    [0.40, 0.40, 0.40],  # Avocado
    [0.59, 0.59, 0.59],  # Orange
    [0.89, 0.89, 0.89],  # Banana
    [0.61, 0.61, 0.61],  # Kiwifruit
    [0.77, 0.77, 0.77],  # Mangos
    [0.21, 0.21, 0.21]   # Grapes
]

NUM_SLICES = len(values)
CHART_TITLE = "Youth Fruit Preferences in Gachororo"
PI = math.pi

def draw_bitmap_text(x, y, text):
    """Draw text using GLUT bitmap characters"""
    glRasterPos2f(x, y)
    for character in text:
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, ord(character))

def display():
    """Main display callback function"""
    glClear(GL_COLOR_BUFFER_BIT)
    
    # Calculate total value for percentages
    total = sum(values)
    
    # Pie chart configuration
    center_x, center_y = 0.0, 0.0
    radius = 0.5
    current_angle = 0.0
    
    for i in range(NUM_SLICES):
        # Calculate slice dimensions
        slice_percentage = values[i] / total
        slice_angle = slice_percentage * 360.0
        
        # Set grayscale color
        glColor3f(*grayscale_colors[i])
        
        # Draw pie slice using triangle fan
        glBegin(GL_TRIANGLE_FAN)
        glVertex2f(center_x, center_y)  # Center point
        angle = current_angle
        while angle <= current_angle + slice_angle:
            rad = math.radians(angle)
            glVertex2f(center_x + math.cos(rad) * radius, 
                      center_y + math.sin(rad) * radius)
            angle += 1.0
        # Final vertex to complete the slice
        end_rad = math.radians(current_angle + slice_angle)
        glVertex2f(center_x + math.cos(end_rad) * radius,
                  center_y + math.sin(end_rad) * radius)
        glEnd()
        
        # Label positioning
        mid_angle = current_angle + (slice_angle / 2.0)
        mid_rad = math.radians(mid_angle)
        label_radius = radius + 0.1
        
        # Special adjustments
        if labels[i] == "Banana":
            label_radius = radius + 0.42
        elif labels[i] == "Kiwifruit":
            label_radius = radius + 0.20
            mid_rad += 0.05  # Angular adjustment
            
        # Calculate label position
        label_x = center_x + math.cos(mid_rad) * label_radius
        label_y = center_y + math.sin(mid_rad) * label_radius
        
        # Create label text
        label_text = f"{labels[i]} ({slice_percentage * 100:.1f}%)"
        
        # Draw white text
        glColor3f(1.0, 1.0, 1.0)
        draw_bitmap_text(label_x, label_y, label_text)
        
        # Draw white separation lines
        glColor3f(1.0, 1.0, 1.0)
        glBegin(GL_LINES)
        start_rad = math.radians(current_angle)
        glVertex2f(center_x, center_y)
        glVertex2f(center_x + math.cos(start_rad) * radius,
                  center_y + math.sin(start_rad) * radius)
        glEnd()
        
        current_angle += slice_angle
    
    # Draw outer boundary
    glColor3f(1.0, 1.0, 1.0)
    glBegin(GL_LINE_LOOP)
    for angle in range(360):
        rad = math.radians(angle)
        glVertex2f(center_x + math.cos(rad) * radius,
                  center_y + math.sin(rad) * radius)
    glEnd()
    
    # Draw chart title
    glColor3f(1.0, 1.0, 1.0)
    draw_bitmap_text(-0.35, 0.8, CHART_TITLE)
    
    glutSwapBuffers()

def reshape(width, height):
    """Window reshape callback"""
    glViewport(0, 0, width, height)
    glMatrixMode(GL_PROJECTION)
    glLoadIdentity()
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0)
    glMatrixMode(GL_MODELVIEW)

def main():
    """Main function"""
    glutInit(sys.argv)
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB)
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT)
    glutCreateWindow(b"Grayscale Fruit Consumption Chart")
    
    # Set black background
    glClearColor(0.0, 0.0, 0.0, 1.0)
    
    # Register callbacks
    glutDisplayFunc(display)
    glutReshapeFunc(reshape)
    
    glutMainLoop()

if __name__ == "__main__":
    main()