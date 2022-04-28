class Tetromino {
public:
	enum Type {
		None = -1,
		I,
		J,
		L,
		O,
		S,
		T,
		Z
	};

	Tetromino(Type t, sf::Texture& te, std::vector<sf::Sprite*>* blocks);
	Tetromino* clone(std::vector<sf::Sprite*>* blocks);
	~Tetromino();

	void rotate(bool clockwise);
	bool rotate(bool clockwise, std::vector<sf::Sprite*>* blocks);
	void move(int x, int y);
	bool move(int x, int y, std::vector<sf::Sprite*>* blocks);
	void draw(sf::RenderWindow& window);
	void update_color();
	void update_rects();
	Type getType();

	// rects
	std::array<sf::Sprite*, 4> rects;

	unsigned char grace_period = 1;
	int angle = 0;

	bool collision_flag = false;
	bool game_over_flag = false;
	sf::Color color;
	sf::Texture* tile;
	sf::Vector2f position;

private:
	Type type;
	Grid shape;

	
	// offset of each rect from origin
	std::array<sf::Vector2i, 4> rectOffsets;


};