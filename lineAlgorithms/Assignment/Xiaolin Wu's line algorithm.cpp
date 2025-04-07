#include <GL/glut.h>
#include <iostream>
#include <cmath>
#include <string>

// Function to plot a pixel with intensity
void plotPixel(int x, int y, float intensity) {
   glColor3f(1.0, 1.0, 1.0)
   glPointSize(5.0); 
   glBegin(GL_POINTS);
   glVertex2i(x, y);
   glEnd();


   std::cout << "(" << x << ", " << y << ") Intensity: " << intensity << "\n";
}

// Function to implement Xiaolin Wu's Line Algorithm
void drawLineXiaolinWu(int x1, int y1, int x2, int y2) {
   bool steep = abs(y2 - y1) > abs(x2 - x1);

   if (steep) {
       std::swap(x1, y1);
       std::swap(x2, y2);
   }
   if (x1 > x2) {
       std::swap(x1, x2);
       std::swap(y1, y2);
   }

   float dx = x2 - x1;
   float dy = y2 - y1;
   float gradient = (dx == 0.0f) ? 1.0f : dy / dx;

   // Handle first endpoint
   int xend = round(x1);
   float yend = y1 + gradient * (xend - x1);
   float xgap = 1.0f - (x1 + 0.5f) - floor(x1 + 0.5f);
   int xpxl1 = xend;
   int ypxl1 = floor(yend);
   if (steep) {
       plotPixel(ypxl1, xpxl1, 1.0f - (yend - floor(yend)) * xgap);
       plotPixel(ypxl1 + 1, xpxl1, (yend - floor(yend)) * xgap);
   }
   else {
       plotPixel(xpxl1, ypxl1, 1.0f - (yend - floor(yend)) * xgap);
       plotPixel(xpxl1, ypxl1 + 1, (yend - floor(yend)) * xgap);
   }
   float intery = yend + gradient;

   // Handle second endpoint
   xend = round(x2);
   yend = y2 + gradient * (xend - x2);
   xgap = (x2 + 0.5f) - floor(x2 + 0.5f);
   int xpxl2 = xend;
   int ypxl2 = floor(yend);
   if (steep) {
       plotPixel(ypxl2, xpxl2, 1.0f - (yend - floor(yend)) * xgap);
       plotPixel(ypxl2 + 1, xpxl2, (yend - floor(yend)) * xgap);
   }
   else {
       plotPixel(xpxl2, ypxl2, 1.0f - (yend - floor(yend)) * xgap);
       plotPixel(xpxl2, ypxl2 + 1, (yend - floor(yend)) * xgap);
   }

   // Main loop
   if (steep) {
       for (int x = xpxl1 + 1; x < xpxl2; x++) {
           plotPixel(floor(intery), x, 1.0f - (intery - floor(intery)));
           plotPixel(floor(intery) + 1, x, intery - floor(intery));
           intery += gradient;
       }
   }
   else {
       for (int x = xpxl1 + 1; x < xpxl2; x++) {
           plotPixel(x, floor(intery), 1.0f - (intery - floor(intery)));
           plotPixel(x, floor(intery) + 1, intery - floor(intery));
           intery += gradient;
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
   glColor3f(0.0, 1.0, 0.0); // axis color to green

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
       if (i == 0) continue; 

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

   drawCartesianPlane(); 
   drawLineXiaolinWu(1, 1, 3, 5); 

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
   glutCreateWindow("Xiaolin Wu's Line.");
   init();
   glutDisplayFunc(display);
   glutMainLoop();
   return 0;
}