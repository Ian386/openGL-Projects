// NAME: IAN NDOLO MWAU
// REG NO: SCT211-0034/2022

// QUESTION 2- Ellipse drawing
/*a) Using Open GL and midpoint ellipse algorithm, draw an ellipse with centre
as (1,2) given by
i) Apply the flood-fill algorithm in OpenGL to fill the interior of the
above ellipse with orange color (#FFA500)*/

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <cmath> // For sqrtf

// Window dimensions
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;

struct Point2D
{
    int x;
    int y;
};

void plotEllipsePoints(int cx, int cy, int x, int y, std::vector<Point2D> &points)
{
    points.push_back({cx + x, cy + y});
    points.push_back({cx - x, cy + y});
    points.push_back({cx - x, cy - y});
    points.push_back({cx + x, cy - y});
}

void midpointEllipse(int cx, int cy, int a, int b, std::vector<Point2D> &points)
{
    float d1 = (b * b) - (a * a * b) + (0.25f * a * a);
    int x = 0, y = b;
    plotEllipsePoints(cx, cy, x, y, points);
    while ((a * a) * (y - 0.5f) > (b * b) * (x + 1))
    {
        if (d1 < 0)
            d1 += (b * b) * (2 * x + 3);
        else
        {
            d1 += (b * b) * (2 * x + 3) + (a * a) * (-2 * y + 2);
            y--;
        }
        x++;
        plotEllipsePoints(cx, cy, x, y, points);
    }
    float d2 = (b * b) * (x + 0.5f) * (x + 0.5f) + (a * a) * (y - 1) * (y - 1) - (a * a * b * b);
    while (y > 0)
    {
        if (d2 < 0)
        {
            d2 += (b * b) * (2 * x + 2) + (a * a) * (-2 * y + 3);
            x++;
        }
        else
            d2 += (a * a) * (-2 * y + 3);
        y--;
        plotEllipsePoints(cx, cy, x, y, points);
    }
}

void drawAxes()
{
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINES);
    glVertex2f(-20.0f, 0.0f);
    glVertex2f(20.0f, 0.0f);
    glVertex2f(0.0f, -20.0f);
    glVertex2f(0.0f, 20.0f);
    glEnd();
    glBegin(GL_LINES);
    for (int i = -20; i <= 20; i++)
    {
        glVertex2f(i, -0.2f);
        glVertex2f(i, 0.2f);
        glVertex2f(-0.2f, i);
        glVertex2f(0.2f, i);
    }
    glEnd();
}

void renderScene()
{
    glClear(GL_COLOR_BUFFER_BIT);
    drawAxes();

    std::vector<Point2D> ellipsePoints;
    int cx = 2, cy = -1, a = 6, b = 5;
    midpointEllipse(cx, cy, a, b, ellipsePoints);

    // Fill the ellipse with orange color
    glColor3f(1.0f, 0.647f, 0.0f); // #FFA500
    glBegin(GL_LINES);
    for (float y = cy - b; y <= cy + b; y += 0.1f)
    {
        float y_rel = y - cy; // y + 1 since cy is -1
        float term = 1.0f - (y_rel * y_rel) / (b * b);
        if (term < 0.0f)
            continue;
        float sqrt_term = sqrtf(term);
        float x_offset = a * sqrt_term;
        float x_left = cx - x_offset;
        float x_right = cx + x_offset;
        glVertex2f(x_left, y);
        glVertex2f(x_right, y);
    }
    glEnd();

    // Draw ellipse boundary
    glColor3f(1.0f, 0.0f, 0.0f);
    glPointSize(4.0f);
    glBegin(GL_POINTS);
    for (auto &p : ellipsePoints)
        glVertex2i(p.x, p.y);
    glEnd();
}

int main()
{
    if (!glfwInit())
        return -1;
    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Ellipse Fill", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK)
        return -1;
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-20.0, 20.0, -20.0, 20.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    while (!glfwWindowShouldClose(window))
    {
        renderScene();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}