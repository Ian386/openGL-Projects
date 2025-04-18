//  GROUP 9 WORK ASSESSMENT

// -----------------------------------------------------------------------------
// The program calculates slice angles proportionally, renders them using triangle fans, and labels each slice with a white percentage and fruit name positioned slightly outside the chart circumference.
// Radial lines divide the chart, and label positions for certain slices (like Banana and Kiwifruit) are adjusted for optimal clarity.
// -----------------------------------------------------------------------------

/*Data

    Ovacado : 36

    Orange : 41

    Banana : 19

    Kiwifruit : 28

    Mangos : 30

    Grapes : 16
*/

#include <GL/glut.h>
#include <cmath>
#include <cstdio>
#include <string>

// Define window width/height
const int WINDOW_WIDTH = 600;
const int WINDOW_HEIGHT = 600;

// Data for the pie chart
static float values[] = {36.0f, 41.0f, 19.0f, 28.0f, 30.0f, 16.0f};
static const char *labels[] = {
    "Ovacado",
    "Orange",
    "Banana",
    "Kiwifruit",
    "Mangos",
    "Grapes"};

// The number of slices in the pie chart
static const int NUM_SLICES = sizeof(values) / sizeof(values[0]);

// The title of the pie chart
static const char *chartTitle = "Youth Fruit Preferences in Gachororo";

// PI constant
const float PI = 3.14159265f;

// -------------------------------------------------------------------------
//  Function to draw text on the screen using a simple bitmap font.
// -------------------------------------------------------------------------

// x, y: position in world coordinates
// string: the text to be drawn
void drawBitmapText(float x, float y, const char *string)
{
    glRasterPos2f(x, y);
    for (const char *c = string; *c != '\0'; c++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
}

// -------------------------------------------------------------------------
//  Display callback
// This function draws the pie chart and its labels.
// -------------------------------------------------------------------------

void display()
{
    // Clear the window with the background color.
    glClear(GL_COLOR_BUFFER_BIT);

    // Compute total value for percentage calculations.
    float total = 0.0f;
    for (int i = 0; i < NUM_SLICES; ++i)
    {
        total += values[i];
    }

    // Pie chart parameters.
    float centerX = 0.0f;
    float centerY = 0.0f;
    float radius = 0.5f;       // Pie radius.
    float currentAngle = 0.0f; // Starting angle in degrees.

    // First, draw the filled slices.
    for (int i = 0; i < NUM_SLICES; ++i)
    {
        // Calculate the slice angle (in degrees).
        float slicePercentage = values[i] / total;
        float sliceAngle = slicePercentage * 360.0f;

        // Fill each slice with black for simplicity.
        glColor3f(0.0f, 0.0f, 0.0f);
        // Draw the pie slice using triangle fan.
        glBegin(GL_TRIANGLE_FAN);
        // Center of the pie.
        glVertex2f(centerX, centerY);
        // Draw the arc.
        for (float angle = currentAngle; angle <= currentAngle + sliceAngle; angle += 1.0f)
        {
            // Convert angle to radians for OpenGL.
            float rad = angle * PI / 180.0f;
            // Calculate vertex position on the circle boundary.
            glVertex2f(centerX + cos(rad) * radius, centerY + sin(rad) * radius);
        }
        // Ensure the last vertex reaches the exact end angle.
        // This is important for the last slice and a workaround to ensure the last vertex reaches the exact end angle.
        float endRad = (currentAngle + sliceAngle) * PI / 180.0f;
        glVertex2f(centerX + cos(endRad) * radius, centerY + sin(endRad) * radius);
        glEnd();

        // Compute the midpoint angle for placing the label.
        float midAngle = currentAngle + sliceAngle / 2.0f;
        // Convert to radians for label positioning.
        float midRad = midAngle * PI / 180.0f;
        // Calculate label position based on the midpoint angle.
        float labelRadius = radius + 0.1f;
        // Increase label distance for Banana slice due to its size.
        if (std::string(labels[i]) == "Banana")
        {
            labelRadius = radius + 0.42f;
        }
        // Increase label distance for Kiwifruit slice
        if (std::string(labels[i]) == "Kiwifruit")
        {
            labelRadius = radius + 0.20f;
        }
        float labelX = centerX + cos(midRad) * labelRadius;
        float labelY = centerY + sin(midRad) * labelRadius;

        // Adjust KiwiFruit label position further to the left.
        if (std::string(labels[i]) == "Kiwifruit")
        {
            labelX -= 0.09f; // Change the value as needed to move it further left.
        }

        // Build and draw the label string.
        char labelString[64];
        // Format the label string to include the percentage.
        float percentage = slicePercentage * 100.0f;
        // Use snprintf to format the string safely.
        std::snprintf(labelString, sizeof(labelString), "%s (%.1f%%)", labels[i], percentage);
        // Draw the label string at the calculated position.
        glColor3f(1.0f, 1.0f, 1.0f); // White text.
        drawBitmapText(labelX, labelY, labelString);

        // After filling this slice, draw the radial line (boundary) at the starting angle.
        glColor3f(1.0f, 1.0f, 1.0f); // White line.
        glBegin(GL_LINES);
        float rad = currentAngle * PI / 180.0f;
        // Draw the line from the center to the edge of the pie slice.
        glVertex2f(centerX, centerY);
        // Calculate the end point of the line at the edge of the pie slice.
        glVertex2f(centerX + cos(rad) * radius, centerY + sin(rad) * radius);
        glEnd();

        // Update the current angle for the next slice.
        currentAngle += sliceAngle;
    }

    // Draw the final radial boundary at 360 degrees (or currentAngle).
    // This is to ensure the last slice has a boundary line.
    glColor3f(1.0f, 1.0f, 1.0f);
    // Draw the line from the center to the edge of the pie slice.
    glBegin(GL_LINES);
    // Use the final angle to draw the last line.
    float finalRad = currentAngle * PI / 180.0f;
    glVertex2f(centerX, centerY);
    glVertex2f(centerX + cos(finalRad) * radius, centerY + sin(finalRad) * radius);
    glEnd();

    // Draw the overall circle outline (white).
    glColor3f(1.0f, 1.0f, 1.0f);
    // Draw the outer circle boundary for the pie chart using a line loop.
    glBegin(GL_LINE_LOOP);
    for (int angle = 0; angle < 360; angle += 1)
    {
        // Convert angle to radians for OpenGL.
        float rad = angle * PI / 180.0f;
        // Calculate vertex position on the circle boundary.
        glVertex2f(centerX + cos(rad) * radius, centerY + sin(rad) * radius);
    }
    glEnd();

    // Draw the chart title at the top.
    drawBitmapText(-0.3f, 0.8f, chartTitle);

    // Flush the OpenGL commands and swap buffers.
    glutSwapBuffers();
}

// -------------------------------------------------------------------------
//  Reshape callback: maintain an orthographic projection.
// -------------------------------------------------------------------------
void reshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Set coordinates from -1 to 1 in both axes.
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// -------------------------------------------------------------------------
//  Main entry point.
// -------------------------------------------------------------------------
int main(int argc, char **argv)
{
    // Initialize GLUT and create a window.
    glutInit(&argc, argv);
    // Initialize GLUT with double buffering and RGB color mode.
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    // Set the initial window size and position.
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Fruit Consumption Pie Chart");

    // Set the background color to black.
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Register callbacks.
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glutMainLoop();
    return 0;
}
