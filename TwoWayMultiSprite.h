#ifndef TWOWAYMULTISPRITE__H
#define TWOWAYMULTISPRITE__H
#include "drawable.h"
#include <string>
#include <vector>
#include <cmath>

class TwoWayMultiSprite: public Drawable{

  public:
    TwoWayMultiSprite(const std::string&);
    //TwoWayMultiSprite(const std::string&, bool randomLoc);
    TwoWayMultiSprite(const TwoWayMultiSprite&);

    virtual void draw() const;
    virtual void update(Uint32 ticks);

    virtual const Image* getImage() const { 
      return images[currentFrame]; 
    }
    int getScaledWidth()  const { 
    return getScale()*images[currentFrame]->getWidth(); 
    } 
    int getScaledHeight()  const { 
      return getScale()*images[currentFrame]->getHeight(); 
    } 
    virtual const SDL_Surface* getSurface() const { 
      return images[currentFrame]->getSurface();
    }



  protected: 
    std::vector<Image*> images;
    
    unsigned currentFrame;
    unsigned numberOfFrames;
    unsigned frameInterval;
    float timeSinceLastFrame;
    int worldWidth;
    int worldHeight;

    void advanceFrame(Uint32 ticks);
    TwoWayMultiSprite& operator=(const TwoWayMultiSprite&);
};
#endif
