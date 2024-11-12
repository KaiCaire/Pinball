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
class Chinchou;
class Makuhita;
class Chikorita;

class Block;

class PalancaDer;
class PalancaIzq;

class RightFlipper;
class LeftFlipper;


#define NoInteraction 1 

#define LeftImpulser 2 
#define RightImpulser 3 

#define Points 5
#define Impulser 6
#define PikachuImpulser 7 
#define SpringImpulser 8
#define startBlocker 10

#define Dead 9

#define ChinchouBumper 11

class ModuleGame : public Module
{
public:
	ModuleGame(Application* app, bool start_enabled = true);
	~ModuleGame();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB, int dir);

	enum State{INGAME, DEAD, SCORE, WIN};
public:

	std::vector<PhysicEntity*> entities;
	Music music;
	Sound gameOverMusic;
	Sound extraLifeSound;
	Sound winMusic;
	Sound pointsSFX;
	Sound deadSFX;
	Sound impulserSFX;


	int flipperFX;
	int spoink_chargeSFX;
	int spoink_releaseSFX;
	int chinchou_hitSFX;

	int currentFrame = 0;
	float frameTime = 0.15f;   // Duración de cada frame en segundos
	float timer = 0.0f;
	Texture2D frames[8];

	int currentFrame_pikachu = 0;
	float frameTime_pikachu = 0.15f;   // Duración de cada frame en segundos
	float timer_pikachu = 0.0f;
	Texture2D frames_pikachu[2];

	Texture2D frames_Win[2];

	int currentFrames_latias = 1;
	float framesTime_latias = 0.08f;   // Duración de cada frame en segundos
	float timer_latias = 0.0f;
	Texture2D frames_Latias[14];
	bool backwards;

	int currentFrame_chinchou = 0;
	float frameTime_chinchou = 0.15f;   // Duración de cada frame en segundos
	float timer_chinchou = 0.0f;
	Texture2D frames_chinchou_idle[2];
	Texture2D frames_chinchou_hit[2];

	int currentFrame_makuhita = 0;
	float frameTime_makuhita = 0.30f;   // Duración de cada frame en segundos
	float timer_makuhita = 0.0f;
	Texture2D frames_makuhita_idle[2];

	int currentFrame_chikorita = 0;
	float frameTime_chikorita = 0.35f;   // Duración de cada frame en segundos
	float timer_chikorita = 0.0f;
	Texture2D frames_chikorita_idle[2];

	PhysBody* sensor;
	PhysBody* sensorBlock;

	Ball* ball;
	int ballRad = 15;
	Vector2 springForce = { 0.0f, -10.0f };
	bool sensed;
	bool canImpulse = false;

	bool basicImpulser;

	float spoinkPos;

	Board* rubyBoard;
	Obstacle* rubyObstacle;

	Texture2D emptyBoard;
	Texture2D ballTex;
	Texture2D palancaderSheet;
	Texture2D palancaizqSheet;
	Texture2D gameOver;
	Texture2D ballSave;


	Texture2D ContactImpulserLeft;
	Texture2D ContactImpulserRight;

	Spring* spoink;
	Texture2D spoinkSheet;
	Pikachu* pikachu;
	RightFlipper* rFlip;
	LeftFlipper* lFlip;

	Chinchou* chinchou1;
	Chinchou* chinchou2;
	Chinchou* chinchou3;

	Makuhita* makuhita;
	Chikorita* chikorita;

	Block* blocker;

	Texture2D pikachuSheet;
	Texture2D chinchouSheet;
	Texture2D makuhitaSheet;
	Texture2D chikoritaSheet;

	Vector2 initBallPos = { 243 * 2, 250 * 2 };

	bool bumper_hit = false;

	int cnt = 0;
	int cntAnimation = 0;
	bool dead = false;

	struct PlayerStats {
		int bestScore = 0;
		int actualScore = 0;
		int lifes = 3;
	}player;
	 
	State state;

	Font font;
	char cadena[100];

	bool start;
	bool oneTime;
	bool changeAnimation;

	bool contactRight;
	bool contactLeft;

	bool extralife = false;
	int textCounter;

	vec2<int> ray;
	bool ray_on;
};
