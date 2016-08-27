
#include "Shape.h"

Shape::Shape(std::vector<Vertex2F> vertices, bool isFilled, ShapeType shapeType) {
	mVertices = vertices;
	mIsFilled = isFilled;
	mShapeType = shapeType;
}

Vertex2F Shape::getVertex(int pos) {
	return mVertices[pos];
}

bool Shape::getIsFilled() {
	return mIsFilled;
}

float Shape::getSize() {
	return mSize;
}

std::vector<Vertex2F> Shape::getAllVertices() {
	return mVertices;
}

ShapeType Shape::getShapeType() {
	return mShapeType;
}

void Shape::addVertex(Vertex2F newVertex) {
	mVertices.push_back(newVertex);
}

void Shape::addVertex(float x, float y) {
	Vertex2F newVertex;
	newVertex.x = x;
	newVertex.y = y;

	mVertices.push_back(newVertex);
}

void Shape::clearAllVertices() {
	mVertices.clear();
}

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

void Shape::operator=(const Shape& rhShape) {
	mVertices = rhShape.mVertices;
	mIsFilled = rhShape.mIsFilled;
	mShapeType = rhShape.mShapeType;
	mSize = rhShape.mSize;
}