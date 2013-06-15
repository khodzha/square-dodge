#include "helpers.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;

const int PLAYER_WIDTH = 20;
const int PLAYER_HEIGHT = 5;

const int BALL_SIZE = 10;
const int MAX_BALLS = 25;

const int BALL_VELOCITY = 2;

static SDL_Surface *background = NULL;
static SDL_Surface *screen = NULL;
static SDL_Surface* message = NULL;
static SDL_Surface* player = NULL;
static SDL_Surface* ball = NULL;

static SDL_Event event;
static TTF_Font *font = NULL;

static SDL_Rect balls[MAX_BALLS];

static SDL_Color textColor = { 0, 0, 0 };

bool init();
bool load_files();
void clean_up();

void main_game();
void game_over();