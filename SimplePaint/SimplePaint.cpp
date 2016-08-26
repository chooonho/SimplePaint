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

int windowWidth = 640;
int windowHeight = 480;
bool drawStart = false;
Shape* shapeDrawn = NULL;
ShapeType shapeTypeSelected = NONE;
Vertex2F mouseDownPoint;
Vertex2F mouseDragPoint;
Vertex2F mouseUpPoint;
std::vector<Button*> ptrUIButtons;
std::vector<Shape*> shapesDrawn;

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

		Button* ptrUIButton = new Button();
		ptrUIButton->setWidth(BUTTON_WIDTH);
		ptrUIButton->setHeight(BUTTON_HEIGHT);
		ptrUIButton->addShapeType(SHAPE_TYPES[i]);
		ptrUIButton->addShapeVertex(vertex.x + BUTTON_WIDTH, vertex.y);
		ptrUIButton->addShapeVertex(vertex.x + BUTTON_WIDTH, vertex.y - BUTTON_HEIGHT);
		ptrUIButton->addShapeVertex(vertex.x, vertex.y - BUTTON_HEIGHT);
		ptrUIButton->addShapeVertex(vertex.x, vertex.y);
		ptrUIButtons.push_back(ptrUIButton);

		Vertex2F centerPoint;
		centerPoint.x = vertex.x + (BUTTON_WIDTH / 2.0);
		centerPoint.y = vertex.y - (BUTTON_HEIGHT / 2.0);
		float iconHeight = BUTTON_HEIGHT - 20.0;
		float iconWidth = BUTTON_WIDTH - 20.0;
		
		ptrUIButton->setIcon(makeIcon(SHAPE_TYPES[i], iconWidth, iconHeight, centerPoint));

		vertex.x += BUTTON_WIDTH;
	}
}

void disposeUIButton() {
	for (int i = 0; i < MAX_BUTTON_COUNT; i++) {
		delete ptrUIButtons.back();
		ptrUIButtons.pop_back();
	}
}

void disposeDrawnShape() {
	while (!shapesDrawn.empty()) {
		delete shapesDrawn.back();
		shapesDrawn.pop_back();
	}
}

void renderUIButton() {
	glPointSize(2);
	glLineWidth(2);

	for (int i = 0; i < MAX_BUTTON_COUNT; i++) {
		std::vector<Vertex2F> buttonVertices = ptrUIButtons[i]->getShape().getAllVertices();
		std::vector<Vertex2F> iconVertices = ptrUIButtons[i]->getIcon().getShape().getAllVertices();

		if (ptrUIButtons[i]->getShape().getShapeType() == shapeTypeSelected) {
			glColor3f(1.0, 0.0, 0.0);
		}
		else {
			glColor3f(0.0, 0.0, 0.0);
		}
		glBegin(GL_LINE_LOOP);
			for (int j = 0; j < buttonVertices.size(); j++) {
				glVertex2f(buttonVertices[j].x, buttonVertices[j].y);
			}
		glEnd();

		glColor3f(0.0, 0.0, 0.0);
		if (ptrUIButtons[i]->getIcon().getShape().getShapeType() == S_POINT) {
			glBegin(GL_POINTS);
				glVertex2f(iconVertices[0].x, iconVertices[0].y);
			glEnd();
		}
		else {
			if (ptrUIButtons[i]->getIcon().getShape().getIsFilled()) {
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

void setUIButtonClicked() {
	for (int i = 0; i < MAX_BUTTON_COUNT; i++) {
		Vertex2F topLeftPoint = ptrUIButtons[i]->getShape().getVertex(3);
		Vertex2F bottomRightPoint = ptrUIButtons[i]->getShape().getVertex(1);

		if ((mouseUpPoint.x > topLeftPoint.x && mouseUpPoint.x < bottomRightPoint.x) &&
			(mouseUpPoint.y > bottomRightPoint.y && mouseUpPoint.y < topLeftPoint.y)) {
			shapeTypeSelected = ptrUIButtons[i]->getShape().getShapeType();
		}
	}
}

void draw() {
	std::vector<Vertex2F> shapeDrawnVertices;

	for (int i = 0; i < shapesDrawn.size(); i++) {
		shapeDrawnVertices = shapesDrawn[i]->getAllVertices();

		if (shapesDrawn[i]->getShapeType() == S_POINT) {
			glBegin(GL_POINTS);
				for (int j = 0; j < shapeDrawnVertices.size(); j++) {
					glVertex2f(shapeDrawnVertices[j].x, shapeDrawnVertices[j].y);
				}
			glEnd();
		}
		else {
			if (shapesDrawn[i]->getIsFilled()) {
				glBegin(GL_POLYGON);
				for (int j = 0; j < shapeDrawnVertices.size(); j++) {
					glVertex2f(shapeDrawnVertices[j].x, shapeDrawnVertices[j].y);
				}
				glEnd();
			}
			else {
				glBegin(GL_LINE_LOOP);
				for (int j = 0; j < shapeDrawnVertices.size(); j++) {
					glVertex2f(shapeDrawnVertices[j].x, shapeDrawnVertices[j].y);
				}
				glEnd();
			}
		}
	}

	//shapeDrawnVertices.clear();
	//shapeDrawnVertices = shapeDrawn->getAllVertices();
	//if (shapeDrawnVertices.size() == 0) {
	//	return;
	//}

	//if (shapeDrawn->getShapeType() == S_POINT) {
	//	glBegin(GL_POINTS);
	//		glVertex2f(shapeDrawnVertices[0].x, shapeDrawnVertices[0].y);
	//	glEnd();
	//}
	//else {
	//	if (shapeDrawn->getIsFilled()) {
	//		glBegin(GL_POLYGON);
	//			for (int j = 0; j < shapeDrawnVertices.size(); j++) {
	//				glVertex2f(shapeDrawnVertices[j].x, shapeDrawnVertices[j].y);
	//			}
	//		glEnd();
	//	}
	//	else {
	//		glBegin(GL_LINE_LOOP);
	//			for (int j = 0; j < shapeDrawnVertices.size(); j++) {
	//				glVertex2f(shapeDrawnVertices[j].x, shapeDrawnVertices[j].y);
	//			}
	//		glEnd();
	//	}
	//}
}

void handleStartDraw(float x, float y) {
	if (shapeTypeSelected == NONE) {
		return;
	}

	drawStart = true;

	shapeDrawn = new Shape();
	shapeDrawn->addVertex(x, y);
	shapeDrawn->addVertex(x, y);
	shapeDrawn->addVertex(x, y);
	shapeDrawn->addVertex(x, y);
	shapeDrawn->setShapeType(shapeTypeSelected);
	if (shapeTypeSelected == TRIANGLE_F || shapeTypeSelected == RECTANGLE_F || shapeTypeSelected == OVAL_F) {
		shapeDrawn->setIsFilled(true);
	}

	shapesDrawn.push_back(shapeDrawn);
}

void handleContinueDraw(float x, float y) {
	if (!drawStart || shapeDrawn == NULL) {
		return;
	}

	if (shapeDrawn->getShapeType() == S_POINT) {
		shapeDrawn->addVertex(x, y);
		glutPostRedisplay();

		return;
	}

	shapeDrawn->clearAllVertices();

	if (x > mouseDownPoint.x) {
		if (y > mouseDownPoint.y) {
			shapeDrawn->addVertex(x, y);
			shapeDrawn->addVertex(x, mouseDownPoint.y);
			shapeDrawn->addVertex(mouseDownPoint.x, mouseDownPoint.y);
			shapeDrawn->addVertex(mouseDownPoint.x, y);
		}
		else {
			shapeDrawn->addVertex(x, mouseDownPoint.y);
			shapeDrawn->addVertex(x, y);
			shapeDrawn->addVertex(mouseDownPoint.x, y);
			shapeDrawn->addVertex(mouseDownPoint.x, mouseDownPoint.y);
		}
	}
	else {
		if (y > mouseDownPoint.y) {
			shapeDrawn->addVertex(mouseDownPoint.x, mouseDownPoint.y);
			shapeDrawn->addVertex(mouseDownPoint.x, y);
			shapeDrawn->addVertex(x, y);
			shapeDrawn->addVertex(x, mouseDownPoint.y);
		}
		else {
			shapeDrawn->addVertex(mouseDownPoint.x, y);
			shapeDrawn->addVertex(mouseDownPoint.x, mouseDownPoint.y);
			shapeDrawn->addVertex(x, mouseDownPoint.y);
			shapeDrawn->addVertex(x, y);
		}
	}

	glutPostRedisplay();
}

void handleCompleteDraw(float x, float y) {
	if (!drawStart || shapeDrawn == NULL) {
		return;
	}

	drawStart = false;

	if (shapeDrawn->getShapeType() != S_POINT && (x == mouseDownPoint.x && y == mouseDownPoint.y)) {
		delete shapeDrawn;
		shapeDrawn = NULL;

		shapesDrawn.pop_back();
	}

	glutPostRedisplay();
}

void mouseDrag(int x, int y) {
	mouseDragPoint.x = x;
	mouseDragPoint.y = windowHeight - y;

	if (drawStart) {
		handleContinueDraw(x, windowHeight - y);
	}
}

void mouseClick(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) {
			mouseDownPoint.x = x;
			mouseDownPoint.y = windowHeight - y;

			handleStartDraw(x, windowHeight - y);
		}

		if (state == GLUT_UP) {
			mouseUpPoint.x = x;
			mouseUpPoint.y = windowHeight - y;

			handleCompleteDraw(x, windowHeight - y);
		}
	}
}

void renderScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	setUIButtonClicked();
	renderUIButton();

	draw();

	glutSwapBuffers();
}

void reshapeScene(int width, int height) {
	if (height == 0) {
		height = 1;
	}

	float ratio = 1.0 * width / height;
	windowWidth = width;
	windowHeight = height;
	
	disposeUIButton();
	initUIButton();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, width, height);
	gluOrtho2D(0, width, 0, height);
	glMatrixMode(GL_MODELVIEW);

}

void init(void) {
	glClearColor(0.8, 0.8, 0.8, 0.0);
	glColor3f(1.0f, 1.0f, 1.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, windowWidth, 0, windowHeight);
}

int main(int argc, char** argv) {
	initUIButton();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(windowWidth, windowHeight);
	glutCreateWindow("CSCI336-Assignment1 Simple Paint");
	init();

	glutDisplayFunc(renderScene);
	glutReshapeFunc(reshapeScene);
	glutMouseFunc(mouseClick);
	glutMotionFunc(mouseDrag);

	glutMainLoop();

	disposeUIButton();

	return 0;
}