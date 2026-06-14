#include "MeshAnimatorOps.h"
#include "UVOps.h"

namespace pix
{
	
	std::vector<SpriteMeshUVKeyframe> CreateSpriteMeshUVKeyframeSequence(const UVRect& startRect, int frameCount, float durationTicks)
	{
		std::vector<SpriteMeshUVKeyframe> frameSequence;

		if (!IsInsideTexBounds(startRect) || frameCount < 1 || durationTicks <= 0.0f) return frameSequence;

		UVRect sampleRect = startRect;
		const float rectWidth = sampleRect.GetWidth();
		const float rectHeight = sampleRect.GetHeight();

		if (rectWidth <= 0.0f || rectHeight <= 0.0f) return frameSequence;

		Vec2f& minXCorner = sampleRect.TopLeft.X < sampleRect.BottomRight.X ? sampleRect.TopLeft : sampleRect.BottomRight;
		Vec2f& maxXCorner = sampleRect.TopLeft.X < sampleRect.BottomRight.X ? sampleRect.BottomRight : sampleRect.TopLeft;
		Vec2f& maxYCorner = sampleRect.TopLeft.Y < sampleRect.BottomRight.Y ? sampleRect.BottomRight : sampleRect.TopLeft;

		frameSequence.reserve(frameCount);
		frameSequence.emplace_back(GetUVQuad(sampleRect), durationTicks);

		for (int i = 1; i < frameCount; i++)
		{
			if (maxXCorner.X + rectWidth <= 1.0f) // Is there enough horizontal space for another frame in this row?
			{
				sampleRect.TopLeft.X += rectWidth;
				sampleRect.BottomRight.X += rectWidth;

				frameSequence.emplace_back(GetUVQuad(sampleRect), durationTicks);
			}
			else if (maxYCorner.Y + rectHeight <= 1.0f) // Is there enough vertical space for another frame in the next row?
			{
				minXCorner.X = 0.0f;
				maxXCorner.X = rectWidth;
				sampleRect.TopLeft.Y += rectHeight;
				sampleRect.BottomRight.Y += rectHeight;

				frameSequence.emplace_back(GetUVQuad(sampleRect), durationTicks);
			}
			else
			{
				break; // No space left for more frames
			}
		}

		return frameSequence;
	}
	
	std::vector<SpriteMeshUVKeyframe> CreateSpriteMeshUVKeyframeSequence(int texWidth, int texHeight, const SDL_Rect& startRect, int frameCount, float durationTicks, SDL_RendererFlip flip)
	{
		if (!IsInsideTexBounds(texWidth, texHeight, startRect) || startRect.w <= 0 || startRect.h <= 0 || frameCount < 1 || durationTicks <= 0.0f)
			return std::vector<SpriteMeshUVKeyframe>();

		UVRect startUVRect = GetUVRect(texWidth, texHeight, startRect);

		if (flip & SDL_FLIP_HORIZONTAL)
			FlipX(startUVRect);

		if (flip & SDL_FLIP_VERTICAL)
			FlipY(startUVRect);

		return CreateSpriteMeshUVKeyframeSequence(startUVRect, frameCount, durationTicks);
	}

	std::vector<MeshUVKeyframe> CreateMeshUVKeyframeSequence(const UVRect& startRect, int frameCount, float durationTicks, const Vec2i& subdivisionAmount)
	{
		std::vector<MeshUVKeyframe> frameSequence;

		if (!IsInsideTexBounds(startRect) || frameCount < 1 || durationTicks <= 0.0f) return frameSequence;

		UVRect sampleRect = startRect;

		const float rectWidth = sampleRect.GetWidth();
		const float rectHeight = sampleRect.GetHeight();

		if (rectWidth <= 0.0f || rectHeight <= 0.0f) return frameSequence;

		Vec2f& minXCorner = sampleRect.TopLeft.X < sampleRect.BottomRight.X ? sampleRect.TopLeft : sampleRect.BottomRight;
		Vec2f& maxXCorner = sampleRect.TopLeft.X < sampleRect.BottomRight.X ? sampleRect.BottomRight : sampleRect.TopLeft;
		Vec2f& maxYCorner = sampleRect.TopLeft.Y < sampleRect.BottomRight.Y ? sampleRect.BottomRight : sampleRect.TopLeft;

		frameSequence.reserve(frameCount);
		frameSequence.push_back(CreateMeshUVKeyframe(sampleRect, durationTicks, subdivisionAmount));

		for (int i = 1; i < frameCount; i++)
		{
			if (maxXCorner.X + rectWidth <= 1.0f)
			{
				sampleRect.TopLeft.X += rectWidth;
				sampleRect.BottomRight.X += rectWidth;

				frameSequence.push_back(CreateMeshUVKeyframe(sampleRect, durationTicks, subdivisionAmount));
			}
			else if (maxYCorner.Y + rectHeight <= 1.0f)
			{
				minXCorner.X = 0.0f;
				maxXCorner.X = rectWidth;

				sampleRect.TopLeft.Y += rectHeight;
				sampleRect.BottomRight.Y += rectHeight;

				frameSequence.push_back(CreateMeshUVKeyframe(sampleRect, durationTicks, subdivisionAmount));
			}
			else
			{
				break;
			}
		}

		return frameSequence;
	}

	std::vector<MeshUVKeyframe> CreateMeshUVKeyframeSequence(int texWidth, int texHeight, const SDL_Rect& startRect, int frameCount, float durationTicks, const Vec2i& subdivisionAmount, SDL_RendererFlip flip)
	{
		if (!IsInsideTexBounds(texWidth, texHeight, startRect) || startRect.w <= 0 || startRect.h <= 0 || frameCount < 1 || durationTicks <= 0.0f)
			return std::vector<MeshUVKeyframe>();

		UVRect startUVRect = GetUVRect(texWidth, texHeight, startRect);

		if (flip & SDL_FLIP_HORIZONTAL)
			FlipX(startUVRect);

		if (flip & SDL_FLIP_VERTICAL)
			FlipY(startUVRect);

		return CreateMeshUVKeyframeSequence(startUVRect, frameCount, durationTicks, subdivisionAmount);
	}

	void FlipX(SpriteMeshUVKeyframe& keyframe)
	{
		float tempX = keyframe.TopLeft().X;

		keyframe.TopLeft().X = keyframe.TopRight().X;
		keyframe.TopRight().X = tempX;

		tempX = keyframe.BottomRight().X;

		keyframe.BottomRight().X = keyframe.BottomLeft().X;
		keyframe.BottomLeft().X = tempX;
	}

	void FlipY(SpriteMeshUVKeyframe& keyframe)
	{
		float tempY = keyframe.TopLeft().Y;

		keyframe.TopLeft().Y = keyframe.BottomLeft().Y;
		keyframe.BottomLeft().Y = tempY;

		tempY = keyframe.TopRight().Y;

		keyframe.TopRight().Y = keyframe.BottomRight().Y;
		keyframe.BottomRight().Y = tempY;
	}

	void FlipXY(SpriteMeshUVKeyframe& keyframe)
	{
		FlipX(keyframe);
		FlipY(keyframe);
	}


	SpriteMeshKeyframe CreateSpriteMeshKeyframe(const SpriteMesh& mesh, float durationTicks)
	{
		return SpriteMeshKeyframe(mesh.Vertices[0], mesh.Vertices[1], mesh.Vertices[2], mesh.Vertices[3], durationTicks);
	}

	SpriteMeshPositionKeyframe CreateSpriteMeshPositionKeyframe(const SpriteMesh& mesh, float durationTicks)
	{
		return SpriteMeshPositionKeyframe(mesh.Vertices[0].Position, mesh.Vertices[1].Position, mesh.Vertices[2].Position, mesh.Vertices[3].Position, durationTicks);
	}

	SpriteMeshColorKeyframe CreateSpriteMeshColorKeyframe(const SpriteMesh& mesh, float durationTicks)
	{
		return SpriteMeshColorKeyframe(mesh.Vertices[0].Color, mesh.Vertices[1].Color, mesh.Vertices[2].Color, mesh.Vertices[3].Color, durationTicks);
	}

	SpriteMeshUVKeyframe CreateSpriteMeshUVKeyframe(const SpriteMesh& mesh, float durationTicks)
	{
		UVQuad uvQuad(mesh.Vertices[0].UV, mesh.Vertices[1].UV, mesh.Vertices[2].UV, mesh.Vertices[3].UV);

		return SpriteMeshUVKeyframe(uvQuad, durationTicks);
	}

	SpriteMeshUVKeyframe CreateSpriteMeshUVKeyframe(const UVRect& uvRect, float durationTicks)
	{
		return SpriteMeshUVKeyframe(GetUVQuad(uvRect), durationTicks);
	}

	SpriteMeshUVKeyframe CreateSpriteMeshUVKeyframe(const UVQuad& quad, float durationTicks)
	{
		return SpriteMeshUVKeyframe(quad, durationTicks);
	}
	

	Mesh2DKeyframe CreateMesh2DKeyframe(const Mesh2D& mesh, float durationTicks)
	{
		return Mesh2DKeyframe(mesh.Vertices, durationTicks);
	}

	Mesh2DPositionKeyframe CreateMesh2DPositionKeyframe(const Mesh2D& mesh, float durationTicks)
	{
		std::vector<Vec2f> positions;
		const int vertexCount = mesh.Vertices.size();
		positions.reserve(vertexCount);

		for (int i = 0; i < vertexCount; i++)
			positions.push_back(mesh.Vertices[i].Position);

		return Mesh2DPositionKeyframe(positions, durationTicks);
	}

	MeshColorKeyframe CreateMeshColorKeyframe(const Mesh2D& mesh, float durationTicks)
	{
		std::vector<SDL_Color> colors;
		const int vertexCount = mesh.Vertices.size();
		colors.reserve(vertexCount);

		for (int i = 0; i < vertexCount; i++)
			colors.push_back(mesh.Vertices[i].Color);

		return MeshColorKeyframe(colors, durationTicks);
	}

	MeshUVKeyframe CreateMeshUVKeyframe(const Mesh2D& mesh, float durationTicks)
	{
		std::vector<Vec2f> uvs;
		const int vertexCount = mesh.Vertices.size();
		uvs.reserve(vertexCount);

		for (int i = 0; i < vertexCount; i++)
			uvs.push_back(mesh.Vertices[i].UV);

		return MeshUVKeyframe(uvs, durationTicks);
	}

	MeshUVKeyframe CreateMeshUVKeyframe(const UVRect& uvRect, float durationTicks, const Vec2i& subdivisionAmount)
	{
		MeshUVKeyframe keyframe;
		keyframe.DurationTicks = durationTicks;

		const int cellCountX = subdivisionAmount.X < 1 ? 1 : subdivisionAmount.X;
		const int cellCountY = subdivisionAmount.Y < 1 ? 1 : subdivisionAmount.Y;

		const float cellWidth = uvRect.GetSignedWidth() / cellCountX;
		const float cellHeight = uvRect.GetSignedHeight() / cellCountY;

		keyframe.UVs.reserve(cellCountX * cellCountY * 6);

		for (int y = 0; y < cellCountY; y++)
		{
			const float cellY = y; // Convert to float for repeated use

			for (int x = 0; x < cellCountX; x++)
			{
				const float cellX = x; // Convert to float for repeated use

				const Vec2f topLeftUV(uvRect.TopLeft.X + cellX * cellWidth, uvRect.TopLeft.Y + cellY * cellHeight);
				const Vec2f bottomRightUV(uvRect.TopLeft.X + (cellX + 1.0f) * cellWidth, uvRect.TopLeft.Y + (cellY + 1.0f) * cellHeight);

				const Vec2f topRightUV(bottomRightUV.X, topLeftUV.Y);
				const Vec2f bottomLeftUV(topLeftUV.X, bottomRightUV.Y);

				keyframe.UVs.push_back(topLeftUV);
				keyframe.UVs.push_back(topRightUV);
				keyframe.UVs.push_back(bottomRightUV);

				keyframe.UVs.push_back(bottomRightUV);
				keyframe.UVs.push_back(bottomLeftUV);
				keyframe.UVs.push_back(topLeftUV);
			}
		}

		return keyframe;
	}

	MeshNormalKeyframe CreateMeshNormalKeyframe(const Mesh2D& mesh, float durationTicks)
	{
		std::vector<Vec3f> normals;
		const int vertexCount = mesh.Vertices.size();
		normals.reserve(vertexCount);

		for (int i = 0; i < vertexCount; i++)
			normals.push_back(mesh.Vertices[i].Normal);

		return MeshNormalKeyframe(normals, durationTicks);
	}


	Mesh3DKeyframe CreateMesh3DKeyframe(const Mesh3D& mesh, float durationTicks)
	{
		return Mesh3DKeyframe(mesh.Vertices, durationTicks);
	}

	Mesh3DPositionKeyframe CreateMesh3DPositionKeyframe(const Mesh3D& mesh, float durationTicks)
	{
		std::vector<Vec3f> positions;
		const int vertexCount = mesh.Vertices.size();
		positions.reserve(vertexCount);

		for (int i = 0; i < vertexCount; i++)
			positions.push_back(mesh.Vertices[i].Position);

		return Mesh3DPositionKeyframe(positions, durationTicks);
	}

	MeshColorKeyframe CreateMeshColorKeyframe(const Mesh3D& mesh, float durationTicks)
	{
		std::vector<SDL_Color> colors;
		const int vertexCount = mesh.Vertices.size();
		colors.reserve(vertexCount);

		for (int i = 0; i < vertexCount; i++)
			colors.push_back(mesh.Vertices[i].Color);

		return MeshColorKeyframe(colors, durationTicks);
	}

	MeshUVKeyframe CreateMeshUVKeyframe(const Mesh3D& mesh, float durationTicks)
	{
		std::vector<Vec2f> uvs;
		const int vertexCount = mesh.Vertices.size();
		uvs.reserve(vertexCount);

		for (int i = 0; i < vertexCount; i++)
			uvs.push_back(mesh.Vertices[i].UV);

		return MeshUVKeyframe(uvs, durationTicks);
	}

	MeshNormalKeyframe CreateMeshNormalKeyframe(const Mesh3D& mesh, float durationTicks)
	{
		std::vector<Vec3f> normals;
		const int vertexCount = mesh.Vertices.size();
		normals.reserve(vertexCount);

		for (int i = 0; i < vertexCount; i++)
			normals.push_back(mesh.Vertices[i].Normal);

		return MeshNormalKeyframe(normals, durationTicks);
	}

}