#pragma once

class Rect
{
protected:
	float posX, posY;
	float width, height;

public:
	Rect();
	virtual ~Rect();

	float getX();		void setX(float x);
	float getY();		void setY(float y);
	float getWidth();	void setWidth(float width);
	float getHeight();	void setHeight(float height);

	void setSize(float width, float height);

	void move(float dx, float dy);
	void moveX(float dx);	// Di chuyển ngang 1 đoạn dx nhập vào
	void moveY(float dy);
	void moveTo(float x, float y);

	float getLeft();
	float getRight();
	float getTop();
	float getBottom();
	float getMidX();
	float getMidY();
};

