using namespace std;
#include<Windows.h>
#include<GL/glut.h>
#include<stdlib.h>
#include<math.h>
#include<conio.h>
#include<stdio.h>
#include <iostream>
#include <iomanip>
#include <GL/glut.h>


void init(void);
void RenderScene(void);//draw a function in a window
void SetupRC(void);//set the color and OpenGL system state
void ChangeSize(int, int);//set window size

float cxy;//global constant from initial window size and each redraw
float xl = -11.0, xr = 11.0, yb = -30, yt = 50.0;//the graph will go to 10 in each x direction and 50 up, 30 down
int xw = 900, yh = 900;//window size
float pi = 3.1459;

int main(int argc, char* *argv) {
	char header[] = "graph of three functions by Jacob Blessing";
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);//set up single buffer with RGB
	cxy = ((xr - xl) / (float)xw) / ((yt - yb) / (float)yh);//create initial user units to pixels constant
	//initialize window size and position
	cout << "calc cxy" << cxy << endl;
	glutInitWindowSize(900, 900);
	glutInitWindowPosition(20, 20);

	glutCreateWindow(header);
	glutDisplayFunc(RenderScene);//point to scene drawing f(x)
	SetupRC();
	glutReshapeFunc(ChangeSize);

	glutMainLoop();//execute

	return 0;
}//end of main


void SetupRC(void) {
	//set the clear color of the window
	glClearColor(0.3f, 0.3f, 0.3f, 0.3f);
	return;
}//end of SetupRC

void RenderScene(void) {
	float x, y, xdel = 0.25;
	cout << "in render scene" << endl;
	//clear the window
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(0.0, 0.0, 0.0);
	cout << xw << " " << yh << " " << endl;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, xw, yh);



	cout << "Ortho" << xl << "" << xr << " " << yb << " " << yt << endl;

	glOrtho(xl, xr, yb, yt, -1.0, 1.0);
	//draw both axis and put tic marks
	glBegin(GL_LINES);
	glVertex2f(-10.0, 0.0);//x left
	glVertex2f(10.0, 0.0);//x right

	for (x = -10.0; x < 10.0; x += 1.0) {//tic marks
		glVertex2f(x, 0.0);
		glVertex2f(x, 0.5);

	}
	//y axis endpoints
	glVertex2f(0, -30.0);
	glVertex2f(0, 50);

	for (y = -50.0; y < 50.0; y += 1.0) {//tic marks
		glVertex2f(-0.15, y);
		glVertex2f(0.15, y);

	};

	glEnd();
	//start draw functions
	glBegin(GL_LINE_STRIP);
	glColor3f(0.7, 0.0, 0.7);
	for (x = -10.0; x <= 10; x += xdel) {
		y = x * x*x + x * x - 20 * x;
		glVertex2f(x, y);
	}

	glEnd();
	glFlush();//clear buffers
	glBegin(GL_LINE_STRIP);
	glColor3f(0.0, 0.7, 0.0);
	for (x = -2 * pi; x <= 2 * pi; x += xdel) {
		y = 10 * cos(x) + 3;
		glVertex2f(x, y);
	}

	glEnd();
	glFlush();//clear buffers
	glBegin(GL_LINE_STRIP);
	glColor3f(0.0, 0.0, 0.7);
	for (x = -10.0; x <= 10; x += xdel) {
		y = (x*x*x + x * x - 20 * x) - (10 * cos(x) + 3);
		glVertex2f(x, y);
	}

	glEnd();
	glFlush();//clear buffers
	return;
}//end RenderScene

void ChangeSize(int w, int h) {
	float ytp, xrp, wf, hf;
	cout << "in changeSize" << w << "  " << h << endl;
	if (h == 0) h = 1;
	wf = w; hf = h;
	//set Viewport to entire window
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, w, h);
	if (w <= h) {
		ytp = ((xr - xl) * hf) / (wf*cxy) + yb;
		cout << "ortho" << xl << "  " << xr << "  " << yb << "  " << ytp << endl;
		glOrtho(xl, xr, yb, ytp, -1.0, 1.0);
	}
	else {
		xrp = (yt - yb)*cxy*wf / hf + xl;
		cout << "ortho" << xl << "  " << xrp << "  " << yb << "  " << yt << endl;
		glOrtho(xl, xrp, yb, yt, -1.0, 1.0);
	}
	return;

}//end of ChangeSize
