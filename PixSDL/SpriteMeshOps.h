#pragma once

#include <SDL_pixels.h>
#include "PixMath.h"
#include "UV.h"
#include "SpriteMesh.h"

namespace pix
{
	// Creates a rectangular, axis-aligned SpriteMesh between two corner positions in model space.
	// The UVRect defines the UV coordinates of the rectangle corners.
	// The vertex color of all vertices is set to color.
	// Positions and texture coordinates are not validated; unusual layouts produce corresponding geometry and texture mapping.
	SpriteMesh CreateSpriteMesh(const Vec2f& topLeftPosition, const Vec2f& bottomRightPosition, const UVRect& uvRect = UVRect(), SDL_Color color = { 255, 255, 255, 255 });

	// Creates a rectangular, axis-aligned SpriteMesh between two corner positions in model space.
	// The UVQuad defines the UV coordinates of the corresponding mesh corners.
	// The vertex color of all vertices is set to color.
	// Positions and texture coordinates are not validated; unusual layouts produce corresponding geometry and texture mapping.
	SpriteMesh CreateSpriteMesh(const Vec2f& topLeftPosition, const Vec2f& bottomRightPosition, const UVQuad& uvQuad, SDL_Color color = { 255, 255, 255, 255 });

	// Creates a SpriteMesh between four corner positions in model space.
	// The UVQuad defines the UV coordinates of the corresponding mesh corners.
	// The vertex color of all vertices is set to color.
	// Positions and texture coordinates are not validated; unusual layouts produce corresponding geometry and texture mapping.
	SpriteMesh CreateSpriteMesh(const Vec2f& topLeftPosition, const Vec2f& topRightPosition, const Vec2f& bottomRightPosition, const Vec2f& bottomLeftPosition, const UVQuad& uvQuad = UVQuad(), SDL_Color color = { 255, 255, 255, 255 });

	// Returns the minimum position of the mesh's axis-aligned bounding box
	Vec2f GetMinBounds(const SpriteMesh& mesh);

	// Returns the maximum position of the mesh's axis-aligned bounding box
	Vec2f GetMaxBounds(const SpriteMesh& mesh);

	// Returns the width and height of the mesh's axis-aligned bounding box
	Vec2f GetBoundsSize(const SpriteMesh& mesh);

	// Sets the UV coordinates of the mesh from a UVRect.
	// mesh and uvRect are not validated (no clamping or corner-order enforcement); corners are mapped by name.
	void SetUV(SpriteMesh& mesh, const UVRect& uvRect);

	// Sets the UV coordinates of the mesh from a UVQuad.
	// mesh and uvQuad are not validated (no clamping or corner-order enforcement); corners are mapped by name.
	void SetUV(SpriteMesh& mesh, const UVQuad& uvQuad);

	// Sets the RGBA vertex color of all mesh vertices to color
	void SetRGBA(SpriteMesh& mesh, SDL_Color color);

	// Sets the RGBA vertex color of all mesh vertices to the provided channel values
	void SetRGBA(SpriteMesh& mesh, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

	// Sets the RGB values of all mesh vertex colors while preserving alpha
	void SetRGB(SpriteMesh& mesh, Uint8 r, Uint8 g, Uint8 b);

	// Sets the alpha value of all mesh vertex colors while preserving RGB
	void SetAlpha(SpriteMesh& mesh, Uint8 alpha);

	// Sets the position of the mesh vertices to the provided values.
	// Positions are not validated; unusual layouts produce corresponding geometry.
	void SetPositions(SpriteMesh& mesh, const Vec2f& topLeftPosition, const Vec2f& topRightPosition, const Vec2f& bottomRightPosition, const Vec2f& bottomLeftPosition);

	// Sets the position of the mesh vertices to the provided axis-aligned rectangle corners.
	// Positions are not validated; inverted corners produce inverted geometry.
	void SetPositions(SpriteMesh& mesh, const Vec2f& topLeftPosition, const Vec2f& bottomRightPosition);

}
