#pragma once

#include <SDL_pixels.h>
#include "PixMath.h"
#include "UV.h"
#include "Mesh2D.h"

namespace pix
{
	// Creates a rectangular, axis-aligned, subdivided Mesh2D between two corner positions in model space.
	// subdivisionAmount defines the number of rectangular cells of the grid along X and Y.
	// Each cell is emitted as 6 vertices forming two triangles. subdivisionAmount = Vec2i(1, 1) produces a single cell made of two triangles.
	// Subdivision values smaller than 1 are clamped to 1.
	// The UVs are defined by UVRect. They are linearly distributed across the generated vertices.
	// Flipped UVRect corners intentionally produce flipped texture mapping.
	// Cells are emitted in book-reading order: left-to-right within a row, then top-to-bottom across rows.
	// The vertex color of all vertices is set to color.
	// Vertex normals are set to normal without normalizing it.
	// Positions are not validated; the provided corner relationship is preserved.
	// Swapped position corners produce flipped geometry and may change the effective triangle winding.
	//
	// An example mesh with subdivisionAmount = Vec2i(3, 2):
	// 
	// •--•--•--•
	// |  |  |  |
	// •--•--•--•
	// |  |  |  |
	// •--•--•--•
	//
	// Conceptual corner layout inside each cell as seen by a viewer looking into the screen:
	// 0--1
	// |\ |
	// | \|
	// 3--2
	//
	// Triangle corner order, assigned to the 6 emitted vertices of each cell:
	// First triangle:  0-1-2
	// Second triangle: 2-3-0
	Mesh2D CreateGridMesh2D(const Vec2f& topLeftPosition, const Vec2f& bottomRightPosition, const UVRect& uvRect = UVRect(), const Vec2i& subdivisionAmount = Vec2i(1, 1), SDL_Color color = { 255, 255, 255, 255 }, const Vec3f& normal = Vec3f(0.0f, 0.0f, 1.0f));

	// Wrapper for the four-corner CreateQuadMesh2D() overload.
	// Creates a rectangular, axis-aligned Mesh2D quad between two corner positions in model space.
	Mesh2D CreateQuadMesh2D(const Vec2f& topLeftPosition, const Vec2f& bottomRightPosition, const UVQuad& uvQuad = UVQuad(), SDL_Color color = { 255, 255, 255, 255 }, const Vec3f& normal = Vec3f(0.0f, 0.0f, 1.0f));

	// Creates a Mesh2D quad between four corner positions in model space.
	// The UVQuad defines the UV coordinates of the corresponding mesh corners.
	// The mesh is emitted as 6 vertices forming two triangles that represent the quad.
	// The vertex color of all vertices is set to color.
	// Vertex normals are set to normal without normalizing it.
	// Positions and texture coordinates are not validated; unusual layouts produce corresponding geometry and texture mapping.
	// 
	// Conceptual corner layout as seen by a viewer looking into the screen:
	// 0--1
	// |\ |
	// | \|
	// 3--2
	//
	// Triangle corner order, assigned to the 6 emitted vertices:
	// First triangle:  0-1-2
	// Second triangle: 2-3-0
	Mesh2D CreateQuadMesh2D(const Vec2f& topLeftPosition, const Vec2f& topRightPosition, const Vec2f& bottomRightPosition, const Vec2f& bottomLeftPosition, const UVQuad& uvQuad = UVQuad(), SDL_Color color = { 255, 255, 255, 255 }, const Vec3f& normal = Vec3f(0.0f, 0.0f, 1.0f));

	// Returns the minimum position of the mesh's axis-aligned bounding box.
	// Returns zero vector if the mesh is empty.
	Vec2f GetMinBounds(const Mesh2D& mesh);

	// Returns the maximum position of the mesh's axis-aligned bounding box.
	// Returns zero vector if the mesh is empty.
	Vec2f GetMaxBounds(const Mesh2D& mesh);

	// Returns the width and height of the mesh's axis-aligned bounding box.
	// Returns zero vector if the mesh is empty.
	Vec2f GetBoundsSize(const Mesh2D& mesh);

	// Sets the RGBA vertex color of all mesh vertices to color.
	void SetRGBA(Mesh2D& mesh, SDL_Color color);

	// Sets the RGBA vertex color of all mesh vertices to the provided channel values.
	void SetRGBA(Mesh2D& mesh, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

	// Sets the RGB values of all mesh vertex colors while preserving alpha.
	void SetRGB(Mesh2D& mesh, Uint8 r, Uint8 g, Uint8 b);

	// Sets the alpha value of all mesh vertex colors while preserving RGB.
	void SetAlpha(Mesh2D& mesh, Uint8 alpha);

	// Sets the normal of all mesh vertices to normal.
	// normal is assigned as provided and is not normalized.
	void SetNormal(Mesh2D& mesh, const Vec3f& normal);

}