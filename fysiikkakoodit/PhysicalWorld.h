#ifndef BASIC2DPHYSICS_PHYSICALWORLD_H
#define BASIC2DPHYSICS_PHYSICALWORLD_H

#include <CollisionDetector.h>
#include <CollisionSolver.h>
#include <Shape.h>

#include <Engine/UserInterface.hpp>

//struct Block
//{
//	//Pointers to components
//	emt::ModelComponent* model;
//	emt::TextureComponent* texture;
//	emt::TransformComponent* trans;
//	emt::RenderComponent* render;
//
//	std::string textureName;
//
//	float density;
//	float inv_mass;
//	float e;
//	emt::Vec2f velocity;
//	float ang_velocity;
//	float inertia;
//
//
//	float width;
//	float heigth;
//	emt::Vec2f position;
//
//	bool isDynamic;
//	bool fixed_pos;
//
//	float orientation;
//
//	Block(
//		std::string textureName,
//		float width,
//		float heigth,
//		float density,
//		emt::Vec2f position,
//		float e = 1.0f,
//		emt::Vec2f velocity = emt::Vec2f(0.0f),
//		bool isDynamic = true,
//		bool fixed_pos = false,
//		float ang_velocity = 0.0f,
//		float orientation = 0.0f)
//		: textureName(textureName)
//		, width(width)
//		, heigth(heigth)
//		, density(density)
//		, position(position)
//		, e(e)
//		, velocity(velocity)
//		, isDynamic(isDynamic)
//		, fixed_pos(fixed_pos)
//		, ang_velocity(ang_velocity)
//		, inertia(0.0f)
//		, orientation(orientation)
//	{
//		//Components
//		model = emt::CreateComponent(emt::ModelComponent(emt::RectangleShape(), emt::ModelDynamicState::Dynamic));
//		texture = emt::CreateComponent(emt::TextureComponent(textureName));
//		trans = emt::CreateComponent(emt::TransformComponent(emt::Vec3f(position.x, position.y, 0.0f), emt::Vec3f(0.0f, 0.0f, orientation), emt::Vec3f(width, heigth, 1.0f)));
//		render = emt::CreateComponent(emt::RenderComponent(model, texture, trans));
//		
//		//Calculate inverse mass
//		inv_mass = 1.0f / (width * heigth * density);
//		
//		//Calculate inertia
//		inertia = ((1.0f / 12.0f) / inv_mass) * (sqrt(pow(width, 2) + pow(heigth, 2)));
//	}
//
//	~Block()
//	{
//		model = nullptr;
//		texture = nullptr;
//		trans = nullptr;
//		render = nullptr;
//	}
//};



class PhysicalWorld : public emt::Scene
{
public:
	//
	PhysicalWorld();
	//
	~PhysicalWorld();

	void Unload() override;
	//
	void Load() override;
	//
	void Update(float deltaTime) override;


private:
	//
	CollisionDetector colldetc;
	CollisionSolver collsolver;

	//Object's physical qualities
	float f_mass;
	emt::Vec2f f_velo;
	float s_mass;
	emt::Vec2f s_velo;

	std::vector<Shape*> shapes;

	Shape* player;

	float gravity;

};


#endif
