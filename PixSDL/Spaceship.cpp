#include "Spaceship.h"
#include "SpriteMeshOps.h"
#include "MeshAnimatorOps.h"

namespace pix
{
	void Spaceship::Reinit(const Vec2f& shipMeshSize, const Vec2f& effectMeshSize, int effectFrameCount, float effectDurationTicks, const ObjectInput& input)
	{
		if (effectFrameCount < 1) effectFrameCount = 1;

		input_ = input;

		shipBodyMesh_ = CreateSpriteMesh(Vec2f(-shipMeshSize.X / 2.0f, shipMeshSize.Y / 2.0f), Vec2f(shipMeshSize.X / 2.0f, -shipMeshSize.Y / 2.0f));

		UVRect startRect(0.0f, 0.0f, 1.0f / effectFrameCount, 1.0f);
		jetEffectMesh_ = CreateSpriteMesh(Vec2f(-effectMeshSize.X / 2.0f, effectMeshSize.Y / 2.0f), Vec2f(effectMeshSize.X / 2.0f, -effectMeshSize.Y / 2.0f), startRect);

		jetEffectSequence_ = CreateSpriteMeshUVKeyframeSequence(startRect, effectFrameCount, effectDurationTicks);

		jetEffectAnimator_.SetFrameSequence(&jetEffectSequence_);
        jetEffectAnimator_.SetTargetMesh(&jetEffectMesh_);

		shipBodyNode_.Mesh = &shipBodyMesh_;
		jetEffectNode_.Mesh = &jetEffectMesh_;
		shipBodyNode_.SetParent(this);
		jetEffectNode_.SetParent(this);

	    jetEffectNode_.Transform.Position = Vec2(-shipMeshSize.X / 2.0f - effectMeshSize.X / 2.0f, -0.1f * shipMeshSize.Y);
	}

	void Spaceship::Update()
	{
		BeginUpdate();

		for (Sprite2DNode* childNode : GetChildren())
			childNode->BeginUpdate();

		input_.Update();

		UpdateMovement();

		jetEffectAnimator_.Update();
	}

	void Spaceship::RenderBody(SpriteMeshRenderer2D& renderer)
	{
		renderer.Render(shipBodyNode_);
	}

	void Spaceship::RenderEffect(SpriteMeshRenderer2D& renderer)
	{
		renderer.Render(jetEffectNode_);
	}

	void Spaceship::UpdateMovement()
	{
		double speed = 10.0;
		Vec2 velocity = Vec2(0.0, 0.0);

		if (input_.IsPositive("Up"))
			velocity.Y += speed;
		if (input_.IsPositive("Down"))
			velocity.Y -= speed;
		if (input_.IsPositive("Left"))
			velocity.X -= speed;
		if (input_.IsPositive("Right"))
			velocity.X += speed;

		Transform.Position += velocity;
	}

}
