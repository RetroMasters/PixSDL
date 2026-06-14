#include "SpriteMeshRenderer3D.h"
#include "Renderer.h"

namespace pix
{

	SpriteMeshRenderer3D::SpriteMeshRenderer3D(int initialVertexBatchCapacity)
	{
		if (initialVertexBatchCapacity > 0)
		{
			vertexBatch_.reserve(initialVertexBatchCapacity);
			vertexIndices_.reserve((initialVertexBatchCapacity * 6) / 4);
		}
	}


	void SpriteMeshRenderer3D::Render(const SpriteMesh& mesh, const Transform3D& transform)
	{
		const Vertex2D* const vertices = mesh.Vertices;

		// Precompute the local scale and rotation of the flat object
		Vec3f scaledXAxis = transform.Rotation.GetXAxis() * transform.Scale.X;
		Vec3f scaledYAxis = transform.Rotation.GetYAxis() * transform.Scale.Y;

		// Transform the scaled local axes to camera space
		scaledXAxis = Vec3f(config_.CameraXAxis.GetDotProduct(scaledXAxis), config_.CameraYAxis.GetDotProduct(scaledXAxis), config_.CameraZAxis.GetDotProduct(scaledXAxis));
		scaledYAxis = Vec3f(config_.CameraXAxis.GetDotProduct(scaledYAxis), config_.CameraYAxis.GetDotProduct(scaledYAxis), config_.CameraZAxis.GetDotProduct(scaledYAxis));

		// Transform the vector from camera to object origin to camera space.
		// Float precision is sufficient in camera-relative space.
		Vec3f originOffset(transform.Position - config_.CameraPosition);
		originOffset = Vec3f(config_.CameraXAxis.GetDotProduct(originOffset), config_.CameraYAxis.GetDotProduct(originOffset), config_.CameraZAxis.GetDotProduct(originOffset));

		for (int i = 0; i < 4; i++)
		{
			// Scale and rotate the flat vertex position, then translate by the origin offset.
			// The vertex position is then in camera space.
			Vec3f vertexPosition = (scaledXAxis * vertices[i].Position.X) + (scaledYAxis * vertices[i].Position.Y) + originOffset;

			// Discard this sprite if any vertex is in front of the near clip plane
			if (vertexPosition.Z > -NEAR_CLIP_DISTANCE)
			{
				// Remove already added vertices of the current quad from the batch
				for (int j = 0; j < i; j++)
					vertexBatch_.pop_back();

				return;
			}

			// Project the vertex position to logical render-target coordinates (Y increases downward)
			Vec2f renderTargetCoords(vertexPosition.X * config_.CameraDistanceToScreen / (-vertexPosition.Z), vertexPosition.Y * config_.CameraDistanceToScreen / vertexPosition.Z);
			renderTargetCoords += config_.RenderTargetOffset;

			// Add the transformed vertex to the batch
			vertexBatch_.emplace_back(renderTargetCoords, vertices[i].Color, vertices[i].UV);
		}
	}


	void SpriteMeshRenderer3D::Render(const Sprite3D& sprite)
	{
		if (!sprite.Mesh) return;

		const Vertex2D* const vertices = sprite.Mesh->Vertices;

		// Interpolate the sprite transform
		const Transform3D interpolatedTransform = GetInterpolated(sprite.GetPrevTransform(), sprite.Transform, config_.InterpolationAlpha);

		// Precompute the local scale and rotation of the flat object
		Vec3f scaledXAxis = interpolatedTransform.Rotation.GetXAxis() * interpolatedTransform.Scale.X;
		Vec3f scaledYAxis = interpolatedTransform.Rotation.GetYAxis() * interpolatedTransform.Scale.Y;

		// Transform the scaled local axes to camera space
		scaledXAxis = Vec3f(config_.CameraXAxis.GetDotProduct(scaledXAxis), config_.CameraYAxis.GetDotProduct(scaledXAxis), config_.CameraZAxis.GetDotProduct(scaledXAxis));
		scaledYAxis = Vec3f(config_.CameraXAxis.GetDotProduct(scaledYAxis), config_.CameraYAxis.GetDotProduct(scaledYAxis), config_.CameraZAxis.GetDotProduct(scaledYAxis));

		// Transform the vector from camera to object origin to camera space.
		// Float precision is sufficient in camera-relative space.
		Vec3f originOffset(interpolatedTransform.Position - config_.CameraPosition);
		originOffset = Vec3f(config_.CameraXAxis.GetDotProduct(originOffset), config_.CameraYAxis.GetDotProduct(originOffset), config_.CameraZAxis.GetDotProduct(originOffset));

		for (int i = 0; i < 4; i++)
		{
			// Scale and rotate the flat vertex position, then translate by the origin offset.
			// The vertex position is then in camera space.
			Vec3f vertexPosition = (scaledXAxis * vertices[i].Position.X) + (scaledYAxis * vertices[i].Position.Y) + originOffset;

			// Discard this sprite if any vertex is in front of the near clip plane
			if (vertexPosition.Z > -NEAR_CLIP_DISTANCE)
			{
				// Remove already added vertices of the current quad from the batch
				for (int j = 0; j < i; j++)
					vertexBatch_.pop_back();

				return;
			}

			// Project the vertex position to logical render-target coordinates (Y increases downward)
			Vec2f renderTargetCoords(vertexPosition.X * config_.CameraDistanceToScreen / (-vertexPosition.Z), vertexPosition.Y * config_.CameraDistanceToScreen / vertexPosition.Z);
			renderTargetCoords += config_.RenderTargetOffset;

			// Add the transformed vertex to the batch
			vertexBatch_.emplace_back(renderTargetCoords, vertices[i].Color, vertices[i].UV);
		}
	}


	void SpriteMeshRenderer3D::Render(const Sprite3DNode& node)
	{
		if (!node.Mesh) return;

		const Vertex2D* const vertices = node.Mesh->Vertices;
		const Sprite3DNode* parent = &node;
		const double interpolationAlpha = (double)config_.InterpolationAlpha; // Convert to double for repeated use

		// Cache initial vertex positions
		Vec3 worldPositionBuffer[4] =
		{
			Vec3(vertices[0].Position.X, vertices[0].Position.Y, 0.0),
			Vec3(vertices[1].Position.X, vertices[1].Position.Y, 0.0),
			Vec3(vertices[2].Position.X, vertices[2].Position.Y, 0.0),
			Vec3(vertices[3].Position.X, vertices[3].Position.Y, 0.0)
		};

		Vec3 prevWorldPositionBuffer[4] =
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
			const Vec3 worldPosition = GetInterpolatedUnchecked(prevWorldPositionBuffer[i], worldPositionBuffer[i], interpolationAlpha);

			// Start with the vector from camera to world-space vertex.
			// Float precision is sufficient in camera-relative space.
			const Vec3f cameraToVertex(worldPosition - config_.CameraPosition);

			// Compute camera-space depth first so discarded sprite nodes can early-out
			const float z = config_.CameraZAxis.GetDotProduct(cameraToVertex);
			if (z > -NEAR_CLIP_DISTANCE)
			{
				// Remove already added vertices of the current quad from the batch
				for (int j = 0; j < i; j++)
					vertexBatch_.pop_back();

				return;
			}

			// Complete the camera-space position
			const float x = config_.CameraXAxis.GetDotProduct(cameraToVertex);
			const float y = config_.CameraYAxis.GetDotProduct(cameraToVertex);

			// Project the vertex position to logical render-target coordinates (Y increases downward)
			Vec2f renderTargetCoords(x * config_.CameraDistanceToScreen / (-z), y * config_.CameraDistanceToScreen / z);
			renderTargetCoords += config_.RenderTargetOffset;

			// Add the transformed vertex to the batch
			vertexBatch_.emplace_back(renderTargetCoords, vertices[i].Color, vertices[i].UV);
		}
	}

	void SpriteMeshRenderer3D::RenderLine(const SpriteMesh& mesh, const Vec3& startPoint, const Vec3& endPoint, float lineWidth)
	{
		const Vertex2D* const vertices = mesh.Vertices;

		// Vectors from camera to the line endpoints (float precision is sufficient in camera-relative space)
		const Vec3f cameraToStartPoint(startPoint - config_.CameraPosition);
		const Vec3f cameraToEndPoint(endPoint - config_.CameraPosition);

		// Compute camera-space depth first so fully clipped lines can early-out
		float z1 = config_.CameraZAxis.GetDotProduct(cameraToStartPoint);
		float z2 = config_.CameraZAxis.GetDotProduct(cameraToEndPoint);

		// Discard lines fully in front of the near clip plane
		if (z1 > -NEAR_CLIP_DISTANCE && z2 > -NEAR_CLIP_DISTANCE) return;

		// Complete the camera-space positions
		float x1 = config_.CameraXAxis.GetDotProduct(cameraToStartPoint);
		float y1 = config_.CameraYAxis.GetDotProduct(cameraToStartPoint);
		float x2 = config_.CameraXAxis.GetDotProduct(cameraToEndPoint);
		float y2 = config_.CameraYAxis.GetDotProduct(cameraToEndPoint);

		// Clip the line segment against the near clip plane
		if (z1 <= -NEAR_CLIP_DISTANCE && z2 > -NEAR_CLIP_DISTANCE)
		{
			const float t = (-NEAR_CLIP_DISTANCE - z1) / (z2 - z1);

			x2 = x1 + t * (x2 - x1);
			y2 = y1 + t * (y2 - y1);
			z2 = -NEAR_CLIP_DISTANCE;
		}
		else if (z2 <= -NEAR_CLIP_DISTANCE && z1 > -NEAR_CLIP_DISTANCE)
		{
			const float t = (-NEAR_CLIP_DISTANCE - z2) / (z1 - z2);

			x1 = x2 + t * (x1 - x2);
			y1 = y2 + t * (y1 - y2);
			z1 = -NEAR_CLIP_DISTANCE;
		}

		// Project the line points to logical render-target coordinates (Y increases downward)
		Vec2f startPointRenderTargetCoords(x1 * config_.CameraDistanceToScreen / (-z1), y1 * config_.CameraDistanceToScreen / z1);
		startPointRenderTargetCoords += config_.RenderTargetOffset;

		Vec2f endPointRenderTargetCoords(x2 * config_.CameraDistanceToScreen / (-z2), y2 * config_.CameraDistanceToScreen / z2);
		endPointRenderTargetCoords += config_.RenderTargetOffset;

		const Vec2f halfWidthNormal = ((startPointRenderTargetCoords - endPointRenderTargetCoords).Normalize() * (lineWidth * 0.5f)).GetNormal();

		// Add quad points in clockwise order on the screen around the centered line segment
		Vec2f quadPoint = startPointRenderTargetCoords + halfWidthNormal;
		vertexBatch_.emplace_back(quadPoint, vertices[0].Color, vertices[0].UV);

		quadPoint = endPointRenderTargetCoords + halfWidthNormal;
		vertexBatch_.emplace_back(quadPoint, vertices[1].Color, vertices[1].UV);

		quadPoint = endPointRenderTargetCoords - halfWidthNormal;
		vertexBatch_.emplace_back(quadPoint, vertices[2].Color, vertices[2].UV);

		quadPoint = startPointRenderTargetCoords - halfWidthNormal;
		vertexBatch_.emplace_back(quadPoint, vertices[3].Color, vertices[3].UV);
	}

	void SpriteMeshRenderer3D::RenderPoint(const SpriteMesh& mesh, const Vec3& point, float pointSize)
	{
		const Vertex2D* const vertices = mesh.Vertices;

		// Start with the vector from camera to the point.
		// Float precision is sufficient in camera-relative space.
		const Vec3f cameraToPoint(point - config_.CameraPosition);

		// Compute camera-space depth first so clipped points can early-out
		const float z = config_.CameraZAxis.GetDotProduct(cameraToPoint);
		if (z > -NEAR_CLIP_DISTANCE) return;

		// Complete the camera-space position
		const float x = config_.CameraXAxis.GetDotProduct(cameraToPoint);
		const float y = config_.CameraYAxis.GetDotProduct(cameraToPoint);

		// Project the point to logical render-target coordinates (Y increases downward)
		Vec2f renderTargetCoords(x * config_.CameraDistanceToScreen / (-z), y * config_.CameraDistanceToScreen / z);
		renderTargetCoords += config_.RenderTargetOffset;

		const float halfSize = pointSize * 0.5f; // Half side length of the generated quad

		// Top-left corner
		Vec2f quadPoint = renderTargetCoords + Vec2f(-halfSize, -halfSize);
		vertexBatch_.emplace_back(quadPoint, vertices[0].Color, vertices[0].UV);

		// Top-right corner
		quadPoint = renderTargetCoords + Vec2f(halfSize, -halfSize);
		vertexBatch_.emplace_back(quadPoint, vertices[1].Color, vertices[1].UV);

		// Bottom-right corner
		quadPoint = renderTargetCoords + Vec2f(halfSize, halfSize);
		vertexBatch_.emplace_back(quadPoint, vertices[2].Color, vertices[2].UV);

		// Bottom-left corner
		quadPoint = renderTargetCoords + Vec2f(-halfSize, halfSize);
		vertexBatch_.emplace_back(quadPoint, vertices[3].Color, vertices[3].UV);
	}



	void SpriteMeshRenderer3D::BeginBatch(const MovableObject3D& camera, const Vec2f& renderTargetOffset, float interpolationAlpha, float verticalFOV)
	{
		vertexBatch_.clear();

		interpolationAlpha = GetClamped(interpolationAlpha, 0.0f, 1.0f);
		verticalFOV = GetClamped(verticalFOV, 1.0f, 179.0f);

		config_.InterpolationAlpha = interpolationAlpha;
		config_.CameraDistanceToScreen = (Renderer::Get().GetLogicalResolutionHeight() * 0.5f) / std::tan(verticalFOV * 0.5f * (float)RADIANS_PER_DEGREE);
		config_.RenderTargetOffset = renderTargetOffset;
		config_.CameraPosition = GetInterpolatedUnchecked(camera.GetPrevTransform().Position, camera.Transform.Position, (double)interpolationAlpha);

		Rotation3D interpolatedCameraRotation = GetInterpolated(camera.GetPrevTransform().Rotation, camera.Transform.Rotation, interpolationAlpha);
		config_.CameraXAxis = interpolatedCameraRotation.GetXAxis();
		config_.CameraYAxis = interpolatedCameraRotation.GetYAxis();
		config_.CameraZAxis = interpolatedCameraRotation.GetZAxis();
	}



	void SpriteMeshRenderer3D::RenderBatch(const Texture& texture, TargetTexture* renderTarget)
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

		renderer.SetRenderScale(cachedRenderScale.X, cachedRenderScale.Y); // Restore cached render scale
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
	void SpriteMeshRenderer3D::UpdateVertexIndices()
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
