#include "includes.hpp"

class RandBag {
public:
	RandBag();
	~RandBag();

	void randomize();
	void reset();

	Tetromino::Type get_next();
	Tetromino::Type get_current();

private:
	std::array<Tetromino::Type, 7> bag;
	int loc;
};