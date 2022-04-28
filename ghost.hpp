#define LETERBOX_H
#include "includes.hpp"

void update_ghost(Tetromino* current_tetromino, Tetromino*& ghost_tetromino, std::vector<sf::Sprite*>* blocks);