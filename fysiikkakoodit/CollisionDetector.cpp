#include <CollisionDetector.h>
//#include <Engine/Engine.hpp>
#include <limits>

CollisionDetector::CollisionDetector()
	: cdata(nullptr)
	, m_max(std::numeric_limits<float>::max())
	, m_min(-std::numeric_limits<float>::max())
	, f_max(m_min)
	, s_max(m_min)
	, f_min(m_max)
	, s_min(m_max)
	, f_proj(0.0f)
	, s_proj(0.0f)
	, mtv(m_max)
	, mtv_magnitude(m_max)
	, mtv_is_from_s_obj(false)
	, f_mtv(m_max)
	, f_mtv_magnitude(0.0f)
	, s_mtv(m_max)
	, s_mtv_magnitude(0.0f)
	, coll_edge_ind(0)
{

}
//
CollisionDetector::~CollisionDetector()
{
	cdata = nullptr;
}
//
std::shared_ptr<CollisionData> CollisionDetector::DetectCollision(const std::vector<emt::Vec2f>& f_verts, const emt::Vec2f& f_center, const std::vector<emt::Vec2f>& s_verts, const emt::Vec2f& s_center)
{
	//If broad-phase collision is detected, we call narrow-phase
	if (_broadPhaseDet(f_verts, s_verts))
	{
		//and re-init mtvs and their magnitude to max and mtv_is_from_s_obj to false
		mtv = emt::Vec2f(m_max);
		mtv_magnitude = m_max;
		mtv_is_from_s_obj = false;
		f_mtv = mtv;
		s_mtv = mtv;
		f_mtv_magnitude = m_max;
		s_mtv_magnitude = m_max;

		return _narrowPhaseDet(f_verts, s_verts);
	}
	return nullptr;
}
//
//privates
//
bool CollisionDetector::_broadPhaseDet(const std::vector<emt::Vec2f>& f_vert, const std::vector<emt::Vec2f>& s_vert)
{
	//Re-init mins and maxes
	f_max = emt::Vec2f(m_min);
	s_max = emt::Vec2f(m_min);
	f_min = emt::Vec2f(m_max);
	s_min = emt::Vec2f(m_max);

	//Get min and max values
	_createAABB(f_vert, f_max, f_min);
	_createAABB(s_vert, s_max, s_min);

	//Check if bounding boxes collide
	return (
		f_max.x > s_min.x &&
		f_min.x < s_max.x &&
		f_max.y > s_min.y &&
		f_min.y < s_max.y
		||
		s_max.x > f_min.x &&
		s_min.x < f_max.x &&
		s_max.y > f_min.y &&
		s_min.y < f_max.y
		);
}
//
std::shared_ptr<CollisionData> CollisionDetector::_narrowPhaseDet(const std::vector<emt::Vec2f>& f_vert, const std::vector<emt::Vec2f>& s_vert)
{
	//Re-init mins and maxes
	f_max = emt::Vec2f(m_min);
	s_max = emt::Vec2f(m_min);
	f_min = emt::Vec2f(m_max);
	s_min = emt::Vec2f(m_max);

	//Create placeholder for axis
	emt::Vec2f axis;

	auto lCheckPolygon = [&](const std::vector<emt::Vec2f>& f_vert, const std::vector<emt::Vec2f>& s_vert, emt::Vec2u& coll_edge_ind, bool handlingSecObj = false) -> bool
	{
		//Loop through all the polygon's axes
		unsigned int f_numOfAxes = f_vert.size();
		for (unsigned int i = 0; i < f_numOfAxes; i++)
		{
			//Edge indices
			unsigned int edge_ind_from = 0;
			unsigned int edge_ind_to = 0;

			//Get one side of the polygon
			//axis = (i < f_numOfAxes - 1) ? f_vert.at(i + 1) - f_vert.at(i) : f_vert.at(0) - f_vert.at(i);
			if (i < f_numOfAxes - 1)
			{
				edge_ind_from = i;
				edge_ind_to = i + 1;
				//axis = f_vert.at(i + 1) - f_vert.at(i);
			}
			else
			{
				edge_ind_from = i;
				edge_ind_to = 0;
				//axis = f_vert.at(0) - f_vert.at(i);
			}
			axis = f_vert.at(edge_ind_to) - f_vert.at(edge_ind_from);

			//Make it as left-hand normal (multiply y with -1, switch y and x)
			axis = emt::Vec2f(-axis.y, axis.x);
			//Normalize axis
			emt::Vec2f normalizedAxis = glm::normalize(axis);
			//Calculate projections
			_calcProjVector(f_vert, f_proj, normalizedAxis);
			_calcProjVector(s_vert, s_proj, normalizedAxis);

			//If gap is found, there's no collision
			if (f_proj.y < s_proj.x || f_proj.x > s_proj.y)
				return false;
			//If not, we check if this is the current smallest MTV (minimum translation vector) and continue
			else
			{
				//If calculated overlap is the smallest, we place indices to "coll_edge_ind"
				if (_calcProjMinOverlap(f_proj, s_proj, axis, handlingSecObj))
					coll_edge_ind = emt::Vec2u(edge_ind_from, edge_ind_to);
			}
		}
		return true;
	};
	//Check both polygons
	if (lCheckPolygon(f_vert, s_vert, coll_edge_ind) && lCheckPolygon(s_vert, f_vert, coll_edge_ind, true))
	{
		//If there is no gap in any of the axis, we must be colliding
		emt::DebugMessage msg("Bodies colliding", emt::MessageType::Info);

		//MTV should be normalized to ease calculations
		mtv = glm::normalize(mtv);

		//Placeholder for collision point
		emt::Vec2f coll_point(0.0f);

		//Check from which one of the objects the mtv originates
		//If it originates from the second object
		if (mtv_is_from_s_obj)
		{
			//we place mtv as a value for s_mtv
			s_mtv = mtv;
			//and negated mtv as a value for f_mtv
			f_mtv = -mtv;
			//and create new CollisionData -struct with penetrator's vertices as first parameter
			cdata = std::make_shared<CollisionData>(&f_vert, s_vert.at(coll_edge_ind.x), s_vert.at(coll_edge_ind.y), mtv, mtv_magnitude, mtv_is_from_s_obj, f_mtv, s_mtv);
		}
		//If it originates from the first, do the things vice versa
		else
		{
			f_mtv = mtv;
			s_mtv = -mtv;
			cdata = std::make_shared<CollisionData>(&s_vert, f_vert.at(coll_edge_ind.x), f_vert.at(coll_edge_ind.y), mtv, mtv_magnitude, mtv_is_from_s_obj, f_mtv, s_mtv);
		}

		return cdata;
	}
	//If there is gap between one of the axes, we are not colliding and return a nullptr as collision data
	return nullptr;
}
//
void CollisionDetector::CoordsToWorldSpace(const std::vector<emt::Vec3f>& from, std::vector<emt::Vec2f>& to, const emt::Mat4f& modelMat)
{
	for (auto& orig : from)
	{
		to.emplace_back(modelMat * emt::Vec4f(orig, 1.0f));
	}
}
//
//privates
//
void CollisionDetector::_createAABB(const std::vector<emt::Vec2f>& verts, emt::Vec2f& max, emt::Vec2f& min)
{
	for (auto& v : verts)
	{
		//Check min x
		if (v.x < min.x)
			min.x = v.x;
		//Check max x
		if (v.x > max.x)
			max.x = v.x;
		//Check min y
		if (v.y < min.y)
			min.y = v.y;
		//Check max y
		if (v.y > max.y)
			max.y = v.y;
	}
}
//
void CollisionDetector::_calcProjVector(const std::vector<emt::Vec2f>& vertices, emt::Vec2f& proj, const emt::Vec2f& axis)
{
	//We calculate dot product with normalized axis and every vertice to get minimum and maximum 
	//projection values for later use (see "_calcProjMinOverlap()")
	float min = m_max;
	float max = m_min;
	for (const auto& v : vertices)
	{
		//Calculate dot product
		float dp = glm::dot(axis, v);
		//Compare to min and max values
		if (dp < min)
			min = dp;
		if (dp > max)
			max = dp;
	}
	//In this 1D-vector x is min, y is max
	proj = emt::Vec2f(min, max);
}
//
bool CollisionDetector::_calcProjMinOverlap(const emt::Vec2f& f_proj, const emt::Vec2f& s_proj, emt::Vec2f axis, bool handlingSecObject)
{
	//Check how projections are oriented in the axis (first left, then second right, or vice versa) and
	//calculate overlap accordingly
	float overlap = (f_proj.x > s_proj.x) ? (f_proj.x - s_proj.y) : (f_proj.y - s_proj.x);

	//If absolute value of the overlap is smaller than the current absolute value of the mtv's magnitude,
	//place current axis as the mtv and current overlap as the magnitude
	if (abs(overlap) < abs(mtv_magnitude))
	{
		mtv = axis;
		mtv_magnitude = overlap;
		if (handlingSecObject)
			mtv_is_from_s_obj = true;
		return true;
	}
	return false;
}