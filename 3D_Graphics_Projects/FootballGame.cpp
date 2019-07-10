#include "include/freeglut.h"
#include <Windows.h>
#include <iostream>
#include <math.h>
#include <string>
#include <cmath>
#include "cw2_15330190.h"
#define _USE_MATH_DEFINES

using namespace std;

//The TGAHEADER struct was imported from Tutorial 10
#pragma pack(1)
typedef struct
{
	GLbyte	identsize;
	GLbyte	colorMapType;
	GLbyte	imageType;
	unsigned short	colorMapStart;
	unsigned short	colorMapLength;
	unsigned char 	colorMapBits;
	unsigned short	xstart;
	unsigned short	ystart;
	unsigned short	width;
	unsigned short	height;
	GLbyte	bits;
	GLbyte	descriptor;
} TGAHEADER;

//The *gltLoadTGA code block was imported from Tutorial 10
#pragma pack(8)
GLbyte *gltLoadTGA(const char *szFileName, GLint *cordinateWidth, GLint *cordinateHeight, GLint *cordinateCo, GLenum *textureFormat)
{
	FILE *pFile;
	TGAHEADER tgaHeader;
	unsigned long lImageSize;
	short sDepth;
	GLbyte	*pBits = NULL;

	*cordinateWidth = 0;
	*cordinateHeight = 0;
	*textureFormat = GL_BGR_EXT;
	*cordinateCo = GL_RGB8;

	fopen_s(&pFile, szFileName, "rb");

	if (pFile == NULL)
		return NULL;

	fread(&tgaHeader, 18, 1, pFile);


	*cordinateWidth = tgaHeader.width;
	*cordinateHeight = tgaHeader.height;
	sDepth = tgaHeader.bits / 8;

	if (tgaHeader.bits != 8 && tgaHeader.bits != 24 && tgaHeader.bits != 32)
		return NULL;

	lImageSize = tgaHeader.width * tgaHeader.height * sDepth;
	pBits = (GLbyte*)malloc(lImageSize * sizeof(GLbyte));
	if (pBits == NULL)
		return NULL;

	if (fread(pBits, lImageSize, 1, pFile) != 1)
	{
		free(pBits);
		return NULL;
	}

	switch (sDepth)
	{
	case 3:
		*textureFormat = GL_BGR_EXT;
		*cordinateCo = GL_RGB8;
		break;
	case 4:
		*textureFormat = GL_BGRA_EXT;
		*cordinateCo = GL_RGBA8;
		break;
	case 1:
		*textureFormat = GL_LUMINANCE;
		*cordinateCo = GL_LUMINANCE8;
		break;
	};


	fclose(pFile);
	return pBits;
}

//Window Resolutions
GLsizei ww = 1280;
GLsizei wh = 720;

//Statistic variables
int scorePoints = 0;
int numberOfShots = 10;
int numberOfHoops = 8;

//Enables shot animation
bool playShot = false;

//Enables/Disables ball camera perspective
bool cam = false;

//Axis Coordinates for ball
GLfloat ballXCoord = 0;
GLfloat ballYCoord = 0;
GLfloat ballZCoord = 0;

//Predefined variables for equation r = st
GLfloat speed = 1000;
GLfloat time = 0;
GLfloat radial;

//Variables to define projection of ball & rotation of ball
float theta;
float pi;
GLfloat ballRotation = 0;

//Variable for skybox rotation
GLfloat skyboxRot = 0;

//Variables for loading textures
char* pointer;
GLbyte *imgPointer;
GLint iWidth;
GLint iHeight;
GLint iComponent;
GLenum eFormat;
GLfloat imgParams[] = { 0.1, 0.0, 0.1, 1.0 };
GLfloat textureParams[] = { 0.0, 0.1, 0.1, 1.0 };

//Variables for adjusting position of sliding bars
double adjdrawDirectionalMeter = 105;
double adjdrawHeightMeter = wh - 105;

//Variables for lighting
GLfloat light[] = { 0.9, 0.9, 0.9, 1 };
GLfloat	backgroundSpotlight[] = { 20, 50, -100, 1 };
GLfloat	ballSpotlight[] = { 0 + ballXCoord, -11 + ballYCoord, 30 + ballZCoord, 1 };

//Variables for hoop target positions
GLfloat hoopTarget0XCoord = -37;
GLfloat hoopTarget1XCoord = -30;
GLfloat hoopTarget2XCoord = -10;
GLfloat hoopTarget3XCoord = -2;
GLfloat hoopTarget4XCoord = 10;
GLfloat hoopTarget5XCoord = 20;
GLfloat hoopTarget6XCoord = 20;
GLfloat hoopTarget7XCoord = 35;
GLfloat hoopTarget0YCoord = -10;
GLfloat hoopTarget1YCoord = 10;
GLfloat hoopTarget2YCoord = 16;
GLfloat hoopTarget3YCoord = -8;
GLfloat hoopTarget4YCoord = 13;
GLfloat hoopTarget5YCoord = -7;
GLfloat hoopTarget6YCoord = 10;
GLfloat hoopTarget7YCoord = 5;
GLfloat hoopTargetZCoord = -54.9;

//Variables for flower positions
GLfloat orangeFlowerY1 = -13;
GLfloat orangeFlowerY2 = -5;
GLfloat orangeFlowerZ = -54.8;

//Variables for initialising textures for hoops and saving them to an array
char* redTarget = "C:/Users/Syedu/Documents/3D Graphics Programming Tutorial Solutions/Tutorial/textures/targetRed.tga";
char* blueTarget = "C:/Users/Syedu/Documents/3D Graphics Programming Tutorial Solutions/Tutorial/textures/targetBlue.tga";
char* dullTarget = "C:/Users/Syedu/Documents/3D Graphics Programming Tutorial Solutions/Tutorial/textures/targetDull.tga";
char* greenTarget = "C:/Users/Syedu/Documents/3D Graphics Programming Tutorial Solutions/Tutorial/textures/targetGreen.tga";
char* hoopTargets[8] = { redTarget, greenTarget, redTarget, blueTarget, blueTarget, greenTarget, redTarget, redTarget };

//Variables for initialising textures for all other components of the game
char* ball = "C:/Users/Syedu/Documents/3D Graphics Programming Tutorial Solutions/Tutorial/textures/FootballCompleteMap.tga";
char* hMeter = "C:/Users/Syedu/Documents/3D Graphics Programming Tutorial Solutions/Tutorial/textures/fillBarVerticalR.tga";
char* dMeter = "C:/Users/Syedu/Documents/3D Graphics Programming Tutorial Solutions/Tutorial/textures/fillBarHorizontal.tga";
char* grass = "C:/Users/Syedu/Documents/3D Graphics Programming Tutorial Solutions/Tutorial/textures/grass_diff.tga";
char* brickWall = "C:/Users/Syedu/Documents/3D Graphics Programming Tutorial Solutions/Tutorial/textures/old_wall_texture_TGA.tga";
char* orangeFlower = "C:/Users/Syedu/Documents/3D Graphics Programming Tutorial Solutions/Tutorial/textures/orangeFlowerFinal5.tga";
char* palmTree = "C:/Users/Syedu/Documents/3D Graphics Programming Tutorial Solutions/Tutorial/textures/palmBranchA.tga";
char* skyboxBackground = "C:/Users/Syedu/Documents/3D Graphics Programming Tutorial Solutions/Tutorial/textures/stormydays_large.tga";

//This method is executed when the window is resized or moved
//This has been used in all tutorial exercises
void reshape(int w, int h)
{
	//Prevents a mathematical error of division by zero
	if (h == 0) {
		h = 1;
	}
	glMatrixMode(GL_PROJECTION);
	glViewport(0, 0, w, h);
	glLoadIdentity();

	//Aspect Ratio
	if (w <= h)
	{
		glOrtho(0, ww, 0, wh*h / w, 1, -1);
	}
	else
	{
		glOrtho(0, ww*w / h, 0, wh, 1, -1);
	}
	gluPerspective(30, (GLfloat)w / (GLfloat)h, 1.0, 600);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

//Sets the Orthographic view, for the sliders and score indicator
//This is predefined from Appendix C of coursework specification
void setOrthographicProjection() {
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, ww, 0, wh);
	glScalef(1, -1, 1);
	glTranslatef(0, -wh, 0);
	glMatrixMode(GL_MODELVIEW);
}

//Resets the Perspective Projection
//This is predefined from Appendix C of coursework specification
void resetPerspectiveProjection() {
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

//Shows the default perspective of the game or the ball camera perspective
void cameraPerspective() {
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	if (cam == true) {
		//Ball Camera Perspective
		gluPerspective(30, ((float)ww / (float)wh), 0.1, 250);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		gluLookAt(ballXCoord, -5 + ballYCoord, 80 + ballZCoord, ballXCoord, ballYCoord, ballZCoord, 0, 1, 0);
	}
	else {
		//Normal game perspective
		gluPerspective(30, ((float)ww / (float)wh), 45, 600);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		gluLookAt(0, 0, 100, 0, 0, 0, 0, 1, 0);
	}
}

//Function that draws the 3 walls along with the textures
void drawWalls() {
	//Retreives the brick wall texture from the file path and loads it
	pointer = brickWall;
	imgPointer = gltLoadTGA(pointer, &iWidth, &iHeight, &iComponent, &eFormat);
	glTexImage2D(GL_TEXTURE_2D, 0, iComponent, iWidth, iHeight, 0, eFormat, GL_UNSIGNED_BYTE, imgPointer);
	free(imgPointer);

	//Generates a set of texture coordinates
	//This processes and loads the textures for the walls
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glEnable(GL_TEXTURE_GEN_S);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glEnable(GL_TEXTURE_GEN_T);

	GLfloat wallX = 40;
	GLfloat wallY1 = -15;
	GLfloat wallY2 = 20;
	GLfloat wallZ1 = -55;
	GLfloat wallZ2 = 20;

	//Displays the walls in a quad with the defined coordinates
	glBegin(GL_QUADS);
	glVertex3f(wallX, wallY1, wallZ1);
	glVertex3f(-wallX, wallY1, wallZ1);
	glVertex3f(-wallX, wallY2, wallZ1);
	glVertex3f(wallX, wallY2, wallZ1);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(-wallX, wallY2, wallZ2);
	glVertex3f(-wallX, wallY2, wallZ1);
	glVertex3f(-wallX, wallY1, wallZ1);
	glVertex3f(-wallX, wallY1, wallZ2);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(wallX, wallY2, wallZ1);
	glVertex3f(wallX, wallY2, wallZ2);
	glVertex3f(wallX, wallY1, wallZ2);
	glVertex3f(wallX, wallY1, wallZ1);
	glEnd();

	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
}

//Function that draws the ground along with the grass texture
void drawGround() {
	//Retreives the grass texture from the file path and loads it
	pointer = grass;
	imgPointer = gltLoadTGA(pointer, &iWidth, &iHeight, &iComponent, &eFormat);
	glTexImage2D(GL_TEXTURE_2D, 0, iComponent, iWidth, iHeight, 0, eFormat, GL_UNSIGNED_BYTE, imgPointer);
	free(imgPointer);

	//Generates a set of texture coordinates within the range of the array
	//This processes and loads the textures for the ground
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGenfv(GL_S, GL_OBJECT_PLANE, imgParams);
	glEnable(GL_TEXTURE_GEN_S);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGenfv(GL_T, GL_OBJECT_PLANE, textureParams);
	glEnable(GL_TEXTURE_GEN_T);

	//Displays the ground in a flat quad with the defined coordinates
	GLfloat groundX = 45;
	GLfloat groundY = -13;
	GLfloat groundZ = 60;
	glBegin(GL_QUADS);
	glVertex3f(groundX, groundY, groundZ);
	glVertex3f(-groundX, groundY, groundZ);
	glVertex3f(-groundX, groundY, -groundZ);
	glVertex3f(groundX, groundY, -groundZ);
	glEnd();

	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
}

//Function that draws the football along with the texture
void drawFootball() {
	//Retreives the football texture from the file path and loads it
	pointer = ball;
	imgPointer = gltLoadTGA(pointer, &iWidth, &iHeight, &iComponent, &eFormat);
	glTexImage2D(GL_TEXTURE_2D, 0, iComponent, iWidth, iHeight, 0, eFormat, GL_UNSIGNED_BYTE, imgPointer);
	free(imgPointer);

	//Wraps the texture on to the ball
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

	GLUquadricObj *ballQuadric = gluNewQuadric();
	//Applies the texture into the quadric with the wrap mode set to fill
	gluQuadricDrawStyle(ballQuadric, GLU_FILL);
	gluQuadricTexture(ballQuadric, true);
	//Positions the ball on the flat ground
	glTranslatef(0 + ballXCoord, -11.0 + ballYCoord, 30 + ballZCoord);
	glRotatef(ballRotation, 1, 0, 0);
	//Applies the quadric with the texture to the sphere
	gluSphere(ballQuadric, 2, 30, 15);
	gluDeleteQuadric(ballQuadric);
}

//Function that draws the quad skybox along with the textures
void drawSkybox() {
	//Retreives the skybox texture from the file path and loads it
	pointer = skyboxBackground;
	imgPointer = gltLoadTGA(pointer, &iWidth, &iHeight, &iComponent, &eFormat);
	glTexImage2D(GL_TEXTURE_2D, 0, iComponent, iWidth, iHeight, 0, eFormat, GL_UNSIGNED_BYTE, imgPointer);
	free(imgPointer);

	//The dimensions of the skybox
	//Changing these will affect the size and position of the skybox
	GLfloat x = 80;
	GLfloat y = 40;
	GLfloat z = 200;

	//Using a 6 face skybox with 6 quads 
	//Top Face
	glRotatef(skyboxRot, 0, 1, 0);
	glBegin(GL_QUADS);
	glTexCoord2f(0.5, 0.67);
	glVertex3f(x, y, -z);
	glTexCoord2f(0.25, 0.67);
	glVertex3f(-x, y, -z);
	glTexCoord2f(0.25, 1);
	glVertex3f(-x, y, z);
	glTexCoord2f(0.5, 1);
	glVertex3f(x, y, z);
	glEnd();

	//Back face
	glBegin(GL_QUADS);
	glTexCoord2f(0.5, 0.67);
	glVertex3f(x, y, -z);
	glTexCoord2f(0.25, 0.67);
	glVertex3f(-x, y, -z);
	glTexCoord2f(0.25, 0.33);
	glVertex3f(-x, -y, -z);
	glTexCoord2f(0.5, 0.33);
	glVertex3f(x, -y, -z);
	glEnd();

	//Right face
	glBegin(GL_QUADS);
	glTexCoord2f(0.5, 0.67);
	glVertex3f(x, y, -z);
	glTexCoord2f(0.75, 0.67);
	glVertex3f(x, y, z);
	glTexCoord2f(0.75, 0.33);
	glVertex3f(x, -y, z);
	glTexCoord2f(0.5, 0.33);
	glVertex3f(x, -y, -z);
	glEnd();

	//Left face
	glBegin(GL_QUADS);
	glTexCoord2f(0.25, 0.67);
	glVertex3f(-x, y, -z);
	glTexCoord2f(0, 0.67);
	glVertex3f(-x, y, z);
	glTexCoord2f(0, 0.33);
	glVertex3f(-x, -y, z);
	glTexCoord2f(0.25, 0.33);
	glVertex3f(-x, -y, -z);
	glEnd();

	//Front face
	glBegin(GL_QUADS);
	glTexCoord2f(0.75, 0.67);
	glVertex3f(x, y, z);
	glTexCoord2f(1, 0.67);
	glVertex3f(-x, y, z);
	glTexCoord2f(1, 0.33);
	glVertex3f(-x, -y, z);
	glTexCoord2f(0.75, 0.33);
	glVertex3f(x, -y, z);
	glEnd();

	//Bottom face
	glBegin(GL_QUADS);
	glTexCoord2f(0.5, 0.67);
	glVertex3f(x, -y, -z);
	glTexCoord2f(0.25, 0.67);
	glVertex3f(-x, -y, -z);
	glTexCoord2f(0.25, 1);
	glVertex3f(-x, -y, z);
	glTexCoord2f(0.5, 1);
	glVertex3f(x, -y, z);
	glEnd();
}

//Initialising the directional meter (left or right) with textures and shape
void drawDirectionalMeter() {
	//Retreives the meter texture from the file path and loads it
	glEnable(GL_TEXTURE_2D);
	pointer = dMeter;
	imgPointer = gltLoadTGA(pointer, &iWidth, &iHeight, &iComponent, &eFormat);
	glTexImage2D(GL_TEXTURE_2D, 0, iComponent, iWidth, iHeight, 0, eFormat, GL_UNSIGNED_BYTE, imgPointer);
	free(imgPointer);

	GLfloat directionalX1 = 15;
	GLfloat directionalX2 = 210;
	GLfloat directionalY1 = 30;
	GLfloat directionalY2 = 50;

	glBegin(GL_POLYGON);
	glTexCoord2f(0, 0);
	glVertex2d(directionalX1, directionalY1);
	glTexCoord2f(0, 1);
	glVertex2d(directionalX1, directionalY2);
	glTexCoord2f(1, 1);
	glVertex2d(directionalX2, directionalY2); // changes length of textured bar
	glTexCoord2f(1, 0);
	glVertex2d(directionalX2, directionalY1); // changes length of textured bar
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

//Initialising the height meter with textures and shape
void drawHeightMeter() {
	//Retreives the meter texture from the file path and loads it
	glEnable(GL_TEXTURE_2D);
	pointer = hMeter;
	imgPointer = gltLoadTGA(pointer, &iWidth, &iHeight, &iComponent, &eFormat);
	glTexImage2D(GL_TEXTURE_2D, 0, iComponent, iWidth, iHeight, 0, eFormat, GL_UNSIGNED_BYTE, imgPointer);
	free(imgPointer);

	GLfloat heightX1 = ww - 30;
	GLfloat heightX2 = ww - 50;
	GLfloat heightY1 = wh - 30;
	GLfloat heightY2 = wh - 210;

	glBegin(GL_POLYGON);
	glTexCoord2f(1, 0);
	glVertex2d(heightX1, heightY1);
	glTexCoord2f(0, 0);
	glVertex2d(heightX2, heightY1);
	glTexCoord2f(0, 1);
	glVertex2d(heightX2, heightY2); // changes height of textured bar
	glTexCoord2f(1, 1);
	glVertex2d(heightX1, heightY2); // changes height of textured bar
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

//Function that draws the hoops along with the textures
void drawHoops(char* pointer, double x, double y, double z, double in, double out, bool hoop) {
	glPushMatrix();
	//Loads the texture, sets the filter and wrap modes
	imgPointer = gltLoadTGA(pointer, &iWidth, &iHeight, &iComponent, &eFormat);
	glTexImage2D(GL_TEXTURE_2D, 0, iComponent, iWidth, iHeight, 0, eFormat, GL_UNSIGNED_BYTE, imgPointer);
	free(imgPointer);

	//Shapes the loop and positions it on the defined spot
	GLUquadricObj *quadricHoop = gluNewQuadric();
	gluQuadricDrawStyle(quadricHoop, GLU_FILL);
	gluQuadricNormals(quadricHoop, GLU_SMOOTH);
	gluQuadricOrientation(quadricHoop, GLU_OUTSIDE);
	gluQuadricTexture(quadricHoop, true);
	glTranslatef(x, y, z);
	GLfloat hoopRotation;
	//This is to differentiate between the hoops on the wall and
	//the hoop on the ground beneath the ball
	if (pointer == dullTarget && !hoop)
	{
		hoopRotation = 90;
	}
	else {
		hoopRotation = 0;
	}
	glRotatef(hoopRotation, 1, 0, 0); //This is to rotate the loop; only the loop under the ball rotates by 90 degrees
	gluDisk(quadricHoop, in, out, 30, 20);
	glPopMatrix();
	glPushMatrix();
	glDisable(GL_TEXTURE_2D); //Prevents colours of text being the same as the hoop colour

	double midX = x - 1;
	double midY = y - 0.75;
	glRasterPos3d(midX, midY, z); //Positions the points text in the middle of the hoop
	string pointsText;
	//Retrieves the texture from the file path via pointer and assigns it with the value of the points
	if (pointer == redTarget) {
		pointsText = "30";
	}
	else if (pointer == blueTarget) {
		pointsText = "20";
	}
	else if (pointer == greenTarget) {
		pointsText = "10";
	}
	const unsigned char* pointsTxt = reinterpret_cast<const unsigned char*>(pointsText.c_str());
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, pointsTxt); //Displays the points text on screen
	glEnable(GL_TEXTURE_2D); //Shows the texture of the hoops
	glPopMatrix();
}

//Function that draws the flowers along with the textures
//It can draw multiple flowers depending on value of the parameter
void drawFlowers(char* pointer, int numOfFlowers, float flowerX) {
	glPushMatrix();
	//Blends the texture with the components around it
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//Retreives the flower texture from the file path and loads it
	imgPointer = gltLoadTGA(pointer, &iWidth, &iHeight, &iComponent, &eFormat);
	glTexImage2D(GL_TEXTURE_2D, 0, iComponent, iWidth, iHeight, 0, eFormat, GL_UNSIGNED_BYTE, imgPointer);
	free(imgPointer);

	glBegin(GL_QUADS);
	glTexCoord2f(numOfFlowers, 0.05);
	glVertex3f(flowerX, orangeFlowerY1, orangeFlowerZ);
	glTexCoord2f(0, 0.05);
	glVertex3f(flowerX + (5 * numOfFlowers), orangeFlowerY1, orangeFlowerZ);
	glTexCoord2f(0, 1);
	glVertex3f(flowerX + (5 * numOfFlowers), orangeFlowerY2, orangeFlowerZ);
	glTexCoord2f(numOfFlowers, 1);
	glVertex3f(flowerX, orangeFlowerY2, orangeFlowerZ);
	glEnd();

	glDisable(GL_BLEND);
	glPopMatrix();
}

//Function that draws the trees along with the texture
void drawTrees() {
	glPushMatrix();
	//Blends the texture with the components around it
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//Retreives the tree texture from the file path and loads it
	pointer = palmTree;
	imgPointer = gltLoadTGA(pointer, &iWidth, &iHeight, &iComponent, &eFormat);
	glTexImage2D(GL_TEXTURE_2D, 0, iComponent, iWidth, iHeight, 0, eFormat, GL_UNSIGNED_BYTE, imgPointer);
	free(imgPointer);

	GLfloat wallX1 = 40;
	GLfloat wallX2 = 35;
	GLfloat wallY1 = -15;
	GLfloat wallY2 = 10;
	GLfloat wallZ1 = -30;
	//Draws trees on left wall
	for (int i = 0; i < 10; i++) {
		glBegin(GL_QUADS);
		glTexCoord2f(0, 0);
		glVertex3f(-wallX1, wallY1, wallZ1 + (5 * i));
		glTexCoord2f(1, 0);
		glVertex3f(-wallX2, wallY1, wallZ1 + (5 * i));
		glTexCoord2f(1, 1);
		glVertex3f(-wallX2, wallY2, wallZ1 + (5 * i));
		glTexCoord2f(0, 1);
		glVertex3f(-wallX1, wallY2, wallZ1 + (5 * i));
		glEnd();
	}

	glPopMatrix();
	glPushMatrix();

	//Draws trees on right wall
	for (int i = 0; i < 10; i++) {
		glBegin(GL_QUADS);
		glTexCoord2f(0, 0);
		glVertex3f(wallX1, wallY1, wallZ1 + (5 * i));
		glTexCoord2f(1, 0);
		glVertex3f(wallX2, wallY1, wallZ1 + (5 * i));
		glTexCoord2f(1, 1);
		glVertex3f(wallX2, wallY2, wallZ1 + (5 * i));
		glTexCoord2f(0, 1);
		glVertex3f(wallX1, wallY2, wallZ1 + (5 * i));
		glEnd();
	}

	glDisable(GL_BLEND);
	glPopMatrix();
}

//Combines all UI components (meters and text) 
void finalGameUI() {
	glColor3f(0.0, 0.0, 0.0); //Black score text
	glRasterPos2d(ww / 2 - 60, 50.0); //Positions text in the top middle of the window
	string text;
	if ((numberOfShots == 0) || (numberOfHoops = 0)) {
		text = "Game Over. Final Score: " + to_string(scorePoints);
	}
	else {
		text = "Score: " + to_string(scorePoints);
	}
	//defines the string text as const char*
	const unsigned char* txt = reinterpret_cast <const unsigned char*>(text.c_str());
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, txt);
	glColor3f(1.0, 1.0, 1.0); //Resets colour scheme to white
	drawDirectionalMeter();
	drawHeightMeter();
	glColor3f(1.0, 0.0, 0.0); //Red sliding bar for directional meter
	glRectd(adjdrawDirectionalMeter, 20.0, adjdrawDirectionalMeter + 5, 60.0);
	glColor3f(0.0, 0.0, 1.0); //Blue sliding bar for height meter
	glRectd(ww - 20.0, adjdrawHeightMeter, ww - 60.0, adjdrawHeightMeter + 5.0);
	glColor3f(1.0, 1.0, 1.0); //Resets colour scheme to white
	glPopMatrix();
}

//Combines all game components (plants, trees, football, skybox and hoops)
void finalGameScene() {
	glEnable(GL_TEXTURE_2D); //Enables the textures for all components
	glPushMatrix();
	//Loads the skybox in its own matrix
	drawSkybox();
	glPopMatrix();
	glPushMatrix();
	//Loads the 3 walls and the ground in the next matrix
	drawWalls();
	drawGround();
	glPopMatrix();
	//Loads the trees and flowers
	drawFlowers(orangeFlower, 4, -32);
	drawFlowers(orangeFlower, 2, 3);
	drawFlowers(orangeFlower, 3, 27);
	drawTrees();
	glPushMatrix();
	//Load the football in the next matrix
	drawFootball();
	glPopMatrix();
	//Loads the ball placement hoop
	drawHoops(dullTarget, 0, -12.9, 30, 0.9, 1.8, false);
	//Variables for the rings of the hoop
	GLfloat in30 = 2.1;
	GLfloat in20 = 3.1;
	GLfloat in10 = 4.1;
	GLfloat out30 = 2.7;
	GLfloat out20 = 3.8;
	GLfloat out10 = 5.1;
	//Loads the hoops 
	drawHoops(hoopTargets[0], hoopTarget0XCoord, hoopTarget0YCoord, hoopTargetZCoord, in30, out30, true);
	drawHoops(hoopTargets[1], hoopTarget1XCoord, hoopTarget1YCoord, hoopTargetZCoord, in10, out10, true);
	drawHoops(hoopTargets[2], hoopTarget2XCoord, hoopTarget2YCoord, hoopTargetZCoord, in30, out30, true);
	drawHoops(hoopTargets[3], hoopTarget3XCoord, hoopTarget3YCoord, hoopTargetZCoord, in20, out20, true);
	drawHoops(hoopTargets[4], hoopTarget4XCoord, hoopTarget4YCoord, hoopTargetZCoord, in20, out20, true);
	drawHoops(hoopTargets[5], hoopTarget5XCoord, hoopTarget5YCoord, hoopTargetZCoord, in10, out10, true);
	drawHoops(hoopTargets[6], hoopTarget6XCoord, hoopTarget6YCoord, hoopTargetZCoord, in30, out30, true);
	drawHoops(hoopTargets[7], hoopTarget7XCoord, hoopTarget7YCoord, hoopTargetZCoord, in30, out30, true);
	glDisable(GL_TEXTURE_2D);
	glPushMatrix();
	glLoadIdentity();
}

//This scoring system checks if the ball hits the hoops according to the x and y coordinates
void scoringSystem(GLfloat xCoord, GLfloat yCoord) {
	//The if statement has the condition of the ball hitting within the range of x and y coordinates of the hoop
	//and the hoop not being dull to prevent the user scoring when the ball hits a dull target

	//30 point hoop
	if (hoopTargets[0] != dullTarget && xCoord > -40 && xCoord < -34 && yCoord > -13 && yCoord < -7) {
		scorePoints = scorePoints + 30;
		numberOfHoops = numberOfHoops - 1;
		hoopTargets[0] = dullTarget;
	}
	//10 point hoop
	else if (hoopTargets[1] != dullTarget && xCoord > -35 && xCoord < -25 && yCoord > 5 && yCoord < 15) {
		scorePoints = scorePoints + 10;
		numberOfHoops = numberOfHoops - 1;
		hoopTargets[1] = dullTarget;
	}
	//30 point hoop
	else if (hoopTargets[2] != dullTarget && xCoord > -13 && xCoord < -7 && yCoord > 13 && yCoord < 19) {
		scorePoints = scorePoints + 30;
		numberOfHoops = numberOfHoops - 1;
		hoopTargets[2] = dullTarget;
	}
	//20 point hoop
	else if (hoopTargets[3] != dullTarget && xCoord > -5 && xCoord < 2 && yCoord > -11 && yCoord < -4) {
		scorePoints = scorePoints + 20;
		numberOfHoops = numberOfHoops - 1;
		hoopTargets[3] = dullTarget;
	}
	//20 point hoop
	else if (hoopTargets[4] != dullTarget && xCoord > 7 && xCoord < 14 && yCoord > 10 && yCoord < 17) {
		scorePoints = scorePoints + 20;
		numberOfHoops = numberOfHoops - 1;
		hoopTargets[4] = dullTarget;
	}
	//10 point hoop
	else if (hoopTargets[5] != dullTarget && xCoord > 15 && xCoord < 25 && yCoord > -12 && yCoord < -2) {
		scorePoints = scorePoints + 10;
		numberOfHoops = numberOfHoops - 1;
		hoopTargets[5] = dullTarget;
	}
	//30 point hoop
	else if (hoopTargets[6] != dullTarget && xCoord > 17 && xCoord < 23 && yCoord > 7 && yCoord < 13) {
		scorePoints = scorePoints + 30;
		numberOfHoops = numberOfHoops - 1;
		hoopTargets[6] = dullTarget;
	}
	//30 point hoop
	else if (hoopTargets[7] != dullTarget && xCoord > 32 && xCoord < 38 && yCoord > 2 && yCoord < 8) {
		scorePoints = scorePoints + 30;
		numberOfHoops = numberOfHoops - 1;
		hoopTargets[7] = dullTarget;
	}
}

//Arrow Key controls for adjusting sliding bars
void arrowKeys(int pressedKey, int x, int y) {
	//The if statements also have the condition of the shot not being taken
	//This is to prevent the ball moving left and right while the shot is taken
	if (!playShot)
	{
		switch (pressedKey)
		{
		case GLUT_KEY_LEFT:
			adjdrawDirectionalMeter = adjdrawDirectionalMeter - 1;
			break;
		case GLUT_KEY_RIGHT:
			adjdrawDirectionalMeter = adjdrawDirectionalMeter + 1;
			break;
		case GLUT_KEY_UP:
			adjdrawHeightMeter = adjdrawHeightMeter - 1;
			break;
		case GLUT_KEY_DOWN:
			adjdrawHeightMeter = adjdrawHeightMeter + 1;
			break;
		default:
			break;
		}
	}
	boundaryCheck();
}

//A check function for the sliding bars colliding with the boundary
void boundaryCheck()
{
	//Prevents sliding bars for going beyond textured bars
	if (adjdrawHeightMeter < (wh - 210.0)) {
		adjdrawHeightMeter = wh - 210.0;
	}
	else if (adjdrawHeightMeter > (wh - 30.0)) {
		adjdrawHeightMeter = wh - 30.0;
	}
	if (adjdrawDirectionalMeter > 205.0) {
		adjdrawDirectionalMeter = 205.0;
	}
	else if (adjdrawDirectionalMeter < 15.0) {
		adjdrawDirectionalMeter = 15.0;
	}
}

//Function for keyboard commands
void keyboardInput(unsigned char key, int x, int y) {
	switch (key) {
	case 'z':
		//Plays the shot animation
		//Uses the predefined calculations for pi and theta
		theta = (1 - atan2(85, ((adjdrawDirectionalMeter / 0.8) - 75)));
		pi = atan2(-((660 - adjdrawHeightMeter) / -2), 85);
		playShot = true;
		break;
	case 'f':
		//Changes the default perspective to ball camera perspective
		cam = true;
		break;
	case 'r':
		//Resets the game when there are no targets left or when they played all their shots
		if (numberOfHoops == 0 || numberOfShots == 0)
		{
			numberOfShots = 10;
			scorePoints = 0;
			hoopTargets[0] = redTarget;
			hoopTargets[1] = greenTarget;
			hoopTargets[2] = redTarget;
			hoopTargets[3] = blueTarget;
			hoopTargets[4] = blueTarget;
			hoopTargets[5] = greenTarget;
			hoopTargets[6] = redTarget;
			hoopTargets[7] = redTarget;
		}
		break;
	case 27:
		//Exits the game using Escape key
		exit(0);
		break;
	}
}

//Function that enables the skybox to rotate in the background
void skyboxRotation() {
	//The skybox will rotate slowly in the background
	skyboxRot = skyboxRot + 0.2;
	//Once the skybox completes a full rotation it will reset it back to zero and start again
	if (skyboxRot > 360) {
		skyboxRot = 0;
	}
}

//Function that enables the ball to rotate and move
void shotMotion() {
	//Ball motion will begin when user takes shot
	//Uses the calculations from Appendix A of cw specification
	time = time + 0.01;
	radial = speed * time;
	ballXCoord = radial * sin(theta)*cos(pi);
	ballYCoord = radial * sin(pi)*cos(theta);
	ballZCoord = -radial * cos(theta);
	//Prevents the ball going through the ground
	if (ballYCoord < 0) {
		ballYCoord = 0;
	}
	//Ball will rotate during shot animation
	ballRotation = ballRotation - 100;
	//Once the ball reaches past the back wall it will return to its default position
	resetPerShot();
}

//This function simply resets the ball position and some variables after
//a shot is taken
void resetPerShot()
{
	if (ballZCoord < -85) {
		scoringSystem(ballXCoord, ballYCoord - 11);
		ballXCoord = 0;
		ballYCoord = 0;
		ballZCoord = 0;
		ballRotation = 0;
		time = 0;
		numberOfShots = numberOfShots - 1;
		playShot = false;
		cam = false;
	}
}

//Timer function for the ball movement and skybox rotation
void gameTimer(int num)
{
	skyboxRotation();
	if (playShot)
	{
		shotMotion();
	}
	glutPostRedisplay();
	glutTimerFunc(35, gameTimer, 0);
}

//Sets up the lighting 
void lightingSetup() {
	glFrontFace(GL_CW);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light);
	glLightfv(GL_LIGHT0, GL_POSITION, ballSpotlight);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 60);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT1, GL_POSITION, backgroundSpotlight);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 60);
	glEnable(GL_LIGHT1);
	glEnable(GL_COLOR_MATERIAL);
}

//Method to render the scene per frame
void renderScene(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	cameraPerspective();
	glEnable(GL_DEPTH_TEST);
	glLightfv(GL_LIGHT0, GL_POSITION, ballSpotlight);
	glLightfv(GL_LIGHT1, GL_POSITION, backgroundSpotlight);
	finalGameScene();
	glDisable(GL_DEPTH_TEST);
	setOrthographicProjection();
	finalGameUI();
	glPopMatrix();
	resetPerspectiveProjection();
	glEnable(GL_LIGHTING);
	glutSwapBuffers();
}

//Main method
int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(ww, wh);
	glutCreateWindow("Football Game");
	glutSpecialFunc(arrowKeys);
	glutKeyboardFunc(keyboardInput);
	glutReshapeFunc(reshape);
	glutDisplayFunc(renderScene);
	glutTimerFunc(35, gameTimer, 0);
	lightingSetup();
	glutMainLoop();
	return 0;
}
