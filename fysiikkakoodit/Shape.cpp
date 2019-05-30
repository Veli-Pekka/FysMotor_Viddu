#include <Shape.h>

Shape::Shape(
	std::string textureName,
	float width,
	float heigth,
	float density,
	emt::Vec2f position,
	float e,
	emt::Vec2f velocity,
	emt::Vec2f acceleration,
	bool isDynamic,
	bool fixed_pos,
	float ang_velocity,
	float ang_acceleration,
	float orientation)
	: model(nullptr)
	, texture(nullptr)
	, trans(nullptr)
	, render(nullptr)
	, textureName(textureName)
	, width(width)
	, heigth(heigth)
	, density(density)
	, position(position)
	, e(e)
	, velocity(velocity)
	, isDynamic(isDynamic)
	, fixed_pos(fixed_pos)
	, ang_velocity(ang_velocity)
	, inertia(0.0f)
	, orientation(orientation)
{
}
//
Shape::~Shape()
{
	model = nullptr;
	texture = nullptr;
	trans = nullptr;
	render = nullptr;
}
//
//
//
RectangleShape::RectangleShape(
	std::string textureName,
	float width,
	float heigth,
	float density,
	emt::Vec2f position,
	float e,
	emt::Vec2f velocity,
	emt::Vec2f acceleration,
	bool isDynamic,
	bool fixed_pos,
	float ang_velocity,
	float ang_acceleration,
	float orientation)
	: Shape(
		textureName
		, width
		, heigth
		, density
		, position
		, e
		, velocity
		, acceleration
		, isDynamic
		, fixed_pos
		, ang_velocity
		, ang_acceleration
		, orientation)
{
	//Components
	model = emt::CreateComponent(emt::ModelComponent(emt::Rectangle(width, heigth), emt::ModelDynamicState::Dynamic));
	texture = emt::CreateComponent(emt::TextureComponent(textureName));
	trans = emt::CreateComponent(emt::TransformComponent(emt::Vec3f(position.x, position.y, 0.0f), emt::Vec3f(0.0f, 0.0f, orientation), emt::Vec3f(1.0f)));
	render = emt::CreateComponent(emt::RenderComponent(model, texture, trans));

	//Calculate inverse mass
	inv_mass = 1.0f / (width * heigth * density);

	//Calculate inertia
	inertia = ((1.0f / 12.0f) / inv_mass) * (sqrt(pow(width, 2) + pow(heigth, 2)));
}
//
RectangleShape::~RectangleShape()
{
	model = nullptr;
	texture = nullptr;
	trans = nullptr;
	render = nullptr;
}
//
//
//
TriangleShape::TriangleShape(
	std::string textureName,
	float width,
	float heigth,
	float density,
	emt::Vec2f position,
	float e,
	emt::Vec2f velocity,
	emt::Vec2f acceleration,
	bool isDynamic,
	bool fixed_pos,
	float ang_velocity,
	float ang_acceleration,
	float orientation)
	: Shape(
		textureName
		, width
		, heigth
		, density
		, position
		, e
		, velocity
		, acceleration
		, isDynamic
		, fixed_pos
		, ang_velocity
		, ang_acceleration
		, orientation)
{
	//Components
	model = emt::CreateComponent(emt::ModelComponent(emt::Triangle(
		emt::Vec3f(-width / 2, -heigth / 2, 0.0f),
		emt::Vec3f(width / 2, -heigth / 2, 0.0f),
		emt::Vec3f(0.0f, heigth / 2, 0.0f)
	), emt::ModelDynamicState::Dynamic));
	texture = emt::CreateComponent(emt::TextureComponent(textureName));
	trans = emt::CreateComponent(emt::TransformComponent(emt::Vec3f(position.x, position.y, 0.0f), emt::Vec3f(0.0f, 0.0f, orientation), emt::Vec3f(1.0f)));
	render = emt::CreateComponent(emt::RenderComponent(model, texture, trans));

	//Calculate inverse mass
	inv_mass = 1.0f / ((width * heigth * density) / 2.0f);

	//Calculate inertia
	inertia = ((1.0f / 36.0f) / inv_mass) * width + pow(heigth, 3);
}
//
TriangleShape::~TriangleShape()
{
	model = nullptr;
	texture = nullptr;
	trans = nullptr;
	render = nullptr;
}