#pragma once

#ifndef _SHAPE_H_
#define _SHAPE_H_
#include "Shape.h"
#endif // _SHAPE_H_


class Icon {
	public:
		Icon() { mWidth = 0; mHeight = 0; };
		Icon(float, float, Shape);
		float getWidth();
		float getHeight();
		Shape getShape();
		void setWidth(float);
		void setHeight(float);
		void setShape(Shape);
		void operator=(const Icon&);
	private:
		float mWidth;
		float mHeight;
		Shape mShape;
};

const ColorRGB3F ICON_COLOR = { 0.0f, 0.0f, 0.0f };