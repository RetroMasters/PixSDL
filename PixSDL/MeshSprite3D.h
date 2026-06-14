#pragma once

#include "MovableObject3D.h"
#include "Mesh2D.h"

namespace pix
{
	// MeshSprite3D is a movable 3D object that references a Mesh2D for flat geometry rendering in 3D.
	// Mesh is non-owning and may be nullptr.
	// 
	// Philosophy:
	// MeshSprite3D is a minimal renderable 3D entity that can move through space, using a general Mesh2D instead of SpriteMesh.
	// It is the non-hierarchical optimization of MeshSprite3DNode.
	class MeshSprite3D : public MovableObject3D
	{
	public:

		MeshSprite3D() = default;

		MeshSprite3D(const Mesh2D* mesh, const Transform3D& transform);
		MeshSprite3D(const Mesh2D* mesh, const Transform3D& transform, const Transform3D& prevTransform);
		explicit MeshSprite3D(const Mesh2D* mesh, const Vec3& position = Vec3(0.0, 0.0, 0.0), const Vec3f& scale = Vec3f(1.0f, 1.0f, 1.0f), const Rotation3D& rotation = Rotation3D());

		~MeshSprite3D() override = default;

		const Mesh2D* Mesh = nullptr;
	};

}
