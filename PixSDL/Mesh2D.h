#pragma once

#include <SDL_pixels.h>
#include <vector>
#include "PixMath.h"

namespace pix
{
	// MeshVertex2D is a 2D mesh vertex storing 2D position, color, UV coordinates, and a 3D normal vector.
	//
	// Philosophy:
	// MeshVertex2D defines a point with rendering attributes used by Mesh2D.
	// The normal vector is not used by MeshRenderer2D/MeshRenderer3D,
	// but can support advanced effects, such as Gouraud-style vertex lighting for
	// tessellated sprites with fake volume. The resulting lighting can be written to
	// vertex colors before rendering. The normal can also carry custom per-vertex data.
	struct MeshVertex2D
	{
		MeshVertex2D() = default;

		MeshVertex2D(const Vec2f& position, SDL_Color color, const Vec2f& uv, const Vec3f& normal) :
			Position(position),
			Color(color),
			UV(uv),
			Normal(normal)
		{}

		Vec2f Position = Vec2f(0.0f, 0.0f);
		SDL_Color Color = { 255, 255, 255, 255 };
		Vec2f UV = Vec2f(0.0f, 0.0f);
		Vec3f Normal = Vec3f(0.0f, 0.0f, 1.0f);
	};


	// Mesh2D stores a dynamic list of vertices for a 2D triangle-list mesh.
	// Each triangle is three consecutive MeshVertex2D entries.
	// A valid mesh must contain at least 3 vertices and have a vertex count divisible by 3.
	//
	// Philosophy:
	// Mesh2D defines a 2D model in model space.
	struct Mesh2D
	{
		Mesh2D() = default;

		explicit Mesh2D(const std::vector<MeshVertex2D>& vertices) :
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

		std::vector<MeshVertex2D> Vertices;
	};

}
