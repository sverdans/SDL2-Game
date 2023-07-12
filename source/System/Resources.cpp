#include <SDL.h>
#include <SDL_image.h>

#include <System/Window.h>
#include <System/Resources.h>
#include <Resources/Sprite.h>


Resources& Resources::Instance()
{
	static Resources instance;
	return instance;
}

void Resources::SetRootDir(const std::string& sRootDir) { msRootDir = sRootDir; }

SDL_Texture* Resources::LoadTexture(const std::string& sTextureName, const std::string& sFilePath, std::string& sProblem)
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

SDL_Texture* Resources::GetTexture(const std::string& sTextureName)
{
	auto it = mTexturesMap.find(sTextureName);
	if (it != mTexturesMap.end())
		return it->second;

	return nullptr;
}

Sprite* Resources::LoadSprite(const std::string& sSpriteName,
					const std::string& sTextureName,
					const SDL_Rect& srcRect,
					std::string& sProblem)
{
	if (mSpritesMap.find(sSpriteName) != mSpritesMap.end())
	{
		sProblem = "Sprite with name '" + sTextureName + "' already exist";
		return nullptr;
	}

	auto pTexture = GetTexture(sTextureName);
	if (!pTexture)
	{
		sProblem = "Texture with name '" + sTextureName + "' was not foud";
		return nullptr;
	}

	auto pSprite = new Sprite(pTexture, srcRect);
	mSpritesMap[sSpriteName] = pSprite;
	
	return pSprite;
}

Sprite* Resources::GetSprite(const std::string& sSpriteName)
{
	auto it = mSpritesMap.find(sSpriteName);
	if (it != mSpritesMap.end())
		return it->second;
		
	return nullptr;
}

void Resources::Clear()
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