#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleGame.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"

class PhysicEntity
{
protected:

	PhysicEntity(PhysBody* _body, Module* _listener)
		: body(_body)
		, listener(_listener)
	{
		if (body != nullptr) {
			body->listener = listener;
		}
	}

public:
	virtual ~PhysicEntity() = default;
	virtual void Update() = 0;

	virtual int RayHit(vec2<int> ray, vec2<int> mouse, vec2<float>& normal)
	{
		return 0;
	}

protected:
	PhysBody* body;
	Module* listener;
};

class Ball : public PhysicEntity
{
public:
	Ball(ModulePhysics* physics, int _x, int _y, Module* _listener, Texture2D _texture)
		: PhysicEntity(physics->CreateCircle(_x, _y, 15, b2_dynamicBody), _listener)
		, texture(_texture)
	{

	}

	void Update() override
	{
		int x, y;
		body->GetPhysicPosition(x, y);
		Vector2 position{ (float)x, (float)y };
		float scale = 2.0f;
		Rectangle source = { 0.0f, 0.0f, (float)texture.width, (float)texture.height };
		Rectangle dest = { position.x - (texture.width / 2), position.y - (texture.height / 2), (float)texture.width * scale, (float)texture.height * scale };
		Vector2 origin = { (float)(texture.width / 2.0f), (float)(texture.height / 2.0f) };
		float rotation = body->GetRotation() * RAD2DEG;
		DrawTexturePro(texture, source, dest, origin, rotation, WHITE);
	}

	void ShootBall(const b2Vec2& force) {
		body->body->ApplyLinearImpulseToCenter(force, true);
	}

private:
	Texture2D texture;

};

class Box : public PhysicEntity
{
public:
	Box(ModulePhysics* physics, int _x, int _y, Module* _listener, Texture2D _texture)
		: PhysicEntity(physics->CreateRectangle(_x, _y, 100, 50, b2_dynamicBody), _listener)
		, texture(_texture)
	{

	}

	void Update() override
	{
		int x, y;
		body->GetPhysicPosition(x, y);
		DrawTexturePro(texture, Rectangle{ 0, 0, (float)texture.width, (float)texture.height },
			Rectangle{ (float)x, (float)y, (float)texture.width, (float)texture.height },
			Vector2{ (float)texture.width / 2.0f, (float)texture.height / 2.0f }, body->GetRotation() * RAD2DEG, WHITE);
	}

	int RayHit(vec2<int> ray, vec2<int> mouse, vec2<float>& normal) override
	{
		return body->RayCast(ray.x, ray.y, mouse.x, mouse.y, normal.x, normal.y);;
	}



private:
	Texture2D texture;

};

class Board : public PhysicEntity
{
public:

	static constexpr int board_circuit[98] =
	{
		240, 144,
		187, 151,
		123, 175,
		80, 203,
		47, 253,
		28, 310,
		25, 367,
		26, 437,
		41, 483,
		60, 527,
		75, 549,
		93, 568,
		93, 617,
		86, 628,
		69, 631,
		56, 633,
		49, 645,
		47, 792,
		79, 792,
		79, 766,
		134, 804,
		197, 838,
		197, 848,
		284, 848,
		284, 840,
		400, 769,
		400, 793,
		430, 793,
		430, 662,
		421, 647,
		400, 646,
		383, 637,
		382, 572,
		421, 525,
		447, 459,
		456, 395,
		453, 335,
		438, 289,
		445, 276,
		467, 330,
		471, 411,
		472, 816,
		501, 816,
		499, 355,
		478, 283,
		434, 227,
		376, 185,
		329, 162,
		276, 145
	};



	Board(ModulePhysics* physics, int _x, int _y, Module* _listener, Texture2D _texture)
		: PhysicEntity(physics->CreateChain(0, 0, board_circuit, 98, b2_staticBody), _listener)
		, texture(_texture)
	{

	}

	void Update() override
	{
		int x, y;
		body->GetPhysicPosition(x, y);
		DrawTextureEx(texture, Vector2{ (float)x, (float)y }, body->GetRotation() * RAD2DEG, 2.0f, WHITE);
	}

private:
	Texture2D texture;
};

class Obstacle : public PhysicEntity
{
public:

	static constexpr int circuit1[16] = {
	100, 663,
	101, 723,
	122, 740,
	182, 776,
	185, 790,
	172, 801,
	85, 742,
	85, 661
	};

	static constexpr int circuit2[12] = {
	296, 784,
	308, 797,
	391, 740,
	392, 662,
	382, 661,
	374, 726
	};

	static constexpr int circuit3[26] = {
	132, 558,
	158, 535,
	157, 516,
	123, 449,
	100, 348,
	99, 274,
	116, 229,
	114, 212,
	84, 249,
	66, 321,
	66, 416,
	86, 491,
	109, 524
	};

	static constexpr int circuit4[48] = {
	153, 390,
	163, 389,
	163, 359,
	151, 306,
	152, 283,
	164, 272,
	189, 270,
	207, 281,
	202, 347,
	202, 364,
	211, 369,
	240, 370,
	239, 349,
	221, 308,
	217, 272,
	214, 231,
	220, 227,
	219, 174,
	190, 185,
	166, 208,
	145, 243,
	140, 260,
	139, 318,
	148, 372
	};

	static constexpr int circuit5[12] = {
	263, 227,
	248, 227,
	248, 176,
	252, 173,
	258, 172,
	262, 177
	};

	static constexpr int circuit6[16] = {
	294, 230,
	301, 230,
	304, 227,
	304, 183,
	300, 178,
	294, 178,
	290, 183,
	290, 227
	};

	static constexpr int circuit7[40] = {
	332, 193,
	333, 233,
	368, 252,
	396, 312,
	386, 376,
	345, 450,
	347, 478,
	365, 447,
	380, 417,
	386, 409,
	400, 409,
	404, 417,
	383, 487,
	382, 514,
	398, 480,
	413, 442,
	422, 376,
	416, 321,
	393, 250,
	362, 217
	};



	Obstacle(ModulePhysics* physics, int _x, int _y, Module* _listener, Texture2D _texture)
		: PhysicEntity(nullptr, _listener), texture(_texture)

	{
		CreateChain(physics, circuit1, sizeof(circuit1) / sizeof(circuit1[0]), _x, _y);
		CreateChain(physics, circuit2, sizeof(circuit2) / sizeof(circuit2[0]), _x, _y);
		CreateChain(physics, circuit3, sizeof(circuit3) / sizeof(circuit2[0]), _x, _y);
		CreateChain(physics, circuit4, sizeof(circuit4) / sizeof(circuit2[0]), _x, _y);
		CreateChain(physics, circuit5, sizeof(circuit5) / sizeof(circuit2[0]), _x, _y);
		CreateChain(physics, circuit6, sizeof(circuit6) / sizeof(circuit2[0]), _x, _y);
		CreateChain(physics, circuit7, sizeof(circuit7) / sizeof(circuit2[0]), _x, _y);




	}

	void Update() override
	{
		for (const auto& body : bodies) {
			int x, y;
			body->GetPhysicPosition(x, y);
			DrawTextureEx(texture, Vector2{ (float)x, (float)y }, body->GetRotation() * RAD2DEG, 2.0f, WHITE);
		}
	}

private:
	Texture2D texture;
	std::vector<PhysBody*> bodies; // Vector para almacenar los objetos


	void CreateChain(ModulePhysics* physics, const int* circuit, int size, int _x, int _y)
	{
		PhysBody* body = physics->CreateChain(_x, _y, circuit, size, b2_staticBody);
		if (body != nullptr) {
			bodies.push_back(body);
		}
	}
};

class AcceleratingObstacle : public PhysicEntity 
{
public:

	static constexpr int circuit1[26] = {
	140, 714,
	143, 714,
	164, 727,
	164, 734,
	171, 736,
	176, 731,
	176, 717,
	138, 662,
	130, 663,
	130, 705,
	132, 706,
	132, 714,
	136, 715
	};

	static constexpr int circuit2[26] = {
	303, 731,
	311, 737,
	316, 733,
	317, 727,
	340, 712,
	340, 716,
	347, 715,
	348, 706,
	350, 706,
	350, 664,
	346, 662,
	340, 664,
	304, 718
	};

	AcceleratingObstacle(ModulePhysics* physics, int _x, int _y, Module* _listener, Texture2D _texture)
		: PhysicEntity(nullptr, _listener), texture(_texture)

	{
		CreateChain(physics, circuit1, sizeof(circuit1) / sizeof(circuit1[0]), _x, _y);
		CreateChain(physics, circuit2, sizeof(circuit2) / sizeof(circuit2[0]), _x, _y);
	}
	void Update() override
	{
		for (const auto& body : bodies) {
			int x, y;
			body->GetPhysicPosition(x, y);
			DrawTextureEx(texture, Vector2{ (float)x, (float)y }, body->GetRotation() * RAD2DEG, 2.0f, WHITE);
		}
	}

private:
	Texture2D texture;
	std::vector<PhysBody*> bodies; // Vector para almacenar los objetos


	void CreateChain(ModulePhysics* physics, const int* circuit, int size, int _x, int _y)
	{
		PhysBody* body = physics->CreateChain(_x, _y, circuit, size, b2_staticBody);
		if (body != nullptr) {
			bodies.push_back(body);
		}
	}
};


class Spring : public PhysicEntity {
public:
	
	
	/*PhysBody spoink;
	PhysBody springGround;
	b2Vec2 anchorA;
	b2Vec2 anchorB;*/
	b2Vec2 axis = { 0.0f, -1.0f };

	Spring(ModulePhysics* physics, int _x, int _y, Module* _listener, Texture2D _texture)
		: PhysicEntity(physics->CreateSpring(_x, _y, 20, 40, axis), _listener)
		, texture(_texture)
	{

	}

		
	void Update() override
	{
		int x, y;
		int width = 20;
		int height = 40;
		body->GetPhysicPosition(x, y);
		Vector2 position{ (float)x, (float)y };
		float scale = 2.0f;
		Rectangle source = { 0.0f, 0.0f, width, texture.height };
		Rectangle dest = { position.x, position.y-texture.height, width * scale, (float)texture.height * scale };

		Vector2 origin = { (float)(25), (float)(texture.height / scale) }; //???
		float rotation = body->GetRotation() * RAD2DEG;
		DrawTexturePro(texture, source, dest, origin, rotation, WHITE);
	}

private:

	Texture2D texture;

};


ModuleGame::ModuleGame(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

ModuleGame::~ModuleGame()
{}

// Load assets
bool ModuleGame::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	emptyBoard = LoadTexture("Assets/Ruby/bg+mart.png");
	ballTex = LoadTexture("Assets/Ruby/temp ball.png");
	spoinkSheet = LoadTexture("Assets/Ruby/spoink_sheet.png");

	rubyBoard = new Board(App->physics, 0, 0, this, emptyBoard);
	rubyObstacle = new Obstacle(App->physics, 0, 0, this, emptyBoard);
	rubyAcceleratingObstacle= new AcceleratingObstacle(App->physics, 0, 0, this, emptyBoard);


	spoink = new Spring(App->physics, 472, 735, this, spoinkSheet);

	return ret;
}

// Load assets
bool ModuleGame::CleanUp()
{
	LOG("Unloading Intro scene");
	UnloadTexture(emptyBoard);
	UnloadTexture(ballTex);
	delete ball;
	delete rubyBoard;


	return true;
}

// Update: draw background
update_status ModuleGame::Update()
{

	if (ball == NULL) {
		ball = new Ball(App->physics, initBallPos.x, initBallPos.y, this, ballTex);
	}
	

	rubyBoard->Update();
	rubyObstacle->Update();
	rubyAcceleratingObstacle->Update();

	ball->Update();
	
	spoink->Update();



	return UPDATE_CONTINUE;
}

void ModuleGame::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	/*App->audio->PlayFx(bonus_fx);*/
	if (IsKeyPressed(KEY_DOWN)) {
		//compress string
	}
	if (IsKeyReleased(KEY_DOWN)) {
		if (IsKeyDown(KEY_SPACE)) {
			// Apply a force to the plunger when the space key is pressed
			b2Vec2 force(0.0f, -100.0f); // Force to shoot the ball upwards
			
		}
	}
}
