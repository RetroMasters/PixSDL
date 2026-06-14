#pragma once

#include <vector>
#include "PixMath.h"
#include "TargetTexture.h"
#include "Sprite2D.h"
#include "Sprite2DNode.h"

namespace pix
{
	// SpriteMeshRenderer2D batches and renders SpriteMesh-based 2D objects to a render target.
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
	// SpriteMeshRenderer2D focuses on performant rendering of the supported 2D objects.
	// Geometry is collected into a batch and submitted to the GPU through RenderBatch(),
	// which greatly reduces communication overhead between the CPU and GPU.
	// This trades some convenience of immediate per-object rendering for significant performance gains.
	class SpriteMeshRenderer2D
	{

	public:

		explicit SpriteMeshRenderer2D(int initialVertexBatchCapacity = 50000);
		~SpriteMeshRenderer2D() = default;

		// Renders a SpriteMesh using the specified world transform.
		// This is the most performant alternative for rendering sprites when the final transform
		// is already available (e.g., static meshes or externally computed transform).
		void Render(const SpriteMesh& mesh, const Transform2D& transform);

		// Renders a Sprite2D using interpolated transform state.
		// The sprite's previous and current transforms are interpolated using the interpolation factor specified in BeginBatch().
		// This is the typical rendering path for moving sprites without hierarchical parent transforms.
		void Render(const Sprite2D& sprite);

		// Transforms the node hierarchy to world space and renders the sprite node using interpolated world-space vertex positions.
		// This is the most general rendering path, but also the least performant. Performance decreases as hierarchy depth increases.
		// It evaluates the Transform2D hierarchy per vertex instead of using a precomputed global transform.
		void Render(const Sprite2DNode& node);

		// Renders a point at the specified world-space position using a quad centered on that position.
		// pointSize is specified in logical render-target units and defines the size of the generated quad.
		// If pointSize is negative (not intended), the generated corner ordering is flipped.
		// Fractional values are supported and influence rasterization/rounding.
		void RenderPoint(const SpriteMesh& mesh, const Vec2& point, float pointSize);

		// Renders a line between the world-space positions startPoint and endPoint by stretching the mesh along the segment.
		// lineWidth is specified in logical render-target units. Fractional values are supported and influence rasterization/rounding.
		// If lineWidth is negative (not intended), the generated corner ordering is flipped.
		// The line segment runs through the center of the generated quad.
		void RenderLine(const SpriteMesh& mesh, const Vec2& startPoint, const Vec2& endPoint, float lineWidth);

		// Renders a texture-based square quad in logical render-target space.
		// position is the center of the generated quad.
		// pixelSize is the side length of the generated quad in logical render-target units.
		// If pixelSize is negative (not intended), the generated corner ordering is flipped.
		// No snapping or truncation is performed; callers can snap positions before calling if desired.
		// Low-overhead: operates directly in render-target space and does not apply the camera transform.
		void RenderPixel(const SpriteMesh& mesh, const Vec2f& position, float pixelSize = 1.0f);

		// Renders a texture-based line quad in logical render-target space.
		// startPosition and endPosition define the center line of the generated quad.
		// lineWidth is the width of the generated quad in logical render-target units.
		// If lineWidth is negative (not intended), the generated corner ordering is flipped.
		// Because the line is rendered as a stretched quad, this can also be used for oriented quad rendering.
		// No snapping or truncation is performed; callers can snap positions before calling if desired.
		// Low-overhead: operates directly in render-target space and does not apply the camera transform.
		void RenderPixelLine(const SpriteMesh& mesh, const Vec2f& startPosition, const Vec2f& endPosition, float lineWidth = 1.0f);

		// Like RenderPixelLine(), but optimized for horizontal lines.
		// startPosition is the center of the left edge of the generated quad.
		// length is the horizontal length in logical render-target units.
		// No snapping or truncation is performed; callers can snap positions before calling if desired.
		// Low-overhead: operates directly in render-target space and does not apply the camera transform.
		void RenderHorizontalPixelLine(const SpriteMesh& mesh, const Vec2f& startPosition, float length, float lineWidth = 1.0f);

		// Like RenderPixelLine(), but optimized for vertical lines.
		// startPosition is the center of the top edge of the generated quad.
		// length is the vertical length in logical render-target units.
		// No snapping or truncation is performed; callers can snap positions before calling if desired.
		// Low-overhead: operates directly in render-target space and does not apply the camera transform.
		void RenderVerticalPixelLine(const SpriteMesh& mesh, const Vec2f& startPosition, float length, float lineWidth = 1.0f);

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

		void UpdateVertexIndices();

		Configuration config_;

		std::vector<Vertex2D> vertexBatch_;
		std::vector<int> vertexIndices_;
	};
}


