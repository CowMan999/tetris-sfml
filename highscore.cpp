#define LETERBOX_H
#include "includes.hpp"

size_t get_highscore()
{
	std::ifstream file("highscore");

	if(!file.is_open())
	{
		std::ofstream filetmp("highscore");
		filetmp << "0";
		filetmp.close();
	}

	size_t ret = 0;
	file >> ret;
	file.close();
	return ret;
}

void set_highscore(size_t score)
{
	std::ofstream file("highscore");
	file << score;
	file.close();
}