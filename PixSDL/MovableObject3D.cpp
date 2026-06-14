#include "MovableObject3D.h"

namespace pix
{
	MovableObject3D::MovableObject3D(const Transform3D& transform) :
		Transform(transform),
		prevTransform_(Transform)
	{}

	MovableObject3D::MovableObject3D(const Transform3D& transform, const Transform3D& prevTransform) :
		Transform(transform),
		prevTransform_(prevTransform)
	{}

	MovableObject3D::MovableObject3D(const Vec3& position, const Vec3f& scale, const Rotation3D& rotation) :
		Transform(position, scale, rotation),
		prevTransform_(Transform)
	{}

	void MovableObject3D::BeginUpdate()
	{
		prevTransform_ = Transform;
	}

	void MovableObject3D::TeleportTo(const Transform3D& transform)
	{
		Transform = transform;
		prevTransform_ = transform;
	}

	void MovableObject3D::TeleportTo(const Vec3& position)
	{
		Transform.Position = position;
		prevTransform_.Position = position;
	}

	void MovableObject3D::ShiftTo(const Vec3& newPosition)
	{
		Vec3 prevOffset = prevTransform_.Position - Transform.Position;

		Transform.Position = newPosition;
		prevTransform_.Position = newPosition + prevOffset;
	}

	const Transform3D& MovableObject3D::GetPrevTransform() const
	{
		return prevTransform_;
	}

}

