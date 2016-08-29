/*******************************************************************************
*	Name				: OOI CHOON HO
*	Student ID			: 4805604
*	Coursework Title	: CSCI336 Assignment 1
*	Objecttive			: Develop a simple 2D Paint program
*******************************************************************************/
#include "Icon.h"

// Constructor
Icon::Icon(float width, float height, Shape shape) {
	mWidth = width;
	mHeight = height;
	mShape = shape;
}

// Getters
float Icon::getWidth() {
	return mWidth;
}

float Icon::getHeight() {
	return mHeight;
}

Shape Icon::getShape() {
	return mShape;
}

// Setters
void Icon::setWidth(float width) {
	mWidth = width;
}

void Icon::setHeight(float height) {
	mHeight = height;
}

void Icon::setShape(Shape shape) {
	mShape = shape;
}

// Overloaded assignment operator
void Icon::operator=(const Icon& rhIcon) {
	mWidth = rhIcon.mWidth;
	mHeight = rhIcon.mHeight;
	mShape = rhIcon.mShape;
}