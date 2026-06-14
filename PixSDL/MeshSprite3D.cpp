#include "MeshSprite3D.h"

namespace pix
{
	MeshSprite3D::MeshSprite3D(const Mesh2D* mesh, const Transform3D& transform) : MovableObject3D(transform),
		Mesh(mesh)
	{}

	MeshSprite3D::MeshSprite3D(const Mesh2D* mesh, const Transform3D& transform, const Transform3D& prevTransform) : MovableObject3D(transform, prevTransform),
		Mesh(mesh)
	{}

	MeshSprite3D::MeshSprite3D(const Mesh2D* mesh, const Vec3& position, const Vec3f& scale, const Rotation3D& rotation) : MovableObject3D(position, scale, rotation),
		Mesh(mesh)
	{}
}
