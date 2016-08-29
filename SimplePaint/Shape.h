/*******************************************************************************
*	Name				: OOI CHOON HO
*	Student ID			: 4805604
*	Coursework Title	: CSCI336 Assignment 1
*	Objecttive			: Develop a simple 2D Paint program
*******************************************************************************/
#pragma once

#include <vector>

// Structure that holds the RGB values of the color
struct ColorRGB3F {
	float red;
	float green;
	float blue;
};

// Structure that holds the x and y values of a vertex
struct Vertex2F {
	float x;
	float y;
};

// Enumeration that labels the type of shapes
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

// Enumeration that labels the colors
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
		// Constructors
		Shape() { mIsFilled = false; mShapeType = NONE; };
		Shape(std::vector<Vertex2F>, bool, ShapeType);
		// Getters
		Vertex2F getVertex(int);
		bool getIsFilled();
		float getSize();
		ColorRGB3F getColor();
		ShapeType getShapeType();
		std::vector<Vertex2F> getAllVertices();
		// Setters
		void setVertices(std::vector<Vertex2F>);
		void setIsFilled(bool);
		void setSize(float);
		void setColor(ColorRGB3F);
		void setColor(float, float, float);
		void setShapeType(ShapeType);
		// Adds a vertex to the shape
		void addVertex(Vertex2F);
		void addVertex(float, float);
		// Clears all the vertices contained in the shape
		void clearAllVertices();
		// Overloaded assignment operator
		void operator=(const Shape&);
	private:
		// Attributes
		std::vector<Vertex2F> mVertices;
		bool mIsFilled;
		float mSize;
		ShapeType mShapeType;
		ColorRGB3F mColor;
};

// Constant variables
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

