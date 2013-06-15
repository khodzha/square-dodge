#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <time.h>

bool intersects(SDL_Rect a, SDL_Rect b);

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL );

SDL_Surface *load_image( std::string filename );