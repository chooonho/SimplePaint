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
#include "Frame.h"

int windowWidth = 640;
int windowHeight = 480;
bool drawStart = false;
bool activeFrameClicked = false;
Frame* drawingFrame = NULL;
ShapeType shapeTypeSelected = NONE;
Vertex2F mouseDownPoint;
Vertex2F mouseDragPoint;
Vertex2F mouseUpPoint;
std::vector<Button*> ptrUIButtons;
std::vector<Frame*> drawingFrames;

Shape initShape(ShapeType shapeType, std::vector<Vertex2F> outlineVertices, int mouseDownVPos = 0) {
	int mouseUpVPos = (mouseDownVPos + 2) % 4;
	float halfHeight = (outlineVertices[3].y - outlineVertices[1].y) / 2;
	float halfWidth = (outlineVertices[1].x - outlineVertices[3].x) / 2;

	Vertex2F centerPoint;
	centerPoint.x = outlineVertices[3].x + halfWidth;
	centerPoint.y = outlineVertices[1].y + halfHeight;

	Shape shape;
	shape.setShapeType(shapeType);

	switch (shapeType) {
		case S_POINT:
			shape.addVertex(centerPoint);
			break;
		case LINE:
			shape.addVertex(outlineVertices[mouseDownVPos]);
			shape.addVertex(outlineVertices[mouseUpVPos]);
			break;
		case TRIANGLE_F:
			shape.setIsFilled(true);
		case TRIANGLE:
			shape.addVertex((centerPoint.x), (centerPoint.y + halfHeight));
			shape.addVertex(outlineVertices[1]);
			shape.addVertex(outlineVertices[2]);
			break;
		case RECTANGLE_F:
			shape.setIsFilled(true);
		case RECTANGLE:
			for (int i = 0; i < outlineVertices.size(); i++) {
				shape.addVertex(outlineVertices[i]);
			}
			break;
		case OVAL_F:
			shape.setIsFilled(true);
		case OVAL:
			for (int i = 0; i < 360; i++) {
				float angle = i * MATH_PI / 180;
				shape.addVertex((centerPoint.x + (cos(angle) * halfWidth)), (centerPoint.y + (sin(angle) * halfHeight)));
			}
		break;
	}

	return shape;
}

Icon makeIcon(ShapeType iconShapeType, std::vector<Vertex2F> buttonVertices) {
	buttonVertices[0].x = buttonVertices[0].x - BUTTON_PADDING_INNER;
	buttonVertices[0].y = buttonVertices[0].y - BUTTON_PADDING_INNER;
	buttonVertices[1].x = buttonVertices[1].x - BUTTON_PADDING_INNER;
	buttonVertices[1].y = buttonVertices[1].y + BUTTON_PADDING_INNER;
	buttonVertices[2].x = buttonVertices[2].x + BUTTON_PADDING_INNER;
	buttonVertices[2].y = buttonVertices[2].y + BUTTON_PADDING_INNER;
	buttonVertices[3].x = buttonVertices[3].x + BUTTON_PADDING_INNER;
	buttonVertices[3].y = buttonVertices[3].y - BUTTON_PADDING_INNER;

	Shape shape = initShape(iconShapeType, buttonVertices);

	Icon icon;
	icon.setHeight(buttonVertices[3].y - buttonVertices[1].y);
	icon.setWidth(buttonVertices[1].x - buttonVertices[3].x);
	icon.setShape(shape);

	return icon;
}

void initUIButton() {
	Vertex2F vertex;
	vertex.x = 0.0f;
	vertex.y = windowHeight - BUTTON_PADDING_OUTER;

	for (int i = 0; i < MAX_BUTTON_COUNT; i++) {
		vertex.x += BUTTON_PADDING_OUTER;

		Button* ptrUIButton = new Button();
		ptrUIButton->setWidth(BUTTON_WIDTH);
		ptrUIButton->setHeight(BUTTON_HEIGHT);
		ptrUIButton->addShapeType(SHAPE_TYPES[i]);
		ptrUIButton->addShapeVertex(vertex.x + BUTTON_WIDTH, vertex.y);
		ptrUIButton->addShapeVertex(vertex.x + BUTTON_WIDTH, vertex.y - BUTTON_HEIGHT);
		ptrUIButton->addShapeVertex(vertex.x, vertex.y - BUTTON_HEIGHT);
		ptrUIButton->addShapeVertex(vertex.x, vertex.y);
		ptrUIButton->setIcon(makeIcon(SHAPE_TYPES[i], ptrUIButton->getShape().getAllVertices()));

		ptrUIButtons.push_back(ptrUIButton);

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
	while (!drawingFrames.empty()) {
		delete drawingFrames.back();
		drawingFrames.pop_back();
	}
}

void renderUIButton() {
	glPointSize(2);
	glLineWidth(2);

	for (int i = 0; i < MAX_BUTTON_COUNT; i++) {
		std::vector<Vertex2F> buttonVertices = ptrUIButtons[i]->getShape().getAllVertices();
		std::vector<Vertex2F> iconVertices = ptrUIButtons[i]->getIcon().getShape().getAllVertices();

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
	for (int i = 0; i < drawingFrames.size(); i++) {
		Shape shapeDrawn = drawingFrames[i]->getShapeDrawn();
		std::vector<Vertex2F> shapeDrawnVertices = shapeDrawn.getAllVertices();

		glColor3f(0.0f, 0.0f, 0.0f);
		glLineWidth(2);
		if (shapeDrawn.getShapeType() == S_POINT) {
			glBegin(GL_POINTS);
		}
		else {
			if (shapeDrawn.getIsFilled()) {
				glBegin(GL_POLYGON);
			}
			else {
				glBegin(GL_LINE_LOOP);
			}
		}
		for (int i = 0; i < shapeDrawnVertices.size(); i++) {
			glVertex2f(shapeDrawnVertices[i].x, shapeDrawnVertices[i].y);
		}
		glEnd();

		if (drawingFrames[i]->getActive()) {
			std::vector<Vertex2F> frameOutlineVertices = drawingFrames[i]->getOutline().getAllVertices();

			glColor3f(0.0f, 0.25f, 1.0f);
			glLineWidth(1);
			glEnable(GL_LINE_STIPPLE);
			glLineStipple(1, 0xF0F0);
			glBegin(GL_LINE_LOOP);
			for (int j = 0; j < frameOutlineVertices.size(); j++) {
				glVertex2f(frameOutlineVertices[j].x, frameOutlineVertices[j].y);
			}
			glEnd();
			glDisable(GL_LINE_STIPPLE);
		}
	}
}

void handleStartDraw(float x, float y) {
	if (shapeTypeSelected == NONE) {
		return;
	}

	if (drawingFrame != NULL) {
		drawingFrame->setActive(false);
	}
	drawStart = true;

	Shape outline;
	outline.setShapeType(RECTANGLE);
	outline.addVertex(x, y);
	outline.addVertex(x, y);
	outline.addVertex(x, y);
	outline.addVertex(x, y);

	Shape shapeDrawn = initShape(shapeTypeSelected, outline.getAllVertices());

	drawingFrame = new Frame();
	drawingFrame->setOutline(outline);
	drawingFrame->setShapeDrawn(shapeDrawn);
	drawingFrames.push_back(drawingFrame);
}

void handleContinueDraw(float x, float y) {
	if (!drawStart || drawingFrame == NULL) {
		return;
	}

	Shape outline;
	outline.setShapeType(RECTANGLE);

	Shape shapeDrawn;

	if (x > mouseDownPoint.x) {
		if (y > mouseDownPoint.y) {
			outline.addVertex(x, y);
			outline.addVertex(x, mouseDownPoint.y);
			outline.addVertex(mouseDownPoint.x, mouseDownPoint.y);
			outline.addVertex(mouseDownPoint.x, y);

			shapeDrawn = initShape(shapeTypeSelected, outline.getAllVertices(), 2);
		}
		else {
			outline.addVertex(x, mouseDownPoint.y);
			outline.addVertex(x, y);
			outline.addVertex(mouseDownPoint.x, y);
			outline.addVertex(mouseDownPoint.x, mouseDownPoint.y);

			shapeDrawn = initShape(shapeTypeSelected, outline.getAllVertices(), 3);
		}
	}
	else {
		if (y > mouseDownPoint.y) {
			outline.addVertex(mouseDownPoint.x, y);
			outline.addVertex(mouseDownPoint.x, mouseDownPoint.y);
			outline.addVertex(x, mouseDownPoint.y);
			outline.addVertex(x, y);

			shapeDrawn = initShape(shapeTypeSelected, outline.getAllVertices(), 1);
		}
		else {
			outline.addVertex(mouseDownPoint.x, mouseDownPoint.y);
			outline.addVertex(mouseDownPoint.x, y);
			outline.addVertex(x, y);
			outline.addVertex(x, mouseDownPoint.y);

			shapeDrawn = initShape(shapeTypeSelected, outline.getAllVertices(), 0);
		}
	}

	drawingFrame->setOutline(outline);
	drawingFrame->setShapeDrawn(shapeDrawn);

	glutPostRedisplay();
}

void handleCompleteDraw(float x, float y) {
	if (!drawStart || drawingFrame == NULL) {
		return;
	}

	drawStart = false;

	if (x == mouseDownPoint.x && y == mouseDownPoint.y) {
		delete drawingFrame;
		drawingFrame = NULL;

		drawingFrames.pop_back();
	}

	glutPostRedisplay();
}

void handleDragDrawingFrame(float x, float y) {
	if (drawingFrame == NULL) {
		return;
	}

	if (activeFrameClicked) {
		float offsetX = x - mouseDownPoint.x;
		float offsetY = y - mouseDownPoint.y;

		mouseDownPoint.x = x;
		mouseDownPoint.y = y;

		Shape outline;
		outline.setShapeType(RECTANGLE);
		outline.addVertex(drawingFrame->getOutline().getVertex(0).x + offsetX,
										drawingFrame->getOutline().getVertex(0).y + offsetY);
		outline.addVertex(drawingFrame->getOutline().getVertex(1).x + offsetX,
										drawingFrame->getOutline().getVertex(1).y + offsetY);
		outline.addVertex(drawingFrame->getOutline().getVertex(2).x + offsetX,
										drawingFrame->getOutline().getVertex(2).y + offsetY);
		outline.addVertex(drawingFrame->getOutline().getVertex(3).x + offsetX,
										drawingFrame->getOutline().getVertex(3).y + offsetY);

		Shape shapeDrawn = initShape(shapeTypeSelected, outline.getAllVertices());

		drawingFrame->setOutline(outline);
		drawingFrame->setShapeDrawn(shapeDrawn);

		glutPostRedisplay();
	}
}

bool isActiveFrameClicked(float x, float y) {
	if (drawingFrame == NULL) {
		return false;
	}

	if (drawingFrame->getActive()) {
		Vertex2F topLeftVertex = drawingFrame->getOutline().getVertex(3);
		Vertex2F bottomRightVertex = drawingFrame->getOutline().getVertex(1);

		if ((x > topLeftVertex.x && x < bottomRightVertex.x) &&
			(y > bottomRightVertex.y && y < topLeftVertex.y)) {
			return true;
		}
	}

	return false;
}

void mouseDrag(int x, int y) {
	mouseDragPoint.x = x;
	mouseDragPoint.y = windowHeight - y;

	handleDragDrawingFrame(x, windowHeight - y);

	if (drawStart) {
		handleContinueDraw(x, windowHeight - y);
	}
}

void mouseClick(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) {
			mouseDownPoint.x = x;
			mouseDownPoint.y = windowHeight - y;

			if (!(activeFrameClicked = isActiveFrameClicked(x, windowHeight - y))) {
				activeFrameClicked = false;
				handleStartDraw(x, windowHeight - y);
			}
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