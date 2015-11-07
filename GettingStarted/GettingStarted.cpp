#include <iostream>
#include <GL/freeglut.h>
#include <PxPhysicsAPI.h> 
#include <extensions\PxExtensionsAPI.h>
#include <extensions\PxDefaultErrorCallback.h>
#include <extensions\PxDefaultAllocator.h> 
#include <extensions\PxDefaultSimulationFilterShader.h>
#include <extensions\PxDefaultCpuDispatcher.h>
#include <extensions\PxShapeExt.h>
#include <foundation\PxMat33.h> 
#include <extensions\PxSimpleFactory.h>

using namespace std;
using namespace physx;

#ifdef _DEBUG
#pragma comment(lib, "PhysX3DEBUG_x86.lib")
#pragma comment(lib, "PxTask.lib")
#pragma comment(lib, "PhysX3CommonDEBUG_x86.lib")
#pragma comment(lib, "PhysX3ExtensionsDEBUG.lib")
#else
#pragma comment(lib, "PhysX3_x86.lib")
#pragma comment(lib, "PxTask.lib")
#pragma comment(lib, "PhysX3Common_x86.lib")
#pragma comment(lib, "PhysX3Extensions.lib")
#endif

const int	WINDOW_WIDTH = 1024,
WINDOW_HEIGHT = 768;


static PxPhysics* gPhysicsSDK = NULL;
static PxDefaultErrorCallback gDefaultErrorCallback;
static PxDefaultAllocator gDefaultAllocatorCallback;
static PxFoundation* gFoundation = NULL;

void RenderSpacedBitmapString(
	int x,
	int y,
	int spacing,
	void *font,
	char *string) {
	char *c;
	int x1 = x;
	for(c = string; *c!='\0'; c++) {
		glRasterPos2i(x1, y);
		glutBitmapCharacter(font, *c);
		x1 = x1+glutBitmapWidth(font, *c)+spacing;
	}
}

void DrawAxes() {
	//To prevent the view from disturbed on repaint
	//this push matrix call stores the current matrix state
	//and restores it once we are done with the arrow rendering
	glPushMatrix();
	glColor3f(0, 0, 1);
	glPushMatrix();
	glTranslatef(0, 0, 0.8f);
	glutSolidCone(0.0325, 0.2, 4, 1);
	//Draw label			
	glTranslatef(0, 0.0625, 0.225f);
	RenderSpacedBitmapString(0, 0, 0, GLUT_BITMAP_HELVETICA_10, "Z");
	glPopMatrix();
	glutSolidCone(0.0225, 1, 4, 1);

	glColor3f(1, 0, 0);
	glRotatef(90, 0, 1, 0);
	glPushMatrix();
	glTranslatef(0, 0, 0.8f);
	glutSolidCone(0.0325, 0.2, 4, 1);
	//Draw label
	glTranslatef(0, 0.0625, 0.225f);
	RenderSpacedBitmapString(0, 0, 0, GLUT_BITMAP_HELVETICA_10, "X");
	glPopMatrix();
	glutSolidCone(0.0225, 1, 4, 1);

	glColor3f(0, 1, 0);
	glRotatef(90, -1, 0, 0);
	glPushMatrix();
	glTranslatef(0, 0, 0.8f);
	glutSolidCone(0.0325, 0.2, 4, 1);
	//Draw label
	glTranslatef(0, 0.0625, 0.225f);
	RenderSpacedBitmapString(0, 0, 0, GLUT_BITMAP_HELVETICA_10, "Y");
	glPopMatrix();
	glutSolidCone(0.0225, 1, 4, 1);
	glPopMatrix();
}

void DrawGrid(int GRID_SIZE) {
	glBegin(GL_LINES);
	glColor3f(0.75f, 0.75f, 0.75f);
	for(int i = -GRID_SIZE; i<=GRID_SIZE; i++) {
		glVertex3f((float) i, 0, (float) -GRID_SIZE);
		glVertex3f((float) i, 0, (float) GRID_SIZE);

		glVertex3f((float) -GRID_SIZE, 0, (float) i);
		glVertex3f((float) GRID_SIZE, 0, (float) i);
	}
	glEnd();
}


void InitializePhysX() {

	gFoundation = PxCreateFoundation(
		PX_PHYSICS_VERSION,
		gDefaultAllocatorCallback,
		gDefaultErrorCallback);

	// Creating instance of PhysX SDK
	gPhysicsSDK = PxCreatePhysics(
		PX_PHYSICS_VERSION,
		*gFoundation,
		PxTolerancesScale());

	if(gPhysicsSDK==NULL) {
		cerr<<"Error creating PhysX device."<<endl;
		cerr<<"Exiting..."<<endl;
		exit(1);
	}
}
void ShutdownPhysX() {
	gPhysicsSDK->release();
}

void InitGL() {}

void OnReshape(int nw, int nh) {
	glViewport(0, 0, nw, nh);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (GLfloat) nw/(GLfloat) nh, 0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
}

void OnRender() {
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	DrawAxes();
	DrawGrid(10);

	glutSwapBuffers();
}

void OnShutdown() {
	ShutdownPhysX();
}
void main(int argc, char** argv) {
	atexit(OnShutdown);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("GLUT PhysX3 Demo");

	glutDisplayFunc(OnRender);
	glutReshapeFunc(OnReshape);

	InitGL();
	InitializePhysX();

	glutMainLoop();
}

