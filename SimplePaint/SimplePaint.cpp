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

// Constant variables
const ColorRGB3F BACKGROUND_COLOR = { 0.8f, 0.8f, 0.8f };
const int UI_TOOLBAR_HEIGHT = BUTTON_HEIGHT + (BUTTON_PADDING_OUTER * 2);
const int DEFAULT_POINT_SIZE = 1;
const int DEFAULT_LINE_WIDTH = 1;

// Global variables
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

// Enumeration for labelling menu events
enum MenuEvent {
	CLEAR,
	EXIT
};

// Enumeration for labelling the point size and line width
enum MarkerSize {
	M_SIZE_1,
	M_SIZE_2,
	M_SIZE_3,
	M_SIZE_4,
	M_SIZE_5,
	M_SIZE_6,
	M_SIZE_7
};

// Clean up the UI buttons
void disposeUIButton() {
	for (int i = 0; i < MAX_BUTTON_COUNT; i++) {
		delete ptrUIButtons.back();
		ptrUIButtons.pop_back();
	}
}

// Clean up the drawing frames (shape drawn)
void disposeDrawingFrames() {
	while (!drawingFrames.empty()) {
		delete drawingFrames.back();
		drawingFrames.back() = NULL;
		drawingFrames.pop_back();
	}

	drawingFrame = NULL;
}

// Initialize a shape based on its specification and vertices
Shape initShape(ShapeType shapeType, std::vector<Vertex2F> outlineVertices, float size, ColorRGB3F color) {
	// Sets the shape to its specification
	// Then determine and store the vertices of the shape based on the type of shapes
	// At the same time, check if the shape requires to be filled or just its wireframe
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

// Handles the process of drawing (Start)
void handleStartDraw(float x, float y) {
	// Do not do anything if no shape is selected
	// Deactivates the previous drawing frame if there is any
	// Turn on the switch to indicate that the process of drawing has started
	// Determines the outline of the drawing frame to be used based on the shape selected
	// Calls the function to get the shape drawn based on its specifications
	// Assign both the outline and the shape drawn to the drawing frame
	// Store the drawing frame for later use
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

	glutPostRedisplay();
}

// Handles the process of drawing (Continuous/Dragging)
void handleContinueDraw(float x, float y) {
	// Do not do anything if there is no indication of that drawing has started OR if there is no drawing frame available
	// Determines the outline of the drawing frame to be used based on the shape selected
	// Calls the function to get the shape drawn based on its specifications
	// Re-assign both the outline and the shape drawn to the drawing frame
	if (!drawStart || drawingFrame == NULL) {
		return;
	}

	Shape outline;
	outline.setShapeType(drawingFrame->getOutline().getShapeType());

	if (outline.getShapeType() == S_POINT) {
		return;
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

// Handles the process of drawing (Complete/End)
void handleCompleteDraw(float x, float y) {
	// Do not do anything if there is no indication of that drawing has started OR if there is no drawing frame available
	// Turn off the switch to indicates the process of drawing has ended
	// Dispose the previous drawing frame if there is nothing drawn
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

// Handles the dragging of active drawing frames
void handleDragDrawingFrame(float x, float y) {
	// Do not do anything if there is no drawing frame available
	// Do not do anything if there is no active drawing frame selected/clicked
	// Calculates the offset of both X and Y
	// Determine the outline to be used based on the specifications with new coordinates
	// Determine the shape to be used based on the specifications with new coordinates
	// Re-assign both the outline and the shape drawn to the drawing frame
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

// Check if there is an active drawing frame clicked
bool isActiveFrameClicked(float x, float y) {
	// Return false if there is no drawing frame available
	// Return false if the current drawing frame is not active
	// For drawing frame that holds a line:
	// - Check if the clicked position is within the drawing frame
	// - Check if the slope generated from clicked position and the line end point
	//   is the same as the original line (with max deviation of 0.1)
	// For drawing frame that holds other shapes:
	// - Check if the clicked position is within the drawing frame
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

// Callback function for mouse dragging
void mouseDrag(int x, int y) {
	// Call a function to handle the dragging of active drawing frames
	handleDragDrawingFrame(x, windowHeight - y);

	// Re-assign the new mouse draging position
	mouseDragPoint.x = x;
	mouseDragPoint.y = windowHeight - y;

	// Call a function to handle the continuation of drawing
	handleContinueDraw(x, windowHeight - y);
}

// Callback function for mouse click
void mouseClick(int button, int state, int x, int y) {
	// Mouse left button clicked:
	// - Re-assign the new mouse clicked position
	// - Do not start drawing if the mouse clicked position is within the UI toolbar OR
	//   if the mouse clicked position is within an active drawing frame
	// - If the previous condition does not hold, call a function to handle the start process of drawing
	// Mouse left button released:
	// - Re-assign the new mouse released position
	// - Call a function to handle the end process of drawing
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

			if ((activeFrameClicked = isActiveFrameClicked(x, windowHeight - y))) {
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

// Create an icon for a button
Icon makeIcon(ShapeType iconShapeType, std::vector<Vertex2F> buttonVertices) {
	// Re-position the vertices (to shrink the size of icon to fit it into the button)
	// Calculates the height and width of icon
	// Determines the vertices of the icon based on the type of shape
	// Gets the shape required based on the icon specifications
	// Assign the height, width and the shape to icon
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

// Creates UI button
void initUIButton() {
	// Gets the start position of the first UI button
	// Keeps the loop running until all the buton has been created and stored for later use
	// Within the loop, determines the new position of the vertices and icon of buttons
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

// Checked the UI button that is selected/clicked (type of shape selected)
void setUIButtonClicked() {
	// Do not do anything if the mouse clicked position is not within the UI toolbar
	// Go through all the buttons and checked the button that has been selected
	// Set the type of shape to the shape selected
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

// Draws the UI buttons
void renderUIButton() {
	// Set the line width and point size to be used
	// Go through all the buttons:
	// - Get the icon of the button and draw it
	// - Get the shape of the button and draw it
	// - At the end of the loop, draw an extra button that shows the color selected next to the last UI button
	glPointSize(DEFAULT_POINT_SIZE + 1);
	glLineWidth(DEFAULT_LINE_WIDTH + 1);

	for (int i = 0; i < MAX_BUTTON_COUNT; i++) {
		std::vector<Vertex2F> buttonVertices = ptrUIButtons[i]->getShape().getAllVertices();
		std::vector<Vertex2F> iconVertices = ptrUIButtons[i]->getIcon().getShape().getAllVertices();

		glColor3f(ICON_COLOR.red, ICON_COLOR.green, ICON_COLOR.blue);
		if (ptrUIButtons[i]->getIcon().getShape().getShapeType() == S_POINT) {
			glBegin(GL_POINTS);
				glVertex2f(iconVertices[0].x, iconVertices[0].y);
			glEnd();
		}
		else {
			if (ptrUIButtons[i]->getIcon().getShape().getIsFilled()) {
				if (ptrUIButtons[i]->getIcon().getShape().getShapeType() == TRIANGLE_F) {
					glBegin(GL_TRIANGLES);
				}
				else if (ptrUIButtons[i]->getIcon().getShape().getShapeType() == RECTANGLE_F) {
					glBegin(GL_QUADS);
				}
				else {
					glBegin(GL_TRIANGLE_FAN);
				}
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
			glColor3f(BUTTON_COLOR_ACTIVE.red, BUTTON_COLOR_ACTIVE.green, BUTTON_COLOR_ACTIVE.blue);
		}
		else {
			glColor3f(BUTTON_COLOR.red, BUTTON_COLOR.green, BUTTON_COLOR.blue);
		}
		glBegin(GL_LINE_LOOP);
		for (int j = 0; j < buttonVertices.size(); j++) {
			glVertex2f(buttonVertices[j].x, buttonVertices[j].y);
		}
		glEnd();

		if (i == MAX_BUTTON_COUNT - 1) {
			glColor3f(color.red, color.green, color.blue);
			glBegin(GL_QUADS);
			for (int j = 0; j < buttonVertices.size(); j++) {
				glVertex2f(buttonVertices[j].x + BUTTON_PADDING_OUTER + BUTTON_WIDTH, buttonVertices[j].y);
			}
			glEnd();

			glColor3f(BUTTON_COLOR.red, BUTTON_COLOR.green, BUTTON_COLOR.blue);
			glBegin(GL_LINE_LOOP);
			for (int j = 0; j < buttonVertices.size(); j++) {
				glVertex2f(buttonVertices[j].x + BUTTON_PADDING_OUTER + BUTTON_WIDTH, buttonVertices[j].y);
			}
			glEnd();
		}
	}
}

// Draws all the stored drawing frames
void draw() {
	// Go through all the drawing frames:
	// - Draw all the shapes contained in the frames
	// - Draw the outline of the drawing frame if it is active
	// Lastly, draw an overlay layer as the background of the UI toolbar
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
				if (shapeDrawn.getShapeType() == TRIANGLE_F) {
					glBegin(GL_TRIANGLES);
				}
				else if (shapeDrawn.getShapeType() == RECTANGLE_F) {
					glBegin(GL_QUADS);
				}
				else {
					glBegin(GL_TRIANGLE_FAN);
				}
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

			glColor3f(OUTLINE_COLOR.red, OUTLINE_COLOR.green, OUTLINE_COLOR.blue);
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

	glColor3f(BACKGROUND_COLOR.red, BACKGROUND_COLOR.green, BACKGROUND_COLOR.blue);
	glBegin(GL_POLYGON);
		glVertex2f(windowWidth, windowHeight);
		glVertex2f(windowWidth, windowHeight - UI_TOOLBAR_HEIGHT);
		glVertex2f(0, windowHeight - UI_TOOLBAR_HEIGHT);
		glVertex2f(0, windowHeight);
	glEnd();

	glLineWidth(DEFAULT_LINE_WIDTH + 1);
	glColor3f(BUTTON_COLOR.red, BUTTON_COLOR.green, BUTTON_COLOR.blue);
	glBegin(GL_LINES);
		glVertex2f(0, windowHeight - UI_TOOLBAR_HEIGHT);
		glVertex2f(windowWidth, windowHeight - UI_TOOLBAR_HEIGHT);
	glEnd();
}

// Callback function for glut display
void renderScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Calls the function to draw all the drawing frames
	draw();

	// Calls the function to set the UI button clicked and the type of shape selected
	// Draws the UI button
	setUIButtonClicked();
	renderUIButton();

	glutSwapBuffers();
}

// Callback function for glut reshape function
void reshapeScene(int width, int height) {
	// Do not let the height to be 0 (min height is 1)
	// Re-assign the height and width of the window
	// Clear up all the UI buttons then re-initialize them using the new window height and width
	if (height == 0) {
		height = 1;
	}

	windowWidth = width;
	windowHeight = height;
	
	disposeUIButton();
	initUIButton();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, width, height);
	gluOrtho2D(0, width, 0, height);
}

// Process the main menu events
void processMenuEvents(int option) {
	// For CLEAR:
	// - Clear up all the drawing frames
	// For EXIT:
	// - Destroys the current window then exit the program
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

// Process the color menu events
void processColorMenuEvents(int option) {
	// Assign the color to the color selected
	// Re-render UI button (to change the color of the last button that shows selected color)
	// If there is an active drawing frame, change the color of the shape drawn within the drawing frame
	color = COLORS[option];

	renderUIButton();

	if (drawingFrame != NULL && drawingFrame->getActive()) {
		Shape shapeDrawn = initShape(drawingFrame->getShapeDrawn().getShapeType(), drawingFrame->getOutline().getAllVertices(),
			drawingFrame->getShapeDrawn().getSize(), color);

		drawingFrame->setShapeDrawn(shapeDrawn);
	}

	glutPostRedisplay();
}

// Process the point size menu events
void processPointSizeMenuEvents(int option) {
	// Assign the point size to the size selected
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

	glutPostRedisplay();
}

// Process the line width menu events
void processLineWidthMenuEvents(int option) {
	// Assign the line width to the size selected
	// If there is an active drawing frame AND the shape is not a point,
	// change the line width of the shape drawn within the drawing frame
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

	if (!drawingFrame->getActive()) {
		return;
	}

	if (drawingFrame->getShapeDrawn().getShapeType() != S_POINT) {
		Shape shapeDrawn = initShape(drawingFrame->getShapeDrawn().getShapeType(), drawingFrame->getOutline().getAllVertices(),
										lineWidth, drawingFrame->getShapeDrawn().getColor());

		drawingFrame->setShapeDrawn(shapeDrawn);
	}

	glutPostRedisplay();
}

// Creates the popup menu and sub menus
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
	// Calls the function to initialize the UI buttons
	initUIButton();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(windowWidth, windowHeight);
	windowId = glutCreateWindow("CSCI336-Assignment1 Simple Paint");
	init();
	createGLUTMenus();

	// Assign the callback functions
	glutDisplayFunc(renderScene);
	glutReshapeFunc(reshapeScene);
	glutMouseFunc(mouseClick);
	glutMotionFunc(mouseDrag);

	glutMainLoop();

	// Before the program exits, clear up the UI buttons and drawing frames
	disposeUIButton();
	disposeDrawingFrames();

	return 0;
}