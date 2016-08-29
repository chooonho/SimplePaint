/*******************************************************************************
*	Name				: OOI CHOON HO
*	Student ID			: 4805604
*	Coursework Title	: CSCI336 Assignment 1
*	Objecttive			: Develop a simple 2D Paint program
*******************************************************************************/
#include "Shape.h"

// Constructors
Shape::Shape(std::vector<Vertex2F> vertices, bool isFilled, ShapeType shapeType) {
	mVertices = vertices;
	mIsFilled = isFilled;
	mShapeType = shapeType;
}

// Getters
Vertex2F Shape::getVertex(int pos) {
	return mVertices[pos];
}

bool Shape::getIsFilled() {
	return mIsFilled;
}

float Shape::getSize() {
	return mSize;
}

ColorRGB3F Shape::getColor() {
	return mColor;
}

std::vector<Vertex2F> Shape::getAllVertices() {
	return mVertices;
}

ShapeType Shape::getShapeType() {
	return mShapeType;
}

// Setters
void Shape::setVertices(std::vector<Vertex2F> vertices) {
	mVertices = vertices;
}

void Shape::setIsFilled(bool isFilled) {
	mIsFilled = isFilled;
}

void Shape::setShapeType(ShapeType shapeType) {
	mShapeType = shapeType;
}

void Shape::setSize(float size) {
	mSize = size;
}

void Shape::setColor(ColorRGB3F color) {
	mColor = color;
}

void Shape::setColor(float red, float green, float blue) {
	mColor.red = red;
	mColor.green = green;
	mColor.blue = blue;
}

// Adds a vertex to the shape
void Shape::addVertex(Vertex2F newVertex) {
	mVertices.push_back(newVertex);
}

void Shape::addVertex(float x, float y) {
	Vertex2F newVertex;
	newVertex.x = x;
	newVertex.y = y;

	mVertices.push_back(newVertex);
}

// Clears all the vertices contained in the shape
void Shape::clearAllVertices() {
	mVertices.clear();
}

// Overloaded assignment operator
void Shape::operator=(const Shape& rhShape) {
	mVertices = rhShape.mVertices;
	mIsFilled = rhShape.mIsFilled;
	mShapeType = rhShape.mShapeType;
	mSize = rhShape.mSize;
	mColor = rhShape.mColor;
}