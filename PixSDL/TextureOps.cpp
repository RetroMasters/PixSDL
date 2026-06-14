#include "TextureOps.h"
#include "Renderer.h"
#include "ErrorLogger.h"


namespace pix
{

	bool BlendTextureRegion(Texture& baseTexture, const SDL_Rect* baseRect, const Texture& modifierTexture, const SDL_Rect* modifierRect, TargetTexture& outputTexture, SDL_RendererFlip modifierFlip)
	{
		// Rect dimensions must be positive
		if ((baseRect && (baseRect->w <= 0 || baseRect->h <= 0)) || (modifierRect && (modifierRect->w <= 0 || modifierRect->h <= 0)))
		{
			ErrorLogger::Get().LogError("BlendTextureRegion() failure", "Rect dimensions must be positive!");
			return false;
		}

		// Base and modifier must be valid textures
		if (!baseTexture.IsInitialized() || !modifierTexture.IsInitialized())
		{
			ErrorLogger::Get().LogError("BlendTextureRegion() failure", "Base or modifier texture is not initialized!");
			return false;
		}

		// Output texture must not be one of the input textures
		if (outputTexture.IsInitialized() && (outputTexture.GetSDLTexture() == baseTexture.GetSDLTexture() || outputTexture.GetSDLTexture() == modifierTexture.GetSDLTexture()))
		{
			ErrorLogger::Get().LogError("BlendTextureRegion() failure", "Output texture must not be one of the input textures!");
			return false;
		}

		Renderer& renderer = Renderer::Get();

		Vec2i baseTexSize;

		if (baseRect)
			baseTexSize = Vec2i(baseRect->w, baseRect->h);
		else
			baseTexSize = baseTexture.GetSize();

		if (!outputTexture.IsInitialized() && !outputTexture.Realloc(baseTexSize.X, baseTexSize.Y)) return false;

		Vec2i outputTexSize = outputTexture.GetSize();

		if (outputTexSize != baseTexSize && !outputTexture.Realloc(baseTexSize.X, baseTexSize.Y))
			return false;

		if (!renderer.SetRenderTarget(&outputTexture)) return false;

		SDL_BlendMode cachedBlendMode = baseTexture.GetBlendMode();

		// Set this temporary blend mode to copy the base region exactly into outputTexture
		bool success = baseTexture.SetBlendMode(SDL_BLENDMODE_NONE);

		if (!renderer.Render(baseTexture, baseRect, nullptr))
		{
			ErrorLogger::Get().LogSDLError("BlendTextureRegion() - Renderer::Render(baseTexture) failure");
			success = false;
		}

		// Restore cached blend mode.
		// If cachedBlendMode is SDL_BLENDMODE_INVALID, SetBlendMode() should fail/log.
		success = baseTexture.SetBlendMode(cachedBlendMode) && success;

		if (!renderer.RenderEx(modifierTexture, modifierRect, nullptr, 0.0, nullptr, modifierFlip))
		{
			ErrorLogger::Get().LogSDLError("BlendTextureRegion() - Renderer::RenderEx(modifierTexture) failure");
			success = false;
		}

		return success;
	}


	bool BlendTextureRegion(Texture& texture, const SDL_Rect* baseRect, const SDL_Rect* modifierRect, TargetTexture& outputTexture, SDL_RendererFlip modifierFlip)
	{
		return BlendTextureRegion(texture, baseRect, texture, modifierRect, outputTexture, modifierFlip);
	}

}