#pragma once
#include <string>
#include <map>

class SDL_Texture;
class Sprite;

class ResourceManager final
{
private:
	ResourceManager() = default;
	~ResourceManager() = default;
	
	std::string msRootDir;
	std::map<std::string, SDL_Texture*> mTexturesMap;
	std::map<std::string, Sprite*> mSpritesMap;

public:
	ResourceManager(const ResourceManager &) = delete;
	ResourceManager(ResourceManager &&) = delete;

	ResourceManager& operator=(const ResourceManager &) = delete;
	ResourceManager& operator=(ResourceManager &&) = delete;

	static ResourceManager& Instance();

	void SetRootDir(const std::string& sRootDir);

	SDL_Texture* LoadTexture(const std::string& sTextureName, const std::string& sFilePath, std::string& sProblem);

	SDL_Texture* GetTexture(const std::string& sTextureName);

	Sprite* LoadSprite(const std::string& sSpriteName,
					   const std::string& sTextureName,
					   int x, int y, int w, int h,
					   std::string& sProblem);

	Sprite* GetSprite(const std::string& sSpriteName);

	void Clear();
};