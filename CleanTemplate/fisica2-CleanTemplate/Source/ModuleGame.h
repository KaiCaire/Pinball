#pragma once

#include "Globals.h"
#include "Module.h"

#include "p2Point.h"

#include "raylib.h"
#include <vector>

class PhysBody;
class PhysicEntity;
class Board;
class Ball;
class Obstacle;
class Spring;
class Pikachu;

class PalancaDer;
class PalancaIzq;

#define NoInteraction 1 

#define LeftImpulser 2 
#define RightImpulser 3 

#define Points 5
#define Impulser 6
#define PikachuImpulser 7 
#define SpringImpulser 8

#define Dead 9



class ModuleGame : public Module
{
public:
	ModuleGame(Application* app, bool start_enabled = true);
	~ModuleGame();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB, int dir);

public:

	std::vector<PhysicEntity*> entities;
	Music music;
	Music pointsSFX;
	Music deadSFX;


	PhysBody* sensor;
	Ball* ball;
	int ballRad = 15;
	Vector2 springForce = { 0.0f, -10.0f };
	bool sensed;
	bool canImpulse = false;

	Board* rubyBoard;
	Obstacle* rubyObstacle;

	Texture2D emptyBoard;
	Texture2D ballTex;
	Texture2D palancaderSheet;
	Texture2D palancaizqSheet;

	Spring* spoink;
	Texture2D spoinkSheet;
	Pikachu* pikachu;
	PalancaDer* palancaDer;
	PalancaIzq* palancaIzq;

	Texture2D pikachuSheet;

	Vector2 initBallPos = { 243 * 2, 250 * 2 };

	int cnt = 0;
	bool dead = false;

};
