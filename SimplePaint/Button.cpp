/*******************************************************************************
*	Name				: OOI CHOON HO
*	Student ID			: 4805604
*	Coursework Title	: CSCI336 Assignment 1
*	Objecttive			: Develop a simple 2D Paint program
*******************************************************************************/
#include "Button.h"

//Getters
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

// Setters
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

// Adds a vertex to the shape of button
void Button::addShapeVertex(Vertex2F vertex) {
	mShape.addVertex(vertex);
}

void Button::addShapeVertex(float x, float y) {
	mShape.addVertex(x, y);
}

// Adds the type of shape to the shape of button
void Button::addShapeType(ShapeType shapeType) {
	mShape.setShapeType(shapeType);
}