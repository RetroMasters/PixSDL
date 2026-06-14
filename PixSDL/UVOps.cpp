#include "UVOps.h"
#include "GeometryUtils2D.h"

namespace pix
{
	// ########################################### UVRECT OPERATIONS #################################


	UVRect GetUVRect(int texWidth, int texHeight, const SDL_Rect& rect)
	{
		if (texWidth <= 0 || texHeight <= 0)
			return UVRect(0.0f, 0.0f, 0.0f, 0.0f);

		const Vec2f topLeft((float)rect.x, (float)rect.y);
		const Vec2f bottomRight((float)(rect.x + rect.w), (float)(rect.y + rect.h));
		const Vec2f texSize((float)texWidth, (float)texHeight);

		return UVRect(topLeft / texSize, bottomRight / texSize);
	}

	Vec2f GetBoundsSize(const UVRect& rect)
	{
		return Vec2f(rect.GetWidth(), rect.GetHeight());
	}

	Vec2f GetMinBounds(const UVRect& rect)
	{
		float minX = rect.TopLeft.X < rect.BottomRight.X ? rect.TopLeft.X : rect.BottomRight.X;
		float minY = rect.TopLeft.Y < rect.BottomRight.Y ? rect.TopLeft.Y : rect.BottomRight.Y;

		return Vec2f(minX, minY);
	}

	Vec2f GetMaxBounds(const UVRect& rect)
	{
		float maxX = rect.TopLeft.X < rect.BottomRight.X ? rect.BottomRight.X : rect.TopLeft.X;
		float maxY = rect.TopLeft.Y < rect.BottomRight.Y ? rect.BottomRight.Y : rect.TopLeft.Y;

		return Vec2f(maxX, maxY);
	}

	bool IsInsideTexBounds(int texWidth, int texHeight, const SDL_Rect& rect)
	{
		return (texWidth > 0 && texHeight > 0 && rect.w >= 0 && rect.h >= 0 &&
			rect.x >= 0 && rect.y >= 0 && rect.w <= texWidth - rect.x && rect.h <= texHeight - rect.y);
	}

	bool IsInsideTexBounds(const UVRect& rect)
	{
		return rect.TopLeft.X >= 0.0f && rect.BottomRight.X >= 0.0f && rect.TopLeft.X <= 1.0f && rect.BottomRight.X <= 1.0f &&
			rect.TopLeft.Y >= 0.0f && rect.BottomRight.Y >= 0.0f && rect.TopLeft.Y <= 1.0f && rect.BottomRight.Y <= 1.0f;
	}

	void FlipX(UVRect& rect)
	{
		const float topLeftX = rect.TopLeft.X;

		rect.TopLeft.X = rect.BottomRight.X;
		rect.BottomRight.X = topLeftX;
	}

	void FlipY(UVRect& rect)
	{
		const float topLeftY = rect.TopLeft.Y;

		rect.TopLeft.Y = rect.BottomRight.Y;
		rect.BottomRight.Y = topLeftY;
	}

	void FlipXY(UVRect& rect)
	{
		FlipX(rect);
		FlipY(rect);
	}

	UVRect GetFlippedX(const UVRect& rect)
	{
		return UVRect(rect.GetTopRight(), rect.GetBottomLeft());
	}

	UVRect GetFlippedY(const UVRect& rect)
	{
		return UVRect(rect.GetBottomLeft(), rect.GetTopRight());
	}

	UVRect GetFlippedXY(const UVRect& rect)
	{
		return UVRect(rect.BottomRight, rect.TopLeft);
	}


	// ########################################### UVQUAD OPERATIONS #################################


	UVQuad GetUVQuad(const UVRect& rect)
	{
		return UVQuad(rect.TopLeft, rect.GetTopRight(), rect.BottomRight, rect.GetBottomLeft());
	}

	UVQuad GetUVQuad(int texWidth, int texHeight, const SDL_Rect& rect)
	{
		return GetUVQuad(GetUVRect(texWidth, texHeight, rect));
	}

	Vec2f GetBoundsSize(const UVQuad& quad)
	{
		const Vec2f* const uvs = quad.UVs;

		return GetBoundsSize2D(uvs, UVQuad::UV_COUNT);
	}

	Vec2f GetMinBounds(const UVQuad& quad)
	{
		const Vec2f* const uvs = quad.UVs;

		return GetMinBounds2D(uvs, UVQuad::UV_COUNT);
	}

	Vec2f GetMaxBounds(const UVQuad& quad)
	{
		const Vec2f* const uvs = quad.UVs;

		return GetMaxBounds2D(uvs, UVQuad::UV_COUNT);
	}

	void FlipX(UVQuad& quad)
	{
		float tempX = quad.TopLeft().X;

		quad.TopLeft().X = quad.TopRight().X;
		quad.TopRight().X = tempX;

		tempX = quad.BottomLeft().X;

		quad.BottomLeft().X = quad.BottomRight().X;
		quad.BottomRight().X = tempX;
	}

	void FlipY(UVQuad& quad)
	{
		float tempY = quad.TopLeft().Y;

		quad.TopLeft().Y = quad.BottomLeft().Y;
		quad.BottomLeft().Y = tempY;

		tempY = quad.TopRight().Y;

		quad.TopRight().Y = quad.BottomRight().Y;
		quad.BottomRight().Y = tempY;
	}

	void FlipXY(UVQuad& quad)
	{
		FlipX(quad);
		FlipY(quad);
	}

	UVQuad GetFlippedX(const UVQuad& quad)
	{
		return UVQuad(quad.TopRight(), quad.TopLeft(), quad.BottomLeft(), quad.BottomRight());
	}

	UVQuad GetFlippedY(const UVQuad& quad)
	{
		return UVQuad(quad.BottomLeft(), quad.BottomRight(), quad.TopRight(), quad.TopLeft());
	}

	UVQuad GetFlippedXY(const UVQuad& quad)
	{
		return UVQuad(quad.BottomRight(), quad.BottomLeft(), quad.TopLeft(), quad.TopRight());
	}
}