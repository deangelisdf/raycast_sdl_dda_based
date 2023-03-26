#include "SDLWindow.h"

SDLWindow::SDLWindow() :win(NULL), surface_win(NULL), ren(NULL), hardware(true), idwin(-1){}

SDLWindow::~SDLWindow()
{
	if (surface_win) SDL_FreeSurface(surface_win);
	if (ren) SDL_DestroyRenderer(ren);
	if (win) SDL_DestroyWindow(win);
}

void SDLWindow::Init(int wm, int hm, Uint32 flags)
{
	win = SDL_CreateWindow("SDLWindow", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, wm, hm, flags);
	if (win != NULL)
	{
		ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
		if (ren)
		{
			SDL_SetRenderDrawColor(ren, 0, 0, 0, 0xFF);
		}
		else
		{
			hardware = false;
			surface_win = SDL_GetWindowSurface(win);
		}
	}
	idwin = SDL_GetWindowID(win);
}

SDL_Rect SDLWindow::getRect() const
{
	SDL_Rect rect;
	SDL_GetWindowSize(win, &rect.w, &rect.h);
	SDL_GetWindowPosition(win, &rect.x, &rect.y);
	return rect;
}

void SDLWindow::setTitle(const char* str)
{
	SDL_SetWindowTitle(win, str);
}