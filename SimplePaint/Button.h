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
		Button() { };
		Shape getShape();
		Icon getIcon();
		float getHeight();
		float getWidth();
		Vertex2F getCenterPoint();
		void setShape(Shape);
		void setIcon(Icon);
		void setHeight(float);
		void setWidth(float);
		void setCenterPoint(Vertex2F);
		void setCenterPoint(float, float);
		void addShapeVertex(Vertex2F);
		void addShapeVertex(float, float);
	private:
		Shape mShape;
		Icon mIcon;
		float mHeight;
		float mWidth;
		Vertex2F mCenterPoint;
};