#include <SFML/Graphics.hpp>
#include <time.h>
#include <iostream>

using namespace sf;

int sizeCharacter = 54; // Размер текстуры персонажа

int board[8][8] =
{ 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0 };

Texture textureHunter; // Это странно. Вроде атрибут у класса Hunter. Но я не могу его обьявлять внутри него - он должен быть всевиден.
class Hunter {
public:
	int xPos;
	int yPos;
	int id = 1;
	Sprite spriteHunter;

	Hunter() { }

	Hunter(int x, int y) {
		this->xPos = x;
		this->yPos = y;
		textureHunter.loadFromFile("res/hunter2.png");
		spriteHunter.setTextureRect(IntRect(0, 0, sizeCharacter, sizeCharacter)); // Резка текстуры на спрайты
		spriteHunter.setTexture(textureHunter);
	}
};


Hunter hunter1;

void loadPosition() {
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++) {
			int n = board[i][j]; // Некоторая позиция на доске
			if (!n) continue;
			if (n == 1) {
				hunter1.spriteHunter.setPosition(sizeCharacter*j, sizeCharacter*i);//
			}
		}
}

int main()
{
	RenderWindow window(VideoMode(432, 432), "The Hunter");

	Texture tCharacter, tBoard;
	tBoard.loadFromFile("res/board432.png");

	Sprite spriteBoard(tBoard);

	hunter1 = Hunter(4, 3); // Появляясь на координате 43 игрок может ходить только на 42 33 53 44
	board[hunter1.yPos - 1][hunter1.xPos - 1] = hunter1.id; // Hunter(4, 3) = board[2][3]

	loadPosition();

	bool isMove = false;
	float dx = 0, dy = 0;
	int n = 0;

	while (window.isOpen()) {
		Vector2i mousePos = Mouse::getPosition(window);

		Event e;
		while (window.pollEvent(e)) {
			if (e.type == Event::Closed)
				window.close();
		}

		////// draw  ///////
		window.clear();
		window.draw(spriteBoard);
		window.draw(hunter1.spriteHunter);
		window.display();
	}

	return 0;
}
