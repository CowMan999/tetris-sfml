#define MAIN_CPP

#include "includes.hpp"

/*
Tetris © 1985~2022 Tetris Holding.
Tetris logos, Tetris theme song and Tetriminos are trademarks of Tetris Holding.
The Tetris trade dress is owned by Tetris Holding.
Licensed to The Tetris Company.
Tetris Game Design by Alexey Pajitnov.
Tetris Logo Design by Roger Dean.
All Rights Reserved.
*/

// globals
float deltatime = 0.0f;
char* filename;
float update_interval = get_drop_speed(1);
float update_timer = 0.0f;
unsigned int score = 0;
int level = 1;
int lines = 0;
size_t highscore;

bool heldblock = false;
bool paused = false;
Dialog* loading_dialog;

sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), TETRIS_STR);
sf::Event event;
sf::Clock deltaclock;

TileMap background;
sf::Texture tile;
sf::Font font;
sf::Text score_text, menu_text, level_text, high_text;
sf::Image logo;

sf::Music song;
sf::Sound thump, clear;
sf::SoundBuffer thump_b, clear_b;

std::vector<sf::Sprite*> leftover_blocks;
std::array<sf::Texture, 7> holdblock_textures;

// held block
Tetromino::Type holdblock = Tetromino::None;
sf::Sprite holdblock_sprite;

// next block
Tetromino::Type nextblock = Tetromino::None;
sf::Sprite nextblock_sprite;

// random bag
RandBag randbag;

Tetromino* current_tetromino = nullptr;
Tetromino* ghost_tetromino = nullptr;

int main(int argc, char** argv)
{
	// init certain globals
	filename = argv[0];
	(void)argc;
	chdir(dirname(filename));

	window.setFramerateLimit(FRAMERATE);
	font.loadFromFile("resources/font.ttf");

	// load assets and initialize game
	loading_dialog = new Dialog("Loading...","","",&window,font, 48, 1, 1, false);
	std::thread load_thread(load);
	loading_dialog->display();

	// show splash screen
	Dialog splash_screen(TETRIS_STR, "Press enter to continue...", "", &window, font);
	splash_screen.display();

	song.play();
	thump.play();

	// slow pc's saving grace
	deltaclock.restart();

	// mainloop
	while (window.isOpen())
	{
		// deltatime
		deltatime = deltaclock.restart().asSeconds();

		bool input = 0;

		// handle events
		while (window.pollEvent(event))
		{
			switch(event.type)
			{
				case sf::Event::Closed:
					window.close();
					break;

				case sf::Event::KeyPressed:

					if(current_tetromino != nullptr)
					{
						switch (event.key.code)
						{
							case sf::Keyboard::Left:   if(!paused) { current_tetromino->move(-32,0, &leftover_blocks); input = 1; } break;
							case sf::Keyboard::Right:  if(!paused) { current_tetromino->move(32,0, &leftover_blocks);  input = 1; } break;
							case sf::Keyboard::Down:   if(!paused) { current_tetromino->move(0,32, &leftover_blocks);  input = 1; } break;
							case sf::Keyboard::Up:     if(!paused) { current_tetromino->rotate(1, &leftover_blocks);   input = 1; } break;
							case sf::Keyboard::A:      if(!paused) { current_tetromino->move(-32,0, &leftover_blocks); input = 1; } break;
							case sf::Keyboard::D:      if(!paused) { current_tetromino->move(32,0, &leftover_blocks);  input = 1; } break;
							case sf::Keyboard::S:      if(!paused) { current_tetromino->move(0,32, &leftover_blocks);  input = 1; } break;
							case sf::Keyboard::W:      if(!paused) { current_tetromino->rotate(1, &leftover_blocks);   input = 1; } break;

							case sf::Keyboard::Z:      if(!paused) { current_tetromino->rotate(0, &leftover_blocks);   input = 1; } break;
							case sf::Keyboard::X:      if(!paused) { current_tetromino->rotate(1, &leftover_blocks);   input = 1; } break;

							case sf::Keyboard::Space:  if(!paused) { hard_drop();                                      input = 1; } break;

							case sf::Keyboard::P:      pause(); break;

							case sf::Keyboard::LShift: if(!paused) { hold(); } break;
							case sf::Keyboard::RShift: if(!paused) { hold(); } break;
							case sf::Keyboard::C:      if(!paused) { hold(); } break;

							default:
								break;
						}

						if(input)
						{
							check_tetromino_collision();
							update_ghost(current_tetromino, ghost_tetromino, &leftover_blocks);
						}
					}

					break;

				case sf::Event::Resized:
					window.setView(getLetterboxView(window.getView(), event.size.width, event.size.height));
					break;

				default:
					break;
			}
		}

		// update
		if(!paused)
			update(deltatime);

		// draw
		window.clear();
		draw();
		window.display();
	}

}

// loader functions to be called in threads
void load_holdblock_textures_thread1()
{
	holdblock_textures[Tetromino::I].loadFromFile("resources/I.png");
	holdblock_textures[Tetromino::J].loadFromFile("resources/J.png");
}

void load_holdblock_textures_thread2()
{
	holdblock_textures[Tetromino::L].loadFromFile("resources/L.png");
	holdblock_textures[Tetromino::O].loadFromFile("resources/O.png");
}

void load_holdblock_textures_thread3()
{
	holdblock_textures[Tetromino::S].loadFromFile("resources/S.png");
	holdblock_textures[Tetromino::T].loadFromFile("resources/T.png");
}

void load_holdblock_textures_thread4()
{
	holdblock_textures[Tetromino::Z].loadFromFile("resources/Z.png");
}

void load()
{
	// srand
	srand(time(NULL));

	// load tile - most used resource
	tile.loadFromFile("resources/tile.png");

	// load window icon and set framerate
	logo.loadFromFile("resources/logo.png");
	window.setIcon(logo.getSize().x, logo.getSize().y, logo.getPixelsPtr());

	// load tilemap
	background.load(tile, sf::Vector2u(32, 32), bg_char_grid, GRID_WIDTH, GRID_HEIGHT);

	// get highscore
	highscore = get_highscore();

	// load holdblock textures quickly with threads
	std::thread load_worker1(&load_holdblock_textures_thread1);
	std::thread load_worker2(&load_holdblock_textures_thread2);
	std::thread load_worker3(&load_holdblock_textures_thread3);
	std::thread load_worker4(&load_holdblock_textures_thread4);
	load_worker1.join();
	load_worker2.join();
	load_worker3.join();
	load_worker4.join();

	// load score text
	score_text.setFont(font);
	score_text.setCharacterSize(18);
	score_text.setPosition(GRID_WIDTH*32 + 32, 32);
	score_text.setFillColor(sf::Color::White);
	update_score(0);

	// create menu text
	menu_text = score_text;
	menu_text.setString(MENU_STR + TETRIS_STR);

	// create level text
	level_text = score_text;
	level_text.setPosition(GRID_WIDTH*32 + 32, GRID_HEIGHT*32 - 64);
	level_text.setString(LEVEL_STR + "1");
	level_text.setCharacterSize(14);

	// create high text
	high_text = score_text;
	high_text.setPosition(GRID_WIDTH*32 + 32, GRID_HEIGHT*32 - 32);
	high_text.setString(HIGH_STR + std::to_string(highscore));
	high_text.setCharacterSize(14);

	// init randbag
	randbag.reset();
	randbag.randomize();

	// init holdblock
	holdblock_sprite.setTexture(tile);
	holdblock_sprite.setPosition(GRID_WIDTH*32 + 64, 128);
	holdblock_sprite.setScale(2,2);

	// init nextblock
	nextblock_sprite.setTexture(tile);
	nextblock_sprite.setPosition(GRID_WIDTH*32 + 64, 256);
	nextblock_sprite.setScale(2,2);
	update_nextblock();

	// allocate space for one tetromino at location ghost_tetromino
	ghost_tetromino = new Tetromino(Tetromino::I, tile, &leftover_blocks);

	// load tetris theme
	song.openFromFile("resources/theme.ogg");
	song.setLoop(true);

	// load sounds
	clear_b.loadFromFile("resources/clear.wav");
	thump_b.loadFromFile("resources/thump.wav");
	clear.setBuffer(clear_b);
	thump.setBuffer(thump_b);

	// kill loading screen
	loading_dialog->kill();
}

void update_score(unsigned int amount)
{
	score += amount;
	score_text.setString(SCORE_STR + std::to_string(score));
	if(score > highscore)
	{
		highscore = score;
		high_text.setString(HIGH_STR + std::to_string(highscore));
	}
}

float get_drop_speed(int l)
{
	return pow((0.8-((l-1)*0.007)), l-1);
}

void draw()
{
	window.draw(background);

	if (ghost_tetromino != nullptr) ghost_tetromino->draw(window);
	if (current_tetromino != nullptr) current_tetromino->draw(window); 

	for(auto& block : leftover_blocks)
	{
		window.draw(*block);
	}

	window.draw(score_text);
	window.draw(level_text);
	window.draw(holdblock_sprite);
	window.draw(nextblock_sprite);
	window.draw(menu_text);
	window.draw(high_text);
}

void update_nextblock()
{
	nextblock = randbag.get_current();
	nextblock_sprite.setTexture(holdblock_textures[nextblock]);
}

void update(float deltatime)
{
	// if there is no current tetromino, spawn one
	if(current_tetromino == nullptr)
	{
		// get next tetromino type from randbag and use it
		heldblock = false;
		delete current_tetromino;
		current_tetromino = new Tetromino(randbag.get_next(), tile, &leftover_blocks);

		// update next tetromino
		update_nextblock();

		update_ghost(current_tetromino, ghost_tetromino, &leftover_blocks);

		if(current_tetromino->game_over_flag)
		{	
			// reset game
			reset();
			return;
		}

	}

	// increment update timer
	update_timer += deltatime;

	// update current tetromino
	if(update_timer >= update_interval)
	{
		current_tetromino->move(0,32, &leftover_blocks);
		update_timer -= update_interval;
		
		check_tetromino_collision();

	}

}

void hard_drop()
{
	current_tetromino->grace_period = 0;

	while(true)
	{
		current_tetromino->move(0,32, &leftover_blocks);

		if(current_tetromino->collision_flag)
		{
			break;
		}
	}

	score += 1;
}

void check_tetromino_collision()
{
	if(current_tetromino->collision_flag)
	{
		// add current tetromino to leftover blocks
		for (size_t k = 0; k < current_tetromino->rects.size(); k++)
		{
			leftover_blocks.push_back(current_tetromino->rects[k]);

		}

		// update score
		update_score(1);

		// ready to spawn new tetromino
		current_tetromino = nullptr;

		int lines_cleared = 0;

		// check for lines
		for(int i = 0; i < GRID_HEIGHT*32; i += 32)
		{
			int blocks_in_line = 0;
			for(size_t j = 0; j < leftover_blocks.size(); j++)
			{
				if(leftover_blocks[j]->getPosition().y == i)
				{
					blocks_in_line++;
				}
			}

			if(blocks_in_line == GRID_WIDTH)
			{
				// remove all blocks in line
				for(size_t j = 0; j < leftover_blocks.size(); j++)
				{
					if(leftover_blocks[j]->getPosition().y == i)
					{
						delete leftover_blocks[j];
						leftover_blocks.erase(leftover_blocks.begin() + j);
						j--;
					}
				}

				// move all blocks above line down
				for(size_t j = 0; j < leftover_blocks.size(); j++)
				{
					if(leftover_blocks[j]->getPosition().y < i)
					{
						leftover_blocks[j]->move(0,32);
					}
				}

				// count lines
				lines_cleared++;

				i -= 32;

			}
		}

		// update score
		if(lines_cleared > 0)
		{
			switch (lines_cleared)
			{
			case 1:
				update_score(100*level);
				lines++;
				break;

			case 2:
				update_score(300*level);
				lines+=3;
				break;

			case 3:
				update_score(500*level);
				lines+=5;
				break;

			case 4:
				update_score(800*level);
				lines+=8;
				break;
			
			default:
				break;
			}

			if(lines >= level*5)
			{
				level++;
				lines -= (level-1)*5;

				// calculate game speed increase
				update_interval = get_drop_speed(level);
				level_text.setString(LEVEL_STR + std::to_string(level));
			}

			// play clear sound
			clear.play();

		}
		else
		{
			// play thump sound
			thump.play();
		}

	}
}

void hold()
{
	if(heldblock) return;
	if(current_tetromino == nullptr) return;

	if(holdblock == Tetromino::None)
	{
		holdblock = current_tetromino->getType();
		holdblock_sprite.setTexture(holdblock_textures[holdblock]);
		current_tetromino = nullptr;
	}
	else
	{
		Tetromino* temp = new Tetromino(holdblock, tile, &leftover_blocks);
		holdblock = current_tetromino->getType();
		holdblock_sprite.setTexture(holdblock_textures[holdblock]);

		delete current_tetromino;
		current_tetromino = temp;
	}
	
	heldblock = true;
	if(current_tetromino != nullptr) update_ghost(current_tetromino, ghost_tetromino, &leftover_blocks);
}

void pause()
{
	paused = !paused;
	if(paused) { menu_text.setString(MENU_STR + PAUSE_STR); song.pause(); }
	else { menu_text.setString(MENU_STR + TETRIS_STR); song.play(); }
}

void reset()
{
	std::string message = "Game Over!";

	// check for highscore
	if(score >= highscore)
	{
		highscore = score;
		high_text.setString(HIGH_STR + std::to_string(highscore));
		message = "Highscore!";
		set_highscore(highscore);
	}

	song.pause();

	// alert player of game over via dialog
	Dialog game_over_dialog(message, SCORE_STR + std::to_string(score), LEVEL_STR + std::to_string(level), &window, font);
	game_over_dialog.display();

	song.play();
	thump.play();

	// reset current tetromino
	current_tetromino = nullptr;

	// reset leftover blocks
	for(size_t i = 0; i < leftover_blocks.size(); i++)
	{
		delete leftover_blocks[i];
	}

	leftover_blocks.clear();

	// reset holdblock sprite texture
	holdblock_sprite.setTexture(tile);
	holdblock = Tetromino::None;

	// reset randbag
	randbag.reset();
	randbag.randomize();
	update_nextblock();

	// reset score and other timing-based variables
	score = 0;
	update_score(0);

	deltatime = 0;
	update_timer = 0;
	update_interval = get_drop_speed(1);
	deltaclock.restart();

	lines = 0;
	level = 1;
	level_text.setString(LEVEL_STR + std::to_string(level));

}