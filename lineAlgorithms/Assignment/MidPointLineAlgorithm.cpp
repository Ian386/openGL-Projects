#include <GL/glut.h>
#include <iostream>
#include <cmath>
#include <string>

// Function to implement the Mid-Point Line Drawing Algorithm
void drawLineMidpoint(int x1, int y1, int x2, int y2) {
   int dx = abs(x2 - x1);
   int dy = abs(y2 - y1);
   int x = x1, y = y1;
   int sx = (x1 < x2) ? 1 : -1; // Step for x
   int sy = (y1 < y2) ? 1 : -1; // Step for y
   bool steep = dy > dx;

   if (steep) std::swap(dx, dy);
   int d = 2 * dy - dx;

   glColor3f(1.0, 1.0, 1.0);
   glPointSize(5.0);
   glBegin(GL_POINTS);
   for (int i = 0; i <= dx; i++) {
       glVertex2i(x, y);
       std::cout << "(" << x << ", " << y << ")\n";
       if (d > 0) {
           if (steep) x += sx;
           else y += sy;
           d -= 2 * dx;
       }
       if (steep) y += sy;
       else x += sx;
       d += 2 * dy;
   }
   glEnd();
   glFlush();
}

// Function to draw text on the screen
void drawText(float x, float y, const std::string& text) {
   glRasterPos2f(x, y);
   for (char c : text) {
       glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
   }
}

// Function to draw the Cartesian plane
void drawCartesianPlane() {
   glColor3f(0.0, 1.0, 0.0);
   glBegin(GL_LINES);
   glVertex2f(-5, 0);
   glVertex2f(5, 0);
   glVertex2f(0, -5);
   glVertex2f(0, 5);
   glEnd();

   for (int i = -5; i <= 5; i++) {
       if (i == 0) continue;
       glBegin(GL_LINES);
       glVertex2f(i, -0.1);
       glVertex2f(i, 0.1);
       glVertex2f(-0.1, i);
       glVertex2f(0.1, i);
       glEnd();
       drawText(i - 0.2, -0.5, std::to_string(i));
       drawText(-0.5, i - 0.1, std::to_string(i));
   }
   glFlush();
}

void display() {
   glClear(GL_COLOR_BUFFER_BIT);
   drawCartesianPlane();
   drawLineMidpoint(0, 2, -1, 4); // Example line
   glFlush();
}

void init() {
   glClearColor(0.0, 0.0, 0.0, 1.0);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluOrtho2D(-5, 5, -5, 5);
}

int main(int argc, char** argv) {
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100);
   glutCreateWindow("Mid-Point Line Drawing");
   init();
   glutDisplayFunc(display);
   glutMainLoop();
   return 0;
}
