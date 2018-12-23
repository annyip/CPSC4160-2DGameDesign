#include <string>
#include <vector>
#include <map>
#include "image.h"
#include "gameData.h"
#include "spriteSheet.h"

class ImageFactory {
public:

  //static ImageFactory* getInstance(); //gof
  static ImageFactory& getInstance();
  ~ImageFactory();

  Image* getImage(const std::string&);
  std::vector<Image*> getImages(const std::string&);

private:
  //static ImageFactory* instance; //gof
  const Gamedata& gdata;

  std::map<std::string, SDL_Surface*> surfaces;
  std::map<std::string, SDL_Texture*> textures;
  std::map<std::string, Image*> images;

  std::map<std::string, std::vector<SDL_Surface*> > multiSurfaces;
  std::map<std::string, std::vector<SDL_Texture*> > multiTextures;
  std::map<std::string, std::vector<Image*> > multiImages;

  ImageFactory() : 
    gdata( Gamedata::getInstance() ), 
    surfaces(),
    textures(),
    images(),
    multiSurfaces(),
    multiTextures(),
    multiImages()
  {}
  
  //added delete to explicitly disallow compiler generated funcs
  ImageFactory(const ImageFactory&) = delete; 
  ImageFactory& operator=(const ImageFactory&) = delete;
};
