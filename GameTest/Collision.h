
#pragma once
#include "app\app.h"

/*------------------------------------------------------------------------
  Script by: Nathan Alphonse
  Email: Nathan_Alphonse@hotmail.com
  goal:
  - hitbox class created with own draw call for debugging
//------------------------------------------------------------------------
*/
class Collision
{
public:
	Collision(float posX, float posY, float s, float w, float h);
	~Collision();

//private:
	float x, y;
	float scale;
	float width, height;
	float originalW, originalH;
	bool isHit;

	float r, g, b;
	void DrawHitBox();
	bool CollisionCheck(Collision *other);
	void SetScale(float ScaleX, float ScaleY, bool isSquare);
};

Collision::Collision(float posX = 0, float posY = 0, float s = 1, float w = 1, float h = 1)
	: x(posX), y(posY), scale(s), originalW(w), originalH(h), isHit(false),
	width(w), height(h)
{
	r = 1;
	g = 1;
	b = 1;
}

void Collision::DrawHitBox() {
	float w(width / 2), h(height / 2);

	if (isHit) {
		App::DrawLine(x - w, y - h, x + w, y + h, r, 0, 0);
	}
	else {
		App::DrawLine(x - w, y - h, x + w, y + w, r, g, b);
		App::DrawLine(x - w, y - h, x + w, y - h, r, g, b);
		App::DrawLine(x - w, y + h, x + w, y + h, r, g, b);
		App::DrawLine(x - w, y - h, x - w, y + h, r, g, b);
		App::DrawLine(x + w, y - h, x + w, y + h, r, g, b);
	}
}
// 1 value will uniformly scale, bool is false required if not
void Collision::SetScale(float scaleX = 1, float scaleY = 1, bool isSquare = true)
{
	scaleY = (isSquare) ? scaleX : scaleY;
	width = originalW * scaleX;
	height = originalH * scaleY;
	scale = (scaleX + scaleY) / 2;
}

bool Collision::CollisionCheck(Collision *other)
{
	//this collider
	float x1L(x - width / 2), x1R(x + width / 2);
	float y1T(y + height/ 2), y1B(y - height/ 2);
	//other collider
	float x2L(other->x - other->width / 2), x2R(other->x + other->width / 2);
	float y2T(other->y + other->height / 2), y2B(other->y - other->height / 2);

	if (((x1L <= x2R) && (x1R > x2L)) && ((y1B <= y2T) && (y1T > y2B))) {
		isHit = true;
		return true;
	}

	isHit = false;
	return false;
}

Collision::~Collision()
{
}