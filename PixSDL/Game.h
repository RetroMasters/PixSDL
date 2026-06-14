#pragma once

#include "Gameloop.h"
#include "ObjectInput.h"
#include "ImageTexture.h"
#include "SpriteMeshRenderer2D.h"
#include "Spaceship.h"

namespace pix
{
	class Game : public GameLoop
	{
	public:

		Game(const std::string& companyName, const std::string& appName, const LaunchConfigData& configData);

		~Game();

		void Update() override;

		void Render() override;

	private:

		ObjectInput input_;

		SpriteMeshRenderer2D spriteMeshRenderer2D_;

		Spaceship spaceship_;
		ImageTexture spaceshipTex_;
		ImageTexture effectTex_;
	};
}