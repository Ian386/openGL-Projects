#include <GL/glut.h>
#include <iostream>
#include <cmath>
#include <string>

// Function to implement the DDA Line Drawing Algorithm
void drawLineDDA(int x1, int y1, int x2, int y2) {
    float dx = x2 - x1;
    float dy = y2 - y1;
    float steps = std::max(abs(dx), abs(dy)); // Determine the number of steps
    float xInc = dx / steps; // Increment in x for each step
    float yInc = dy / steps; // Increment in y for each step

    float x = x1, y = y1;
    std::cout << "Calculated Points:\n";

    glColor3f(1.0, 1.0, 1.0); // Set line color to white
    glPointSize(5.0); // Enlarge the plotted points
    glBegin(GL_POINTS);
    for (int i = 0; i <= steps; i++) {
        std::cout << "(" << round(x) << ", " << round(y) << ")\n";
        glVertex2i(round(x), round(y)); // Plot the pixel
        x += xInc;
        y += yInc;
    }
    glEnd();

    // Draw a line connecting the points
    glBegin(GL_LINE_STRIP);
    x = x1, y = y1;
    for (int i = 0; i <= steps; i++) {
        glVertex2i(round(x), round(y));
        x += xInc;
        y += yInc;
    }
    glEnd();

    glFlush();
}

// Function to draw text on the screen
void drawText(float x, float y, const std::string& text) {
    glRasterPos2f(x, y);
    for (char c : text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }
}

// Function to draw the Cartesian plane (axes) with numbered ticks
void drawCartesianPlane() {
    glColor3f(0.0, 1.0, 0.0); // Set axis color to green

    // Draw X and Y axes
    glBegin(GL_LINES);
    // X-axis
    glVertex2i(-20, 0);
    glVertex2i(20, 0);

    // Y-axis
    glVertex2i(0, -20);
    glVertex2i(0, 20);
    glEnd();

    // Draw ticks and numbers on the X-axis
    for (int i = -20; i <= 20; i++) {
        if (i == 0) continue; // Skip the origin to avoid overlapping text

        // Draw tick marks
        glBegin(GL_LINES);
        glVertex2i(i, -0.5);
        glVertex2i(i, 0.5);
        glEnd();

        // Draw numbers
        if (i % 5 == 0) { // Label every 5 units
            std::string num = std::to_string(i);
            drawText(i - 0.2, -1.5, num);
        }
    }

    // Draw ticks and numbers on the Y-axis
    for (int i = -20; i <= 20; i++) {
        if (i == 0) continue; // Skip the origin to avoid overlapping text

        // Draw tick marks
        glBegin(GL_LINES);
        glVertex2i(-0.5, i);
        glVertex2i(0.5, i);
        glEnd();

        // Draw numbers
        if (i % 5 == 0) { // Label every 5 units
            std::string num = std::to_string(i);
            drawText(-2.0, i - 0.2, num);
        }
    }

    glFlush();
}

// Display function
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    drawCartesianPlane(); // Draw the Cartesian plane
    drawLineDDA(5, 2, 10, 3); // Draw the DDA line

    glFlush();
}

// Initialize OpenGL settings
void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0); // Set background to black
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(-20, 20, -20, 20); // Define coordinate system (Cartesian plane)
}

// Main function
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("DDA Line");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}