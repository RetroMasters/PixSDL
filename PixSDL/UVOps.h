#pragma once

#include <SDL_rect.h>
#include "UV.h"

namespace pix
{

	// ########################################### UVRECT OPERATIONS #################################

	// Returns a UVRect from the texture region provided in pixels by a SDL_Rect.
	// texWidth/texHeight are used for normalization and must match the texture size in pixels.
	// GetUVRect() only converts rect into UVRect format without validating it,
	// so negative width or height will invert the resulting corner relationship.
	// If one of the texSize dimensions is zero or smaller, a zero-initialized UVRect is returned.
	UVRect GetUVRect(int texWidth, int texHeight, const SDL_Rect& rect);

	// Returns the width and the height of the rect's axis-aligned bounding box
	Vec2f GetBoundsSize(const UVRect& rect);

	// Returns the minimum point of the axis-aligned bounding box of the UVRect
	Vec2f GetMinBounds(const UVRect& rect);

	// Returns the maximum point of the axis-aligned bounding box of the UVRect
	Vec2f GetMaxBounds(const UVRect& rect);

	// Returns true if rect lies fully inside a texture of size (texWidth, texHeight).
	// Returns false if texture dimensions are non-positive, or if rect has negative position or negative width/height.
	// Zero width or height is allowed.
	bool IsInsideTexBounds(int texWidth, int texHeight, const SDL_Rect& rect);

	// Returns true if rect lies fully inside the normalized texture area [0, 1].
	// Flipped rect corners are allowed. Zero width or height is allowed.
	bool IsInsideTexBounds(const UVRect& rect);

	// Flips rect's X-coordinates in-place
	void FlipX(UVRect& rect);

	// Flips rect's Y-coordinates in-place
	void FlipY(UVRect& rect);

	// Flips rect's X- and Y-coordinates in-place
	void FlipXY(UVRect& rect);

	// Returns a copy of rect with flipped X-coordinates
	UVRect GetFlippedX(const UVRect& rect);

	// Returns a copy of rect with flipped Y-coordinates
	UVRect GetFlippedY(const UVRect& rect);

	// Returns a copy of rect with flipped XY-coordinates
	UVRect GetFlippedXY(const UVRect& rect);


	// ########################################### UVQUAD OPERATIONS #################################

	// Returns a UVQuad from a UVRect
	UVQuad GetUVQuad(const UVRect& rect);

	// Returns a UVQuad from the texture region provided in pixels by a SDL_Rect.
	// texWidth/texHeight are used for normalization and must match the texture size in pixels.
	UVQuad GetUVQuad(int texWidth, int texHeight, const SDL_Rect& rect);

	// Returns the width and the height of the quad's axis-aligned bounding box
	Vec2f GetBoundsSize(const UVQuad& quad);

	// Returns the minimum point of the axis-aligned bounding box of the UVQuad
	Vec2f GetMinBounds(const UVQuad& quad);

	// Returns the maximum point of the axis-aligned bounding box of the UVQuad
	Vec2f GetMaxBounds(const UVQuad& quad);

	// Flips quad's X-coordinates in-place
	void FlipX(UVQuad& quad);

	// Flips quad's Y-coordinates in-place
	void FlipY(UVQuad& quad);

	// Flips quad's X- and Y-coordinates in-place
	void FlipXY(UVQuad& quad);

	// Returns a copy of quad with flipped X-coordinates
	UVQuad GetFlippedX(const UVQuad& quad);

	// Returns a copy of quad with flipped Y-coordinates
	UVQuad GetFlippedY(const UVQuad& quad);

	// Returns a copy of quad with flipped XY-coordinates
	UVQuad GetFlippedXY(const UVQuad& quad);
}