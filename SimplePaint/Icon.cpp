
#include "Icon.h"

Icon::Icon(float width, float height, Shape shape) {
	mWidth = width;
	mHeight = height;
	mShape = shape;
}

float Icon::getWidth() {
	return mWidth;
}

float Icon::getHeight() {
	return mHeight;
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

void Icon::setShape(Shape shape) {
	mShape = shape;
}

void Icon::operator=(const Icon& rhIcon) {
	mWidth = rhIcon.mWidth;
	mHeight = rhIcon.mHeight;
	mShape = rhIcon.mShape;
}