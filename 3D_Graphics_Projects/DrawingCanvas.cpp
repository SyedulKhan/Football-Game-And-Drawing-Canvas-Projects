#include "include\freeglut.h"
#include <fstream>
#include <cstdlib>
#include <vector>
#include <string>
#include <iostream>
#include <stdio.h>
#include <math.h>

#define LENGTH   20.0     
using namespace std;

//Global variables
//For rasterisation
GLint xRaster = 25, x = 30;
GLint k, j;
GLint i = 0;
//Window dimensions
GLsizei wh = 600, ww = 600;
//Viewport background colour
GLfloat red = 1.0, green = 1.0, blue = 1.0;
//Speeda for animation
GLfloat ySpeed = 20.0;
GLfloat ySpeed1 = 480.0;
GLfloat ySpeed2 = 480.0;
GLfloat ySpeed3 = 480.0;
GLfloat ySpeed4 = 480.0;
GLfloat ySpeed5 = 480.0;
GLfloat ySpeed6 = 480.0;
GLfloat ySpeed7 = 20.0;
GLfloat ySpeed8 = 20.0;
GLfloat ySpeed9 = 20.0;
GLfloat ySpeed10 = 20.0;
GLfloat ySpeed11 = 20.0;
//movement for key controls
GLfloat horizontalMovement = 0;
GLfloat verticalMovement = 0;
//maths functions for drawing circles
GLdouble PI = 3.14159;
GLfloat angle = 2.0 * PI;

//Global Booleans
bool grid = true;
bool anim = false;
bool del = false;
bool shapeSelected = false;
bool isDotButtonActivated = false;
bool isLineButtonActivated = false;
bool isRectLineButtonActivated = false;
bool isTriangleLineButtonActivated = false;
bool isTriangleFilledButtonActivated = false;
bool isSquareFilledButtonActivated = false;
bool isCircleFilledButtonActivated = false;
bool isInactiveBarsActivated = false;

//Vectors for each primitive and mouse clicks
vector<GLfloat> mouseClicks;
vector<GLfloat> dots;
vector<GLfloat> lines;
vector<GLfloat> triangles;
vector<GLfloat> triangleLines;
vector<GLfloat> rectangles;
vector<GLfloat> squares;
vector<GLfloat> circles;

//All functions in header
void drawRectangleFilled(GLfloat lengthX, GLfloat lengthY, GLfloat x, GLfloat y);
void drawRectangle(GLfloat lengthX, GLfloat lengthY, GLfloat x, GLfloat y);
void drawSquareFilled(GLfloat length, GLfloat x, GLfloat y);
void drawSquare(GLfloat length, GLfloat x, GLfloat y);
void drawAnimSquare(GLfloat length, GLfloat x, GLfloat y);
void drawCircle(GLfloat radius, GLfloat x, GLfloat y);
void dotButton();
void lineButton();
void rectLineButton();
void triangleLineButton();
void triangleFilledButton();
void squareFilledButton();
void circleFilledButton();
void inactiveBars();
void gridLines();
void shapesOnCanvas();
void drawShapes();
void pickPrimitive();
void animation();
void deleteAllShapes();

//Draws a bitmap character string.
//This method was created by the author Anastassia Angelopoulou 
void writeBitmapString(void *font, char *string)
{
	char *c;
	for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
}

//This is executed when the window is resized or moved 
void reshape(GLsizei w, GLsizei h)
{
	//Prevents an error when a number is divided by zero
	if (h == 0) {
		h = 1;
	}
	glMatrixMode(GL_PROJECTION);
	//Sets the Viewport according to the window dimensions
	glViewport(0, 0, w, h);
	//Resets the coordinate system
	glLoadIdentity();
	//Defines the aspect ratio of the window even when window is resized
	if (w <= h) 
	{
		glOrtho(0.0f, ww, 0.0f, wh*h / w, 1.0, -1.0);
	}
	else 
	{
		glOrtho(0.0f, ww*w / h, 0.0f, wh, 1.0, -1.0);
	}
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void display()
{
	//Clears the window
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(red, green, blue);

	//Plot and position text on screen - Title
	glColor3f(0.0, 0.0, 0.0);
	glRasterPos3f(225, 10, 0.0);
	writeBitmapString(GLUT_BITMAP_HELVETICA_18, "2D Drawing Canvas");

	//Determines if grid is shown on canvas or not
	if (grid)
	{
		gridLines();
	}

	//Determines if the animation is enabled on canvas or not
	if (anim)
	{
		animation();
	}

	//Determines if all shapes currently on canvas are deleted when user presses space bar
	if (del)
	{
		deleteAllShapes();
	}

	//Calls pickPrimitive() to determine which button is pressed
	pickPrimitive();

	//Draws shapes function
	drawShapes();

	//Keeps track of all shapes currently on the canvas
	shapesOnCanvas();

	//buttons on the vertical menu
	dotButton();
	lineButton();
	rectLineButton();
	triangleLineButton();

	//buttons on the horizontal menu
	triangleFilledButton();
	squareFilledButton();
	circleFilledButton();

	//both inactive bars
	inactiveBars();

	glutSwapBuffers();
	glFlush();
}

//checks which button in the menu, depending on x and y coordinate, is pressed using booleans to enable the button pressed
void pickPrimitive() {
	//the loop will keep checking which area the mouse is clicked on
	//if it clicks on the area where a button is positioned then the boolean for that function will be true
	//and hence it will allow the user to then draw that shape
	for (GLfloat j = 0; j < mouseClicks.size(); j = j + 2)
	{
		glVertex2i(mouseClicks[j], mouseClicks[j + 1]);
		if (mouseClicks[j] >= 0 && mouseClicks[j] <= 75 && mouseClicks[j + 1] < 525 && mouseClicks[j + 1] >= 450) {
			isDotButtonActivated = true;
			isLineButtonActivated = false;
			isRectLineButtonActivated = false;
			isTriangleLineButtonActivated = false;
			isTriangleFilledButtonActivated = false;
			isSquareFilledButtonActivated = false;
			isCircleFilledButtonActivated = false;
			isInactiveBarsActivated = false;
			shapeSelected = false;
		}
		else if (mouseClicks[j] >= 0 && mouseClicks[j] <= 75 && mouseClicks[j + 1] < 450 && mouseClicks[j + 1] >= 372) {
			isDotButtonActivated = false;
			isLineButtonActivated = true;
			isRectLineButtonActivated = false;
			isTriangleLineButtonActivated = false;
			isTriangleFilledButtonActivated = false;
			isSquareFilledButtonActivated = false;
			isCircleFilledButtonActivated = false;
			isInactiveBarsActivated = false;
			shapeSelected = false;
		}
		else if (mouseClicks[j] >= 0 && mouseClicks[j] <= 75 && mouseClicks[j + 1] < 372 && mouseClicks[j + 1] >= 300) {
			isDotButtonActivated = false;
			isLineButtonActivated = false;
			isRectLineButtonActivated = true;
			isTriangleLineButtonActivated = false;
			isTriangleFilledButtonActivated = false;
			isSquareFilledButtonActivated = false;
			isCircleFilledButtonActivated = false;
			isInactiveBarsActivated = false;
			shapeSelected = false;
		}
		else if (mouseClicks[j] >= 0 && mouseClicks[j] <= 75 && mouseClicks[j + 1] < 300 && mouseClicks[j + 1] >= 223) {
			isDotButtonActivated = false;
			isLineButtonActivated = false;
			isRectLineButtonActivated = false;
			isTriangleLineButtonActivated = true;
			isTriangleFilledButtonActivated = false;
			isSquareFilledButtonActivated = false;
			isCircleFilledButtonActivated = false;
			isInactiveBarsActivated = false;
			shapeSelected = false;
		}
		else if ((mouseClicks[j] >= 0 && mouseClicks[j] <= 75 && mouseClicks[j + 1] < 223 && mouseClicks[j + 1] >= 0) ||
			(mouseClicks[j] >= 310 && mouseClicks[j] <= 600 && mouseClicks[j + 1] < 625 && mouseClicks[j + 1] >= 500)) {
			isDotButtonActivated = false;
			isLineButtonActivated = false;
			isRectLineButtonActivated = false;
			isTriangleLineButtonActivated = false;
			isTriangleFilledButtonActivated = false;
			isSquareFilledButtonActivated = false;
			isCircleFilledButtonActivated = false;
			isInactiveBarsActivated = true;
			shapeSelected = false;
		}
		else if (mouseClicks[j] > 75 && mouseClicks[j] <= 160 && mouseClicks[j + 1] < 625 && mouseClicks[j + 1] >= 500) {
			isDotButtonActivated = false;
			isLineButtonActivated = false;
			isRectLineButtonActivated = false;
			isTriangleLineButtonActivated = false;
			isTriangleFilledButtonActivated = true;
			isSquareFilledButtonActivated = false;
			isCircleFilledButtonActivated = false;
			isInactiveBarsActivated = false;
			shapeSelected = false;
		}
		else if (mouseClicks[j] > 160 && mouseClicks[j] <= 235 && mouseClicks[j + 1] < 625 && mouseClicks[j + 1] >= 500) {
			isDotButtonActivated = false;
			isLineButtonActivated = false;
			isRectLineButtonActivated = false;
			isTriangleLineButtonActivated = false;
			isTriangleFilledButtonActivated = false;
			isSquareFilledButtonActivated = true;
			isCircleFilledButtonActivated = false;
			isInactiveBarsActivated = false;
			shapeSelected = false;
		}
		else if (mouseClicks[j] > 235 && mouseClicks[j] <= 310 && mouseClicks[j + 1] < 625 && mouseClicks[j + 1] >= 500) {
			isDotButtonActivated = false;
			isLineButtonActivated = false;
			isRectLineButtonActivated = false;
			isTriangleLineButtonActivated = false;
			isTriangleFilledButtonActivated = false;
			isSquareFilledButtonActivated = false;
			isCircleFilledButtonActivated = true;
			isInactiveBarsActivated = false;
			shapeSelected = false;
		}
	}
}

//Draws new shapes depending on which button is pressed
void drawShapes() {
	if (isDotButtonActivated)
	{
		for (GLfloat j = 0; j < dots.size(); j = j + 2) {
			glVertex2i(dots[j], dots[j + 1]);
			if (dots[j] > 80 && dots[j] < ww + 150 && dots[j + 1] < wh - 90 && dots[j + 1] > 80)
			{
				glPointSize(4.0);
				glColor3f(0.0f, 0.0f, 0.0f);
				glBegin(GL_POINTS);
				glVertex2f(dots[j], dots[j + 1]);
				glEnd();
			}
		}
	}
	else if (isLineButtonActivated) {
		for (GLfloat j = 0; j < lines.size(); j = j + 4) {
			glVertex2i(lines[j], lines[j + 1]);
			glVertex2i(lines[j], lines[j + 1]);
			if (lines[j] > 80 && lines[j] < ww + 150 && lines[j + 1] < wh - 90 && lines[j + 1] > 80)
			{
				glBegin(GL_LINE_LOOP);
				glColor3f(0.0, 0.0, 0.0);
				glVertex2f(lines[j], lines[j + 1]);
				glVertex2f(lines[j] + 100, lines[j + 1]);
				glEnd();
			}
		}
	}
	else if (isRectLineButtonActivated) {
		for (GLfloat j = 0; j < rectangles.size(); j = j + 2) {
			glVertex2i(rectangles[j], rectangles[j + 1]);
			if (rectangles[j] > 80 && rectangles[j] < ww + 150 && rectangles[j + 1] < wh - 90 && rectangles[j + 1] > 80)
			{
				glColor3f(0.0, 0.0, 0.0);
				drawRectangle(100.0, 50.0, rectangles[j], rectangles[j + 1]);
			}
		}
	}
	else if (isTriangleLineButtonActivated)
	{
		for (GLfloat j = 0; j < triangleLines.size(); j = j + 2) {
			glVertex2i(triangleLines[j], triangleLines[j + 1]);
			if (triangleLines[j] > 80 && triangleLines[j] < ww + 150 && triangleLines[j + 1] < wh - 90 && triangleLines[j + 1] > 80)
			{
				glBegin(GL_LINE_LOOP);
				glColor3f(0.0, 0.0, 0.0);
				glVertex2f(triangleLines[j] - LENGTH, triangleLines[j + 1] - LENGTH);
				glVertex2f(triangleLines[j] - LENGTH, triangleLines[j + 1] + LENGTH);
				glVertex2f(triangleLines[j] + LENGTH, triangleLines[j + 1] + LENGTH);
				glEnd();
			}
		}
	}
	else if (isTriangleFilledButtonActivated)
	{
		for (GLfloat j = 0; j < triangles.size(); j = j + 2) {
			glVertex2i(triangles[j], triangles[j + 1]);
			if (triangles[j] > 80 && triangles[j] < ww + 150 && triangles[j + 1] < wh - 90 && triangles[j + 1] > 80)
			{
				glBegin(GL_TRIANGLES);
				glColor3f(1.0, 0.0, 0.0);
				glVertex2f(triangles[j] - LENGTH, triangles[j + 1] - LENGTH);
				glVertex2f(triangles[j] - LENGTH, triangles[j + 1] + LENGTH);
				glVertex2f(triangles[j] + LENGTH, triangles[j + 1] + LENGTH);
				glEnd();
			}
		}
	}
	else if (isSquareFilledButtonActivated)
	{
		for (GLfloat j = 0; j < squares.size(); j = j + 2) {
			glVertex2i(squares[j], squares[j + 1]);
			if (squares[j] > 80 && squares[j] < ww + 150 && squares[j + 1] < wh - 90 && squares[j + 1] > 80)
			{
				glColor3f(1.0, 0.0, 0.0);
				drawSquareFilled(100.0, squares[j], squares[j + 1]);
			}
		}
	}
	else if (isCircleFilledButtonActivated)
	{
		for (GLfloat j = 0; j < circles.size(); j = j + 2) {
			glVertex2i(circles[j], circles[j + 1]);
			if (circles[j] > 80 && circles[j] < ww + 150 && circles[j + 1] < wh - 90 && circles[j + 1] > 80)
			{
				glColor3f(1.0, 0.0, 0.0);
				drawCircle(50.0, circles[j], circles[j + 1]);
			}
		}
	}
}

//Keeps track of all shapes currently on the canvas and lets it stay on the canvas
//unless deleted by user
void shapesOnCanvas() {

	for (GLfloat j = 0; j < dots.size(); j = j + 2) {
		glVertex2i(mouseClicks[j], mouseClicks[j + 1]);
		if (dots[j] > 80 && dots[j] < ww + 150 && dots[j + 1] < wh - 90 && dots[j + 1] > 80)
		{
			glPointSize(4.0);
			glColor3f(0.0f, 0.0f, 0.0f);
			glBegin(GL_POINTS);
			glVertex2f(dots[j] + horizontalMovement, dots[j + 1] + verticalMovement);
			glEnd();
		}

	}

	for (GLfloat j = 0; j < lines.size(); j = j + 2) {
		if (lines[j] > 80 && lines[j] < ww + 150 && lines[j + 1] < wh - 90 && lines[j + 1] > 80)
		{
			glBegin(GL_LINE_LOOP);
			glColor3f(0.0, 0.0, 0.0);
			glVertex2f(lines[j], lines[j + 1]);
			glVertex2f(lines[j] + 100, lines[j + 1]);
			glEnd();
		}
	}

	for (GLfloat j = 0; j < rectangles.size(); j = j + 2) {
		if (rectangles[j] > 80 && rectangles[j] < ww + 150 && rectangles[j + 1] < wh - 90 && rectangles[j + 1] > 80)
		{
			glColor3f(0.0, 0.0, 0.0);
			drawRectangle(100.0, 50.0, rectangles[j], rectangles[j + 1]);
		}
	}

	for (GLfloat j = 0; j < triangleLines.size(); j = j + 2) {
		if (triangleLines[j] > 80 && triangleLines[j] < ww + 150 && triangleLines[j + 1] < wh - 90 && triangleLines[j + 1] > 80)
		{
			glBegin(GL_LINE_LOOP);
			glColor3f(0.0, 0.0, 0.0);
			glVertex2f(triangleLines[j] - LENGTH, triangleLines[j + 1] - LENGTH);
			glVertex2f(triangleLines[j] - LENGTH, triangleLines[j + 1] + LENGTH);
			glVertex2f(triangleLines[j] + LENGTH, triangleLines[j + 1] + LENGTH);
			glEnd();
		}
	}

	for (GLfloat j = 0; j < triangles.size(); j = j + 2) {
		if (triangles[j] > 80 && triangles[j] < ww + 150 && triangles[j + 1] < wh - 90 && triangles[j + 1] > 80)
		{
			glBegin(GL_TRIANGLES);
			glColor3f(1.0, 0.0, 0.0);
			glVertex2f(triangles[j] - LENGTH, triangles[j + 1] - LENGTH);
			glVertex2f(triangles[j] - LENGTH, triangles[j + 1] + LENGTH);
			glVertex2f(triangles[j] + LENGTH, triangles[j + 1] + LENGTH);
			glEnd();
		}
	}

	for (GLfloat j = 0; j < squares.size(); j = j + 2) {
		if (squares[j] > 80 && squares[j] < ww + 150 && squares[j + 1] < wh - 90 && squares[j + 1] > 80)
		{
			glColor3f(1.0, 0.0, 0.0);
			drawSquareFilled(100.0, squares[j], squares[j + 1]);
		}
	}

	for (GLfloat j = 0; j < circles.size(); j = j + 2) {
		if (circles[j] > 80 && circles[j] < ww + 150 && circles[j + 1] < wh - 90 && circles[j + 1] > 80)
		{
			glColor3f(1.0, 0.0, 0.0);
			drawCircle(50.0, circles[j], circles[j + 1]);
		}
	}
}

//This function creates the animated polygons along with the differing move speeds - ySpeed
void animation() {
	//each function has parameters of the x and y c=postion of the polygon and its own move speed
	drawAnimSquare(20, 300, ySpeed);
	drawAnimSquare(20, 330, ySpeed1);
	drawAnimSquare(20, 250, ySpeed7);
	drawAnimSquare(20, 360, ySpeed2);
	drawAnimSquare(20, 200, ySpeed8);
	drawAnimSquare(20, 170, ySpeed3);
	drawAnimSquare(20, 390, ySpeed9);
	drawAnimSquare(20, 330, ySpeed4);
	drawAnimSquare(20, 420, ySpeed10);
	drawAnimSquare(20, 500, ySpeed5);
	drawAnimSquare(20, 470, ySpeed11);
	drawAnimSquare(20, 140, ySpeed6);
}

//This function deletes all shapes from the canvas by setting each vector as null
void deleteAllShapes() {
	for (GLint i = 0; i < dots.size(); i++)
	{
		dots[i] = NULL;
	}
	for (GLint i = 0; i < lines.size(); i++)
	{
		lines[i] = NULL;
	}
	for (GLint i = 0; i < rectangles.size(); i++)
	{
		rectangles[i] = NULL;
	}
	for (GLint i = 0; i < triangleLines.size(); i++)
	{
		triangleLines[i] = NULL;
	}
	for (GLint i = 0; i < triangles.size(); i++)
	{
		triangles[i] = NULL;
	}
	for (GLint i = 0; i < squares.size(); i++)
	{
		squares[i] = NULL;
	}
	for (GLint i = 0; i < circles.size(); i++)
	{
		circles[i] = NULL;
	}
	del = false;
}

//Draws grid using Line Stipple
void gridLines() {

	glPointSize(1.0);
	glColor3f(0.0f, 0.0f, 0.0f);
	int startX = 80;
	int endX = ww + 150;
	int startY = 80;
	int endY = wh - 90;

	//for horizontal grid lines
	glLineStipple(1, 0xAAAA);
	glColor3f(0.8, 0.8, 0.8);
	glBegin(GL_LINES);
	for (int i = startX; i < endX; i = i + 5)
	{
		for (int j = startY; j < endY; j = j + 5)
		{
			glVertex2f(i, j);
		}
		i = i + startX;
		j = j + startY;
	}
	glEnd();

	//for vertical grid lines
	glLineStipple(1, 0xAAAA);
	glBegin(GL_LINES);
	for (int j = startY; j < endY; j = j + 5)
	{
		for (int i = startX; i < endX; i = i + 5)
		{
			glVertex2f(i, j);
		}
		i = i + startX;
		j = j + startY;
	}
	glEnd();
}

//All buttons for both menus
//If a certain button is enabled then it changes background colour as implemented in the if statement
void dotButton() {
	if (isDotButtonActivated)
	{
		glColor3f(1.0, 1.0, 0.0);
		drawSquareFilled(75.0, 40, 485);
	}
	else {
		glColor3f(0.9, 0.9, 0.9);
		drawSquareFilled(75.0, 40, 485);
	}

	glColor3f(0.0, 0.0, 0.0);
	drawSquare(75.0, 40, 485);
	glPointSize(4.0);
	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_POINTS);
	glVertex2f(40, 485);
	glEnd();
}

void lineButton() {
	if (isLineButtonActivated)
	{
		glColor3f(1.0, 1.0, 0.0);
		drawSquareFilled(75.0, 40, 410);
	}
	else {
		glColor3f(0.9, 0.9, 0.9);
		drawSquareFilled(75.0, 40, 410);
	}

	glColor3f(0.0, 0.0, 0.0);
	drawSquare(75.0, 40, 410);
	glBegin(GL_LINE_LOOP);
	glColor3f(0.0, 0.0, 0.0);
	glVertex2f(40 - LENGTH, 410 - LENGTH);
	glVertex2f(40 + LENGTH, 410 + LENGTH);
	glEnd();
}

void rectLineButton() {
	if (isRectLineButtonActivated)
	{
		glColor3f(1.0, 1.0, 0.0);
		drawSquareFilled(75.0, 40, 335);
	}
	else {
		glColor3f(0.9, 0.9, 0.9);
		drawSquareFilled(75.0, 40, 335);
	}

	glColor3f(0.0, 0.0, 0.0);
	drawSquare(75.0, 40, 335);
	glColor3f(0.0, 0.0, 0.0);
	drawRectangle(50.0, 30.0, 40.0, 335.0);
}

void triangleLineButton() {
	if (isTriangleLineButtonActivated)
	{
		glColor3f(1.0, 1.0, 0.0);
		drawSquareFilled(75.0, 40, 260);
	}
	else {
		glColor3f(0.9, 0.9, 0.9);
		drawSquareFilled(75.0, 40, 260);
	}

	glColor3f(0.0, 0.0, 0.0);
	drawSquare(75.0, 40, 260);
	glBegin(GL_LINE_LOOP);
	glColor3f(0.0, 0.0, 0.0);
	glVertex2f(40 - LENGTH, 260 - LENGTH);
	glVertex2f(40 - LENGTH, 260 + LENGTH);
	glVertex2f(40 + LENGTH, 260 + LENGTH);
	glEnd();
}

void triangleFilledButton() {
	if (isTriangleFilledButtonActivated)
	{
		glColor3f(1.0, 1.0, 0.0);
		drawSquareFilled(75.0, 125, 550);
	}
	else {
		glColor3f(0.9, 0.9, 0.9);
		drawSquareFilled(75.0, 125, 550);
	}

	glColor3f(0.0, 0.0, 0.0);
	drawSquare(75.0, 125, 550);
	glBegin(GL_TRIANGLES);
	glColor3f(1.0, 0.0, 0.0);
	glVertex2f(125 - LENGTH, 550 - LENGTH);
	glVertex2f(125 - LENGTH, 550 + LENGTH);
	glVertex2f(125 + LENGTH, 550 + LENGTH);
	glEnd();
}

void squareFilledButton() {
	if (isSquareFilledButtonActivated)
	{
		glColor3f(1.0, 1.0, 0.0);
		drawSquareFilled(75.0, 200, 550);
	}
	else {
		glColor3f(0.9, 0.9, 0.9);
		drawSquareFilled(75.0, 200, 550);
	}

	glColor3f(0.0, 0.0, 0.0);
	drawSquare(75.0, 200, 550);
	glColor3f(1.0, 0.0, 0.0);
	drawSquareFilled(20.0, 200, 550);
}

void circleFilledButton() {
	if (isCircleFilledButtonActivated)
	{
		glColor3f(1.0, 1.0, 0.0);
		drawSquareFilled(75.0, 275, 550);
	}
	else {
		glColor3f(0.9, 0.9, 0.9);
		drawSquareFilled(75.0, 275, 550);
	}

	glColor3f(0.0, 0.0, 0.0);
	drawSquare(75.0, 275, 550);
	glColor3f(1.0, 0.0, 0.0);
	drawCircle(20.0, 275, 550);
}

void inactiveBars() {
	glColor3f(0.9, 0.9, 0.9);
	drawRectangleFilled(75.0, 443.0, 40.0, 0.0); // vertical bar
	drawRectangleFilled(500.0, 75.0, 565.0, 550.0); // horizontal bar
}

//Functions that draw the shapes when called using lengths, radius, x values and y values
//that are passed to the functions
//These functions have been created during the tutorial exerices
void drawSquareFilled(GLfloat squareLength, GLfloat xCoord, GLfloat yCoord)
{
	GLfloat x1 = xCoord - squareLength / 2;
	GLfloat y1 = yCoord + squareLength / 2;
	GLfloat x2 = xCoord + squareLength / 2;
	GLfloat y2 = yCoord + squareLength / 2;
	GLfloat x3 = xCoord + squareLength / 2;
	GLfloat y3 = yCoord - squareLength / 2;
	GLfloat x4 = xCoord - squareLength / 2;
	GLfloat y4 = yCoord - squareLength / 2;

	glBegin(GL_POLYGON);
	glVertex2f(x1, y1);
	glVertex2f(x2, y2);
	glVertex2f(x3, y3);
	glVertex2f(x4, y4);
	glEnd();

	glFlush();
}

void drawAnimSquare(GLfloat animSquareLength, GLfloat xCoord, GLfloat yCoord)
{
	GLfloat x1 = xCoord - animSquareLength / 2;
	GLfloat y1 = yCoord + animSquareLength / 2;
	GLfloat x2 = xCoord + animSquareLength / 2;
	GLfloat y2 = yCoord + animSquareLength / 2;
	GLfloat x3 = xCoord + animSquareLength / 2;
	GLfloat y3 = yCoord - animSquareLength / 2;
	GLfloat x4 = xCoord - animSquareLength / 2;
	GLfloat y4 = yCoord - animSquareLength / 2;

	glBegin(GL_POLYGON);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex2f(x1, y1);
	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex2f(x2, y2);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex2f(x3, y3);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex2f(x4, y4);
	glEnd();

	glFlush();
}

void drawSquare(GLfloat squareLength, GLfloat xCoord, GLfloat yCoord)
{
	GLfloat x1 = xCoord - squareLength / 2;
	GLfloat y1 = yCoord + squareLength / 2;
	GLfloat x2 = xCoord + squareLength / 2;
	GLfloat y2 = yCoord + squareLength / 2;
	GLfloat x3 = xCoord + squareLength / 2;
	GLfloat y3 = yCoord - squareLength / 2;
	GLfloat x4 = xCoord - squareLength / 2;
	GLfloat y4 = yCoord - squareLength / 2;

	glBegin(GL_LINE_LOOP);
	glVertex2f(x1, y1);
	glVertex2f(x2, y2);
	glVertex2f(x3, y3);
	glVertex2f(x4, y4);
	glEnd();

	glFlush();
}

void drawRectangleFilled(GLfloat lengthX, GLfloat lengthY, GLfloat xCoord, GLfloat yCoord)
{
	GLfloat x1 = xCoord - lengthX / 2;
	GLfloat y1 = yCoord + lengthY / 2;
	GLfloat x2 = xCoord + lengthX / 2;
	GLfloat y2 = yCoord + lengthY / 2;
	GLfloat x3 = xCoord + lengthX / 2;
	GLfloat y3 = yCoord - lengthY / 2;
	GLfloat x4 = xCoord - lengthX / 2;
	GLfloat y4 = yCoord - lengthY / 2;

	glBegin(GL_POLYGON);
	glVertex2f(x1, y1);
	glVertex2f(x2, y2);
	glVertex2f(x3, y3);
	glVertex2f(x4, y4);
	glEnd();

	glFlush();
}

void drawRectangle(GLfloat lengthX, GLfloat lengthY, GLfloat xCoord, GLfloat yCoord)
{
	GLfloat x1 = xCoord - lengthX / 2;
	GLfloat y1 = yCoord + lengthY / 2;
	GLfloat x2 = xCoord + lengthX / 2;
	GLfloat y2 = yCoord + lengthY / 2;
	GLfloat x3 = xCoord + lengthX / 2;
	GLfloat y3 = yCoord - lengthY / 2;
	GLfloat x4 = xCoord - lengthX / 2;
	GLfloat y4 = yCoord - lengthY / 2;

	glBegin(GL_LINE_LOOP);
	glVertex2f(x1, y1);
	glVertex2f(x2, y2);
	glVertex2f(x3, y3);
	glVertex2f(x4, y4);
	glEnd();

	glFlush();
}

void drawCircle(GLfloat radius, GLfloat xCoord, GLfloat yCoord)
{
	glBegin(GL_POLYGON);
	for (i = 0; i <= 1000; i++)
	{
		glVertex2f(xCoord + radius * cos(i * angle / 1000), yCoord + radius * sin(i * angle / 1000));
	}
	glEnd();

	glFlush();
}

//The timer functions that ticks when animation is enabled
//Each timer has different speeds, some may cause the square to move slow or fast or up or down
void TimerFunctionUp(int value)
{
	glClear(GL_COLOR_BUFFER_BIT);

	ySpeed++;

	if (ySpeed == 500)
	{
		ySpeed = 40;
	}

	glutPostRedisplay();
	glutTimerFunc(5, TimerFunctionUp, 0);
}
void TimerFunctionUp1(int value)
{
	glClear(GL_COLOR_BUFFER_BIT);

	ySpeed7++;

	if (ySpeed7 == 500)
	{
		ySpeed7 = 40;
	}

	glutPostRedisplay();
	glutTimerFunc(7, TimerFunctionUp1, 0);
}
void TimerFunctionUp2(int value)
{
	glClear(GL_COLOR_BUFFER_BIT);

	ySpeed8++;

	if (ySpeed8 == 500)
	{
		ySpeed8 = 40;
	}

	glutPostRedisplay();
	glutTimerFunc(2, TimerFunctionUp2, 0);
}
void TimerFunctionUp3(int value)
{
	glClear(GL_COLOR_BUFFER_BIT);

	ySpeed9++;

	if (ySpeed9 == 500)
	{
		ySpeed9 = 40;
	}

	glutPostRedisplay();
	glutTimerFunc(1, TimerFunctionUp3, 0);
}
void TimerFunctionUp4(int value)
{
	glClear(GL_COLOR_BUFFER_BIT);

	ySpeed10++;

	if (ySpeed10 == 500)
	{
		ySpeed10 = 40;
	}

	glutPostRedisplay();
	glutTimerFunc(6, TimerFunctionUp4, 0);
}
void TimerFunctionUp5(int value)
{
	glClear(GL_COLOR_BUFFER_BIT);

	ySpeed11++;

	if (ySpeed11 == 500)
	{
		ySpeed11 = 40;
	}

	glutPostRedisplay();
	glutTimerFunc(4, TimerFunctionUp5, 0);
}

void TimerFunctionDown(int value)
{
	glClear(GL_COLOR_BUFFER_BIT);

	ySpeed1--;

	if (ySpeed1 == 30)
	{
		ySpeed1 = 480;
	}

	glutPostRedisplay();
	glutTimerFunc(3, TimerFunctionDown, 0);
}
void TimerFunctionDown1(int value)
{
	glClear(GL_COLOR_BUFFER_BIT);

	ySpeed2--;

	if (ySpeed2 == 30)
	{
		ySpeed2 = 480;
	}

	glutPostRedisplay();
	glutTimerFunc(1, TimerFunctionDown1, 0);
}
void TimerFunctionDown2(int value)
{
	glClear(GL_COLOR_BUFFER_BIT);

	ySpeed3--;

	if (ySpeed3 == 30)
	{
		ySpeed3 = 480;
	}

	glutPostRedisplay();
	glutTimerFunc(4, TimerFunctionDown2, 0);
}
void TimerFunctionDown3(int value)
{
	glClear(GL_COLOR_BUFFER_BIT);

	ySpeed4--;

	if (ySpeed4 == 30)
	{
		ySpeed4 = 480;
	}

	glutPostRedisplay();
	glutTimerFunc(2, TimerFunctionDown3, 0);
}
void TimerFunctionDown4(int value)
{
	glClear(GL_COLOR_BUFFER_BIT);

	ySpeed5--;

	if (ySpeed5 == 30)
	{
		ySpeed5 = 480;
	}

	glutPostRedisplay();
	glutTimerFunc(9, TimerFunctionDown4, 0);
}
void TimerFunctionDown5(int value)
{
	glClear(GL_COLOR_BUFFER_BIT);

	ySpeed6--;

	if (ySpeed6 == 30)
	{
		ySpeed6 = 480;
	}

	glutPostRedisplay();
	glutTimerFunc(7, TimerFunctionDown5, 0);
}

//Creates menu with 3 options
void createOptionMenu(GLint id)
{
	//grid on
	if (id == 1)
	{
		grid = true;
		anim = false;
	}
	//grid off
	else if (id == 2)
	{
		grid = false;
		anim = false;
	}
	//animation on
	else if (id == 3)
	{
		anim = true;
	}
	glutPostRedisplay();
}

//Function for mouse interaction 
//Keeps track of all mouse clicks either on canvas or in the menus
void mouseInteraction(GLint button, GLint state, GLint x, GLint y)
{
	//Depending on what button or other area on canvas is pressed
	//different vectors will be enabled so that a primitive is drawn i.e. circle
	//the vector will be stored on that particular list i.e. circles
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		mouseClicks.push_back(x);
		mouseClicks.push_back(wh - y);
		if (isDotButtonActivated)
		{
			dots.push_back(x);
			dots.push_back(wh - y);
		}
		else if (isLineButtonActivated)
		{
			lines.push_back(x);
			lines.push_back(wh - y);
		}
		else if (isRectLineButtonActivated)
		{
			rectangles.push_back(x);
			rectangles.push_back(wh - y);
		}
		else if (isTriangleFilledButtonActivated)
		{
			triangles.push_back(x);
			triangles.push_back(wh - y);
		}
		else if (isTriangleLineButtonActivated)
		{
			triangleLines.push_back(x);
			triangleLines.push_back(wh - y);
		}
		else if (isSquareFilledButtonActivated)
		{
			squares.push_back(x);
			squares.push_back(wh - y);
		}
		else if (isCircleFilledButtonActivated)
		{
			circles.push_back(x);
			circles.push_back(wh - y);
		}

		// print in the console
		for (int i = 0; i < mouseClicks.size(); i++)
		{
			std::cout << mouseClicks[i] << " ";
		}
	}
	glutPostRedisplay();
}

//Function for keyboard inputs
void keyboardInput(unsigned char key, int x, int y)
{
	switch (key)
	{

	case 27:
		//Non-ASCII for Escape button
		exit(0);
		break;
	case 32:
		//Non-ASCII for Space bar
		//Deletes all shapes on canvas
		del = true;
		break;
	case 'l':
		//Deletes all lines on canvas
		for (GLint i = 0; i < lines.size(); i++)
		{
			lines[i] = NULL;
		}
		break;
	case 't':
		//Deletes all triangles on canvas
		for (GLint i = 0; i < triangles.size(); i++)
		{
			triangles[i] = NULL;
		}
		for (GLint i = 0; i < triangleLines.size(); i++)
		{
			triangleLines[i] = NULL;
		}
		break;
	case 'p':
		//Deletes all points/dots on canvas
		for (GLint i = 0; i < dots.size(); i++)
		{
			dots[i] = NULL;
		}
		break;
	case 'c':
		//Deletes all circles on canvas
		for (GLint i = 0; i < circles.size(); i++)
		{
			circles[i] = NULL;
		}
		break;
	case 'r':
		//Deletes all rectangles/squares on canvas
		for (GLint i = 0; i < rectangles.size(); i++)
		{
			rectangles[i] = NULL;
		}
		for (GLint i = 0; i < squares.size(); i++)
		{
			squares[i] = NULL;
		}
		break;
	case 'w':
		//Moves selected shape up
		if (shapeSelected)
		{
			verticalMovement++;
		}
		break;
	case 'a':
		//Moves selected shape left
		if (shapeSelected)
		{
			horizontalMovement--;
		}

		break;
	case 's':
		//Moves selected shape down
		if (shapeSelected)
		{
			verticalMovement--;
		}

		break;
	case 'd':
		//Moves selected shape right
		if (shapeSelected)
		{
			horizontalMovement++;
		}

		break;
	}
	glutPostRedisplay();
}

//Function to print instructions on black viewport
void printInteraction(void)
{
	cout << "How to use the 2D drawing canvas" << endl
		<< "" << endl
		<< "" << endl;
	cout << "1) Left click on the horizontal or vertical menu to select a primitive or filled shape" << endl
		<< "" << endl
		<< "" << endl;
	cout << "2) Left click on the canvas after selecting a shape" << endl
		<< "" << endl
		<< "" << endl;
	cout << "3) Left click on the grey bars to deselect a primitive or shape" << endl
		<< "" << endl
		<< "" << endl;
	cout << "4) Right click to view canvas options - grid or animation" << endl;

}

void init(void)
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
}

int main(int argc, char** argv)
{
	printInteraction();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(ww, wh);
	glutInitWindowPosition(300, 50);
	glutCreateWindow("Coursework");

	init();

	//Option Menu
	glutCreateMenu(createOptionMenu);
	glutAddMenuEntry("Grid On", 1);
	glutAddMenuEntry("Grid Off", 2);
	glutAddMenuEntry("Animation", 3);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	//Mouse interaction
	glutMouseFunc(mouseInteraction);

	//Keyboard Controls
	glutKeyboardFunc(keyboardInput);

	//Animation timers
	glutTimerFunc(5, TimerFunctionUp, 0);
	glutTimerFunc(7, TimerFunctionUp1, 0);
	glutTimerFunc(2, TimerFunctionUp2, 0);
	glutTimerFunc(1, TimerFunctionUp3, 0);
	glutTimerFunc(6, TimerFunctionUp4, 0);
	glutTimerFunc(4, TimerFunctionUp5, 0);
	glutTimerFunc(3, TimerFunctionDown, 0);
	glutTimerFunc(1, TimerFunctionDown1, 0);
	glutTimerFunc(4, TimerFunctionDown2, 0);
	glutTimerFunc(2, TimerFunctionDown3, 0);
	glutTimerFunc(9, TimerFunctionDown4, 0);
	glutTimerFunc(7, TimerFunctionDown5, 0);


	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();
	return 0;
}

