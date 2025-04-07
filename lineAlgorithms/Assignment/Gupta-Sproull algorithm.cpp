#include <GL/glut.h>
#include <iostream>
#include <cmath>
#include <string>

// Function to plot a pixel with intensity
void plotPixel(int x, int y, float intensity) {
   glColor3f(1.0, 1.0, 1.0);
   glPointSize(5.0);
   glBegin(GL_POINTS);
   glVertex2i(x, y);
   glEnd();


   std::cout << "(" << x << ", " << y << ") Intensity: " << intensity << "\n";
}

// Function to calculate the intensity using the Gupta-Sproull filter
float guptaSproullIntensity(float distance, float lineWidth) {
   float radius = lineWidth / 2.0f;
   if (distance > radius) return 0.0f; // Outside the filter radius
   return 1.0f - (distance / radius); // Linear falloff
}

// Function to implement the Gupta-Sproull Line Algorithm
void drawLineGuptaSproull(int x1, int y1, int x2, int y2) {
   int dx = abs(x2 - x1);
   int dy = abs(y2 - y1);
   int sx = (x1 < x2) ? 1 : -1;
   int sy = (y1 < y2) ? 1 : -1;
   int err = dx - dy;

   float lineWidth = 2.0f; // Width of the line for anti-aliasing

   glColor3f(1.0, 1.0, 1.0); // Set line color to white
   glPointSize(5.0); // Enlarge the plotted points

   std::cout << "Calculated Points:\n";

   while (true) {
       // Plot the main pixel
       plotPixel(x1, y1, 1.0f);

       // Anti-aliasing: Plot surrounding pixels with intensity
       for (int i = -1; i <= 1; i++) {
           for (int j = -1; j <= 1; j++) {
               if (i == 0 && j == 0) continue; // Skip the main pixel
               float distance = sqrt(i * i + j * j); // Distance from the main pixel
               float intensity = guptaSproullIntensity(distance, lineWidth);
               plotPixel(x1 + i, y1 + j, intensity);
           }
       }

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
   drawLineGuptaSproull(-2, 3, 1, 4); // Draw the Gupta-Sproull line

   glFlush();
}

// Initialize OpenGL settings
void init() {
   glClearColor(0.0, 0.0, 0.0, 1.0); // Set background to black
   glMatrixMode(GL_PROJECTION);
   gluOrtho2D(-10, 10, -10, 10); 
}

// Main function
int main(int argc, char** argv) {
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100);
   glutCreateWindow("Gupta-Sproull Line.");
   init();
   glutDisplayFunc(display);
   glutMainLoop();
   return 0;
}