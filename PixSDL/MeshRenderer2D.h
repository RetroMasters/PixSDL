#pragma once

#include <vector>
#include "PixMath.h"
#include "TargetTexture.h"
#include "SpriteMesh.h"
#include "MeshSprite2D.h"
#include "MeshSprite2DNode.h"

namespace pix
{
	// MeshRenderer2D batches and renders Mesh2D-based 2D objects to a render target.
	// Only complete triangles are rendered; trailing vertices that do not form a complete triangle are ignored.
	//
	// This renderer is a basic textured/color mesh renderer without built-in lighting.
	// It uses vertex position, color, and UV data. Mesh normals are intentionally ignored here.
	// For advanced effects, update the relevant vertex data first, then render the modified mesh with this renderer,
	// or use a custom renderer.
	// 
	// Coordinate spaces:
	// - World space: X right, Y up.
	//   Maps directly to logical screen space with a default camera (position = (0, 0), rotation = identity, zoom = (1, 1)).
	// - Camera-relative space: world space after subtracting the camera position.	
	// - Camera space: camera-relative space after applying the inverse camera rotation. Camera zoom has not been applied yet.
	// - Logical screen space: camera space after applying camera zoom. Zoom can be non-uniform for intentional morphing.
	// - Logical render-target space: X right, Y down.
	//   The origin is (0, 0) at the top-left of the render target, and (logicalResolutionWidth, logicalResolutionHeight) is at the bottom-right.
	//
	// Mapping logical screen space to logical render-target space:
	// - renderTargetOffset defines where the logical screen-space origin appears on the render target.
	// - For example, renderTargetOffset = (0, logicalResolutionHeight) places the logical screen-space
	//   origin at the bottom-left of the render target, so logical screen-space point
	//   (logicalResolutionWidth, logicalResolutionHeight) maps to the top-right.
	//
	// Camera:
	// - The camera is provided as a MovableObject2D in world space.
	// - Its position defines the view origin.
	// - Its scale represents the zoom vector (applied after camera rotation and can be non-uniform).
	// - Its rotation affects the rendered orientation.
	// 
	// Usage:
	// 1) Call BeginBatch() once per frame (or whenever configuration changes).
	// 2) Call the render methods to submit geometry to the batch.
	// 3) Call RenderBatch() to draw all submitted geometry. It may be called multiple times in a row with different textures or render targets.
	// 
	// Philosophy:
	// MeshRenderer2D focuses on performant rendering of the supported 2D objects.
	// Geometry is collected into a batch and submitted to the GPU through RenderBatch(),
	// which greatly reduces communication overhead between the CPU and GPU.
	// This trades some convenience of immediate per-object rendering for significant performance gains.
	class MeshRenderer2D
	{

	public:

		explicit MeshRenderer2D(int initialVertexBatchCapacity = 50000);
		~MeshRenderer2D() = default;

		// Renders a Mesh2D using the specified world transform.
		// This is the most performant rendering path when the final transform
		// is already available (e.g., static meshes or externally computed transform).
		void Render(const Mesh2D& mesh, const Transform2D& transform);

		// Renders a MeshSprite2D using interpolated transform state.
		// The sprite's previous and current transforms are interpolated using the interpolation factor specified in BeginBatch().
		// This is the typical rendering path for moving sprites without hierarchical parent transforms.
		void Render(const MeshSprite2D& sprite);

		// Transforms the node hierarchy to world space and renders the mesh sprite node using interpolated world-space vertex positions.
		// This is the most general rendering path, but also the least performant. Performance decreases as hierarchy depth and vertex count increase.
		// It evaluates the Transform2D hierarchy per vertex instead of using a precomputed global transform.
		void Render(const MeshSprite2DNode& node);

		// Clears the current batch and updates the rendering configuration.
		// After calling BeginBatch(), subsequent render calls append geometry to the batch, transformed according to this configuration.
		// 
		// Parameters:
		// - camera: World-space camera used for view position, rotation and zoom (scale).
		// - renderTargetOffset: Logical render-target coordinate where the logical screen-space origin appears.
		// - interpolationAlpha: Interpolation factor used for previous -> current transform blending (internally clamped to [0.0f, 1.0f]).
		void BeginBatch(const MovableObject2D& camera, const Vec2f& renderTargetOffset, float interpolationAlpha = 1.0f);

		// Renders the current batch to the specified render target using the given texture.
		// If renderTarget is nullptr, rendering is performed to the default back buffer.
		// RenderBatch() may be called multiple times in a row with different textures or render targets; 
		// it does not modify the batch or the configuration. The batch is cleared only by calling BeginBatch().
		// 
		// Note: 
		// Render target is renderer-global state. This function sets the render target and does not restore the previous one.
		void RenderBatch(const Texture& texture, TargetTexture* renderTarget);

	private:

		struct Configuration
		{
			float InterpolationAlpha = 1.0f;
			Vec2f RenderTargetOffset = Vec2f(0.0f, 0.0f);
			Vec2  CameraPosition = Vec2(0.0, 0.0);

			// Camera axes after inverse camera rotation and camera zoom.
			// Used to convert coordinates from camera-relative space to logical screen space.
			Vec2f ScaledCameraXAxis = Vec2f(1.0f, 0.0f);
			Vec2f ScaledCameraYAxis = Vec2f(0.0f, 1.0f);
		};


		Configuration config_;

		std::vector<Vertex2D> vertexBatch_;

		std::vector<Vec2> worldPositionBuffer_;     // Reusable temporary storage for current world-space vertex positions
		std::vector<Vec2> prevWorldPositionBuffer_; // Reusable temporary storage for previous world-space vertex positions
	};

}


