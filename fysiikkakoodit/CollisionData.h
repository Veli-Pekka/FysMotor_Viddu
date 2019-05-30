#ifndef BASIC2DPHYSICS_COLLISIONDATA_H
#define BASIC2DPHYSICS_COLLISIONDATA_H

#include <vector>

#include <Engine/UserInterface.hpp>

#ifndef PI
#define PI 3.14159265359f
#endif



//CollisionData -struct contains collision information, such as minimum translation vector,
//...
struct CollisionData
{
	//Constructor
	CollisionData(
		const std::vector<emt::Vec2f>* verts_of_pentr,
		emt::Vec2f p1_on_edge,
		emt::Vec2f p2_on_edge,
		emt::Vec2f mtv,
		float mtv_magnitude,
		bool mtvisfroms,
		emt::Vec2f f_mtv = emt::Vec2f(0.0f),
		emt::Vec2f s_mtv = emt::Vec2f(0.0f)
	)
		: verts_of_pentr(verts_of_pentr)
		, p1_on_edge(p1_on_edge)
		, p2_on_edge(p2_on_edge)
		, mtv(mtv)
		, mtv_magnitude(mtv_magnitude)
		, mtv_is_from_s(mtvisfroms)
		, f_mtv(f_mtv)
		, s_mtv(s_mtv)
		, coll_point(0.0f)
	{}
	//Destructor
	~CollisionData() { verts_of_pentr = nullptr; }

	//Const pointer to container holding the penetrating object's vertices in world coordinates. Used for calculating collision point
	const std::vector<emt::Vec2f>* verts_of_pentr;

	//Vertices that form the colliding edge.
	emt::Vec2f p1_on_edge;
	emt::Vec2f p2_on_edge;

	//Minimum translation vector
	emt::Vec2f mtv;

	//Magnitude of minimun translating vector
	float mtv_magnitude;

	//Boolean for defining if minimum translating vector is formed from second object's axis.
	bool mtv_is_from_s;

	//First object's mtv, default is zero
	emt::Vec2f f_mtv;

	//Second object's mtv, default is zero
	emt::Vec2f s_mtv;

	//Collision point in world coordinates. Initialized to 0.0f
	emt::Vec2f coll_point;

};


#endif
