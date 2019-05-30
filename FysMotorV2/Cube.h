#pragma once
#include <SFML\Graphics.hpp>


class Cube
{

public:
	Cube();
	Cube(sf::Vector2f Extents, sf::Vector2f Position, sf::Vector2f Velocity);
	Cube(sf::Vector2f Extents, sf::Vector2f Position, sf::Vector2f Velocity, float Mass);
	Cube(sf::Vector2f Extents, sf::Vector2f Position, sf::Vector2f Vel, float Mass, sf::Vector2f Rot, sf::Color Col);
	~Cube();

	//No need for this
	//sf::RectangleShape CubeShape;
	sf::ConvexShape ConvShape;

	sf::Vector2f Extents;
	sf::Vector2f Position;
	sf::Vector2f Velocity;
	float angularVelocity; //In degrees
	sf::Vector2f Rotation;
	sf::Vector2f Acceleration;
	sf::Vector2f Impulse;

	sf::Color CubeColor;
	void setColor(sf::Color);

	float Mass;
	void setMass(float Mass);
	bool dynamic = true;
	bool isWall = false;
	bool isVerticalWall = false;
	float e = 0.8f; //Restituutio kerroin
	float inertia;


	void setPosition(sf::Vector2f newposition);
	void setVelocity(sf::Vector2f newVelocity);
	void setAcceleration(sf::Vector2f newAcceleration);
	void setRotation(sf::Vector2f newRotation);


	//Näitä ei mahra tarvita koska haettavat arvot ovat julkisia???
	sf::Vector2f getPosition();
	sf::Vector2f getVelocity();
	sf::Vector2f getAcceleration();
	sf::Vector2f getRotation();
	float getAngularVelocity();

	sf::ConvexShape& getConvShape();

	void Rotate(float Degrees);

	void setDynamic(bool isDynamic);


	sf::Vector2f getImpulse();
	void addImpulse(sf::Vector2f Impulse);

	void Update(float DeltaTime);
	//void DrawCubeShape(sf::RenderWindow &window);
	void DrawConvexShape(sf::RenderWindow &window);

private:
	float maxVelocity;
};