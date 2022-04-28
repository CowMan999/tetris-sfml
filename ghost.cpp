#define LETERBOX_H
#include "includes.hpp"

void update_ghost(Tetromino* current_tetromino, Tetromino*& ghost_tetromino, std::vector<sf::Sprite*>* blocks)
{
	if(current_tetromino == nullptr)
	{
		return;
	}

	if(ghost_tetromino != nullptr)
		delete ghost_tetromino;

	ghost_tetromino = current_tetromino->clone(blocks);
	ghost_tetromino->color = sf::Color(0,0,0,72);
	ghost_tetromino->update_color();
	while(!ghost_tetromino->move(0, 32, blocks));
	
}