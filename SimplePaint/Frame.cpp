
#include "Frame.h"

Shape Frame::getOutline() {
	return mOutline;
}

Shape Frame::getShapeDrawn() {
	return mShapeDrawn;
}

bool Frame::getActive() {
	return mActive;
}

void Frame::setShapeDrawn(Shape shapeDrawn) {
	mShapeDrawn = shapeDrawn;
}

void Frame::setOutline(Shape outline) {
	mOutline = outline;
}

void Frame::setActive(bool active) {
	mActive = active;
}

void Frame::clearOutline() {
	mOutline.clearAllVertices();
	mOutline.setIsFilled(false);
	mOutline.setShapeType(NONE);
}

void Frame::clearShapeDrawn() {
	mShapeDrawn.clearAllVertices();
	mShapeDrawn.setIsFilled(false);
	mShapeDrawn.setShapeType(NONE);
}