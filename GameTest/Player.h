#pragma once
#include "app\app.h"
#include <../App/SimpleSprite.h>
#include "Collision.h"
class Player
{
public:
	Player();
	~Player();

	

//private:
	CSimpleSprite* sprite;	
	Collision* hitbox;
	
	int health;
	float speed;
};

Player::Player()
{

}

Player::~Player()
{
}