#pragma once

#include "PixMath.h"

namespace pix
{
	// MovableObject2D is the base class for a spatial 2D object. It represents only a transform: position, scale, rotation.
	// 
	// Philosophy:
	// MovableObject2D is the foundation for 2D objects that move through space.
	// It provides native support for visual motion interpolation by maintaining the previous transform state.
	class MovableObject2D
	{
	public:

		MovableObject2D() = default;
		explicit MovableObject2D(const Transform2D& transform);
		MovableObject2D(const Transform2D& transform, const Transform2D& prevTransform);
		explicit MovableObject2D(const Vec2& position, const Vec2f& scale = Vec2f(1.0f, 1.0f), const Rotation2D& rotation = Rotation2D());

		virtual ~MovableObject2D() = default;

		// Syncs previous transform state with the current one.
		// Call once per update tick before modifying Transform. 
		// This snapshots the current transform into prevTransform_ for interpolation.
		void BeginUpdate();

		// Sets both current and previous transform state to transform.
		// Use this for discontinuous movement such as teleporting, spawning, respawning,
		// or camera jumps to prevent visual interpolation from the old transform.
		void TeleportTo(const Transform2D& transform);

		// Sets both current and previous position to newPosition while scale and rotation stay unchanged.
		// Use this for discontinuous position changes to prevent visual interpolation from the old position.
		void TeleportTo(const Vec2& newPosition);

		// Shifts both the current and previous position by the same amount.
		// The current position is moved to newPosition while scale and rotation stay unchanged.
		// Use this to preserve continuous movement when the object is displaced or mirrored to another location.
		void ShiftTo(const Vec2& newPosition);



		const Transform2D& GetPrevTransform() const;

		Transform2D Transform;

	protected:

		Transform2D prevTransform_;
	};

}

