
#include "Icon.h"

Icon::Icon(float width, float height, Vertex2F centerPoint, Shape shape) {
	mWidth = width;
	mHeight = height;
	mCenterPoint.x = centerPoint.x;
	mCenterPoint.y = centerPoint.y;
	mShape = shape;
}

float Icon::getWidth() {
	return mWidth;
}

float Icon::getHeight() {
	return mHeight;
}

Vertex2F Icon::getCenterPoint() {
	return mCenterPoint;
}

Shape Icon::getShape() {
	return mShape;
}

void Icon::setWidth(float width) {
	mWidth = width;
}

void Icon::setHeight(float height) {
	mHeight = height;
}

void Icon::setCenterPoint(Vertex2F centerPoint) {
	mCenterPoint.x = centerPoint.x;
	mCenterPoint.y = centerPoint.y;
}

void Icon::setCenterPoint(float x, float y) {
	mCenterPoint.x = x;
	mCenterPoint.y = y;
}

void Icon::setShape(Shape shape) {
	mShape = shape;
}

void Icon::operator=(const Icon& rhIcon) {
	mWidth = rhIcon.mWidth;
	mHeight = rhIcon.mHeight;
	mCenterPoint.x = rhIcon.mCenterPoint.x;
	mCenterPoint.y = rhIcon.mCenterPoint.y;
	mShape = rhIcon.mShape;
}