#include <vector>
#include <SDL.h>
#include "ioMod.h"
#include "renderContext.h"
#include "clock.h"
#include "world.h"
#include "viewport.h"
#include "player.h"
#include "collisionStrategy.h"
#include "HUD.h"
#include "sound.h"

class Engine {
public:
  Engine ();
  ~Engine ();
  bool play();
  void switchSprite();


private:
  const RenderContext& rc;
  const IoMod& io;
  Clock& clock;

  SDL_Renderer * const renderer;

  World sky;
  World sea;
  World cloud;
  World land;

  Viewport& viewport;

  Player* player;

  std::vector<SmartSprite*> mySmartSprites;
  int currentSprite;
  //std::vector<CollisionStrategy*> strategies;
  CollisionStrategy* strategy;
  //int currentStrategy;

  bool collision;
  HUD& hud;

  std::vector<Drawable*> regularSprites;
  std::vector<Drawable*> paintersAlgoBack;
  std::vector<Drawable*> paintersAlgoMiddle;
  std::vector<Drawable*> paintersAlgoFront;



  bool godMode;
  int pointCounter;
  SDLSound sound;
  bool makeVideo;


  void draw() const;
  void update(Uint32);

  //added delete to explicitly disallow compiler generated funcs
  Engine(const Engine&) = delete;
  Engine& operator=(const Engine&) = delete;

  //void printScales() const; not sure hwat ithis is for
  void checkForCollisions();
  void toggleGodMode();
  void drawEngineHUD();

};
