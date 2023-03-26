#pragma once

#include <SDL.h>

class SDLWindow
{
protected:
	int idwin;
	bool hardware;
	SDL_Window* win;
	SDL_Renderer* ren;
	SDL_Surface* surface_win;
public:
	SDLWindow();
	virtual ~SDLWindow();
	void Init(int w, int h, Uint32 flags);
	bool isAccelerated() const { return hardware; }
	inline SDL_Renderer* const getRenderer() const { return ren; }
	inline SDL_Surface* const getSurface() const { return surface_win; }
	inline int getID() { return idwin; }
	inline operator SDL_Window* () { return win; }
	SDL_Rect getRect() const;
	void setTitle(const char* str);
};

