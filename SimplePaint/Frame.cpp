
#include "Frame.h"

Shape Frame::getOutline() {
	return mOutline;
}

Shape Frame::getShapeDrawn() {
	return mShapeDrawn;
}

float Frame::getHeight() {
	return mHeight;
}

float Frame::getWidth() {
	return mWidth;
}

bool Frame::getActive() {
	return mActive;
}

void Frame::setHeight(float height) {
	mHeight = height;
}

void Frame::setWidth(float width) {
	mWidth = width;
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