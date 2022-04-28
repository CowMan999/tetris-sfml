#define LETERBOX_H
#include "includes.hpp"

Grid I_shape = {
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	1, 1, 1, 1, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0
};


Grid L_shape = {
	0, 0, 0, 0, 0,
	0, 1, 0, 0, 0,
	0, 1, 1, 1, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0
};


Grid J_shape = {
	0, 0, 0, 0, 0,
	0, 0, 0, 1, 0,
	0, 1, 1, 1, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0
};


Grid O_shape = {
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 1, 1, 0,
	0, 0, 1, 1, 0,
	0, 0, 0, 0, 0
};


Grid Z_shape = {
	0, 0, 0, 0, 0,
	0, 0, 1, 1, 0,
	0, 1, 1, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0
};


Grid T_shape = {
	0, 0, 0, 0, 0,
	0, 0, 1, 0, 0,
	0, 1, 1, 1, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0
};


Grid S_shape = {
	0, 0, 0, 0, 0,
	0, 1, 1, 0, 0,
	0, 0, 1, 1, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0
};


Tetromino::Tetromino(Type t, sf::Texture& te, std::vector<sf::Sprite*>* blocks)
{
	type = t;
	tile = &te;
	switch(type)
	{
		case I:
			shape = I_shape;
			color = sf::Color::Cyan;
			break;
		case J:
			shape = J_shape;
			color = sf::Color::Blue;
			break;
		case L:
			shape = L_shape;
			color = sf::Color(0xfa, 0x8b, 0x01, 0xff);
			break;
		case O:
			shape = O_shape;
			color = sf::Color(0xfb, 0xd8, 0x00, 0xff);
			break;
		case S:
			shape = S_shape;
			color = sf::Color::Green;
			break;
		case T:
			shape = T_shape;
			color = sf::Color::Magenta;
			break;
		case Z:
			shape = Z_shape;
			color = sf::Color::Red;
			break;
		default:
			break;
	}

	// find locations of 1's and add rects
	char iter = 0;
	for(int i = 0; i < 5; i++)
	{
		for(int j = 0; j < 5; j++)
		{
			if(shape[i][j] == 1)
			{
				rects[iter] = new sf::Sprite(te);
				rects[iter]->setColor(color);
				rectOffsets[iter] = sf::Vector2i(i*32,j*32);
				iter++;
			}
		}
	}

	// set position and rotation
	if(this->move(64,-32, blocks))
	{
		// collision on start, game over
		game_over_flag = true;
	}

	this->rotate(0);

}

Tetromino::Type Tetromino::getType()
{
	return type;
}

void Tetromino::rotate(bool clockwise)
{
	if(clockwise) angle++;
	else angle--;

	// don't waste time rotating an O
	if(type == O)
		return;

	Grid tmp;

	// rotate shape
	if(clockwise)
	{
		for(int i = 0; i < 5; i++)
		{
			for(int j = 0; j < 5; j++)
			{
				tmp[i][j] = shape[4-j][i];
			}
		}
	}
	else
	{
		for(int i = 0; i < 5; i++)
		{
			for(int j = 0; j < 5; j++)
			{
				tmp[i][j] = shape[j][4-i];
			}
		}
	}

	shape = tmp;

	// update rects
	int iter = 0;
	for(int i = 0; i < 5; i++)
	{
		for(int j = 0; j < 5; j++)
		{
			if(shape[i][j] == 1)
			{
				rectOffsets[iter] = sf::Vector2i(i*32,j*32);
				iter++;
			}
		}
	}

	// update position
	this->move(0,0);

}

void Tetromino::update_color()
{
	for(size_t i = 0; i < rects.size(); i++)
	{
		rects[i]->setColor(color);
	}
}

// rotate with collision detection
bool Tetromino::rotate(bool clockwise, std::vector<sf::Sprite*>* blocks)
{
	if(clockwise) angle++;
	else angle--;

	// don't waste time rotating an O
	if(type == O)
		return false;

	Grid tmp;

	bool colliding = false;

	// rotate shape
	if(clockwise)
	{
		for(int i = 0; i < 5; i++)
		{
			for(int j = 0; j < 5; j++)
			{
				tmp[i][j] = shape[4-j][i];
			}
		}
	}
	else
	{
		for(int i = 0; i < 5; i++)
		{
			for(int j = 0; j < 5; j++)
			{
				tmp[i][j] = shape[j][4-i];
			}
		}
	}

	// check collision
	for(int x = 0; x < 5; x++)
	{
		for(int y = 0; y < 5; y++)
		{
			if(tmp[x][y] == 1)
			{
				sf::Vector2f rect_pos = position + sf::Vector2f(x*32,y*32);
				for(size_t i = 0; i < blocks->size(); i++)
				{
					// if colliding with any block
					if(blocks->at(i)->getPosition() == rect_pos)
					{
						colliding = true;
						break;
					}
				}

				// if colliding with wall
				if(rect_pos.y >= GRID_HEIGHT * 32)
				{
					colliding = true;
					break;
				}

				if(rect_pos.x < 0)
				{
					colliding = true;
					break;
				}

				if(rect_pos.x+32 > GRID_WIDTH * 32)
				{
					colliding = true;
					break;
				}
			}
			
			if(colliding)
				break;
		}

		if(colliding)
			break;
	}
	
	if(!colliding)
	{
		shape = tmp;

		// update rects
		int iter = 0;
		for(int i = 0; i < 5; i++)
		{
			for(int j = 0; j < 5; j++)
			{
				if(shape[i][j] == 1)
				{
					rectOffsets[iter] = sf::Vector2i(i*32,j*32);
					iter++;
				}
			}
		}

		// update position
		this->move(0,0);
	}


	return colliding;

}

void Tetromino::move(int x, int y)
{
	position.x += x;
	position.y += y;

	// update rects
	for(int i = 0; i < 4; i++)
	{
		rects[i]->setPosition(position.x + rectOffsets[i].x, position.y + rectOffsets[i].y);
	}

}

// move with collision checks
bool Tetromino::move(int x, int y, std::vector<sf::Sprite*>* blocks)
{
	sf::Vector2f tmp_position = position + sf::Vector2f(x,y);

	bool collided = false;
	bool vertical_collision = false;

	// check for collision with all blocks and edges
	for(size_t i = 0; i < rects.size(); i++)
	{
		sf::Vector2f rect_pos = tmp_position + sf::Vector2f(rectOffsets[i]);

		if(rect_pos.y >= GRID_HEIGHT * 32)
		{
			vertical_collision = true;
			collided = true;
			break;
		}

		if(rect_pos.x < 0)
		{
			collided = true;
			break;
		}

		if(rect_pos.x+32 > GRID_WIDTH * 32)
		{
			collided = true;
			break;
		}

		for(size_t j = 0; j < blocks->size(); j++)
		{
			if(rect_pos == blocks->at(j)->getPosition())
			{
				if(y > 0) vertical_collision = true;
				collided = 1;
				break;
			}

		}

		if(collided)
			break;

	}

	if(!collided)
	{
		position = tmp_position;
	}
	if(vertical_collision)
	{
		if(grace_period == 0)
			collision_flag = true;
		else grace_period--;

	}


	// update rects
	for(int i = 0; i < 4; i++)
	{
		rects[i]->setPosition(position.x + rectOffsets[i].x, position.y + rectOffsets[i].y);
	}

	return collided;

}

void Tetromino::update_rects()
{
	// update rects
	for(int i = 0; i < 4; i++)
	{
		rects[i]->setPosition(position.x + rectOffsets[i].x, position.y + rectOffsets[i].y);
	}
}

void Tetromino::draw(sf::RenderWindow& window)
{
	for(int i = 0; i < 4; i++)
	{
		window.draw(*rects[i]);
	}
}

Tetromino::~Tetromino()
{
	for(int i = 0; i < 4; i++)
	{
		delete rects[i];
	}
}

Tetromino* Tetromino::clone(std::vector<sf::Sprite*>* blocks)
{
	Tetromino* t = new Tetromino(type, *tile, blocks);

	t->shape = shape;
	t->position = position;

	// update rects to shape
	int iter = 0;
	for(int i = 0; i < 5; i++)
	{
		for(int j = 0; j < 5; j++)
		{
			if(shape[i][j] == 1)
			{
				t->rectOffsets[iter] = sf::Vector2i(i*32,j*32);
				iter++;
			}
		}
	}

	// update rects to position
	t->update_rects();


	return t;
}
