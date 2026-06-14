#pragma once

#include "MovableObject2D.h"
#include "Mesh2D.h"

namespace pix
{
	// MeshSprite2D is a movable 2D object that references a Mesh2D for rendering.
	// The Mesh pointer is non-owning and may be nullptr.
	// 
	// Philosophy:
	// MeshSprite2D is a minimal renderable 2D entity that can move through space, using a general Mesh2D instead of SpriteMesh.
	// It is the optimized non-hierarchical counterpart to MeshSprite2DNode.
	class MeshSprite2D : public MovableObject2D
	{
	public:

		MeshSprite2D() = default;
		MeshSprite2D(const Mesh2D* mesh, const Transform2D& transform);
		MeshSprite2D(const Mesh2D* mesh, const Transform2D& transform, const Transform2D& prevTransform);
		explicit MeshSprite2D(const Mesh2D* mesh, const Vec2& position = Vec2(0.0, 0.0), const Vec2f& scale = Vec2f(1.0f, 1.0f), const Rotation2D& rotation = Rotation2D());

		~MeshSprite2D() override = default;

		const Mesh2D* Mesh = nullptr;

	};

}

