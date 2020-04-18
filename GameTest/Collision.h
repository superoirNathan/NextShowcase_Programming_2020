#pragma once
#include "app\app.h"


class Collision
{
public:
	Collision();
	Collision(float posX = 0, float posY = 0, float s = 1, float w = 1, float h = 1);
	~Collision();

//private:
	float x, y;
	float scale;
	float width, height;
	bool isHit;

	float r, g, b;
	void DrawHitBox();
	bool CollisionCheck();
};

Collision::Collision()
	:x(0),y(0),scale(1),width(1),height(1),isHit(false)
{
	r = 1;
	g = 1;
	b = 1;
}
Collision::Collision(float posX = 0, float posY = 0, float s = 1, float w = 1, float h = 1)
	: x(posX), y(posY), scale(s), width(w), height(h), isHit(false)
{
	r = 1;
	g = 1;
	b = 1;
}
void Collision::DrawHitBox() {
	float w(width / 2), h(height / 2);

	if (isHit) {
		App::DrawLine(x - w, y - h, x + w, y + w, r, g, b);
	}
	else {
		App::DrawLine(x - w, y - h, x + w, y + w, r, 0, 0);
	}
}

Collision::~Collision()
{
}