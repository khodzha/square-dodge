#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;

const int PLAYER_WIDTH = 20;
const int PLAYER_HEIGHT = 5;

const int BALL_SIZE = 10;
const int MAX_BALLS = 25;

const int BALL_VELOCITY = 2;

SDL_Surface *background = NULL;
SDL_Surface *screen = NULL;
SDL_Surface* message = NULL;
SDL_Surface* player = NULL;
SDL_Surface* ball = NULL;

SDL_Event event;
TTF_Font *font = NULL;

SDL_Rect balls[MAX_BALLS];

SDL_Color textColor = { 0, 0, 0 };

void main_game();
void game_over();

bool intersects(SDL_Rect a, SDL_Rect b)
{
	if(a.x + a.w < b.x || b.x+b.w < a.x || a.y+ a.h < b.y || b.y+b.h < a.y)
		return false;
	return true;
}

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL )
{
	SDL_Rect offset;
	offset.x = x;
	offset.y = y;
	SDL_BlitSurface( source, clip, destination, &offset );
}

SDL_Surface *load_image( std::string filename )
{
    
    SDL_Surface* loadedImage = NULL;
    SDL_Surface* optimizedImage = NULL;
    loadedImage = IMG_Load( filename.c_str() );
    
    if( loadedImage != NULL )
    {
        optimizedImage = SDL_DisplayFormat( loadedImage );
        SDL_FreeSurface( loadedImage );
        
        if( optimizedImage != NULL )
        {
            SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, SDL_MapRGB( optimizedImage->format, 0, 0xFF, 0xFF ) );
        }
    }
    
    return optimizedImage;
}

bool init()
{
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        return false;
    }
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );
    
    if( screen == NULL )
    {
        return false;
    }
    
    if( TTF_Init() == -1 )
    {
        return false;
    }
	srand ( time(NULL) );    
    SDL_WM_SetCaption( "Poop", NULL );
    
    return true;
}

bool load_files()
{
    
    background = load_image( "background.png" );
    font = TTF_OpenFont( "lazy.ttf", 24 );

    player = load_image( "player_rocket.bmp" );
    ball = load_image( "enemy_ball.bmp" );

    if( background == NULL )
    {
        return false;
    }
    
    if( font == NULL )
    {
        return false;
    }

    return true;
}

void clean_up()
{
    
    SDL_FreeSurface( background );
    SDL_FreeSurface( message );
    SDL_FreeSurface( screen );
    SDL_FreeSurface( ball );
    
    TTF_CloseFont( font );
    TTF_Quit();
    
    SDL_Quit();
}
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

void main_game()
{
	bool quit = false;
	int player_position = SCREEN_WIDTH/2;
	Uint8 *keystates = NULL;
	int start_time  = SDL_GetTicks();
	int current_balls = 0;
	int i = 0;
	while (quit == false)
	{
		if (SDL_GetTicks() - start_time > 1)
		{
			start_time = SDL_GetTicks();
			for (i = 0; i < current_balls; i++)
			{
				balls[i].y += BALL_VELOCITY;
			}
		}
		if (current_balls < MAX_BALLS)
		{
			for ( i = 0; i < MAX_BALLS; i++)
			{
				if (balls[i].y > SCREEN_HEIGHT || balls[i].y == 0)
				{
					SDL_Rect new_ball;
					new_ball.x = BALL_SIZE/2 + rand() % (SCREEN_WIDTH - BALL_SIZE/2);
					new_ball.y = -(5+rand()% 350);
					new_ball.w = new_ball.h = BALL_SIZE;
					balls[i] = new_ball;
				}
			}
			current_balls = MAX_BALLS;
		}
		if( SDL_PollEvent( &event ) )
		{
			if( event.type == SDL_QUIT )
			{
				quit = true;
			}
		}

		keystates = SDL_GetKeyState( NULL );

		if( keystates[SDLK_ESCAPE] )
		{
			quit = true;
		}

		if( keystates[SDLK_LEFT] && player_position > PLAYER_WIDTH/2 )
		{
			player_position--;
		}

		if( keystates[SDLK_RIGHT] && player_position < SCREEN_WIDTH - PLAYER_WIDTH/2)
		{
			player_position++;
		}

		apply_surface( 0, 0, background, screen );
		for (i = 0; i < MAX_BALLS; i++)
		{
			// printf("ball %i: %i %i\n",i , balls[i].x, balls[i].y);
			apply_surface ( balls[i].x - BALL_SIZE/2, balls[i].y - BALL_SIZE/2, ball, screen);
			if (balls[i].y > SCREEN_HEIGHT )
			{
				current_balls--;
			}
			SDL_Rect player_rect;
			player_rect.x = player_position - PLAYER_WIDTH/2;
			player_rect.y = SCREEN_HEIGHT - PLAYER_HEIGHT;
			player_rect.w = PLAYER_WIDTH;
			player_rect.h = PLAYER_HEIGHT;
			if( intersects(balls[i], player_rect) )
			{
				game_over();
				quit = true;
			}
		}
		apply_surface( player_position - PLAYER_WIDTH/2, SCREEN_HEIGHT - PLAYER_HEIGHT, player, screen );

		SDL_Flip( screen );
	}
}

void game_over()
{
	message = TTF_RenderText_Solid( font, "Game over", textColor );
	apply_surface(0, 0, background, screen);
	apply_surface( ( SCREEN_WIDTH - message->w ) / 2, ( SCREEN_HEIGHT - message->h ) / 2, message, screen );
	SDL_Flip( screen );
	while (true)
	{
		if( SDL_PollEvent( &event ) )
		{
			if( event.type == SDL_QUIT || event.type == SDL_KEYDOWN)
			{
				break;
			}
		}
	}
}