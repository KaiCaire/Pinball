#pragma once

#include "Module.h"
#include "Globals.h"

#include "box2d\box2d.h"

#define GRAVITY_X 0.0f
#define GRAVITY_Y -0.6f

#define PIXELS_PER_METER 50.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.02f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)


// Small class to return to other modules to track position and rotation of physics bodies
class PhysBody
{
public:
	PhysBody() : listener(NULL), body(NULL) {}

	//void GetPosition(int& x, int& y) const;
	void GetPhysicPosition(int& x, int& y) const;
	float GetRotation() const;
	bool Contains(int x, int y) const;
	int RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const;

public:
	int width, height;
	b2Body* body;
	Module* listener;
	int id;
};


class ModulePhysics : public Module, public b2ContactListener // TODO
{
public:
	ModulePhysics(Application* app, bool start_enabled = true);
	~ModulePhysics();

	bool Start();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	PhysBody* CreateCircle(int x, int y, int radius, b2BodyType bType);
	PhysBody* CreateRectangle(int x, int y, int width, int height, b2BodyType bType, int inf);
	PhysBody* CreateRectangleSensor(int x, int y, int width, int height, b2BodyType bType, int inf);
	PhysBody* CreateChain(int x, int y, const int* points, int size, b2BodyType bType, int inf);
	b2RevoluteJoint* CreateFlipper(PhysBody* bodyA, PhysBody* bodyB, b2Vec2 anchor);
	b2PrismaticJoint* CreateSpring(PhysBody* bodyA, PhysBody* bodyB, b2Vec2 axis);

	void BeginContact(b2Contact* contact);
	bool debug = false;

	

private:
	b2World* world;

};