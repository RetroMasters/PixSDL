#pragma once

#include <vector>
#include <SDL_pixels.h>
#include "PixMath.h"

namespace pix
{
	// MeshVertex3D is a 3D mesh vertex storing 3D position, color, UV coordinates, and a 3D normal vector.
	//
	// Philosophy:
	// MeshVertex3D defines a point with rendering attributes used by Mesh3D.
	// The normal vector is not used by the MeshRenderer3D,
	// but can support advanced effects, such as Gouraud-style vertex lighting for
	// tessellated sprites with fake volume. The resulting lighting can be written to
	// vertex colors before rendering. The normal can also carry custom per-vertex data.
	struct MeshVertex3D
	{
		MeshVertex3D() = default;

		MeshVertex3D(const Vec3f& position, SDL_Color color, const Vec2f& uv, const Vec3f& normal) :
			Position(position),
			Color(color),
			UV(uv),
			Normal(normal)
		{}

		Vec3f Position = Vec3f(0.0f, 0.0f, 0.0f);
		SDL_Color Color = { 255, 255, 255, 255 };
		Vec2f UV = Vec2f(0.0f, 0.0f);
		Vec3f Normal = Vec3f(0.0f, 0.0f, 1.0f);
	};

	// Mesh3D stores a dynamic list of vertices for a 3D triangle-list mesh.
	// Each triangle is three consecutive MeshVertex3D entries.
	// A valid mesh must contain at least 3 vertices and have a vertex count divisible by 3.
	//
	// Philosophy:
	// Mesh3D defines a 3D model in model space.
	struct Mesh3D
	{
		Mesh3D() = default;

		explicit Mesh3D(const std::vector<MeshVertex3D>& vertices) :
			Vertices(vertices)
		{}

		bool IsValid() const
		{
			return Vertices.size() >= 3 && Vertices.size() % 3 == 0;
		}

		int GetTriangleCount() const
		{
			return Vertices.size() / 3;
		}

		std::vector<MeshVertex3D> Vertices;
	};

}
