/*------------------------------------------------------------------------
  Script Modified by: Nathan Alphonse
  Email: Nathan_Alphonse@hotmail.com
  Main changes: 
  - Added 4 points where a sprite will randombly lerp towards
  - When the player pressed B button while test sprite and board sprite overlap
	- a uniqe sound will play depending on which board piece
  - player classe added to bundle script with a hitbox
  - hitbox class created with own draw call for debugging
//------------------------------------------------------------------------
*/

//------------------------------------------------------------------------
// GameTest.cpp
//------------------------------------------------------------------------
#include "stdafx.h"
//------------------------------------------------------------------------
#include <windows.h> 
#include <math.h>  
//------------------------------------------------------------------------
#include "app\app.h"
//------------------------------------------------------------------------
#include "Collision.h"
#include "Player.h"
#include <iostream>
#include <string>

CSimpleSprite *testSprite;
CSimpleSprite *testSprite2;
CSimpleSprite* background;

float right(APP_INIT_WINDOW_WIDTH), bot(APP_INIT_WINDOW_HEIGHT);
float lStickX(0.0f), lStickY(0.0f);

Collision *testBox;
Collision* testBox2;
Player* player;
const int boardSize(5);
Player* board[boardSize];
// Board
CSimpleSprite* rightSprite;
CSimpleSprite* botSprite;
CSimpleSprite* leftSprite;
CSimpleSprite* topSprite;

Collision* rightBox;
Collision* botBox;
Collision* leftBox;
Collision* topBox;
float offset(50.0f);

const int soundSize(4);
std::string sound1(".\\TestData\\Test.wav");
std::string sound2(".\\TestData\\Test2.wav");
std::string sound3(".\\TestData\\Test3.wav");
std::string sound4(".\\TestData\\Test4.wav");
std::string sounds[soundSize]{sound1,sound2,sound3,sound4};

std::string sound5(".\\TestData\\Test5.wav"); // background

int targetLocation(0);
int counter(0);
enum
{
	ANIM_FORWARDS,
	ANIM_BACKWARDS,
	ANIM_LEFT,
	ANIM_RIGHT,
};
//------------------------------------------------------------------------

//------------------------------------------------------------------------
// Called before first update. Do any initial setup here.
//------------------------------------------------------------------------
void Init()
{

	//------------------------------------------------------------------------
	// Example Sprite Code....
	
	testSprite = App::CreateSprite(".\\TestData\\Test.bmp", 8, 4);
	testSprite->SetPosition(400.0f, 400.0f);
	float speed = 1.0f / 15.0f;
	testSprite->CreateAnimation(ANIM_BACKWARDS, speed, { 0,1,2,3,4,5,6,7 });
	testSprite->CreateAnimation(ANIM_LEFT, speed, { 8,9,10,11,12,13,14,15 });
	testSprite->CreateAnimation(ANIM_RIGHT, speed, { 16,17,18,19,20,21,22,23 });
	testSprite->CreateAnimation(ANIM_FORWARDS, speed, { 24,25,26,27,28,29,30,31 });
	testSprite->SetScale(2.0f);

	testSprite2 = App::CreateSprite(".\\TestData\\Ships.bmp", 2, 12);
	testSprite2->SetPosition(400.0f, 400.0f);	
	testSprite2->SetFrame(2);
	testSprite2->SetScale(0.5f);

	background = App::CreateSprite(".\\TestData\\test2.bmp", 1, 1);
	background->SetPosition(right/2, bot/2);
	background->SetFrame(0);
	background->SetScale(2.8);

	//------------------------------------------------------------------------
	rightSprite = App::CreateSprite(".\\TestData\\Ships.bmp", 2, 12);
	rightSprite->SetPosition(right - offset, bot/2 );
	rightSprite->SetFrame(0);
	rightSprite->SetScale(1.0f);
	rightBox = new Collision(1, 1, rightSprite->GetScale(), rightSprite->GetWidth() / 3, rightSprite->GetHeight() / 3);
	rightBox->r = 0;
	rightBox->g = 0;

	botSprite = App::CreateSprite(".\\TestData\\Ships.bmp", 2, 12);
	botSprite->SetPosition(right / 2, 0 + offset);
	botSprite->SetFrame(4);
	botSprite->SetScale(1.0f);
	botBox = new Collision(1, 1, botSprite->GetScale(), botSprite->GetWidth() / 3, botSprite->GetHeight() / 3);
	botBox->r = 0;
	botBox->b = 0;

	leftSprite = App::CreateSprite(".\\TestData\\Ships.bmp", 2, 10);
	leftSprite->SetPosition(0 + offset, bot/2);
	leftSprite->SetFrame(6);
	leftSprite->SetScale(1.0f);
	leftBox = new Collision(1, 1, leftSprite->GetScale(), leftSprite->GetWidth() / 3, leftSprite->GetHeight() / 3);
	leftBox->g = 0; // purple

	topSprite = App::CreateSprite(".\\TestData\\Ships.bmp", 2, 10);
	topSprite->SetPosition(right/2 , bot - offset);
	topSprite->SetFrame(10);
	topSprite->SetScale(1.0f);
	topBox = new Collision(1, 1, topSprite->GetScale(), topSprite->GetWidth() / 3, topSprite->GetHeight() / 3);
	topBox->b = 0; // yellow

	//------------------------------------------------------------------------

	for (int i = 0; i < soundSize; i++) {
		App::PlaySound(sounds[i].c_str());
		App::StopSound(sounds[i].c_str());
	}

	testBox = new Collision(1,1, testSprite->GetScale(), testSprite->GetWidth()/3, testSprite->GetHeight()/2);
	testSprite->GetPosition(testBox->x, testBox->g);

	player = new Player();
	testBox2 = new Collision(1, 1, testSprite2->GetScale(), testSprite2->GetWidth() / 3, testSprite2->GetHeight() / 3);
	player->sprite = testSprite2;
	player->hitbox = testBox2;

	for (int i = 0; i < boardSize; i++) {
		board[i] =  new Player();
	}

	board[0]->sprite = rightSprite;
	board[1]->sprite = botSprite;
	board[2]->sprite = leftSprite;
	board[3]->sprite = topSprite;

	board[0]->hitbox = rightBox;
	board[1]->hitbox = botBox;
	board[2]->hitbox = leftBox;
	board[3]->hitbox = topBox;


	// dummy board to lerp towards
	board[4]->sprite = App::CreateSprite(".\\TestData\\Ships.bmp", 1,2);
	board[4]->sprite->SetPosition(right / 2, bot / 2);
	board[4]->sprite->SetScale(0.001);
	board[4]->hitbox = new Collision();

	App::PlaySound(sound5.c_str(), true);
}

//------------------------------------------------------------------------
// Update your simulation here. deltaTime is the elapsed time since the last update in ms.
// This will be called at no greater frequency than the value of APP_MAX_FRAME_RATE
//------------------------------------------------------------------------

void MoveSprite(CSimpleSprite* sprite, float distX = 0.0f, float distY = 0.0f) {
	float x, y;
	sprite->GetPosition(x, y);
	sprite->SetPosition(x + distX, y + distY);
}
void SetSprite(CSimpleSprite* sprite, float x2 = 0.0f, float y2 = 0.0f) {
	sprite->SetPosition(x2, y2);
}


bool MoveSpriteTowards(CSimpleSprite* a, CSimpleSprite* b, float step) {
	float ax(0), ay(0);
	a->GetPosition(ax,ay);
	float bx(0), by(0);
	b->GetPosition(bx, by);
	float dirx = bx - ax;
	float diry = by - ay;
	if (abs(dirx) < 2.5)
		return false;
	if (abs(diry) < 2.5)
		return false;

	float length = std::sqrt((dirx * dirx) + (diry * diry));
	dirx /= length;
	diry /= length;

	ax += dirx * step/2;
	ay += diry * step/2;

	a->SetPosition(ax, ay);
	return true;
}
void Update(float deltaTime)
{
	//------------------------------------------------------------------------
	// Update
	player->Update(deltaTime);	
	
	for (int i = 0; i < boardSize; i++) {
		board[i]->Update(deltaTime);
		board[i]->sprite->GetPosition(board[i]->hitbox->x, board[i]->hitbox->y);
	}

	testSprite->Update(deltaTime);
	testSprite->GetPosition(testBox->x, testBox->y);
	testBox->SetScale(testSprite->GetScale());

	// get stick output
	lStickX = App::GetController().GetLeftThumbStickX();
	lStickY = App::GetController().GetLeftThumbStickY();
	
	// moves one sprite towards another sprite. Returns false if too close
	if (!MoveSpriteTowards(testSprite, board[targetLocation]->sprite,deltaTime)) {
		targetLocation = (rand() % boardSize);
	}
	
	// move player sprite through stick
//	MoveSprite(testSprite, lStickX*2.0f, lStickY*2.0f);

	// check collisions
	for (int i = 0; i < boardSize; i++) {
		board[i]->hitbox->CollisionCheck(testBox);
	}
	player->hitbox->CollisionCheck(testBox);
	// Check left stick inputs
	if (lStickX > 0.5f)
	{
		testSprite->SetAnimation(ANIM_RIGHT);
		SetSprite(player->sprite, right - offset, bot / 2);
	}
	if (lStickX < -0.5f)
	{
		testSprite->SetAnimation(ANIM_LEFT);
		SetSprite(player->sprite, 0.0f + offset, bot / 2);
	}
	if (lStickY  > 0.5f)
	{
		testSprite->SetAnimation(ANIM_FORWARDS);
		SetSprite(player->sprite, right / 2, bot - offset);
	}
	if (lStickY < -0.5f)
	{
		testSprite->SetAnimation(ANIM_BACKWARDS);
		SetSprite(player->sprite, right / 2, 0.0f + offset);
	}
	if (App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_UP, false))
	{
		testSprite->SetScale(testSprite->GetScale() + 0.1f);
	}
	if (App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_DOWN, false))
	{
		testSprite->SetScale(testSprite->GetScale() - 0.1f);
	}
	if (App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_LEFT, false))
	{
		testSprite->SetAngle(testSprite->GetAngle() + 0.1f);
	}
	if (App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_RIGHT, false))
	{
		testSprite->SetAngle(testSprite->GetAngle() - 0.1f);
	}
	if (App::GetController().CheckButton(XINPUT_GAMEPAD_A, true))
	{
		testSprite->SetAnimation(-1);
	}
	//------------------------------------------------------------------------
	// Sound.
	//------------------------------------------------------------------------
	if (App::GetController().CheckButton(XINPUT_GAMEPAD_B, true))
	{
		// if overlap of player with board and target, play sound when b is pressed
		for (int i = 0; i < boardSize; i++) {
			if (board[i]->hitbox->CollisionCheck(player->hitbox)) {
				if (player->hitbox->CollisionCheck(testBox) && i != 4) {
					App::PlaySound(sounds[i].c_str());
					counter++;
				}
			}
		}
	}
}

//------------------------------------------------------------------------
// Add your display calls here (DrawLine,Print, DrawSprite.) 
// See App.h 
//------------------------------------------------------------------------
void Render()
{	
	// drawline sx,sy is start point & ex,ey is end point 
	// draw background first
	background->Draw();
	//------------------------------------------------------------------------
	testSprite->Draw();
	player->Draw();
	//------------------------------------------------------------------------
	testBox->DrawHitBox();
	for (int i = 0; i < boardSize; i++) {
		board[i]->Draw();
	}
	//------------------------------------------------------------------------
	// Example Text.
	//------------------------------------------------------------------------
	App::Print(100, 100, std::to_string(counter).c_str());

}
//------------------------------------------------------------------------
// Add your shutdown code here. Called when the APP_QUIT_KEY is pressed.
// Just before the app exits.
//------------------------------------------------------------------------
void Shutdown()
{	
	//------------------------------------------------------------------------
	// Example Sprite Code....
	delete testSprite;
	delete testSprite2;
	//------------------------------------------------------------------------
}