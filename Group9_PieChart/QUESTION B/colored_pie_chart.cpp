#include <GL/glut.h>
#include <cmath>
#include <cstdio>
#include <string>

// Window dimensions
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;

// Pie chart data values and labels
static float values[] = {36.0f, 41.0f, 19.0f, 28.0f, 30.0f, 16.0f};
static const char *labels[] = {"Avocado", "Orange", "Banana", "Kiwifruit", "Mangos", "Grapes"};

// RGB color values for each fruit slice
static float colors[][3] = {
    {0.82f, 0.81f, 0.41f},  // Avocado
    {0.93f, 0.55f, 0.14f},  // Orange
    {1.0f, 0.87f, 0.35f},   // Banana
    {0.43f, 0.51f, 0.04f},  // Kiwifruit
    {1.0f, 0.51f, 0.26f},   // Mango
    {0.44f, 0.18f, 0.66f}   // Grapes
};

// Number of slices
static const int NUM_SLICES = sizeof(values) / sizeof(values[0]);

// Chart title
static const char *chartTitle = "Youth Fruit Preferences in Gachororo";

// Pi constant for angle conversion
const float PI = 3.14159265f;

// Utility function to draw text on screen using bitmap fonts
void drawBitmapText(float x, float y, const char *string) {
    glRasterPos2f(x, y); // Set text position
    for (const char *c = string; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c); // Render each character
    }
}

// Main display callback
void display() {
    glClear(GL_COLOR_BUFFER_BIT);  // Clear background
    glLoadIdentity();

    // Calculate total value for percentage calculations
    float total = 0.0f;
    for (int i = 0; i < NUM_SLICES; ++i) {
        total += values[i];
    }

    // Pie chart parameters
    float centerX = 0.0f, centerY = 0.0f, radius = 0.6f;
    float currentAngle = 0.0f;

    // Draw chart title
    glColor3f(0.0f, 0.0f, 0.0f);
    drawBitmapText(-0.4f, 0.9f, chartTitle);

    // Draw each pie slice
    for (int i = 0; i < NUM_SLICES; ++i) {
        float sliceAngle = 360.0f * values[i] / total;  // Convert value to angle in degrees

        // Draw filled colored slice using triangle fan
        glColor3f(colors[i][0], colors[i][1], colors[i][2]);
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(centerX, centerY); // Pie center
        for (float angle = currentAngle; angle <= currentAngle + sliceAngle; angle += 1.0f) {
            float rad = angle * PI / 180.0f;
            glVertex2f(centerX + cos(rad) * radius, centerY + sin(rad) * radius);
        }
        // Close the fan
        float endRad = (currentAngle + sliceAngle) * PI / 180.0f;
        glVertex2f(centerX + cos(endRad) * radius, centerY + sin(endRad) * radius);
        glEnd();

        // Optional: draw black outline around each slice
        glColor3f(0.0f, 0.0f, 0.0f);
        glLineWidth(1.0f);
        glBegin(GL_LINE_LOOP);
        glVertex2f(centerX, centerY);
        for (float angle = currentAngle; angle <= currentAngle + sliceAngle; angle += 1.0f) {
            float rad = angle * PI / 180.0f;
            glVertex2f(centerX + cos(rad) * radius, centerY + sin(rad) * radius);
        }
        glEnd();

        // Compute label position at the middle angle of the slice
        float midAngle = currentAngle + sliceAngle / 2.0f;
        float midRad = midAngle * PI / 180.0f;
        float labelRadius = radius + 0.15f;

        // Fine-tune label positions for better spacing
        if (i == 2) labelRadius += 0.1f;  // Banana
        if (i == 3) labelRadius += 0.05f; // Kiwifruit

        float labelX = centerX + cos(midRad) * labelRadius;
        float labelY = centerY + sin(midRad) * labelRadius;

        // Format label with percentage
        char labelText[50];
        sprintf(labelText, "%s (%.1f%%)", labels[i], 100.0f * values[i] / total);

        // Draw label
        glColor3f(0.0f, 0.0f, 0.0f);
        drawBitmapText(labelX - 0.05f, labelY - 0.02f, labelText);

        // Update angle for next slice
        currentAngle += sliceAngle;
    }

    glutSwapBuffers();  // Swap buffers for smooth double buffering
}

// Reshape callback to handle window resizing
void reshape(int w, int h) {
    glViewport(0, 0, w, h);              // Reset viewport
    glMatrixMode(GL_PROJECTION);        // Switch to projection matrix
    glLoadIdentity();                   
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);    // Set 2D orthographic projection
    glMatrixMode(GL_MODELVIEW);         // Switch back to modelview matrix
}

// Program entry point
int main(int argc, char **argv) {
    glutInit(&argc, argv);                                      // Initialize GLUT
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);                // Double buffer and RGB color
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);            // Set window size
    glutCreateWindow("Fruit Preferences Pie Chart");            // Create window
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);                        // Set white background
    glutDisplayFunc(display);                                   // Register display callback
    glutReshapeFunc(reshape);                                   // Register reshape callback
    glutMainLoop();                                             // Enter main event loop
    return 0;
}
