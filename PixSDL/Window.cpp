#include "Window.h"
#include "ErrorLogger.h"

namespace pix
{
	Window& Window::Get()
	{
		static Window window_;

		return window_;
	}

	bool Window::Init(const std::string& title, int windowedWidth, int windowedHeight, bool isFullscreen)
	{
		Uint32 flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN;

		if (isFullscreen)
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;

		return InitWithFlags(title, windowedWidth, windowedHeight, flags);
	}

	bool Window::InitWithFlags(const std::string& title, int windowedWidth, int windowedHeight, Uint32 windowFlags)
	{
		if (isInitialized_) return true;

		if (windowedWidth < 1) windowedWidth = 1;
		if (windowedHeight < 1) windowedHeight = 1;

		sdlWindow_ = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowedWidth, windowedHeight, windowFlags);
		if (!sdlWindow_)
		{
			ErrorLogger::Get().LogSDLError("Window::InitWithFlags() - SDL_CreateWindow() failure");
			return false;
		}

		// Try to get the desktop resolution of the main monitor (SDL display 0) 
		SDL_DisplayMode desktopMode;
		if (SDL_GetDesktopDisplayMode(0, &desktopMode) != 0)
		{
			ErrorLogger::Get().LogSDLError("Window::InitWithFlags() - SDL_GetDesktopDisplayMode() failure");
			// Use the initial windowed size as the best available approximation when desktop size in unavailable
			screenSize_ = Vec2i(windowedWidth, windowedHeight);
		}
		else
		{
			// Cache the desktop resolution of the main monitor
			screenSize_ = Vec2i(desktopMode.w, desktopMode.h);
		}

		windowedSize_ = Vec2i(windowedWidth, windowedHeight);

		title_ = title;
		isInitialized_ = true;

		return true;
	}

	void Window::Destroy()
	{
		if (!sdlWindow_) return;

		SDL_DestroyWindow(sdlWindow_);
		sdlWindow_ = nullptr;
		windowedSize_ = Vec2i(1, 1);
		screenSize_ = Vec2i(1, 1);
		title_.clear();
		isInitialized_ = false;
	}

	bool Window::SetFullscreen(bool isFullscreen)
	{
		if (IsFullscreen() == isFullscreen) return true;

		if (isFullscreen)
		{
			if (SDL_SetWindowFullscreen(sdlWindow_, SDL_WINDOW_FULLSCREEN_DESKTOP) != 0)
			{
				ErrorLogger::Get().LogSDLError("Window::SetFullscreen() - SDL_SetWindowFullscreen() failure");
				return false;
			}
		}
		else
		{
			if (SDL_SetWindowFullscreen(sdlWindow_, 0) != 0)
			{
				ErrorLogger::Get().LogSDLError("Window::SetFullscreen() - SDL_SetWindowFullscreen() failure");
				return false;
			}
			else
			{
				SDL_SetWindowSize(sdlWindow_, windowedSize_.X, windowedSize_.Y);
			}
		}

		return true;
	}

	void Window::SetWindowedSize(int width, int height)
	{
		if (width < 1)  width = 1;
		if (height < 1) height = 1;

		// Only apply immediately if we're in windowed mode
		if (!IsFullscreen())
			SDL_SetWindowSize(sdlWindow_, width, height);

		// Always update the preferred windowed size
		windowedSize_ = Vec2i(width, height);
	}

	void Window::SetTitle(const std::string& title)
	{
		SDL_SetWindowTitle(sdlWindow_, title.c_str());
		title_ = title;
	}



	bool Window::IsFullscreen() const
	{
		return (SDL_GetWindowFlags(sdlWindow_) & (SDL_WINDOW_FULLSCREEN | SDL_WINDOW_FULLSCREEN_DESKTOP)) != 0;
	}

	Vec2i Window::GetWindowedSize() const
	{
		return windowedSize_;
	}

	Vec2i Window::GetScreenSize() const
	{
		return screenSize_;
	}


	const std::string& Window::GetTitle() const
	{
		return title_;
	}

	SDL_Window* Window::GetSDLWindow() const
	{
		return sdlWindow_;
	}

	bool Window::IsInitialized() const
	{
		return isInitialized_;
	}

	Window::~Window()
	{
		Destroy();
	}

}