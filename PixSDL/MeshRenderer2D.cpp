#include "MeshRenderer2D.h"
#include "Renderer.h"

namespace pix
{

	MeshRenderer2D::MeshRenderer2D(int initialVertexBatchCapacity)
	{
		if (initialVertexBatchCapacity > 0)
			vertexBatch_.reserve(initialVertexBatchCapacity);

		// A reasonable default, used only in Render(const MeshSprite2DNode&)
		worldPositionBuffer_.reserve(100);
		prevWorldPositionBuffer_.reserve(100);
	}


	void MeshRenderer2D::Render(const Mesh2D& mesh, const Transform2D& transform)
	{
		const std::vector<MeshVertex2D>& vertices = mesh.Vertices;
		const int completeVertexCount = (vertices.size() / 3) * 3;

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

		for (int i = 0; i < completeVertexCount; i++)
		{
			// Scale and rotate the vertex position, then translate it by the origin offset.
			// The vertex position is then in logical screen space.
			Vec2f vertexPosition = (scaledXAxis * vertices[i].Position.X) + (scaledYAxis * vertices[i].Position.Y) + originOffset;

			// Logical screen space -> render target space (Y increases downward)
			vertexPosition.X = config_.RenderTargetOffset.X + vertexPosition.X;
			vertexPosition.Y = config_.RenderTargetOffset.Y - vertexPosition.Y;

			// Add the transformed vertex to the batch
			vertexBatch_.emplace_back(vertexPosition, vertices[i].Color, vertices[i].UV);
		}
	}

	void MeshRenderer2D::Render(const MeshSprite2D& sprite)
	{
		if (!sprite.Mesh) return;

		const std::vector<MeshVertex2D>& vertices = sprite.Mesh->Vertices;
		const int completeVertexCount = (vertices.size() / 3) * 3;

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

		for (int i = 0; i < completeVertexCount; i++)
		{
			// Scale and rotate the vertex position, then translate it by the origin offset.
			// The vertex position is then in logical screen space.
			Vec2f vertexPosition = (scaledXAxis * vertices[i].Position.X) + (scaledYAxis * vertices[i].Position.Y) + originOffset;

			// Logical screen space -> render target space (Y increases downward)
			vertexPosition.X = config_.RenderTargetOffset.X + vertexPosition.X;
			vertexPosition.Y = config_.RenderTargetOffset.Y - vertexPosition.Y;

			// Add the transformed vertex to the batch
			vertexBatch_.emplace_back(vertexPosition, vertices[i].Color, vertices[i].UV);
		}
	}

	void MeshRenderer2D::Render(const MeshSprite2DNode& node)
	{
		if (!node.Mesh) return;

		const MeshSprite2DNode* parent = &node;
		const std::vector<MeshVertex2D>& vertices = node.Mesh->Vertices;
		const int completeVertexCount = (vertices.size() / 3) * 3;
		const double interpolationAlpha = (double)config_.InterpolationAlpha; // Convert to double for repeated use

		if (worldPositionBuffer_.size() < completeVertexCount)
		{
			worldPositionBuffer_.resize(completeVertexCount * 2);
			prevWorldPositionBuffer_.resize(completeVertexCount * 2);
		}

		// Cache initial vertex positions
		for (int i = 0; i < completeVertexCount; i++)
		{
			const Vec2 position(vertices[i].Position);

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
			const Vec2 worldPosition = GetInterpolatedUnchecked(prevWorldPositionBuffer_[i], worldPositionBuffer_[i], interpolationAlpha);

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

	void MeshRenderer2D::BeginBatch(const MovableObject2D& camera, const Vec2f& renderTargetOffset, float interpolationAlpha)
	{
		vertexBatch_.clear();

		interpolationAlpha = GetClamped(interpolationAlpha, 0.0f, 1.0f);

		config_.InterpolationAlpha = interpolationAlpha;
		config_.RenderTargetOffset = renderTargetOffset;
		config_.CameraPosition = GetInterpolatedUnchecked(camera.GetPrevTransform().Position, camera.Transform.Position, (double)interpolationAlpha);

		Vec2f interpolatedCameraZoom = GetInterpolatedUnchecked(camera.GetPrevTransform().Scale, camera.Transform.Scale, interpolationAlpha);
		const Rotation2D interpolatedInverseCameraRotation = GetInterpolated(camera.GetPrevTransform().Rotation, camera.Transform.Rotation, interpolationAlpha).Inverse();
		config_.ScaledCameraXAxis = interpolatedInverseCameraRotation.GetXAxis() * interpolatedCameraZoom;
		config_.ScaledCameraYAxis = interpolatedInverseCameraRotation.GetYAxis() * interpolatedCameraZoom;
	}


	void MeshRenderer2D::RenderBatch(const Texture& texture, TargetTexture* renderTarget)
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


