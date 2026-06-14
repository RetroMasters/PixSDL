#include "Mesh2DOps.h"
#include <vector>

namespace pix
{
	Mesh2D CreateGridMesh2D(const Vec2f& topLeftPosition, const Vec2f& bottomRightPosition, const UVRect& uvRect, const Vec2i& subdivisionAmount, SDL_Color color, const Vec3f& normal)
	{
		const int cellCountX = subdivisionAmount.X < 1 ? 1 : subdivisionAmount.X;
		const int cellCountY = subdivisionAmount.Y < 1 ? 1 : subdivisionAmount.Y;

		const float cellWidth = (bottomRightPosition.X - topLeftPosition.X) / cellCountX;
		const float cellHeight = (topLeftPosition.Y - bottomRightPosition.Y) / cellCountY;
		const float uvWidth = uvRect.GetSignedWidth() / cellCountX;
		const float uvHeight = uvRect.GetSignedHeight() / cellCountY;

		std::vector<MeshVertex2D> vertices;
		vertices.reserve(cellCountX * cellCountY * 6);

		for (int y = 0; y < cellCountY; y++)
		{
			const float cellY = y; // Convert to float for repeated use

			for (int x = 0; x < cellCountX; x++)
			{
				const float cellX = x; // Convert to float for repeated use

				const Vec2f topLeftPos(topLeftPosition.X + cellX * cellWidth, topLeftPosition.Y - cellY * cellHeight);
				// bottomRightPos is computed from cell indices so overlapping vertices have perfect bitwise equality
				const Vec2f bottomRightPos(topLeftPosition.X + (cellX + 1.0f) * cellWidth, topLeftPosition.Y - (cellY + 1.0f) * cellHeight);
				const Vec2f topRightPos(bottomRightPos.X, topLeftPos.Y);
				const Vec2f bottomLeftPos(topLeftPos.X, bottomRightPos.Y);

				const Vec2f topLeftUV(uvRect.TopLeft.X + cellX * uvWidth, uvRect.TopLeft.Y + cellY * uvHeight);
				const Vec2f bottomRightUV(uvRect.TopLeft.X + (cellX + 1.0f) * uvWidth, uvRect.TopLeft.Y + (cellY + 1.0f) * uvHeight);
				const Vec2f topRightUV(bottomRightUV.X, topLeftUV.Y);
				const Vec2f bottomLeftUV(topLeftUV.X, bottomRightUV.Y);

				// First triangle
				vertices.emplace_back(topLeftPos, color, topLeftUV, normal);
				vertices.emplace_back(topRightPos, color, topRightUV, normal);
				vertices.emplace_back(bottomRightPos, color, bottomRightUV, normal);

				// Second triangle
				vertices.emplace_back(bottomRightPos, color, bottomRightUV, normal);
				vertices.emplace_back(bottomLeftPos, color, bottomLeftUV, normal);
				vertices.emplace_back(topLeftPos, color, topLeftUV, normal);
			}
		}

		return Mesh2D(vertices);
	}


	Mesh2D CreateQuadMesh2D(const Vec2f& topLeftPosition, const Vec2f& bottomRightPosition, const UVQuad& uvQuad, SDL_Color color, const Vec3f& normal)
	{
		const Vec2f topRightPosition(bottomRightPosition.X, topLeftPosition.Y);
		const Vec2f bottomLeftPosition(topLeftPosition.X, bottomRightPosition.Y);

		return CreateQuadMesh2D(topLeftPosition, topRightPosition, bottomRightPosition, bottomLeftPosition, uvQuad, color, normal);
	}

	Mesh2D CreateQuadMesh2D(const Vec2f& topLeftPosition, const Vec2f& topRightPosition, const Vec2f& bottomRightPosition, const Vec2f& bottomLeftPosition, const UVQuad& uvQuad, SDL_Color color, const Vec3f& normal)
	{
		const MeshVertex2D topLeft(topLeftPosition, color, uvQuad.TopLeft(), normal);
		const MeshVertex2D topRight(topRightPosition, color, uvQuad.TopRight(), normal);
		const MeshVertex2D bottomRight(bottomRightPosition, color, uvQuad.BottomRight(), normal);
		const MeshVertex2D bottomLeft(bottomLeftPosition, color, uvQuad.BottomLeft(), normal);

		std::vector<MeshVertex2D> vertices;
		vertices.reserve(6);

		// First triangle: 0-1-2
		vertices.push_back(topLeft);
		vertices.push_back(topRight);
		vertices.push_back(bottomRight);

		// Second triangle: 2-3-0
		vertices.push_back(bottomRight);
		vertices.push_back(bottomLeft);
		vertices.push_back(topLeft);

		return Mesh2D(vertices);
	}

	Vec2f GetMinBounds(const Mesh2D& mesh)
	{
		const std::vector<MeshVertex2D>& vertices = mesh.Vertices;
		const int vertexCount = vertices.size();

		if (vertexCount <= 0) return Vec2f(0.0f, 0.0f);

		Vec2f min = vertices[0].Position;

		for (int i = 1; i < vertexCount; i++)
		{
			// Update min
			if (vertices[i].Position.X < min.X) min.X = vertices[i].Position.X;
			if (vertices[i].Position.Y < min.Y) min.Y = vertices[i].Position.Y;
		}

		return min;
	}

	Vec2f GetMaxBounds(const Mesh2D& mesh)
	{
		const std::vector<MeshVertex2D>& vertices = mesh.Vertices;
		const int vertexCount = vertices.size();

		if (vertexCount <= 0) return Vec2f(0.0f, 0.0f);

		Vec2f max = vertices[0].Position;

		for (int i = 1; i < vertexCount; i++)
		{
			// Update max
			if (vertices[i].Position.X > max.X) max.X = vertices[i].Position.X;
			if (vertices[i].Position.Y > max.Y) max.Y = vertices[i].Position.Y;
		}

		return max;
	}


	Vec2f GetBoundsSize(const Mesh2D& mesh)
	{
		const std::vector<MeshVertex2D>& vertices = mesh.Vertices;
		const int vertexCount = vertices.size();

		if (vertexCount <= 0) return Vec2f(0.0f, 0.0f);

		Vec2f min = vertices[0].Position;
		Vec2f max = min;

		for (int i = 1; i < vertexCount; i++)
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

	void SetRGBA(Mesh2D& mesh, SDL_Color color)
	{
		for (MeshVertex2D& vertex : mesh.Vertices)
			vertex.Color = color;
	}

	void SetRGBA(Mesh2D& mesh, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
	{
		SDL_Color color = { r, g, b, a };
		SetRGBA(mesh, color);
	}

	void SetRGB(Mesh2D& mesh, Uint8 r, Uint8 g, Uint8 b)
	{
		for (MeshVertex2D& vertex : mesh.Vertices)
		{
			vertex.Color.r = r;
			vertex.Color.g = g;
			vertex.Color.b = b;
		}
	}

	void SetAlpha(Mesh2D& mesh, Uint8 alpha)
	{
		for (MeshVertex2D& vertex : mesh.Vertices)
			vertex.Color.a = alpha;
	}

	void SetNormal(Mesh2D& mesh, const Vec3f& normal)
	{
		for (MeshVertex2D& vertex : mesh.Vertices)
			vertex.Normal = normal;
	}


}