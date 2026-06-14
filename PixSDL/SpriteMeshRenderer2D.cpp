#include "SpriteMeshRenderer2D.h"
#include "Renderer.h"

namespace pix
{

	SpriteMeshRenderer2D::SpriteMeshRenderer2D(int initialVertexBatchCapacity)
	{
		if (initialVertexBatchCapacity > 0)
		{
			vertexBatch_.reserve(initialVertexBatchCapacity);
			vertexIndices_.reserve((initialVertexBatchCapacity * 6) / 4);
		}
	}

	void SpriteMeshRenderer2D::Render(const SpriteMesh& mesh, const Transform2D& transform)
	{
		// Precompute the local scale and rotation of the object
		Vec2f scaledXAxis = transform.Rotation.GetXAxis() * transform.Scale.X;
		Vec2f scaledYAxis = transform.Rotation.GetYAxis() * transform.Scale.Y;

		// Transform the scaled local axes through the combined camera rotation and zoom axes.
		// After this step, the axes convert local vertex coordinates into logical-screen-space offsets.
		scaledXAxis = config_.ScaledCameraXAxis * scaledXAxis.X + config_.ScaledCameraYAxis * scaledXAxis.Y;
		scaledYAxis = config_.ScaledCameraXAxis * scaledYAxis.X + config_.ScaledCameraYAxis * scaledYAxis.Y;

		// Transform the vector from camera to object origin to logical screen space.
		// Float precision is sufficient in camera-relative space.
		Vec2f originOffset(transform.Position - config_.CameraPosition);
		originOffset = config_.ScaledCameraXAxis * originOffset.X + config_.ScaledCameraYAxis * originOffset.Y;

		for (const Vertex2D& vertex : mesh.Vertices)
		{
			// Scale and rotate the vertex position, then translate it by the origin offset.
			// The vertex position is then in logical screen space.
			Vec2f vertexPosition = (scaledXAxis * vertex.Position.X) + (scaledYAxis * vertex.Position.Y) + originOffset;

			// Logical screen space -> render target space (Y increases downward)
			vertexPosition.X = config_.RenderTargetOffset.X + vertexPosition.X;
			vertexPosition.Y = config_.RenderTargetOffset.Y - vertexPosition.Y;

			// Add the transformed vertex to the batch
			vertexBatch_.emplace_back(vertexPosition, vertex.Color, vertex.UV);
		}
	}


	void SpriteMeshRenderer2D::Render(const Sprite2D& sprite)
	{
		if (!sprite.Mesh) return;

		// Interpolate the sprite transform
		const Transform2D interpolatedTransform = GetInterpolated(sprite.GetPrevTransform(), sprite.Transform, config_.InterpolationAlpha);

		// Precompute the local scale and rotation of the object
		Vec2f scaledXAxis = interpolatedTransform.Rotation.GetXAxis() * interpolatedTransform.Scale.X;
		Vec2f scaledYAxis = interpolatedTransform.Rotation.GetYAxis() * interpolatedTransform.Scale.Y;

		// Transform the scaled local axes through the combined camera rotation and zoom axes.
		// After this step, the axes convert local vertex coordinates into logical-screen-space offsets.
		scaledXAxis = config_.ScaledCameraXAxis * scaledXAxis.X + config_.ScaledCameraYAxis * scaledXAxis.Y;
		scaledYAxis = config_.ScaledCameraXAxis * scaledYAxis.X + config_.ScaledCameraYAxis * scaledYAxis.Y;

		// Transform the vector from camera to object origin to logical screen space.
		// Float precision is sufficient in camera-relative space.
		Vec2f originOffset(interpolatedTransform.Position - config_.CameraPosition);
		originOffset = config_.ScaledCameraXAxis * originOffset.X + config_.ScaledCameraYAxis * originOffset.Y;

		for (const Vertex2D& vertex : sprite.Mesh->Vertices)
		{
			// Scale and rotate the vertex position, then translate it by the origin offset.
			// The vertex position is then in logical screen space.
			Vec2f vertexPosition = (scaledXAxis * vertex.Position.X) + (scaledYAxis * vertex.Position.Y) + originOffset;

			// Logical screen space -> render target space (Y increases downward)
			vertexPosition.X = config_.RenderTargetOffset.X + vertexPosition.X;
			vertexPosition.Y = config_.RenderTargetOffset.Y - vertexPosition.Y;

			// Add the transformed vertex to the batch
			vertexBatch_.emplace_back(vertexPosition, vertex.Color, vertex.UV);
		}
	}


	void SpriteMeshRenderer2D::Render(const Sprite2DNode& node)
	{
		if (!node.Mesh) return;

		const Vertex2D* const vertices = node.Mesh->Vertices;
		const Sprite2DNode* parent = &node;
		const double interpolationAlpha = (double)config_.InterpolationAlpha; // Convert to double for repeated use

		// Cache initial vertex positions
		Vec2 worldPositionBuffer[4] =
		{
			Vec2(vertices[0].Position),
			Vec2(vertices[1].Position),
			Vec2(vertices[2].Position),
			Vec2(vertices[3].Position)
		};

		Vec2 prevWorldPositionBuffer[4] =
		{
			worldPositionBuffer[0],
			worldPositionBuffer[1],
			worldPositionBuffer[2],
			worldPositionBuffer[3]
		};

		// Transform current and previous vertex positions to world space
		while (parent)
		{
			parent->Transform.TransformPoints(worldPositionBuffer, 4);
			parent->GetPrevTransform().TransformPoints(prevWorldPositionBuffer, 4);
			parent = parent->GetParent();
		}

		for (int i = 0; i < 4; i++)
		{
			// Interpolate the world-space vertex position
			const Vec2 worldPosition = GetInterpolatedUnchecked(prevWorldPositionBuffer[i], worldPositionBuffer[i], interpolationAlpha);

			// Start with the world-space vector from camera to vertex position.
			// Float precision is sufficient in camera-relative space.
			Vec2f vertexPosition(worldPosition - config_.CameraPosition);

			// Transform the camera-relative position to logical screen space
			vertexPosition = config_.ScaledCameraXAxis * vertexPosition.X + config_.ScaledCameraYAxis * vertexPosition.Y;

			// Logical screen space -> render target space (Y increases downward)
			vertexPosition.X = config_.RenderTargetOffset.X + vertexPosition.X;
			vertexPosition.Y = config_.RenderTargetOffset.Y - vertexPosition.Y;

			// Add the transformed vertex to the batch
			vertexBatch_.emplace_back(vertexPosition, vertices[i].Color, vertices[i].UV);
		}
	}

	void SpriteMeshRenderer2D::RenderPoint(const SpriteMesh& mesh, const Vec2& point, float pointSize)
	{
		const Vertex2D* const vertices = mesh.Vertices;

		// Start with the world-space vector from camera to the point.
		// Float precision is sufficient in camera-relative space.
		Vec2f transformedPoint(point - config_.CameraPosition);

		// Transform the camera-relative point to logical screen space
		transformedPoint = config_.ScaledCameraXAxis * transformedPoint.X + config_.ScaledCameraYAxis * transformedPoint.Y;

		// Logical screen space -> render target space (Y increases downward)
		transformedPoint.X = config_.RenderTargetOffset.X + transformedPoint.X;
		transformedPoint.Y = config_.RenderTargetOffset.Y - transformedPoint.Y;

		const float halfSize = pointSize * 0.5f; // Half side length of the generated quad

		// Top-left corner
		Vec2f quadPoint(transformedPoint.X - halfSize, transformedPoint.Y - halfSize);
		vertexBatch_.emplace_back(quadPoint, vertices[0].Color, vertices[0].UV);

		// Top-right corner
		quadPoint = Vec2f(transformedPoint.X + halfSize, transformedPoint.Y - halfSize);
		vertexBatch_.emplace_back(quadPoint, vertices[1].Color, vertices[1].UV);

		// Bottom-right corner
		quadPoint = Vec2f(transformedPoint.X + halfSize, transformedPoint.Y + halfSize);
		vertexBatch_.emplace_back(quadPoint, vertices[2].Color, vertices[2].UV);

		// Bottom-left corner
		quadPoint = Vec2f(transformedPoint.X - halfSize, transformedPoint.Y + halfSize);
		vertexBatch_.emplace_back(quadPoint, vertices[3].Color, vertices[3].UV);
	}

	void SpriteMeshRenderer2D::RenderLine(const SpriteMesh& mesh, const Vec2& startPoint, const Vec2& endPoint, float lineWidth)
	{
		const Vertex2D* const vertices = mesh.Vertices;

		// Start with the vectors from camera to the line points.
		// Float precision is sufficient in camera-relative space.
		Vec2f transformedStartPoint(startPoint - config_.CameraPosition);
		Vec2f transformedEndPoint(endPoint - config_.CameraPosition);

		// Transform the camera-relative points to logical screen space
		transformedStartPoint = config_.ScaledCameraXAxis * transformedStartPoint.X + config_.ScaledCameraYAxis * transformedStartPoint.Y;
		transformedEndPoint = config_.ScaledCameraXAxis * transformedEndPoint.X + config_.ScaledCameraYAxis * transformedEndPoint.Y;

		// Logical screen space -> render target space (Y increases downward)
		transformedStartPoint.X = config_.RenderTargetOffset.X + transformedStartPoint.X;
		transformedStartPoint.Y = config_.RenderTargetOffset.Y - transformedStartPoint.Y;
		transformedEndPoint.X = config_.RenderTargetOffset.X + transformedEndPoint.X;
		transformedEndPoint.Y = config_.RenderTargetOffset.Y - transformedEndPoint.Y;

		const Vec2f halfWidthNormal = ((transformedStartPoint - transformedEndPoint).Normalize() * (lineWidth * 0.5f)).GetNormal();

		// Add quad points in clockwise order on the screen around the centered line segment
		Vec2f quadPoint = transformedStartPoint + halfWidthNormal;
		vertexBatch_.emplace_back(quadPoint, vertices[0].Color, vertices[0].UV);

		quadPoint = transformedEndPoint + halfWidthNormal;
		vertexBatch_.emplace_back(quadPoint, vertices[1].Color, vertices[1].UV);

		quadPoint = transformedEndPoint - halfWidthNormal;
		vertexBatch_.emplace_back(quadPoint, vertices[2].Color, vertices[2].UV);

		quadPoint = transformedStartPoint - halfWidthNormal;
		vertexBatch_.emplace_back(quadPoint, vertices[3].Color, vertices[3].UV);
	}


	void SpriteMeshRenderer2D::RenderPixel(const SpriteMesh& mesh, const Vec2f& position, float pixelSize)
	{
		const Vertex2D* const vertices = mesh.Vertices;

		const float halfSize = pixelSize * 0.5f; // Half side length of the generated quad

		// Top-left corner
		Vec2f quadPoint(position.X - halfSize, position.Y - halfSize);
		vertexBatch_.emplace_back(quadPoint, vertices[0].Color, vertices[0].UV);

		// Top-right corner
		quadPoint = Vec2f(position.X + halfSize, position.Y - halfSize);
		vertexBatch_.emplace_back(quadPoint, vertices[1].Color, vertices[1].UV);

		// Bottom-right corner
		quadPoint = Vec2f(position.X + halfSize, position.Y + halfSize);
		vertexBatch_.emplace_back(quadPoint, vertices[2].Color, vertices[2].UV);

		// Bottom-left corner
		quadPoint = Vec2f(position.X - halfSize, position.Y + halfSize);
		vertexBatch_.emplace_back(quadPoint, vertices[3].Color, vertices[3].UV);
	}


	void SpriteMeshRenderer2D::RenderPixelLine(const SpriteMesh& mesh, const Vec2f& startPosition, const Vec2f& endPosition, float lineWidth)
	{
		const Vertex2D* const vertices = mesh.Vertices;

		const Vec2f halfWidthNormal = ((startPosition - endPosition).Normalize() * (lineWidth * 0.5f)).GetNormal();

		// Add quad points in clockwise order on the render target around the centered line segment
		Vec2f quadPoint = startPosition + halfWidthNormal;
		vertexBatch_.emplace_back(quadPoint, vertices[0].Color, vertices[0].UV);

		quadPoint = endPosition + halfWidthNormal;
		vertexBatch_.emplace_back(quadPoint, vertices[1].Color, vertices[1].UV);

		quadPoint = endPosition - halfWidthNormal;
		vertexBatch_.emplace_back(quadPoint, vertices[2].Color, vertices[2].UV);

		quadPoint = startPosition - halfWidthNormal;
		vertexBatch_.emplace_back(quadPoint, vertices[3].Color, vertices[3].UV);
	}

	void SpriteMeshRenderer2D::RenderHorizontalPixelLine(const SpriteMesh& mesh, const Vec2f& startPosition, float length, float lineWidth)
	{
		const Vertex2D* const vertices = mesh.Vertices;

		const float halfWidth = lineWidth * 0.5f;

		// Add quad points in clockwise order on the render target around the centered line segment
		Vec2f quadPoint(startPosition.X, startPosition.Y - halfWidth);
		vertexBatch_.emplace_back(quadPoint, vertices[0].Color, vertices[0].UV);

		quadPoint = Vec2f(startPosition.X + length, startPosition.Y - halfWidth);
		vertexBatch_.emplace_back(quadPoint, vertices[1].Color, vertices[1].UV);

		quadPoint = Vec2f(startPosition.X + length, startPosition.Y + halfWidth);
		vertexBatch_.emplace_back(quadPoint, vertices[2].Color, vertices[2].UV);

		quadPoint = Vec2f(startPosition.X, startPosition.Y + halfWidth);
		vertexBatch_.emplace_back(quadPoint, vertices[3].Color, vertices[3].UV);
	}

	void SpriteMeshRenderer2D::RenderVerticalPixelLine(const SpriteMesh& mesh, const Vec2f& startPosition, float length, float lineWidth)
	{
		const Vertex2D* const vertices = mesh.Vertices;

		const float halfWidth = lineWidth * 0.5f;

		// Add quad points in clockwise order on the render target around the centered line segment
		Vec2f quadPoint(startPosition.X - halfWidth, startPosition.Y);
		vertexBatch_.emplace_back(quadPoint, vertices[0].Color, vertices[0].UV);

		quadPoint = Vec2f(startPosition.X + halfWidth, startPosition.Y);
		vertexBatch_.emplace_back(quadPoint, vertices[1].Color, vertices[1].UV);

		quadPoint = Vec2f(startPosition.X + halfWidth, startPosition.Y + length);
		vertexBatch_.emplace_back(quadPoint, vertices[2].Color, vertices[2].UV);

		quadPoint = Vec2f(startPosition.X - halfWidth, startPosition.Y + length);
		vertexBatch_.emplace_back(quadPoint, vertices[3].Color, vertices[3].UV);
	}

	void SpriteMeshRenderer2D::BeginBatch(const MovableObject2D& camera, const Vec2f& renderTargetOffset, float interpolationAlpha)
	{
		vertexBatch_.clear();

		interpolationAlpha = GetClamped(interpolationAlpha, 0.0f, 1.0f);

		config_.InterpolationAlpha = interpolationAlpha;
		config_.RenderTargetOffset = renderTargetOffset;
		config_.CameraPosition = GetInterpolatedUnchecked(camera.GetPrevTransform().Position, camera.Transform.Position, (double)interpolationAlpha);

		const Vec2f interpolatedCameraZoom = GetInterpolatedUnchecked(camera.GetPrevTransform().Scale, camera.Transform.Scale, interpolationAlpha);
		const Rotation2D interpolatedInverseCameraRotation = GetInterpolated(camera.GetPrevTransform().Rotation, camera.Transform.Rotation, interpolationAlpha).Inverse();
		config_.ScaledCameraXAxis = interpolatedInverseCameraRotation.GetXAxis() * interpolatedCameraZoom;
		config_.ScaledCameraYAxis = interpolatedInverseCameraRotation.GetYAxis() * interpolatedCameraZoom;
	}

	void SpriteMeshRenderer2D::RenderBatch(const Texture& texture, TargetTexture* renderTarget)
	{
		const int verticesPerSprite = 4;
		const int indicesPerSprite = 6;

		const int spriteCount = vertexBatch_.size() / verticesPerSprite;

		if (spriteCount <= 0) return;

		UpdateVertexIndices();

		const int indexCount = spriteCount * indicesPerSprite;
		const Vertex2D* const vertexArray = vertexBatch_.data();

		Renderer& renderer = Renderer::Get();

		renderer.SetRenderTarget(renderTarget);

		const Vec2f cachedRenderScale = renderer.GetRenderScale(); // Cache current render scale

		if (renderTarget)
		{
			Vec2i size = renderTarget->GetSize();

			renderer.SetRenderScale((float)size.X / renderer.GetLogicalResolutionWidth(), (float)size.Y / renderer.GetLogicalResolutionHeight());
		}

		constexpr int stride = sizeof(Vertex2D);

		renderer.RenderGeometryRaw(
			texture,
			&(vertexArray->Position.X), stride,
			&(vertexArray->Color), stride,
			&(vertexArray->UV.X), stride,
			vertexBatch_.size(),
			vertexIndices_.data(), indexCount, 4);

		renderer.SetRenderScale(cachedRenderScale.X, cachedRenderScale.Y);
	}

	// Triangles are emitted in clockwise order on the screen.
	// First triangle:  0-1-2
	// Second triangle: 2-3-0
	//
	// Vertex layout for a non-rotated SpriteMesh:
	// 0--1
	// |\ |
	// | \|
	// 3--2
	void SpriteMeshRenderer2D::UpdateVertexIndices()
	{
		const int verticesPerSprite = 4;
		const int indicesPerSprite = 6;

		const int vertexCount = vertexBatch_.size();
		const int indexCount = vertexIndices_.size();

		const int spriteCount = vertexCount / verticesPerSprite;
		const int targetIndexCount = spriteCount * indicesPerSprite;

		if (indexCount >= targetIndexCount) return;

		// Only complete sprites can receive indices.
		// In normal use, vertexCount should already be a multiple of verticesPerSprite.
		const int completeVertexCount = spriteCount * verticesPerSprite;

		const int startVertexIndex = (indexCount / indicesPerSprite) * verticesPerSprite;

		for (int i = startVertexIndex; i < completeVertexCount; i += verticesPerSprite)
		{
			vertexIndices_.push_back(i);
			vertexIndices_.push_back(i + 1);
			vertexIndices_.push_back(i + 2);

			vertexIndices_.push_back(i + 2);
			vertexIndices_.push_back(i + 3);
			vertexIndices_.push_back(i);
		}
	}

}