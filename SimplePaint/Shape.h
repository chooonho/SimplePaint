#pragma once

#include <vector>

struct ColorRGB3F {
	float red;
	float green;
	float blue;
};

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

enum ColorName {
	BLACK,
	WHITE,
	RED,
	GREEN,
	BLUE,
	YELLOW,
	ORANGE,
	PURPLE
};

class Shape {
	public:
		Shape() { mIsFilled = false; mShapeType = NONE; };
		Shape(std::vector<Vertex2F>, bool, ShapeType);
		Vertex2F getVertex(int);
		bool getIsFilled();
		float getSize();
		ColorRGB3F getColor();
		ShapeType getShapeType();
		std::vector<Vertex2F> getAllVertices();
		void addVertex(Vertex2F);
		void addVertex(float, float);
		void clearAllVertices();
		void setVertices(std::vector<Vertex2F>);
		void setIsFilled(bool);
		void setSize(float);
		void setColor(ColorRGB3F);
		void setColor(float, float, float);
		void setShapeType(ShapeType);
		void operator=(const Shape&);
	private:
		std::vector<Vertex2F> mVertices;
		bool mIsFilled;
		float mSize;
		ShapeType mShapeType;
		ColorRGB3F mColor;
};

const int MAX_COLOR_COUNT = 8;
const double MATH_PI = 3.141592;
const ColorRGB3F COLORS[MAX_COLOR_COUNT] = {
	// Black
	// White
	// Red
	// Green
	// Blue
	// Yellow
	// Orange
	// Purple
	{ 0.0f, 0.0f, 0.0f },
	{ 1.0f, 1.0f, 1.0f },
	{ 1.0f, 0.0f, 0.0f},
	{ 0.0f, 1.0f, 0.0f },
	{ 0.0f, 0.0f, 1.0f },
	{ 1.0f, 1.0f, 0.0f },
	{ 1.0f, 0.647f, 0.0f },
	{ 0.502f, 0.0f, 0.502f },
};

