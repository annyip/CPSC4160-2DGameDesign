#include "HUD.h"
#include "gameData.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include "ioMod.h"
#include "clock.h"
#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
#include <random>
#include <iomanip>


HUD::HUD():
	textColor(),
	xLoc(Gamedata::getInstance().getXmlInt("HUD/location/x")),
	yLoc(Gamedata::getInstance().getXmlInt("HUD/location/y")),
	HUDwidth(Gamedata::getInstance().getXmlInt("HUD/width")),
	HUDheight(Gamedata::getInstance().getXmlInt("HUD/height")),
	font(TTF_OpenFont(Gamedata::getInstance().getXmlStr("HUD/fontType").c_str(),
                    Gamedata::getInstance().getXmlInt("HUD/fontSize"))),
	isVisible(true)
	//c(Clock::getInstance())
{
	textColor.r = Gamedata::getInstance().getXmlInt("HUD/fontColorR");
	textColor.g = Gamedata::getInstance().getXmlInt("HUD/fontColorG");
	textColor.b = Gamedata::getInstance().getXmlInt("HUD/fontColorB");

}

HUD::HUD(const HUD& h):
	textColor(h.textColor),
	xLoc(h.xLoc),
	yLoc(h.yLoc),
	HUDwidth(h.HUDwidth),
	HUDheight(h.HUDheight),
	font(h.font),
	isVisible(h.isVisible)
	//c(h.c)
{}

void HUD::drawHUD() const {
	if(isVisible){
		SDL_Rect rect = {xLoc, yLoc, HUDwidth, HUDheight};

	 	SDL_Renderer* renderer = IoMod::getInstance().getRenderer();

		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	  SDL_SetRenderDrawColor( renderer, 0, 0, 255, 255/4 );
	  SDL_RenderFillRect ( renderer, &rect );
		SDL_RenderDrawRect( renderer, &rect );

		IoMod::getInstance().writeText("Welcome to the beach!", 50, 120, textColor, *font);
		IoMod::getInstance().writeText("How to play:", 50, 150, textColor, *font);
		IoMod::getInstance().writeText("A to move left", 50, 180, textColor, *font);
		IoMod::getInstance().writeText("D to move right", 50, 200, textColor, *font);
		IoMod::getInstance().writeText("Space to shoot", 50, 220, textColor, *font);
		IoMod::getInstance().writeText("Press F1 to show/hide HUD", 50, 240, textColor, *font);
		IoMod::getInstance().writeText("Shoot the crabs!", 50, 300, textColor, *font);
		IoMod::getInstance().writeText("Get 100 pts to win", 50, 320, textColor, *font);
		IoMod::getInstance().writeText("Don't get too close, they bite!", 50, 340, textColor, *font);

		std::stringstream ss;
		ss << "FPS: " << Clock::getInstance().getFps();
		IoMod::getInstance().writeText(ss.str(), 50, 260, textColor, *font);

	}
}

HUD& HUD::getInstance(){
	static HUD instance;
	return instance;
}

void HUD::toggleIsVisible() {
	if(isVisible) isVisible = false;
	else isVisible = true;
}

bool HUD::getIsVisible(){
	return isVisible;
}
