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
std::vector<Button*> ptrButtons;

void renderButton() {
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

	glFlush();
}

void renderScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	renderButton();
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

	std::cout << centerPoint.x << " " << centerPoint.y << std::endl;
	
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
	const float VERTEX_TOP_POS_Y = 0.95;
	const float VERTEX_BOTTOM_POS_Y = 0.75;
	const float VERTEX_LEFT_DX = 0.05;
	const float VERTEX_RIGHT_DX = 0.20;
	const ShapeType SHAPE_TYPES[MAX_BUTTON_COUNT] = { S_POINT, LINE, TRIANGLE, TRIANGLE_F, RECTANGLE, RECTANGLE_F, OVAL, OVAL_F };

	float vertexX = -1.0f;
	float buttonWidth = VERTEX_RIGHT_DX - VERTEX_LEFT_DX;
	float buttonHeight = VERTEX_TOP_POS_Y - VERTEX_BOTTOM_POS_Y;

	for (int i = 0; i < MAX_BUTTON_COUNT; i++) {
		Button* ptrButton = new Button();
		ptrButton->addShapeVertex(vertexX + VERTEX_RIGHT_DX, VERTEX_TOP_POS_Y);
		ptrButton->addShapeVertex(vertexX + VERTEX_RIGHT_DX, VERTEX_BOTTOM_POS_Y);
		ptrButton->addShapeVertex(vertexX + VERTEX_LEFT_DX, VERTEX_BOTTOM_POS_Y);
		ptrButton->addShapeVertex(vertexX + VERTEX_LEFT_DX, VERTEX_TOP_POS_Y);
		ptrButtons.push_back(ptrButton);

		Vertex2F centerPoint;
		centerPoint.x = vertexX + VERTEX_LEFT_DX + (buttonWidth / 2.0);
		centerPoint.y = VERTEX_TOP_POS_Y - (buttonHeight / 2.0);
		float iconHeight = VERTEX_TOP_POS_Y - VERTEX_BOTTOM_POS_Y - 0.05;
		float iconWidth = VERTEX_RIGHT_DX - VERTEX_LEFT_DX - 0.05;
		
		ptrButton->setIcon(makeIcon(SHAPE_TYPES[i], iconWidth, iconHeight, centerPoint));

		vertexX += 0.2;
	}
}

void disposeUIButton() {
	for (int i = 0; i < MAX_BUTTON_COUNT; i++) {
		delete ptrButtons.back();
		ptrButtons.pop_back();
	}
}

int main(int argc, char** argv) {
	initUIButton();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(640, 480);
	glutCreateWindow("CSCI336-Assignment1 Simple Paint");

	glutDisplayFunc(renderScene);

	glutMainLoop();

	disposeUIButton();

	return 0;
}