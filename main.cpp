#include <stdlib.h>
#include <stdio.h>
#include "game_states.h"

int main(int argc, char* argv[])
{
    bool quit = false;

    if( init() == false )
    {
        return 1;
    }

    if( load_files() == false )
    {
        return 1;
    }

    message = TTF_RenderText_Solid( font, "Press space to start, any other to quit", textColor );
	apply_surface( 0, 0, background, screen );
	apply_surface( ( 640 - message->w ) / 2, ( 480 - message->h ) / 2, message, screen );
	SDL_Flip( screen );
    while (quit == false)
    {
		if( SDL_PollEvent( &event ) )
		{
			if( event.type == SDL_KEYDOWN )
			{
				switch( event.key.keysym.sym )
				{
					case SDLK_SPACE:
					{
						message = NULL;
						main_game();
						quit = true;
						break;
					}
					default: quit = true; break;
				}
			}

			else if( event.type == SDL_QUIT )
			{
				quit = true;
			}
		}
    }

    clean_up();
	return 0;
}
