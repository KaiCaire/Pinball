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

	~Ball() {};

	void Update() override
	{
		int x, y;
		body->GetPhysicPosition(x, y);
		Vector2 position{ (float)x, (float)y };
		float scale = 2.0f;
		Rectangle source = { 0.0f, 0.0f, (float)texture.width, (float)texture.height };
		Rectangle dest = { position.x, position.y, (float)texture.width * scale, (float)texture.height * scale };
		Vector2 origin = { (float)(texture.width * scale / 2), (float)(texture.height * scale / 2) };
		float rotation = body->GetRotation() * RAD2DEG;
		DrawTexturePro(texture, source, dest, origin, rotation, WHITE);
	}

	void ShootBall(const b2Vec2& force) {
		body->body->ApplyLinearImpulseToCenter(force, true);
	}
	void updatePosition()
	{
		b2Vec2 pos;
		pos.x = 9.7;
		pos.y = 10.0;

		body->body->SetTransform(pos, 0.0f);
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
	static constexpr int board_circuit[108] =
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
		1, 844,
		0, 2,
		511, 1,
		509, 844,
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
		: PhysicEntity(physics->CreateChain(0, 0, board_circuit, 108, b2_staticBody,1), _listener)
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

class Block : public PhysicEntity
{
public:

	static constexpr int board_limit[16] =
	{
		437, 284,
		423, 253,
		413, 237,
		393, 206,
		371, 182,
		396, 200,
		422, 236,
		444, 274
	};

	Block(ModulePhysics* physics, int _x, int _y, Module* _listener, Texture2D _texture)
		: PhysicEntity(physics->CreateChain(0, 0, board_limit, 16, b2_staticBody, 1), _listener)
		, texture(_texture)
	{

	}

	void Update() override
	{
		int x, y;
		body->GetPhysicPosition(x, y);
		DrawTextureEx(texture, Vector2{ (float)x, (float)y }, body->GetRotation() * RAD2DEG, 2.0f, WHITE);
	}
	void changeColision(bool flag) {
		body->body->SetEnabled(flag);
	}

private:
	Texture2D texture;
};


class Obstacle : public PhysicEntity
{
public:

	static constexpr int circuit1[12] = {
		138, 662,
		175, 716,
		175, 725,
		173, 727,
		137, 669, 
		132, 663
	};

	static constexpr int circuit10[10] = {
		133, 660,
		169, 726,
		140, 711,
		130, 701,
		130, 678
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
		332, 209
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
		208, 179
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

	static constexpr int circuit9[8] = {
		347, 663,
		342, 663,
		305, 716,
		305, 725
	};
	static constexpr int circuit19[10] = {

		347, 662,
		350, 665,
		349, 704,
		313, 727,
		307, 726
	};

	Obstacle(ModulePhysics* physics, int _x, int _y, Module* _listener, Texture2D _texture)
		: PhysicEntity(nullptr, _listener), texture(_texture)

	{
		CreateChain(physics, circuit1, sizeof(circuit1) / sizeof(circuit1[0]), _x, _y, LeftImpulser);
		CreateChain(physics, circuit10, sizeof(circuit10) / sizeof(circuit10[0]), _x, _y, NoInteraction);

		CreateChain(physics, circuit2, sizeof(circuit2) / sizeof(circuit2[0]), _x, _y, NoInteraction);
		CreateChain(physics, circuit3, sizeof(circuit3) / sizeof(circuit3[0]), _x, _y, NoInteraction);
		CreateChain(physics, circuit4, sizeof(circuit4) / sizeof(circuit4[0]), _x, _y, NoInteraction);
		CreateChain(physics, circuit5, sizeof(circuit5) / sizeof(circuit5[0]), _x, _y, NoInteraction);
		CreateChain(physics, circuit6, sizeof(circuit6) / sizeof(circuit6[0]), _x, _y, NoInteraction);
		CreateChain(physics, circuit7, sizeof(circuit7) / sizeof(circuit7[0]), _x, _y, NoInteraction);
		CreateChain(physics, circuit8, sizeof(circuit8) / sizeof(circuit8[0]), _x, _y, NoInteraction);

		CreateChain(physics, circuit9, sizeof(circuit9) / sizeof(circuit9[0]), _x, _y, RightImpulser);
		CreateChain(physics, circuit19, sizeof(circuit19) / sizeof(circuit19[0]), _x, _y, NoInteraction);
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
	b2Vec2 axis = { 0.0f, -1.0f };
	b2PrismaticJoint* joint;
	PhysBody* bodyA;
	PhysBody* bodyB;

	Spring(ModulePhysics* physics, int _x, int _y, Module* _listener, const Texture2D& _texture) 
		: PhysicEntity(physics->CreateRectangle(_x, _y, 40, 80, b2_dynamicBody, SpringImpulser), _listener)
		, texture(_texture)
	{
		
		bodyA = this->body;
		bodyB = physics->CreateRectangle(_x + 15, _y + bodyA->height, 40, 10, b2_staticBody, SpringImpulser);
		joint = physics->CreateSpring(bodyA, bodyB, axis);
		
	
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
		Rectangle dest = { position.x, position.y, (float)width * scale, (float)texture.height * scale };

		Vector2 origin = { source.width * scale /2 , source.height * scale / 2 }; 
		float rotation = body->GetRotation() * RAD2DEG;
		DrawTexturePro(texture, source, dest, origin, rotation, WHITE);
	}

public:
	Texture2D texture;
	
};

class Pikachu : public PhysicEntity
{
public:
	Pikachu(ModulePhysics* physics, int _x, int _y, Module* _listener, const Texture2D& _texture)
		: PhysicEntity(physics->CreateRectangleSensor(_x, _y, 20,20, b2_staticBody, PikachuImpulser), _listener), texture(_texture)
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

	Texture2D texture;


private:
	
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

class RightFlipper : public PhysicEntity
{
public:

	b2RevoluteJoint* revJoint;
	PhysBody* rightAnchor;
	
	
	RightFlipper(ModulePhysics* physics, int _x, int _y, Module* _listener, const Texture2D& _texture)
		: PhysicEntity(physics->CreateRectangle(_x, _y, 60, 20, b2_dynamicBody, NoInteraction), _listener), texture(_texture)
	{
		// Initialize the bounding box based on the texture
		width = 32;
		height = 16;

		rightAnchor = physics->CreateRectangle(305, 790, 1, 1, b2_staticBody, NoInteraction);
		revJoint = physics->CreateFlipper(this->body, rightAnchor, b2Vec2(rightAnchor->body->GetPosition()));
	}
	//bool rotate = false;
	//int x = 7;

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


class LeftFlipper : public PhysicEntity
{
public:

	b2RevoluteJoint* revJoint;
	PhysBody* leftAnchor;

	LeftFlipper(ModulePhysics* physics, int _x, int _y, Module* _listener, const Texture2D& _texture)
		: PhysicEntity(physics->CreateRectangle(_x, _y, 60, 20, b2_dynamicBody, 1), _listener), texture(_texture)
	{
		// Initialize the bounding box based on the texture
		width = 32;
		height = 16;
		
		leftAnchor = physics->CreateRectangle(175, 790, 1, 1, b2_staticBody, NoInteraction);
		revJoint = physics->CreateFlipper(this->body, leftAnchor, b2Vec2(leftAnchor->body->GetPosition()));
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

ModuleGame::ModuleGame(Application* app, bool start_enabled) : Module(app, start_enabled)
{}

ModuleGame::~ModuleGame()
{}

// Load assets
bool ModuleGame::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	font = LoadFont("Assets/Ruby/Tiny5-Regular.ttf");

	emptyBoard = LoadTexture("Assets/Ruby/bg+mart.png");
	spoinkSheet = LoadTexture("Assets/Ruby/spoink_sheet.png");
	pikachuSheet = LoadTexture("Assets/Ruby/pikachu_sheet.png");
	palancaizqSheet = LoadTexture("Assets/Ruby/Left_Flipper.png");
	palancaderSheet = LoadTexture("Assets/Ruby/Right_Flipper.png");
	ballTex = LoadTexture("Assets/Ruby/temp ball.png");
	gameOver = LoadTexture("Assets/Ruby/GAME OVER.png");

	ballSave = LoadTexture("Assets/Ruby/ball_save.png");
	ballSave.height = ballSave.height * 2;
	ballSave.width = ballSave.width * 2;


	ContactFlipperRight = LoadTexture("Assets/Ruby/ContactImpulserRight.png");
	ContactFlipperRight.height = ContactFlipperRight.height * 2;
	ContactFlipperRight.width = ContactFlipperRight.width * 2;

	ContactFlipperLeft = LoadTexture("Assets/Ruby/ContactImpulserLeft.png");
	ContactFlipperLeft.height = ContactFlipperLeft.height * 2;
	ContactFlipperLeft.width = ContactFlipperLeft.width * 2;

	rubyBoard = new Board(App->physics, 0, 0, this, emptyBoard);
	rubyObstacle = new Obstacle(App->physics, 0, 0, this, emptyBoard);

	spoink = new Spring(App->physics, 472, 775, this, spoinkSheet);


	//CARGAR FRAMES DE LA ANIMACIÓN

	frames[0] = LoadTexture("Assets/Ruby/spoink_sheet/spoink_sheet_1.png");
	frames[1] = LoadTexture("Assets/Ruby/spoink_sheet/spoink_sheet_3.png");
	frames[2] = LoadTexture("Assets/Ruby/spoink_sheet/spoink_sheet_4.png");
	frames[3] = LoadTexture("Assets/Ruby/spoink_sheet/spoink_sheet_2.png");
	frames[4] = LoadTexture("Assets/Ruby/spoink_sheet/spoink_sheet_1.png");

	frames[5] = LoadTexture("Assets/Ruby/spoink_sheet_A/spoink_sheet_A_1.png");
	frames[6] = LoadTexture("Assets/Ruby/spoink_sheet_A/spoink_sheet_A_2.png");
	frames[7] = LoadTexture("Assets/Ruby/spoink_sheet_A/spoink_sheet_A_3.png");

	for (int z = 1;z < 14;z++)
	{
		sprintf_s(cadena, "Assets/Ruby/ball_save/ball_save_%d.png", z);
		frames_Latias[z] = LoadTexture(cadena);
		frames_Latias[z].height = frames_Latias[z].height * 2;
		frames_Latias[z].width = frames_Latias[z].width * 2;
	}


	pikachu = new Pikachu(App->physics, 415, 775, this, pikachuSheet);

	//CARGAR FRAMES DE LA ANIMACIÓN PIKACHU
	frames_pikachu[0] = LoadTexture("Assets/Ruby/pikachu_sheet/pikachu_sheet_1.png");
	frames_pikachu[1] = LoadTexture("Assets/Ruby/pikachu_sheet/pikachu_sheet_2.png");
	
	frames_Win[0] = LoadTexture("Assets/Ruby/win_1.png");
	frames_Win[0].height = frames_Win[0].height * 2;
	frames_Win[0].width = frames_Win[0].width * 2;

	frames_Win[1] = LoadTexture("Assets/Ruby/win_2.png");
	frames_Win[1].height = frames_Win[1].height * 2;
	frames_Win[1].width = frames_Win[1].width * 2;

	pikachu = new Pikachu(App->physics, 415, 775, this, pikachuSheet);

	rFlip = new RightFlipper(App->physics, 280, 790, this, palancaderSheet);
	lFlip = new LeftFlipper(App->physics, 200, 790, this, palancaizqSheet);

	sensor = App->physics->CreateRectangleSensor(65, 780, 30, 20, b2_staticBody, Impulser);
	sensor = App->physics->CreateRectangleSensor(275, 210, 120, 10, b2_staticBody, Points); //Top points

	blocker = new Block(App->physics, 198, 798, this, emptyBoard);
	blocker->changeColision(false);

	sensorBlock = App->physics->CreateRectangleSensor(360, 200, 20, 50, b2_staticBody, 10);

	sensor = App->physics->CreateRectangleSensor(390, 705, 80, 20, b2_staticBody, Points);  //Left points

	sensor = App->physics->CreateRectangleSensor(90, 705, 80, 20, b2_staticBody, Points);   //Right points

	sensor = App->physics->CreateRectangleSensor(242, 850, 82, 10, b2_staticBody, Dead);   //Dead

	music = LoadMusicStream("Assets/Ruby/Music Tracks/RedTableTrack.mp3");
	gameOverMusic = LoadSound("Assets/Ruby/Music Tracks/Game Over.mp3");
	winMusic = LoadSound("Assets/Ruby/Music Tracks/You Win.mp3");

	pointsSFX = LoadSound("Assets/Ruby/Sounds/Another pling.WAV");
	deadSFX = LoadSound("Assets/Ruby/Sounds/DOOoo.WAV");
	impulserSFX = LoadSound("Assets/Ruby/Sounds/Do.WAV");

	flipperFX = App->audio->LoadFx("Assets/Ruby/Sounds/flipperFX.mp3");
	spoink_chargeSFX = App->audio->LoadFx("Assets/Ruby/Sounds/spoink_charge.wav");
	spoink_releaseSFX = App->audio->LoadFx("Assets/Ruby/Sounds/spoink_release.wav");

	if (music.stream.buffer == NULL) // Verifica que se haya cargado correctamente
	{
		LOG("Error loading music stream");
		ret = false;
	}
	else
	{
		PlayMusicStream(music);
	}
	
	if (gameOverMusic.stream.buffer == NULL) // Verifica que se haya cargado correctamente
	{
		LOG("Error loading gameOverMusic stream");
		ret = false;
	}

	state = State::INGAME;


	return ret;
}


// Update: draw background
update_status ModuleGame::Update()
{
	switch (state)
	{
	case State::INGAME:
		
		UpdateMusicStream(music);

		rubyBoard->Update();
		rubyObstacle->Update();

		if(start && !oneTime)
		{ 
			blocker->changeColision(true);
			oneTime = true;
		}

		if (ball == NULL) {
			ball = new Ball(App->physics, initBallPos.x, initBallPos.y, this, ballTex);
		}

		//ANIMATION PIKACHU
		timer_pikachu += GetFrameTime();
		if (timer_pikachu >= frameTime_pikachu)
		{
			timer_pikachu = 0.0f;
			currentFrame_pikachu++;
			if (currentFrame_pikachu >= 2) currentFrame_pikachu = 0;	// Reinicia el ciclo

		}
		pikachu->texture = frames_pikachu[currentFrame_pikachu];

		// ANIMACION SPOINK
		timer += GetFrameTime();

		if(!changeAnimation) {
			if (timer >= frameTime) {
				timer = 0.0f;
				currentFrame++;
				if (currentFrame >= 5) currentFrame = 0; // Reinicia el ciclo
			}
		}
		else
		{
			if (timer >= frameTime) {
				timer = 0.0f;
				currentFrame++;
				if (currentFrame >= 7) currentFrame = 5; // Reinicia el ciclo
			}
		}

		spoink->texture = frames[currentFrame];

		if (canImpulse) {
			
			DrawRectangle(0, 440, 700, 25, WHITE);
			DrawTextEx(font, "Hold/release DOWN arrow to shot!", { 100, 440 }, 25, 0, BLACK);

			if (basicImpulser)
			{
				if (IsKeyReleased(KEY_DOWN)) {
					// Apply a force to the plunger when the space key is pressed
					b2Vec2 force(0.0f, -0.7f);
					ball->ShootBall(force);
					canImpulse = false;
					basicImpulser = false;
				}
			}
			else
			{
				if (IsKeyDown(KEY_DOWN)){
					App->audio->PlayFx(spoink_chargeSFX);
					changeAnimation = true;
					spoink->joint->SetMotorSpeed(-0.5f);
				}

				else if (IsKeyReleased(KEY_DOWN))
				{
					App->audio->PlayFx(spoink_releaseSFX);
					changeAnimation = false;
					spoink->joint->SetMotorSpeed(200.0f);
					canImpulse = false;
				}
			}
		}

		//spring movement
		spoinkPos = spoink->joint->GetJointTranslation();

		if (spoinkPos >= spoink->joint->GetUpperLimit() - 0.001f) {
			// Joint has reached or is very close to the upper limit
			spoink->joint->SetMotorSpeed(-0.2f);  // Move it back down
		}
		else if (spoinkPos <= spoink->joint->GetLowerLimit() + 0.001f) {
			// Joint has reached or is very close to the lower limit
			spoink->joint->SetMotorSpeed(0.0f);  // Stop at the bottom
		}

		//flipper movement

		if (IsKeyPressed(KEY_RIGHT)) {
			App->audio->PlayFx(flipperFX);
			//The revolute joint angle is positive when its body rotates CCW about the angle point.
			pikachu = new Pikachu(App->physics, 415, 775, this, pikachuSheet);
			rFlip->revJoint->SetMotorSpeed(-4.0f);
			
		}
		else if (IsKeyReleased(KEY_RIGHT)) {
			rFlip->revJoint->SetMotorSpeed(4.0f);
			
		}

		if (IsKeyPressed(KEY_LEFT)) {
			App->audio->PlayFx(flipperFX);
			pikachu = new Pikachu(App->physics, 66, 775, this, pikachuSheet);
			lFlip->revJoint->SetMotorSpeed(4.0f);
		}
		else if (IsKeyReleased(KEY_LEFT)) {
			lFlip->revJoint->SetMotorSpeed(-4.0f);
		}

		//lives management
		if (dead) {
			if (cnt < 1500 && player.lifes != 1){
				PlaySound(deadSFX);
				
				if (cnt<=150 || cnt >= 1200){
					DrawTexture(ballSave, cntAnimation, 450, WHITE);
					cntAnimation += 5;
				}
				else
				{
					timer_latias += GetFrameTime();
					if (timer_latias >= framesTime_latias)
					{
						timer_latias = 0.0f;
						if(backwards)currentFrames_latias--;
						else currentFrames_latias++;
						if (currentFrames_latias >= 13) backwards = true;
						else if (currentFrames_latias <= 2) backwards = false;	// Reinicia el ciclo
						
					}
					DrawTexture(frames_Latias[currentFrames_latias], 150, 450, WHITE);
				}
				cnt +=5;
			}
			else 
			{ 
				ball->updatePosition();
				player.lifes -= 1;
				oneTime = false;
				start = false;
				blocker->changeColision(false);
				dead = false;
				cntAnimation = 0;
				cnt = 0;
			}
		}
		else
		{
			if (contactLeft && cnt < 12)
			{
				DrawTexture(ContactFlipperLeft, 130, 660, WHITE);
				cnt++;
			}
			else if (!contactRight) {
				cnt = 0;
				contactLeft = false;
			}

			if (contactRight && cnt < 12)
			{
				DrawTexture(ContactFlipperRight, 305, 660, WHITE);
				cnt++;
			}
			else if (!contactLeft) {
				cnt = 0;
				contactRight = false;
			}
		}


		if (IsKeyReleased(KEY_A)) {
			printf("%d, %d, \n", GetMouseX(), GetMouseY());
		}

		if (player.lifes == 0) {

			StopMusicStream(music);

			if(player.actualScore < player.bestScore){
				PlaySound(gameOverMusic);
				state = State::DEAD;
			}
			else {
				PlaySound(winMusic);
				state = State::WIN;
			}
		}

		pikachu->Update();
		spoink->Update();

		//text (scores) rendering
		 
		sprintf_s(cadena, "%d", player.actualScore);
		DrawTextEx(font, cadena, { 410, 822}, 30,0, WHITE);

		sprintf_s(cadena, "%d", player.bestScore);
		DrawTextEx(font, cadena, { 410, 805 }, 25, 0, YELLOW);
		DrawTextEx(font, "BEST:", { 360, 808 }, 20, 0, YELLOW);

		ball->Update();

		break;

	case State::DEAD:

		rubyBoard->Update();

		DrawTexture(gameOver,40, 400, WHITE);

		if (cnt >= 20) {
			DrawTextEx(font, "PRESS SPACE TO CONTINUE", { 100, 440 }, 25, 0, BLACK);
		}
		if (cnt >= 80) cnt = 0;
		cnt++;

		if (IsKeyPressed(KEY_SPACE)) {
			state = State::SCORE;
			player.lifes = 3;
			cnt = 0;
		}

		break;

	case State::SCORE:

		if (player.actualScore > player.bestScore){
			player.bestScore = player.actualScore;
		}
	
		player.actualScore = 0;

		StopSound(gameOverMusic);
		PlayMusicStream(music);

		state = State::INGAME;
		break;

	case State::WIN:

		rubyBoard->Update();

		sprintf_s(cadena, "NEW RECORD : %d", player.actualScore);

		if (cnt >= 20) {
			
			DrawTexture(frames_Win[0], 40, 400, WHITE);

			DrawTextEx(font, cadena, { 120, 600 }, 35, 0, ORANGE);
		}
		else{
			DrawTexture(frames_Win[1], 40, 400, WHITE);

			DrawTextEx(font, cadena, { 120, 600 }, 35, 0, YELLOW);
		}

		if (cnt >= 40) cnt = 0;
		cnt++;

		if (IsKeyPressed(KEY_SPACE)) {
			StopSound(winMusic);
			state = State::SCORE;
			player.lifes = 3;
			cnt = 0;
		}

		break;
	default:
		break;
	}


	DrawTexture(ballTex, 60, 825, WHITE);
	sprintf_s(cadena, "%d", player.lifes);
	DrawTextEx(font, cadena, { 80, 820 }, 25, 0, WHITE);

	rFlip->Update();
	lFlip->Update();

	pikachu->Update();
	spoink->Update();

	UpdateMusicStream(music);

	return UPDATE_CONTINUE;
}

void ModuleGame::OnCollision(PhysBody* bodyA, PhysBody* bodyB, int dir)
{	
	b2Vec2 force(0.0f, 0.0f);

	if (dir == SpringImpulser || dir == PikachuImpulser || dir == Impulser){
		canImpulse = true;
		if (dir == PikachuImpulser || dir == Impulser) basicImpulser = true;
	}

	if (dir == LeftImpulser){
		PlaySound(impulserSFX);
		contactLeft = true;
		force = { 0.4f, -0.9f };
	}

	else if (dir == RightImpulser){ 
		PlaySound(impulserSFX);
		contactRight = true;
		force = { -0.4f, -0.9f };
	}

	else if (dir == Points) {
		player.actualScore += 100;
		canImpulse = false;
		basicImpulser = false;
		PlaySound(pointsSFX);
	}
	else if (dir == Dead) dead = true;

	if (dir == SpringImpulser) {
		canImpulse = true;
	}

	if (dir == 10 && start == false) start = true;

	// Force to shoot the ball
	ball->ShootBall(force);
}

// Load assets
bool ModuleGame::CleanUp()
{
	LOG("Unloading Intro scene");
	UnloadTexture(emptyBoard);
	UnloadTexture(ballTex);

	StopMusicStream(music);
	UnloadMusicStream(music);

	delete ball;
	delete rubyBoard;
	delete spoink;
	delete pikachu;
	//delete palancaDer;
	//delete palancaIzq;
	delete rFlip;
	delete lFlip;

	return true;
}