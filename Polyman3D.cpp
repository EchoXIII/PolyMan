using namespace std;
#include <Windows.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <conio.h>
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <gl/glut.h>
#include <array>


void init(void);
void RenderScene(void);//draw a function in a window
void SetupRC(void);//set the color and OpenGL system state
void loadpm(float[], float[], float[], float[], float[], float[], float[], float[], float[], float[], float[], float[], float[], float[], float[], float[]);//load Polyman
void loadpw(float[], float[], float[], float[], float[], float[], float[], float[], float[], float[], float[], float[], float[], float[], float[], float[]);//load Polywoman
void loadsm(float[], float[], float[], float[], float[], float[], float[], float[], float[], float[], float[], float[], float[], float[]);//load Squareman
void loadbackground(float[], float[], float[], float[], float[], float[], float[], float[], float[], float[], float[], float[]); //load Polyland
void drawpm(float[], float[], int, float, float, float, float);//draw polybodies
void drawfoot(float[], float[], float);//draw polyfeets
void draweye(float[], float[]);//draw poleye
void drawswd(float[], float[], float, float, float);
void drawground(float[], float[], int, float, float, float);//draw polyground
void drawlines(float[], float[], int, int);
void drawlightning(void);

void settranspm(void);//set Polyman transformation matrix
void settransff(void);//set Polyman front foot matrix
void settransrf(void);//set Polyman rear foot matrix
void settranspw(void);//set Polywoman transformation matrix
void settranspwff(void);//set Polywoman front foot matrix
void settranspwrf(void);//set Polywoman rear foot matrix
void settranssq(void);//set Squareman transformation matrix
void settranssqff(void);//set Squareman front foot matrix
void settranssqrf(void);//set Squareman rear foot matrix
void settransswd(void);//set sword matrix
void settransgrnd(void);//set polyground matrix
void settranscldlft(int);
void settranscldrgt(int);
void settranslight(int);
void settransrain(void);

void TimerFunction(int); //called every 30ms
void drawString(float, float, char *);


float theta = 0.0; //angular rotation value polyman
float dx = 11.0, dy = -5.0, dz = 0.0; //movement values for Polyman body
float dxff = 11.0, dyff = -5.0;//movement values for Polyman front foot
float dxrf = 11.0, dyrf = -5.0;//ovement values for Polyman rear foot
float thetaw = 0.0;//polywoman rotation
float pwx = -11.0, pwy = -5.0;//movement values for Polywoman body
float pwxff = -11.0, pwyff = -5.0;// movement values for Polywoman front foot
float pwxrf = -11.0, pwyrf = -5.0;//movement values for Polywoman rear foot
float thetas = 0.0;//squareman rotation
float sx = -13.0, sy = -5.0;//movement values for Squareman body
float sxff = -13.0, syff = -5.0;// movement values for Squareman front foot
float sxrf = -13.0, syrf = -5.0;//movement values for Squareman rear foot
float swdx = -13.0, swdy = -5.0;//movement values for sword
float thetaswd = 0.0;//rotation for sword
float cldxl = -11.0;//movement for clouds
float cldxr = 13.5;//movement for clouds

float lightningx = 0.0;
float lightningy = 6.5;
float ry = 7.0;
float rx = 0.0;

bool closed = true;
bool closedw = true;
bool talkw = false;
bool turnpw = true;
bool closedsq = true;
bool talksq = false;
bool turnsq = false;
bool dead = false;
bool storm = false;
bool striking = false;


int talk = 0;
int countrf = 1, countff = 1;
int frame = 1;

int main(int argc, char* *argv) {
	char header[] = "Polyman by Jacob Blessing";
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);//set up single buffer with RGB
	//initialize window size and position
	glutInitWindowSize(1080, 1080);
	glutInitWindowPosition(460, 0);//set position and size of window


	glutCreateWindow(header);
	SetupRC();
	glutDisplayFunc(RenderScene);//point to scene drawing f(x)
	glutTimerFunc(30, TimerFunction, 1);

	glutMainLoop();//execute

	return 0;
}//end of main


void SetupRC(void) {
	//set the clear color of the window
	glClearColor(0.0f, 0.7f, 1.0f, 0.0f);

	return;
}//end of SetupRC

void RenderScene(void) {
	float xdel = 0.25;
	float px[5], py[5];//holds points and lines for Polyman top
	float bpx[5], bpy[5];//holds points and lines for Polyman bottom
	float bopx[7], bopy[7];//mouth open
	float ffpx[3], ffpy[3];//polyfrontfoot
	float rfpx[3], rfpy[3];//polyrearfoot
	float epx[1], epy[1];//polyeye
	float depx[4], depy[4];//dead polyeye
	float bldx[12], bldy[12];


	float pwx[5], pwy[5];//holds points and lines for Polywoman top
	float bpwx[5], bpwy[5];//holds points and lines for Polywoman bottom
	float bopwx[7], bopwy[7];//mouth open
	float ffpwx[3], ffpwy[3];//polyfrontfoot
	float rfpwx[3], rfpwy[3];//polyrearfoot
	float epwx[1], epwy[1];//polyeye
	float pwcrx[8], pwcry[8];//polyhair
	float bowx[1], bowy[1];//polybows


	float sx[4], sy[4];//Squareman top
	float bsx[4], bsy[4];//Sqaureman bottom
	float bosx[6], bosy[6];//Squareman mouth
	float ffsx[3], ffsy[3];//Squareman ff
	float rfsx[3], rfsy[3];//Squareman rf 
	float esx[1], esy[1];//Squareman eye
	float swsx[5], swsy[5];//Squareman sword

	float gpx[5], gpy[5];//polyground
	float cpx[7], cpy[7];//polyclouds
	float skyx[5], skyy[5];//polysky
	float rx1[22], ry1[22];//polyrain 1
	float rx2[22], ry2[22];//polyrain 2
	float lgtx[10], lgty[10];//polylightning
	cout << "in renderscene" << endl; //clear window

	glColor3f(0.0, 1.0, 0.0);//set outline color
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, 1080, 1080); //same as window size
	glOrtho(-11.0, 11.0, -11.0, 11.0, 11.0, -11.0);
	loadpm(px, py, bpx, bpy, bopx, bopy, ffpx, ffpy, rfpx, rfpy, epx, epy, depx, depy, bldx, bldy);//load of Polyman
	loadpw(pwx, pwy, bpwx, bpwy, bopwx, bopwy, ffpwx, ffpwy, rfpwx, rfpwy, epwx, epwy, pwcrx, pwcry, bowx, bowy);
	loadsm(sx, sy, bsx, bsy, bosx, bosy, ffsx, ffsy, rfsx, rfsy, esx, esy, swsx, swsy);
	loadbackground(gpx, gpy, cpx, cpy, skyx, skyy, rx1, ry1, rx2, ry2, lgtx, lgty);

	glClear(GL_COLOR_BUFFER_BIT);//clear with background color
	//polyland and dialogue render
	settransgrnd();
	drawground(gpx, gpy, 5, 0.1, 0.4, 0.0);
	if (storm) {
		drawground(skyx, skyy, 5, .05, .05, .05);
	}
	if (storm) {
		glColor3f(1.0, 1.0, 1.0);
		for (int i = 0; i < 11; i++) {
			settransrain();
			drawlines(rx1, ry1, 22, i);
			drawlines(rx2, ry2, 22, i);
		}
	}
	if (striking) {
		glColor3f(1.0, 1.0, 1.0);
		settranslight(lightningx);
		glLineWidth(3);
		drawlines(lgtx, lgty, 10, 0);
	}
	glLineWidth(1);
	for (int i = 0; i < 5; i++) {
		settranscldlft(i);
		drawground(cpx, cpy, 7, .2, .2, .2);
		settranscldrgt(i);
		drawground(cpx, cpy, 7, .2, .2, .2);
	}

	settransgrnd();
	char string1[] = "It's me ya boi!";
	char string2[] = "Polyman senpai!";
	char string3[] = "Squareman?!";
	char string4[] = "I'm bout to end your whole career!";
	char string5[] = "Omae wa mou...";
	char string6[] = "Shindeiru!";
	char string7[] = "NANI?!?!";
	char string8[] = "OH! Squareman bout that thug life!";
	char string9[] = "THE GODS WEEP FOR POLYMAN";
	glColor3f(0.0, 0.0, 0.0);
	if (talk == 1)drawString(-1.0, -3.5, string1);
	if (talk == 2)drawString(-8.0, -3.5, string2);
	if (talk == 3)drawString(-1.0, -3.5, string3);
	if (talk == 4)drawString(-1.0, -3.5, string4);
	if (talk == 5)drawString(-4.0, -3.5, string5);
	if (talk == 6)drawString(-4.0, -3.5, string6);
	if (talk == 7)drawString(-1.0, -3.5, string7);
	if (talk == 8)drawString(-8.0, -3.5, string8);

	//polywoman render
	glColor3f(0.0, 0.0, 0.0);
	settranspwrf();
	if (turnpw) glRotatef(180, 0.0, 1.0, 0.0);
	drawfoot(rfpwx, rfpwy, -.5);
	glColor3f(0.0, 0.5, 0.0);
	settranspw();
	if (turnpw) glRotatef(180, 0.0, 1.0, 0.0);
	drawpm(pwx, pwy, 4, 0.0, 0.5, 0.0, 0.5);
	if (closedw) {
		drawpm(bpwx, bpwy, 4, 0.0, 0.5, 0.0, 0.5);
	}
	if (!closedw) {
		drawpm(bopwx, bopwy, 6, 0.0, 0.5, 0.0, 0.5);
	}
	glColor3f(0.5, 0.0, 0.5);
	drawlines(pwcrx, pwcry, 8, 0);
	glPointSize(5);
	draweye(epwx, epwy);
	glColor3f(0.0, 0.0, 0.0);
	settranspwff();
	if (turnpw) glRotatef(180, 0.0, 1.0, 0.0);
	drawfoot(ffpwx, ffpwy, .5);

	//squareman render
	glColor3f(0.0, 0.0, 0.0);
	settranssqrf();
	if (turnsq) glRotatef(180, 0.0, 1.0, 0.0);
	drawfoot(rfsx, rfsy, -.5);
	glColor3f(0.3, 0.3, 0.3);
	settranssq();
	if (turnsq) glRotatef(180, 0.0, 1.0, 0.0);
	drawpm(sx, sy, 3, 0.3, 0.3, 0.3, 0.5);
	if (closedsq) {
		drawpm(bsx, bsy, 3, 0.3, 0.3, 0.3, 0.5);
	}
	if (!closedsq) {
		drawpm(bosx, bosy, 5, 0.3, 0.3, 0.3, 0.5);
	}
	glColor3f(1.0, 0.5, 0.0);
	draweye(esx, esy);
	glColor3f(0.0, 0.0, 0.0);
	settranssqff();
	if (turnsq) glRotatef(180, 0.0, 1.0, 0.0);
	drawfoot(ffsx, ffsy, .5);
	settransswd();
	if (turnsq) glRotatef(180, 0.0, 1.0, 0.0);
	drawswd(swsx, swsy, 0.0, 0.0, 0.0);

	//polyman render
	settransrf();
	drawfoot(rfpx, rfpy, -.5);//polyman rearfoot
	glColor3f(0.5, 0.0, 0.5);//polyman body color
	settranspm();//set up modelview matrix of Polyman
	drawpm(px, py, 4, 0.5, 0.0, 0.5, 0.5);//polyman draw
	if (closed) {
		drawpm(bpx, bpy, 4, 0.5, 0.0, 0.5, 0.5);//polyman mouth closed
	}
	if (!closed) {
		drawpm(bopx, bopy, 6, 0.5, 0.0, 0.5, 0.5);//polyman mouth open
	}
	glColor3f(0.0, 0.5, 0.0);//polyman eye color
	if (!dead) {
		glPointSize(5);
		draweye(epx, epy);
	}
	if (dead) {
		drawlines(depx, depy, 4, 0);
		glColor3f(1.0, 0.0, 0.0);
		glLineWidth(3);
		drawlines(bldx, bldy, 12, 0);
		glLineWidth(1);
	}
	glColor3f(0.0, 0.0, 0.0);
	settransff();
	drawfoot(ffpx, ffpy, .5);//polyman front foot

	glColor3f(1.0, 1.0, 1.0);
	if (talk == 9)drawString(-2.5, 13.0, string9);
	glEnd();
	glutSwapBuffers();
	return;
}//end RenderScene
void loadbackground(float gpx[], float gpy[], float cpx[], float cpy[], float skyx[], float skyy[], float rx1[], float ry1[], float rx2[], float ry2[], float lgtx[], float lgty[]) {
	gpx[0] = -11.0; gpy[0] = -5.0;
	gpx[1] = -11.0; gpy[1] = -11.0;
	gpx[2] = 11.0;  gpy[2] = -11.0;
	gpx[3] = 11.0;  gpy[3] = -5.0;
	gpx[4] = -11.0; gpy[4] = -5.0;

	cpx[0] = 0;    cpy[0] = 11;
	cpx[1] = 0;    cpy[1] = 6.5;
	cpx[2] = -.5;  cpy[2] = 6;
	cpx[3] = -2;;  cpy[3] = 6;
	cpx[4] = -2.5; cpy[4] = 6.5;
	cpx[5] = -2.5; cpy[5] = 11;
	cpx[6] = 0;    cpy[6] = 11;

	skyx[0] = -11.0; skyy[0] = -5.0;
	skyx[1] = -11.0; skyy[1] = 11.0;
	skyx[2] = 11.0;  skyy[2] = 11.0;
	skyx[3] = 11.0;  skyy[3] = -5.0;
	skyx[4] = -11.0; skyy[4] = -5.0;

	rx1[0] = -10.5; ry1[0] = 0.0;
	rx1[1] = -10.5; ry1[1] = 0.5;
	rx1[2] = -8.5;  ry1[2] = 0.0;
	rx1[3] = -8.5;  ry1[3] = 0.5;
	rx1[4] = -6.5;  ry1[4] = 0.0;
	rx1[5] = -6.5;  ry1[5] = 0.5;
	rx1[6] = -4.5;  ry1[6] = 0.0;
	rx1[7] = -4.5;  ry1[7] = 0.5;
	rx1[8] = -2.5;  ry1[8] = 0.0;
	rx1[9] = -2.5;  ry1[9] = 0.5;
	rx1[10] = -0.5;  ry1[10] = 0.0;
	rx1[11] = -0.5;  ry1[11] = 0.5;
	rx1[12] = 1.5;   ry1[12] = 0.0;
	rx1[13] = 1.5;   ry1[13] = 0.5;
	rx1[14] = 3.5;   ry1[14] = 0.0;
	rx1[15] = 3.5;   ry1[15] = 0.5;
	rx1[16] = 5.5;   ry1[16] = 0.0;
	rx1[17] = 5.5;   ry1[17] = 0.5;
	rx1[18] = 7.5;   ry1[18] = 0.0;
	rx1[19] = 7.5;   ry1[19] = 0.5;
	rx1[20] = 9.5;   ry1[20] = 0.0;
	rx1[21] = 9.5;   ry1[21] = 0.5;

	rx2[0] = -10; ry2[0] = 1.0;
	rx2[1] = -10; ry2[1] = 1.5;
	rx2[2] = -8;  ry2[2] = 1.0;
	rx2[3] = -8;  ry2[3] = 1.5;
	rx2[4] = -6;  ry2[4] = 1.0;
	rx2[5] = -6;  ry2[5] = 1.5;
	rx2[6] = -4;  ry2[6] = 1.0;
	rx2[7] = -4;  ry2[7] = 1.5;
	rx2[8] = -2;  ry2[8] = 1.0;
	rx2[9] = -2;  ry2[9] = 1.5;
	rx2[10] = 0;  ry2[10] = 1.0;
	rx2[11] = 0;  ry2[11] = 1.5;
	rx2[12] = 2;   ry2[12] = 1.0;
	rx2[13] = 2;   ry2[13] = 1.5;
	rx2[14] = 4;   ry2[14] = 1.0;
	rx2[15] = 4;   ry2[15] = 1.5;
	rx2[16] = 6;   ry2[16] = 1.0;
	rx2[17] = 6;   ry2[17] = 1.5;
	rx2[18] = 8;   ry2[18] = 1.0;
	rx2[19] = 8;   ry2[19] = 1.5;
	rx2[20] = 10;   ry2[20] = 1.0;
	rx2[21] = 10;   ry2[21] = 1.5;

	lgtx[0] = 0.0; lgty[0] = 6.5;
	lgtx[1] = -0.5; lgty[1] = 5.5;
	lgtx[2] = -0.5; lgty[2] = 5.5;
	lgtx[3] = .5; lgty[3] = 5.0;
	lgtx[4] = .5; lgty[4] = 5.0;
	lgtx[5] = 0.0; lgty[5] = 3.5;
	lgtx[6] = 0.0; lgty[6] = 3.5;
	lgtx[7] = 1.0; lgty[7] = 3.0;
	lgtx[8] = 1.0; lgty[8] = 3.0;
	lgtx[9] = .5; lgty[9] = 1.5;

	return;
}
void loadpm(float px[], float py[], float bpx[], float bpy[], float bopx[], float bopy[], float ffpx[], float ffpy[], float rfpx[], float rfpy[], float epx[], float epy[], float depx[], float depy[], float bldx[], float bldy[]) {


	px[0] = -1.125; py[0] = 0.0;
	px[1] = -.625;  py[1] = .75;
	px[2] = .625;   py[2] = .75;
	px[3] = 1.125;  py[3] = 0.0;
	px[4] = -1.125; py[4] = 0.0;

	bpx[0] = -1.125; bpy[0] = 0.0;
	bpx[1] = -.625;  bpy[1] = -.75;
	bpx[2] = .625;   bpy[2] = -.75;
	bpx[3] = 1.125;  bpy[3] = 0.0;
	bpx[4] = -1.125; bpy[4] = 0.0;

	bopx[0] = -1.125; bopy[0] = 0.0;
	bopx[1] = -.375; bopy[1] = 0.0;
	bopx[2] = -.875; bopy[2] = -.5;
	bopx[3] = -.625; bopy[3] = -.75;
	bopx[4] = .625;  bopy[4] = -.75;
	bopx[5] = 1.125; bopy[5] = 0.0;
	bopx[6] = -.375; bopy[6] = 0.0;

	ffpx[0] = -.25; ffpy[0] = -.5;
	ffpx[1] = -.25; ffpy[1] = -1;
	ffpx[2] = -.5;  ffpy[2] = -1;

	rfpx[0] = .25; rfpy[0] = -.5;
	rfpx[1] = .25; rfpy[1] = -1;
	rfpx[2] = 0.0;  rfpy[2] = -1;

	epx[0] = -.5; epy[0] = .5;

	depx[0] = -.65; depy[0] = .65;
	depx[1] = -.35; depy[1] = .35;
	depx[2] = -.65; depy[2] = .35;
	depx[3] = -.35; depy[3] = .65;

	bldx[0] = -.675;  bldy[0] = .75;
	bldx[1] = -.675;  bldy[1] = .5;
	bldx[2] = -1.125;    bldy[2] = 0;
	bldx[3] = -1.125;    bldy[3] = -.25;
	bldx[4] = 1.125;     bldy[4] = 0;
	bldx[5] = 1.125;     bldy[5] = -.25;
	bldx[6] = -.675;  bldy[6] = -.75;
	bldx[7] = -.675;  bldy[7] = -1;
	bldx[8] = .675;   bldy[8] = -.75;
	bldx[9] = .675;   bldy[9] = -1;
	bldx[10] = .675; bldy[10] = .75;
	bldx[11] = .675; bldy[11] = .5;
	return;

}//end of loadicon

void loadpw(float pwx[], float pwy[], float bpwx[], float bpwy[], float bopwx[], float bopwy[], float ffpwx[], float ffpwy[], float rfpwx[], float rfpwy[], float epwx[], float epwy[],
	float pwcrx[], float pwcry[], float bowx[], float bowy[]) {


	pwx[0] = -1.125; pwy[0] = 0.0;
	pwx[1] = -.625;  pwy[1] = .75;
	pwx[2] = .625;   pwy[2] = .75;
	pwx[3] = 1.125;  pwy[3] = 0.0;
	pwx[4] = -1.125; pwy[4] = 0.0;

	bpwx[0] = -1.125; bpwy[0] = 0.0;
	bpwx[1] = -.625;  bpwy[1] = -.75;
	bpwx[2] = .625;   bpwy[2] = -.75;
	bpwx[3] = 1.125;  bpwy[3] = 0.0;
	bpwx[4] = -1.125; bpwy[4] = 0.0;

	bopwx[0] = -1.125; bopwy[0] = 0.0;
	bopwx[1] = -.375; bopwy[1] = 0.0;
	bopwx[2] = -.875; bopwy[2] = -.5;
	bopwx[3] = -.625; bopwy[3] = -.75;
	bopwx[4] = .625;  bopwy[4] = -.75;
	bopwx[5] = 1.125; bopwy[5] = 0.0;
	bopwx[6] = -.375; bopwy[6] = 0.0;

	ffpwx[0] = -.25; ffpwy[0] = -.5;
	ffpwx[1] = -.25; ffpwy[1] = -1;
	ffpwx[2] = -.5;  ffpwy[2] = -1;

	rfpwx[0] = .25; rfpwy[0] = -.5;
	rfpwx[1] = .25; rfpwy[1] = -1;
	rfpwx[2] = 0.0; rfpwy[2] = -1;

	epwx[0] = -.5; epwy[0] = .5;

	pwcrx[0] = 0;    pwcry[0] = .75;
	pwcrx[1] = 0;    pwcry[1] = .5;
	pwcrx[2] = -.25; pwcry[2] = .75;
	pwcrx[3] = -.25; pwcry[3] = .5;
	pwcrx[4] = .25;  pwcry[4] = .75;
	pwcrx[5] = .25;  pwcry[5] = .5;
	pwcrx[6] = .5;   pwcry[6] = .75;
	pwcrx[7] = .5;   pwcry[7] = .5;

	bowx[0] = 0; bowy[0] = .375;


	return;

}//end of loadicon

void loadsm(float sx[], float sy[], float bsx[], float bsy[], float bosx[], float bosy[], float ffsx[], float ffsy[], float rfsx[], float rfsy[], float esx[], float esy[], float swsx[], float swsy[]) {
	sx[0] = 1;  sy[0] = 0;
	sx[1] = 0;  sy[1] = 1;
	sx[2] = -1; sy[2] = 0;
	sx[3] = 1;  sy[3] = 0;

	bsx[0] = -1; bsy[0] = 0;
	bsx[1] = 0;  bsy[1] = -1;
	bsx[2] = 1;  bsy[2] = 0;
	bsx[3] = -1; bsy[3] = 0;

	bosx[0] = -1;  bosy[0] = 0;
	bosx[1] = 0;   bosy[1] = -1;
	bosx[2] = .5;  bosy[2] = -.5;
	bosx[3] = .25; bosy[3] = -.25;
	bosx[4] = 1;   bosy[4] = 0;
	bosx[5] = -1;  bosy[5] = 0;

	ffsx[0] = .25; ffsy[0] = -.5;
	ffsx[1] = .25; ffsy[1] = -1.25;
	ffsx[2] = .5;  ffsy[2] = -1.25;

	rfsx[0] = -.25; rfsy[0] = -.5;
	rfsx[1] = -.25; rfsy[1] = -1.25;
	rfsx[2] = 0;    rfsy[2] = -1.25;

	esx[0] = .25; esy[0] = .5;

	swsx[0] = -.25; swsy[0] = -.25;
	swsx[1] = 1.25; swsy[1] = 1.25;
	swsx[2] = .75;    swsy[2] = 1;
	swsx[3] = 1;    swsy[3] = .75;
	swsx[4] = 1.25; swsy[4] = 1.25;
}
void drawString(float x, float y, char *string) {
	glRasterPos2f(x, y);

	for (char* c = string; *c != '\0'; c++) {
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *c);  // Updates the position
	}
}

void drawfoot(float pxp[], float pyp[], float z) {
	cout << "in drawfoot" << endl;
	glBegin(GL_LINES);
	glVertex3f(pxp[0], pyp[0], z);
	glVertex3f(pxp[1], pyp[1], z);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(pxp[1], pyp[1], z);
	glVertex3f(pxp[2], pyp[2], z);
	glEnd();
	glFlush();
	return;
}//end drawfoot


void drawlines(float pwcrx[], float pwcry[], int length, int j) {
	cout << "in drawlines" << endl;
	glBegin(GL_LINES);
	int k = 0;
	if (j > 0)k = j;
	for (int i = 0; i < length; i++) {
		glVertex2f(pwcrx[i], pwcry[i] + j + k);
	}
	glEnd();
	glFlush();
	return;
}//end drawlines

void draweye(float pxp[], float pyp[]) {
	cout << "in draweye" << endl;
	glBegin(GL_POINTS);
	glVertex3f(pxp[0], pyp[0], .5);
	glEnd();
	glBegin(GL_POINTS);
	glVertex3f(pxp[0], pyp[0], -.5);
	glEnd();
	glFlush();
	return;
}//end draweye

void drawpm(float pxp[], float pyp[], int j, float r, float g, float b, float z) {
	int i;
	cout << "in drawpm" << endl;
	glColor3f(r, g, b);
	glBegin(GL_LINE_STRIP);
	glVertex2f(pxp[0], pyp[0]);
	for (i = 0; i < j - 1; i++) {
		glVertex3f(pxp[i], pyp[i], z);
	}
	glEnd();

	glBegin(GL_LINE_STRIP);
	glVertex2f(pxp[0], pyp[0]);
	for (i = 0; i < j - 1; i++) {
		glVertex3f(pxp[i], pyp[i], z - 1);
	}
	glEnd();


	//glVertex2f(pxp[0], pyp[0]);
	for (i = 0; i < j - 1; i++) {
		glBegin(GL_LINE_STRIP);
		glVertex3f(pxp[i], pyp[i], z - 1);
		glVertex3f(pxp[i], pyp[i], z);
		if (i < j - 1)glVertex3f(pxp[i + 1], pyp[i + 1], z);
		else glVertex3f(pxp[0], pyp[0], z);
		if (i < j - 1)glVertex3f(pxp[i + 1], pyp[i + 1], z - 1);
		else glVertex3f(pxp[0], pyp[0], z - 1);
		glVertex3f(pxp[i], pyp[i], z - 1);
		glEnd();
	}



	glFlush();

	/*glColor3f(r, g, b);
	glShadeModel(GL_FLAT);
	glBegin(GL_POLYGON);
	for (i = 0; i < j; i++) {
		glVertex3f(pxp[i], pyp[i], z);
	}

	glEnd();
	glBegin(GL_POLYGON);
	for (i = 0; i < j; i++) {
		glVertex3f(pxp[i], pyp[i], z-1);
	}

	glEnd();
	glFlush();*/
	return;

}//end drawpm

void drawswd(float pxp[], float pyp[], float r, float g, float b) {
	int i;
	cout << "in drawswd" << endl;
	glBegin(GL_LINE_STRIP);
	glVertex2f(pxp[0], pyp[0]);
	for (i = 0; i <= sizeof(pxp); i++) {
		glVertex2f(pxp[i], pyp[i]);
	}
	glEnd();

	glFlush();

	glColor3f(r, g, b);
	glShadeModel(GL_FLAT);
	glBegin(GL_POLYGON);
	for (i = 1; i <= sizeof(pxp); i++) {
		glVertex2f(pxp[i], pyp[i]);
	}

	glEnd();
	glFlush();
	return;

}//end drawsm

void drawground(float pxp[], float pyp[], int length, float r, float g, float b) {
	int i;
	cout << "in drawground" << endl;
	/*glBegin(GL_LINE_STRIP);
	glVertex2f(pxp[0], pyp[0]);
	for (i = 0; i <= sizeof(pxp); i++) {
		glVertex2f(pxp[i], pyp[i]);
	}
	glEnd();
	glFlush();*/

	glColor3f(r, g, b);
	glShadeModel(GL_FLAT);
	glBegin(GL_POLYGON);
	for (i = 0; i < length; i++) {
		glVertex3f(pxp[i], pyp[i], 11);
	}
	for (i = 0; i < length; i++) {
		glVertex3f(pxp[i], pyp[i], -11);
	}
	glEnd();
	glFlush();
	return;

}//end drawground

void drawlightning(void) {
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINES);
	glVertex2f(-1, 5.5);
	glVertex2f(0, 6.5);
}

void settranspm(void) {
	cout << "in settranspm" << endl;
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(dx, dy, dz);
	glRotatef(theta, 0.0, 0.0, 1.0);
	glRotatef(-15, 0.0, 1.0, 0.0);
	return;
}//settranspm

void settransff(void) {
	cout << "in settransff" << endl;
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(dxff, dyff, 0.0);
	glRotatef(theta, 0.0, 0.0, 1.0);
	glRotatef(-15, 0.0, 1.0, 0.0);
	return;
}//settransff
void settransrf(void) {
	cout << "in settransrf" << endl;
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(dxrf, dyrf, 0.0);
	glRotatef(theta, 0.0, 0.0, 1.0);
	glRotatef(-15, 0.0, 1.0, 0.0);
	return;
}//settransrf

void settranspw(void) {
	cout << "in settranspm" << endl;
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(pwx, pwy, 0.0);
	glRotatef(thetaw, 0.0, 0.0, 1.0);
	glRotatef(-15, 0.0, 1.0, 0.0);
	return;
}//settranspw
void settranspwff(void) {
	cout << "in settransff" << endl;
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(pwxff, pwyff, 0.0);
	glRotatef(0.0, 0.0, 0.0, 1.0);
	glRotatef(-15, 0.0, 1.0, 0.0);
	return;
}//settranspwff
void settranspwrf(void) {
	cout << "in settransrf" << endl;
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(pwxrf, pwyrf, 0.0);
	glRotatef(0.0, 0.0, 0.0, 1.0);
	glRotatef(-15, 0.0, 1.0, 0.0);
	return;
}//settranspwrf

void settranssq(void) {
	cout << "in settranspm" << endl;
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(sx, sy, 0.0);
	glRotatef(thetas, 0.0, 0.0, 1.0);
	glRotatef(-15, 0.0, 1.0, 0.0);
	return;
}//settranssq
void settranssqff(void) {
	cout << "in settransff" << endl;
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(sxff, syff, 0.0);
	glRotatef(thetas, 0.0, 0.0, 1.0);
	glRotatef(-15, 0.0, 1.0, 0.0);
	return;
}//settranssqff
void settranssqrf(void) {
	cout << "in settransrf" << endl;
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(sxrf, syrf, 0.0);
	glRotatef(thetas, 0.0, 0.0, 1.0);
	glRotatef(-15, 0.0, 1.0, 0.0);
	return;
}//settranssqrf
void settransswd(void) {
	cout << "in settransrf" << endl;
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(swdx, swdy, 0.0);
	glRotatef(thetaswd, 0.0, 0.0, 1.0);
	glRotatef(-15, 0.0, 1.0, 0.0);
	return;
}//settransswd

void settransgrnd(void) {
	cout << "in settransgrnd" << endl;
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, 0.0);
	glRotatef(0.0, 0.0, 0.0, 1.0);
	return;
}//settransgrnd
void settranscldlft(int i) {
	cout << "in settransgrnd" << endl;
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(cldxl - 2.5*i, 0.0, 0.0);
	glRotatef(0.0, 0.0, 0.0, 1.0);
	return;
}//settranscldlft
void settranscldrgt(int i) {
	cout << "in settransgrnd" << endl;
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(cldxr + 2.5*i, 0.0, 0.0);
	glRotatef(0.0, 0.0, 0.0, 1.0);
	return;
}//settranscldrgt
void settranslight(int i) {
	cout << "in settranslight" << endl;
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(i, 0.0, 0.0);
	glRotatef(0.0, 0.0, 0.0, 1.0);
	return;
}//settranslight
void settransrain() {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(rx, ry, 0.0);
	glRotatef(0.0, 0.0, 0.0, 1.0);
	return;
}//settransrain


void TimerFunction(int value) {
	switch (frame) {
	case 1: //enter polyman and polywoman
		if (pwx < -7.0) {
			pwx += 0.1;
			if (countrf < 3) {
				pwxrf += .2;
				pwyrf += .15;
			}
			else if (countrf == 3) {
				pwxrf += .2;
			}
			else if (countrf < 6) {
				pwxrf += .2;
				pwyrf -= .15;
			}
			else if (countrf < 10) {
			}
			else if (countrf == 10) {
			}

			if (pwx > -10.5) {
				if (countff < 3) {
					pwxff += .2;
					pwyff += .15;
				}
				else if (countff == 3) {
					pwxff += .2;
				}
				else if (countff < 6) {
					pwxff += .2;
					pwyff -= .15;
				}
				else if (countff < 10) {
				}
				else if (countff == 10) {
				}
			}
		}
		if (pwx >= -7.0) {
			pwx = -7.0;
			pwxff = pwx;
			pwxrf = pwx;
			pwyff = pwy;
			pwyrf = pwy;
		}

		dx -= 0.1;
		if (countrf < 3) {
			dxrf -= .2;
			dyrf += .15;
			countrf++;
		}
		else if (countrf == 3) {
			dxrf -= .2;
			countrf++;
		}
		else if (countrf < 6) {
			dxrf -= .2;
			dyrf -= .15;
			countrf += 1;
		}
		else if (countrf < 10) {
			countrf++;
		}
		else if (countrf == 10) {
			countrf = 1;
		}

		if (dx < 10.5) {
			if (countff < 3) {
				dxff -= .2;
				dyff += .15;
				countff++;
			}
			else if (countff == 3) {
				dxff -= .2;
				countff++;
			}
			else if (countff < 6) {
				dxff -= .2;
				dyff -= .15;
				countff += 1;
			}
			else if (countff < 10) {
				countff++;
			}
			else if (countff == 10) {
				countff = 1;
			}
		}
		else dxff -= .05;
		if (dx <= 0.0) {
			countrf = 1;
			countff = 1;
			dx = 0.0;
			dxff = dx;
			dxrf = dx;
			dyff = dy;
			dyrf = dy;
			frame = 2;
		}

		break;
	case 2: //polyman jumps up
		if (dy == -5.0 || countrf < 4) {
			dy -= .15;
			countrf++;
			break;
		}
		else if (dy < -5.2) {
			dy += .5;
			break;
		}
		theta -= 7.2;
		dy += 0.2;
		dyrf = dy;
		dyff = dy;
		if (dy > 5.0) {
			dy = 5.0;
			dyrf = 5.0;
			dyff = 5.0;
			frame = 3;
		}
		break;
	case 3://polyman comes back down
		theta -= 7.2;
		dy -= 0.2;
		dyrf -= .2;
		dyff -= .2;
		if (dy < -5.0) {
			dy = -5.0;
			dyrf = -5.0;
			dyff = -5.0;
			theta = 0.0;
			frame = 4;
		}
		countrf = 1;
		break;
	case 4://polyman lands and speaks
		dxff = dx + .2; //fix misposition of feet when landing
		dxrf = dx + .2;
		if (countrf < 64) {
			talk = 1;
			if (countrf % 16 < 8) {
				closed = false;
				countrf++;
				break;
			}
			else {
				closed = true;
				countrf++;
				break;
			}
		}
		talk = 0;
		closed = true;
		frame = 5;
		countff = 1;
		countrf = 1;
		break;
	case 5://polywoman replies
		talk = 2;
		closedw = false;
		if (thetaw > -45.0 & countrf == 1) {
			thetaw -= 2.5;
		}if (thetaw <= -45.0) countrf = 0;
		if (thetaw < 45.0 & countrf == 0) {
			thetaw += 2.5;
		}if (thetaw >= 45.0) countrf = 2;
		if (countrf == 2) {
			thetaw -= 2.5;
			if (thetaw <= 0.0) {
				countrf = 1;
				countff = 1;
				thetaw = 0.0;
				talk = 0;
				closedw = true;
				frame = 6;
			}
		}
		break;
	case 6://enter squareman
		sx += .1;
		swdx += .1;
		if (countrf < 3) {
			sxrf += .2;
			syrf += .15;
			countrf++;
		}
		else if (countrf == 3) {
			sxrf += .2;
			countrf++;
		}
		else if (countrf < 6) {
			sxrf += .2;
			syrf -= .15;
			countrf += 1;
		}
		else if (countrf < 10) {
			countrf++;
		}
		else if (countrf == 10) {
			countrf = 1;
		}

		if (sx > -12.5) {
			if (countff < 3) {
				sxff += .2;
				syff += .15;
				countff++;
			}
			else if (countff == 3) {
				sxff += .2;
				countff++;
			}
			else if (countff < 6) {
				sxff += .2;
				syff -= .15;
				countff += 1;
			}
			else if (countff < 10) {
				countff++;
			}
			else if (countff == 10) {
				countff = 1;
			}
		}
		else sxff += .05;
		if (sx >= -3.2) {
			countrf = 1;
			countff = 1;
			sx = -3.25;
			sxff = sx;
			sxrf = sx;
			syff = sy;
			syrf = sy;
			frame = 7;
		}
		break;
	case 7://talking shit
		if (countrf < 32) {
			talk = 3;
			if (countrf % 16 < 8) {
				closed = false;
				countrf++;
				break;
			}
			else {
				closed = true;
				countrf++;
				break;
			}
		}
		if (countrf >= 32 & countrf < 160) {
			talk = 4;
			if (countrf % 16 < 8) {
				closed = false;
				countrf++;
				break;
			}
			else {
				closed = true;
				countrf++;
				break;
			}
		}

		countrf = 1;
		countff = 0;
		talk = 0;
		frame = 8;
	case 8://get ganked
		countff++;
		switch (countff) {
		case 1:
			if (thetaswd > -45) thetaswd -= 2.15;
			break;
		case 2:
			if (thetaswd > -45) thetaswd -= 2.15;
			break;
		case 3:
			if (thetaswd > -45) thetaswd -= 2.15;
			break;
		case 4:
			if (thetaswd > -45) thetaswd -= 2.15;
			break;
		case 5:
			if (thetaswd > -45) thetaswd -= 2.15;
			break;
		case 6:
			if (thetaswd > -45) thetaswd -= 2.15;
			break;
		case 7:
			if (thetaswd > -45) thetaswd -= 2.15;
			break;
		case 8:
			if (thetaswd > -45) thetaswd -= 2.15;
			break;
		case 9:
			if (thetaswd > -45) thetaswd -= 2.15;
			break;
		case 10:
			if (thetaswd > -45) thetaswd -= 2.15;
			break;
		case 11:
			if (thetaswd > -45) thetaswd -= 2.15;
			break;
		case 12:
			if (thetaswd > -45) thetaswd -= 2.15;
			break;
		case 13:
			if (thetaswd > -45) thetaswd -= 2.15;
			break;
		case 14:
			if (thetaswd > -45) thetaswd -= 2.15;
			break;
		case 15:
			if (thetaswd > -45) thetaswd -= 2.15;
			break;
		case 16:
			if (thetaswd > -45) thetaswd -= 2.15;
			break;
		case 17:
			if (thetaswd > -45) thetaswd -= 2.15;
			break;
		case 18:
			if (thetaswd > -45) thetaswd -= 2.15;
			break;
		case 19:
			if (thetaswd > -45) thetaswd -= 2.15;
			break;
		case 20:
			if (thetaswd > -45) thetaswd -= 2.15;
			break;
		case 21:
			if (thetaswd > -45) thetaswd -= 2.15;
			if (thetaswd = -45) swdx += .75;
			break;
		case 26:
			turnsq = true;
			thetaswd = 45;
			swdx = 3.2;
			sxff = 3.2;
			sxrf = 3.2;
			sx = 3.2;
			break;
		case 31:
			swdx = 2.45;
			break;
		case 36:
			turnsq = false;
			thetaswd = -90;
			thetas = 0;
			swdx = -2.0;
			sxff = -2.0;
			sxrf = -2.0;
			sx = -2.0;
			swdy = -3.0;
			syff = -3.0;
			syrf = -3.0;
			sy = -3.0;
			break;
		case 41:
			swdy -= .25;
			swdx += .25;
			break;
		case 46:
			turnsq = true;
			thetaswd = 0;
			thetas = 0;
			swdx = 2.0;
			sxff = 2.0;
			sxrf = 2.0;
			sx = 2.0;
			swdy = -7.0;
			syff = -7.0;
			syrf = -7.0;
			sy = -7.0;
			break;
		case 51:
			swdy += .25;
			swdx -= .25;
			break;
		case 56:
			turnsq = true;
			thetaswd = 90;
			thetas = 0;
			swdx = 2.0;
			sxff = 2.0;
			sxrf = 2.0;
			sx = 2.0;
			swdy = -3.0;
			syff = -3.0;
			syrf = -3.0;
			sy = -3.0;
			break;
		case 61:
			swdy -= .25;
			swdx -= .25;
			break;
		case 66:
			turnsq = false;
			thetaswd = 0;
			thetas = 0;
			swdx = -2.0;
			sxff = -2.0;
			sxrf = -2.0;
			sx = -2.0;
			swdy = -7.0;
			syff = -7.0;
			syrf = -7.0;
			sy = -7.0;
			break;
		case 71:
			swdy += .25;
			swdx += .25;
			break;
		case 76:
			turnsq = false;
			thetaswd = -45;
			swdx = -3.2;
			sxff = -3.2;
			sxrf = -3.2;
			sx = -3.2;
			swdy = -5;
			syff = -5;
			syrf = -5;
			sy = -5;
			break;
		}
		if (countff <= 76) break;
		if (countff > 76 & countff < 86) {
			thetaswd += 5;
			break;
		}
		frame = 9;
		countff = 1;
	case 9://talking shit again
		if (countrf < 64) {
			talk = 5;
			if (countrf % 16 < 8) {
				closedsq = false;
				countrf++;
				break;
			}
			else {
				closedsq = true;
				countrf++;
				break;
			}
		}
		if (countrf >= 64 & countrf < 112) {
			talk = 6;
			if (countrf % 16 < 8) {
				closedsq = false;
				countrf++;
				break;
			}
			else {
				closedsq = true;
				countrf++;
				break;
			}
		}
		if (countrf >= 112 & countrf < 134) {
			talk = 7;
			if (countrf % 16 < 8) {
				closed = false;
				countrf++;
				break;
			}
			else {
				closed = true;
				countrf++;
				break;
			}
		}if (countrf <= 175) {
			closed = false;
			countrf++;
			break;
		}
		frame = 10;
		countrf = 1;
	case 10://bleed and die
		dead = true;
		if (dy > -5.3) {
			dy -= .025;
			break;
		}
		if (countrf < 112) {
			talk = 8;
			if (countrf % 16 < 8) {
				closedw = false;
				countrf++;
				break;
			}
			else {
				closedw = true;
				countrf++;
				break;
			}
		}
		turnsq = true;
		turnpw = false;
		talk = 0;
		countrf = 1;
		frame = 11;
		break;
	case 11:
		pwx -= 0.1;
		if (countrf < 3) {
			pwxrf -= .2;
			pwyrf += .15;
			countrf++;
		}
		else if (countrf == 3) {
			pwxrf -= .2;
			countrf++;
		}
		else if (countrf < 6) {
			pwxrf -= .2;
			pwyrf -= .15;
			countrf++;
		}
		else if (countrf < 10) {
			countrf++;
		}
		else if (countrf == 10) {
			countrf = 1;
		}

		if (pwx < -7.5) {
			if (countff < 3) {
				pwxff -= .2;
				pwyff += .15;
				countff++;
			}
			else if (countff == 3) {
				pwxff -= .2;
				countff++;
			}
			else if (countff < 6) {
				pwxff -= .2;
				pwyff -= .15;
				countff++;
			}
			else if (countff < 10) {
				countff++;
			}
			else if (countff == 10) {
				countff = 1;
			}
		}
		else (pwxff -= .1);
		sx -= .1;
		swdx -= .1;
		if (countrf < 3) {
			sxrf -= .2;
			syrf += .15;
		}
		else if (countrf == 3) {
			sxrf -= .2;
		}
		else if (countrf < 6) {
			sxrf -= .2;
			syrf -= .15;
		}
		else if (countrf < 10) {
		}
		else if (countrf == 10) {
		}

		if (sx <= -3.7) {
			if (countff < 3) {
				sxff -= .2;
				syff += .15;
			}
			else if (countff == 3) {
				sxff -= .2;
			}
			else if (countff < 6) {
				sxff -= .2;
				syff -= .15;
			}
			else if (countff < 10) {
			}
			else if (countff == 10) {
			}
		}
		else(sxff -= .1);
		if (sx <= -13.0) {
			frame = 12;
			countrf = 1;
			countff = 1;
			break;
		}
		break;
	case 12:
		if (storm) {
			talk = 9;
			lightningx = -10 + rand() % 20;
			if (countrf % 50 != 0)countrf++;
			if (countrf % 50 == 0) {

				striking = true;
				countff++;
				if (countff % 30 == 0) {
					countrf = 1;
					countff = 1;
					striking = false;
				}
			}
			if (ry > -6.0) {
				ry -= .1;
				//	rx = -.2;
				break;
			}
			else {
				ry += 2.0;
				//	rx = .2;
				break;
			}

		}
		if (cldxl >= 0) storm = true;
		if (cldxl < 0) cldxl += .1;
		if (cldxr > 2.5) cldxr -= .1;
		break;
	}
	glutPostRedisplay();
	glutTimerFunc(30, TimerFunction, 1);
}//end TimerFunction