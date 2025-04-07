#include <GL/glut.h>
#include <iostream>
#include <cmath>
#include <string>

// Function to implement Bresenham�s Line-Drawing Algorithm
void drawLineBresenham(int x1, int y1, int x2, int y2) {
   int dx = abs(x2 - x1);
   int dy = abs(y2 - y1);
   int sx = (x1 < x2) ? 1 : -1;
   int sy = (y1 < y2) ? 1 : -1;
   int err = dx - dy;

   glColor3f(1.0, 1.0, 1.0);
   glPointSize(5.0); 

   std::cout << "Calculated Points:\n";

   glBegin(GL_POINTS);
   while (true) {
       glVertex2i(x1, y1); 
       std::cout << "(" << x1 << ", " << y1 << ")\n"; // Print the point to the terminal

       if (x1 == x2 && y1 == y2) break; // Exit loop when the endpoint is reached

       int e2 = 2 * err;
       if (e2 > -dy) {
           err -= dy;
           x1 += sx;
       }
       if (e2 < dx) {
           err += dx;
           y1 += sy;
       }
   }
   glEnd();

   // Draw a line connecting the points
   glBegin(GL_LINE_STRIP);
   x1 = x1, y1 = y1;
   int x = x1, y = y1;
   err = dx - dy;
   while (true) {
       glVertex2i(x, y);

       if (x == x2 && y == y2) break;

       int e2 = 2 * err;
       if (e2 > -dy) {
           err -= dy;
           x += sx;
       }
       if (e2 < dx) {
           err += dx;
           y += sy;
       }
   }
   glEnd();

   glFlush();
}

// Function to draw text on the screen
void drawText(float x, float y, const std::string& text) {
   glRasterPos2f(x, y);
   for (char c : text) {
       glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
   }
}

// Function to draw the Cartesian plane (axes) with numbered ticks
void drawCartesianPlane() {
   glColor3f(0.0, 1.0, 0.0); // Set axis color to green

   // Draw X and Y axes
   glBegin(GL_LINES);
   // X-axis
   glVertex2i(-10, 0);
   glVertex2i(10, 0);

   // Y-axis
   glVertex2i(0, -10);
   glVertex2i(0, 10);
   glEnd();

   // Draw ticks and numbers on the X-axis
   for (int i = -10; i <= 10; i++) {
       if (i == 0) continue; // Skip the origin to avoid overlapping text

       // Draw tick marks
       glBegin(GL_LINES);
       glVertex2i(i, -0.2);
       glVertex2i(i, 0.2);
       glEnd();

       // Draw numbers
       if (i % 2 == 0) { // Label every 2 units
           std::string num = std::to_string(i);
           drawText(i - 0.1, -0.5, num);
       }
   }

   // Draw ticks and numbers on the Y-axis
   for (int i = -10; i <= 10; i++) {
       if (i == 0) continue; // Skip the origin to avoid overlapping text

       // Draw tick marks
       glBegin(GL_LINES);
       glVertex2i(-0.2, i);
       glVertex2i(0.2, i);
       glEnd();

       // Draw numbers
       if (i % 2 == 0) { // Label every 2 units
           std::string num = std::to_string(i);
           drawText(-0.5, i - 0.1, num);
       }
   }

   glFlush();
}

// Display function
void display() {
   glClear(GL_COLOR_BUFFER_BIT);

   drawCartesianPlane(); // Draw the Cartesian plane
   drawLineBresenham(1, 5, 2, 8); // Draw the Bresenham line

   glFlush();
}

// Initialize OpenGL settings
void init() {
   glClearColor(0.0, 0.0, 0.0, 1.0); // Set background to black
   glMatrixMode(GL_PROJECTION);
   gluOrtho2D(-10, 10, -10, 10); // Define coordinate system (Cartesian plane)
}

// Main function
int main(int argc, char** argv) {
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100);
   glutCreateWindow("Bresenham�s Line");
   init();
   glutDisplayFunc(display);
   glutMainLoop();
   return 0;
}