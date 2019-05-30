#include <PhysicalWorld.h>
#include <Windows.h>
#include <iostream>

PhysicalWorld::PhysicalWorld()
	: Scene("physicalWorld")
	, colldetc()
	, collsolver()
	, f_mass(100)
	, f_velo(emt::Vec2f(0.0f))
	, s_mass(10)
	, s_velo(emt::Vec2f(0.0f))
	, shapes{}
	, player(nullptr)
{

}
//
PhysicalWorld::~PhysicalWorld()
{
	for (auto b : shapes)
	{
		auto tmp = b;
		b = nullptr;
		delete tmp;
	}
	player = nullptr;
}
//
void PhysicalWorld::Unload()
{

}
//
void PhysicalWorld::Load()
{
	//Create directional light
	emt::DirectionalLight sun;
	sun.SetDirection(emt::Vec3f(0.0f, 0.0f, -1.0f));
	sun.SetColor(emt::Vec3f(1.0f, 1.0f, 1.0f));
	emt::LightComponent* sky = emt::CreateComponent(emt::LightComponent(sun, nullptr));

	float wallRot = 0.f;
	bool wallIsDyn = false;
	float roomWidth = 20.f;
	float roomHeigth = 40.f;


	float wallBounciness = 0.8f;
	float floorBounciness = 1.0f;

	float low_density = 2.0f;
	float med_density = 10.0f;
	float high_density = 25.0f;
	float god_density = 150.0f;

	gravity = 1.0f;

	//Left wall
	shapes.emplace_back(new RectangleShape("wall.png", 1.0f, roomHeigth, high_density, emt::Vec2f(-roomWidth, 0.0f), wallBounciness, emt::Vec2f(0.0f), emt::Vec2f(0.0f), wallIsDyn));
	//Right wall
	shapes.emplace_back(new RectangleShape("wall.png", 1.0f, roomHeigth, high_density, emt::Vec2f(roomWidth, 0.0f), wallBounciness, emt::Vec2f(0.0f), emt::Vec2f(0.0f), wallIsDyn));
	//Roof
	shapes.emplace_back(new RectangleShape("wall.png", roomWidth, 1.0f, high_density, emt::Vec2f(0.0f, roomHeigth), wallBounciness, emt::Vec2f(0.0f), emt::Vec2f(0.0f), wallIsDyn));
	//Floor
	shapes.emplace_back(new RectangleShape("floor.png", roomWidth, 1.0f, high_density, emt::Vec2f(0.0f, -roomHeigth), floorBounciness, emt::Vec2f(0.0f), emt::Vec2f(0.0f), wallIsDyn));

	//"GOD"
	player = new RectangleShape("player.png", 1.0, 3.0f, god_density, emt::Vec2f(-3.0f, -roomHeigth/2+8), 0.5f, emt::Vec2f(0.0f), emt::Vec2f(0.0f), true);
	shapes.emplace_back(player);


	//Upper level shapes
	shapes.emplace_back(new TriangleShape("mediumM.png", 3.0, 5.0f, med_density, emt::Vec2f(7.0f, 25.0f), 0.2f, emt::Vec2f(0.0f), emt::Vec2f(0.0f), true, false, 0.0f, 0.0f));
	shapes.emplace_back(new RectangleShape("mediumM.png", 2.0, 5.0f, med_density, emt::Vec2f(2.0f, 25.0f), 0.2f, emt::Vec2f(0.0f), emt::Vec2f(0.0f), true, false, 0.0f, 0.0f));
	shapes.emplace_back(new RectangleShape("lightM.png", 2.0, 2.0f, low_density, emt::Vec2f(-6.0f, 25.0f), 0.5f, emt::Vec2f(0.0f), emt::Vec2f(0.0f), true, false, 0.0f, 0.0f));

	//Middle level shapes
	shapes.emplace_back(new TriangleShape("lightM.png", 3.0, 2.0f, low_density, emt::Vec2f(1.0f, 3.2f), 0.5f, emt::Vec2f(0.0f), emt::Vec2f(0.0f), true, false, 0.0f, 0.0f));
	shapes.emplace_back(new TriangleShape("lightM.png", 4.0, 3.5f, low_density, emt::Vec2f(7.0f, 3.2f), 0.5f, emt::Vec2f(0.0f), emt::Vec2f(0.0f), true, false, 0.0f, 0.0f));
	shapes.emplace_back(new RectangleShape("lightM.png", 3.0, 3.5f, low_density, emt::Vec2f(-7.5f, 3.2f), 0.5f, emt::Vec2f(0.0f), emt::Vec2f(0.0f), true, false, 0.0f, 0.0f));

	//Rotator 1
	shapes.emplace_back(new RectangleShape("heavyM.png", roomWidth - (roomWidth / 3.0f), 0.5f, high_density, emt::Vec2f(0.0f, 0.0f), 0.2f, emt::Vec2f(0.0f), emt::Vec2f(0.0f), true, true, 0.0f, 0.0f));
	//Rotator 2
	shapes.emplace_back(new RectangleShape("heavyM.png", roomWidth - (roomWidth / 3.0f), 0.5f, high_density, emt::Vec2f(0.0f, 20.0f), 0.2f, emt::Vec2f(0.0f), emt::Vec2f(0.0f), true, true, 0.0f, 0.0f));
	//Rotator 3
	shapes.emplace_back(new RectangleShape("heavyM.png", roomWidth - (roomWidth / 3.0f), 0.5f, high_density, emt::Vec2f(0.0f, -20.0f), 0.2f, emt::Vec2f(0.0f), emt::Vec2f(0.0f), true, true, 0.0f, 0.0f));


	///Every scene needs a camera!
	auto cam = emt::CreateComponent(emt::CameraComponent(/*CAMERA_TYPE::STATIC*/));
	//cam->targetTransform = player->myTransform;
	cam->position = emt::Vec3f(0.0f, 0.0f, (roomWidth + roomHeigth) /*/ 2.3f*/);
	cam->up = emt::Vec3f(0.0f, 1.0f, 0.0f);
	cam->direction = emt::Vec3f(0.0f, 0.0f, -1.0f);
}
//
void PhysicalWorld::Update(float deltaTime)
{
	float speed = 5.25f;

	//PLAYER MOVEMENT
	if (emt::isKeyPressed(emt::key::Up))
		player->velocity.y += speed * deltaTime;
	if (emt::isKeyPressed(emt::key::Down))
		player->velocity.y -= speed * deltaTime;
	if (emt::isKeyPressed(emt::key::Right))
		player->velocity.x += speed * deltaTime;
	if (emt::isKeyPressed(emt::key::Left))
		player->velocity.x -= speed * deltaTime;
	//PLAYER ROTATION
	if (emt::isKeyPressed(emt::key::Z))
		player->ang_velocity += speed * deltaTime;
	if (emt::isKeyPressed(emt::key::X))
		player->ang_velocity -= speed * deltaTime;

	//APPLY VELOCITY TO ALL DYNAMIC OBJECTS
	if (emt::isKeyPressed(emt::key::LControl))
	{
		for (auto b : shapes)
		{
			if (b->isDynamic)
				b->velocity.y += 20.0f * deltaTime;
		}
	}

	//Check collisions
	auto lCheckCollisions = [&](float deltaTime) {
		for (auto itr = shapes.begin(); itr != shapes.end() - 1; itr++)
		{
			//Create containers that hold vertices moved from model-space to world-space
			std::vector<emt::Vec2f> fTranslatedVerts;
			colldetc.CoordsToWorldSpace((*itr)->model->myMeshes.at(0).vertices, fTranslatedVerts, (*itr)->trans->myModelMatrix);

			for (auto next = itr + 1; next != shapes.end(); next++)
			{
				//We only solve collisions between dyn-stat or dyn-dyn pairs!
				if ((*itr)->isDynamic || (*next)->isDynamic)
				{
					std::vector<emt::Vec2f> sTranslatedVerts;
					//Translate coordinates to world space
					colldetc.CoordsToWorldSpace((*(next))->model->myMeshes.at(0).vertices, sTranslatedVerts, (*(next))->trans->myModelMatrix);

					//Check for collision
					auto collisionData = colldetc.DetectCollision(fTranslatedVerts, (*itr)->trans->position, sTranslatedVerts, (*(next))->trans->position);
					//If collisionData is not a nullptr, collision has happened
					if (collisionData != nullptr)
					{
						//and we solve it.			
						collsolver.SolveCollision(*collisionData, deltaTime, (*itr)->inv_mass, (*itr)->e, (*itr)->inertia,(*itr)->isDynamic, (*itr)->velocity, (*itr)->ang_velocity, (*itr)->trans->position, (*next)->inv_mass, (*next)->e, (*next)->inertia,(*next)->isDynamic, (*next)->velocity, (*next)->ang_velocity, (*next)->trans->position);
					}
				}
			}
		}
		//Move shapes accordingly
		for (auto b : shapes)
		{
			if (b->isDynamic && !b->fixed_pos)
			{
				b->velocity.y -= gravity * deltaTime;
				b->trans->position += emt::Vec3f(b->velocity * deltaTime, 0.0f);
			}
			else
				b->velocity = emt::Vec2f(0.0f);
		}
		//Rotate shapes accordingly
		for (auto b : shapes)
		{
			//Remember to convert to degrees
			b->trans->orientation.z += (b->ang_velocity * 180.0f / PI) * deltaTime;
		}
	};
	lCheckCollisions(deltaTime);
	

}