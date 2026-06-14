#pragma once

#include <stddef.h>
#include <vector>
#include <SDL_pixels.h>
#include "PixMath.h"
#include "UV.h"
#include "SpriteMesh.h"
#include "Mesh2D.h"
#include "Mesh3D.h"


namespace pix
{
	// Animation keyframe for the complete vertices of a SpriteMesh.
	// DurationTicks is the number of update ticks the frame should persist and may be fractional to express an average duration.
	struct SpriteMeshKeyframe
	{
		SpriteMeshKeyframe() = default;

		SpriteMeshKeyframe(const Vertex2D& topLeft, const Vertex2D& topRight, const Vertex2D& bottomRight, const Vertex2D& bottomLeft, float durationTicks) :
			Vertices{ topLeft, topRight, bottomRight, bottomLeft },
			DurationTicks(durationTicks)
		{}

		Vertex2D& TopLeft() { return Vertices[0]; }
		Vertex2D& TopRight() { return Vertices[1]; }
		Vertex2D& BottomRight() { return Vertices[2]; }
		Vertex2D& BottomLeft() { return Vertices[3]; }

		const Vertex2D& TopLeft()     const { return Vertices[0]; }
		const Vertex2D& TopRight()    const { return Vertices[1]; }
		const Vertex2D& BottomRight() const { return Vertices[2]; }
		const Vertex2D& BottomLeft()  const { return Vertices[3]; }

		Vertex2D Vertices[SpriteMesh::VERTEX_COUNT];

		float DurationTicks = 1.0f;
	};

	// Animation keyframe for the position attributes of a SpriteMesh.
	// DurationTicks is the number of update ticks the frame should persist and may be fractional to express an average duration.
	struct SpriteMeshPositionKeyframe
	{
		SpriteMeshPositionKeyframe() = default;

		SpriteMeshPositionKeyframe(const Vec2f& topLeft, const Vec2f& topRight, const Vec2f& bottomRight, const Vec2f& bottomLeft, float durationTicks) :
			Positions{ topLeft, topRight, bottomRight, bottomLeft },
			DurationTicks(durationTicks)
		{}

		Vec2f& TopLeft() { return Positions[0]; }
		Vec2f& TopRight() { return Positions[1]; }
		Vec2f& BottomRight() { return Positions[2]; }
		Vec2f& BottomLeft() { return Positions[3]; }

		const Vec2f& TopLeft() const { return Positions[0]; }
		const Vec2f& TopRight() const { return Positions[1]; }
		const Vec2f& BottomRight() const { return Positions[2]; }
		const Vec2f& BottomLeft() const { return Positions[3]; }

		Vec2f Positions[SpriteMesh::VERTEX_COUNT] = { {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f} };

		float DurationTicks = 1.0f;
	};

	// Animation keyframe for the color attributes of a SpriteMesh.
	// DurationTicks is the number of update ticks the frame should persist and may be fractional to express an average duration.
	struct SpriteMeshColorKeyframe
	{
		SpriteMeshColorKeyframe() = default;

		SpriteMeshColorKeyframe(SDL_Color topLeft, SDL_Color topRight, SDL_Color bottomRight, SDL_Color bottomLeft, float durationTicks) :
			Colors{ topLeft, topRight, bottomRight, bottomLeft },
			DurationTicks(durationTicks)
		{}

		SDL_Color& TopLeft() { return Colors[0]; }
		SDL_Color& TopRight() { return Colors[1]; }
		SDL_Color& BottomRight() { return Colors[2]; }
		SDL_Color& BottomLeft() { return Colors[3]; }

		const SDL_Color& TopLeft() const { return Colors[0]; }
		const SDL_Color& TopRight() const { return Colors[1]; }
		const SDL_Color& BottomRight() const { return Colors[2]; }
		const SDL_Color& BottomLeft() const { return Colors[3]; }

		SDL_Color Colors[SpriteMesh::VERTEX_COUNT] = { { 255, 255, 255, 255}, { 255, 255, 255, 255 }, { 255, 255, 255, 255 }, { 255, 255, 255, 255 } };

		float DurationTicks = 1.0f;
	};

	// Animation keyframe for the UV attributes of a SpriteMesh.
	// DurationTicks is the number of update ticks the frame should persist and may be fractional to express an average duration.
	struct SpriteMeshUVKeyframe
	{
		SpriteMeshUVKeyframe() = default;

		SpriteMeshUVKeyframe(const UVRect& uvRect, float durationTicks) :
			UVs{ uvRect.TopLeft, uvRect.GetTopRight(), uvRect.BottomRight, uvRect.GetBottomLeft() },
			DurationTicks(durationTicks)
		{}

		SpriteMeshUVKeyframe(const UVQuad& uvFrame, float durationTicks) :
			UVs{ uvFrame.UVs[0], uvFrame.UVs[1], uvFrame.UVs[2], uvFrame.UVs[3] },
			DurationTicks(durationTicks)
		{}

		Vec2f& TopLeft() { return UVs[0]; }
		Vec2f& TopRight() { return UVs[1]; }
		Vec2f& BottomRight() { return UVs[2]; }
		Vec2f& BottomLeft() { return UVs[3]; }

		const Vec2f& TopLeft()     const { return UVs[0]; }
		const Vec2f& TopRight()    const { return UVs[1]; }
		const Vec2f& BottomRight() const { return UVs[2]; }
		const Vec2f& BottomLeft()  const { return UVs[3]; }

		Vec2f UVs[SpriteMesh::VERTEX_COUNT] = { {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f} };
		float DurationTicks = 1.0f;
	};

	// Animation keyframe for the complete vertices of a Mesh2D.
	// DurationTicks is the number of update ticks the frame should persist and may be fractional to express an average duration.
	struct Mesh2DKeyframe
	{
		Mesh2DKeyframe() = default;

		Mesh2DKeyframe(const std::vector<MeshVertex2D>& vertices, float durationTicks) :
			Vertices(vertices),
			DurationTicks(durationTicks)
		{}

		std::vector<MeshVertex2D> Vertices;
		float DurationTicks = 1.0f;
	};

	// Animation keyframe for the position attributes of a Mesh2D.
	// DurationTicks is the number of update ticks the frame should persist and may be fractional to express an average duration.
	struct Mesh2DPositionKeyframe
	{
		Mesh2DPositionKeyframe() = default;

		Mesh2DPositionKeyframe(const std::vector<Vec2f>& positions, float durationTicks) :
			Positions(positions),
			DurationTicks(durationTicks)
		{}

		std::vector<Vec2f> Positions;
		float DurationTicks = 1.0f;
	};

	// Animation keyframe for the color attributes of a Mesh2D or Mesh3D.
	// DurationTicks is the number of update ticks the frame should persist and may be fractional to express an average duration.
	struct MeshColorKeyframe
	{
		MeshColorKeyframe() = default;

		MeshColorKeyframe(const std::vector<SDL_Color>& colors, float durationTicks) :
			Colors(colors),
			DurationTicks(durationTicks)
		{}

		std::vector<SDL_Color> Colors;
		float DurationTicks = 1.0f;
	};

	// Animation keyframe for the UV attributes of a Mesh2D or Mesh3D.
	// DurationTicks is the number of update ticks the frame should persist and may be fractional to express an average duration.
	struct MeshUVKeyframe
	{
		MeshUVKeyframe() = default;

		MeshUVKeyframe(const std::vector<Vec2f>& uvs, float durationTicks) :
			UVs(uvs),
			DurationTicks(durationTicks)
		{}

		std::vector<Vec2f> UVs;
		float DurationTicks = 1.0f;
	};

	// Animation keyframe for the normal attributes of a Mesh2D or Mesh3D.
	// DurationTicks is the number of update ticks the frame should persist and may be fractional to express an average duration.
	struct MeshNormalKeyframe
	{
		MeshNormalKeyframe() = default;

		MeshNormalKeyframe(const std::vector<Vec3f>& normals, float durationTicks) :
			Normals(normals),
			DurationTicks(durationTicks)
		{}

		std::vector<Vec3f> Normals;
		float DurationTicks = 1.0f;
	};

	// Animation keyframe for the complete vertices of a Mesh3D.
	// DurationTicks is the number of update ticks the frame should persist and may be fractional to express an average duration.
	struct Mesh3DKeyframe
	{
		Mesh3DKeyframe() = default;

		Mesh3DKeyframe(const std::vector<MeshVertex3D>& vertices, float durationTicks) :
			Vertices(vertices),
			DurationTicks(durationTicks)
		{}

		std::vector<MeshVertex3D> Vertices;
		float DurationTicks = 1.0f;
	};

	// Animation keyframe for the position attributes of a Mesh3D.
	// DurationTicks is the number of update ticks the frame should persist and may be fractional to express an average duration.
	struct Mesh3DPositionKeyframe
	{
		Mesh3DPositionKeyframe() = default;

		Mesh3DPositionKeyframe(const std::vector<Vec3f>& positions, float durationTicks) :
			Positions(positions),
			DurationTicks(durationTicks)
		{}

		std::vector<Vec3f> Positions;
		float DurationTicks = 1.0f;
	};



	// MeshAnimator iterates over a keyframe sequence and optionally applies its current keyframe state to a target mesh.
	// The concrete application behavior is defined by UpdateMesh() specializations.
	// It runs the animation from a given start position to the end of the sequence, and then loops to the first keyframe of the sequence if looping is enabled. 
	// DurationTicks is expected to be positive, but is processed consistently by a tick accumulator for all values.
	// Keyframes with DurationTicks <= 0.0f are advanced past immediately when another frame is available.
	// The amount of keyframe advancements per Update() is capped to the sequence size.
	// 
	// MeshAnimator measures time in update ticks, so it is intended for fixed-step update loops.
	// For variable-step updates, adjust the speed scale dynamically based on delta time to match the desired timing.
	// 
	// Elapsed ticks are incremented by the speed scale (1.0f by default) each update tick and consumed by advancing the animation. 
	// As a result, they track the unprocessed time since the current keyframe was shown.
	// elapsedTicks_ starts at -speedScale_ so that the initially selected keyframe is presented once before its duration starts being consumed.
	// Negative elapsed ticks can be used to delay the animation by the respective amount.
	// Restart() applies the selected start frame immediately and resets elapsedTicks_ to 0.0f.
	// When restarting a non-looped animation after it completes, call Restart() after Update()/IsFinished() during the same frame for the most seamless timing.
	// 
	// Philosophy:
	// MeshAnimator offers "flipbook" animations for a mesh.
	// Different specializations of MeshAnimator animate complete vertices or individual attributes (positions, colors, UVs, normals) to keep animations modular and composable.
	// MeshAnimator does not own the keyframe sequence and the target mesh; their validity is the responsibility of the caller.
	//
	// Full-vertex SpriteMesh keyframes replace all four SpriteMesh vertices.
	// Full-vertex Mesh2D/Mesh3D keyframes replace the target mesh's complete vertex array, which means they may change the mesh's vertex count.
	// 
	// Attribute-only Mesh2D/Mesh3D keyframes are applied by vertex index using a best-effort prefix policy:
	// if the keyframe contains fewer attributes than the mesh has vertices, only that prefix is updated;
	// if the keyframe contains more attributes than the mesh has vertices, excess keyframe data is ignored.
	// Uncovered mesh vertices keep their previous attribute values.
	// SpriteMesh attribute keyframes always update the fixed four SpriteMesh vertices.
	template<typename KeyframeType, typename TargetMeshType> class MeshAnimator
	{
	public:

		//###################################### INITIALIZATION ###################################

		// Creates the MeshAnimator with the respective parameters.
		// elapsedTicks_ is initialized to -speedScale_ so that the initially selected
		// keyframe is presented once before its duration starts being consumed.
		explicit MeshAnimator(const std::vector<KeyframeType>* frameSequence = nullptr, TargetMeshType* targetMesh = nullptr, bool isLooped = true, int currentFrameIndex = 0, float speedScale = 1.0f) :
			frameSequence_(frameSequence),
			targetMesh_(targetMesh),
			isLooped_(isLooped),
			currentFrameIndex_(0)
		{
			SetCurrentFrameIndex(currentFrameIndex);
			SetSpeedScale(speedScale);
			elapsedTicks_ = -speedScale_;
		}

		~MeshAnimator() = default;

		//###################################### FUNCTIONALITY ###################################


		// Advances the animation state for the current update tick.
		// If a target mesh is assigned, applies the resulting current keyframe state to it.
		// Returns false if there is no valid sequence, the animation was already finished before this update, or applying the current frame to the target mesh fails.
		// Returns true if the animator processed the update tick and, if a target mesh is assigned, the current frame was applied successfully.	
		bool Update()
		{
			if (IsFinished()) return false;

			elapsedTicks_ += speedScale_;

			const int sequenceSize = frameSequence_->size();
			const KeyframeType* frame = &((*frameSequence_)[currentFrameIndex_]);

			int iterationCount = 0;
			while (elapsedTicks_ >= frame->DurationTicks && (isLooped_ || currentFrameIndex_ < (sequenceSize - 1)))
			{
				elapsedTicks_ -= frame->DurationTicks;
				currentFrameIndex_ = (currentFrameIndex_ + 1) % sequenceSize;
				frame = &((*frameSequence_)[currentFrameIndex_]);

				// Prevents infinite loops for pathological duration values
				if (++iterationCount >= sequenceSize)
				{
					elapsedTicks_ = 0.0f;
					break;
				}
			}

			if (targetMesh_) return UpdateMesh(*targetMesh_);

			return true;
		}

		// Applies the current keyframe state to the mesh.
		// Concrete behavior is provided by explicit specializations.
		// Full-vertex mesh keyframes may replace the complete vertex array.
		// Attribute keyframes may update only selected vertex attributes.
		// The generic implementation returns false.
		bool UpdateMesh(TargetMeshType& mesh) const
		{
			return false;
		}

		// Resets the animation state to the given keyframe index.
		// If a target mesh is assigned, applies the selected start keyframe state immediately.
		// Returns false immediately without side effects if startFrameIndex is invalid.
		// Otherwise, elapsedTicks_ is set to 0.0f.
		// Returns false if a target mesh is assigned and applying the frame fails, returns true otherwise.
		// When restarting a non-looped animation after it completes, call Restart() after Update()/IsFinished()
		// during the same frame for the most seamless timing.
		bool Restart(int startFrameIndex = 0)
		{
			if (!SetCurrentFrameIndex(startFrameIndex)) return false;

			elapsedTicks_ = 0.0f;

			if (targetMesh_) return UpdateMesh(*targetMesh_);

			return true;
		}

		void SetElapsedTicks(float elapsedTicks)
		{
			elapsedTicks_ = elapsedTicks;
		}

		// Sets the current frame index.
		// Returns false without side effects if index is invalid or no valid sequence is assigned.
		bool SetCurrentFrameIndex(int index)
		{
			if (!frameSequence_ || index < 0 || index >= frameSequence_->size()) return false;

			currentFrameIndex_ = index;

			return true;
		}


		void SetIsLooped(bool isLooped)
		{
			isLooped_ = isLooped;
		}

		// Sets the speed scale by which elapsed ticks are incremented with each Update() call.
		// speedScale is clamped to non-negative values.
		// A speedScale of 0.0f pauses the animation.
		void SetSpeedScale(float speedScale)
		{
			speedScale_ = speedScale < 0.0f ? 0.0f : speedScale;
		}

		// Sets or switches the keyframe sequence without applying a frame to the target mesh.
		// currentFrameIndex is clamped to the valid range, and elapsedTicks is preserved.
		// This can enable seamless animation flow when switching to a related frame sequence on the fly.
		void SetFrameSequence(const std::vector<KeyframeType>* sequence)
		{
			frameSequence_ = sequence;

			if (!frameSequence_ || frameSequence_->empty())
			{
				currentFrameIndex_ = 0;
				return;
			}

			if (currentFrameIndex_ >= frameSequence_->size())
				currentFrameIndex_ = frameSequence_->size() - 1;
		}

		void SetTargetMesh(TargetMeshType* targetMesh)
		{
			targetMesh_ = targetMesh;
		}


		//###################################################################################### GETTERS ############################################################


		// Checks if the animation is finished or cannot run.
		// Call after Update() during the same frame for earliest possible timing.
		// Returns true if the sequence is null/empty, the current frame index is no longer valid, or looping is disabled and the last frame has completed.
		bool IsFinished() const
		{
			if (!frameSequence_ || currentFrameIndex_ >= frameSequence_->size()) return true;

			return !isLooped_ && currentFrameIndex_ + 1 == frameSequence_->size() && elapsedTicks_ >= (*frameSequence_)[currentFrameIndex_].DurationTicks;
		}

		float GetElapsedTicks() const
		{
			return elapsedTicks_;
		}

		int GetCurrentFrameIndex() const
		{
			return currentFrameIndex_;
		}

		bool GetIsLooped() const
		{
			return isLooped_;
		}

		float GetSpeedScale() const
		{
			return speedScale_;
		}

		// Returns nullptr on failure
		const KeyframeType* GetCurrentFrame() const
		{
			if (!frameSequence_ || currentFrameIndex_ >= frameSequence_->size()) return nullptr;

			return &((*frameSequence_)[currentFrameIndex_]);
		}

		const std::vector<KeyframeType>* GetFrameSequence() const
		{
			return frameSequence_;
		}

		TargetMeshType* GetTargetMesh() const
		{
			return targetMesh_;
		}

	private:

		static int GetMinSize(size_t a, size_t b) { return a < b ? a : b; }

		const std::vector<KeyframeType>* frameSequence_;
		TargetMeshType* targetMesh_;

		bool isLooped_;
		int currentFrameIndex_;
		float speedScale_;
		float elapsedTicks_;
	};

	using SpriteMeshAnimator = MeshAnimator<SpriteMeshKeyframe, SpriteMesh>;
	using SpriteMeshPositionAnimator = MeshAnimator<SpriteMeshPositionKeyframe, SpriteMesh>;
	using SpriteMeshColorAnimator = MeshAnimator<SpriteMeshColorKeyframe, SpriteMesh>;
	using SpriteMeshUVAnimator = MeshAnimator<SpriteMeshUVKeyframe, SpriteMesh>;

	using Mesh2DAnimator = MeshAnimator<Mesh2DKeyframe, Mesh2D>;
	using Mesh2DPositionAnimator = MeshAnimator<Mesh2DPositionKeyframe, Mesh2D>;
	using Mesh2DColorAnimator = MeshAnimator<MeshColorKeyframe, Mesh2D>;
	using Mesh2DUVAnimator = MeshAnimator<MeshUVKeyframe, Mesh2D>;
	using Mesh2DNormalAnimator = MeshAnimator<MeshNormalKeyframe, Mesh2D>;

	using Mesh3DAnimator = MeshAnimator<Mesh3DKeyframe, Mesh3D>;
	using Mesh3DPositionAnimator = MeshAnimator<Mesh3DPositionKeyframe, Mesh3D>;
	using Mesh3DColorAnimator = MeshAnimator<MeshColorKeyframe, Mesh3D>;
	using Mesh3DUVAnimator = MeshAnimator<MeshUVKeyframe, Mesh3D>;
	using Mesh3DNormalAnimator = MeshAnimator<MeshNormalKeyframe, Mesh3D>;

	template<>
	inline bool SpriteMeshAnimator::UpdateMesh(SpriteMesh& mesh) const
	{
		if (!frameSequence_ || currentFrameIndex_ >= frameSequence_->size()) return false;

		const SpriteMeshKeyframe& currentFrame = (*frameSequence_)[currentFrameIndex_];

		for (int i = 0; i < SpriteMesh::VERTEX_COUNT; i++)
			mesh.Vertices[i] = currentFrame.Vertices[i];

		return true;
	}

	template<>
	inline bool SpriteMeshPositionAnimator::UpdateMesh(SpriteMesh& mesh) const
	{
		if (!frameSequence_ || currentFrameIndex_ >= frameSequence_->size()) return false;

		const SpriteMeshPositionKeyframe& currentFrame = (*frameSequence_)[currentFrameIndex_];

		for (int i = 0; i < SpriteMesh::VERTEX_COUNT; i++)
			mesh.Vertices[i].Position = currentFrame.Positions[i];

		return true;
	}

	template<>
	inline bool SpriteMeshColorAnimator::UpdateMesh(SpriteMesh& mesh) const
	{
		if (!frameSequence_ || currentFrameIndex_ >= frameSequence_->size()) return false;

		const SpriteMeshColorKeyframe& currentFrame = (*frameSequence_)[currentFrameIndex_];

		for (int i = 0; i < SpriteMesh::VERTEX_COUNT; i++)
			mesh.Vertices[i].Color = currentFrame.Colors[i];

		return true;
	}

	template<>
	inline bool SpriteMeshUVAnimator::UpdateMesh(SpriteMesh& mesh) const
	{
		if (!frameSequence_ || currentFrameIndex_ >= frameSequence_->size()) return false;

		const SpriteMeshUVKeyframe& currentFrame = (*frameSequence_)[currentFrameIndex_];

		for (int i = 0; i < SpriteMesh::VERTEX_COUNT; i++)
			mesh.Vertices[i].UV = currentFrame.UVs[i];

		return true;
	}

	template<>
	inline bool Mesh2DAnimator::UpdateMesh(Mesh2D& mesh) const
	{
		if (!frameSequence_ || currentFrameIndex_ >= frameSequence_->size()) return false;

		const Mesh2DKeyframe& currentFrame = (*frameSequence_)[currentFrameIndex_];

		mesh.Vertices = currentFrame.Vertices;

		return true;
	}

	template<>
	inline bool Mesh2DPositionAnimator::UpdateMesh(Mesh2D& mesh) const
	{
		if (!frameSequence_ || currentFrameIndex_ >= frameSequence_->size()) return false;

		const Mesh2DPositionKeyframe& currentFrame = (*frameSequence_)[currentFrameIndex_];

		const int positionsSize = GetMinSize(currentFrame.Positions.size(), mesh.Vertices.size());

		for (int i = 0; i < positionsSize; i++)
			mesh.Vertices[i].Position = currentFrame.Positions[i];

		return true;
	}

	template<>
	inline bool Mesh2DColorAnimator::UpdateMesh(Mesh2D& mesh) const
	{
		if (!frameSequence_ || currentFrameIndex_ >= frameSequence_->size()) return false;

		const MeshColorKeyframe& currentFrame = (*frameSequence_)[currentFrameIndex_];

		const int colorsSize = GetMinSize(currentFrame.Colors.size(), mesh.Vertices.size());

		for (int i = 0; i < colorsSize; i++)
			mesh.Vertices[i].Color = currentFrame.Colors[i];

		return true;
	}

	template<>
	inline bool Mesh2DUVAnimator::UpdateMesh(Mesh2D& mesh) const
	{
		if (!frameSequence_ || currentFrameIndex_ >= frameSequence_->size()) return false;

		const MeshUVKeyframe& currentFrame = (*frameSequence_)[currentFrameIndex_];

		const int uvsSize = GetMinSize(currentFrame.UVs.size(), mesh.Vertices.size());

		for (int i = 0; i < uvsSize; i++)
			mesh.Vertices[i].UV = currentFrame.UVs[i];

		return true;
	}

	template<>
	inline bool Mesh2DNormalAnimator::UpdateMesh(Mesh2D& mesh) const
	{
		if (!frameSequence_ || currentFrameIndex_ >= frameSequence_->size()) return false;

		const MeshNormalKeyframe& currentFrame = (*frameSequence_)[currentFrameIndex_];

		const int normalsSize = GetMinSize(currentFrame.Normals.size(), mesh.Vertices.size());

		for (int i = 0; i < normalsSize; i++)
			mesh.Vertices[i].Normal = currentFrame.Normals[i];

		return true;
	}

	template<>
	inline bool Mesh3DAnimator::UpdateMesh(Mesh3D& mesh) const
	{
		if (!frameSequence_ || currentFrameIndex_ >= frameSequence_->size()) return false;

		const Mesh3DKeyframe& currentFrame = (*frameSequence_)[currentFrameIndex_];

		mesh.Vertices = currentFrame.Vertices;

		return true;
	}

	template<>
	inline bool Mesh3DPositionAnimator::UpdateMesh(Mesh3D& mesh) const
	{
		if (!frameSequence_ || currentFrameIndex_ >= frameSequence_->size()) return false;

		const Mesh3DPositionKeyframe& currentFrame = (*frameSequence_)[currentFrameIndex_];

		const int positionsSize = GetMinSize(currentFrame.Positions.size(), mesh.Vertices.size());

		for (int i = 0; i < positionsSize; i++)
			mesh.Vertices[i].Position = currentFrame.Positions[i];

		return true;
	}

	template<>
	inline bool Mesh3DColorAnimator::UpdateMesh(Mesh3D& mesh) const
	{
		if (!frameSequence_ || currentFrameIndex_ >= frameSequence_->size()) return false;

		const MeshColorKeyframe& currentFrame = (*frameSequence_)[currentFrameIndex_];

		const int colorsSize = GetMinSize(currentFrame.Colors.size(), mesh.Vertices.size());

		for (int i = 0; i < colorsSize; i++)
			mesh.Vertices[i].Color = currentFrame.Colors[i];

		return true;
	}

	template<>
	inline bool Mesh3DUVAnimator::UpdateMesh(Mesh3D& mesh) const
	{
		if (!frameSequence_ || currentFrameIndex_ >= frameSequence_->size()) return false;

		const MeshUVKeyframe& currentFrame = (*frameSequence_)[currentFrameIndex_];

		const int uvsSize = GetMinSize(currentFrame.UVs.size(), mesh.Vertices.size());

		for (int i = 0; i < uvsSize; i++)
			mesh.Vertices[i].UV = currentFrame.UVs[i];

		return true;
	}

	template<>
	inline bool Mesh3DNormalAnimator::UpdateMesh(Mesh3D& mesh) const
	{
		if (!frameSequence_ || currentFrameIndex_ >= frameSequence_->size()) return false;

		const MeshNormalKeyframe& currentFrame = (*frameSequence_)[currentFrameIndex_];

		const int normalsSize = GetMinSize(currentFrame.Normals.size(), mesh.Vertices.size());

		for (int i = 0; i < normalsSize; i++)
			mesh.Vertices[i].Normal = currentFrame.Normals[i];

		return true;
	}

}

