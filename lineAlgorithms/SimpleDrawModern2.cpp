/*
 * SimpleDrawModern.cpp - Version 1.7 - September 3, 2020
 *
 * Example program illustrating a simple use
 * of Modern OpenGL to draw points and straight lines, and
 * to draw overlapping triangles in 3D.
 *
 * Author: Sam Buss
 *
 * Software accompanying POSSIBLE SECOND EDITION TO the book
 *		3D Computer Graphics: A Mathematical Introduction with OpenGL,
 *		by S. Buss, Cambridge University Press, 2003.
 *
 * Software is "as-is" and carries no warranty.  It may be used without
 *   restriction, but if you modify it, please change the filenames to
 *   prevent confusion between different versions.
 * Bug reports: Sam Buss, sbuss@ucsd.edu.
 * Web page: http://math.ucsd.edu/~sbuss/MathCG2
 */

// Use space to toggle what image is shown.
// Use Escape or 'X' or 'x' to exit.

// These libraries are needed to link the program (Visual Studio specific)
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "glew32.lib")

// Use the static library (so glew32.dll is not needed):
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "ShaderMgrSDM.h"
bool check_for_opengl_errors(); // Function prototype (should really go in a header file)

// Enable standard input and output via printf(), etc.
// Put this include *after* the includes for glew and GLFW!
#include <stdio.h>


//from ShaderMgrSDM.cpp



/*
 * The shader programs are compiled and linked with the code below.
 * They are declared as global variables SimpleDrawModern.cpp
 *   and used there in the myRenderScene routine.
 */
extern unsigned int shaderProgram1;

// ***********************************
// The vertex shader and fragment shader allow each
//  vertex to have its own position and color.
// The position and color are called "vertex attributes"
//  since they (potentially) vary on a per-vertex basis.
// This is a very simple shader program:
//      it has no transformations and no Phong lighting.
// ***********************************

// Sets the position and color of a vertex.
//   This implementations just copies the position with no transformations.
//   It copies the color to "theColor" so that the fragment shader can access it.
const char *vertexShader_PosColorOnly =
    "#version 330 core\n"
    "layout (location = 0) in vec3 vertPos;	   // Position in attribute location 0\n"
    "layout (location = 1) in vec3 vertColor;  // Color in attribute location 1\n"
    "out vec3 theColor;					       // Output a color to the fragment shader\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(vertPos.x, vertPos.y, vertPos.z, 1.0);\n"
    "   theColor = vertColor;\n"
    "}\0";

// Set a general color using a fragment shader. (A "fragment" is a "pixel".)
//    The color value is passed in, obtained from the colors on the vertices.
//    Color values range from 0.0 to 1.0.
//    First three values are Red/Green/Blue (RGB).
//    Fourth color value (alpha) is 1.0, meaning there is no transparency.
const char *fragmentShader_ColorOnly =
    "#version 330 core\n"
    "in vec3 theColor;		// Color value came from the vertex shader (smoothed) \n"
    "out vec4 FragColor;	// Color that will be used for the fragment\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(theColor, 1.0f);   // Add alpha value of 1.0.\n"
    "}\n\0";

/*
 * Build and compile our shader programs
 */
void setup_shaders()
{
    // A very simple shader program: has no transformations and no Phong lighting.
    //      Has a position and color for each vertex.
    shaderProgram1 = setup_shader_vertfrag(vertexShader_PosColorOnly, fragmentShader_ColorOnly);
}

/*
 * Compile a vertex shader and a fragment shader,
 * and combine them into a shader program.
 * Check for compilation and linkage errors (Highly recommended!)
 * Returns the "shaderProgram"
 */
unsigned int setup_shader_vertfrag(const char *vertexShaderSource, const char *fragmentShaderSource)
{
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    check_compilation_shader(vertexShader);

    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    check_compilation_shader(fragmentShader);

    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    check_link_status(shaderProgram);

    // Deallocate shaders since we do not need to use these for other shader programs.
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram; // Return the compiled shaders as a single shader program.
}

/*
 * Check for compile errors for a shader.
 * Parameters:
 *    - shader. The shader identifier (an unsigned integer)
 * Returns:
 *    shader (the same value as was passed in) if compile succeeded.  Or,
 *    0 if an error occured in compilation or if not a valid shader.
 */
GLuint check_compilation_shader(GLuint shader)
{
    if (!glIsShader(shader))
    {
        printf("ERROR: Not a shader! Possibly an allocation error.\n");
        return 0;
    }

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success)
    {
        return shader; // Compilation was successful
    }

    // Compilation failed
    int infoLogLength;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
    char *infoLog = new char[infoLogLength];
    glGetShaderInfoLog(shader, infoLogLength, NULL, infoLog);
    printf("ERROR::Shader compilation failed!\n%s\n", infoLog);
    delete[] infoLog;
    return 0;
}

/*
 * Check for link errors for a program.
 * Parameters:
 *    - program. The program identifier (an unsigned integer)
 *          A "program" is a combination of one or more shaders.
 * Returns:
 *    program (the same value as was passed in) if there are no link errors.  Or,
 *    0 if there is a link error or if not a valid program identifier.
 */
GLuint check_link_status(GLuint program)
{
    if (!glIsProgram(program))
    {
        printf("ERROR: Not a shader program! Possibly an allocation error.\n");
        return 0;
    }

    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (success)
    {
        return program; // Linkage was successful
    }

    int infoLogLength;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
    char *infoLog = new char[infoLogLength];
    glGetProgramInfoLog(program, infoLogLength, NULL, infoLog);
    printf("ERROR::Shader program link failed!\n%s\n", infoLog);
    return 0;
}

//end of ShaderMgrSDM.cpp




// ********************
// Animation controls and state infornation
// ********************

int CurrentMode = 0; // Controls what is drawn.

// ************************
// General data helping with setting up VAO (Vertex Array Objects)
//    and Vertex Buffer Objects.
// ***********************

const int NumObjects = 5; // Increased to accommodate the new star shapes
const int iPoints = 0;
const int iLines = 1;
const int iTriangles = 2;
const int iStar1 = 3; // Index for the first star
const int iStar2 = 4; // Index for the second star

unsigned int myVBO[NumObjects]; // Vertex Buffer Object - holds an array of data
unsigned int myVAO[NumObjects]; // Vertex Array Object - holds info about how the vertex data is formatted

// We create one shader program: it consists of a vertex shader and a fragment shader
unsigned int shaderProgram1;
const unsigned int vertPos_loc = 0;   // Corresponds to "location = 0" in the verter shader definition
const unsigned int vertColor_loc = 1; // Corresponds to "location = 1" in the verter shader definition


// *************************
// mySetupGeometries defines the scene data, especially vertex  positions and colors.
//    - It also loads all the data into the VAO's (Vertex Array Objects) and
//      into the VBO's (Vertex Buffer Objects).
// This routine is only called once to initialize the data.
// *************************
void mySetupGeometries()
{

    // In this simple example, we do not use the Projection or
    //   ModelView matrices. Hence, all x, y, z positions
    //   should be in the range [-1,1].

    // Allocate Vertex Array Objects (VAOs) and Vertex Buffer Objects (VBOs).
    glGenVertexArrays(NumObjects, &myVAO[0]);
    glGenBuffers(NumObjects, &myVBO[0]);

    // FIRST GEOMETRY: Three points
    // Specify three vertices, two value (x,y) for each point.
    //     The z value defaults to zero.
    //     The suffixes "f" make them floats, not doubles, to avoid compiler warnings.
    float threeVerts[] = {
        -0.6f, -0.3f, // First point
        0.6f, -0.3f,  // Second point
        0.6f, 0.3f    // Third point
    };

    // Bind (and initialize) the Vertex Array Object
    glBindVertexArray(myVAO[iPoints]);

    // Allocate space in the vertex buffer (the VBO)
    //   and load the three pairs of x,y values into the VBO.
    //   This is potentially stored in the GPU for quick access.
    glBindBuffer(GL_ARRAY_BUFFER, myVBO[iPoints]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(threeVerts), threeVerts, GL_STATIC_DRAW);

    // Bind the Vertex Array Object and then configure vertex attributes(s).
    //   The vertex attributes as used later by the vertex shader program
    //     consist of three coordinates (x, y, z values)
    //   The vertex shader accesses them through "location = 0",
    //     namely, vertPos_loc is the value 0.
    //   The x,y values come from the VBO array. The z values default to 0.0.
    //   These facts are stored in the VAO.
    //  The color values are not set here: this is done in myRenderScene().
    glVertexAttribPointer(vertPos_loc, 2, GL_FLOAT, GL_FALSE, 0, (void *)0); // Info about where positions are in the VBO
    glEnableVertexAttribArray(vertPos_loc);                                  // Enable the stored vertex positions

    // SECOND GEOMETRY(-IES): Line segments, Line strip, Line loop
    // Specify six vertices that will be used to form lines,
    // Will render with one of GL_LINES or GL_LINE_STRIP or GL_LINE_LOOP
    float sixVertsForLines[] = {
        -0.2f,
        -0.8f,
        0.8f,
        0.2f,
        0.6f,
        0.8f,
        -0.5f,
        0.6f,
        -0.4f,
        -0.0f,
        -0.7f,
        -0.3f,
    };
    // Do the same as above, now for the vertices that specify lines.
    glBindVertexArray(myVAO[iLines]);
    glBindBuffer(GL_ARRAY_BUFFER, myVBO[iLines]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(sixVertsForLines), sixVertsForLines, GL_STATIC_DRAW);
    glVertexAttribPointer(vertPos_loc, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glEnableVertexAttribArray(vertPos_loc);

    // THIRD GEOMETRY: Three triangles
    // Specify nine vertices that will be used to form triangles.
    // The vertices are listed in counterclockwise order.
    // Will render with GL_TRIANGLES
    float trianglesVerts[] = {
        // x,y,z coordinates	// R,G,B colors
        0.7f,
        -0.42f,
        0.0f,
        1.0f,
        0.8f,
        0.8f, // First triangle
        0.7f,
        -0.18f,
        0.0f,
        1.0f,
        0.8f,
        0.8f,
        -0.7f,
        -0.3f,
        0.5f,
        1.0f,
        0.0f,
        0.0f,

        -0.25f,
        0.7f,
        0.0f,
        0.8f,
        1.0f,
        0.8f, // Second triangle
        -0.40f,
        0.55f,
        0.0f,
        0.8f,
        1.0f,
        0.8f,
        0.5f,
        -0.6f,
        0.5f,
        0.0f,
        1.0f,
        0.0f,

        -0.57f,
        -0.53f,
        0.0f,
        0.8f,
        0.8f,
        1.0f, // Third triangle
        -0.43f,
        -0.67f,
        0.0f,
        0.8f,
        0.8f,
        1.0f,
        0.32f,
        0.62f,
        0.5f,
        0.0f,
        0.0f,
        1.0f,
    };

    // Do similarly to above, now for the three triangles.
    // This time, both vertex positions AND vertex colors are loaded into the VBO.
    // Note that glVertexAttribPointer uses a non-zero "stride" parameter.
    // Also note that the second call to glVertexAttribPointer uses
    //        3*sizeof(float) to specify where the color information is.
    glBindVertexArray(myVAO[iTriangles]);
    glBindBuffer(GL_ARRAY_BUFFER, myVBO[iTriangles]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(trianglesVerts), trianglesVerts, GL_STATIC_DRAW);
    glVertexAttribPointer(vertPos_loc, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(vertPos_loc);
    glVertexAttribPointer(vertColor_loc, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(vertColor_loc);






    // FOURTH GEOMETRY: First star shape
    float star1Verts[] = {
        // Define the vertices for the first star shape
        0.0f, 1.0f,
        0.2f, 0.3f,
        0.9f, 0.3f,
        0.3f, -0.2f,
        0.6f, -0.9f,
        0.0f, -0.4f,
        -0.6f, -0.9f,
        -0.3f, -0.2f,
        -0.9f, 0.3f,
        -0.2f, 0.3f};

    glBindVertexArray(myVAO[iStar1]);
    glBindBuffer(GL_ARRAY_BUFFER, myVBO[iStar1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(star1Verts), star1Verts, GL_STATIC_DRAW);
    glVertexAttribPointer(vertPos_loc, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glEnableVertexAttribArray(vertPos_loc);

    // FIFTH GEOMETRY: Nautical star shape
    float nauticalStarVerts[] = {
        // First triangle (light color)
        0.0f, 1.0f,     // Top point
        -0.95f, -0.31f, // Bottom left
        0.95f, -0.31f,  // Bottom right

        // Second triangle (dark color, rotated 180 degrees)
        0.0f, -1.0f,  // Bottom point
        0.95f, 0.31f, // Top right
        -0.95f, 0.31f // Top left
    };

    float nauticalStarColors[] = {
        // Colors for the first triangle (light)
        1.0f, 1.0f, 0.0f, // Yellow (light)
        1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,

        // Colors for the second triangle (dark)
        0.0f, 0.0f, 1.0f, // Blue (dark)
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f};

    glBindVertexArray(myVAO[iStar2]);
    glBindBuffer(GL_ARRAY_BUFFER, myVBO[iStar2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(nauticalStarVerts) + sizeof(nauticalStarColors), nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(nauticalStarVerts), nauticalStarVerts);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(nauticalStarVerts), sizeof(nauticalStarColors), nauticalStarColors);

    glVertexAttribPointer(vertPos_loc, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glEnableVertexAttribArray(vertPos_loc);
    glVertexAttribPointer(vertColor_loc, 3, GL_FLOAT, GL_FALSE, 0, (void *)(sizeof(nauticalStarVerts)));
    glEnableVertexAttribArray(vertColor_loc);

    // This is optional, but allowed.  The VAO already knows which buffer (VBO) is holding its
    //     vertex data, so it is OK to unbind the VBO here.
    // This can help with debugging: by unbinding here, a bug in later the code will not
    //     quietly affect the current GL_ARRAY_BUFFER (VBO) or the current Vertex Array Object (VAO)
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    check_for_opengl_errors(); // Really a great idea to check for errors -- esp. good for debugging!
}
// *************************************
// Main routine for rendering the scene
// myRenderScene() is called every time the scene needs to be redrawn.
// mySetupGeometries() has already created the vertex and buffer objects, etc.
// setup_shaders() has already created the shader programs.
// *************************************
void myRenderScene()
{

    // Clear the rendering window
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // This very simple shader program is used for all the items.
    glUseProgram(shaderProgram1);

    switch (CurrentMode)
    {
    case 0:
        // Draw three overlapping triangles
        // Colors for the triangles have already been loaded into the VBO.
        glBindVertexArray(myVAO[iTriangles]);
        glDrawArrays(GL_TRIANGLES, 0, 9);
        break;
    case 1:
        // Draw separate lines:
        glBindVertexArray(myVAO[iLines]);
        glVertexAttrib3f(vertColor_loc, 0.5f, 1.0f, 0.2f); // A greenish color (R, G, B values).
        glDrawArrays(GL_LINES, 0, 6);
        break;
    case 2:
        // Draw line strip:
        glBindVertexArray(myVAO[iLines]);
        glVertexAttrib3f(vertColor_loc, 1.0f, 0.2f, 1.0f); // Magenta color (R, G, B values).
        glDrawArrays(GL_LINE_STRIP, 0, 6);
        break;
    case 3:
        // Draw line loop:
        glBindVertexArray(myVAO[iLines]);
        glVertexAttrib3f(vertColor_loc, 1.0f, 1.0f, 0.2f); // A yellow-ish color (R, G, B values).
        glDrawArrays(GL_LINE_LOOP, 0, 6);
        break;
    case 4:
        // Draw line's vertices (points):
        glBindVertexArray(myVAO[iLines]);
        glVertexAttrib3f(vertColor_loc, 1.0f, 1.0f, 1.0f); // A white color (R, G, B values).
        glDrawArrays(GL_POINTS, 0, 6);
        break;
    case 5:
        // Draw three points
        glBindVertexArray(myVAO[iPoints]);
        glVertexAttrib3f(vertColor_loc, 1.0f, 0.5f, 0.2f); // An orange-red color (R, G, B values).
        glDrawArrays(GL_POINTS, 0, 3);
        break;
    case 6:
        // Draw the first star shape
        glBindVertexArray(myVAO[iStar1]);
        glVertexAttrib3f(vertColor_loc, 1.0f, 0.0f, 0.0f); // Red color
        glDrawArrays(GL_LINE_LOOP, 0, 10);
        break;
    case 7:
        // Draw the nautical star shape
        glBindVertexArray(myVAO[iStar2]);
        glDrawArrays(GL_TRIANGLES, 0, 6); // Draw two triangles
        break;
    }

    glBindVertexArray(0);      // Not necessary, but a good idea
    check_for_opengl_errors(); // Really a great idea to check for errors -- esp. good for debugging!
}

void my_setup_SceneData()
{
    mySetupGeometries();
    setup_shaders();

    check_for_opengl_errors(); // Really a great idea to check for errors -- esp. good for debugging!
}

// *******************************************************
// Process all key press events.
// This routine is called each time a key is pressed or released or repeats.
// *******************************************************
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_RELEASE)
    {
        return; // Ignore key up (key release) events
    }
    if (key == GLFW_KEY_ESCAPE || key == GLFW_KEY_X)
    {
        glfwSetWindowShouldClose(window, true);
    }
    else if (key == GLFW_KEY_SPACE)
    {
        CurrentMode = (CurrentMode + 1) % 8; // Now cycles through 0 to 7
    }
}

// *************************************************
// This function is called when the graphics window is first created,
//    and again whenever it is resized.
// The Projection View Matrix is typically set here.
//    But this program does not use any transformations or matrices.
// *************************************************
void window_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height); // Draw into entire window
}

void my_setup_OpenGL()
{

    glEnable(GL_DEPTH_TEST); // Enable depth buffering
    glDepthFunc(GL_LEQUAL);  // Useful for multipass shaders

    // Set polygon drawing mode for front and back of each triangle
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

    // Disable backface culling to render both sides of triangles
    // glDisable(GL_CULL_FACE);

// TRY IT OUT: How do the results look different if you disable the next block
// of code. (By changing "#if 1" to "#if 0"
#if 1
    // The following commands should induce OpenGL to create round points and
    //	antialias points and lines.  (This is implementation dependent unfortunately.)
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST); // Make round points, not square points
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);  // Antialias the lines
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#endif

    // Specify the diameter of points, and the width of lines. (Implementation dependent.)
    //   Permissible values are 1 and usually greater.
    // TRY IT OUT: Experiment with increasing and decreasing these values.
    glPointSize(8);
    glLineWidth(5);
}

void error_callback(int error, const char *description)
{
    // Print error
    fputs(description, stderr);
}

void setup_callbacks(GLFWwindow *window)
{
    // Set callback function for resizing the window
    glfwSetFramebufferSizeCallback(window, window_size_callback);

    // Set callback for key up/down/repeat events
    glfwSetKeyCallback(window, key_callback);

    // Set callbacks for mouse movement (cursor position) and mouse botton up/down events.
    // glfwSetCursorPosCallback(window, cursor_pos_callback);
    // glfwSetMouseButtonCallback(window, mouse_button_callback);
}

// **********************
// Here is the main program
// **********************

int main()
{
    glfwSetErrorCallback(error_callback); // Supposed to be called in event of errors. (doesn't work?)
    glfwInit();
#if defined(__APPLE__) || defined(__linux__)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    const int initWidth = 800;
    const int initHeight = 600;
    GLFWwindow *window = glfwCreateWindow(initWidth, initHeight, "SimpleDrawModern", NULL, NULL);
    if (window == NULL)
    {
        fprintf(stdout, "Failed to create GLFW window!\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glewInit();

    // Print info of GPU and supported OpenGL version
    printf("Renderer: %s\n", glGetString(GL_RENDERER));
    printf("OpenGL version supported %s\n", glGetString(GL_VERSION));
#ifdef GL_SHADING_LANGUAGE_VERSION
    printf("Supported GLSL version is %s.\n", (char *)glGetString(GL_SHADING_LANGUAGE_VERSION));
#endif
    printf("Using GLEW version %s.\n", glewGetString(GLEW_VERSION));
    printf("------------------------------\n");
    printf("Press space bar to cycle thru images.\n");
    printf("Press ESCAPE or 'X' or 'x' to exit.\n");

    setup_callbacks(window);
    window_size_callback(window, initWidth, initHeight);

    // Initialize OpenGL, the scene and the shaders
    my_setup_OpenGL();
    check_for_opengl_errors();

    my_setup_SceneData();

    // Loop while program is not terminated.
    while (!glfwWindowShouldClose(window))
    {

        myRenderScene();         // Render into the current buffer
        glfwSwapBuffers(window); // Displays what was just rendered (using double buffering).

        // Poll events (key presses, mouse events)
        glfwWaitEvents(); // Use this if no animation.
                          // glfwWaitEventsTimeout(1.0/60.0);	// Use this to animate at 60 frames/sec (timing is NOT reliable)
        //  glfwPollEvents();				// Use this version when animating as fast as possible
    }

    glfwTerminate();
    return 0;
}

// If an error is found, it could have been caused by any command since the
//   previous call to check_for_opengl_errors()
// To find what generated the error, you can try adding more calls to
//   check_for_opengl_errors().
char errNames[9][36] = {
    "Unknown OpenGL error",
    "GL_INVALID_ENUM", "GL_INVALID_VALUE", "GL_INVALID_OPERATION",
    "GL_INVALID_FRAMEBUFFER_OPERATION", "GL_OUT_OF_MEMORY",
    "GL_STACK_UNDERFLOW", "GL_STACK_OVERFLOW", "GL_CONTEXT_LOST"};
bool check_for_opengl_errors()
{
    int numErrors = 0;
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR)
    {
        numErrors++;
        int errNum = 0;
        switch (err)
        {
        case GL_INVALID_ENUM:
            errNum = 1;
            break;
        case GL_INVALID_VALUE:
            errNum = 2;
            break;
        case GL_INVALID_OPERATION:
            errNum = 3;
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            errNum = 4;
            break;
        case GL_OUT_OF_MEMORY:
            errNum = 5;
            break;
        case GL_STACK_UNDERFLOW:
            errNum = 6;
            break;
        case GL_STACK_OVERFLOW:
            errNum = 7;
            break;
        case GL_CONTEXT_LOST:
            errNum = 8;
            break;
        }
        printf("OpenGL ERROR: %s.\n", errNames[errNum]);
    }
    return (numErrors != 0);
}