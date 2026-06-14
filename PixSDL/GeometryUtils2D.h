#pragma once

#include "PixMath.h"

// Generic 2D geometry utilities for raw points and point arrays.
// These functions operate on vector-like types with X and Y members, such as Vec2, Vec2f, and Vec2i.
// They do not know about higher-level types.
// Type-specific operation headers can use these helpers internally.
namespace pix
{

	// Returns the size of the axis-aligned bounding box of the points.
	// points must point to the first element of an array with count elements.
	// Inputs are not validated; points must not be null, and count must be greater than 0.
	template<typename T> inline T GetBoundsSize2D(const T* points, int count)
	{
		T min = points[0];
		T max = min;

		for (int i = 1; i < count; i++)
		{
			// Update minimum bounds
			if (points[i].X < min.X) min.X = points[i].X;
			if (points[i].Y < min.Y) min.Y = points[i].Y;

			// Update maximum bounds
			if (points[i].X > max.X) max.X = points[i].X;
			if (points[i].Y > max.Y) max.Y = points[i].Y;
		}

		return max - min;
	}

	// Returns the minimum point of the axis-aligned bounding box of the points.
	// points must point to the first element of an array with count elements.
	// Inputs are not validated; points must not be null, and count must be greater than 0.
	template<typename T> inline T GetMinBounds2D(const T* points, int count)
	{
		T min = points[0];

		for (int i = 1; i < count; i++)
		{
			if (points[i].X < min.X) min.X = points[i].X;
			if (points[i].Y < min.Y) min.Y = points[i].Y;
		}

		return min;
	}

	// Returns the maximum point of the axis-aligned bounding box of the points.
	// points must point to the first element of an array with count elements.
	// Inputs are not validated; points must not be null, and count must be greater than 0.
	template<typename T> inline T GetMaxBounds2D(const T* points, int count)
	{
		T max = points[0];

		for (int i = 1; i < count; i++)
		{
			if (points[i].X > max.X) max.X = points[i].X;
			if (points[i].Y > max.Y) max.Y = points[i].Y;
		}

		return max;
	}

	// Returns true if point is inside the axis-aligned box spanned by minBounds and maxBounds.
	// The bounds are inclusive.
	// minBounds and maxBounds are not validated; minBounds must be less than or equal to maxBounds on both axes.
	template<typename T> inline bool IsInsideBounds2D(const T& point, const T& minBounds, const T& maxBounds)
	{
		return (point.X >= minBounds.X) && (point.X <= maxBounds.X) && (point.Y >= minBounds.Y) && (point.Y <= maxBounds.Y);
	}




}

