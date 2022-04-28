#define LETERBOX_H
#include "includes.hpp"

RandBag::RandBag()
{
	loc = 0;
}

RandBag::~RandBag() {}

void RandBag::reset()
{
	loc = 0;
	for (size_t i = 0; i < 7; i++)
	{
		bag[i] = static_cast<Tetromino::Type>(i);
	}
	
}

void RandBag::randomize()
{
	loc = 0;
	Tetromino::Type tmp;
	for (size_t i = 0; i < 7; i++)
	{
		tmp = bag[i];
		int r = rand() % 7;
		bag[i] = bag[r];
		bag[r] = tmp;
	}
}

Tetromino::Type RandBag::get_next()
{
	Tetromino::Type ret = bag[loc];
	loc++;
	if (loc >= 7)
	{
		randomize();
	}
	return ret;
}

Tetromino::Type RandBag::get_current()
{
	return bag[loc];
}