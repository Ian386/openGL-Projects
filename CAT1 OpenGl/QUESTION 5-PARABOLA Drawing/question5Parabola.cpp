// NAME: IAN NDOLO MWAU
// REG NO: SCT211-0034/2022

//QUESTION 5 - PARABOLA Drawing

/*a) Use the midpoint method and symmetry considerations to scan convert the
parabola
x=y^2 for the interval - 10 ≤ y ≤ 10
Show the working of the method and implement it using OpenGL*/

/*---------------------------------------------------------*/

/*g++ question5Parabola.cpp - o question5Parabola - I C :\glfw - 3.4.bin.WIN64\include - I C :\glew - 2.1.0\include - L C :\glfw - 3.4.bin.WIN64\lib - mingw - w64 - L C :\glew - 2.1.0\lib\Release\x64 - lglfw3dll - lglew32 - lopengl32 - lglu32*/

/*---------------------------------------------------------*/

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

// Window dimensions
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

// Function to draw Cartesian axes with tick marks.
void drawAxes()
{
    // Draw x and y axes in white.
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINES);
    // x-axis from -10 to 110
    glVertex2i(-10, 0);
    glVertex2i(110, 0);
    // y-axis from -20 to 20
    glVertex2i(0, -20);
    glVertex2i(0, 20);
    glEnd();

    // Draw tick marks on the x-axis (every 10 units)
    glBegin(GL_LINES);
    for (int i = -10; i <= 110; i += 10)
    {
        glVertex2i(i, -2);
        glVertex2i(i, 2);
    }
    glEnd();

    // Draw tick marks on the y-axis (every 5 units)
    glBegin(GL_LINES);
    for (int j = -20; j <= 20; j += 5)
    {
        glVertex2i(-2, j);
        glVertex2i(2, j);
    }
    glEnd();
}

int main()
{
    // Initialize GLFW
    if (!glfwInit())
    {
        std::cerr << "GLFW initialization failed." << std::endl;
        return -1;
    }

    // Create a windowed mode window and its OpenGL context.
    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Midpoint Scan Conversion of Parabola x = y^2", nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window." << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW.
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "GLEW initialization failed." << std::endl;
        return -1;
    }

    // Set up an orthographic projection.
    // We need to cover x from about 0 to 100 (since 10^2=100) and y from -10 to 10.
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-10, 110, -20, 20);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Clear the background to black.
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // --- Compute parabola points using the incremental (midpoint) method ---
    // We will compute for y from 0 to 10.
    const int numPoints = 11; // y = 0,1,...,10
    int xPoints[numPoints];   // x coordinate for y>=0
    int yPoints[numPoints];   // y coordinate (will be 0,1,...,10)

    // Start at the vertex.
    int x = 0, y = 0;
    xPoints[0] = x;
    yPoints[0] = y;
    std::cout << "(0, 0)" << std::endl;

    // For each subsequent scanline (y increases by 1),
    // update x by the difference: (y+1)^2 - y^2 = 2y + 1.
    for (int i = 1; i < numPoints; i++)
    {
        int delta = 2 * y + 1; // This is exactly the increment in the ideal curve.
        x = x + delta;         // New x coordinate.
        y = y + 1;             // New y coordinate.
        xPoints[i] = x;
        yPoints[i] = y;
        std::cout << "(" << x << ", " << y << ")" << std::endl;
    }

    // --- Main render loop ---
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw the Cartesian plane.
        drawAxes();

        // Draw the parabola points using GL_POINTS.
        // We use a bright color (yellow) for the points.
        glColor3f(1.0f, 1.0f, 0.0f);
        glPointSize(5.0f);
        glBegin(GL_POINTS);
        // For y >= 0 (from the computed points)
        for (int i = 0; i < numPoints; i++)
        {
            glVertex2i(xPoints[i], yPoints[i]);
        }
        // Using symmetry, for each computed point (except y=0) also plot (x, -y)
        for (int i = 1; i < numPoints; i++)
        {
            glVertex2i(xPoints[i], -yPoints[i]);
        }
        glEnd();

        // For a smoother visual, you can connect the points with lines.
        glColor3f(0.0f, 1.0f, 0.0f); // green line
        glBegin(GL_LINE_STRIP);
        for (int i = 0; i < numPoints; i++)
        {
            glVertex2i(xPoints[i], yPoints[i]);
        }
        glEnd();
        glBegin(GL_LINE_STRIP);
        for (int i = 0; i < numPoints; i++)
        {
            glVertex2i(xPoints[i], -yPoints[i]);
        }
        glEnd();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
