/*
Question: How would you convert the chart background to gray scale
using OpenGL code? demonstrate how this can be achieved

Answer:
To convert the chart background to grayscale using OpenGL, we can apply the luminance preservation method.
This method calculates the grayscale value based on the RGB values of the original colors,
ensuring that the resulting grayscale image maintains the perceived brightness of the original colors.
This is particularly important for visualizations where color differentiation is key, such as in pie charts.

1. Color Space Conversion Theory
We used the luminance preservation method to convert RGB colors to grayscale. This standard formula accounts for human eye sensitivity:

Gray = 0.299*R + 0.587*G + 0.114*B
This produces perceptually accurate grayscale values that maintain the original colors' relative brightness relationships.

2. Step-by-Step Conversion Process
For each original fruit color, we:

a. Took the original RGB values
b. Applied the luminance formula
c. Created new RGB triplets where R=G=B=gray_value

Example Conversions:

Fruit	Original Color	Calculation	Grayscale Value
Avocado	(0.34,0.51,0.01)	0.2990.34 + 0.5870.51 + 0.114*0.01=0.40
Orange	(1.0,0.5,0.0)	0.2991.0 + 0.5870.5 + 0.114*0.0=0.59
Banana	(1.0,1.0,0.0)	0.2991.0 + 0.5871.0 + 0.114*0.0=0.89
Kiwifruit	(0.45,0.76,0.23)	0.2990.45 + 0.5870.76 + 0.114*0.23=0.61
Mangos	(1.0,0.8,0.0)	0.2991.0 + 0.5870.8 + 0.114*0.0=0.77
Grapes	(0.5,0.0,0.5)	0.2990.5 + 0.5870.0 + 0.114*0.5=0.21 
*/
#include <GL/glut.h>
#include <cmath>
#include <cstdio>
#include <string>

// Window dimensions
const int WINDOW_WIDTH = 600;
const int WINDOW_HEIGHT = 600;

// Chart data configuration
static float values[] = { 36.0f, 41.0f, 19.0f, 28.0f, 30.0f, 16.0f };
static const char* labels[] = {
    "Avocado",
    "Orange",
    "Banana",
    "Kiwifruit",
    "Mangos",
    "Grapes" };

// Grayscale colors using luminance formula: 0.299*R + 0.587*G + 0.114*B
static float grayscaleColors[][3] = {
    {0.40f, 0.40f, 0.40f},  // Avocado (dark green -> medium gray)
    {0.59f, 0.59f, 0.59f},  // Orange (orange -> light gray)
    {0.89f, 0.89f, 0.89f},  // Banana (yellow -> near white)
    {0.61f, 0.61f, 0.61f},  // Kiwifruit (green -> medium-light gray)
    {0.77f, 0.77f, 0.77f},  // Mangos (orange-yellow -> light gray)
    {0.21f, 0.21f, 0.21f}   // Grapes (purple -> dark gray)
};

const int NUM_SLICES = sizeof(values) / sizeof(values[0]);
const char *chartTitle = "Youth Fruit Preferences in Gachororo";
const float PI = 3.14159265f;

// Text drawing function
void drawBitmapText(float x, float y, const char* string) {
    glRasterPos2f(x, y);
    while (*string) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *string++);
    }
}

// Main rendering function
void display() {
    // Clear window with black background
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Calculate total value for percentage calculations
    float total = 0.0f;
    for (int i = 0; i < NUM_SLICES; ++i) total += values[i];

    // Pie chart configuration
    const float centerX = 0.0f, centerY = 0.0f;
    const float radius = 0.5f;  // Chart radius (relative to [-1,1] coordinate system)
    float currentAngle = 0.0f;   // Starting angle for first slice

    // Draw each pie slice
    for (int i = 0; i < NUM_SLICES; ++i) {
        // Calculate slice dimensions
        const float slicePercentage = values[i] / total;
        const float sliceAngle = slicePercentage * 360.0f;

        // Set grayscale color for this slice
        glColor3fv(grayscaleColors[i]);

        // Draw slice using triangle fan primitive
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(centerX, centerY);  // Center point
        for (float angle = currentAngle; angle <= currentAngle + sliceAngle; angle += 1.0f) {
            const float rad = angle * PI / 180.0f;
            glVertex2f(centerX + cos(rad) * radius, centerY + sin(rad) * radius);
        }
        // Final vertex to ensure complete coverage
        const float endRad = (currentAngle + sliceAngle) * PI / 180.0f;
        glVertex2f(centerX + cos(endRad) * radius, centerY + sin(endRad) * radius);
        glEnd();

        // Label positioning calculations -------------------------------
        const float midAngle = currentAngle + sliceAngle / 2.0f;
        float midRad = midAngle * PI / 180.0f;
        float labelRadius = radius + 0.1f;  // Default label distance

        // Special positioning for crowded sections
        if (std::string(labels[i]) == "Banana") {
            labelRadius = radius + 0.42f;  // Move label further out
        }
        else if (std::string(labels[i]) == "Kiwifruit") {
            labelRadius = radius + 0.20f;
            midRad += 0.05f;  // Angular adjustment for better placement
        }

        // Calculate final label position
        const float labelX = centerX + cos(midRad) * labelRadius;
        const float labelY = centerY + sin(midRad) * labelRadius;

        // Create label text with percentage
        char labelString[64];
        std::snprintf(labelString, sizeof(labelString), "%s (%.1f%%)", labels[i], slicePercentage * 100.0f);

        // Draw white text label
        glColor3f(1.0f, 1.0f, 1.0f);
        drawBitmapText(labelX, labelY, labelString);

        // Draw white separation lines between slices
        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_LINES);
        const float startRad = currentAngle * PI / 180.0f;
        glVertex2f(centerX, centerY);
        glVertex2f(centerX + cos(startRad) * radius, centerY + sin(startRad) * radius);
        glEnd();

        currentAngle += sliceAngle;
    }

    // Draw outer boundary circle
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINE_LOOP);
    for (int angle = 0; angle < 360; ++angle) {
        const float rad = angle * PI / 180.0f;
        glVertex2f(centerX + cos(rad) * radius, centerY + sin(rad) * radius);
    }
    glEnd();

    // Draw chart title in white
    glColor3f(1.0f, 1.0f, 1.0f);
    drawBitmapText(-0.35f, 0.8f, chartTitle);

    glutSwapBuffers();
}

// Window resize handler
void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);  // Maintain [-1,1] coordinate system
    glMatrixMode(GL_MODELVIEW);
}

// Application entry point
int main(int argc, char** argv) {
    // GLUT initialization
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Grayscale Fruit Consumption Chart");

    // Set initial clear color (black background)
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Register callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    // Start main event loop
    glutMainLoop();
    return 0;
}