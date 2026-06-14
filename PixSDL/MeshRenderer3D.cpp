#include "MeshRenderer3D.h"
#include "Renderer.h"

namespace pix
{
	MeshRenderer3D::MeshRenderer3D(int initialVertexBatchCapacity)
	{
		if (initialVertexBatchCapacity > 0)
			vertexBatch_.reserve(initialVertexBatchCapacity);

		// A reasonable default, used only in Render(const Sprite3DExNode& node) 
		worldPositionBuffer_.reserve(100);
		prevWorldPositionBuffer_.reserve(100);
	}

	// Best version
	void MeshRenderer3D::Render(const Mesh3D& mesh, const Transform3D& transform)
	{
		const std::vector<MeshVertex3D>& vertices = mesh.Vertices;
		const int lastTriangleIndex = (int)vertices.size() - 3;

		// Precompute the local scale and rotation of the object
		Vec3f scaledXAxis = transform.Rotation.GetXAxis() * transform.Scale.X;
		Vec3f scaledYAxis = transform.Rotation.GetYAxis() * transform.Scale.Y;
		Vec3f scaledZAxis = transform.Rotation.GetZAxis() * transform.Scale.Z;

		// Transform the scaled local axes to camera space
		scaledXAxis = Vec3f(config_.CameraXAxis.GetDotProduct(scaledXAxis), config_.CameraYAxis.GetDotProduct(scaledXAxis), config_.CameraZAxis.GetDotProduct(scaledXAxis));
		scaledYAxis = Vec3f(config_.CameraXAxis.GetDotProduct(scaledYAxis), config_.CameraYAxis.GetDotProduct(scaledYAxis), config_.CameraZAxis.GetDotProduct(scaledYAxis));
		scaledZAxis = Vec3f(config_.CameraXAxis.GetDotProduct(scaledZAxis), config_.CameraYAxis.GetDotProduct(scaledZAxis), config_.CameraZAxis.GetDotProduct(scaledZAxis));

		// Transform the vector from camera to object origin to camera space.
		// Float precision is sufficient in camera-relative space.
		Vec3f originOffset(transform.Position - config_.CameraPosition);
		originOffset = Vec3f(config_.CameraXAxis.GetDotProduct(originOffset), config_.CameraYAxis.GetDotProduct(originOffset), config_.CameraZAxis.GetDotProduct(originOffset));

		for (int i = 0; i <= lastTriangleIndex; i += 3)
		{
			Vec2f projectedVertices[3];
			bool isClipped = false;

			// Process triangle
			for (int j = 0; j < 3; j++)
			{
				Vec3f vertexPosition = vertices[i + j].Position;

				// Scale and rotate the vertex position, then translate by the origin offset.
				// The vertex position is then in camera space.
				vertexPosition = (scaledXAxis * vertexPosition.X) + (scaledYAxis * vertexPosition.Y) + (scaledZAxis * vertexPosition.Z) + originOffset;

				// Discard this triangle if any vertex is in front of the near clip plane
				if (vertexPosition.Z > -NEAR_CLIP_DISTANCE)
				{
					isClipped = true;
					break;
				}

				// Project the vertex position to logical render-target coordinates (Y increases downward)
				projectedVertices[j] = Vec2f(vertexPosition.X * config_.CameraDistanceToScreen / (-vertexPosition.Z), vertexPosition.Y * config_.CameraDistanceToScreen / vertexPosition.Z);
				projectedVertices[j] += config_.RenderTargetOffset;
			}

			if (isClipped) continue;

			// Perform backface culling or add the transformed triangle to the batch.
			// Front faces appear in clockwise vertex order on the screen.
			const Vec2f edgeNormal(projectedVertices[0].Y - projectedVertices[1].Y, projectedVertices[1].X - projectedVertices[0].X);
			const Vec2f closingEdge(projectedVertices[2].X - projectedVertices[1].X, projectedVertices[2].Y - projectedVertices[1].Y);

			if (closingEdge.GetDotProduct(edgeNormal) > 0.0f)
			{
				vertexBatch_.emplace_back(projectedVertices[0], vertices[i + 0].Color, vertices[i + 0].UV);
				vertexBatch_.emplace_back(projectedVertices[1], vertices[i + 1].Color, vertices[i + 1].UV);
				vertexBatch_.emplace_back(projectedVertices[2], vertices[i + 2].Color, vertices[i + 2].UV);
			}
		}
	}


	void MeshRenderer3D::Render(const Mesh2D& mesh, const Transform3D& transform)
	{
		const std::vector<MeshVertex2D>& vertices = mesh.Vertices;
		const int completeVertexCount = (vertices.size() / 3) * 3; // Ignore trailing vertices that do not form a complete triangle

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

		for (int i = 0; i < completeVertexCount; i++)
		{
			// Scale and rotate the flat vertex position, then translate by the origin offset.
			// The vertex position is then in camera space.
			const Vec3f vertexPosition = (scaledXAxis * vertices[i].Position.X) + (scaledYAxis * vertices[i].Position.Y) + originOffset;

			// Discard this triangle if any vertex is in front of the near clip plane
			if (vertexPosition.Z > -NEAR_CLIP_DISTANCE)
			{
				const int triangleVertexIndex = i % 3;

				// Remove already added vertices of the current triangle from the batch
				for (int j = 0; j < triangleVertexIndex; j++)
					vertexBatch_.pop_back();

				i += (2 - triangleVertexIndex); // Jump to the last vertex of the discarded triangle

				continue;
			}

			// Project the vertex position to logical render-target coordinates (Y increases downward)
			Vec2f renderTargetCoords(vertexPosition.X * config_.CameraDistanceToScreen / (-vertexPosition.Z), vertexPosition.Y * config_.CameraDistanceToScreen / vertexPosition.Z);
			renderTargetCoords += config_.RenderTargetOffset;

			// Add the transformed vertex to the batch
			vertexBatch_.emplace_back(renderTargetCoords, vertices[i].Color, vertices[i].UV);
		}
	}

	void MeshRenderer3D::Render(const MeshSprite3D& sprite)
	{
		if (!sprite.Mesh) return;

		const std::vector<MeshVertex2D>& vertices = sprite.Mesh->Vertices;
		const int completeVertexCount = (vertices.size() / 3) * 3; // Ignore trailing vertices that do not form a complete triangle

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

		for (int i = 0; i < completeVertexCount; i++)
		{
			// Scale and rotate the flat vertex position, then translate by the origin offset.
			// The vertex position is then in camera space.
			const Vec3f vertexPosition = (scaledXAxis * vertices[i].Position.X) + (scaledYAxis * vertices[i].Position.Y) + originOffset;

			// Discard this triangle if any vertex is in front of the near clip plane
			if (vertexPosition.Z > -NEAR_CLIP_DISTANCE)
			{
				const int triangleVertexIndex = i % 3;

				// Remove already added vertices of the current triangle from the batch
				for (int j = 0; j < triangleVertexIndex; j++)
					vertexBatch_.pop_back();

				i += (2 - triangleVertexIndex);  // Jump to the last vertex of the discarded triangle

				continue;
			}

			// Project the vertex position to logical render-target coordinates (Y increases downward)
			Vec2f renderTargetCoords(vertexPosition.X * config_.CameraDistanceToScreen / (-vertexPosition.Z), vertexPosition.Y * config_.CameraDistanceToScreen / vertexPosition.Z);
			renderTargetCoords += config_.RenderTargetOffset;

			// Add the transformed vertex to the batch
			vertexBatch_.emplace_back(renderTargetCoords, vertices[i].Color, vertices[i].UV);
		}
	}


	void MeshRenderer3D::Render(const MeshSprite3DNode& node)
	{
		if (!node.Mesh) return;

		const MeshSprite3DNode* parent = &node;
		const std::vector<MeshVertex2D>& vertices = node.Mesh->Vertices;
		const int completeVertexCount = (vertices.size() / 3) * 3; // Ignore trailing vertices that do not form a complete triangle
		const double interpolationAlpha = (double)config_.InterpolationAlpha; // Convert to double for repeated use

		if (worldPositionBuffer_.size() < completeVertexCount)
		{
			worldPositionBuffer_.resize(completeVertexCount * 2);
			prevWorldPositionBuffer_.resize(completeVertexCount * 2);
		}

		// Cache initial vertex positions
		for (int i = 0; i < completeVertexCount; i++)
		{
			const Vec3 position(vertices[i].Position.X, vertices[i].Position.Y, 0.0);

			worldPositionBuffer_[i] = position;
			prevWorldPositionBuffer_[i] = position;
		}

		// Transform current and previous vertex positions to world space
		while (parent)
		{
			parent->Transform.TransformPoints(worldPositionBuffer_.data(), completeVertexCount);
			parent->GetPrevTransform().TransformPoints(prevWorldPositionBuffer_.data(), completeVertexCount);
			parent = parent->GetParent();
		}

		for (int i = 0; i < completeVertexCount; i++)
		{
			// Interpolate the world-space vertex position
			const Vec3 worldPosition = GetInterpolatedUnchecked(prevWorldPositionBuffer_[i], worldPositionBuffer_[i], interpolationAlpha);

			// Start with the vector from camera to world-space vertex.
			// Float precision is sufficient in camera-relative space.
			const Vec3f cameraToVertex(worldPosition - config_.CameraPosition);

			// Compute camera-space depth first so triangles in front of the near clip plane can early-out
			const float z = config_.CameraZAxis.GetDotProduct(cameraToVertex);
			if (z > -NEAR_CLIP_DISTANCE)
			{
				const int triangleVertexIndex = i % 3;

				// Remove already added vertices of the current triangle from the batch
				for (int j = 0; j < triangleVertexIndex; j++)
					vertexBatch_.pop_back();

				i += (2 - triangleVertexIndex); // Jump to the last vertex of the discarded triangle

				continue;
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


	void MeshRenderer3D::BeginBatch(const MovableObject3D& camera, const Vec2f& renderTargetOffset, float interpolationAlpha, float verticalFOV)
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


	void MeshRenderer3D::RenderBatch(const Texture& texture, TargetTexture* renderTarget)
	{
		if (vertexBatch_.size() < 3) return;

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

		renderer.RenderGeometryRaw(texture, &(vertexArray->Position.X), stride, &(vertexArray->Color), stride, &(vertexArray->UV.X), stride, vertexBatch_.size(), nullptr, 0, 4);

		renderer.SetRenderScale(cachedRenderScale.X, cachedRenderScale.Y); // Restore cached render scale
	}

}