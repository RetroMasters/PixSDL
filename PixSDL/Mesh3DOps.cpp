#include"Mesh3DOps.h"
#include <vector>

namespace pix
{
	Mesh3D CreateGridMesh3D(const Vec2f& topLeftPosition, const Vec2f& bottomRightPosition, const UVRect& uvRect, const Vec2i& subdivisionAmount, SDL_Color color, const Vec3f& normal)
	{
		const int cellCountX = subdivisionAmount.X < 1 ? 1 : subdivisionAmount.X;
		const int cellCountY = subdivisionAmount.Y < 1 ? 1 : subdivisionAmount.Y;

		const float cellWidth = (bottomRightPosition.X - topLeftPosition.X) / cellCountX;
		const float cellHeight = (topLeftPosition.Y - bottomRightPosition.Y) / cellCountY;
		const float uvWidth = uvRect.GetSignedWidth() / cellCountX;
		const float uvHeight = uvRect.GetSignedHeight() / cellCountY;

		std::vector<MeshVertex3D> vertices;
		vertices.reserve(cellCountX * cellCountY * 6);

		for (int y = 0; y < cellCountY; y++)
		{
			const float cellY = y; // Convert to float for repeated use

			for (int x = 0; x < cellCountX; x++)
			{
				const float cellX = x; // Convert to float for repeated use

				const Vec3f topLeftPos(topLeftPosition.X + cellX * cellWidth, topLeftPosition.Y - cellY * cellHeight, 0.0f);
				// bottomRightPos is computed from cell indices so overlapping vertices have perfect bitwise equality
				const Vec3f bottomRightPos(topLeftPosition.X + (cellX + 1.0f) * cellWidth, topLeftPosition.Y - (cellY + 1.0f) * cellHeight, 0.0f);
				const Vec3f topRightPos(bottomRightPos.X, topLeftPos.Y, 0.0f);
				const Vec3f bottomLeftPos(topLeftPos.X, bottomRightPos.Y, 0.0f);

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

		return Mesh3D(vertices);
	}

	Mesh3D CreateQuadMesh3D(const Vec2f& topLeftPosition, const Vec2f& bottomRightPosition, const UVQuad& uvQuad, SDL_Color color, const Vec3f& normal)
	{
		Vec2f topRightPosition(bottomRightPosition.X, topLeftPosition.Y);
		Vec2f bottomLeftPosition(topLeftPosition.X, bottomRightPosition.Y);

		return CreateQuadMesh3D(topLeftPosition, topRightPosition, bottomRightPosition, bottomLeftPosition, uvQuad, color, normal);
	}

	Mesh3D CreateQuadMesh3D(const Vec2f& topLeftPosition, const Vec2f& topRightPosition, const Vec2f& bottomRightPosition, const Vec2f& bottomLeftPosition, const UVQuad& uvQuad, SDL_Color color, const Vec3f& normal)
	{
		const MeshVertex3D topLeft(Vec3f(topLeftPosition.X, topLeftPosition.Y, 0.0f), color, uvQuad.TopLeft(), normal);
		const MeshVertex3D topRight(Vec3f(topRightPosition.X, topRightPosition.Y, 0.0f), color, uvQuad.TopRight(), normal);
		const MeshVertex3D bottomRight(Vec3f(bottomRightPosition.X, bottomRightPosition.Y, 0.0f), color, uvQuad.BottomRight(), normal);
		const MeshVertex3D bottomLeft(Vec3f(bottomLeftPosition.X, bottomLeftPosition.Y, 0.0f), color, uvQuad.BottomLeft(), normal);

		std::vector<MeshVertex3D> vertices;
		vertices.reserve(6);

		// First triangle
		vertices.push_back(topLeft);
		vertices.push_back(topRight);
		vertices.push_back(bottomRight);

		// Second triangle
		vertices.push_back(bottomRight);
		vertices.push_back(bottomLeft);
		vertices.push_back(topLeft);

		return Mesh3D(vertices);
	}

	Vec3f GetMinBounds(const Mesh3D& mesh)
	{
		const std::vector<MeshVertex3D>& vertices = mesh.Vertices;
		const int vertexCount = vertices.size();

		if (vertexCount <= 0) return Vec3f(0.0f, 0.0f, 0.0f);

		Vec3f min = vertices[0].Position;

		for (int i = 1; i < vertexCount; i++)
		{
			// Update min
			if (vertices[i].Position.X < min.X) min.X = vertices[i].Position.X;
			if (vertices[i].Position.Y < min.Y) min.Y = vertices[i].Position.Y;
			if (vertices[i].Position.Z < min.Z) min.Z = vertices[i].Position.Z;
		}

		return min;
	}

	Vec3f GetMaxBounds(const Mesh3D& mesh)
	{
		const std::vector<MeshVertex3D>& vertices = mesh.Vertices;
		const int vertexCount = vertices.size();

		if (vertexCount <= 0) return Vec3f(0.0f, 0.0f, 0.0f);

		Vec3f max = vertices[0].Position;

		for (int i = 1; i < vertexCount; i++)
		{
			// Update max
			if (vertices[i].Position.X > max.X) max.X = vertices[i].Position.X;
			if (vertices[i].Position.Y > max.Y) max.Y = vertices[i].Position.Y;
			if (vertices[i].Position.Z > max.Z) max.Z = vertices[i].Position.Z;
		}

		return max;
	}

	Vec3f GetBoundsSize(const Mesh3D& mesh)
	{
		const std::vector<MeshVertex3D>& vertices = mesh.Vertices;
		const int vertexCount = vertices.size();

		if (vertexCount <= 0) return Vec3f(0.0f, 0.0f, 0.0f);

		Vec3f min = vertices[0].Position;
		Vec3f max = min;

		for (int i = 1; i < vertexCount; i++)
		{
			// Update minimum bounds
			if (vertices[i].Position.X < min.X) min.X = vertices[i].Position.X;
			if (vertices[i].Position.Y < min.Y) min.Y = vertices[i].Position.Y;
			if (vertices[i].Position.Z < min.Z) min.Z = vertices[i].Position.Z;

			// Update maximum bounds
			if (vertices[i].Position.X > max.X) max.X = vertices[i].Position.X;
			if (vertices[i].Position.Y > max.Y) max.Y = vertices[i].Position.Y;
			if (vertices[i].Position.Z > max.Z) max.Z = vertices[i].Position.Z;
		}

		return max - min;
	}

	void SetRGBA(Mesh3D& mesh, SDL_Color color)
	{
		for (MeshVertex3D& vertex : mesh.Vertices)
			vertex.Color = color;
	}

	void SetRGBA(Mesh3D& mesh, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
	{
		SDL_Color color = { r, g, b, a };
		SetRGBA(mesh, color);
	}

	void SetRGB(Mesh3D& mesh, Uint8 r, Uint8 g, Uint8 b)
	{
		for (MeshVertex3D& vertex : mesh.Vertices)
		{
			vertex.Color.r = r;
			vertex.Color.g = g;
			vertex.Color.b = b;
		}
	}

	void SetAlpha(Mesh3D& mesh, Uint8 alpha)
	{
		for (MeshVertex3D& vertex : mesh.Vertices)
			vertex.Color.a = alpha;
	}

	void SetNormal(Mesh3D& mesh, const Vec3f& normal)
	{
		for (MeshVertex3D& vertex : mesh.Vertices)
			vertex.Normal = normal;
	}

}