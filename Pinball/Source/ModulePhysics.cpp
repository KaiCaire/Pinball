#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModulePhysics.h"
#include "ModuleGame.h"

#include "p2Point.h"

#include <math.h>


ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	world = NULL;
	debug = false;
}

// Destructor
ModulePhysics::~ModulePhysics()
{
}

bool ModulePhysics::Start()
{
	LOG("Creating Physics 2D environment");
	world = new b2World(b2Vec2(GRAVITY_X, -GRAVITY_Y));
	world->SetContactListener(this);

	b2BodyDef bd;

	CreateScenarioGround();
	ground = world->CreateBody(&bd);
	
	return true;
}

update_status ModulePhysics::PreUpdate()
{
	world->Step(1.0f / 60.0f, 6, 2);

	for (b2Contact* c = world->GetContactList(); c; c = c->GetNext())
	{
		if (c->GetFixtureA()->IsSensor() && c->IsTouching())
		{
			b2BodyUserData data1 = c->GetFixtureA()->GetBody()->GetUserData();
			b2BodyUserData data2 = c->GetFixtureA()->GetBody()->GetUserData();

			PhysBody* pb1 = (PhysBody*)data1.pointer;
			PhysBody* pb2 = (PhysBody*)data2.pointer;
			if (pb1 && pb2 && pb1->listener)
				pb1->listener->OnCollision(pb1, pb2, pb1->id);
		}
	}

	return UPDATE_CONTINUE;
}

update_status ModulePhysics::Update() {

	return UPDATE_CONTINUE;
}


PhysBody* ModulePhysics::CreateCircle(int x, int y, int radius, b2BodyType bType)
{
	PhysBody* pbody = new PhysBody();

	b2BodyDef body;
	body.type = bType;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));
	body.userData.pointer = reinterpret_cast<uintptr_t>(pbody);

	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(radius);
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 1.0f;

	b->CreateFixture(&fixture);

	pbody->body = b;
	pbody->width = pbody->height = radius;

	return pbody;
}

void ModulePhysics::CreateScenarioGround()
{
	// Get coordinates of the screen center and radius
	int x = SCREEN_WIDTH * 2;
	int y = SCREEN_HEIGHT / 1.5f;
	int diameter = SCREEN_WIDTH / 2;

	// Create a static body in the middle of the screen
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	// Add this static body to the World
	b2Body* big_ball = world->CreateBody(&body);

	// Create a big circle shape
	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(diameter) * 0.5f;

	// Create a fixture and associate the circle to it
	b2FixtureDef fixture;
	fixture.shape = &shape;

	// Add the ficture (plus shape) to the static body
	big_ball->CreateFixture(&fixture);
}

PhysBody* ModulePhysics::CreateRectangle(int x, int y, int width, int height, b2BodyType bType, int inf)
{
	PhysBody* pbody = new PhysBody();

	b2BodyDef body;
	body.type = bType;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));
	body.userData.pointer = reinterpret_cast<uintptr_t>(pbody);

	b2Body* b = world->CreateBody(&body);
	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;

	b->CreateFixture(&fixture);

	pbody->id = inf;
	pbody->body = b;
	pbody->width = (int)(width * 0.5f);
	pbody->height = (int)(height * 0.5f);

	return pbody;
}

PhysBody* ModulePhysics::CreateRectangleSensor(int x, int y, int width, int height, b2BodyType bType, int inf)
{
	PhysBody* pbody = new PhysBody();

	b2BodyDef body;
	body.type = bType;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));
	body.userData.pointer = reinterpret_cast<uintptr_t>(pbody);

	b2Body* b = world->CreateBody(&body);

	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;
	fixture.isSensor = true;

	b->CreateFixture(&fixture);

	pbody->id = inf;
	pbody->body = b;
	pbody->width = width;
	pbody->height = height;

	return pbody;
}

PhysBody* ModulePhysics::CreateBumper(int x, int y, int radius, b2BodyType bType, int inf)
{
	PhysBody* pbody = new PhysBody();

	b2BodyDef body;
	body.type = bType;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));
	body.userData.pointer = reinterpret_cast<uintptr_t>(pbody);

	b2Body* b = world->CreateBody(&body);

	b2PolygonShape box;

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(radius);
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 1.0f;
	fixture.restitution = 1.5f;

	b->CreateFixture(&fixture);
	pbody->width = pbody->height = radius;

	pbody->id = inf;
	pbody->body = b;

	return pbody;
}

PhysBody* ModulePhysics::CreateChain(int x, int y, const int* points, int size, b2BodyType bType, int inf)
{
	PhysBody* pbody = new PhysBody();

	b2BodyDef body;
	body.type = bType;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));
	body.userData.pointer = reinterpret_cast<uintptr_t>(pbody);
	
	b2Body* b = world->CreateBody(&body);

	b2ChainShape shape;
	b2Vec2* p = new b2Vec2[size / 2];

	for (int i = 0; i < size; ++i)
	{
		TraceLog(LOG_INFO, "%i", (points[i]));
	}
	for (int i = 0; i < size / 2; ++i)
	{
		p[i].x = PIXEL_TO_METERS(points[i * 2 + 0]);
		p[i].y = PIXEL_TO_METERS(points[i * 2 + 1]);
	}

	shape.CreateLoop(p, size / 2);

	b2FixtureDef fixture;
	fixture.shape = &shape;

	b->CreateFixture(&fixture);

	delete p;

	pbody->body = b;
	pbody->width = pbody->height = 0;
	pbody->id = inf;

	return pbody;
}

b2RevoluteJoint* ModulePhysics::CreateFlipper(PhysBody* bodyA, PhysBody* bodyB, b2Vec2 anchor) {

	b2RevoluteJointDef def;
	def.Initialize(bodyA->body, bodyB->body, bodyB->body->GetWorldCenter());
	def.collideConnected = false;
	def.enableLimit = true;
	def.enableMotor = true;
	def.maxMotorTorque = 150.0f;
	
	if (anchor.x > PIXEL_TO_METERS(SCREEN_WIDTH/2)) { // Right flipper
		def.upperAngle = 0.15f * b2_pi;
		def.lowerAngle = -0.25f * b2_pi;
		def.motorSpeed = 10.0f;

	}
	else { // Left flipper
		def.upperAngle = 0.25f * b2_pi;
		def.lowerAngle = -0.15f * b2_pi;
		def.motorSpeed = -10.0f;
	}
	
	b2RevoluteJoint* revJoint = (b2RevoluteJoint*)world->CreateJoint(&def);
	return revJoint;
}

b2PrismaticJoint* ModulePhysics::CreateSpring(PhysBody* bodyA, PhysBody* bodyB, b2Vec2 axis) {

	float scale = 2.0f;

	// Define prismatic joint
	b2PrismaticJointDef def;
	def.bodyA = bodyB->body;
	def.bodyB = bodyA->body;
	def.localAnchorA.Set(0, 0);
	def.localAnchorB.Set(0, 0);
	def.upperTranslation = 1.5f; 
	def.lowerTranslation = -1.5f; // How far does the spring extend
	def.enableLimit = true;
	def.localAxisA.Set(axis.x, axis.y);
	def.enableMotor = true;
	def.maxMotorForce = 200.0f;

	// Create & add to world
	b2PrismaticJoint* prismaticJoint = (b2PrismaticJoint*)world->CreateJoint(&def);

	return prismaticJoint;
}

update_status ModulePhysics::PostUpdate()
{
	if (IsKeyPressed(KEY_F1))
	{
		debug = !debug;
	}

	if (!debug)
	{
		return UPDATE_CONTINUE;
	}

	b2Body* mouseSelect = nullptr;
	Vector2 mousePosition = GetMousePosition();
	b2Vec2 pMousePosition = b2Vec2(PIXEL_TO_METERS(mousePosition.x), PIXEL_TO_METERS(mousePosition.y));

	for (b2Body* b = world->GetBodyList(); b; b = b->GetNext())
	{
		for (b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
		{
			switch (f->GetType())
			{
			// Draw circles ------------------------------------------------
			case b2Shape::e_circle:
			{
				b2CircleShape* shape = (b2CircleShape*)f->GetShape();
				b2Vec2 pos = f->GetBody()->GetPosition();

				DrawCircleLines(METERS_TO_PIXELS(pos.x), METERS_TO_PIXELS(pos.y), (float)METERS_TO_PIXELS(shape->m_radius), Color{ 128, 128, 128, 255 });
			}
			break;

			// Draw polygons ------------------------------------------------
			case b2Shape::e_polygon:
			{
				b2PolygonShape* polygonShape = (b2PolygonShape*)f->GetShape();
				int32 count = polygonShape->m_count;
				b2Vec2 prev, v;

				for (int32 i = 0; i < count; ++i)
				{
					v = b->GetWorldPoint(polygonShape->m_vertices[i]);
					if (i > 0)
						DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), RED);

					prev = v;
				}

				v = b->GetWorldPoint(polygonShape->m_vertices[0]);
				DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), RED);
			}
			break;

			// Draw chains contour -------------------------------------------
			case b2Shape::e_chain:
			{
				b2ChainShape* shape = (b2ChainShape*)f->GetShape();
				b2Vec2 prev, v;

				for (int32 i = 0; i < shape->m_count; ++i)
				{
					v = b->GetWorldPoint(shape->m_vertices[i]);
					if (i > 0)
						DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), GREEN);
					prev = v;
				}

				v = b->GetWorldPoint(shape->m_vertices[0]);
				DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), GREEN);
			}
			break;

			// Draw a single segment(edge) ----------------------------------
			case b2Shape::e_edge:
			{
				b2EdgeShape* shape = (b2EdgeShape*)f->GetShape();
				b2Vec2 v1, v2;

				v1 = b->GetWorldPoint(shape->m_vertex0);
				v1 = b->GetWorldPoint(shape->m_vertex1);
				DrawLine(METERS_TO_PIXELS(v1.x), METERS_TO_PIXELS(v1.y), METERS_TO_PIXELS(v2.x), METERS_TO_PIXELS(v2.y), BLUE);
			}
			break;
			}

			if (mouse_joint == nullptr && mouseSelect == nullptr && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {

				if (f->TestPoint(pMousePosition)) {
					mouseSelect = b;
				}
			}
		}
	}

	if (mouseSelect) {
		b2MouseJointDef def;

		def.bodyA = ground;
		def.bodyB = mouseSelect;
		def.target = pMousePosition;
		def.damping = 0.5f;
		def.stiffness = 20.f;
		def.maxForce = 100.f * mouseSelect->GetMass();

		mouse_joint = (b2MouseJoint*)world->CreateJoint(&def);
	}

	else if (mouse_joint && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
		mouse_joint->SetTarget(pMousePosition);
		b2Vec2 anchorPosition = mouse_joint->GetBodyB()->GetPosition();
		anchorPosition.x = METERS_TO_PIXELS(anchorPosition.x);
		anchorPosition.y = METERS_TO_PIXELS(anchorPosition.y);

		DrawLine(anchorPosition.x, anchorPosition.y, mousePosition.x, mousePosition.y, RED);
	}

	else if (mouse_joint && IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
		world->DestroyJoint(mouse_joint);
		mouse_joint = nullptr;
	}
	return UPDATE_CONTINUE;
}


// Called before quitting
bool ModulePhysics::CleanUp()
{
	LOG("Destroying physics world");
	// Delete the whole physics world!
	return true;
}

void PhysBody::GetPhysicPosition(int& x, int& y) const
{
	b2Vec2 pos = body->GetPosition();
	x = METERS_TO_PIXELS(pos.x);
	y = METERS_TO_PIXELS(pos.y);
}

float PhysBody::GetRotation() const
{
	return body->GetAngle();
}

bool PhysBody::Contains(int x, int y) const
{
	b2Vec2 p(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	const b2Fixture* fixture = body->GetFixtureList();

	while (fixture != NULL)
	{
		if (fixture->GetShape()->TestPoint(body->GetTransform(), p) == true)
			return true;
		fixture = fixture->GetNext();
	}

	return false;
}

int PhysBody::RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const
{
	int ret = -1;

	b2RayCastInput input;
	b2RayCastOutput output;

	input.p1.Set(PIXEL_TO_METERS(x1), PIXEL_TO_METERS(y1));
	input.p2.Set(PIXEL_TO_METERS(x2), PIXEL_TO_METERS(y2));
	input.maxFraction = 1.0f;

	const b2Fixture* fixture = body->GetFixtureList();

	while (fixture != NULL)
	{
		if (fixture->GetShape()->RayCast(&output, input, body->GetTransform(), 0) == true)
		{
			float fx = (float)(x2 - x1);
			float fy = (float)(y2 - y1);
			float dist = sqrtf((fx * fx) + (fy * fy));

			normal_x = output.normal.x;
			normal_y = output.normal.y;

			return (int)(output.fraction * dist);
		}
		fixture = fixture->GetNext();
	}

	return ret;
}

void ModulePhysics::BeginContact(b2Contact* contact)
{
	b2BodyUserData dataA = contact->GetFixtureA()->GetBody()->GetUserData();
	b2BodyUserData dataB = contact->GetFixtureB()->GetBody()->GetUserData();
	
	PhysBody* physA = (PhysBody*)dataA.pointer;
	PhysBody* physB = (PhysBody*)dataB.pointer;

	if(physA->id >= 2)
	{ 
		if (physA && physA->listener != NULL)
			physA->listener->OnCollision(physA, physB, physA->id);

		if (physB && physB->listener != NULL)
			physB->listener->OnCollision(physB, physA, physA->id);
	}

}