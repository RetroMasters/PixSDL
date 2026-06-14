#include "MovableObject2D.h"

namespace pix
{

	MovableObject2D::MovableObject2D(const Transform2D& transform) :
		Transform(transform),
		prevTransform_(transform)
	{}

	MovableObject2D::MovableObject2D(const Transform2D& transform, const Transform2D& prevTransform) :
		Transform(transform),
		prevTransform_(prevTransform)
	{}

	MovableObject2D::MovableObject2D(const Vec2& position, const Vec2f& scale, const Rotation2D& rotation) :
		Transform(position, scale, rotation),
		prevTransform_(Transform)
	{}

	void MovableObject2D::BeginUpdate()
	{
		prevTransform_ = Transform;
	}

	void MovableObject2D::TeleportTo(const Transform2D& transform)
	{
		Transform = transform;
		prevTransform_ = transform;
	}

	void MovableObject2D::TeleportTo(const Vec2& newPosition)
	{
		Transform.Position = newPosition;
		prevTransform_.Position = newPosition;
	}

	void MovableObject2D::ShiftTo(const Vec2& newPosition)
	{
		Vec2 prevOffset = prevTransform_.Position - Transform.Position;

		Transform.Position = newPosition;
		prevTransform_.Position = newPosition + prevOffset;
	}

	const Transform2D& MovableObject2D::GetPrevTransform() const
	{
		return prevTransform_;
	}

}




