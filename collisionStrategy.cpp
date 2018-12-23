#include <vector>
#include <algorithm>
#include "ioMod.h"
#include "collisionStrategy.h"
#include "viewport.h"
#include "renderContext.h"

float MidPointCollisionStrategy::
distance(float x1, float y1, float x2, float y2) const {
  float x = x1-x2, y = y1-y2;
  return hypot(x, y);
}

void MidPointCollisionStrategy::draw() const {
  IoMod::getInstance().writeText("Strategy: Distance", 500, 30);
}

bool MidPointCollisionStrategy::execute(
      const Drawable& sprite1, const Drawable& sprite2) const {

  int width1 = sprite1.getScaledWidth();
  int width2 = sprite2.getScaledWidth();
  int height1 = sprite1.getScaledHeight();
  int height2 = sprite2.getScaledHeight();

  int COLLISION_DISTANCE = width1/2 + width2/2;
  float x1 = sprite1.getX() + width1/2;
  float y1 = sprite1.getY() + height1/2;
  float x2 = sprite2.getX() + width2/2;
  float y2 = sprite2.getY() + height2/2;
  return distance(x1, y1, x2, y2) < COLLISION_DISTANCE;
}

void RectangularCollisionStrategy::draw() const {
  IoMod::getInstance().writeText("Strategy: Rectangular", 500, 30);
}

bool RectangularCollisionStrategy::execute(
      const Drawable& sprite1, const Drawable& sprite2) const {
  float left1 = sprite1.getX();
  float left2 = sprite2.getX();

  float right1 = left1+sprite1.getScaledWidth();
  float right2 = left2+sprite2.getScaledWidth();
  if ( right1 < left2 ) return false;
  if ( left1 > right2 ) return false;
  float top1 = sprite1.getY();
  float top2 = sprite2.getY();
  float bottom1 = top1+sprite1.getScaledHeight();
  float bottom2 = top2+sprite2.getScaledHeight();
  if ( bottom1 < top2 ) return false;
  if ( bottom2 < top1 ) return false;
  return true;
}

// This is, essentially, SpriteSheet::cropSurface:
SDL_Surface* scaleSurface(const SDL_Surface* const surf, int width, int height) {

  const auto* fmt = surf->format;
  SDL_Surface* sub = SDL_CreateRGBSurface(0,width,height,
                     fmt->BitsPerPixel,fmt->Rmask,fmt->Gmask,
                     fmt->Bmask,fmt->Amask);

  // Get the old mode
  SDL_BlendMode oldBlendMode;
  SDL_GetSurfaceBlendMode(const_cast<SDL_Surface*>(surf), &oldBlendMode);

  // Set the new mode so copying the source won't change the source
  SDL_SetSurfaceBlendMode(const_cast<SDL_Surface*>(surf), SDL_BLENDMODE_NONE);

  SDL_Rect rect = {0,0,width,height};
  int flag = SDL_BlitScaled(const_cast<SDL_Surface*>(surf),nullptr,sub,&rect);
  if ( flag < 0 ) throw( std::string("SDL_BlitScaled failed!") );
  //std::cout << width  << ", " << surf->w << ", " << sub->w << std::endl;

  // Set the blend mode back to original
  SDL_SetSurfaceBlendMode(const_cast<SDL_Surface*>(surf), oldBlendMode);
  return sub;
}
