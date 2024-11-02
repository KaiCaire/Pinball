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
class AcceleratingObstacle;
class Spring;
class Pikachu;



class ModuleGame : public Module
{
public:
	ModuleGame(Application* app, bool start_enabled = true);
	~ModuleGame();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

public:

	std::vector<PhysicEntity*> entities;

	PhysBody* sensor;
	Ball* ball;
	int ballRad = 10;
	Vector2 springForce = { 0.0f, -10.0f };
	bool sensed;

	Board* rubyBoard;
	Obstacle* rubyObstacle;
	AcceleratingObstacle* rubyAcceleratingObstacle;

	Texture2D emptyBoard;
	Texture2D ballTex;

	Spring* spoink;
	Texture2D spoinkSheet;
	Pikachu* pikachu;
	Texture2D pikachuSheet;

	Vector2 initBallPos = { 243 * 2, 250 * 2 };
};
