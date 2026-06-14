#pragma once

#include <cmath>
#include "PixMath.h"

// UV space used by PixSDLib: (0.0f, 0.0f) is top-left, (1.0f, 1.0f) is bottom-right.
// This matches SDL's top-left texture and render-target convention to avoid friction.
// UVQuad and UVRect do not enforce ordering or clamping. Callers are responsible for valid ranges/order if required.
namespace pix
{

	// UVQuad represents four explicit texture coordinate points.
	// Y increases downward.
	//
	// Philosophy:
	// UVQuad enables UV mapping for SpriteMesh that is not necessarily rectangular.
	// Intended corner order in the array (though coordinates may be flipped): 0 = TopLeft, 1 = TopRight, 2 = BottomRight, 3 = BottomLeft.
	struct UVQuad
	{
		static constexpr int UV_COUNT = 4;

		UVQuad() = default;

		UVQuad(Vec2f topLeft, Vec2f topRight, Vec2f bottomRight, Vec2f bottomLeft) :
			UVs{ topLeft, topRight, bottomRight, bottomLeft }
		{}

		Vec2f& TopLeft() { return UVs[0]; }
		Vec2f& TopRight() { return UVs[1]; }
		Vec2f& BottomRight() { return UVs[2]; }
		Vec2f& BottomLeft() { return UVs[3]; }

		const Vec2f& TopLeft()     const { return UVs[0]; }
		const Vec2f& TopRight()    const { return UVs[1]; }
		const Vec2f& BottomRight() const { return UVs[2]; }
		const Vec2f& BottomLeft()  const { return UVs[3]; }

		Vec2f UVs[UV_COUNT] = { {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f} };
	};


	// UVRect defines a rectangular texture coordinate region by two points: TopLeft and BottomRight.
	// Y increases downward.
	// Coordinates are not validated, clamped, or reordered; flipped regions are preserved as provided.
	//
	// Philosophy:
	// UVRect represents a rectangular UV region by two corner points.
	// This avoids width/height conversions and makes flipped texture regions easy to express.
	struct UVRect
	{
		UVRect() = default;

		UVRect(Vec2f topLeft, Vec2f bottomRight) :
			TopLeft(topLeft),
			BottomRight(bottomRight)
		{}

		UVRect(float left, float top, float right, float bottom) :
			TopLeft(left, top),
			BottomRight(right, bottom)
		{}

		// Moves TopLeft to position while preserving signed width and height
		void SetPosition(const Vec2f& position)
		{
			BottomRight = position + Vec2f(GetSignedWidth(), GetSignedHeight());
			TopLeft = position;
		}

		float GetWidth() const { return std::abs(BottomRight.X - TopLeft.X); }
		float GetHeight() const { return std::abs(BottomRight.Y - TopLeft.Y); }

		float GetSignedWidth() const { return BottomRight.X - TopLeft.X; }
		float GetSignedHeight() const { return BottomRight.Y - TopLeft.Y; }

		Vec2f GetTopRight() const { return Vec2f(BottomRight.X, TopLeft.Y); }
		Vec2f GetBottomLeft() const { return Vec2f(TopLeft.X, BottomRight.Y); }

		Vec2f TopLeft = Vec2f(0.0f, 0.0f);
		Vec2f BottomRight = Vec2f(1.0f, 1.0f);
	};

}
