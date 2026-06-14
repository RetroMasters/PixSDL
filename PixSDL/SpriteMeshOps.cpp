#include "SpriteMeshOps.h"

namespace pix
{
	SpriteMesh CreateSpriteMesh(const Vec2f& topLeftPosition, const Vec2f& bottomRightPosition, const UVRect& uvRect, SDL_Color color)
	{
		Vertex2D topLeft(topLeftPosition, color, uvRect.TopLeft);
		Vertex2D topRight(Vec2f(bottomRightPosition.X, topLeftPosition.Y), color, Vec2f(uvRect.BottomRight.X, uvRect.TopLeft.Y));
		Vertex2D bottomRight(bottomRightPosition, color, uvRect.BottomRight);
		Vertex2D bottomLeft(Vec2f(topLeftPosition.X, bottomRightPosition.Y), color, Vec2f(uvRect.TopLeft.X, uvRect.BottomRight.Y));

		return SpriteMesh(topLeft, topRight, bottomRight, bottomLeft);
	}

	SpriteMesh CreateSpriteMesh(const Vec2f& topLeftPosition, const Vec2f& bottomRightPosition, const UVQuad& uvQuad, SDL_Color color)
	{
		Vec2f topRightPosition(bottomRightPosition.X, topLeftPosition.Y);
		Vec2f bottomLeftPosition(topLeftPosition.X, bottomRightPosition.Y);

		return CreateSpriteMesh(topLeftPosition, topRightPosition, bottomRightPosition, bottomLeftPosition, uvQuad, color);
	}

	SpriteMesh CreateSpriteMesh(const Vec2f& topLeftPosition, const Vec2f& topRightPosition, const Vec2f& bottomRightPosition, const Vec2f& bottomLeftPosition, const UVQuad& uvQuad, SDL_Color color)
	{
		Vertex2D topLeft(topLeftPosition, color, uvQuad.TopLeft());
		Vertex2D topRight(topRightPosition, color, uvQuad.TopRight());
		Vertex2D bottomRight(bottomRightPosition, color, uvQuad.BottomRight());
		Vertex2D bottomLeft(bottomLeftPosition, color, uvQuad.BottomLeft());

		return SpriteMesh(topLeft, topRight, bottomRight, bottomLeft);
	}

	Vec2f GetMinBounds(const SpriteMesh& mesh)
	{
		const Vertex2D* const vertices = mesh.Vertices;

		Vec2f min = vertices[0].Position;

		for (int i = 1; i < SpriteMesh::VERTEX_COUNT; i++)
		{
			if (vertices[i].Position.X < min.X) min.X = vertices[i].Position.X;
			if (vertices[i].Position.Y < min.Y) min.Y = vertices[i].Position.Y;
		}

		return min;
	}

	Vec2f GetMaxBounds(const SpriteMesh& mesh)
	{
		const Vertex2D* const vertices = mesh.Vertices;

		Vec2f max = vertices[0].Position;

		for (int i = 1; i < SpriteMesh::VERTEX_COUNT; i++)
		{
			if (vertices[i].Position.X > max.X) max.X = vertices[i].Position.X;
			if (vertices[i].Position.Y > max.Y) max.Y = vertices[i].Position.Y;
		}

		return max;
	}

	Vec2f GetBoundsSize(const SpriteMesh& mesh) // Compute the size of the axis-aligned bounding box
	{
		const Vertex2D* const vertices = mesh.Vertices;

		Vec2f min = vertices[0].Position;
		Vec2f max = min;

		for (int i = 1; i < SpriteMesh::VERTEX_COUNT; i++)
		{
			// Update minimum bounds
			if (vertices[i].Position.X < min.X) min.X = vertices[i].Position.X;
			if (vertices[i].Position.Y < min.Y) min.Y = vertices[i].Position.Y;

			// Update maximum bounds
			if (vertices[i].Position.X > max.X) max.X = vertices[i].Position.X;
			if (vertices[i].Position.Y > max.Y) max.Y = vertices[i].Position.Y;
		}

		return max - min;
	}

	void SetUV(SpriteMesh& mesh, const UVRect& uvRect)
	{
		mesh.TopLeft().UV = uvRect.TopLeft;
		mesh.TopRight().UV = Vec2f(uvRect.BottomRight.X, uvRect.TopLeft.Y);
		mesh.BottomRight().UV = uvRect.BottomRight;
		mesh.BottomLeft().UV = Vec2f(uvRect.TopLeft.X, uvRect.BottomRight.Y);
	}

	void SetUV(SpriteMesh& mesh, const UVQuad& uvQuad)
	{
		mesh.TopLeft().UV = uvQuad.TopLeft();
		mesh.TopRight().UV = uvQuad.TopRight();
		mesh.BottomRight().UV = uvQuad.BottomRight();
		mesh.BottomLeft().UV = uvQuad.BottomLeft();
	}

	void SetRGBA(SpriteMesh& mesh, SDL_Color color)
	{
		for (Vertex2D& vertex : mesh.Vertices)
			vertex.Color = color;
	}

	void SetRGBA(SpriteMesh& mesh, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
	{
		SDL_Color color = { r, g, b, a };
		SetRGBA(mesh, color);
	}

	void SetRGB(SpriteMesh& mesh, Uint8 r, Uint8 g, Uint8 b)
	{
		for (Vertex2D& vertex : mesh.Vertices)
		{
			vertex.Color.r = r;
			vertex.Color.g = g;
			vertex.Color.b = b;
		}
	}

	void SetAlpha(SpriteMesh& mesh, Uint8 alpha)
	{
		for (Vertex2D& vertex : mesh.Vertices)
			vertex.Color.a = alpha;
	}

	void SetPositions(SpriteMesh& mesh, const Vec2f& topLeftPosition, const Vec2f& topRightPosition, const Vec2f& bottomRightPosition, const Vec2f& bottomLeftPosition)
	{
		mesh.TopLeft().Position = topLeftPosition;
		mesh.TopRight().Position = topRightPosition;
		mesh.BottomRight().Position = bottomRightPosition;
		mesh.BottomLeft().Position = bottomLeftPosition;
	}

	void SetPositions(SpriteMesh& mesh, const Vec2f& topLeftPosition, const Vec2f& bottomRightPosition)
	{
		Vec2f topRightPosition(bottomRightPosition.X, topLeftPosition.Y);
		Vec2f bottomLeftPosition(topLeftPosition.X, bottomRightPosition.Y);

		SetPositions(mesh, topLeftPosition, topRightPosition, bottomRightPosition, bottomLeftPosition);
	}

}