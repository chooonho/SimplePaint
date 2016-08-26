
#include "Button.h"

Shape Button::getShape() {
	return mShape;
}

Icon Button::getIcon() {
	return mIcon;
}

float Button::getHeight() {
	return mHeight;
}

float Button::getWidth() {
	return mWidth;
}

Vertex2F Button::getCenterPoint() {
	return mCenterPoint;
}

void Button::setShape(Shape shape) {
	mShape = shape;
}

void Button::setIcon(Icon icon) {
	mIcon = icon;
}

void Button::setHeight(float height) {
	mHeight = height;
}

void Button::setWidth(float width) {
	mWidth = width;
}

void Button::setCenterPoint(Vertex2F centerPoint) {
	mCenterPoint.x = centerPoint.x;
	mCenterPoint.y = centerPoint.y;
}

void Button::setCenterPoint(float x, float y) {
	mCenterPoint.x = x;
	mCenterPoint.y = y;
}

void Button::addShapeVertex(Vertex2F vertex) {
	mShape.addVertex(vertex);
}

void Button::addShapeVertex(float x, float y) {
	mShape.addVertex(x, y);
}

void Button::addShapeType(ShapeType shapeType) {
	mShape.setShapeType(shapeType);
}