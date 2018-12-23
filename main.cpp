// Brian Malloy        Data-Driven Object oriented Game Construction
#include "engine.h"

//ImageFactory*  ImageFactory::instance = NULL; //gof
//RenderContext* RenderContext::instance = NULL; //gof

int main(int, char*[]) {
    srand(time(0));
    bool keepPlaying = true;
   try {
      while( keepPlaying ){
        Engine engine;
        keepPlaying = engine.play();
      }
   }
   catch (const string& msg) { std::cout << msg << std::endl; }
   catch (...) {
      std::cout << "Oops, someone threw an exception!" << std::endl;
   }
   return 0;
}
