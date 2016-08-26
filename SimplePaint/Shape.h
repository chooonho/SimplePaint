#pragma once

#include <vector>

struct Vertex2F {
	float x;
	float y;
};

enum ShapeType {
	NONE,
	S_POINT,
	LINE,
	TRIANGLE,
	TRIANGLE_F,
	RECTANGLE,
	RECTANGLE_F,
	OVAL,
	OVAL_F
};

class Shape {
	public:
		Shape() { mIsFilled = false; mShapeType = NONE; };
		Shape(std::vector<Vertex2F>, bool, ShapeType);
		Vertex2F getVertex(int);
		bool getIsFilled();
		ShapeType getShapeType();
		std::vector<Vertex2F> getAllVertices();
		void addVertex(Vertex2F);
		void addVertex(float, float);
		void clearAllVertices();
		void setVertices(std::vector<Vertex2F>);
		void setIsFilled(bool);
		void setShapeType(ShapeType);
		void operator=(const Shape&);
	private:
		std::vector<Vertex2F> mVertices;
		bool mIsFilled;
		ShapeType mShapeType;
};
