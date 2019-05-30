#ifndef BASIC2DPHYSICS_COLLISIONDETECTOR_H
#define BASIC2DPHYSICS_COLLISIONDETECTOR_H


#include <vector>
#include <memory>
//
#include <Engine/UserInterface.hpp>
//
#include <CollisionData.h>


//CollisionDetector uses AABBs for broad-phase collision detection
//and SAT for narrow-phase collision detection. If collision is detected,
//shared pointer to a CollisionData -struct is returned.
class CollisionDetector
{
public:
	//Default constructor
	CollisionDetector();
	//Destructor
	~CollisionDetector();
	//No copies allowed
	CollisionDetector(const CollisionDetector&) = delete;
	void operator=(const CollisionDetector&) = delete;


	//DetectCollision -method. Uses broad- and narrow-phase.
	//return value: std::shared_ptr<CollisionData> if collision has occurred, nullptr if otherwise
	//-----------------------------------------------------------------
	//1. param: const ref to first object's vertices in world coordinates
	//2. param: const ref to first object's centerpoint in world coordinates
	//3. param: const ref to second object's vertices in world coordinates
	//4. param: const ref to second object's centerpoint in world coordinates
	std::shared_ptr<CollisionData> DetectCollision(const std::vector<emt::Vec2f>& f_verts, const emt::Vec2f& f_center, const std::vector<emt::Vec2f>& s_verts, const emt::Vec2f& s_center);

	//Translate coordinates from model-space to world-space if needed
	//return value: void
	//-----------------------------------------------------------------
	//1. param: const ref to container holding original vertices
	//2. param: ref to container in which the transleted vertices will be emplaced
	//3. param: const ref to a model matrix that is used to translate original vertices
	void CoordsToWorldSpace(const std::vector<emt::Vec3f>& from, std::vector<emt::Vec2f>& to, const emt::Mat4f& modelMat);

private:
	//Creates axis-aligned bounding boxes from given vertices and testes them. Return true if AABBs intersect
	bool _broadPhaseDet(const std::vector<emt::Vec2f>& f_vert,  const std::vector<emt::Vec2f>& s_vert);

	//Uses SAT algorhitm to detect if convex shapes are intersecting. If they are, returns shared pointer to newly formed CollisionData -struct. Otherwise returns nullptr
	std::shared_ptr<CollisionData> _narrowPhaseDet(const std::vector<emt::Vec2f>& f_vert, const std::vector<emt::Vec2f>& s_vert);


	//Create axis-aligned bounding box
	void _createAABB(const std::vector<emt::Vec2f>& verts, emt::Vec2f& max, emt::Vec2f& min);

	//Calculate projision vector for SAT
	void _calcProjVector(const std::vector<emt::Vec2f>& vertices, emt::Vec2f& proj, const emt::Vec2f& axis);

	//Calculate minimum overlap for projision, emplaces it and the current axis if overlap is the smallest this far.
	//Return true if new minimum overlap is found.
	bool _calcProjMinOverlap(const emt::Vec2f& f_proj, const emt::Vec2f& s_proj, emt::Vec2f axis, bool handlingSecObject = false);

	//Shared pointer to current CollisionData -struct
	std::shared_ptr<CollisionData> cdata;

	//Numerical limit values to initialize data
	const float m_max;
	const float m_min;

	//Min and max corners of two objects for AABB collision test
	//and min and max projections for SAT
	emt::Vec2f f_max;
	emt::Vec2f s_max;
	emt::Vec2f f_min;
	emt::Vec2f s_min;

	//Projection vectors
	emt::Vec2f f_proj;
	emt::Vec2f s_proj;

	//Minimum translating vector (first two values(x,y) are the axis(normal) that gives the smallest overlap, third value(z) is the smallest overlap)
	emt::Vec2f mtv;
	//Magnitude of mtv
	float mtv_magnitude;

	//Boolean for defining if smallest axis is derived from second object. False by default.
	bool mtv_is_from_s_obj;

	//First object's mtv and magnitude, default is zero
	emt::Vec2f f_mtv;
	float f_mtv_magnitude;

	//Second object's mtv and magnitude, default is zero
	emt::Vec2f s_mtv;
	float s_mtv_magnitude;

	//Indices of the colliding edge of the object from which the mtv is formed.
	emt::Vec2u coll_edge_ind;
};


#endif
