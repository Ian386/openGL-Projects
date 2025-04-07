from OpenGL.GL import *
from OpenGL.GLU import *
from OpenGL.GLUT import *
import sys
import math

# Window dimensions.
WINDOW_WIDTH = 600
WINDOW_HEIGHT = 600

# Data for the pie chart.
values = [36.0, 41.0, 19.0, 28.0, 30.0, 16.0]
labels = ["Ovacado", "Orange", "Banana", "Kiwifruit", "Mangos", "Grapes"]

# Chart title.
chartTitle = "Youth Fruit Preferences in Gachororo"

# Pi constant.
PI = 3.14159265

# ---------------------------------------------------------------------
#  Helper function to draw bitmap text on the screen.
# ---------------------------------------------------------------------
def drawBitmapText(x, y, text):
    glRasterPos2f(x, y)
    for ch in text:
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, ord(ch))

# ---------------------------------------------------------------------
#  Display callback.
# ---------------------------------------------------------------------
def display():
    glClear(GL_COLOR_BUFFER_BIT)
    
    # Calculate the total of the data.
    total = sum(values)
    centerX, centerY = 0.0, 0.0
    radius = 0.5
    currentAngle = 0.0  # Starting angle in degrees.
    
    # Draw each pie slice.
    for i in range(len(values)):
        slicePercentage = values[i] / total
        sliceAngle = slicePercentage * 360.0
        
        # Draw the filled slice in black.
        glColor3f(0.0, 0.0, 0.0)
        glBegin(GL_TRIANGLE_FAN)
        glVertex2f(centerX, centerY)
        angle = currentAngle
        while angle <= currentAngle + sliceAngle:
            rad = angle * PI / 180.0
            glVertex2f(centerX + math.cos(rad) * radius, centerY + math.sin(rad) * radius)
            angle += 1.0
        # Ensure the last vertex exactly matches the end of the slice.
        rad = (currentAngle + sliceAngle) * PI / 180.0
        glVertex2f(centerX + math.cos(rad) * radius, centerY + math.sin(rad) * radius)
        glEnd()
        
        # Compute the midpoint angle for the label.
        midAngle = currentAngle + sliceAngle / 2.0
        midRad = midAngle * PI / 180.0
        labelRadius = radius + 0.1
        
        # Increase label distance for Banana alone
        if labels[i] == "Banana":
            labelRadius = radius + 0.42
        # increase label distance for Kiwifruit alone
        if labels[i] == "Kiwifruit":
            labelRadius = radius + 0.20
        
        labelX = centerX + math.cos(midRad) * labelRadius
        labelY = centerY + math.sin(midRad) * labelRadius
        
        # Move "Kiwifruit" label further to the left.
        if labels[i] == "Kiwifruit":
            labelX -= 0.09  # Adjust this offset as needed.
        
        percentage = slicePercentage * 100.0
        labelString = f"{labels[i]} ({percentage:.1f}%)"
        glColor3f(1.0, 1.0, 1.0)
        drawBitmapText(labelX, labelY, labelString)
        
        # Draw a radial line (white) from the center to the circumference for this slice boundary.
        glColor3f(1.0, 1.0, 1.0)
        glBegin(GL_LINES)
        rad = currentAngle * PI / 180.0
        glVertex2f(centerX, centerY)
        glVertex2f(centerX + math.cos(rad) * radius, centerY + math.sin(rad) * radius)
        glEnd()
        
        # Update the current angle for the next slice.
        currentAngle += sliceAngle

    # Draw the final radial boundary.
    glColor3f(1.0, 1.0, 1.0)
    glBegin(GL_LINES)
    rad = currentAngle * PI / 180.0
    glVertex2f(centerX, centerY)
    glVertex2f(centerX + math.cos(rad) * radius, centerY + math.sin(rad) * radius)
    glEnd()
    
    # Draw the overall circle outline in white.
    glColor3f(1.0, 1.0, 1.0)
    glBegin(GL_LINE_LOOP)
    for angle in range(0, 360, 1):
        rad = angle * PI / 180.0
        glVertex2f(centerX + math.cos(rad) * radius, centerY + math.sin(rad) * radius)
    glEnd()
    
    # Draw the chart title.
    drawBitmapText(-0.3, 0.8, chartTitle)
    
    glutSwapBuffers()

# ---------------------------------------------------------------------
#  Reshape callback: sets up an orthographic projection.
# ---------------------------------------------------------------------
def reshape(w, h):
    glViewport(0, 0, w, h)
    glMatrixMode(GL_PROJECTION)
    glLoadIdentity()
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0)
    glMatrixMode(GL_MODELVIEW)
    glLoadIdentity()

# ---------------------------------------------------------------------
#  Main entry point.
# ---------------------------------------------------------------------
def main():
    glutInit(sys.argv)
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB)
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT)
    glutInitWindowPosition(100, 100)
    glutCreateWindow(b"Fruit Consumption Pie Chart")
    
    # Set the background color to black.
    glClearColor(0.0, 0.0, 0.0, 1.0)
    
    glutDisplayFunc(display)
    glutReshapeFunc(reshape)
    glutMainLoop()

if __name__ == '__main__':
    main()
