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
		: PhysicEntity(physics->CreateCircle(_x, _y, 10, b2_dynamicBody), _listener)
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
		Vector2 origin = { (float)(texture.width / 1.0f), (float)(texture.height / 1.0f) };
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
		: PhysicEntity(physics->CreateRectangle(_x, _y, 100, 50, b2_dynamicBody, 0), _listener)
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

	static constexpr int board_circuit[100] =
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
		413, 644,
		393, 641,
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
		: PhysicEntity(physics->CreateChain(0, 0, board_circuit, 98, b2_staticBody,1), _listener)
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

	static constexpr int circuit1[12] = {
		135, 663,
		174, 714,
		174, 735,
		128, 705,
		128, 664,
		135, 660
	};

	static constexpr int circuit2[38] = {
		333, 193,
		375, 229,
		405, 280,
		412, 322,
		418, 379,
		414, 423,
		404, 465,
		385, 507,
		380, 491,
		407, 410,
		394, 404,
		380, 414,
		348, 473,
		345, 450,
		391, 339,
		390, 300,
		372, 255,
		332, 231,
		332, 209,
	};

	static constexpr int circuit3[20] = {
		290, 184,
		293, 179,
		300, 179,
		304, 183,
		304, 211,
		303, 226,
		300, 228,
		293, 228,
		290, 227,
		289, 188
	};

	static constexpr int circuit4[18] = {
		248, 176,
		252, 172,
		257, 172,
		261, 176,
		261, 223,
		259, 227,
		251, 226,
		248, 224,
		248, 181
	};
	static constexpr int circuit5[68] = {
		219, 175,
		219, 224,
		213, 231,
		212, 267,
		217, 287,
		221, 313,
		231, 336,
		240, 352,
		240, 368,
		214, 368,
		202, 363,
		202, 348,
		205, 318,
		208, 308,
		207, 284,
		191, 267,
		175, 267,
		157, 275,
		151, 288,
		153, 318,
		156, 336,
		160, 353,
		163, 372,
		164, 386,
		154, 387,
		149, 374,
		146, 355,
		141, 331,
		140, 305,
		141, 266,
		154, 228,
		164, 211,
		180, 194,
		208, 179,
	};

	static constexpr int circuit6[50] = {

		115, 217,
		115, 236,
		106, 253,
		102, 270,
		97, 293,
		98, 319,
		100, 347,
		103, 371,
		107, 396,
		116, 426,
		123, 448,
		134, 478,
		145, 503,
		158, 517,
		156, 533,
		135, 554,
		122, 545,
		95, 504,
		76, 463,
		65, 420,
		66, 397,
		64, 366,
		65, 320,
		76, 275,
		98, 233
	};

	static constexpr int circuit7[32] = {

		100, 666,
		100, 719,
		103, 724,
		111, 731,
		118, 735,
		132, 744,
		169, 767,
		183, 777,
		184, 788,
		173, 799,
		94, 750,
		90, 746,
		86, 741,
		86, 665,
		90, 661,
		96, 661
	};
	static constexpr int circuit8[26] = {

		380, 667,
		385, 662,
		390, 662,
		393, 666,
		394, 740,
		391, 744,
		308, 799,
		295, 788,
		295, 781,
		371, 731,
		376, 724,
		379, 719,
		380, 672
	};


	static constexpr int circuit9[16] = {
		348, 663,
		349, 704,
		348, 713,
		316, 734,
		310, 734,
		306, 730,
		306, 717,
		342, 663
	};

	Obstacle(ModulePhysics* physics, int _x, int _y, Module* _listener, Texture2D _texture)
		: PhysicEntity(nullptr, _listener), texture(_texture)

	{
		CreateChain(physics, circuit1, sizeof(circuit1) / sizeof(circuit1[0]), _x, _y,2);
		CreateChain(physics, circuit2, sizeof(circuit2) / sizeof(circuit2[0]), _x, _y,1);
		CreateChain(physics, circuit3, sizeof(circuit3) / sizeof(circuit3[0]), _x, _y,1);
		CreateChain(physics, circuit4, sizeof(circuit4) / sizeof(circuit4[0]), _x, _y,1);
		CreateChain(physics, circuit5, sizeof(circuit5) / sizeof(circuit5[0]), _x, _y,1);
		CreateChain(physics, circuit6, sizeof(circuit6) / sizeof(circuit6[0]), _x, _y,1);
		CreateChain(physics, circuit7, sizeof(circuit7) / sizeof(circuit7[0]), _x, _y,1);
		CreateChain(physics, circuit8, sizeof(circuit8) / sizeof(circuit8[0]), _x, _y,1);
		CreateChain(physics, circuit9, sizeof(circuit9) / sizeof(circuit9[0]), _x, _y,3);

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


	void CreateChain(ModulePhysics* physics, const int* circuit, int size, int _x, int _y, int i)
	{
		PhysBody* body = physics->CreateChain(_x, _y, circuit, size, b2_staticBody,i);
		if (body != nullptr) {
			bodies.push_back(body);
		}
	}
};

class Spring : public PhysicEntity {
public:
	b2Vec2 axis = { 0.0f, 0.0f };

	Spring(ModulePhysics* physics, int _x, int _y, Module* _listener, const Texture2D& _texture)
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
		Rectangle dest = { position.x, position.y - texture.height, width * scale, (float)texture.height * scale };

		Vector2 origin = { (float)(25), (float)(texture.height / scale) }; 
		float rotation = body->GetRotation() * RAD2DEG;
		DrawTexturePro(texture, source, dest, origin, rotation, WHITE);
	}

private:
	Texture2D texture;
};

class Pikachu : public PhysicEntity
{
public:
	Pikachu(ModulePhysics* physics, int _x, int _y, Module* _listener, const Texture2D& _texture)
		: PhysicEntity(physics->CreateRectangleSensor(_x, _y, 20,20, b2_staticBody, 1), _listener), texture(_texture)
	{
		// Initialize the bounding box based on the texture
		width = 25;
		height = 40;
	}

	void Update() override
	{
		Vector2 position = GetColliderPosition();
		float scale = 2.0f;

		Rectangle source = { 0.0f, 0.0f, (float)width, (float)texture.height };
		Rectangle dest = { position.x, position.y - (texture.height / 1.5f), width * scale, (float)texture.height * scale };

		Vector2 origin = GetTextureOrigin(); // Updated method to get the origin
		float rotation = body->GetRotation() * RAD2DEG;

		DrawTexturePro(texture, source, dest, origin, rotation, WHITE);
	}

	int RayHit(vec2<int> ray, vec2<int> mouse, vec2<float>& normal) override
	{
		return body->RayCast(ray.x, ray.y, mouse.x, mouse.y, normal.x, normal.y);
	}

private:
	Texture2D texture;
	int width;
	int height;

	Vector2 GetColliderPosition() const
	{
		int x, y;
		body->GetPhysicPosition(x, y);
		return { (float)x, (float)y };
	}

	Vector2 GetTextureOrigin() const
	{
		// Adjust origin based on width and height to center the texture correctly
		return { (float)(width), (float)(texture.height / 2) };
	}
};

class PalancaDer : public PhysicEntity
{
public:
	PalancaDer(ModulePhysics* physics, int _x, int _y, Module* _listener, const Texture2D& _texture)
		: PhysicEntity(physics->CreateRectangle(_x, _y, 60, 20, b2_kinematicBody,5), _listener), texture(_texture)
	{
		// Initialize the bounding box based on the texture
		width = 32;
		height = 16;

	}
	bool rotate = false;
	int x = 7;

	void Update() override
	{
		Vector2 position = GetColliderPosition();
		float scale = 2.0f;

		Rectangle source = { 0.0f, 0.0f, (float)width, (float)texture.height };
		Rectangle dest = { position.x, position.y - (texture.height / 1.5f), width * scale, (float)texture.height * scale };

		Vector2 origin = GetTextureOrigin(); // Updated method to get the origin

		float rotation = body->GetRotation() * RAD2DEG;
		b2Vec2 vel = {0.0f,1.0f};
		if (rotate){
			//rotation -= x;
			if (x < 20){
				body->body->SetAngularVelocity(4.5f);
				vel = { 0.4f,-1.1f };
				x++;
			}
			else{
				body->body->SetAngularVelocity(0.0f);
				vel = { 0.0f,0.0f };
			}
			body->body->SetLinearVelocity(vel);
		}
		else if (x != 0)
		{
			body->body->SetAngularVelocity(-4.5f);
			x--;
			vel = { -0.4f, 1.1f };
			body->body->SetLinearVelocity(vel);
		}
		else{
			body->body->SetAngularVelocity(0.0f);
			vel = { 0.0f,0.0f };

			body->body->SetLinearVelocity(vel);
		}
		DrawTexturePro(texture, source, dest, origin, rotation, WHITE);
	}

	int RayHit(vec2<int> ray, vec2<int> mouse, vec2<float>& normal) override
	{
		return body->RayCast(ray.x, ray.y, mouse.x, mouse.y, normal.x, normal.y);
	}

private:
	Texture2D texture;
	int width;
	int height;

	Vector2 GetColliderPosition() const
	{
		int x, y;
		body->GetPhysicPosition(x, y);
		return { (float)x, (float)y };
	}

	Vector2 GetTextureOrigin() const
	{
		// Adjust origin based on width and height to center the texture correctly
		return { (float)(width), (float)(texture.height / 2) };
	}
};

class PalancaIzq : public PhysicEntity
{
public:
	PalancaIzq(ModulePhysics* physics, int _x, int _y, Module* _listener, const Texture2D& _texture)
		: PhysicEntity(physics->CreateRectangle(_x, _y, 58, 20, b2_kinematicBody, 4), _listener), texture(_texture)
	{
		// Initialize the bounding box based on the texture
		width = 32;
		height = 16;

	}
	bool rotate = false;
	int x = 7;

	void Update() override
	{
		Vector2 position = GetColliderPosition();
		float scale = 2.0f;

		Rectangle source = { 0.0f, 0.0f, (float)width, (float)texture.height };
		Rectangle dest = { position.x, position.y - (texture.height / 1.5f), width * scale, (float)texture.height * scale };

		Vector2 origin = GetTextureOrigin(); // Updated method to get the origin

		float rotation = body->GetRotation() * RAD2DEG;
		b2Vec2 vel = { 0.0f,1.0f };
		if (rotate) {

			if (x < 20) {
				body->body->SetAngularVelocity(-4.5f);
				vel = { -0.4f,-1.1f };
				x++;
			}
			else {
				body->body->SetAngularVelocity(0.0f);
				vel = { 0.0f,0.0f };
			}
			body->body->SetLinearVelocity(vel);
		}
		else if (x != 0)
		{
			body->body->SetAngularVelocity(4.5f);
			x--;
			vel = { 0.4f,1.1f };
			body->body->SetLinearVelocity(vel);
		}
		else {
			body->body->SetAngularVelocity(0.0f);
			vel = { 0.0f,0.0f };

			body->body->SetLinearVelocity(vel);
		}
		DrawTexturePro(texture, source, dest, origin, rotation, WHITE);
	}

	int RayHit(vec2<int> ray, vec2<int> mouse, vec2<float>& normal) override
	{
		return body->RayCast(ray.x, ray.y, mouse.x, mouse.y, normal.x, normal.y);
	}

private:
	Texture2D texture;
	int width;
	int height;

	Vector2 GetColliderPosition() const
	{
		int x, y;
		body->GetPhysicPosition(x, y);
		return { (float)x, (float)y };
	}

	Vector2 GetTextureOrigin() const
	{
		// Adjust origin based on width and height to center the texture correctly
		return { (float)(width), (float)(texture.height / 2) };
	}
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
	pikachuSheet = LoadTexture("Assets/Ruby/pikachu_sheet.png");
	palancaizqSheet = LoadTexture("Assets/Ruby/Left_Flipper.png");
	palancaderSheet = LoadTexture("Assets/Ruby/Right_Flipper.png");

	rubyBoard = new Board(App->physics, 0, 0, this, emptyBoard);
	rubyObstacle = new Obstacle(App->physics, 0, 0, this, emptyBoard);

	spoink = new Spring(App->physics, 472, 755, this, spoinkSheet);
	pikachu = new Pikachu(App->physics, 415, 775, this, pikachuSheet);
	palancaDer = new PalancaDer(App->physics, 285, 798, this, palancaderSheet);
	palancaIzq = new PalancaIzq(App->physics, 198, 798, this, palancaizqSheet);


	sensor = App->physics->CreateRectangleSensor(275, 210, 120, 10, b2_staticBody, 6);
	sensor = App->physics->CreateRectangleSensor(65, 780, 30, 20, b2_staticBody, 5);


	return ret;
}


// Update: draw background
update_status ModuleGame::Update()
{

	if (ball == NULL) {
		ball = new Ball(App->physics, initBallPos.x, initBallPos.y, this, ballTex);
	}
	
	if (IsKeyPressed(KEY_ONE))
	{
		entities.emplace_back(new Ball(App->physics, GetMouseX(), GetMouseY(), this, ballTex));

	}
	rubyBoard->Update();
	rubyObstacle->Update();
	palancaDer->Update();
	palancaIzq->Update();


	if (IsKeyPressed(KEY_SPACE)) {
		// Apply a force to the plunger when the space key is pressed
		b2Vec2 force(0.0f, -1.0f); // Force to shoot the ball upwards
		ball->ShootBall(force);
	}

	if (IsKeyPressed(KEY_E)) {
		palancaDer->rotate = true;
	}
	else if (IsKeyReleased(KEY_E))
	{
		palancaDer->rotate = false;
	}

	if (IsKeyPressed(KEY_Q)) {
		palancaIzq->rotate = true;
	}
	else if (IsKeyReleased(KEY_Q))
	{
		palancaIzq->rotate = false;
	}

	if (IsKeyPressed(KEY_A)) {
		printf("%d, %d, \n", GetMouseX(), GetMouseY());
	}
	ball->Update();
	pikachu->Update();
	spoink->Update();

	return UPDATE_CONTINUE;
}

void ModuleGame::OnCollision(PhysBody* bodyA, PhysBody* bodyB, int dir)
{	
	
	/*App->audio->PlayFx(bonus_fx);*/
	if (IsKeyPressed(KEY_DOWN)) 
	{
		//compress string
	}
	b2Vec2 force(0.0f, 0.0f);
	if (dir == 1) force = { 0.7f, -0.46 };
	else if (dir == 2) force = { -0.7f, -0.6f };
	else if (dir == 5) printf("puntos");
	else if (dir == 4) printf("puntos2");

	//else if (dir == 3 && palancaIzq->x !=0)
	//{
	//	printf("izq");
	//	force = {(float)palancaIzq->x/20, -0.5f};
	//}
	//else if (dir == 4 && palancaDer->x != 0)
	//{
	//	printf("der");
	//	force = {-(float)palancaDer->x / 20, -0.5f };
	//}


	// Force to shoot the ball upwards
	ball->ShootBall(force);
}

// Load assets
bool ModuleGame::CleanUp()
{
	LOG("Unloading Intro scene");
	UnloadTexture(emptyBoard);
	UnloadTexture(ballTex);
	delete ball;
	delete rubyBoard;
	delete spoink;
	delete pikachu;
	delete palancaDer;
	delete palancaIzq;

	return true;
}
