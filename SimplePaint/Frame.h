#pragma once

#ifndef _SHAPE_H_
#define _SHAPE_H_
#include "Shape.h"
#endif

class Frame {
	public:
		Frame() { mHeight = 0; mWidth = 0; }
		Shape getOutline();
		Shape getShapeDrawn();
		float getHeight();
		float getWidth();
		void setHeight(float);
		void setWidth(float);
		void setShapeDrawn(Shape);
		void setOutline(Shape);
		void clearOutline();
		void clearShapeDrawn();
	private:
		float mHeight;
		float mWidth;
		Shape mOutline;
		Shape mShapeDrawn;
};