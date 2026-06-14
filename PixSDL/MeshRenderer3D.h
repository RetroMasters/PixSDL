#pragma once

#include <vector>
#include <cmath>
#include "PixMath.h"
#include "TargetTexture.h"
#include "SpriteMesh.h"
#include "Mesh3D.h"
#include "MeshSprite3D.h"
#include "MeshSprite3DNode.h"

namespace pix
{
	// MeshRenderer3D batches and renders 3D objects based on Mesh2D and Mesh3D to a render target.
	// Only complete triangles are rendered; trailing vertices that do not form a complete triangle are ignored.
	// Backface culling is applied only to Mesh3D.
	// 
	// This renderer is a basic textured/color mesh renderer without built-in lighting.
	// It uses vertex position, color, and UV data. Mesh normals are intentionally ignored here.
	// For advanced effects, update the relevant vertex data first, then render the modified mesh with this renderer,
	// or use a custom renderer.
	// 
	// Coordinate spaces:
	// - World space: X right, Y up, -Z forward.
	//   With identity camera rotation, the camera looks along -Z.	
	// - Camera-relative space: world space after subtracting the camera position.
	// - Camera space: camera-relative space after applying the inverse camera rotation.
	//   With identity camera rotation, this is equivalent to camera-relative space.
	//   Visible geometry has Z <= -NEAR_CLIP_DISTANCE.
	// - Logical render-target space: X right, Y down.
	//   The origin is (0, 0) at the top-left of the render target, and
	//   (logicalResolutionWidth, logicalResolutionHeight) is at the bottom-right.
	// 
	// Projecting camera-space points to the render target:
	// - Camera-space points are perspective-projected directly into logical render-target space using CameraDistanceToScreen.
	//   Projected points outside the render target fall outside the visible render area.
	// - renderTargetOffset defines where the projected center appears on the render target.
	//   For example, renderTargetOffset = (logicalResolutionWidth * 0.5f,
	//   logicalResolutionHeight * 0.5f) places the projection center in the middle.
	//
	// Camera:
	// - The camera is provided as a MovableObject3D in world space.
	// - Its position defines the view origin.
	// - Its rotation affects the rendered orientation.
	// - Its scale property is ignored.
	//
	// Usage:
	// 1) Call BeginBatch() once per frame (or whenever configuration changes).
	// 2) Call the render methods to submit geometry to the batch.
	// 3) Call RenderBatch() to draw all submitted geometry. It may be called multiple times in a row with different textures or render targets.
	//
	// Philosophy:
	// MeshRenderer3D focuses on performant rendering of the supported 3D objects.
	// Geometry is collected into a batch and submitted to the GPU through RenderBatch(),
	// which greatly reduces communication overhead between the CPU and GPU.
	// This trades some convenience of immediate per-object rendering for significant performance gains.
	class MeshRenderer3D
	{

	public:

		// Creates a MeshRenderer3D with optional initial vertex batch capacity.
		// The buffers can still grow automatically if more geometry is submitted.
		explicit MeshRenderer3D(int initialVertexBatchCapacity = 50000);
		~MeshRenderer3D() = default;

		// Renders a Mesh3D using the specified world transform.
		// Backface culling is applied. Front faces appear in clockwise vertex order on the screen.
		void Render(const Mesh3D& mesh, const Transform3D& transform);

		// Renders a Mesh2D using the specified world transform.
		// This is the most performant rendering path for a Mesh2D when the final transform
		// is already available (e.g., static meshes or externally computed transform).
		void Render(const Mesh2D& mesh, const Transform3D& transform);

		// Renders a MeshSprite3D using interpolated transform state.
		// The sprite's previous and current transforms are interpolated using the interpolation factor specified in BeginBatch().
		// This is the typical rendering path for moving sprites without hierarchical parent transforms.
		void Render(const MeshSprite3D& sprite);

		// Transforms the node hierarchy to world space and renders the mesh sprite node using interpolated world-space vertex positions.
		// This is the most general rendering path, but also the least performant. Performance decreases as hierarchy depth and vertex count increase.
		// It evaluates the Transform3D hierarchy per vertex instead of using a precomputed global transform.
		void Render(const MeshSprite3DNode& node);

		// Optimized variant of Render(const MeshSprite3DNode&).
		// Assumes that no rotated non-uniform scaling exists in the ancestor chain
		// (rotated uniform scaling is supported). Under this constraint, hierarchical
		// evaluation can be reduced to a single composed transform, improving performance.
		//void RenderFast(const MeshSprite3DNode& node); 

		// Clears the current batch and updates the rendering configuration.
		// After calling BeginBatch(), subsequent render calls append geometry to the batch, transformed according to this configuration.
		// 
		// Parameters:
		// - camera: World-space camera used for view position and rotation.
		// - renderTargetOffset: Logical render-target coordinate where the projected center appears.
		//   (top-left = (0,0), bottom-right = (logicalResolutionWidth, logicalResolutionHeight)).   
		// - interpolationAlpha: Interpolation factor used for previous -> current transform blending (internally clamped to [0.0f, 1.0f]).
		// - verticalFOV: Vertical field of view in degrees for perspective projection.
		void BeginBatch(const MovableObject3D& camera, const Vec2f& renderTargetOffset, float interpolationAlpha = 1.0f, float verticalFOV = 60.0f);

		// Renders the current batch to the specified render target using the given texture.
		// If renderTarget is nullptr, rendering is performed to the default back buffer.
		// RenderBatch() may be called multiple times in a row with different textures or render targets; 
		// it does not modify the batch or the configuration. The batch is cleared only by calling BeginBatch().
		// 
		// Note: 
		// Render target is renderer-global state. This function sets the render target and does not restore the previous one.
		void RenderBatch(const Texture& texture, TargetTexture* renderTarget);

	private:

		// The near clip plane is at z = -NEAR_CLIP_DISTANCE. 
		// The constant must be greater than zero to avoid projection at z = 0.
		// Triangles are discarded if any vertex has camera-space Z greater than -NEAR_CLIP_DISTANCE.
		// Geometry exactly on the near clip plane is still considered valid.
		static constexpr float NEAR_CLIP_DISTANCE = 0.5f;

		struct Configuration
		{
			float InterpolationAlpha = 1.0f;
			float CameraDistanceToScreen = (1080.0f * 0.5f) / std::tan(60.0f * 0.5f * (float)RADIANS_PER_DEGREE); // = 935.3f for 1080p and verticalFOV = 60.0f
			Vec2f RenderTargetOffset = Vec2f(0.0f, 0.0f);

			Vec3 CameraPosition = Vec3(0.0, 0.0, 0.0);

			// Camera basis axes in world space.
			// Used to convert vectors from camera-relative space to camera space.
			Vec3f CameraXAxis = Vec3f(1.0f, 0.0f, 0.0f);
			Vec3f CameraYAxis = Vec3f(0.0f, 1.0f, 0.0f);
			Vec3f CameraZAxis = Vec3f(0.0f, 0.0f, 1.0f);
		};

		Configuration config_;

		std::vector<Vertex2D> vertexBatch_;
		std::vector<Vec3> worldPositionBuffer_; // Stores current world-space vertex positions
		std::vector<Vec3> prevWorldPositionBuffer_; // Stores previous world-space vertex positions
	};

}

