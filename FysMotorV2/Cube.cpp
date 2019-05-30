#include "Cube.h"

Cube::Cube()
{
	//CubeShape = sf::RectangleShape(sf::Vector2f(20.f,20.f));
	Position = sf::Vector2f(0.f, 0.f);
	Velocity = sf::Vector2f(0.f, 0.f);
	angularVelocity = 0.0f;
	Rotation = sf::Vector2f(0.f, 0.f);
	Acceleration = sf::Vector2f(0.f, 0.f);
	//CubeShape.setFillColor(sf::Color::Green);
	Mass = 1.f;
	inertia = 2.0f;
	//inertia = 1 / 12 * Mass * (pow(Ext.x, 2) + pow(Ext.y, 2));
	//CubeShape.setPosition(Position);
}

//	T‰t‰ k‰yt‰n p‰‰m‰‰r‰isesti
Cube::Cube(sf::Vector2f Ext, sf::Vector2f Pos, sf::Vector2f Vel)
{
	//CubeShape = sf::RectangleShape(Ext);
	Position = Pos;
	Velocity = Vel;
	angularVelocity = 0.0f;
	Rotation = sf::Vector2f(0.f, 0.f);
	Acceleration = sf::Vector2f(0.f, 0.f);
	//CubeShape.setFillColor(sf::Color::Green);
	Mass = 1.f;
	inertia = 1.f / 12.f * (1.f/Mass) * (sqrt(pow(Ext.x, 2) + pow(Ext.y, 2)));

	//

	/*CubeShape.setPosition(Position);
	CubeShape.setRotation(Rotation.x);*/


	ConvShape.setPosition(Pos);
	ConvShape.setPointCount(4);
	ConvShape.setPoint(0, sf::Vector2f(-Ext.x/2, -Ext.y / 2));
	ConvShape.setPoint(1, sf::Vector2f(Ext.x / 2, -Ext.y / 2));
	ConvShape.setPoint(2, sf::Vector2f(Ext.x / 2, Ext.y / 2));
	ConvShape.setPoint(3, sf::Vector2f(-Ext.x / 2, Ext.y / 2));
	ConvShape.setFillColor(sf::Color::Magenta);



	//ConvShape.setPoint(0, sf::Vector2f(Ext.x, Ext.y));
	//ConvShape.setPoint(1, sf::Vector2f(Ext.x*2, Ext.y));
	//ConvShape.setPoint(2, sf::Vector2f(Ext.x, Ext.y*2));
	//ConvShape.setPoint(3, sf::Vector2f(Ext.x*2, Ext.y*2));
}

Cube::Cube(sf::Vector2f Ext, sf::Vector2f Pos, sf::Vector2f Vel, float Mas)
{
	//CubeShape = sf::RectangleShape(Ext);
	Position = Pos;
	Velocity = Vel;
	Rotation = sf::Vector2f(0.f, 0.f);
	Acceleration = sf::Vector2f(0.f, 0.f);
	//CubeShape.setFillColor(sf::Color::Green);
	Mass = Mas;
	inertia = 1 / 12 * (1.f/Mass) * (pow(Ext.x, 2) + pow(Ext.y, 2));
	//CubeShape.setPosition(Position);
}

Cube::Cube(sf::Vector2f Ext, sf::Vector2f Pos, sf::Vector2f Vel, float Mas, sf::Vector2f Rot, sf::Color Col)
{
	//CubeShape = sf::RectangleShape(Ext);
	Position = Pos;
	Velocity = Vel;
	Rotation = Rot;
	Acceleration = sf::Vector2f(0.f, 0.f);
	//CubeShape.setFillColor(Col);
	Mass = Mas;

	//CubeShape.setPosition(Position);
}

Cube::~Cube()
{

}

//Asettaa sis‰v‰rin
void Cube::setColor(sf::Color Col)
{
	CubeColor = Col;
	//CubeShape.setFillColor(CubeColor);
	ConvShape.setFillColor(CubeColor);
}

//Asettaa uuden massan
void Cube::setMass(float Mas)
{
	Mass = Mas;
}

//Asettaa kappaleen uuden paikan Vektorina
void Cube::setPosition(sf::Vector2f Pos)
{
	Position = Pos;
	//CubeShape.setPosition(Position);
	ConvShape.setPosition(Position);
}

//Asettaa nopeuden vektorina
void Cube::setVelocity(sf::Vector2f Vel)
{
	Velocity = Vel;
}

//Asettaa kiihtyvyyden Vektorina
void Cube::setAcceleration(sf::Vector2f Acc)
{
	Acceleration = Acc;
}

//Aseta k‰‰ntyvyys kappaleelle Rotaatio-Vektorilla
void Cube::setRotation(sf::Vector2f Rot)
{
	Rotation = Rot;
	//CubeShape.setRotation(Rotation.x);
	ConvShape.setRotation(Rotation.x);
}


sf::Vector2f Cube::getPosition()
{
	return Position;
}

sf::Vector2f Cube::getVelocity()
{
	return Velocity;
}

sf::Vector2f Cube::getAcceleration()
{
	return Acceleration;
}

sf::Vector2f Cube::getRotation()
{
	return Rotation;
}

sf::ConvexShape& Cube::getConvShape()
{
	return ConvShape;
}

//K‰‰nn‰ kappaletta Deg m‰‰r‰ asteina
void Cube::Rotate(float Deg)
{
	//CubeShape.rotate(Deg);
	ConvShape.rotate(Deg);
}

void Cube::setDynamic(bool isDynamic)
{
	dynamic = isDynamic;
}

//T‰t‰ voi k‰ytt‰‰ tˆrm‰yksess‰?
sf::Vector2f Cube::getImpulse()		//Eikˆs t‰‰ ole ennemminkin voima?? 
{
	Impulse.x = Mass * Acceleration.x;
	Impulse.y = Mass * Acceleration.y;

	return Impulse;
}

float Cube::getAngularVelocity()
{
	return angularVelocity;
}

//Lis‰‰ impulssin
void Cube::addImpulse(sf::Vector2f Imp)
{
	//Acceleration.x = Acceleration.x + Impulse.x;
	//Acceleration.y = Acceleration.y + Impulse.y;

	//Acceleration = Acceleration + Impulse;
	Velocity = Velocity + Imp;
}

//P‰ivitt‰‰ nopeuden ja paikan
void Cube::Update(float DeltaTime)
{
	//Acceleration = Mass*DeltaTime;
	//Velocity = Velocity + (Acceleration * DeltaTime);
	Position = Position + (Velocity*DeltaTime);
	//CubeShape.setPosition(Position);
	ConvShape.setPosition(Position);

	//Update rotation based on angular velocity
	//CubeShape.setRotation(CubeShape.getRotation() + angularVelocity * DeltaTime);
	ConvShape.setRotation(ConvShape.getRotation() + angularVelocity * DeltaTime);
}

//Piirt‰‰ kappaleen
//void Cube::DrawCubeShape(sf::RenderWindow &window)
//{
//	window.draw(CubeShape);
//}

void Cube::DrawConvexShape(sf::RenderWindow &window)
{
	window.draw(ConvShape);
}
