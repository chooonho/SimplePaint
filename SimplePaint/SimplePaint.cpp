/*******************************************************************************
*	Name				: OOI CHOON HO
*	Student ID			: 4805604
*	Coursework Title	: CSCI336 Assignment 1
*	Objecttive			: Develop a simple 2D Paint program
*******************************************************************************/

#include <GL/glut.h>
#include <vector>
#include <iostream>
#include "Button.h"

const int MAX_BUTTON_COUNT = 8;
int windowWidth = 640, windowHeight = 480;
std::vector<Button*> ptrButtons;

void init(void)
{
	glClearColor(0.8, 0.8, 0.8, 0.0);
	glColor3f(1.0f, 1.0f, 1.0f);
	glMatrixMode(GL_PROJECTION);	
	glLoadIdentity();
	gluOrtho2D(0, 640, 0, 480);
}

void renderButton() {
	glColor3f(0.0, 0.0, 0.0);
	glPointSize(2);
	glLineWidth(2);

	for (int i = 0; i < MAX_BUTTON_COUNT; i++) {
		glBegin(GL_LINE_LOOP);
			glVertex2f(ptrButtons[i]->getShape().getVertex(0).x, ptrButtons[i]->getShape().getVertex(0).y);
			glVertex2f(ptrButtons[i]->getShape().getVertex(1).x, ptrButtons[i]->getShape().getVertex(1).y);
			glVertex2f(ptrButtons[i]->getShape().getVertex(2).x, ptrButtons[i]->getShape().getVertex(2).y);
			glVertex2f(ptrButtons[i]->getShape().getVertex(3).x, ptrButtons[i]->getShape().getVertex(3).y);
		glEnd();

		std::vector<Vertex2F> iconVertices = ptrButtons[i]->getIcon().getShape().getAllVertices();
		if (ptrButtons[i]->getIcon().getShape().getShapeType() == S_POINT) {
			glBegin(GL_POINTS);
				glVertex2f(iconVertices[0].x, iconVertices[0].y);
			glEnd();
		}
		else {
			if (ptrButtons[i]->getIcon().getShape().getIsFilled()) {
				glBegin(GL_POLYGON);
				for (int j = 0; j < iconVertices.size(); j++) {
					glVertex2f(iconVertices[j].x, iconVertices[j].y);
				}
				glEnd();
			}
			else {
				glBegin(GL_LINE_LOOP);
				for (int j = 0; j < iconVertices.size(); j++) {
					glVertex2f(iconVertices[j].x, iconVertices[j].y);
				}
				glEnd();
			}
		}
	}
}

Icon makeIcon(ShapeType shapeType, float width, float height, Vertex2F centerPoint) {
	const double MATH_PI = 3.141592;

	Icon icon;
	icon.setCenterPoint(centerPoint);
	icon.setHeight(height);
	icon.setWidth(width);

	float halfWidth = width / 2;
	float halfHeight = height / 2;

	Shape shape;
	shape.setShapeType(shapeType);
	
	switch (shapeType) {
		case S_POINT:
			shape.addVertex(centerPoint);
			break;
		case LINE:
			shape.addVertex((centerPoint.x - halfWidth), (centerPoint.y - halfHeight));
			shape.addVertex((centerPoint.x + halfWidth), (centerPoint.y + halfHeight));
			break;
		case TRIANGLE_F:
			shape.setIsFilled(true);
		case TRIANGLE:
			shape.addVertex((centerPoint.x), (centerPoint.y + halfHeight));
			shape.addVertex((centerPoint.x + halfWidth), (centerPoint.y - halfHeight));
			shape.addVertex((centerPoint.x - halfWidth), (centerPoint.y - halfHeight));
			break;
		case RECTANGLE_F:
			shape.setIsFilled(true);
		case RECTANGLE:
			shape.addVertex((centerPoint.x + halfWidth), (centerPoint.y + halfHeight));
			shape.addVertex((centerPoint.x + halfWidth), (centerPoint.y - halfHeight));
			shape.addVertex((centerPoint.x - halfWidth), (centerPoint.y - halfHeight));
			shape.addVertex((centerPoint.x - halfWidth), (centerPoint.y + halfHeight));
			break;
		case OVAL_F:
			shape.setIsFilled(true);
		case OVAL:
			float radiusX = halfWidth;
			float radiusY = halfHeight / 2;

			for (int i = 0; i < 360; i++) {
				float angle = i * MATH_PI / 180;
				shape.addVertex((centerPoint.x + (cos(angle) * radiusX)), (centerPoint.y + (sin(angle) * radiusY)));
			}
			break;
	}
	
	icon.setShape(shape);

	return icon;
}

void initUIButton() {
	const float BUTTON_HEIGHT = 70.0;
	const float BUTTON_WIDTH = 70.0;
	const float BUTTON_PADDING = 5.0;
	const ShapeType SHAPE_TYPES[MAX_BUTTON_COUNT] = { S_POINT, LINE, TRIANGLE, TRIANGLE_F, RECTANGLE, RECTANGLE_F, OVAL, OVAL_F };

	Vertex2F vertex;
	vertex.x = 0.0f;
	vertex.y = windowHeight - BUTTON_PADDING;

	for (int i = 0; i < MAX_BUTTON_COUNT; i++) {
		vertex.x += BUTTON_PADDING;

		Button* ptrButton = new Button();
		ptrButton->addShapeVertex(vertex.x + BUTTON_WIDTH, vertex.y);
		ptrButton->addShapeVertex(vertex.x + BUTTON_WIDTH, vertex.y - BUTTON_HEIGHT);
		ptrButton->addShapeVertex(vertex.x, vertex.y - BUTTON_HEIGHT);
		ptrButton->addShapeVertex(vertex.x, vertex.y);
		ptrButtons.push_back(ptrButton);

		Vertex2F centerPoint;
		centerPoint.x = vertex.x + (BUTTON_WIDTH / 2.0);
		centerPoint.y = vertex.y - (BUTTON_HEIGHT / 2.0);
		float iconHeight = BUTTON_HEIGHT - 20.0;
		float iconWidth = BUTTON_WIDTH - 20.0;
		
		ptrButton->setIcon(makeIcon(SHAPE_TYPES[i], iconWidth, iconHeight, centerPoint));

		vertex.x += BUTTON_WIDTH;
	}
}

void disposeUIButton() {
	for (int i = 0; i < MAX_BUTTON_COUNT; i++) {
		delete ptrButtons.back();
		ptrButtons.pop_back();
	}
}


void renderScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	renderButton();

	glFlush();
}

int main(int argc, char** argv) {
	initUIButton();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(windowWidth, windowHeight);
	glutCreateWindow("CSCI336-Assignment1 Simple Paint");
	init();

	glutDisplayFunc(renderScene);

	glutMainLoop();

	disposeUIButton();

	return 0;
}