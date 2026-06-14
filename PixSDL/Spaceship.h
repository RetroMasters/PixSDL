#pragma once

#include <vector>
#include "Sprite2DNode.h"
#include "MeshAnimator.h"
#include "ObjectInput.h"
#include "SpriteMeshRenderer2D.h"

namespace pix
{
	// Simple demo class to show how a more complex object can be composed from simpler parts.
	// It is not complete, just showing a possible structure.
	class Spaceship : public Sprite2DNode
	{
	public:

		Spaceship() = default;

		// input must provide the virtual axes "Up", "Down", "Left", and "Right".
		void Reinit(const Vec2f& shipMeshSize, const Vec2f& effectMeshSize, int effectFrameCount, float effectDurationTicks, const ObjectInput& input);

		void Update();

		void RenderBody(SpriteMeshRenderer2D& renderer);
		void RenderEffect(SpriteMeshRenderer2D& renderer);

	private:

		void UpdateMovement();

		Sprite2DNode shipBodyNode_;
		Sprite2DNode jetEffectNode_;
		SpriteMeshUVAnimator jetEffectAnimator_;
		std::vector<SpriteMeshUVKeyframe> jetEffectSequence_;

		ObjectInput input_;

		SpriteMesh shipBodyMesh_; // owned mesh
		SpriteMesh jetEffectMesh_; // owned mesh
	};
}
