/*
* Pyramid with animation
*/
#include <windows.h>	// for MS Windows
#include <glut.h>		// GLUT, include glu.h and gl.h

/* Global variables */
char title[] = "Pyramid with animation";
GLfloat anglePyramid = 0.0f;  // Rotational angle for pyramid [NEW]
int refreshMills = 10;        // refresh interval in milliseconds [NEW]

double mX = 0; // Move X
double mY = 0; // Move Y

// The coordinates for the location of the pyramid on screen
double x = 0.0f;
double y = 0.0f;
double z = -15.0f; //(0.0f, 0.0f, -5.0f);

/* Initialize OpenGL Graphics */
void initGL() {
	glClearColor(160.0/225.0f, 94/225.0f, 247/225.0f, 1.0f); // Set background color to black and opaque
	glClearDepth(1.0f);                   // Set background depth to farthest
	glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
	glDepthFunc(GL_LEQUAL);    // Set the type of depth-test
	glShadeModel(GL_SMOOTH);   // Enable smooth shading
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections
}

// Keyboard arrows function
void keyboard(int key, int x, int y)
{
	if (key == GLUT_KEY_RIGHT)
	{
		mY -= 0.5; //mY += 15;
	}
	else if (key == GLUT_KEY_LEFT)
	{
		mY += 0.5; //mY -= 15;
	}
	else if (key == GLUT_KEY_DOWN)
	{
		mX -= 0.5; //mX -= 15;
	}
	else if (key == GLUT_KEY_UP)
	{
		mX += 0.5; //mX += 15;
	}

	// Request display update
	glutPostRedisplay();
}

/* Handler for window-repaint event. Called back when the window first appears and
whenever the window needs to be re-painted. */
void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers
	glMatrixMode(GL_MODELVIEW);     // To operate on model-view matrix
	
	// Render first pyramid consists of 6 triangles
	glLoadIdentity();                  // Reset the model-view matrix

	// Move when user changes mX and mY
	glRotatef(mX, 1.0, 0.0, 0.0);
	glRotatef(mY, 0.0, 1.0, 0.0);

	glTranslatef(x, y, z);  // Move left and into the screen   // set location  modif
	//glTranslatef(0.0f, 0.0f, -5.0f);  // Move left and into the screen   // set location
	glRotatef(anglePyramid, 1.0f, 1.0f, 0.0f);  // Rotate about the (1,1,0)-axis [NEW]

	glBegin(GL_TRIANGLES);           // Begin drawing the pyramid with 4 triangles
	// Front
	glColor3f(1.0f, 0.0f, 0.0f);     // Red
	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);

	// Right
	glColor3f(1.0f, 0.0f, 1.0f);     // Magenta
	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);

	// Back
	glColor3f(0.0f, 0.0f, 1.0f);     // Blue
	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);

	// Left
	glColor3f(1.0f, 1.0f, 0.0f);     // Yellow
	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);

	// Bottom A
	glColor3f(0.0f, 1.0f, 0.0f);     // Green
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);

	// Bottom B
	glColor3f(0.0f, 1.0f, 0.0f);     // Green
	glVertex3f(1.0f, -1.0f, -1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);

	glEnd();   // Done drawing the pyramid

	glutSwapBuffers();  // Swap the front and back frame buffers (double buffering)

	// Update the rotational angle after each refresh [NEW]
	anglePyramid += 0.3f;
	//glutSpecialFunc(keyboard);
}


/* Called back when timer expired [NEW] */
void timer(int value) {
	glutPostRedisplay();      // Post re-paint request to activate display()
	glutTimerFunc(refreshMills, timer, 0); // next timer call milliseconds later
}

/* Handler for window re-size event. Called back when the window first appears and
whenever the window is re-sized with its new width and height */
void reshape(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer
	// Compute aspect ratio of the new window
	if (height == 0) height = 1;                // To prevent divide by 0
	GLfloat aspect = (GLfloat)width / (GLfloat)height;

	// Set the viewport to cover the new window
	glViewport(0, 0, width, height);

	// Set the aspect ratio of the clipping volume to match the viewport
	glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
	glLoadIdentity();             // Reset
	// Enable perspective projection with fovy, aspect, zNear and zFar
	gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}

/* Main function: GLUT runs as a console application starting at main() */
int main(int argc, char** argv) {
	glutInit(&argc, argv);            // Initialize GLUT
	glutInitDisplayMode(GLUT_DOUBLE); // Enable double buffered mode
	glutInitWindowSize(640, 480);   // Set the window's initial width & height
	glutInitWindowPosition(50, 50); // Position the window's initial top-left corner
	glutCreateWindow(title);        // Create window with the given title
	glutDisplayFunc(display);       // Register callback handler for window re-paint event
	glutSpecialFunc(keyboard);		// Callback functions for keyboard's input
	glutReshapeFunc(reshape);       // Register callback handler for window re-size event
	initGL();                       // Our own OpenGL initialization
	glutTimerFunc(0, timer, 0);     // First timer call immediately [NEW]
	glutMainLoop();                 // Enter the infinite event-processing loop
	return 0;
}