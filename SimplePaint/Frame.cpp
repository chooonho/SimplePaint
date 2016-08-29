/*******************************************************************************
*	Name				: OOI CHOON HO
*	Student ID			: 4805604
*	Coursework Title	: CSCI336 Assignment 1
*	Objecttive			: Develop a simple 2D Paint program
*******************************************************************************/
#include "Frame.h"

// Getters
Shape Frame::getOutline() {
	return mOutline;
}

Shape Frame::getShapeDrawn() {
	return mShapeDrawn;
}

bool Frame::getActive() {
	return mActive;
}

// Setters
void Frame::setShapeDrawn(Shape shapeDrawn) {
	mShapeDrawn = shapeDrawn;
}

void Frame::setOutline(Shape outline) {
	mOutline = outline;
}

void Frame::setActive(bool active) {
	mActive = active;
}

// Clear the outline of the frame
void Frame::clearOutline() {
	mOutline.clearAllVertices();
	mOutline.setIsFilled(false);
	mOutline.setShapeType(NONE);
}

// Clear the shape drawn contained in the frame
void Frame::clearShapeDrawn() {
	mShapeDrawn.clearAllVertices();
	mShapeDrawn.setIsFilled(false);
	mShapeDrawn.setShapeType(NONE);
}