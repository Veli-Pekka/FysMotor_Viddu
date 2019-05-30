#include <CollisionSolver.h>
#include <limits>

CollisionSolver::CollisionSolver()
	: coll_is_dynDyn(false)
{

}
//
CollisionSolver::~CollisionSolver()
{

}
//
//void CollisionSolver::SolveCollision(CollisionData& cdata, emt::TransformComponent& f_trans, const emt::ModelComponent& f_model, emt::TransformComponent& s_trans, const emt::ModelComponent& s_model)
void CollisionSolver::SolveCollision(CollisionData& cdata, float deltaTime, const float f_inv_mass, const float f_e, const float f_inertia, const bool f_isDyn, emt::Vec2f& f_velo, float& f_ang_velo, emt::Vec3f& f_pos, const float s_inv_mass, const float s_e, const float s_inertia, const bool s_isDyn, emt::Vec2f& s_velo, float& s_ang_velo, emt::Vec3f& s_pos)
{
	//At first, we define our type of collision
	coll_is_dynDyn = (f_isDyn && s_isDyn) ? true : false;

	//Then we move objects out of collision
	_separateObjects(cdata, f_inv_mass, f_pos, f_isDyn, s_inv_mass, s_pos, s_isDyn);

	//and calculate the point of collision. As we have moved objects out of collision, collision point is the
	//vertex of the colliding object that is closest to other object's colliding edge
	_calculateCollisionPoint(cdata);

	//Calculate restitution
	//float e = (f_e + s_e) / 2.0f;
	float e = std::min(f_e, s_e);

	//Calculate and apply impulses on objects
	_calculateAndApplyImpulses(cdata, e, f_pos, f_velo, f_ang_velo, f_inv_mass, f_inertia, f_isDyn, s_pos, s_velo, s_ang_velo, s_inv_mass, s_inertia, s_isDyn);

	//FOR TESTING
	//f_velo = emt::Vec2f(0);
	//f_ang_velo = 0.0f;
	//s_velo = emt::Vec2f(0);
	//s_ang_velo = 0.0f;
}
//
//privates
//
void CollisionSolver::_separateObjects(const CollisionData& cdata, float f_inv_mass, emt::Vec3f& f_pos, bool f_isDyn, float s_inv_mass, emt::Vec3f& s_pos, bool s_isDyn)
{
	//Factor by which the amount of movement is multiplied.
	//Value 1.01f should be enough.
	float factor = 1.01f;

	//If one of our pair of objects is static, we do this operation for moving objects out of collision
	if (!coll_is_dynDyn)
	{
		if (f_isDyn)
		{
			f_pos.x -= factor * cdata.f_mtv.x * cdata.mtv_magnitude;
			f_pos.y -= factor * cdata.f_mtv.y * cdata.mtv_magnitude;
		}
		else if (s_isDyn)
		{
			s_pos.x -= factor * cdata.s_mtv.x * cdata.mtv_magnitude;
			s_pos.y -= factor * cdata.s_mtv.y * cdata.mtv_magnitude;
		}
	}
	//Otherwise, both are dynamic and we do separation little differently
	else
	{
		//Ratio by which we move each object, heavier object moves less as we assume that lighter object tend to move
		//faster. This should give more eye-pleasing result.
		float s_ratio = s_inv_mass / (f_inv_mass + s_inv_mass);
		f_pos.x -= factor * cdata.f_mtv.x * cdata.mtv_magnitude * (1.0f - s_ratio);
		f_pos.y -= factor * cdata.f_mtv.y * cdata.mtv_magnitude * (1.0f - s_ratio);
		//
		s_pos.x -= factor * cdata.s_mtv.x * cdata.mtv_magnitude * s_ratio;
		s_pos.y -= factor * cdata.s_mtv.y * cdata.mtv_magnitude * s_ratio;
	}
}
//
void CollisionSolver::_calculateCollisionPoint(CollisionData& cdata)
{
	//Current shortest distance from point to line, initialized to float max
	float shortest_dist(std::numeric_limits<float>::max());
	//Current indice of the vertex with shortest distance to the line
	unsigned int curr_index = 0;
	//Normalized edge
	emt::Vec2f norm_edge = glm::normalize(cdata.p2_on_edge - cdata.p1_on_edge);
	//
	auto verts = cdata.verts_of_pentr;

	//For every vertice we calculate it's distance from the edge
	for (unsigned int i = 0; i < verts->size(); i++)
	{
		//Vector from point on edge to current vertex 
		emt::Vec2f p1_to_vertex = verts->at(i) - cdata.p1_on_edge;
		//Vector from vertex to line
		emt::Vec2f p_to_line = p1_to_vertex - glm::dot(p1_to_vertex, norm_edge) * norm_edge;
		//Vector's length. Since we compare only magnitudes, we can skip the square root
		float lenght = glm::length2(p_to_line);
		//Check if it the closest vertex
		if (lenght < shortest_dist)
		{
			curr_index = i;
			shortest_dist = lenght;
		}
	}

	//TEST 
	//We check that if two shortest vertices are at certain (low) distance away from each other. If they are,
	//we calculate collision point to be in middle of them.

	//TEST end

	//We init factor depenging on our collision type
	float factor = (coll_is_dynDyn) ? 1.0f : 2.0f;
	factor = 1.0f;

	//TO BE REMOVED factor ei toimi kunnolla, tasasivuisissa törmäyksissä pitäisi laskea kahden collision pointin välinen totuus???

	//Collision point is the vertex in curr_index translated by correct mtv, magnitude and factor!
	if (cdata.mtv_is_from_s)
		cdata.coll_point = verts->at(curr_index) - (cdata.s_mtv * cdata.mtv_magnitude) * factor;
	else										 
		cdata.coll_point = verts->at(curr_index) - (cdata.f_mtv * cdata.mtv_magnitude) * factor;


	////TO BE REMOVED
	/*auto s_Model = emt::CreateComponent(emt::ModelComponent(emt::Triangle(), emt::ModelDynamicState::Dynamic));
	auto s_Texture = emt::CreateComponent(emt::TextureComponent("lightM.png"));
	auto s_Trans = emt::CreateComponent(emt::TransformComponent(emt::Vec3f(cdata.coll_point, 0.1f), emt::Vec3f(0.0f, 0.0f, 0.f), emt::Vec3f(0.1f, 0.1f, 0.1f)));
	auto s_Render = emt::CreateComponent(emt::RenderComponent(s_Model, s_Texture, s_Trans));*/
}
//
void CollisionSolver::_calculateAndApplyImpulses(const CollisionData& cdata, float e, emt::Vec3f& f_pos, emt::Vec2f& f_velo, float& f_ang_velo, float f_inv_mass, float f_inertia, bool f_isDyn, emt::Vec3f& s_pos, emt::Vec2f& s_velo, float& s_ang_velo, float s_inv_mass, float s_inertia, bool s_isDyn)
{
	//Dynamic-dynamic collision
	if (coll_is_dynDyn)
	{
		if (!cdata.mtv_is_from_s)
		{
			//Calculate and apply impulse
			_calcAndApplyDynDynImpulse(cdata, e, f_pos, f_velo, f_ang_velo, f_inv_mass, f_inertia, s_pos, s_velo, s_ang_velo, s_inv_mass, s_inertia);
		}
		else
		{
			//Calculate and apply impulse
			_calcAndApplyDynDynImpulse(cdata, e, s_pos, s_velo, s_ang_velo, s_inv_mass, s_inertia, f_pos, f_velo, f_ang_velo, f_inv_mass, f_inertia);
		}
	}
	//Static-dynamic collision
	else
	{
		if (f_isDyn)
		{
			//Calculate and apply impulse
			_calcAndApplyDynStatImpulse(cdata, e, f_pos, f_velo, f_ang_velo, f_inv_mass, f_inertia);
		}
		else
		{
			//Calculate and apply impulse
			_calcAndApplyDynStatImpulse(cdata, e, s_pos, s_velo, s_ang_velo, s_inv_mass, s_inertia);
		}
	}
}
//
void CollisionSolver::_calcAndApplyDynDynImpulse(const CollisionData& cdata, float e, emt::Vec3f& f_pos, emt::Vec2f& f_velo, float& f_ang_velo, float f_inv_mass, float f_inertia, emt::Vec3f& s_pos, emt::Vec2f& s_velo, float& s_ang_velo, float s_inv_mass, float s_inertia)
{
	const auto& coll_normal = cdata.mtv;

	//Vector from positions to collision point
	emt::Vec2f f_r = emt::Vec2f(cdata.coll_point - emt::Vec2f(f_pos));
	emt::Vec2f s_r = emt::Vec2f(cdata.coll_point - emt::Vec2f(s_pos));
	//Make then perpendicular (left hand)
	f_r = emt::Vec2f(-f_r.y, f_r.x);
	s_r = emt::Vec2f(-s_r.y, s_r.x);

	//Perpendicular dot product
	float f_perp_dot_n = glm::dot(f_r, coll_normal);
	float s_perp_dot_n = glm::dot(s_r, coll_normal);

	//Vector from objects' points' relative velocity
	emt::Vec2f relat_velo((s_velo + s_ang_velo * s_r) - (f_velo + f_ang_velo * f_r));

	float relat_velo_along_normal = glm::dot(relat_velo, coll_normal);
	/*if (relat_velo_along_normal < 0)
	return;*/

	float impulse = -(1 + e) * relat_velo_along_normal;
	impulse /= (f_inv_mass + s_inv_mass) + (pow(f_perp_dot_n, 2) / f_inertia) + (pow(s_perp_dot_n, 2) / s_inertia);
	//impulse /= glm::dot(coll_normal, (coll_normal *invTotalMass)) + (pow(f_perp_dot_n, 2) / f_inertia) + (pow(s_perp_dot_n, 2) / s_inertia);

	//Get impulse ratio
	float s_ratio = s_inv_mass / (f_inv_mass + s_inv_mass);
	float f_ratio = 1.0f - s_ratio;
	//float s_ratio = 1.0f;
	//float f_ratio = 1.0f;

	//Apply impulse
	float s_impulse = impulse * s_ratio;
	s_velo += s_impulse * s_inv_mass * coll_normal;
	s_ang_velo += (1.0f / s_inertia) *  glm::dot(s_r, s_impulse * coll_normal);
	//
	impulse *= -1.0f;
	float f_impulse = impulse * f_ratio;
	f_velo += f_impulse * f_inv_mass * coll_normal;
	f_ang_velo += (1.0f / f_inertia) *  glm::dot(f_r, f_impulse * coll_normal);
}
//
void CollisionSolver::_calcAndApplyDynStatImpulse(const CollisionData& cdata, float e, emt::Vec3f& position, emt::Vec2f& velo, float& ang_velo, float inv_mass, float inertia)
{
	//Alias
	const auto& coll_normal = cdata.mtv;

	//Vector from positions to collision point
	emt::Vec2f r = emt::Vec2f(cdata.coll_point - emt::Vec2f(position));
	//Make it perpendicular (left hand)
	r = emt::Vec2f(-r.y, r.x);

	//Perpendicular dot product
	float perp_dot_n = glm::dot(r, coll_normal);

	//Vector from objects' points' relative velocity
	emt::Vec2f relat_velo((velo + ang_velo * r));

	float relat_velo_along_normal = glm::dot(relat_velo, coll_normal);
	/*if (relat_velo_along_normal < 0)
	return;*/

	//Calculate impulse
	float impulse = -(1 + e) * relat_velo_along_normal;
	impulse /= inv_mass + (pow(perp_dot_n, 2) / inertia);
	//impulse /= glm::dot(coll_normal, (coll_normal * f_m_inv)) + (pow(perp_dot_n, 2) / inertia);

	//Apply negated impulse solely on angular velocity
	impulse *= -1.0f;
	ang_velo -= (1.0f / inertia) *  glm::dot(r, impulse * coll_normal);
	//Mirror velocity with normal and scale with restitution factor
	velo = (velo - 2 * glm::dot(velo, coll_normal) * coll_normal) * e;
}
