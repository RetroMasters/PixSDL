#include"PathUtils.h"
#include<SDL.h>


namespace pix
{

	std::string GetPrefPath(const std::string& companyName, const std::string& appName)
	{
		char* prefPath = SDL_GetPrefPath(companyName.c_str(), appName.c_str());
		if (!prefPath) return std::string();

		std::string result(prefPath);
		SDL_free(prefPath);

		return result;
	}

	std::string GetBasePath()
	{
		char* basePath = SDL_GetBasePath();
		if (!basePath) return std::string();

		std::string result(basePath);
		SDL_free(basePath);

		return result;
	}


}