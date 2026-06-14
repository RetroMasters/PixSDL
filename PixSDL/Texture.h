#pragma once

#include "Uncopyable.h"
#include <SDL_render.h>
#include "PixMath.h"

namespace pix
{
	// Texture is a resource-managing non-instantiable base class for SDL_Texture. 
	// It provides common properties and functionality for concrete texture types.
	//
	// Technical note:
	// SDL_Textures are GPU resources that know their internal pixel format, and it may differ from the format used to create or load them. 
	// SDL guarantees correct blending and rendering across textures regardless of their origin, 
	// since it creates SDL_Texture resources in a format accepted by the the GPU.
	// 
	// Philosophy:
	// Conceptually, a texture is just raw pixel data. However, in SDL2, blending state is bound to the texture
	// rather than the renderer. Texture follows this model to avoid unnecessary abstraction friction.
	// Texture uniquely owns its SDL_Texture, but can expose it to interoperate with the C API.
	// Since multiple texture types share common state and behavior, Texture serves as a common base class for concrete texture types.
	// 
	// Note: On failure, getters tend to return semantically invalid values to make misuse visible.
	class Texture : private Uncopyable
	{
	public:

		// RAII: Destroys SDL_Texture
		virtual ~Texture(); // virtual: Ensure proper destruction from a base class pointer

		// Sets the texture blend mode.
		// Returns true on success, false otherwise.
		bool SetBlendMode(SDL_BlendMode blendMode);

		// Modulates the color channels for a blending operation.
		// Returns true on success, false otherwise.
		bool SetRGBMod(Uint8 r, Uint8 g, Uint8 b);

		// Modulates the alpha channel for a blending operation.
		// Returns true on success, false otherwise.
		bool SetAlphaMod(Uint8 alpha);

		// Modulates all channels for a blending operation.
		// Returns true on success, false otherwise.
		bool SetRGBAMod(Uint8 r, Uint8 g, Uint8 b, Uint8 alpha); // Convenience function

		// Modulates all channels for a blending operation using color.
		// Returns true on success, false otherwise.
		bool SetRGBAMod(SDL_Color color); // Convenience function

		// Sets the scale filter used when this texture is rendered with scaling.
		// false = nearest pixel sampling, true = linear filtering.
		// Returns true on success, false otherwise.
		bool SetLinearFilter(bool isLinearFilter);



		// Returns SDL_BLENDMODE_INVALID on failure
		SDL_BlendMode GetBlendMode() const;

		// rgb not modified if the texture is not initialized
		// Returns true on success, false otherwise.
		bool GetRGBMod(Uint8& r, Uint8& g, Uint8& b) const;

		// Returns the texture alpha modulation value, or 0 on failure.
		Uint8 GetAlphaMod() const;

		// rgba not modified if the texture is not initialized
		// Returns true on success, false otherwise.
		bool GetRGBAMod(Uint8& r, Uint8& g, Uint8& b, Uint8& a) const;

		// Returns the texture width and height in pixels, or the zero vector on failure
		Vec2i GetSize() const;

		// Get the texture resource pointer for the C API.
		// Ownership remains with Texture; destroying the exposed SDL_Texture results in undefined behavior.
		SDL_Texture* GetSDLTexture() const;

		// Returns true if this texture uses linear filtering, false if it uses nearest filtering.
		// Returns false if the texture is not initialized or the query fails.
		bool IsLinearFilter() const;

		// Returns true if this Texture currently wraps a valid SDL_Texture, false otherwise.
		// Derived classes must ensure that sdlTexture_ is either nullptr or a valid initialized texture.
		bool IsInitialized() const;

	protected:

		Texture() = default; // Allow construction of derived classes only

		// Destroys SDL_Texture and sets sdlTexture_ to nullptr.
		// The command is ignored if sdlTexture_ is already a nullptr.
		void DestroySDLTexture();

		SDL_Texture* sdlTexture_ = nullptr;
	};
}