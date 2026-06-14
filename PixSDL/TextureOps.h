#pragma once

#include <SDL_rect.h>
#include <SDL_render.h>
#include "TargetTexture.h"

// Texture operations may set the current render target and do not restore the previous one.
// Callers should set the desired render target again when needed.
namespace pix
{
	// Renders a modified texture region into outputTexture.
	// The base region is copied first, then the modifier region is stretched to the same size,
	// optionally flipped, and rendered over it using the modifier texture's current blend mode.
	// If baseRect is nullptr, the full base texture is used.
	// If modifierRect is nullptr, the full modifier texture is used.
	// outputTexture is allocated or reallocated only if it is not initialized or its size does not match the base region size.
	// The base texture's blend mode is temporarily changed internally and restored before returning.
	// Returns true only if the complete operation succeeded; on failure, outputTexture may contain partial results.
	bool BlendTextureRegion(Texture& baseTexture, const SDL_Rect* baseRect, const Texture& modifierTexture, const SDL_Rect* modifierRect, TargetTexture& outputTexture, SDL_RendererFlip modifierFlip = SDL_FLIP_NONE);

	// Overload using two regions from the same texture.
	// Equivalent to BlendTextureRegion(texture, baseRect, texture, modifierRect, outputTexture, modifierFlip).
	// The texture's current blend mode is used for the modifier pass.
	bool BlendTextureRegion(Texture& texture, const SDL_Rect* baseRect, const SDL_Rect* modifierRect, TargetTexture& outputTexture, SDL_RendererFlip modifierFlip = SDL_FLIP_NONE);
}