/*
    Fruit Preferences Pie Chart - OpenGL GLUT

    This program renders a 2D pie chart using OpenGL (GLUT) to visualize 
    youth fruit preferences in Gachororo. Each slice represents a different 
    fruit with a corresponding percentage based on predefined data. 
    Labels and colors are included for clarity.

    Features:
    - Dynamic pie slice rendering with color differentiation
    - Labeling with percentages and fruit names
    - Title display

    Author: Group 9
*/
#include <GL/glut.h>
#include <cmath>
#include <cstdio>
#include <string>

// Window size constants
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;

// Data values and labels for each fruit
static float values[] = {36.0f, 41.0f, 19.0f, 28.0f, 30.0f, 16.0f};
static const char *labels[] = {"Avocado", "Orange", "Banana", "Kiwifruit", "Mangos", "Grapes"};

// RGB colors for each pie slice
static float colors[][3] = {
    {0.82f, 0.81f, 0.41f},
    {0.93f, 0.55f, 0.14f},
    {1.0f, 0.87f, 0.35f},
    {0.43f, 0.51f, 0.04f},
    {1.0f, 0.51f, 0.26f},
    {0.44f, 0.18f, 0.66f}
};

// Total number of slices
static const int NUM_SLICES = sizeof(values) / sizeof(values[0]);

// Chart title
static const char *chartTitle = "Youth Fruit Preferences in Gachororo";

// Pi constant
const float PI = 3.14159265f;

// Renders text at a given position
void drawBitmapText(float x, float y, const char *string) {
    glRasterPos2f(x, y);
    for (const char *c = string; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
    }
}

// Draws the full pie chart
void display() {
    glClear(GL_COLOR_BUFFER_BIT);   // Clear the window
    glLoadIdentity();               // Reset transformations

    float total = 0.0f;
    for (int i = 0; i < NUM_SLICES; ++i) {
        total += values[i];         // Sum up the values
    }

    float centerX = 0.0f, centerY = 0.0f, radius = 0.6f;
    float currentAngle = 0.0f;

    // Draw chart title at the top
    glColor3f(0.0f, 0.0f, 0.0f);
    drawBitmapText(-0.4f, 0.9f, chartTitle);

    // Draw each slice of the pie
    for (int i = 0; i < NUM_SLICES; ++i) {
        float sliceAngle = 360.0f * values[i] / total;

        // Set fill color and draw slice as a triangle fan
        glColor3f(colors[i][0], colors[i][1], colors[i][2]);
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(centerX, centerY);  // Center of the pie
        for (float angle = currentAngle; angle <= currentAngle + sliceAngle; angle += 1.0f) {
            float rad = angle * PI / 180.0f;
            glVertex2f(centerX + cos(rad) * radius, centerY + sin(rad) * radius);
        }
        float endRad = (currentAngle + sliceAngle) * PI / 180.0f;
        glVertex2f(centerX + cos(endRad) * radius, centerY + sin(endRad) * radius);
        glEnd();

        // Draw slice outline
        glColor3f(0.0f, 0.0f, 0.0f);
        glLineWidth(1.0f);
        glBegin(GL_LINE_LOOP);
        glVertex2f(centerX, centerY);
        for (float angle = currentAngle; angle <= currentAngle + sliceAngle; angle += 1.0f) {
            float rad = angle * PI / 180.0f;
            glVertex2f(centerX + cos(rad) * radius, centerY + sin(rad) * radius);
        }
        glEnd();

        // Position the label around the middle of the slice
        float midAngle = currentAngle + sliceAngle / 2.0f;
        float midRad = midAngle * PI / 180.0f;
        float labelRadius = radius + 0.15f;

        if (i == 2) labelRadius += 0.1f;   // Slight offset for Banana
        if (i == 3) labelRadius += 0.05f;  // Slight offset for Kiwifruit

        float labelX = centerX + cos(midRad) * labelRadius;
        float labelY = centerY + sin(midRad) * labelRadius;

        // Format label with percentage
        char labelText[50];
        sprintf(labelText, "%s (%.1f%%)", labels[i], 100.0f * values[i] / total);

        // Draw label
        glColor3f(0.0f, 0.0f, 0.0f);
        drawBitmapText(labelX - 0.05f, labelY - 0.02f, labelText);

        currentAngle += sliceAngle;  // Move to next slice
    }

    glutSwapBuffers();  // Swap buffers to display
}

// Adjusts viewport and coordinate system when window is resized
void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);  // Set 2D projection
    glMatrixMode(GL_MODELVIEW);
}

// Main program entry
int main(int argc, char **argv) {
    glutInit(&argc, argv);                                     // Init GLUT
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);               // Double buffering, RGB mode
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);           // Set window size
    glutCreateWindow("Fruit Preferences Pie Chart");           // Create window
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);                       // Set background to white
    glutDisplayFunc(display);                                  // Register draw callback
    glutReshapeFunc(reshape);                                  // Register resize callback
    glutMainLoop();                                            // Enter main loop
    return 0;
}
