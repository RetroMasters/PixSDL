#pragma once

#include <vector>
#include "MovableObject2D.h"
#include "Mesh2D.h"

namespace pix
{
	// MeshSprite2DNode is functionally a MeshSprite2D that can participate in a transform hierarchy.
	// - Mesh, parent, and child pointers are non-owning and managed externally.
	// - On destruction, the node detaches from its parent and re-roots its children in world space.
	// 
	// NOTE: Changing a child's parent or deleting a child while iterating over GetChildren() shrinks the child list. 
	// Iterate from back to front, use while (!GetChildren().empty()), or snapshot the child pointer list first.
	// 
	// Philosophy:
	// MeshSprite2DNode is the most complete foundation for a hierarchical renderable 2D entity that can move through space.
	// It does not own the parent or children, leaving ownership decisions to the caller to preserve flexibility.
	class MeshSprite2DNode : public MovableObject2D
	{
	public:

		MeshSprite2DNode() = default;
		MeshSprite2DNode(const Mesh2D* mesh, const Transform2D& transform);
		MeshSprite2DNode(const Mesh2D* mesh, const Transform2D& transform, const Transform2D& prevTransform);
		explicit MeshSprite2DNode(const Mesh2D* mesh, const Vec2& position = Vec2(0.0, 0.0), const Vec2f& scale = Vec2f(1.0f, 1.0f), const Rotation2D& rotation = Rotation2D());

		// Prevent copying 
		MeshSprite2DNode(const MeshSprite2DNode&) = delete;
		MeshSprite2DNode& operator= (const MeshSprite2DNode&) = delete;

		// On destruction, parent and children become detached. The children become roots in world space,
		// and this node is removed from its parent's child list.
		~MeshSprite2DNode() override;

		// Sets a new parent with proper attach/detach management.
		// newParent may be nullptr to make this node a root with Transform in world space.
		// The current and previous world transforms are preserved, so render interpolation remains stable after reparenting.
		// If newParent is the current parent, this node, or a descendant of it, the parent won't change.
		// Returns true if parent is already set or has changed to newParent, false otherwise.
		// 
		// Note:
		// Exact transform preservation assumes the resulting local transform can be represented by Transform2D
		// without shear. Avoid rotated non-uniform scale combinations in the hierarchy if exact results are required.
		bool SetParent(MeshSprite2DNode* newParent);

		MeshSprite2DNode* GetParent() const;

		const std::vector<MeshSprite2DNode*>& GetChildren() const;

		// Returns the effective decomposed transform in world space. 
		// Correct result is only guaranteed with no rotated non-uniform scaling in the ancestor chain (rotated uniform nonzero scale is safe). 
		Transform2D GetGlobalTransform() const;

		// Returns the previous effective decomposed transform in world space.
		// Same limitations as GetGlobalTransform().
		Transform2D GetPrevGlobalTransform() const;



		const Mesh2D* Mesh = nullptr;

	private:

		MeshSprite2DNode* parent_ = nullptr;
		std::vector<MeshSprite2DNode*> children_;

	};

}

