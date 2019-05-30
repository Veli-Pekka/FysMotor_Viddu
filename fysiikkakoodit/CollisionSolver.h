#ifndef BASIC2DPHYSICS_COLLISIONSOLVER_H
#define BASIC2DPHYSICS_COLLISIONSOLVER_H

#include <Engine/UserInterface.hpp>
//
#include <CollisionData.h>

class CollisionSolver
{
public:
	//
	CollisionSolver();
	//
	~CollisionSolver();
	//
	CollisionSolver(const CollisionSolver&) = delete;
	void operator=(const CollisionSolver&) = delete;

	//SolveCollision -method. Takes in objects' dynamic qualities and CollisionData -struct
	void SolveCollision(CollisionData& cdata, float deltaTime, const float f_inv_mass, const float f_e, const float f_inertia, const bool f_isDyn, emt::Vec2f& f_velo, float& f_ang_velo, emt::Vec3f& f_pos, const float s_inv_mass, const float s_e, const float s_inertia, const bool s_isDyn, emt::Vec2f& s_velo, float& s_ang_velo, emt::Vec3f& s_pos);


private:
	//Separate objects so that they don't intersect each other
	void _separateObjects(const CollisionData& cdata, float f_mass, emt::Vec3f& f_pos, bool f_isDyn, float s_mass, emt::Vec3f& s_pos, bool s_isDyn);


	//Calculate collision point, return it's position. Notice that this position is inside an other object so we must
	//translate it with minimum translating vector later on in CollisionSolver!
	//emt::Vec2f _calculateCollisionPoint(const std::vector<emt::Vec2f>& verts, const emt::Vec2f& p1_edge, const emt::Vec2f& p2_edge);
	void _calculateCollisionPoint(CollisionData& cdata);

	
	//Calculate impulses and apply them to objects. Depending on collision type, this calls either "_calcAndApplyDynDynImpulse()" or "_calcAndApplyDynStatImpulse()"
	void _calculateAndApplyImpulses(const CollisionData& cdata, float e, emt::Vec3f& f_pos, emt::Vec2f& f_velo, float& f_ang_velo, float f_inv_mass, float f_inertia, bool f_isDyn, emt::Vec3f& s_pos, emt::Vec2f& s_velo, float& s_ang_velo, float s_inv_mass, float s_inertia, bool s_isDyn);
	//
	//Calculate and apply impulse between two dynamic objects. First object given to method must be the one which has the colliding edge.
	void _calcAndApplyDynDynImpulse(const CollisionData& cdata, float e,  emt::Vec3f& f_pos, emt::Vec2f& f_velo, float& f_ang_velo, float f_inv_mass, float f_inertia, emt::Vec3f& s_pos, emt::Vec2f& s_velo, float& s_ang_velo, float s_inv_mass, float s_inertia);
	//
	//Calculate and apply impulse between dynamic and static object
	void _calcAndApplyDynStatImpulse(const CollisionData& cdata, float e, emt::Vec3f& position, emt::Vec2f& velo, float& ang_velo, float inv_mass, float inertia);


	//Boolean for defining if current collision is dyn-dyn or dyn-static
	bool coll_is_dynDyn;

};
#endif
