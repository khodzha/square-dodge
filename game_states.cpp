#include "game_states.h"

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
    
    background = load_image( "assets/background.png" );
    font = TTF_OpenFont( "assets/Terminus.ttf", 24 );

    player = load_image( "assets/player_rocket.bmp" );
    ball = load_image( "assets/enemy_ball.bmp" );

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

void main_game()
{
	bool quit = false;
	int player_position = SCREEN_WIDTH/2;
	Uint8 *keystates = NULL;
	int start_time  = SDL_GetTicks();
	int current_balls = 0;
	int i = 0;

	int fps_timer = 0;
	int delay = 0;
	int frames = 0;
	int fps_calc_timer = SDL_GetTicks();

	while (quit == false)
	{
		fps_timer = SDL_GetTicks();
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

		std::stringstream caption;
		caption << "FPS: " << (int)(frames*1000.0/(SDL_GetTicks() - fps_calc_timer+1));
		message = TTF_RenderText_Solid( font, caption.str().c_str(), textColor );
		if (SDL_GetTicks() - fps_calc_timer > 5000)
		{
			frames = 1;
			fps_calc_timer = SDL_GetTicks();
		}
		apply_surface(10,10, message, screen);

		SDL_Flip( screen );
		frames++;

		delay = SDL_GetTicks() - fps_timer;

		if( delay < 1000 / FRAMES_PER_SECOND )
        {
            SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - delay );
        }
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
