// NAME: IAN NDOLO MWAU
// REG NO: SCT211-0034/2022

//QUESTION 4- Polygon drawing

/*Using OpenGL draw a filled polygon with the following dimensions
(8,4;2,4;0,8;3,12;7,12;10,8) hint (GL POLYGON function) might be
useful
(i). Write a function in OpenGL to fill the polygon above
in Red color
(#FF0000.)
(ii). Write OpenGL program to scale up (scaling) the polygon by
a factor of 2
(ii) Write a procedure in OpenGL to fill the interior of the given
polygon above with shades of green asterisks*/

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

// Window dimensions
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;

// Structure to represent a 2D point
struct Point
{
    float x;
    float y;
};

// Number of vertices in the polygon
const int NUM_VERTICES = 6;

// Original polygon vertices (given order):
// A(8,4); B(2,4); C(0,8); D(3,12); E(7,12); F(10,8)
Point originalPoly[NUM_VERTICES] = {
    {8.0f, 4.0f},
    {2.0f, 4.0f},
    {0.0f, 8.0f},
    {3.0f, 12.0f},
    {7.0f, 12.0f},
    {10.0f, 8.0f}};

// Array to hold the scaled polygon vertices
Point scaledPoly[NUM_VERTICES];

// Scale factor for the polygon (given: 2)
const float SCALE_FACTOR = 2.0f;

// Function to scale a polygon by a given factor.
// 'original' is an array of vertices; the result is stored in 'scaled'.
void scalePolygon(const Point original[], Point scaled[], int numVertices, float factor)
{
    for (int i = 0; i < numVertices; i++)
    {
        scaled[i].x = original[i].x * factor;
        scaled[i].y = original[i].y * factor;
    }
}

// Function to check if a point (x,y) is inside the polygon using the ray-casting algorithm.
bool isPointInPolygon(const Point poly[], int numVertices, float x, float y)
{
    bool inside = false;
    for (int i = 0, j = numVertices - 1; i < numVertices; j = i++)
    {
        if (((poly[i].y > y) != (poly[j].y > y)) &&
            (x < (poly[j].x - poly[i].x) * (y - poly[i].y) / (poly[j].y - poly[i].y) + poly[i].x))
            inside = !inside;
    }
    return inside;
}

// Function to draw Cartesian axes with tick marks.
void drawAxes()
{
    // Draw axes in white.
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINES);
    // X-axis from -5 to 25.
    glVertex2f(-5.0f, 0.0f);
    glVertex2f(25.0f, 0.0f);
    // Y-axis from -5 to 30.
    glVertex2f(0.0f, -5.0f);
    glVertex2f(0.0f, 30.0f);
    glEnd();

    // Draw tick marks (every 1 unit)
    glBegin(GL_LINES);
    for (int i = -5; i <= 25; i++)
    {
        // X-axis ticks.
        glVertex2f(i, -0.2f);
        glVertex2f(i, 0.2f);
    }
    for (int i = -5; i <= 30; i++)
    {
        // Y-axis ticks.
        glVertex2f(-0.2f, i);
        glVertex2f(0.2f, i);
    }
    glEnd();
}

// Function to draw a filled polygon given an array of vertices.
// The polygon is filled using the GL_POLYGON function.
void drawFilledPolygon(const Point poly[], int numVertices, float r, float g, float b)
{
    glColor3f(r, g, b);
    glBegin(GL_POLYGON);
    for (int i = 0; i < numVertices; i++)
    {
        glVertex2f(poly[i].x, poly[i].y);
    }
    glEnd();
}

// Function to draw the outline of a polygon given an array of vertices.
void drawPolygonOutline(const Point poly[], int numVertices, float r, float g, float b)
{
    glColor3f(r, g, b);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < numVertices; i++)
    {
        glVertex2f(poly[i].x, poly[i].y);
    }
    glEnd();
}

// Function to draw an asterisk (a small plus sign) centered at (x,y) with a given size.
void drawAsterisk(float x, float y, float size)
{
    glBegin(GL_LINES);
    // Horizontal line.
    glVertex2f(x - size, y);
    glVertex2f(x + size, y);
    // Vertical line.
    glVertex2f(x, y - size);
    glVertex2f(x, y + size);
    glEnd();
}

// Function to fill the interior of a polygon with green asterisks.
// This function iterates over the polygon's bounding box using a grid,
// tests whether each grid point is inside the polygon using the ray-casting algorithm,
// and if so, draws a green asterisk at that point.
void fillPolygonWithGreenAsterisks(const Point poly[], int numVertices)
{
    // Compute the bounding box.
    float minX = poly[0].x, maxX = poly[0].x;
    float minY = poly[0].y, maxY = poly[0].y;
    for (int i = 1; i < numVertices; i++)
    {
        if (poly[i].x < minX)
            minX = poly[i].x;
        if (poly[i].x > maxX)
            maxX = poly[i].x;
        if (poly[i].y < minY)
            minY = poly[i].y;
        if (poly[i].y > maxY)
            maxY = poly[i].y;
    }

    float step = 1.0f; // Grid step size.
    for (float x = minX; x <= maxX; x += step)
    {
        for (float y = minY; y <= maxY; y += step)
        {
            if (isPointInPolygon(poly, numVertices, x, y))
            {
                // Draw a green asterisk at (x, y) with a fixed size.
                glColor3f(0.0f, 1.0f, 0.0f);
                drawAsterisk(x, y, 0.2f);
            }
        }
    }
}

// Render the scene: draws the Cartesian plane, the original red-filled polygon,
// and the scaled polygon (outlined in white with its interior filled with green asterisks).
void renderScene()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw Cartesian axes.
    drawAxes();

    // Draw the original polygon filled with red (#FF0000).
    drawFilledPolygon(originalPoly, NUM_VERTICES, 1.0f, 0.0f, 0.0f);

    // Compute the scaled polygon vertices.
    scalePolygon(originalPoly, scaledPoly, NUM_VERTICES, SCALE_FACTOR);

    // Draw the outline of the scaled polygon in white.
    drawPolygonOutline(scaledPoly, NUM_VERTICES, 1.0f, 1.0f, 1.0f);

    // Fill the interior of the scaled polygon with green asterisks.
    fillPolygonWithGreenAsterisks(scaledPoly, NUM_VERTICES);
}

// Main function: sets up the OpenGL context, the orthographic projection,
// and enters the main rendering loop.
int main()
{
    // Initialize GLFW.
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    // Create a windowed mode window and its OpenGL context.
    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Filled Polygon, Scaled & Pattern Filled", nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW.
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW\n";
        return -1;
    }

    // Set up viewport and an orthographic projection.
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Set coordinate system: x from -5 to 25 and y from -5 to 30.
    gluOrtho2D(-5.0, 25.0, -5.0, 30.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Set the background color to black.
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Main rendering loop.
    while (!glfwWindowShouldClose(window))
    {
        renderScene();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
