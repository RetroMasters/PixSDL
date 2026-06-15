#include "Game.h"
#include "Window.h"
#include "Renderer.h"
#include "PathUtils.h"

namespace pix
{
	Game::Game(const std::string& companyName, const std::string& appName, const LaunchConfigData& configData) : GameLoop(companyName, appName, configData)
	{
		input_.AddKeyboardBinding(SDL_SCANCODE_ESCAPE, "Esc");
		input_.AddKeyboardBinding(SDL_SCANCODE_SPACE, "Space");

		ObjectInput spaceshipInput;
		spaceshipInput.AddKeyboardBinding(SDL_SCANCODE_UP, "Up");
		spaceshipInput.AddKeyboardBinding(SDL_SCANCODE_LEFT, "Left");
		spaceshipInput.AddKeyboardBinding(SDL_SCANCODE_DOWN, "Down");
		spaceshipInput.AddKeyboardBinding(SDL_SCANCODE_RIGHT, "Right");

		spaceshipInput.AddKeyboardBinding(SDL_SCANCODE_W, "Up");
		spaceshipInput.AddKeyboardBinding(SDL_SCANCODE_A, "Left");
		spaceshipInput.AddKeyboardBinding(SDL_SCANCODE_S, "Down");
		spaceshipInput.AddKeyboardBinding(SDL_SCANCODE_D, "Right");

		std::string basePath = GetBasePath();

		spaceshipTex_.Reload(basePath + "Textures/Spaceship.png");
		effectTex_.Reload(basePath + "Textures/Effect.png");
		
		Vec2f shipMeshSize = Vec2f(spaceshipTex_.GetSize()) * 0.1f;
		Vec2f effectMeshSize = Vec2f(effectTex_.GetSize().X / 4.0f, effectTex_.GetSize().Y) * 0.1f;

		spaceship_.Reinit(shipMeshSize, effectMeshSize, 4, 1.5f, spaceshipInput);
		
		HysteresisUpdateLoopScheduler* bestScheduler = new HysteresisUpdateLoopScheduler(60.0f);
		SetUpdateLoopScheduler(bestScheduler);
	}

	Game::~Game()
	{
	   delete GetUpdateLoopScheduler();
	}


	void Game::Update()
	{
		input_.Update();

		spaceship_.Update();

		if (input_.BecamePositive("Space"))
			Window::Get().SetFullscreen(!Window::Get().IsFullscreen());

		if (input_.BecamePositive("Esc"))
			Quit();
	}

	void Game::Render()
	{
		Renderer& renderer = Renderer::Get();

		Vec2f renderTargetOffset(renderer.GetLogicalResolutionWidth() * 0.5f, renderer.GetLogicalResolutionHeight() * 0.5f);

		renderer.Clear();

	    spriteMeshRenderer2D_.BeginBatch(MovableObject2D(), renderTargetOffset, GetInterpolationAlpha());

			spaceship_.RenderBody(spriteMeshRenderer2D_);

		spriteMeshRenderer2D_.RenderBatch(spaceshipTex_, nullptr);


		spriteMeshRenderer2D_.BeginBatch(MovableObject2D(), renderTargetOffset, GetInterpolationAlpha());

		     spaceship_.RenderEffect(spriteMeshRenderer2D_);

		spriteMeshRenderer2D_.RenderBatch(effectTex_, nullptr);
	}

}