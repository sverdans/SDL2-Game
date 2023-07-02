#pragma once
#include <string>
#include <memory>
#include <map>

#include <SDL.h>
#include <SDL_image.h>

#include <Renderer/Renderer.h>
#include <Renderer/Sprite.h>

class ResourceManager final
{
private:
	ResourceManager() = default;
	~ResourceManager() = default;
	
	std::map<std::string, SDL_Texture*> mTexturesMap;
	std::map<std::string, Sprite*> mSpritesMap;

public:
	ResourceManager(const ResourceManager &) = delete;
	ResourceManager(ResourceManager &&) = delete;

	ResourceManager& operator=(const ResourceManager &) = delete;
	ResourceManager& operator=(ResourceManager &&) = delete;

	static ResourceManager& Instance()
	{
		static ResourceManager instance;
		return instance;
	}

	SDL_Texture* LoadTexture(const std::string& sTextureName, const std::string& sFilePath, std::string& sProblem)
	{
		if (mTexturesMap.find(sTextureName) != mTexturesMap.end())
		{
			sProblem = "Texture with name '" + sTextureName + "' already exist";
			return nullptr;
		}

		auto pSurface = IMG_Load(sFilePath.c_str());
		if (pSurface)
		{
			sProblem = "Can not load surface from '" + sFilePath + "'";
			return nullptr;
		}

		auto pTexture = SDL_CreateTextureFromSurface(Renderer::Instance().GetRenderer(), pSurface);
		SDL_FreeSurface(pSurface);
		if (pTexture)
		{
			sProblem = "Can not create texture '" + sTextureName + "' from surface";
			return nullptr;
		}

		mTexturesMap[sTextureName] = pTexture;
		return pTexture;
	}	

	SDL_Texture* GetTexture(const std::string& sTextureName)
	{
		auto it = mTexturesMap.find(sTextureName);
		if (it != mTexturesMap.end())
			return it->second;

		else return nullptr;
	}

	Sprite* LoadSprite(const std::string& sSpriteName,
					   const std::string& sTextureName,
					   int x, int y, int w, int h,
					   std::string& sProblem)
	{

	}

	Sprite* GetSprite(const std::string& sSpriteName)
	{
		auto it = mSpritesMap.find(sSpriteName);
		if (it != mSpritesMap.end())
			return it->second;
			
		else return nullptr;
	}

	void Clear()
	{
		for (auto& [sTextureName, pTexture] : mTexturesMap)
		{
			if (pTexture)
				SDL_DestroyTexture(pTexture);
		}
		mTexturesMap.clear();

		for (auto& [sSpriteName, pSprite] : mSpritesMap)
		{
			if (pSprite)
				delete pSprite;
		}
		mSpritesMap.clear();
	}
};