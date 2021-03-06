#include "helpers.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;

const int PLAYER_WIDTH = 20;
const int PLAYER_HEIGHT = 5;

const int BALL_SIZE = 10;
const int MAX_BALLS = 25;

const int BALL_VELOCITY = 2;

extern SDL_Surface *background;
extern SDL_Surface *screen;
extern SDL_Surface* message;
extern SDL_Surface* player;
extern SDL_Surface* ball;

extern SDL_Event event;
extern TTF_Font *font;

extern SDL_Rect balls[MAX_BALLS];

extern SDL_Color textColor;

bool init();
bool load_files();
void clean_up();

void main_game();
void game_over();