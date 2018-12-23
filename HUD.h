#ifndef HUD__H
#define HUD__H

#include <iostream>
#include <string>
#include <SDL.h>
#include <SDL_ttf.h>
#include "gameData.h"


class HUD{
private:
	HUD();
	HUD(const HUD& h);

  
 	SDL_Color textColor;
 	int xLoc, yLoc;
 	int HUDwidth, HUDheight;
 	TTF_Font* font;
 	bool isVisible;




public:
	static HUD& getInstance();
	void toggleIsVisible();
	bool getIsVisible();
	void drawHUD() const;

};
#endif
