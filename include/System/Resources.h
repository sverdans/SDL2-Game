#pragma once
#include <string>
#include <map>

#include <SDL.h>

class SDL_Texture;
class Sprite;

class Resources final
{
private:
	Resources() = default;
	~Resources() = default;
	
	std::string msRootDir;
	std::map<std::string, SDL_Texture*> mTexturesMap;
	std::map<std::string, Sprite*> mSpritesMap;

public:
	Resources(const Resources &) = delete;
	Resources(Resources &&) = delete;

	Resources& operator=(const Resources &) = delete;
	Resources& operator=(Resources &&) = delete;

	static Resources& Instance();

	void SetRootDir(const std::string& sRootDir);

	SDL_Texture* LoadTexture(const std::string& sTextureName, const std::string& sFilePath, std::string& sProblem);

	SDL_Texture* GetTexture(const std::string& sTextureName);

	Sprite* LoadSprite(const std::string& sSpriteName,
					   const std::string& sTextureName,
					   const SDL_Rect& srcRect,
					   std::string& sProblem);

	Sprite* GetSprite(const std::string& sSpriteName);

	void Clear();
};