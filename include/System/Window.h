#pragma once
#include <string>

#include <Math/Vec2.h>

class SDL_Window;
class SDL_Renderer;

class Window final
{
private:
	Window() = default;
	~Window() = default;

	std::string mTitle;

	Vec2 mSize { 800, 600 };
	Vec2 mScale { 1, 1 };

	SDL_Window* mpWindow { nullptr };
    SDL_Renderer* mpRenderer { nullptr };

public:

	Window(const Window&) = delete;
	Window(Window&&) = delete;

	Window& operator=(const Window&) = delete;
	Window& operator=(Window&&) = delete;

	static Window& Instance();

	bool Initialize(const std::string& title, const Vec2& size, std::string& sProblem);

	void Finalize();

	void SetClearColor(int r, int g, int b);

	void Draw(SDL_Texture* pTexture, SDL_Rect& srcRect, SDL_Rect& dstRect);
	void Clear();
    void Render();

	SDL_Window* GetWindowPointer();
	SDL_Renderer* GetRendererPointer();
};