#pragma once

#ifndef _SHAPE_H_
#define _SHAPE_H_
#include "Shape.h"
#endif

class Frame {
	public:
		Frame() { mActive = true; }
		Shape getOutline();
		Shape getShapeDrawn();
		bool getActive();
		void setShapeDrawn(Shape);
		void setOutline(Shape);
		void setActive(bool);
		void clearOutline();
		void clearShapeDrawn();
	private:
		bool mActive;
		Shape mOutline;
		Shape mShapeDrawn;
};