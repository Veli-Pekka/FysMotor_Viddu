#ifndef BASIC2DPHYSICS_SHAPE_H
#define BASIC2DPHYSICS_SHAPE_H


#include <Engine/UserInterface.hpp>

class Shape
{
public:
	Shape(
		std::string textureName,
		float width,
		float heigth,
		float density,
		emt::Vec2f position,
		float e = 1.0f,
		emt::Vec2f velocity = emt::Vec2f(0.0f),
		emt::Vec2f acceleration = emt::Vec2f(0.0f),
		bool isDynamic = true,
		bool fixed_pos = false,
		float ang_velocity = 0.0f,
		float ang_acceleration = 0.0f,
		float orientation = 0.0f
	);

	virtual ~Shape();


	//Pointers to components
	emt::ModelComponent* model;
	emt::TextureComponent* texture;
	emt::TransformComponent* trans;
	emt::RenderComponent* render;

	std::string textureName;
	float width;
	float heigth;
	float density;
	emt::Vec2f position;
	float e;
	emt::Vec2f velocity;
	emt::Vec2f acceleration;
	bool isDynamic;
	bool fixed_pos;
	float ang_velocity;
	float ang_acceleration;
	float inertia;
	float orientation;

	float inv_mass;

};
//
//
//
class RectangleShape : public Shape
{
public:
	RectangleShape(
		std::string textureName,
		float width,
		float heigth,
		float density,
		emt::Vec2f position,
		float e = 1.0f,
		emt::Vec2f velocity = emt::Vec2f(0.0f),
		emt::Vec2f acceleration = emt::Vec2f(0.0f),
		bool isDynamic = true,
		bool fixed_pos = false,
		float ang_velocity = 0.0f,
		float ang_acceleration = 0.0f,
		float orientation = 0.0f
	);
	
	~RectangleShape();
};
//
//
//
class TriangleShape : public Shape
{
public:
	TriangleShape(
		std::string textureName,
		float width,
		float heigth,
		float density,
		emt::Vec2f position,
		float e = 1.0f,
		emt::Vec2f velocity = emt::Vec2f(0.0f),
		emt::Vec2f acceleration = emt::Vec2f(0.0f),
		bool isDynamic = true,
		bool fixed_pos = false,
		float ang_velocity = 0.0f,
		float ang_acceleration = 0.0f,
		float orientation = 0.0f
	);

	~TriangleShape();
};


#endif