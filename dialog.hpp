#include "includes.hpp"

class Dialog
{
public:
	Dialog(std::string message, std::string subtitle, std::string minititle, sf::RenderWindow* window, sf::Font& font, unsigned short char_size = 48, unsigned short char_size2 = 20, unsigned short char_size3 = 18, bool allow_close = true);
	~Dialog();

	void display();
	void kill();

private:
	sf::Text text;
	sf::Text text2;
	sf::Text text3;
	sf::RenderWindow* window;
	volatile bool running = true;
	bool allow_close;

};