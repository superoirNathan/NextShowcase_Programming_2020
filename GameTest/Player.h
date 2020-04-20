#pragma once
#include "app\app.h"
#include <../App/SimpleSprite.h>
#include "Collision.h"

/*------------------------------------------------------------------------
  Script by: Nathan Alphonse
  Email: Nathan_Alphonse@hotmail.com
  goal:
  - player classe added to bundle script with a hitbox
//------------------------------------------------------------------------
*/

class Player
{
public:
	Player();
	~Player();
	void Update(float deltaTime);
	void Draw();
//private:
	CSimpleSprite* sprite;	
	Collision* hitbox;
};

void Player::Update(float deltaTime) {
	sprite->Update(deltaTime);
	sprite->GetPosition(hitbox->x, hitbox->y);
	hitbox->SetScale(sprite->GetScale());
}
void Player::Draw() {
	if (sprite != nullptr)
		sprite->Draw();
	if (hitbox != nullptr)
		hitbox->DrawHitBox();
}

Player::Player()
{
}

Player::~Player()
{
	if (sprite != nullptr)
		delete sprite;
	if (hitbox != nullptr)
		delete hitbox;
}