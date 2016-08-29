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


class Icon {
	public:
		// Constructors
		Icon() { mWidth = 0; mHeight = 0; };
		Icon(float, float, Shape);
		// Getters
		float getWidth();
		float getHeight();
		Shape getShape();
		// Setters
		void setWidth(float);
		void setHeight(float);
		void setShape(Shape);
		// Overloaded assignment operator
		void operator=(const Icon&);
	private:
		// Attributes
		float mWidth;
		float mHeight;
		Shape mShape;
};

// Constant variables
const ColorRGB3F ICON_COLOR = { 0.0f, 0.0f, 0.0f };