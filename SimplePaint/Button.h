/*******************************************************************************
*	Name				: OOI CHOON HO
*	Student ID			: 4805604
*	Coursework Title	: CSCI336 Assignment 1
*	Objecttive			: Develop a simple 2D Paint program
*******************************************************************************/
#pragma once

#ifndef _SHAPE_H_
#define _SHAPE_H_
#include "Shape.h"
#endif // _SHAPE_H_

#ifndef _ICON_H_
#define _ICON_H_
#include "Icon.h"
#endif // _ICON_H_

class Button {
	public:
		// Constructor
		Button() { mHeight = 0; mWidth = 0; };
		// Getters
		Shape getShape();
		Icon getIcon();
		float getHeight();
		float getWidth();
		// Setters
		void setShape(Shape);
		void setIcon(Icon);
		void setHeight(float);
		void setWidth(float);
		// Adds a vertex to the shape of button
		void addShapeVertex(Vertex2F);
		void addShapeVertex(float, float);
		// Adds the type of shape to the shape of button
		void addShapeType(ShapeType);
	private:
		// Attributes
		Shape mShape;
		Icon mIcon;
		float mHeight;
		float mWidth;
};

// Constant variables
const int MAX_BUTTON_COUNT = 8;
const float BUTTON_HEIGHT = 50.0;
const float BUTTON_WIDTH = 50.0;
const float BUTTON_PADDING_OUTER = 5.0;
const float BUTTON_PADDING_INNER = 10.0;
const ColorRGB3F BUTTON_COLOR = { 0.0f, 0.0f, 0.0f };
const ColorRGB3F BUTTON_COLOR_ACTIVE = { 0.0f, 0.0f, 1.0f };
const ShapeType SHAPE_TYPES[MAX_BUTTON_COUNT] = { S_POINT, LINE, TRIANGLE, TRIANGLE_F, RECTANGLE, RECTANGLE_F, OVAL, OVAL_F };