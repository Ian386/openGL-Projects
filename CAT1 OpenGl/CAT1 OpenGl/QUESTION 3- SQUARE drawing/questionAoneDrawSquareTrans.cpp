// NAME: IAN NDOLO MWAU
// REG NO: SCT211-0034/2022

//QUESTION 3- SQUARE drawing
/*a)Using OpenGL Draw a square with coordinate points A(0, 4), B(4, 4),C(4, 0), D(0, 0).
(i) Apply the translation with distance 2 towards X axis
and 2 towards Y axis. Obtain the new coordinates of the
square.
(ii) write an OpenGL program to rotate the translated
figure with rotation angle = θ = 55º*/

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

// Window dimensions
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;

// Define a simple 2D point structure
struct Point
{
    float x;
    float y;
};

// Global constants for translation and rotation
const float TRANSLATE_X = 2.0f;
const float TRANSLATE_Y = 2.0f;
const float ROTATION_ANGLE_DEG = 55.0f;
const float PI = 3.14159265358979323846f;

// Original square vertices: A(0,4), B(4,4), C(4,0), D(0,0)
Point originalSquare[4] = {
    {0.0f, 4.0f},
    {4.0f, 4.0f},
    {4.0f, 0.0f},
    {0.0f, 0.0f}};

// Arrays to hold the translated and rotated square vertices
Point translatedSquare[4];
Point rotatedSquare[4];

// Function to compute the translated square vertices
void computeTranslatedSquare()
{
    for (int i = 0; i < 4; i++)
    {
        translatedSquare[i].x = originalSquare[i].x + TRANSLATE_X;
        translatedSquare[i].y = originalSquare[i].y + TRANSLATE_Y;
    }
}

// Function to rotate a point 'p' about a given 'center' by angle (in degrees)
Point rotatePoint(Point p, Point center, float angleDeg)
{
    float angleRad = angleDeg * PI / 180.0f;
    float cosA = cos(angleRad);
    float sinA = sin(angleRad);
    Point result;
    result.x = center.x + (p.x - center.x) * cosA - (p.y - center.y) * sinA;
    result.y = center.y + (p.x - center.x) * sinA + (p.y - center.y) * cosA;
    return result;
}

// Function to compute the rotated square vertices from the translated square.
// We rotate about the center of the translated square, which is (4,4).
void computeRotatedSquare()
{
    // Center of the translated square is the average of its vertices.
    // For our square A'(2,6), B'(6,6), C'(6,2), D'(2,2), the center is (4,4).
    Point center = {4.0f, 4.0f};
    for (int i = 0; i < 4; i++)
    {
        rotatedSquare[i] = rotatePoint(translatedSquare[i], center, ROTATION_ANGLE_DEG);
    }
}

// Function to draw Cartesian axes with tick marks
void drawAxes()
{
    // Draw axes in white
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINES);
    // X-axis from -1 to 10
    glVertex2f(-1.0f, 0.0f);
    glVertex2f(10.0f, 0.0f);
    // Y-axis from -1 to 10
    glVertex2f(0.0f, -1.0f);
    glVertex2f(0.0f, 10.0f);
    glEnd();

    // Draw tick marks (every 1 unit)
    glBegin(GL_LINES);
    for (int i = -1; i <= 10; i++)
    {
        // X-axis ticks
        glVertex2f(i, -0.1f);
        glVertex2f(i, 0.1f);
        // Y-axis ticks
        glVertex2f(-0.1f, i);
        glVertex2f(0.1f, i);
    }
    glEnd();
}

// Function to draw a square (given an array of 4 vertices) in a specified color
void drawSquare(Point square[4], float r, float g, float b)
{
    glColor3f(r, g, b);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 4; i++)
    {
        glVertex2f(square[i].x, square[i].y);
    }
    glEnd();
}

// Render the scene: draw axes, then the original, translated, and rotated squares
void renderScene()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw Cartesian axes
    drawAxes();

    // Draw original square in white
    drawSquare(originalSquare, 1.0f, 1.0f, 1.0f);

    // Draw translated square in red
    drawSquare(translatedSquare, 1.0f, 0.0f, 0.0f);

    // Draw rotated square in blue
    drawSquare(rotatedSquare, 0.0f, 0.0f, 1.0f);
}

int main()
{
    // Compute the translated and rotated square coordinates
    computeTranslatedSquare();
    computeRotatedSquare();

    // Print the new coordinates to the console
    std::cout << "Translated Square Coordinates:" << std::endl;
    std::cout << "A' = (" << translatedSquare[0].x << ", " << translatedSquare[0].y << ")" << std::endl;
    std::cout << "B' = (" << translatedSquare[1].x << ", " << translatedSquare[1].y << ")" << std::endl;
    std::cout << "C' = (" << translatedSquare[2].x << ", " << translatedSquare[2].y << ")" << std::endl;
    std::cout << "D' = (" << translatedSquare[3].x << ", " << translatedSquare[3].y << ")" << std::endl;

    std::cout << "\nRotated Square Coordinates (Rotation angle = " << ROTATION_ANGLE_DEG << " degrees about (4,4)):" << std::endl;
    std::cout << "A'' = (" << rotatedSquare[0].x << ", " << rotatedSquare[0].y << ")" << std::endl;
    std::cout << "B'' = (" << rotatedSquare[1].x << ", " << rotatedSquare[1].y << ")" << std::endl;
    std::cout << "C'' = (" << rotatedSquare[2].x << ", " << rotatedSquare[2].y << ")" << std::endl;
    std::cout << "D'' = (" << rotatedSquare[3].x << ", " << rotatedSquare[3].y << ")" << std::endl;

    // Initialize GLFW
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Create a window and its OpenGL context
    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Translated and Rotated Square", nullptr, nullptr);
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

    // Set up viewport and orthographic projection
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Set coordinate system: from -1 to 10 for both X and Y
    gluOrtho2D(-1.0, 10.0, -1.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Set the background color to black
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

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
