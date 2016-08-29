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
#endif

class Frame {
	public:
		// Constructor
		Frame() { mActive = true; }
		// Getters
		Shape getOutline();
		Shape getShapeDrawn();
		bool getActive();
		// Setters
		void setShapeDrawn(Shape);
		void setOutline(Shape);
		void setActive(bool);
		// Clear the outline of the frame
		void clearOutline();
		// Clear the shape drawn contained in the frame
		void clearShapeDrawn();
	private:
		// Attributes
		bool mActive;
		Shape mOutline;
		Shape mShapeDrawn;
};

// Constant variable
const ColorRGB3F OUTLINE_COLOR = { 0.0f, 0.25f, 1.0f };