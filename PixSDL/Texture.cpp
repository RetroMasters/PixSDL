#include "Texture.h"
#include "ErrorLogger.h"

namespace pix
{
	Texture::~Texture()
	{
		DestroySDLTexture();
	}



	bool Texture::SetBlendMode(SDL_BlendMode blendMode)
	{
		if (!sdlTexture_)
		{
			ErrorLogger::Get().LogError("Texture::SetBlendMode() failure", "sdlTexture_ is nullptr!");
			return false;
		}

		if (SDL_SetTextureBlendMode(sdlTexture_, blendMode) != 0)
		{
			ErrorLogger::Get().LogSDLError("Texture::SetBlendMode() - SDL_SetTextureBlendMode() failure");
			return false;
		}

		return true;
	}

	bool Texture::SetRGBMod(Uint8 r, Uint8 g, Uint8 b)
	{
		if (!sdlTexture_)
		{
			ErrorLogger::Get().LogError("Texture::SetColorMod() failure", "sdlTexture_ is nullptr!");
			return false;
		}

		if (SDL_SetTextureColorMod(sdlTexture_, r, g, b) != 0)
		{
			ErrorLogger::Get().LogSDLError("Texture::SetColorMod() - SDL_SetTextureColorMod() failure");
			return false;
		}

		return true;
	}

	bool Texture::SetAlphaMod(Uint8 alpha)
	{
		if (!sdlTexture_)
		{
			ErrorLogger::Get().LogError("Texture::SetAlphaMod() failure", "sdlTexture_ is nullptr!");
			return false;
		}

		if (SDL_SetTextureAlphaMod(sdlTexture_, alpha) != 0)
		{
			ErrorLogger::Get().LogSDLError("Texture::SetAlphaMod() - SDL_SetTextureAlphaMod() failure");
			return false;
		}

		return true;
	}

	bool Texture::SetRGBAMod(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
	{
		if (!sdlTexture_)
		{
			ErrorLogger::Get().LogError("Texture::SetRGBAMod() failure", "sdlTexture_ is nullptr!");
			return false;
		}

		if (SDL_SetTextureColorMod(sdlTexture_, r, g, b) != 0)
		{
			ErrorLogger::Get().LogSDLError("Texture::SetRGBAMod() - SDL_SetTextureColorMod() failure");
			return false;
		}

		if (SDL_SetTextureAlphaMod(sdlTexture_, a) != 0)
		{
			ErrorLogger::Get().LogSDLError("Texture::SetRGBAMod() - SDL_SetTextureAlphaMod() failure");
			return false;
		}

		return true;
	}

	bool Texture::SetRGBAMod(SDL_Color color)
	{
		return SetRGBAMod(color.r, color.g, color.b, color.a);
	}


	bool Texture::SetLinearFilter(bool isLinearFilter)
	{
		if (!sdlTexture_)
		{
			ErrorLogger::Get().LogError("Texture::SetLinearFilter() failure", "sdlTexture_ is nullptr!");
			return false;
		}

		const SDL_ScaleMode scaleMode = isLinearFilter ? SDL_ScaleModeLinear : SDL_ScaleModeNearest;

		if (SDL_SetTextureScaleMode(sdlTexture_, scaleMode) != 0)
		{
			ErrorLogger::Get().LogSDLError("Texture::SetLinearFilter() - SDL_SetTextureScaleMode() failure");
			return false;
		}

		return true;
	}




	SDL_BlendMode Texture::GetBlendMode() const
	{
		if (!sdlTexture_)
		{
			ErrorLogger::Get().LogError("Texture::GetBlendMode() failure", "sdlTexture_ is nullptr!");
			return SDL_BLENDMODE_INVALID;
		}

		SDL_BlendMode blendMode = SDL_BLENDMODE_INVALID;
		if (SDL_GetTextureBlendMode(sdlTexture_, &blendMode) != 0)
		{
			ErrorLogger::Get().LogSDLError("Texture::GetBlendMode() - SDL_GetTextureBlendMode() failure");
			return SDL_BLENDMODE_INVALID;
		}

		return blendMode;
	}

	bool Texture::GetRGBMod(Uint8& r, Uint8& g, Uint8& b) const
	{
		if (!sdlTexture_)
		{
			ErrorLogger::Get().LogError("Texture::GetRGBMod() failure", "sdlTexture_ is nullptr!");
			return false;
		}

		if (SDL_GetTextureColorMod(sdlTexture_, &r, &g, &b) != 0)
		{
			ErrorLogger::Get().LogSDLError("Texture::GetRGBMod() - SDL_GetTextureColorMod() failure");
			return false;
		}

		return true;
	}

	Uint8 Texture::GetAlphaMod() const
	{
		if (!sdlTexture_)
		{
			ErrorLogger::Get().LogError("Texture::GetAlphaMod() failure", "sdlTexture_ is nullptr!");
			return 0;
		}

		Uint8 alphaMod = 0;
		if (SDL_GetTextureAlphaMod(sdlTexture_, &alphaMod) != 0)
		{
			ErrorLogger::Get().LogSDLError("Texture::GetAlphaMod() - SDL_GetTextureAlphaMod() failure");
			alphaMod = 0;
		}

		return alphaMod;
	}

	bool Texture::GetRGBAMod(Uint8& r, Uint8& g, Uint8& b, Uint8& a) const
	{
		if (!sdlTexture_)
		{
			ErrorLogger::Get().LogError("Texture::GetRGBAMod() failure", "sdlTexture_ is nullptr!");
			return false;
		}

		if (SDL_GetTextureColorMod(sdlTexture_, &r, &g, &b) != 0)
		{
			ErrorLogger::Get().LogSDLError("Texture::GetRGBAMod() - SDL_GetTextureColorMod() failure");
			return false;
		}

		if (SDL_GetTextureAlphaMod(sdlTexture_, &a) != 0)
		{
			ErrorLogger::Get().LogSDLError("Texture::GetRGBAMod() - SDL_GetTextureAlphaMod() failure");
			return false;
		}

		return true;
	}

	Vec2i Texture::GetSize() const
	{
		if (!sdlTexture_)
		{
			ErrorLogger::Get().LogError("Texture::GetSize() failure", "sdlTexture_ is nullptr!");
			return Vec2i(0, 0);
		}

		int width = 0;
		int height = 0;

		if (SDL_QueryTexture(sdlTexture_, nullptr, nullptr, &width, &height) != 0)
		{
			ErrorLogger::Get().LogSDLError("Texture::GetSize() - SDL_QueryTexture() failure");
			return Vec2i(0, 0);
		}

		return Vec2i(width, height);
	}


	SDL_Texture* Texture::GetSDLTexture() const
	{
		return sdlTexture_;
	}

	bool Texture::IsLinearFilter() const
	{
		if (!sdlTexture_)
		{
			ErrorLogger::Get().LogError("Texture::IsLinearFilter() failure", "sdlTexture_ is nullptr!");
			return false;
		}

		SDL_ScaleMode scaleMode;

		if (SDL_GetTextureScaleMode(sdlTexture_, &scaleMode) != 0)
		{
			ErrorLogger::Get().LogSDLError("Texture::IsLinearFilter() - SDL_GetTextureScaleMode() failure");
			return false;
		}

		return scaleMode == SDL_ScaleModeLinear || scaleMode == SDL_ScaleModeBest;
	}

	bool Texture::IsInitialized() const
	{
		return sdlTexture_ != nullptr;
	}

	void Texture::DestroySDLTexture()
	{
		if (sdlTexture_)
		{
			SDL_DestroyTexture(sdlTexture_);
			sdlTexture_ = nullptr;
		}
	}

}






