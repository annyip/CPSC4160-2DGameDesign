#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
#include <random>
#include <iomanip>
#include "sprite.h"
#include "multisprite.h"
#include "gameData.h"
#include "engine.h"
#include "frameGenerator.h"
#include "TwoWayMultiSprite.h"
#include "player.h"
#include "smartSprite.h"
#include "HUD.h"
#include "painterSprite.h"



Engine::~Engine() {

 for(auto & x : mySmartSprites){
  delete x;
 }

 delete player;
 delete strategy;

 for(auto &y : regularSprites){
  delete y;
 }

for(auto &f : paintersAlgoFront) delete f;
for(auto &f : paintersAlgoMiddle) delete f;
for(auto &f : paintersAlgoBack) delete f;
  std::cout << "Terminating program" << std::endl;
}

Engine::Engine() :
  rc( RenderContext::getInstance() ),
  io( IoMod::getInstance() ),
  clock( Clock::getInstance() ),
  renderer( rc.getRenderer() ),
  sky("back", Gamedata::getInstance().getXmlInt("back/factor") ),
  sea("middle", Gamedata::getInstance().getXmlInt("middle/factor") ),
  cloud("middle2", Gamedata::getInstance().getXmlInt("middle2/factor") ),
  land("front", Gamedata::getInstance().getXmlInt("front/factor") ),
  viewport( Viewport::getInstance() ),
  player(new Player("BeachDude")),
  mySmartSprites(),
  currentSprite(0),
  strategy(new MidPointCollisionStrategy),
  collision(false),
  hud(HUD::getInstance()),
  regularSprites({
                  new TwoWayMultiSprite("Bird"),
                  new TwoWayMultiSprite("Boat"),
                  new TwoWayMultiSprite("Bird"),
                }),
  paintersAlgoBack(),
  paintersAlgoMiddle(),
  paintersAlgoFront(),
  godMode(false),
  pointCounter(0),
  sound( ),
  makeVideo(false)
{
  regularSprites[1]->setScale(0.6);
  mySmartSprites.reserve(Gamedata::getInstance().getXmlInt("Crab/numCrabs"));
  for(int i = 0; i < Gamedata::getInstance().getXmlInt("Crab/numCrabs"); i++){
    mySmartSprites.push_back(new SmartSprite("Crab", player->getPosition(), player->getScaledWidth(), player->getScaledHeight()));

    while(mySmartSprites[i]->getX() >= player->getX() - 200 && mySmartSprites[i]->getX() <= player->getX() + 200){
      printf("crap I messed up the crab position - gotta do it again\n");
      mySmartSprites[i]->setX( rand() % Gamedata::getInstance().getXmlInt("world/width") );
    }

    player->attach(mySmartSprites[i]);
  }

  int NumFlowers = Gamedata::getInstance().getXmlInt("Flowers/NumFlowers");
  paintersAlgoBack.reserve(NumFlowers);
  paintersAlgoFront.reserve(NumFlowers);
  paintersAlgoMiddle.reserve(NumFlowers);

  for(int i = 0 ; i < static_cast<int>(NumFlowers/3); i++){ //3
    //Drawable* s = new painterSprite("BeachBall");
    //s->setScale(0);
    //paintersAlgoFront.push_back(s);

    paintersAlgoFront.push_back(new painterSprite("Flowers"));
    paintersAlgoFront[i]->setScale(1);
    paintersAlgoFront[i]->setX( rand() % Gamedata::getInstance().getXmlInt("world/width") );
    //paintersAlgoFront[i]->setY( rand() % Gamedata::getInstance().getXmlInt("world/height") );
    paintersAlgoFront[i]->setVelocityY( Gamedata::getInstance().getRandFloat(40,250) );

  }

  for(int i = 0 ; i < static_cast<int>(NumFlowers/2); i++){ //2
    //Drawable* s = new painterSprite("BeachBall");
    //s->setScale(0.5);
    //paintersAlgoMiddle.push_back(s);
    paintersAlgoMiddle.push_back(new painterSprite("Flowers"));
    paintersAlgoMiddle[i]->setScale(0.5);
    paintersAlgoMiddle[i]->setX( rand() % Gamedata::getInstance().getXmlInt("world/width") );
    paintersAlgoMiddle[i]->setVelocityY( Gamedata::getInstance().getRandFloat(40,250) );

  }

  for(int i = 0 ; i < NumFlowers; i++){
    //Drawable* s = new painterSprite("BeachBall");
    //s->setScale(0.5);
    paintersAlgoBack.push_back(new painterSprite("Flowers"));
    paintersAlgoBack[i]->setScale(0.2);
    paintersAlgoBack[i]->setX( rand() % Gamedata::getInstance().getXmlInt("world/width") );
    //paintersAlgoFront[i]->setY( rand() % Gamedata::getInstance().getXmlInt("world/height") );
    paintersAlgoBack[i]->setVelocityY( Gamedata::getInstance().getRandFloat(40,250) );
  }


  Viewport::getInstance().setObjectToTrack(player);


  std::cout << "Loading complete" << std::endl;
}

void Engine::drawEngineHUD(){
  int xLoc = 500;
  int yLoc = 100;
  int HUDwidth = 300;
  int HUDheight = 300;
  SDL_Rect rect = {xLoc, yLoc, HUDwidth, HUDheight};;

  SDL_Renderer* renderer = IoMod::getInstance().getRenderer();

  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  SDL_SetRenderDrawColor( renderer, 0, 0, 255, 255/4 );
  SDL_RenderFillRect ( renderer, &rect );
  SDL_RenderDrawRect( renderer, &rect );
}

void Engine::draw() const {
  sky.draw();
  for(auto &back : paintersAlgoBack){
    back->draw();
  }

  sea.draw();

  for(auto &y : regularSprites){
    y->draw();
  }

  for(auto &mid : paintersAlgoMiddle){
    mid->draw();
  }

  cloud.draw();

  land.draw();

  player->draw();

  for(auto &x : mySmartSprites){
    x->draw();
  }

  for(auto &z : paintersAlgoFront){
    z->draw();
  }


  hud.drawHUD();
  viewport.draw();
  //Display FPS and my name to screen
  std::stringstream ss;
  SDL_Color c = {0,0,0,255};
  //s << "FPS: " << clock.getFps();
  ss << "Ann Yip";
  //io.writeText(s.str(), 30, 870, c);
  io.writeText(ss.str(), 30, 900, c);

  std::stringstream str;

  SDL_Rect rect = {760, 100, 200, 100};

  SDL_Renderer* renderer = IoMod::getInstance().getRenderer();

  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  SDL_SetRenderDrawColor( renderer, 0, 0, 255, 255/4);
  SDL_RenderFillRect ( renderer, &rect );
  SDL_RenderDrawRect( renderer, &rect );

  if(godMode){
    str << "God Mode - On";
    io.writeText(str.str(), 770, 170, c);
  }
  else{
    str << "God Mode - Off";
    io.writeText(str.str(), 770, 170, c);
  }

  str.str("");
  str.clear();

  str << "Lives: " << player->getPlayerLives() << std::endl;
  io.writeText(str.str(), 770, 140, c);

  str.str("");
  str.clear();

  str << "Points: " << pointCounter << std::endl;
  io.writeText(str.str(), 770, 110, c);


  if(player->getPlayerLives() > 0 && pointCounter >= 100){
    //YOU WON
    std::stringstream myString;
    myString << "You won! - press R to restart" << std::endl;
    SDL_Rect rect = {0, 0, 990, 1000};

    SDL_Renderer* renderer = IoMod::getInstance().getRenderer();

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255/1.5);
    SDL_RenderFillRect ( renderer, &rect );
    SDL_RenderDrawRect( renderer, &rect );
    SDL_Color textColor = {255,255,255,255};
    io.writeText(myString.str(), 390, 380, textColor);
    clock.pause();

  }
  else if(player->getPlayerLives() <= 0){
    //YOU LOST
    std::stringstream myString;
    myString << "You lost! - press R to restart" << std::endl;

    SDL_Rect rect = {0, 0, 990, 1000};

	 	SDL_Renderer* renderer = IoMod::getInstance().getRenderer();

		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	  SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255/1.5);
	  SDL_RenderFillRect ( renderer, &rect );
		SDL_RenderDrawRect( renderer, &rect );
    SDL_Color textColor = {255,255,255,255};
    io.writeText(myString.str(), 390, 380, textColor);
    clock.pause();
  }


  SDL_RenderPresent(renderer);
}

void Engine::checkForCollisions() {
  collision = false;

  std::vector<SmartSprite *>::iterator it = mySmartSprites.begin();// it = mySmartSprites.begin();
  while (it != mySmartSprites.end()) {

    //if player collides with crab
      if (!godMode && strategy->execute(*player, **it) && (*it)->isExploding() == false) {
        sound[1];
        collision = true;
        player->explode();
        (*it)->explode();


        //greater than or eq to player x and less than equal to player x
        //use this to put the crab to a random place after it dies
          while((*it)->getX() > player->getX() - 300 && (*it)->getX() < player->getX() + 300){
            //printf("this is the old x for dead crab: %f\n", (*it)->getX());

            (*it)->setX( rand() % Gamedata::getInstance().getXmlInt("world/width") + 1);
            //printf("this is the new x for resurected crab: %f\n", (*it)->getX());
          }

          std::cout << "In Engine collision check - Player Lives: " << player->getPlayerLives() << std::endl;
          ++it;
      }
      else if(player->collidedWith(*it)){
        collision = true;

        dynamic_cast<SmartSprite*>(*it)->explode();
        sound[0];
        pointCounter = pointCounter + 5;
        std::cout << "Da points: " << pointCounter << std::endl;

        while((*it)->getX() > player->getX() - 300 && (*it)->getX() < player->getX() + 300){
          //printf("this is the old x for crab death by pineapple: %f\n", (*it)->getX());

          (*it)->setX( rand() % Gamedata::getInstance().getXmlInt("world/width") + 1);
          //printf("this is the new x for the crab: %f\n", (*it)->getX());
        }
        ++it;
      }

    else ++it;
  }
}

void Engine::update(Uint32 ticks) {

  checkForCollisions();

  player->update(ticks);

  for(auto &y : mySmartSprites){
    y->update(ticks);
  }

  for(auto &x : regularSprites){
    x->update(ticks);
  }

  for(auto &z : paintersAlgoFront){
    z->update(ticks);
  }

  for(auto &b : paintersAlgoBack){
    b->update(ticks);
  }

  for(auto &m : paintersAlgoMiddle){
    m->update(ticks);
  }

  sky.update();
  cloud.update();
  sea.update();
  land.update();


  viewport.update(); // always update viewport last
}

void Engine::switchSprite(){
  ++currentSprite;
  currentSprite = currentSprite % 2;
  if ( currentSprite ) {
    Viewport::getInstance().setObjectToTrack(player);
  }
  else {
    Viewport::getInstance().setObjectToTrack(mySmartSprites[2]);
  }
}

void Engine::toggleGodMode(){
  if(godMode) godMode = false;
  else godMode = true;
}

bool Engine::play() {
  SDL_Event event;
  const Uint8* keystate;
  bool done = false;
  Uint32 ticks = clock.getElapsedTicks();
  FrameGenerator frameGen;


  while ( !done ) {
    // The next loop polls for events, guarding against key bounce:
    while ( SDL_PollEvent(&event) ) {
      keystate = SDL_GetKeyboardState(NULL);
      if (event.type ==  SDL_QUIT) { done = true; break; }
      if(event.type == SDL_KEYDOWN) {
        if (keystate[SDL_SCANCODE_ESCAPE] || keystate[SDL_SCANCODE_Q]) {
          done = true;
          break;
        }
        if ( keystate[SDL_SCANCODE_P] ) {
          if ( clock.isPaused() ) clock.unpause();
          else clock.pause();
        }
        if ( keystate[SDL_SCANCODE_T] ) {
          switchSprite();
        }
        if (keystate[SDL_SCANCODE_SPACE]) {
          sound[2];
          player->shoot();

        }
        if(keystate[SDL_SCANCODE_G]){
          toggleGodMode();
        }
        if ( keystate[SDL_SCANCODE_R] ) {
          clock.unpause();
          return true;
        }

        if (keystate[SDL_SCANCODE_F4] && !makeVideo) {
          std::cout << "Initiating frame capture" << std::endl;
          makeVideo = true;
        }
        else if (keystate[SDL_SCANCODE_F4] && makeVideo) {
          std::cout << "Terminating frame capture" << std::endl;
          makeVideo = false;
        }

        if (keystate[SDL_SCANCODE_F1]) {
          hud.toggleIsVisible();
        }
      }
}

    // In this section of the event loop we allow key bounce:

    //my player will only go left or right
    ticks = clock.getElapsedTicks();
    if ( ticks > 0 ) {
      clock.incrFrame();


      if (keystate[SDL_SCANCODE_A]) {
        static_cast<Player*>(player)->left();

       //std::cout<< "pressed A for left" << std::endl;
      }
      if (keystate[SDL_SCANCODE_D]) {
        static_cast<Player*>(player)->right();

        //std::cout << "pressed D for right " << std::endl;
      }

      draw();
      update(ticks);
      if ( makeVideo ) {
        frameGen.makeFrame();
      }
    }
  }

  return false;
}
