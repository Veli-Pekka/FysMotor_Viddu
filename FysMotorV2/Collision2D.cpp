#include "Collision2D.h"
#include <iostream>
#include <algorithm>


Collision2D::Collision2D(sf::RenderWindow* window)
	: window(window)
{

}


Collision2D::~Collision2D()
{
}

bool Collision2D::isOverlap(const sf::Vector2f& a, const sf::Vector2f& b, bool is_from_second)
{
	float overlap = (a.x > b.x) ? a.x - b.y : a.y - b.x;
	if (abs(overlap) < abs(currenOverlap))
	{
		currenOverlap = overlap;
		currenMTV = axis;
		colliding_edge = edge;

		if (is_from_second)
		{
			MTV_is_from_second = true;
		}
		return true;
	}
	return false;
}

//float Collision2D::getOverlapLenght(const sf::Vector2f& a, const sf::Vector2f& b)
//{
//
//
//	//if (!isOverlap(a, b)) { return 0.f; }
//	return std::min(a.y, b.y) - std::max(a.x, b.x);
//}

//Kollisionin check using SAT
bool Collision2D::SAT(Cube& p_obj_a, Cube& p_obj_b)
{
	obj_a = &p_obj_a;
	obj_b = &p_obj_b;

	auto& a = obj_a->ConvShape;
	auto& b = obj_b->ConvShape;

	currenOverlap = std::numeric_limits<float>::max();
	currenMTV = sf::Vector2f(0, 0);
	a_vertices.clear();
	b_vertices.clear();
	MTV_is_from_second = false;

	for (int i = 0; i < a.getPointCount(); i++)
	{

		sf::Vector2f temp_a = rotatePoint(a.getPoint(i), a.getRotation());
		sf::Vector2f tmp = a.getPosition();
		a_vertices.emplace_back(temp_a + a.getPosition());

	}
	for (int i = 0; i < b.getPointCount(); i++)
	{
		sf::Vector2f temp_b = rotatePoint(b.getPoint(i), b.getRotation());
		sf::Vector2f tmp = b.getPosition();
		b_vertices.emplace_back(temp_b + b.getPosition());

	}

	auto SATlambda = [&](std::vector<sf::Vector2f>& a_vertices, std::vector<sf::Vector2f>& b_vertices, bool is_from_second = false) -> bool {
		for (int i = 0; i < a_vertices.size(); i++)
		{

			float minA = 100000;
			float maxA = -100000;
			float minB = 100000;
			float maxB = -100000;

			if (i < a_vertices.size() - 2)
			{
				edge = a_vertices.at(i + 1) - a_vertices.at(i);
			}
			else
			{
				edge = a_vertices.at(0) - a_vertices.at(i);
			}

			axis = sf::Vector2f(-edge.y, edge.x);
			_normalize(axis);

			for (int i = 0; i < a_vertices.size(); i++)
			{
				float dp = a_vertices.at(i).x * axis.x + a_vertices.at(i).y* axis.y;

				if (dp < minA)
				{
					minA = dp;
				}
				if (dp > maxA)
				{
					maxA = dp;
				}

			}

			for (int i = 0; i < b_vertices.size(); i++)
			{
				float dp = b_vertices.at(i).x* axis.x + b_vertices.at(i).y * axis.y;

				if (dp < minB)
				{
					minB = dp;
				}
				if (dp > maxB)
				{
					maxB = dp;
				}

			}
			if (maxA < minB || maxB < minA)
			{
				return false;
			}
			else
			{
				if (isOverlap(sf::Vector2f(minA, maxA), sf::Vector2f(minB, maxB), is_from_second))
				{
					if (i < a_vertices.size() - 2)
					{
						coll_edge_indices = sf::Vector2u(i, (i + 1));
						coll_edge_point_1 = a_vertices.at(i);
						coll_edge_point_2 = a_vertices.at(i + 1);
					}
					else
					{
						coll_edge_indices = sf::Vector2u(i, 0);
						coll_edge_point_1 = a_vertices.at(i);
						coll_edge_point_2 = a_vertices.at(0);
					}

				}
			}

		}
		return true;
	};
	if (SATlambda(a_vertices, b_vertices) && SATlambda(b_vertices, a_vertices, true))
	{
		_normalize(currenMTV);

		CollisionSolver();
		//std::cout << "osui";

		//std::cout << " Collision x:" << collision_point.x << " ja y:";
		//std::cout << collision_point.y << std::endl;
		return true;
	}
	else
	{
		return false;
	}



}

sf::Vector2f Collision2D::rotatePoint(const sf::Vector2f& point, float angle)
{
	angle = angle * 3.14 / 180;
	sf::Vector2f rotatedPoint(point.x * cos(angle) + point.y * sin(angle), -point.x * sin(angle) + point.y * cos(angle));
	return rotatedPoint;
}

float Collision2D::getLength(const sf::Vector2f& v1)
{
	float len = sqrtf((v1.x * v1.x) + (v1.y * v1.y));

	return len;
}

//Collision solving
void Collision2D::CollisionSolver()
{
	bothDynamic = (obj_a->dynamic && obj_b->dynamic) ? true : false;

	//Separate objects using minimum translating vector.
	//This must be done so that we can calculate accurately collision point
	float factor = 1.1f;	//Erottaa kappaleet juuri ja juuri toisistaan jotta ne eiv‰t en‰‰ tˆrm‰‰ !!HUOM Vain debugissa!

	if (bothDynamic) //  || 1
	{
		if (MTV_is_from_second)
		{
			obj_a->Position.x -= factor * -currenMTV.x * currenOverlap;
			obj_a->Position.y -= factor * -currenMTV.y * currenOverlap;
			obj_a->ConvShape.setPosition(obj_a->Position);
			obj_b->Position.x -= factor * currenMTV.x * currenOverlap;
			obj_b->Position.y -= factor * currenMTV.y * currenOverlap;
			obj_b->ConvShape.setPosition(obj_b->Position);
		}
		else
		{
			obj_a->Position.x -= factor * currenMTV.x * currenOverlap;
			obj_a->Position.y -= factor * currenMTV.y * currenOverlap;
			obj_a->ConvShape.setPosition(obj_a->Position);
			obj_b->Position.x -= factor * -currenMTV.x * currenOverlap;
			obj_b->Position.y -= factor * -currenMTV.y * currenOverlap;
			obj_b->ConvShape.setPosition(obj_b->Position);
		}
		//FOR DEBUG
		//obj_a->Velocity = sf::Vector2f(0, 0);
		//obj_b->Velocity = sf::Vector2f(0, 0);
		//obj_a->angularVelocity = 0.0f;
		//obj_b->angularVelocity = 0.0f;
		//std::cout << "Osui";
		//return;
	}
	else
	{
		//Add static hit
		if (obj_a->dynamic)
		{
			//obj_a->setVelocity(obj_a->getVelocity()*-1.f);	// Kimmottaa takaisin tulosuuntaan
			printf("A dynamic!");

			sf::Vector2f tmpVel = obj_a->getVelocity();
			

			if (obj_b->isWall && obj_b->isVerticalWall)
			{
				obj_a->setVelocity(sf::Vector2f(tmpVel.x * -1.f ,tmpVel.y));	// K‰‰nnet‰‰n x suuntainen liike
			}
			else if (obj_b->isWall && !obj_b->isVerticalWall)
			{
				obj_a->setVelocity(sf::Vector2f(tmpVel.x, tmpVel.y * -1.f));	// K‰‰nnet‰‰n y suuntainen liike
			}
		}
		else
		{
			//obj_b->setVelocity(obj_b->getVelocity()*-1.f);	// Kimmottaa takaisin tulosuuntaan
			printf("B dynamic!");

			sf::Vector2f tmpVel = obj_b->getVelocity();

			if (obj_a->isWall && obj_a->isVerticalWall)
			{
				obj_b->setVelocity(sf::Vector2f(tmpVel.x * -1.f, tmpVel.y));	// K‰‰nnet‰‰n x suuntainen liike
			}
			else if (obj_a->isWall && !obj_a->isVerticalWall)
			{
				obj_b->setVelocity(sf::Vector2f(tmpVel.x, tmpVel.y * -1.f));	// K‰‰nnet‰‰n y suuntainen liike
			}
		}
	}

	//Calculate collision point
	//Current shortest distance from point to line, initialized to float max
	float shortest_dist(std::numeric_limits<float>::max());
	//Current indice of the vertex with shortest distance to the line
	unsigned int curr_index = 0;

	auto& verts = (!MTV_is_from_second) ? b_vertices : a_vertices;

	sf::Vector2f norm_edge = sf::Vector2f(coll_edge_point_2 - coll_edge_point_1);
	_normalize(norm_edge);

	for (unsigned int i = 0; i < verts.size(); i++)
	{
		sf::Vector2f p1_to_vertex = verts.at(i) - coll_edge_point_1;

		sf::Vector2f p_to_edge = p1_to_vertex - _dot(p1_to_vertex, norm_edge) * norm_edge;

		float length = getLength(p_to_edge);

		if (length < shortest_dist)
		{
			curr_index = i;
			shortest_dist = length;
		}
	}

	collision_point = verts.at(curr_index);

	//std::cout << "Normikohta! Collision x:" << collision_point.x << " ja y:";
	//std::cout << collision_point.y << std::endl << std::endl;

	//obj_a->Velocity = sf::Vector2f(0, 0);
	//obj_b->Velocity = sf::Vector2f(0, 0);
	//obj_a->angularVelocity = 0.0f;
	//obj_b->angularVelocity = 0.0f;
	//return;





	//Calculate and apply impulses
	sf::Vector2f a_r(collision_point - obj_a->Position);
	sf::Vector2f b_r(collision_point - obj_b->Position);

	a_r = sf::Vector2f(-a_r.y, a_r.x);
	b_r = sf::Vector2f(-b_r.y, b_r.x);

	float a_perp_dot_n = _dot(a_r, currenMTV);
	float b_perp_dot_n = _dot(b_r, currenMTV);

	if (!MTV_is_from_second) //
	{
		//printf("MTV from second?");
		sf::Vector2f relative_velocity((obj_a->Velocity + obj_a->angularVelocity * a_r) - (obj_b->Velocity + obj_b->angularVelocity * b_r));

		float relative_velocity_along_normal = _dot(relative_velocity, currenMTV);

		float rest_factor = std::min(obj_a->e, obj_b->e);
		//float rest_factor = (obj_a->e + obj_b->e) / 2.0f;

		float impulse = (1.0f + rest_factor)*relative_velocity_along_normal;

		impulse /= (1.0f / obj_a->Mass + 1.0f / obj_b->Mass) + (pow(a_perp_dot_n, 2) / obj_a->inertia) + (pow(b_perp_dot_n, 2) / obj_b->inertia);

		float b_ratio = 1.0f;// / obj_b->Mass / (1.0f / obj_a->Mass + 1.0f / obj_b->Mass);
		float a_ratio = 1.0f;// - b_ratio;

		float b_impulse = impulse * b_ratio;
		obj_b->Velocity += impulse * 1.0f / obj_b->Mass*currenMTV;
		obj_b->angularVelocity += (1.0f / obj_b->inertia) * _dot(b_r, b_impulse * currenMTV);

		impulse *= -1.0f; 
		float a_impulse = impulse * a_ratio;
		obj_a->Velocity += impulse * 1.0f / obj_a->Mass*currenMTV;
		obj_a->angularVelocity += (1.0f / obj_a->inertia) * _dot(a_r, a_impulse * currenMTV);

	}
	else
	{
		//printf("MTV from first?");
		sf::Vector2f relative_velocity((obj_b->Velocity + obj_b->angularVelocity * b_r) - (obj_a->Velocity + obj_a->angularVelocity * a_r));

		float relative_velocity_along_normal = _dot(relative_velocity, currenMTV);

		float rest_factor = std::min(obj_a->e, obj_b->e);
		//float rest_factor = (obj_a->e + obj_b->e) / 2.0f;

		float impulse = (1.0f + rest_factor)*relative_velocity_along_normal;

		impulse /= (1.0f / obj_b->Mass + 1.0f / obj_a->Mass) + (pow(b_perp_dot_n, 2) / obj_b->inertia) + (pow(a_perp_dot_n, 2) / obj_a->inertia);

		float a_ratio = 1.0f;// / obj_a->Mass / (1.0f / obj_b->Mass + 1.0f / obj_a->Mass);
		float b_ratio = 1.0f;// - a_ratio;

		float a_impulse = impulse * a_ratio;
		obj_a->Velocity += impulse * 1.0f / obj_a->Mass*currenMTV;
		obj_a->angularVelocity += (1.0f / obj_a->inertia) * _dot(a_r, a_impulse * currenMTV);

		impulse *= -1.0f;
		float b_impulse = impulse * b_ratio;
		obj_b->Velocity += impulse * 1.0f / obj_b->Mass*currenMTV;
		obj_b->angularVelocity += (1.0f / obj_b->inertia) * _dot(b_r, b_impulse * currenMTV);
	}

	//_sleep(5000);
}

void Collision2D::_normalize(sf::Vector2f& vec)
{
	float l = getLength(vec);
	vec.x /= l;
	vec.y /= l;
}

void Collision2D::DrawCollisionPoint()
{
	sf::CircleShape red(3.f);
	red.setPosition(collision_point);
	red.setFillColor(sf::Color::Red);
	window->draw(red);
}

//Toinen SAT	//TODO Ei toimi?!?!?
//bool Collision2D::SAT(sf::RectangleShape a, sf::RectangleShape b)
//{
//
//	currenOverlap = 10000;
//	currenMTV = sf::Vector2f(0, 0);
//	std::vector<sf::Vector2f> a_vertices;
//	std::vector<sf::Vector2f> b_vertices;
//
//	MTV_is_from_second = false;
//
//	for (int i = 0; i < a.getPointCount(); i++)
//	{
//
//		sf::Vector2f temp_a = rotatePoint(a.getPoint(i), a.getRotation());
//		a_vertices.emplace_back(temp_a + a.getPosition());
//
//	}
//	for (int i = 0; i < b.getPointCount(); i++)
//	{
//		sf::Vector2f temp_b = rotatePoint(b.getPoint(i), b.getRotation());
//		b_vertices.emplace_back(temp_b + b.getPosition());
//
//	}
//
//	auto SATlambda = [&](sf::RectangleShape& a, sf::RectangleShape& b, std::vector<sf::Vector2f>& a_vertices, std::vector<sf::Vector2f>& b_vertices, bool is_from_second = false) -> bool {
//		for (int i = 0; i < a_vertices.size(); i++)
//		{
//
//			float minA = 100000;
//			float maxA = -100000;
//			float minB = 100000;
//			float maxB = -100000;
//
//			if (i < a_vertices.size() - 2)
//			{
//
//				edge = a_vertices.at(i + 1) - a_vertices.at(i);
//				//edge = a.getPoint(i + 1)- a.getPoint(i);
//			}
//			else
//			{
//
//				edge = a_vertices.at(0) - a_vertices.at(i);
//				//edge = a.getPoint(0) - a.getPoint(i);
//			}
//
//			axis = sf::Vector2f(-edge.y, edge.x);
//
//			for (int i = 0; i < a_vertices.size(); i++)
//			{
//				float dp = a_vertices.at(i).x * axis.x + a_vertices.at(i).y* axis.y;
//
//				if (dp < minA)
//				{
//					minA = dp;
//				}
//				if (dp > maxA)
//				{
//					maxA = dp;
//				}
//
//			}
//
//			for (int i = 0; i < b_vertices.size(); i++)
//			{
//				float dp = b_vertices.at(i).x* axis.x + b_vertices.at(i).y * axis.y;
//
//				if (dp < minB)
//				{
//					minB = dp;
//				}
//				if (dp > maxB)
//				{
//					maxB = dp;
//				}
//
//			}
//			if (maxA < minB || maxB < minA)
//			{
//				return false;
//			}
//			else
//			{
//				isOverlap(sf::Vector2f(minA, maxA), sf::Vector2f(minB, maxB));
//			}
//
//		}
//		return true;
//	};
//	if (SATlambda(a, b, a_vertices, b_vertices) && SATlambda(b, a, b_vertices, a_vertices, true))
//	{
//
//
//		std::cout << "osui" << std::endl;
//		return true;
//	}
//	else
//	{
//		//std::cout << "Ei osu" << std::endl;
//		return false;
//	}
//
//
//
//}

//bool Collision2D::CollisionDetection(sf::ConvexShape a, float size_a, sf::ConvexShape b, float size_b)
//{
//
//
//
//
//	Vector2 vecA_0(a.getPosition().x + a.getPoint(0).x, a.getPosition().y + a.getPoint(0).y);
//
//	std::cout << vecA_0.x << " : " << vecA_0.y << std::endl;
//	Vector2 vecA_2(a.getPosition().x + a.getPoint(2).x, a.getPosition().y + a.getPoint(2).y);
//
//	Vector2 vecB_0(b.getPosition().x + b.getPoint(0).x, b.getPosition().y + b.getPoint(0).y);
//	Vector2 vecB_2(b.getPosition().x + b.getPoint(2).x, b.getPosition().y + b.getPoint(2).y);
//
//
//
//
//	//	Vector2 vecA_0_N = vecA_0.getNormal(vecA_0);
//	//	Vector2 vecA_2_N = vecA_2.getNormal(vecA_2);
//	//
//	//	Vector2 vecB_0_N = vecB_0.getNormal(vecB_0);
//	//	Vector2 vecB_2_N = vecB_2.getNormal(vecB_2);
//
//
//
//	Vector2 vecA_Pos(a.getPosition().x, a.getPosition().y);
//	Vector2 vecB_Pos(b.getPosition().x, b.getPosition().y);
//
//	std::cout << vecA_Pos.x << " : " << vecA_Pos.y << std::endl;
//
//
//	//Vector2 C(vecB_0.x - vecA_0.x, vecB_0.y - vecA_0.y);
//	//Vector2 A(vecA_2.x - vecA_0.x, vecA_2.y - vecA_0.y);
//	//Vector2 B(vecB_2.x - vecB_0.x, vecB_2.y - vecB_0.y);
//
//	//float numberC = C.calcDot2(C.x, 1, C.y, -1);
//	//float numberA = A.calcDot2(A.x, 1, A.y, -1);
//	//float numberB = B.calcDot2(B.x, 1, B.y, -1);
//
//	if (vecA_Pos.x < vecB_Pos.x + size_b &&
//		vecA_Pos.x + size_b > vecB_Pos.x &&
//		vecA_Pos.y < vecB_Pos.y + size_b &&
//		size_a + vecA_Pos.y > vecB_Pos.y)
//	{
//		std::cout << "Osuma" << std::endl;
//		return true;
//	}
//
//
//	//if (vecA_Pos.x < vecB_Pos.x + (b.getPoint(0).x + b.getPoint(2).x) &&
//	//	vecA_Pos.x + (a.getPoint(0).x + a.getPoint(2).x)  > vecB_Pos.x &&
//	//	vecA_Pos.y < vecB_Pos.y + (b.getPoint(0).y + b.getPoint(2).y )&&
//	//	(a.getPoint(0).y + a.getPoint(2).y)  + vecA_Pos.y > vecB_Pos.y)
//	//{
//	//	std::cout << "Osuma"<< std::endl;
//	//	return true;
//	//}
//	//
//
//
//	//float number = numberC - numberA + numberB;
//	//if (vecA_2_N.x < vecB_0_N.x || vecA_0_N.x > vecB_2_N.x)
//	//{
//	//	//std::cout << "ei osu" << std::endl;
//	//	return false;
//	//}
//	//else if (vecA_0_N.y < vecB_0_N.y || vecA_0.y > vecB_2_N.y)
//	//{
//	//	//std::cout << "ei osu" << std::endl;
//	//	return false;
//	//}
//	//else
//	//	std::cout <<"osu"<< std::endl;
//	//
//	return false;
//}

//Vector2 Collision2D::min(const sf::ConvexShape& a)
//{
//
//
//	Vector2 min((a.getPoint(0).x - a.getPoint(2).x), (a.getPoint(0).y - a.getPoint(2).y));
//	return min;
//
//}
//Vector2 Collision2D::max(const sf::ConvexShape& a)
//{
//	Vector2 max((a.getPoint(0).x + a.getPoint(2).x), (a.getPoint(0).y + a.getPoint(2).y));
//	return max;
//}