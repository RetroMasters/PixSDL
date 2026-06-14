#pragma once

#include <vector>
#include <SDL_rect.h>
#include <SDL_render.h>
#include "MeshAnimator.h"

namespace pix
{
	// Samples keyframes in book-reading order in normalized UV space: left-to-right, then top-to-bottom.
	// Starts at startRect. Each subsequent frame advances by the width of startRect in X.
	// When the next frame would exceed the normalized texture width 1.0f, sampling continues on the next row:
	// Y += height of startRect and the lower X bound is reset to 0.0f.
	// Frames are generated only while the full rectangle fits inside the normalized texture area [0, 1];
	// sampling stops early if no further full frame fits.
	// Flipped UV corners of startRect are preserved and produce flipped UV mapping.
	// Each keyframe's duration is set to durationTicks.
	// On failure (invalid inputs), returns an empty sequence.
	std::vector<SpriteMeshUVKeyframe> CreateSpriteMeshUVKeyframeSequence(const UVRect& startRect, int frameCount, float durationTicks);

	// Pixel-space convenience wrapper for CreateSpriteMeshUVKeyframeSequence(const UVRect&, ...).
	// texWidth/texHeight are the texture size in pixels and are used to normalize startRect.
	// Sampling starts at startRect and follows book-reading order: left-to-right, then top-to-bottom.
	// Frames are generated only while the full rectangle fits inside the texture.
	// flip flags can be combined (SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL) and are applied to each frame.
	// Each keyframe's duration is set to durationTicks.
	// On failure (invalid inputs), returns an empty sequence.
	std::vector<SpriteMeshUVKeyframe> CreateSpriteMeshUVKeyframeSequence(int texWidth, int texHeight, const SDL_Rect& startRect, int frameCount, float durationTicks, SDL_RendererFlip flip = SDL_FLIP_NONE);

	// Like CreateSpriteMeshUVKeyframeSequence(const UVRect&, ...), but creates MeshUVKeyframes for a subdivided grid mesh.
	// Samples keyframes in book-reading order in normalized UV space: left-to-right, then top-to-bottom.
	// subdivisionAmount defines the number of rectangular UV cells along X and Y within each frame.
	// Each cell is emitted as 6 UVs corresponding to two triangles. subdivisionAmount = Vec2i(1, 1) produces one cell with 6 UVs.
	// Subdivision values smaller than 1 are clamped to 1.
	// Flipped UV corners of startRect are preserved and produce flipped UV mapping.
	// The generated UV order matches CreateGridMesh2D()/CreateGridMesh3D(): cells are emitted in book-reading order.
	// On failure (invalid inputs), returns an empty sequence.
	// 
	// Conceptual UV corner layout inside each cell as seen by a viewer looking into the screen:
	// 0--1
	// |\ |
	// | \|
	// 3--2
	//
	// Triangle corner order, corresponding to the 6 emitted UVs:
	// First triangle:  0-1-2
	// Second triangle: 2-3-0
	std::vector<MeshUVKeyframe> CreateMeshUVKeyframeSequence(const UVRect& startRect, int frameCount, float durationTicks, const Vec2i& subdivisionAmount);

	// Pixel-space convenience wrapper for CreateMeshUVKeyframeSequence(const UVRect&, ...).
	// texWidth/texHeight are the texture size in pixels and are used to normalize startRect.
	// Sampling starts at startRect and follows book-reading order: left-to-right, then top-to-bottom.
	// Frames are generated only while the full rectangle fits inside the texture.
	// Each sampled frame is subdivided according to subdivisionAmount.
	// Subdivision values smaller than 1 are treated as 1.
	// flip flags can be combined (SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL) and are applied to each frame.
	// Each keyframe's duration is set to durationTicks.
	// On failure (invalid inputs), returns an empty sequence.
	std::vector<MeshUVKeyframe> CreateMeshUVKeyframeSequence(int texWidth, int texHeight, const SDL_Rect& startRect, int frameCount, float durationTicks, const Vec2i& subdivisionAmount = Vec2i(1, 1), SDL_RendererFlip flip = SDL_FLIP_NONE);


	// Creates a copy of sequence with reversed keyframe order
	template<typename KeyframeType>
	inline std::vector<KeyframeType> CreateReversedSequence(const std::vector<KeyframeType>& sequence)
	{
		return std::vector<KeyframeType>(sequence.rbegin(), sequence.rend());
	}


	// Switches the animator to reversedSequence.
	// reversedSequence is typically the current frame sequence in reversed order,
	// but may also contain additional frames.
	// The current frame index and elapsed time within the keyframe are mirrored
	// so that playback continues seamlessly in the opposite direction.
	// If the new sequence is shorter, the frame index is clamped to a valid range.
	// Returns true if the switch was performed (the sequence must be non-empty), false otherwise.
	template<typename KeyframeType, typename TargetMeshType>
	inline bool SwitchToReversedSequence(MeshAnimator<KeyframeType, TargetMeshType>& animator, const std::vector<KeyframeType>& reversedSequence)
	{
		if (reversedSequence.empty()) return false;

		const KeyframeType* currentFrame = animator.GetCurrentFrame();

		if (!currentFrame) return false; // An existing currentFrame ensures that the animator sequence is not empty

		animator.SetCurrentFrameIndex(animator.GetFrameSequence()->size() - 1 - animator.GetCurrentFrameIndex());
		animator.SetElapsedTicks(currentFrame->DurationTicks - animator.GetElapsedTicks());

		animator.SetFrameSequence(&reversedSequence);

		return true;
	}

	// Flips the horizontal UV coordinates of keyframe
	void FlipX(SpriteMeshUVKeyframe& keyframe);

	// Flips the vertical UV coordinates of keyframe
	void FlipY(SpriteMeshUVKeyframe& keyframe);

	// Flips the horizontal and vertical UV coordinates of keyframe
	void FlipXY(SpriteMeshUVKeyframe& keyframe);


	// Creates a full-vertex keyframe that captures the current vertices of mesh.
	// The keyframe's duration is set to durationTicks.
	SpriteMeshKeyframe CreateSpriteMeshKeyframe(const SpriteMesh& mesh, float durationTicks);


	// Creates a position keyframe that captures the current vertex positions of mesh.
	// The keyframe's duration is set to durationTicks.
	SpriteMeshPositionKeyframe CreateSpriteMeshPositionKeyframe(const SpriteMesh& mesh, float durationTicks);

	// Creates a color keyframe that captures the current vertex colors of mesh.
	// The keyframe's duration is set to durationTicks.
	SpriteMeshColorKeyframe CreateSpriteMeshColorKeyframe(const SpriteMesh& mesh, float durationTicks);

	// Creates a UV keyframe that captures the current vertex UV coordinates of mesh.
	// The keyframe's duration is set to durationTicks.
	SpriteMeshUVKeyframe CreateSpriteMeshUVKeyframe(const SpriteMesh& mesh, float durationTicks);

	// Creates a SpriteMesh UV keyframe from uvRect.
    // The UVRect is used as-is; bounds are not validated.
    // Flipped UVRect corners are preserved and produce flipped UV mapping.
    // Each keyframe's duration is set to durationTicks.
	SpriteMeshUVKeyframe CreateSpriteMeshUVKeyframe(const UVRect& uvRect, float durationTicks);

	// Returns a SpriteMesh UV keyframe from quad.
	// The UVQuad is used as-is; bounds are not validated.
	// Each keyframe's duration is set to durationTicks.
	SpriteMeshUVKeyframe CreateSpriteMeshUVKeyframe(const UVQuad& quad, float durationTicks);

	// Creates a full-vertex keyframe that captures the current vertices of mesh.
	// The keyframe's duration is set to durationTicks.
	Mesh2DKeyframe CreateMesh2DKeyframe(const Mesh2D& mesh, float durationTicks);

	// Creates a position keyframe that captures the current vertex positions of mesh.
	// The keyframe's duration is set to durationTicks.
	Mesh2DPositionKeyframe CreateMesh2DPositionKeyframe(const Mesh2D& mesh, float durationTicks);

	// Creates a color keyframe that captures the current vertex colors of mesh.
	// The keyframe's duration is set to durationTicks.
	MeshColorKeyframe CreateMeshColorKeyframe(const Mesh2D& mesh, float durationTicks);

	// Creates a UV keyframe that captures the current vertex UV coordinates of mesh.
	// The keyframe's duration is set to durationTicks.
	MeshUVKeyframe CreateMeshUVKeyframe(const Mesh2D& mesh, float durationTicks);

	// Creates a Mesh UV keyframe from uvRect.
	// The UVRect is subdivided according to subdivisionAmount.
	// The UVRect is used as-is; bounds are not validated.
	// Flipped UVRect corners are preserved and produce flipped UV mapping.
	// If a subdivision dimension is less than 1, it is treated as 1.
	// Each keyframe's duration is set to durationTicks.
	MeshUVKeyframe CreateMeshUVKeyframe(const UVRect& uvRect, float durationTicks, const Vec2i& subdivisionAmount);

	// Creates a normal keyframe that captures the current vertex normals of mesh.
	// The keyframe's duration is set to durationTicks.
	MeshNormalKeyframe CreateMeshNormalKeyframe(const Mesh2D& mesh, float durationTicks);

	// Creates a full-vertex keyframe that captures the current vertices of mesh.
	// The keyframe's duration is set to durationTicks.
	Mesh3DKeyframe CreateMesh3DKeyframe(const Mesh3D& mesh, float durationTicks);

	// Creates a position keyframe that captures the current vertex positions of mesh.
	// The keyframe's duration is set to durationTicks.
	Mesh3DPositionKeyframe CreateMesh3DPositionKeyframe(const Mesh3D& mesh, float durationTicks);

	// Creates a color keyframe that captures the current vertex colors of mesh.
	// The keyframe's duration is set to durationTicks.
	MeshColorKeyframe CreateMeshColorKeyframe(const Mesh3D& mesh, float durationTicks);

	// Creates a UV keyframe that captures the current vertex UV coordinates of mesh.
	// The keyframe's duration is set to durationTicks.
	MeshUVKeyframe CreateMeshUVKeyframe(const Mesh3D& mesh, float durationTicks);

	// Creates a normal keyframe that captures the current vertex normals of mesh.
	// The keyframe's duration is set to durationTicks.
	MeshNormalKeyframe CreateMeshNormalKeyframe(const Mesh3D& mesh, float durationTicks);

}
