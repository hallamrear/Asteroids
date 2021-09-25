#include "PCH.h"
#include "Collision.h"

namespace
{
	//Box to Box
	bool CheckCollision_AABBvsAABB(BoundingBox& one, BoundingBox& two)
	{
		return ((one.TopLeft.X <= two.BottomRight.X && one.BottomRight.X >= two.TopLeft.X)
			&&
			(one.TopLeft.Y <= two.BottomRight.Y && one.BottomRight.Y >= two.TopLeft.Y));
	};

	//Box to Sphere
	bool CheckCollision_AABBvsSPHERE(BoundingBox& one, BoundingSphere& two)
	{
		Vector2f corners[4] =
		{
			one.TopLeft,
			Vector2f(one.TopLeft.X, one.BottomRight.Y),
			one.BottomRight,
			Vector2f(one.BottomRight.X, one.TopLeft.Y)
		};

		//TODO : Finish

		//return dist <= two.mRadius * two.mRadius;
		return false;
	};

	//Sphere to Sphere
	bool CheckCollision_SPHEREvsSPHERE(BoundingSphere& one, BoundingSphere& two)
	{
		double deltaXSquared = one.mOrigin.X - two.mOrigin.X; // calc. delta X
		deltaXSquared *= deltaXSquared; // square delta X

		double deltaYSquared = one.mOrigin.Y - two.mOrigin.Y; // calc. delta Y
		deltaYSquared *= deltaYSquared; // square delta Y

		// Calculate the sum of the radii, then square it
		double sumRadiiSquared = one.mRadius + two.mRadius;
		sumRadiiSquared *= sumRadiiSquared;

		if (deltaXSquared + deltaYSquared <= sumRadiiSquared)
		{
			// A and B are touching
			return true;
		}

		return false;
	};


	bool CheckCollision_OBBvsOBB(OrientedBoundingBox& one, OrientedBoundingBox& two)
	{
		Vector2f pointsOne[4];
		one.GetBoxAsPoints(pointsOne);
		Vector2f pointsTwo[4];
		two.GetBoxAsPoints(pointsTwo);

		//todo : Make this useable with n points.
		size_t pointCount = sizeof(pointsOne) / sizeof(Vector2f);

		for (int s = 0; s < 2; s++)
		{
			if (s == 1)
			{
				two.GetBoxAsPoints(pointsOne);
				one.GetBoxAsPoints(pointsTwo);
			}
			else
			{
				one.GetBoxAsPoints(pointsOne);
				two.GetBoxAsPoints(pointsTwo);
			}

			//Check each shape direction
			for (size_t a = 0; a < pointCount; a++)
			{
				//wraparound
				int b = (a + 1) % pointCount;

				Vector2f axisProj = Vector2f(-(pointsOne[b].Y - pointsOne[a].Y), (pointsOne[b].X - pointsOne[a].X));

				float min_r1 = INFINITY; float max_r1 = -INFINITY;
				//todo : make one loop?
				for (size_t P = 0; P < pointCount; P++)
				{
					//project each point onto line 
					float q = pointsOne[P].Dot(axisProj);

					//get the min and max of the projection extents
					min_r1 = std::min(min_r1, q);
					max_r1 = std::max(min_r1, q);
				}

				float min_r2 = INFINITY; float max_r2 = -INFINITY;
				for (size_t P = 0; P < pointCount; P++)
				{
					//project each point onto line 
					float q = pointsTwo[P].Dot(axisProj);

					//get the min and max of the projection extents
					min_r2 = std::min(min_r2, q);
					max_r2 = std::max(max_r2, q);
				}
				
				//if they overlap, continue else if they dont, theyre not colliding so can return
				if (!(max_r2 >= min_r1) && (max_r1 >= min_r2))
					return false;
			}
		}
		return true;
	}

	bool CheckCollision_AABBvsOBB(BoundingBox& one, OrientedBoundingBox& two)
	{
		OrientedBoundingBox obb{one.mOrigin, 0.0f, (int)one.Size.X, (int)one.Size.Y };
		return CheckCollision_OBBvsOBB(obb, two);
	}
}


bool Collision_Detection::CheckCollision(Collider* one, Collider* two)
{
	if (one->mType == COLLIDER_TYPE::COLLIDER_AABB && two->mType == COLLIDER_TYPE::COLLIDER_AABB)
		return CheckCollision_AABBvsAABB(dynamic_cast<BoundingBox&>(*one), dynamic_cast<BoundingBox&>(*two));

	if (one->mType == COLLIDER_TYPE::COLLIDER_SPHERE && two->mType == COLLIDER_TYPE::COLLIDER_SPHERE)
		return CheckCollision_SPHEREvsSPHERE(dynamic_cast<BoundingSphere&>(*one), dynamic_cast<BoundingSphere&>(*two));

	if (one->mType == COLLIDER_TYPE::COLLIDER_AABB && two->mType == COLLIDER_TYPE::COLLIDER_SPHERE)
		return CheckCollision_AABBvsSPHERE(dynamic_cast<BoundingBox&>(*one), dynamic_cast<BoundingSphere&>(*two));

	if (one->mType == COLLIDER_TYPE::COLLIDER_SPHERE && two->mType == COLLIDER_TYPE::COLLIDER_AABB)
		return CheckCollision_AABBvsSPHERE(dynamic_cast<BoundingBox&>(*two), dynamic_cast<BoundingSphere&>(*one));

	if (one->mType == COLLIDER_TYPE::COLLIDER_AABB && two->mType == COLLIDER_TYPE::COLLIDER_OBB)
		return CheckCollision_AABBvsOBB(dynamic_cast<BoundingBox&>(*one), dynamic_cast<OrientedBoundingBox&>(*two));
	if (one->mType == COLLIDER_TYPE::COLLIDER_OBB && two->mType == COLLIDER_TYPE::COLLIDER_AABB)
		return CheckCollision_AABBvsOBB(dynamic_cast<BoundingBox&>(*two), dynamic_cast<OrientedBoundingBox&>(*one));
	if (one->mType == COLLIDER_TYPE::COLLIDER_OBB && two->mType == COLLIDER_TYPE::COLLIDER_OBB)
		return CheckCollision_OBBvsOBB(dynamic_cast<OrientedBoundingBox&>(*one), dynamic_cast<OrientedBoundingBox&>(*two));

	throw;
	return false;
};