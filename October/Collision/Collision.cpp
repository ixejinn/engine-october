#include "Collision.h"

#include <glm/glm.hpp>
#include "../Component/FixedUpdatable/BoxCollider.h"
#include "Manifold.h"

glm::vec2 GetVertexMinDotproduct(glm::vec2 normal, BoxCollider* other)
{
	double minDotProduct = DBL_MAX;
	glm::vec2 minVertex{};

	for (int i = 0; i < 4; i++)
	{
		glm::vec2 v = other->GetVertex(i);
		double dotproduct = glm::dot(v, normal);

		if (dotproduct < minDotProduct)
		{
			minVertex = v;
			minDotProduct = dotproduct;
		}
	}

	return minVertex;
}

double FindAxisLeastPenetration(unsigned int* faceIdx, BoxCollider* box1, BoxCollider* box2)
{
	double leastPenetration = -DBL_MAX;
	unsigned int leastIdx = 0;

	for (int i = 0; i < 4; i++)
	{
		double dotproduct = glm::dot(box1->GetNormal(i), GetVertexMinDotproduct(box1->GetNormal(i), box2) - box1->GetVertex(i));

		if (dotproduct > leastPenetration)
		{
			leastIdx = i;
			leastPenetration = dotproduct;
		}
	}

	*faceIdx = leastIdx;
	return leastPenetration;
}

void FindIncidentFace(glm::vec2* incFaceIdx, const glm::vec2& refNormal, BoxCollider* incident)
{
	int faceIdx = 0;
	double minDotproduct = DBL_MAX;

	for (int i = 0; i < 4; i++)
	{
		double dotproduct = glm::dot(refNormal, incident->GetNormal(i));

		if (dotproduct < minDotproduct)
		{
			faceIdx = i;
			minDotproduct = dotproduct;
		}
	}

	incFaceIdx[0] = incident->GetNormal(faceIdx);
	faceIdx = faceIdx < 4 ? faceIdx + 1 : 0;
	incFaceIdx[1] = incident->GetNormal(faceIdx);
}

int Clip(const glm::vec2& normal, float c, glm::vec2* incFace)
{
	int sp = 0;
	glm::vec2 out[2] = { incFace[0], incFace[1] };

	float d1 = glm::dot(normal, incFace[0]) - c;
	float d2 = glm::dot(normal, incFace[1]) - c;

	if (d1 <= 0.f) out[sp++] = incFace[0];
	if (d2 <= 0.f) out[sp++] = incFace[1];

	if (d1 * d2 < 0.f)
	{
		float alpha = d1 / (d1 - d2);
		if (sp < 2)
			out[sp] = incFace[0] + alpha * (incFace[1] - incFace[0]);
		++sp;
	}

	incFace[0] = out[0];
	incFace[1] = out[1];

	return sp;
}

bool ConvextoConvex(Manifold* m)
{
	unsigned int faceIdx1;
	double penetration1 = FindAxisLeastPenetration(&faceIdx1, static_cast<BoxCollider*>(m->col1_), static_cast<BoxCollider*>(m->col2_));
	if (penetration1 >= 0)
		return false;

	unsigned int faceIdx2;
	double penetration2 = FindAxisLeastPenetration(&faceIdx2, static_cast<BoxCollider*>(m->col2_), static_cast<BoxCollider*>(m->col1_));
	if (penetration2 >= 0)
		return false;

	BoxCollider* reference{ nullptr };
	BoxCollider* incident{ nullptr };
	unsigned int refNormalIdx;
	bool flip{ false };

	if (penetration1 >= penetration2)
	{
		reference = static_cast<BoxCollider*>(m->col1_);
		incident = static_cast<BoxCollider*>(m->col2_);
		refNormalIdx = faceIdx1;
	}
	else
	{
		flip = true;

		reference = static_cast<BoxCollider*>(m->col2_);
		incident = static_cast<BoxCollider*>(m->col1_);
		refNormalIdx = faceIdx2;
	}

	glm::vec2 incidentFace[2]{};
	FindIncidentFace(incidentFace, reference->GetNormal(refNormalIdx), incident);

	glm::vec2 referenceFace[2]{};
	referenceFace[0] = reference->GetVertex(refNormalIdx);
	referenceFace[1] = reference->GetVertex(refNormalIdx + 1 < 4 ? refNormalIdx + 1 : 0);

	glm::vec2 sidePlaneNormal = referenceFace[1] - referenceFace[0];
	sidePlaneNormal = glm::normalize(sidePlaneNormal);
	glm::vec2 refNormal{ sidePlaneNormal.y, -sidePlaneNormal.x };

	//////////////////
	float refC = glm::dot(refNormal, referenceFace[0]);
	float negSide = -glm::dot(sidePlaneNormal, referenceFace[0]);
	float posSide = glm::dot(sidePlaneNormal, referenceFace[1]);

	if (Clip(-sidePlaneNormal, negSide, incidentFace) < 2)
		return false;

	if (Clip(sidePlaneNormal, posSide, incidentFace) < 2)
		return false;

	m->normal_ = flip ? -refNormal : refNormal;

	int cp = 0;
	float separation = glm::dot(refNormal, incidentFace[0]) - refC;
	if (separation <= 0.f)
	{
		m->contacts_[cp] = incidentFace[0];
		m->penetration_ = -separation;
		++cp;
	}
	else
		m->penetration_ = 0;

	separation = glm::dot(refNormal, incidentFace[1]) - refC;
	if (separation <= 0.f)
	{
		m->contacts_[cp] = incidentFace[1];
		m->penetration_ += -separation;
		++cp;
		m->penetration_ /= (float)cp;
	}
	
	m->contactCnt_ = cp;
	return cp > 0;

	return true;
}

bool ConvextoCircle(Manifold* m)
{
	return false;
}

bool CircletoConvex(Manifold* m)
{
	return false;
}

bool CircletoCircle(Manifold* m)
{
	return false;
}

CollisionCallback Dispatch[Collider::ColliderTypeCnt][Collider::ColliderTypeCnt] =
{
	{ ConvextoConvex, ConvextoCircle },
	{ CircletoConvex, CircletoCircle }
};