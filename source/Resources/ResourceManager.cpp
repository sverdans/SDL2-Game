#include <SDL.h>
#include <SDL_image.h>

#include <Resources/ResourceManager.h>
#include <System/Window.h>
#include <Resources/Sprite.h>


ResourceManager& ResourceManager::Instance()
{
	static ResourceManager instance;
	return instance;
}

void ResourceManager::SetRootDir(const std::string& sRootDir) { msRootDir = sRootDir; }

SDL_Texture* ResourceManager::LoadTexture(const std::string& sTextureName, const std::string& sFilePath, std::string& sProblem)
{
	if (mTexturesMap.find(sTextureName) != mTexturesMap.end())
	{
		sProblem = "Texture with name '" + sTextureName + "' already exist";
		return nullptr;
	}

	auto pSurface = IMG_Load((msRootDir + sFilePath).c_str());
	if (!pSurface)
	{
		sProblem = "Can not load surface from '" + sFilePath + "'";
		return nullptr;
	}

	auto pTexture = SDL_CreateTextureFromSurface(Window::Instance().GetRendererPointer(), pSurface);
	SDL_FreeSurface(pSurface);
	if (!pTexture)
	{
		sProblem = "Can not create texture '" + sTextureName + "' from surface";
		return nullptr;
	}

	mTexturesMap[sTextureName] = pTexture;
	return pTexture;
}	

SDL_Texture* ResourceManager::GetTexture(const std::string& sTextureName)
{
	auto it = mTexturesMap.find(sTextureName);
	if (it != mTexturesMap.end())
		return it->second;

	return nullptr;
}

Sprite* ResourceManager::LoadSprite(const std::string& sSpriteName,
					const std::string& sTextureName,
					int x, int y, int w, int h,
					std::string& sProblem)
{
	return nullptr;
}

Sprite* ResourceManager::GetSprite(const std::string& sSpriteName)
{
	auto it = mSpritesMap.find(sSpriteName);
	if (it != mSpritesMap.end())
		return it->second;
		
	else return nullptr;
}

void ResourceManager::Clear()
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