#pragma once

#ifndef _SHAPE_H_
#define _SHAPE_H_
#include "Shape.h"
#endif

class Frame {
	public:
		Frame() { mActive = true; mHeight = 0; mWidth = 0; }
		Shape getOutline();
		Shape getShapeDrawn();
		float getHeight();
		float getWidth();
		bool getActive();
		void setHeight(float);
		void setWidth(float);
		void setShapeDrawn(Shape);
		void setOutline(Shape);
		void setActive(bool);
		void clearOutline();
		void clearShapeDrawn();
	private:
		bool mActive;
		float mHeight;
		float mWidth;
		Shape mOutline;
		Shape mShapeDrawn;
};