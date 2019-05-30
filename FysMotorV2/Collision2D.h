#ifndef COLLISION2D_H
#define COLLISION2D_H
//#include "Vector2.h"
#include <SFML\Graphics.hpp>
#include <Cube.h>
#include <math.h>
#include <vector>


class Collision2D
{
	void _normalize(sf::Vector2f& vec);
	float _dot(const sf::Vector2f a, const sf::Vector2f b)
	{
		return a.x * b.x + a.y * b.y;
	}

public:
	Collision2D(sf::RenderWindow* window);
	~Collision2D();


	bool SAT(Cube& obj_a, Cube& obj_b);

	void CollisionSolver();

	//bool SAT(sf::RectangleShape a, sf::RectangleShape b);	//TODO Ei toimi?!?!?
	bool isOverlap(const sf::Vector2f& a, const sf::Vector2f& b, bool is_from_second = false);
	sf::Vector2f rotatePoint(const sf::Vector2f& point, float angle);

	float getLength(const sf::Vector2f& v1);
	
	float currenOverlap;
	sf::Vector2f currentAxis;
	sf::Vector2f currenMTV;
	sf::Vector2f edge;
	sf::Vector2u coll_edge_indices;
	sf::Vector2f coll_edge_point_1;
	sf::Vector2f coll_edge_point_2;
	sf::Vector2f colliding_edge;
	sf::Vector2f axis;
	bool MTV_is_from_second;

	std::vector<sf::Vector2f> a_vertices;
	std::vector<sf::Vector2f> b_vertices;

	sf::Vector2f collision_point;

	Cube* obj_a;
	Cube* obj_b;
	bool bothDynamic;

	sf::RenderWindow* window;

	void DrawCollisionPoint();
};
#endif
