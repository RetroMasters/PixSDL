#include "MeshSprite2D.h"

namespace pix
{

	MeshSprite2D::MeshSprite2D(const Mesh2D* mesh, const Transform2D& transform) : MovableObject2D(transform),
		Mesh(mesh)
	{}

	MeshSprite2D::MeshSprite2D(const Mesh2D* mesh, const Transform2D& transform, const Transform2D& prevTransform) : MovableObject2D(transform, prevTransform),
		Mesh(mesh)
	{}

	MeshSprite2D::MeshSprite2D(const Mesh2D* mesh, const Vec2& position, const Vec2f& scale, const Rotation2D& rotation) : MovableObject2D(position, scale, rotation),
		Mesh(mesh)
	{}

}





