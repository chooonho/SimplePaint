/*******************************************************************************
*	Name				: OOI CHOON HO
*	Student ID			: 4805604
*	Coursework Title	: CSCI336 Assignment 1
*	Objecttive			: Develop a simple 2D Paint program
*******************************************************************************/

#include <cstdlib>
#include <GL/glut.h>
#include <vector>
#include <iostream>
#include "Button.h"
#include "Frame.h"

const int UI_TOOLBAR_HEIGHT = BUTTON_HEIGHT + (BUTTON_PADDING_OUTER * 2);
const int DEFAULT_POINT_SIZE = 1;
const int DEFAULT_LINE_WIDTH = 1;

int windowId;
int windowWidth = 640;
int windowHeight = 480;
bool drawStart = false;
bool activeFrameClicked = false;
float pointSize = DEFAULT_POINT_SIZE;
float lineWidth = DEFAULT_LINE_WIDTH;
ColorRGB3F color;
Frame* drawingFrame = NULL;
ShapeType shapeTypeSelected = NONE;
Vertex2F mouseDownPoint;
Vertex2F mouseDragPoint;
Vertex2F mouseUpPoint;
std::vector<Button*> ptrUIButtons;
std::vector<Frame*> drawingFrames;

enum MenuEvent {
	CLEAR,
	EXIT
};

enum MarkerSize {
	M_SIZE_1,
	M_SIZE_2,
	M_SIZE_3,
	M_SIZE_4,
	M_SIZE_5,
	M_SIZE_6,
	M_SIZE_7
};

Shape initShape(ShapeType shapeType, std::vector<Vertex2F> outlineVertices, float size, ColorRGB3F color) {
	Shape shape;
	shape.setShapeType(shapeType);
	shape.setSize(size);
	shape.setColor(color);

	if (shapeType != S_POINT && shapeType != LINE) {
		float halfHeight = (outlineVertices[3].y - outlineVertices[1].y) / 2;
		float halfWidth = (outlineVertices[1].x - outlineVertices[3].x) / 2;

		Vertex2F centerPoint;
		centerPoint.x = outlineVertices[3].x + halfWidth;
		centerPoint.y = outlineVertices[1].y + halfHeight;

		switch (shapeType) {
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
	}
	else {
		switch (shapeType) {
			case S_POINT:
				shape.addVertex(outlineVertices[0]);
				break;

			case LINE:
				shape.addVertex(outlineVertices[0]);
				shape.addVertex(outlineVertices[1]);
				break;
		}
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

	float iconHeight = buttonVertices[3].y - buttonVertices[1].y;
	float iconWidth = buttonVertices[1].x - buttonVertices[3].x;

	if (iconShapeType == S_POINT) {
		Vertex2F vertex;
		vertex.x = buttonVertices[3].x + ((buttonVertices[1].x - buttonVertices[3].x) / 2);
		vertex.y = buttonVertices[1].y + ((buttonVertices[3].y - buttonVertices[1].y) / 2);

		buttonVertices.clear();
		buttonVertices.push_back(vertex);
	}
	else if (iconShapeType == LINE) {
		Vertex2F vertexStart;
		Vertex2F vertexEnd;

		vertexStart.x = buttonVertices[2].x;
		vertexStart.y = buttonVertices[2].y;
		vertexEnd.x = buttonVertices[0].x;
		vertexEnd.y = buttonVertices[0].y;

		buttonVertices.clear();
		buttonVertices.push_back(vertexStart);
		buttonVertices.push_back(vertexEnd);
	}

	float size = (iconShapeType == S_POINT) ? DEFAULT_POINT_SIZE : DEFAULT_LINE_WIDTH;

	Icon icon;
	icon.setHeight(iconHeight);
	icon.setWidth(iconWidth);
	icon.setShape(initShape(iconShapeType, buttonVertices, size, COLORS[BLACK]));

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

void disposeDrawingFrames() {
	while (!drawingFrames.empty()) {
		delete drawingFrames.back();
		drawingFrames.back() = NULL;
		drawingFrames.pop_back();
	}

	drawingFrame = NULL;
}

void renderUIButton() {
	glPointSize(DEFAULT_POINT_SIZE + 1);
	glLineWidth(DEFAULT_LINE_WIDTH + 1);

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
	if (mouseDownPoint.y < (windowHeight - UI_TOOLBAR_HEIGHT)) {
		return;
	}

	for (int i = 0; i < MAX_BUTTON_COUNT; i++) {
		Vertex2F vertexTopLeft = ptrUIButtons[i]->getShape().getVertex(3);
		Vertex2F vertexBottomRight = ptrUIButtons[i]->getShape().getVertex(1);

		if ((mouseUpPoint.x > vertexTopLeft.x && mouseUpPoint.x < vertexBottomRight.x) &&
			(mouseUpPoint.y > vertexBottomRight.y && mouseUpPoint.y < vertexTopLeft.y)) {
			shapeTypeSelected = ptrUIButtons[i]->getShape().getShapeType();
		}
	}
}

void draw() {
	for (int i = 0; i < drawingFrames.size(); i++) {
		Shape shapeDrawn = drawingFrames[i]->getShapeDrawn();
		std::vector<Vertex2F> shapeDrawnVertices = shapeDrawn.getAllVertices();

		glColor3f(shapeDrawn.getColor().red, shapeDrawn.getColor().green, shapeDrawn.getColor().blue);
		glPointSize(shapeDrawn.getSize());
		glLineWidth(shapeDrawn.getSize());
		if (shapeDrawn.getShapeType() == S_POINT) {
			glBegin(GL_POINTS);
		}
		else if (shapeDrawn.getShapeType() == LINE) {
			glBegin(GL_LINES);
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
			std::vector<Vertex2F> outlineVertices = drawingFrames[i]->getOutline().getAllVertices();

			glColor3f(0.0f, 0.25f, 1.0f);
			glLineWidth(DEFAULT_LINE_WIDTH);
			glEnable(GL_LINE_STIPPLE);
			glLineStipple(1, 0xF0F0);
			glBegin(GL_LINE_LOOP);
				for (int j = 0; j < outlineVertices.size(); j++) {
					glVertex2f(outlineVertices[j].x, outlineVertices[j].y);
				}
			glEnd();
			glDisable(GL_LINE_STIPPLE);
		}
	}

	glColor3f(0.8f, 0.8f, 0.8f);
	glBegin(GL_POLYGON);
		glVertex2f(windowWidth, windowHeight);
		glVertex2f(windowWidth, windowHeight - UI_TOOLBAR_HEIGHT);
		glVertex2f(0, windowHeight - UI_TOOLBAR_HEIGHT);
		glVertex2f(0, windowHeight);
	glEnd();

	glLineWidth(DEFAULT_LINE_WIDTH + 1);
	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);
		glVertex2f(0, windowHeight - UI_TOOLBAR_HEIGHT);
		glVertex2f(windowWidth, windowHeight - UI_TOOLBAR_HEIGHT);
	glEnd();
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
	if (shapeTypeSelected == S_POINT) {
		outline.setShapeType(S_POINT);
		outline.addVertex(x, y);
	}
	else if (shapeTypeSelected == LINE) {
		outline.setShapeType(LINE);
		outline.addVertex(x, y);
		outline.addVertex(x, y);
	}
	else {
		outline.setShapeType(RECTANGLE);
		outline.addVertex(x, y);
		outline.addVertex(x, y);
		outline.addVertex(x, y);
		outline.addVertex(x, y);
	}

	float size = (shapeTypeSelected == S_POINT) ? pointSize : lineWidth;
	Shape shapeDrawn = initShape(shapeTypeSelected, outline.getAllVertices(), size, color);

	drawingFrame = new Frame();
	drawingFrame->setOutline(outline);
	drawingFrame->setShapeDrawn(shapeDrawn);
	if (shapeTypeSelected == S_POINT) {
		drawingFrame->setActive(false);
	}

	drawingFrames.push_back(drawingFrame);
}

void handleContinueDraw(float x, float y) {
	if (!drawStart || drawingFrame == NULL) {
		return;
	}

	Shape outline;
	outline.setShapeType(drawingFrame->getOutline().getShapeType());

	if (outline.getShapeType() == S_POINT) {
		outline.addVertex(x, y);
	}
	else if (outline.getShapeType() == LINE) {
		outline.addVertex(mouseDownPoint);
		outline.addVertex(x, y);
	}
	else {
		if (x > mouseDownPoint.x) {
			if (y > mouseDownPoint.y) {
				outline.addVertex(x, y);
				outline.addVertex(x, mouseDownPoint.y);
				outline.addVertex(mouseDownPoint);
				outline.addVertex(mouseDownPoint.x, y);
			}
			else {
				outline.addVertex(x, mouseDownPoint.y);
				outline.addVertex(x, y);
				outline.addVertex(mouseDownPoint.x, y);
				outline.addVertex(mouseDownPoint);
			}
		}
		else {
			if (y > mouseDownPoint.y) {
				outline.addVertex(mouseDownPoint.x, y);
				outline.addVertex(mouseDownPoint);
				outline.addVertex(x, mouseDownPoint.y);
				outline.addVertex(x, y);
			}
			else {
				outline.addVertex(mouseDownPoint);
				outline.addVertex(mouseDownPoint.x, y);
				outline.addVertex(x, y);
				outline.addVertex(x, mouseDownPoint.y);
			}
		}
	}

	Shape shapeDrawn = initShape(drawingFrame->getShapeDrawn().getShapeType(), outline.getAllVertices(),
									drawingFrame->getShapeDrawn().getSize(), drawingFrame->getShapeDrawn().getColor());

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
		if (drawingFrame->getShapeDrawn().getShapeType() != S_POINT) {
			delete drawingFrame;
			drawingFrame = NULL;

			drawingFrames.pop_back();
		}
	}

	glutPostRedisplay();
}

void handleDragDrawingFrame(float x, float y) {
	if (drawingFrame == NULL) {
		return;
	}

	if (activeFrameClicked) {
		float offsetX = x - mouseDragPoint.x;
		float offsetY = y - mouseDragPoint.y;

		Shape outline;
		outline.setShapeType(drawingFrame->getOutline().getShapeType());

		if (outline.getShapeType() == LINE) {
			outline.addVertex(drawingFrame->getOutline().getVertex(0).x + offsetX,
								drawingFrame->getOutline().getVertex(0).y + offsetY);
			outline.addVertex(drawingFrame->getOutline().getVertex(1).x + offsetX,
								drawingFrame->getOutline().getVertex(1).y + offsetY);
		}
		else {
			outline.addVertex(drawingFrame->getOutline().getVertex(0).x + offsetX,
								drawingFrame->getOutline().getVertex(0).y + offsetY);
			outline.addVertex(drawingFrame->getOutline().getVertex(1).x + offsetX,
								drawingFrame->getOutline().getVertex(1).y + offsetY);
			outline.addVertex(drawingFrame->getOutline().getVertex(2).x + offsetX,
								drawingFrame->getOutline().getVertex(2).y + offsetY);
			outline.addVertex(drawingFrame->getOutline().getVertex(3).x + offsetX,
								drawingFrame->getOutline().getVertex(3).y + offsetY);
		}

		Shape shapeDrawn = initShape(drawingFrame->getShapeDrawn().getShapeType(), outline.getAllVertices(),
										drawingFrame->getShapeDrawn().getSize(), drawingFrame->getShapeDrawn().getColor());

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
		if (drawingFrame->getOutline().getShapeType() == LINE) {
			Vertex2F startVertex = drawingFrame->getOutline().getVertex(0);
			Vertex2F endVertex = drawingFrame->getOutline().getVertex(1);

			if (startVertex.x > endVertex.x) {
				if (x < endVertex.x || x > startVertex.x) {
					return false;
				}
			}
			else {
				if (x < startVertex.x || x > endVertex.x) {
					return false;
				}
			}

			if (startVertex.y > endVertex.y) {
				if (y < endVertex.y || y > startVertex.y) {
					return false;
				}
			}
			else {
				if (y < startVertex.y || y > endVertex.y) {
					return false;
				}
			}

			float slopeOriginal = (endVertex.y - startVertex.y) / (endVertex.x - startVertex.x);
			float slopeNew = (endVertex.y - y) / (endVertex.x - x);
			if (slopeNew > (slopeOriginal - 0.1) && slopeNew < (slopeOriginal + 0.1)) {
				return true;
			}
		}
		else {
			Vertex2F topLeftVertex = drawingFrame->getOutline().getVertex(3);
			Vertex2F bottomRightVertex = drawingFrame->getOutline().getVertex(1);

			if ((x > topLeftVertex.x && x < bottomRightVertex.x) &&
				(y > bottomRightVertex.y && y < topLeftVertex.y)) {
				return true;
			}
		}
	}

	return false;
}

void mouseDrag(int x, int y) {
	handleDragDrawingFrame(x, windowHeight - y);

	mouseDragPoint.x = x;
	mouseDragPoint.y = windowHeight - y;

	handleContinueDraw(x, windowHeight - y);
}

void mouseClick(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) {
			mouseDownPoint.x = x;
			mouseDownPoint.y = windowHeight - y;

			if ((windowHeight - y) > (windowHeight - UI_TOOLBAR_HEIGHT)) {
				activeFrameClicked = false;

				if (drawingFrame != NULL) {
					drawingFrame->setActive(false);
				}

				return;
			}

			if (activeFrameClicked = isActiveFrameClicked(x, windowHeight - y)) {
				mouseDragPoint.x = mouseDownPoint.x;
				mouseDragPoint.y = mouseDownPoint.y;
				
				return;
			}

			handleStartDraw(x, windowHeight - y);
		}

		if (state == GLUT_UP) {
			mouseUpPoint.x = x;
			mouseUpPoint.y = windowHeight - y;

			handleCompleteDraw(x, windowHeight - y);
		}

		return;
	}
}

void renderScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	draw();

	setUIButtonClicked();
	renderUIButton();

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

void processMenuEvents(int option) {
	switch (option) {
		case CLEAR:
			disposeDrawingFrames();
			glutPostRedisplay();
			break;
		case EXIT:
			glutDestroyWindow(windowId);
			exit(0);
			break;
	}
}

void processColorMenuEvents(int option) {
	color = COLORS[option];

	if (drawingFrame == NULL) {
		return;
	}

	Shape shapeDrawn = initShape(drawingFrame->getShapeDrawn().getShapeType(), drawingFrame->getOutline().getAllVertices(),
									drawingFrame->getShapeDrawn().getSize(), color);

	drawingFrame->setShapeDrawn(shapeDrawn);

	glutPostRedisplay();
}

void processPointSizeMenuEvents(int option) {
	switch (option) {
		case M_SIZE_1:
			pointSize = 1.0f;
			break;
		case M_SIZE_2:
			pointSize = 2.0f;
			break;
		case M_SIZE_3:
			pointSize = 3.0f;
			break;
		case M_SIZE_4:
			pointSize = 4.0f;
			break;
		case M_SIZE_5:
			pointSize = 5.0f;
			break;
		case M_SIZE_6:
			pointSize = 6.0f;
			break;
		case M_SIZE_7:
			pointSize = 7.0f;
			break;
	}

	if (drawingFrame == NULL) {
		return;
	}

	float size = (drawingFrame->getShapeDrawn().getShapeType() == S_POINT) ? pointSize : lineWidth;
	Shape shapeDrawn = initShape(drawingFrame->getShapeDrawn().getShapeType(), drawingFrame->getOutline().getAllVertices(),
									size, drawingFrame->getShapeDrawn().getColor());

	drawingFrame->setShapeDrawn(shapeDrawn);

	glutPostRedisplay();
}

void processLineWidthMenuEvents(int option) {
	switch (option) {
		case M_SIZE_1:
			lineWidth = 1.0f;
			break;
		case M_SIZE_2:
			lineWidth = 2.0f;
			break;
		case M_SIZE_3:
			lineWidth = 3.0f;
			break;
		case M_SIZE_4:
			lineWidth = 4.0f;
			break;
		case M_SIZE_5:
			lineWidth = 5.0f;
			break;
		case M_SIZE_6:
			lineWidth = 6.0f;
			break;
		case M_SIZE_7:
			lineWidth = 7.0f;
			break;
	}

	if (drawingFrame == NULL) {
		return;
	}

	float size = (drawingFrame->getShapeDrawn().getShapeType() == S_POINT) ? pointSize : lineWidth;
	Shape shapeDrawn = initShape(drawingFrame->getShapeDrawn().getShapeType(), drawingFrame->getOutline().getAllVertices(),
									size, drawingFrame->getShapeDrawn().getColor());

	drawingFrame->setShapeDrawn(shapeDrawn);

	glutPostRedisplay();
}

void createGLUTMenus() {
	int colorMenu = glutCreateMenu(processColorMenuEvents);
	glutAddMenuEntry("Black", BLACK);
	glutAddMenuEntry("White", WHITE);
	glutAddMenuEntry("Red", RED);
	glutAddMenuEntry("Green", GREEN);
	glutAddMenuEntry("Blue", BLUE);
	glutAddMenuEntry("Yellow", YELLOW);
	glutAddMenuEntry("Orange", ORANGE);
	glutAddMenuEntry("Purple", PURPLE);

	int pointSizeMenu = glutCreateMenu(processPointSizeMenuEvents);
	glutAddMenuEntry("1.0", M_SIZE_1);
	glutAddMenuEntry("2.0", M_SIZE_2);
	glutAddMenuEntry("3.0", M_SIZE_3);
	glutAddMenuEntry("4.0", M_SIZE_4);
	glutAddMenuEntry("5.0", M_SIZE_5);
	glutAddMenuEntry("6.0", M_SIZE_6);
	glutAddMenuEntry("7.0", M_SIZE_7);

	int lineWidthMenu = glutCreateMenu(processLineWidthMenuEvents);
	glutAddMenuEntry("1.0", M_SIZE_1);
	glutAddMenuEntry("2.0", M_SIZE_2);
	glutAddMenuEntry("3.0", M_SIZE_3);
	glutAddMenuEntry("4.0", M_SIZE_4);
	glutAddMenuEntry("5.0", M_SIZE_5);
	glutAddMenuEntry("6.0", M_SIZE_6);
	glutAddMenuEntry("7.0", M_SIZE_7);

	int menu = glutCreateMenu(processMenuEvents);
	glutAddSubMenu("Color", colorMenu);
	glutAddSubMenu("Point Size", pointSizeMenu);
	glutAddSubMenu("Line Width", lineWidthMenu);
	glutAddMenuEntry("Clear", CLEAR);
	glutAddMenuEntry("Exit", EXIT);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
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
	windowId = glutCreateWindow("CSCI336-Assignment1 Simple Paint");
	init();
	createGLUTMenus();

	glutDisplayFunc(renderScene);
	glutReshapeFunc(reshapeScene);
	glutMouseFunc(mouseClick);
	glutMotionFunc(mouseDrag);

	glutMainLoop();

	disposeUIButton();
	disposeDrawingFrames();

	return 0;
}