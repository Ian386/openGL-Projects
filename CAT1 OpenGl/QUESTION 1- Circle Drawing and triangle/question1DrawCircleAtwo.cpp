// NAME: IAN NDOLO MWAU
// REG NO: SCT211-0034/2022

// QUESTION 1- Circle Drawing and triangle

/*(ii) write an OpenGL program to rotate the Circle 60 degrees clockwise and fill it with red shade(hint RGB #hex ff0000)*/

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

    // Window dimensions
    const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;

// Simple 2D point structure
struct Point
{
    float x;
    float y;
};

// Draw coordinate axes with tick marks (in cm)
void drawAxes()
{
    glColor3f(1.0f, 1.0f, 1.0f); // White color
    glBegin(GL_LINES);
    // x-axis from -5 to 10
    glVertex2f(-5.0f, 0.0f);
    glVertex2f(10.0f, 0.0f);
    // y-axis from -5 to 10
    glVertex2f(0.0f, -5.0f);
    glVertex2f(0.0f, 10.0f);
    glEnd();

    // Tick marks for x-axis
    glBegin(GL_LINES);
    for (int i = -5; i <= 10; i++)
    {
        glVertex2f(i, -0.2f);
        glVertex2f(i, 0.2f);
    }
    glEnd();

    // Tick marks for y-axis
    glBegin(GL_LINES);
    for (int i = -5; i <= 10; i++)
    {
        glVertex2f(-0.2f, i);
        glVertex2f(0.2f, i);
    }
    glEnd();
}

// Plot the eight symmetric points for the circle boundary
void plotCirclePoints(int cx, int cy, int x, int y, std::vector<float> &vertices)
{
    vertices.push_back(cx + x);
    vertices.push_back(cy + y);
    vertices.push_back(cx - x);
    vertices.push_back(cy + y);
    vertices.push_back(cx + x);
    vertices.push_back(cy - y);
    vertices.push_back(cx - x);
    vertices.push_back(cy - y);
    vertices.push_back(cx + y);
    vertices.push_back(cy + x);
    vertices.push_back(cx - y);
    vertices.push_back(cy + x);
    vertices.push_back(cx + y);
    vertices.push_back(cy - x);
    vertices.push_back(cx - y);
    vertices.push_back(cy - x);
}

// Bresenham Circle Drawing Algorithm (integer arithmetic)
// This function fills the provided vector with the circle boundary points (in cm)
void drawBresenhamCircle(int cx, int cy, int radius, std::vector<float> &vertices)
{
    int x = 0;
    int y = radius;
    int d = 3 - (2 * radius);
    while (x <= y)
    {
        plotCirclePoints(cx, cy, x, y, vertices);
        if (d < 0)
        {
            d += (4 * x) + 6;
        }
        else
        {
            d += (4 * (x - y)) + 10;
            y--;
        }
        x++;
    }
}

// Render the scene: draw axes and a filled, rotated circle
void renderScene()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw coordinate axes
    drawAxes();

    // Generate the circle boundary vertices using Bresenham's algorithm.
    std::vector<float> circleVertices;
    int cx = 1;     // Center x in cm
    int cy = 1;     // Center y in cm
    int radius = 4; // Radius in cm
    drawBresenhamCircle(cx, cy, radius, circleVertices);

    // Convert the flat list into a vector of Point structures.
    std::vector<Point> points;
    for (size_t i = 0; i < circleVertices.size(); i += 2)
    {
        points.push_back({circleVertices[i], circleVertices[i + 1]});
    }

    // Sort the boundary points by angle relative to the circle’s center (1,1)
    std::sort(points.begin(), points.end(), [cx, cy](const Point &a, const Point &b)
              {
        float angleA = std::atan2(a.y - cy, a.x - cx);
        float angleB = std::atan2(b.y - cy, b.x - cx);
        return angleA < angleB; });

    // Apply rotation transformation:
    // 1. Translate the coordinate system so that the circle's center is at the origin.
    // 2. Rotate by -60° (clockwise).
    // 3. Translate back.
    glPushMatrix();
    glTranslatef(cx, cy, 0.0f);
    glRotatef(-60.0f, 0.0f, 0.0f, 1.0f); // Negative angle for clockwise rotation.
    glTranslatef(-cx, -cy, 0.0f);

    // Fill the circle using a triangle fan.
    // Set the fill color to red (hex #ff0000)
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_TRIANGLE_FAN);
    // Center of the fan
    glVertex2f(cx, cy);
    // Boundary vertices in order
    for (const auto &p : points)
    {
        glVertex2f(p.x, p.y);
    }
    // Ensure closure by repeating the first vertex.
    if (!points.empty())
        glVertex2f(points[0].x, points[0].y);
    glEnd();
    glPopMatrix();
}

// Main function
int main()
{
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Circle Rotated & Filled", nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    // Set up viewport and an orthographic projection (coordinate units in cm)
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-5.0, 10.0, -5.0, 10.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black background

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        renderScene();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
