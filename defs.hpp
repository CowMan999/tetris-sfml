void load();
void draw();
void hold();
void reset();
void pause();
void hard_drop();
void update_nextblock();
float get_drop_speed(int l);
void update(float deltatime);
void check_tetromino_collision();
void update_score(unsigned int amount);
sf::View getLetterboxView(sf::View view, int windowWidth, int windowHeight);

typedef std::array<std::array<bool,5>,5> Grid;

// constants
const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 640;
const int GRID_WIDTH = 10;
const int GRID_HEIGHT = 20;
const int FRAMERATE = 120;

const std::string MENU_STR = "\n\n\n\n  Hold\n\n\n\n\n\n\n  Next\n\n\n\n\n\n\n\n\n\n\n  ";
const std::string TETRIS_STR = "Tetris!";
const std::string PAUSE_STR = "Paused.";
const std::string LEVEL_STR = "Level: ";
const std::string HIGH_STR = "Best: ";
const std::string SCORE_STR = "Score: ";


#ifdef MAIN_CPP
char bg_char_grid[GRID_WIDTH*GRID_HEIGHT] {
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
};
#endif