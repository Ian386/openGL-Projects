// NAME: IAN NDOLO MWAU
// REG NO: SCT211-0034/2022


// QUESTION 1- Circle Drawing and triangle.

/* Using OpenGL, Write a program to draw a triangle with vertices (-1,6 ; 2,0; -4,9)                        i) Draw a Circle round the triangle (Circumscribed)-the circle should be in red color. */

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

// Window dimensions
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;

// Structure for 2D points
struct Point2D
{
    float x;
    float y;
};

// Compute circumcenter of triangle ABC
Point2D computeCircumcenter(Point2D A, Point2D B, Point2D C)
{
    float d = 2 * (A.x * (B.y - C.y) + B.x * (C.y - A.y) + C.x * (A.y - B.y));
    Point2D center;
    center.x = ((A.x * A.x + A.y * A.y) * (B.y - C.y) +
                (B.x * B.x + B.y * B.y) * (C.y - A.y) +
                (C.x * C.x + C.y * C.y) * (A.y - B.y)) /
               d;
    center.y = ((A.x * A.x + A.y * A.y) * (C.x - B.x) +
                (B.x * B.x + B.y * B.y) * (A.x - C.x) +
                (C.x * C.x + C.y * C.y) * (B.x - A.x)) /
               d;
    return center;
}

// Compute Euclidean distance between two points
float distance(Point2D p1, Point2D p2)
{
    return std::sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
}

// Draw the triangle (white outline)
void drawTriangle(Point2D A, Point2D B, Point2D C)
{
    glColor3f(1.0f, 1.0f, 1.0f); // White color
    glBegin(GL_LINE_LOOP);
    glVertex2f(A.x, A.y);
    glVertex2f(B.x, B.y);
    glVertex2f(C.x, C.y);
    glEnd();
}

// Draw a circle (using a line loop) centered at 'center' with the given radius.
// The circle is drawn in red (RGB #ff0000).
void drawCircle(Point2D center, float radius)
{
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_LINE_LOOP);
    const int segments = 100;
    for (int i = 0; i < segments; i++)
    {
        float theta = 2.0f * 3.1415926f * float(i) / float(segments);
        float x = radius * std::cos(theta);
        float y = radius * std::sin(theta);
        glVertex2f(center.x + x, center.y + y);
    }
    glEnd();
}

// Draw a Cartesian plane covering the full visible area.
// Here, we draw the x-axis and y-axis along with tick marks (every 5 units).
void drawCartesianPlane(float xMin, float xMax, float yMin, float yMax)
{
    // Draw axes in a light gray color
    glColor3f(0.7f, 0.7f, 0.7f);
    glBegin(GL_LINES);
    // x-axis (if y=0 is in range)
    if (yMin <= 0.0f && yMax >= 0.0f)
    {
        glVertex2f(xMin, 0.0f);
        glVertex2f(xMax, 0.0f);
    }
    // y-axis (if x=0 is in range)
    if (xMin <= 0.0f && xMax >= 0.0f)
    {
        glVertex2f(0.0f, yMin);
        glVertex2f(0.0f, yMax);
    }
    glEnd();

    // Draw tick marks on the x-axis (every 5 units)
    glBegin(GL_LINES);
    for (float x = std::ceil(xMin / 5.0f) * 5.0f; x <= xMax; x += 5.0f)
    {
        if (std::fabs(x) < 1e-6)
            continue; // skip origin
        glVertex2f(x, -0.5f);
        glVertex2f(x, 0.5f);
    }
    glEnd();

    // Draw tick marks on the y-axis (every 5 units)
    glBegin(GL_LINES);
    for (float y = std::ceil(yMin / 5.0f) * 5.0f; y <= yMax; y += 5.0f)
    {
        if (std::fabs(y) < 1e-6)
            continue; // skip origin
        glVertex2f(-0.5f, y);
        glVertex2f(0.5f, y);
    }
    glEnd();
}

// Render the scene: draw the Cartesian plane, triangle, and circumscribed circle.
void renderScene(Point2D A, Point2D B, Point2D C, Point2D circumcenter, float radius)
{
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw the Cartesian plane covering the full view.
    // Our orthographic projection is set from x = -32 to 3 and y = -22 to 13.
    drawCartesianPlane(-32.0f, 3.0f, -22.0f, 13.0f);

    // Draw the triangle (white)
    drawTriangle(A, B, C);

    // Draw the circumscribed circle (red)
    drawCircle(circumcenter, radius);
}

int main()
{
    // Initialize GLFW
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Create a windowed mode window and its OpenGL context
    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Triangle and Circumscribed Circle with 1:5 Scale", nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    // Define the triangle vertices (in our chosen coordinate system)
    Point2D A = {-1.0f, 6.0f};
    Point2D B = {2.0f, 0.0f};
    Point2D C = {-4.0f, 9.0f};

    // Compute the circumcenter and radius for the triangle
    Point2D circumcenter = computeCircumcenter(A, B, C);
    float radius = distance(circumcenter, A); // (A, B, C lie on the circle)

    // Set up viewport and orthographic projection.
    // The circumscribed circle (centered at about (-14.5, -4.5) with radius ~17.1)
    // extends roughly from x = -32 to 3 and y = -22 to 13.
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-32.0, 3.0, -22.0, 13.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Set the background color to black
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        renderScene(A, B, C, circumcenter, radius);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
