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
		void setShape(Shape);
		void setIcon(Icon);
		void setHeight(float);
		void setWidth(float);
		void addShapeVertex(Vertex2F);
		void addShapeVertex(float, float);
		void addShapeType(ShapeType);
	private:
		Shape mShape;
		Icon mIcon;
		float mHeight;
		float mWidth;
};

const int MAX_BUTTON_COUNT = 8;
const float BUTTON_HEIGHT = 70.0;
const float BUTTON_WIDTH = 70.0;
const float BUTTON_PADDING_OUTER = 5.0;
const float BUTTON_PADDING_INNER = 10.0;
const ShapeType SHAPE_TYPES[MAX_BUTTON_COUNT] = { S_POINT, LINE, TRIANGLE, TRIANGLE_F, RECTANGLE, RECTANGLE_F, OVAL, OVAL_F };