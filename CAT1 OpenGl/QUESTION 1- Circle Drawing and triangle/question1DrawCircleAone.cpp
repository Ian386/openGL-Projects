// NAME: IAN NDOLO MWAU
// REG NO: SCT211-0034/2022

// QUESTION 1- Circle Drawing and triangle

/* a) Using OpenGL, write a program that draws a circle
    using Brensenham circle drawing algorithm: The parameters for the circle
    are as follows – the radius should be 4 Centimeters and the starting point
    coordinates are (1,1)*/

// Ensure that all your drawings have coordinates(x.y) labelled and scaled

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

// Window dimensions
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;

// Draw tick marks along axes for clarity
void drawAxes() {
    // Set color for axes (white)
    glColor3f(1.0f, 1.0f, 1.0f);
    
    // Draw the main x-axis and y-axis lines
    glBegin(GL_LINES);
        // x-axis from -5 to 10
        glVertex2f(-5.0f, 0.0f);
        glVertex2f(10.0f, 0.0f);
        // y-axis from -5 to 10
        glVertex2f(0.0f, -5.0f);
        glVertex2f(0.0f, 10.0f);
    glEnd();
    
    // Draw tick marks along the x-axis
    glBegin(GL_LINES);
    for (int i = -5; i <= 10; i++) {
        glVertex2f(i, -0.2f);
        glVertex2f(i, 0.2f);
    }
    glEnd();
    
    // Draw tick marks along the y-axis
    glBegin(GL_LINES);
    for (int i = -5; i <= 10; i++) {
        glVertex2f(-0.2f, i);
        glVertex2f(0.2f, i);
    }
    glEnd();
    
    // Note: For actual numeric labels you would need to render text,
    // which requires additional libraries or routines.
}

// Plot the eight symmetric points of the circle
void plotCirclePoints(int cx, int cy, int x, int y, std::vector<float> &vertices) {
    // Each push_back adds one coordinate (we use units in cm)
    vertices.push_back(cx + x); vertices.push_back(cy + y);
    vertices.push_back(cx - x); vertices.push_back(cy + y);
    vertices.push_back(cx + x); vertices.push_back(cy - y);
    vertices.push_back(cx - x); vertices.push_back(cy - y);
    vertices.push_back(cx + y); vertices.push_back(cy + x);
    vertices.push_back(cx - y); vertices.push_back(cy + x);
    vertices.push_back(cx + y); vertices.push_back(cy - x);
    vertices.push_back(cx - y); vertices.push_back(cy - x);
}

// Bresenham Circle Drawing Algorithm (using integer arithmetic)
void drawBresenhamCircle(int cx, int cy, int radius, std::vector<float> &vertices) {
    int x = 0;
    int y = radius;
    int d = 3 - (2 * radius);
    
    while (x <= y) {
        plotCirclePoints(cx, cy, x, y, vertices);
        if (d < 0) {
            d += (4 * x) + 6;
        } else {
            d += (4 * (x - y)) + 10;
            y--;
        }
        x++;
    }
}

// Render the scene: draw axes and the circle
void renderScene() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Draw coordinate axes
    drawAxes();
    
    // Prepare circle vertices using Bresenham's algorithm
    std::vector<float> circleVertices;
    int cx = 1;      // Center x (in cm)
    int cy = 1;      // Center y (in cm)
    int radius = 4;  // Radius (in cm)
    drawBresenhamCircle(cx, cy, radius, circleVertices);
    
    // Draw the circle points
    glColor3f(0.0f, 1.0f, 0.0f); // Green color for the circle
    glPointSize(5.0f);           // Increase point size for visibility
    glBegin(GL_POINTS);
    for (size_t i = 0; i < circleVertices.size(); i += 2) {
        glVertex2f(circleVertices[i], circleVertices[i + 1]);
    }
    glEnd();
}

// Main function
int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    
    // Create a windowed mode window and its OpenGL context
    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Bresenham Circle Drawing", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    
    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }
    
    // Set viewport and projection
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Set an orthographic projection that uses cm as units.
    // This view ranges from -5 cm to 10 cm in both x and y,
    // which ensures our circle (centered at (1,1) with radius 4)
    // will be clearly visible.
    glOrtho(-5.0, 10.0, -5.0, 10.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // Main loop
    while (!glfwWindowShouldClose(window)) {
        renderScene();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    // Cleanup and exit
    glfwTerminate();
    return 0;
}
