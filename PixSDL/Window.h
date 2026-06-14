#pragma once

#include <SDL_video.h>
#include <string>
#include "Uncopyable.h"
#include "PixMath.h"

namespace pix
{
	// The Window singleton manages the lifetime, state, and configuration of its single SDL window,
	// providing controlled access to resizing and fullscreen/windowed mode transitions.
	//
	// Must be initialized with Init() before normal use.
	// 
	// Philosophy:
	// This Window abstraction is intentionally minimal.
	// The default initialization path provides a single visible, resizable main window
	// with optional borderless desktop fullscreen.
	// Specific setups such as OpenGL or Vulkan can use the explicit SDL-flags initialization path.
	// Legacy exclusive fullscreen can be requested through InitWithFlags(),
	// but SetFullscreen() uses borderless desktop fullscreen when enabling fullscreen.
	// The main monitor's desktop resolution is cached at initialization for simple, stable access.
	// 
	// Rendering is assumed to operate at a fixed internal resolution that is
	// upscaled/downscaled to the native desktop resolution.
	// 
	// Windowing is a critical subsystem; after Init() failure, normal engine execution should not continue.
	// Window methods therefore assume successful initialization and do not perform repeated initialization checks.
	class Window : private Uncopyable
	{

	public:

		// Returns the Window instance
		static Window& Get();



		// Initializes the Window singleton using PixSDL's default window flags:
		// shown, resizable, and optionally borderless desktop fullscreen.
		bool Init(const std::string& title, int windowedWidth, int windowedHeight, bool isFullscreen);

		// Initializes the Window singleton using explicit SDL window flags.
		// windowFlags should be composed from SDL_WindowFlags values. 
		// All desired flags must be provided explicitly.
		// This allows specific setups such as SDL_WINDOW_OPENGL or SDL_WINDOW_VULKAN.
		// Returns true if the SDL_Window was created, or if the Window is already initialized.
		// Calling any Init method again after successful initialization has no effect and returns true.
		// Window setup failures after creation are logged but do not make initialization fail.
		// Window is not meant to be reinitialized during normal program execution.
		bool InitWithFlags(const std::string& title, int windowedWidth, int windowedHeight, Uint32 windowFlags);

		// Destroys the window resource.
		// Must be called after destroying the Renderer to ensure proper cleanup order.
		void Destroy();


		// Sets borderless desktop fullscreen mode.
		// Returns true on success, false otherwise.
		bool SetFullscreen(bool isFullscreen);

		void SetWindowedSize(int width, int height);

		void SetTitle(const std::string& title);



		bool IsFullscreen() const;

		// Returns the preferred windowed size, restored when exiting fullscreen
		Vec2i GetWindowedSize() const;

		// Returns the cached desktop size of the main monitor
		Vec2i GetScreenSize() const;

		const std::string& GetTitle() const;

		SDL_Window* GetSDLWindow() const;

		bool IsInitialized() const;

	private:

		Window() = default;

		// Final safety cleanup.
		// Prefer explicit Destroy() calls during normal shutdown to control destruction order (Renderer should be destroyed before Window).
		~Window();

		SDL_Window* sdlWindow_ = nullptr;

		Vec2i windowedSize_ = Vec2i(1, 1); // Preferred windowed size, restored when exiting fullscreen
		Vec2i screenSize_ = Vec2i(1, 1);   // Main monitor's desktop resolution, cached at initialization

		std::string title_;
		bool isInitialized_ = false;

	};

}