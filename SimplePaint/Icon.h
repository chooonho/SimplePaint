#pragma once

#ifndef _SHAPE_H_
#define _SHAPE_H_
#include "Shape.h"
#endif // _SHAPE_H_


class Icon {
	public:
		Icon() { mWidth = 0; mHeight = 0; mCenterPoint.x = 0; mCenterPoint.y = 0; };
		Icon(float, float, Vertex2F, Shape);
		float getWidth();
		float getHeight();
		Vertex2F getCenterPoint();
		Shape getShape();
		void setWidth(float);
		void setHeight(float);
		void setCenterPoint(Vertex2F);
		void setCenterPoint(float, float);
		void setShape(Shape);
		void operator=(const Icon&);
	private:
		float mWidth;
		float mHeight;
		Vertex2F mCenterPoint;
		Shape mShape;
};