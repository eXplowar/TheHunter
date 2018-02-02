#include <SFML/Graphics.hpp>
#include <time.h>
#include <iostream>
#include <vector>

using namespace sf;

const int sizeCharacter = 54; // ������ �������� ���������
const int restartMenuHeight = 54; // ������ ������� �������� ����
const int restartMenuWidth = 162; // ������ ������� �������� ����
bool animalSeeTrap = false; // �������� ����� �������
bool humanTurn = false; // True - ��� ������. False - ��� ����������
bool gameOver = false; // ������ ����� ����

Texture textureHunter;
Texture textureAnimal;
Texture textureTrap;
Texture textureRestart;

const int worldMapHeight = 4; // ������ �����
const int worldMapWidth = 4; // ������ �����
int worldMap[worldMapWidth][worldMapHeight]; // ������ �����

class Hunter {
public:
	int xPos;
	int yPos;
	int id = 1;
	bool isMoveByPlayer = false;
	bool loaded = false;
	Sprite sprite;

	Hunter() { }
	
	Hunter(int x, int y) {
		this->xPos = x;
		this->yPos = y;
		textureHunter.loadFromFile("res/hunter.png");
		sprite.setTextureRect(IntRect(0, 0, sizeCharacter, sizeCharacter)); // ����� �������� �� �������
		sprite.setTexture(textureHunter);
	}
};

class Animal {
public:
	int xPos;
	int yPos;
	int id = 2;
	bool isMoveByPlayer = false;
	Sprite sprite;

	Animal() {}

	Animal(int x, int y) {
		this->xPos = x;
		this->yPos = y;
		textureAnimal.loadFromFile("res/animal.png");
		sprite.setTextureRect(IntRect(0, 0, sizeCharacter, sizeCharacter));
		sprite.setTexture(textureAnimal);
	}
};

class Trap {
public:
	int xPos;
	int yPos;
	int id = 3;
	bool isMoveByPlayer = false;
	Sprite sprite;

	Trap() {}

	Trap(int x, int y) {
		this->xPos = x;
		this->yPos = y;
		textureTrap.loadFromFile("res/trap.png");
		sprite.setTextureRect(IntRect(0, 0, sizeCharacter, sizeCharacter));
		sprite.setTexture(textureTrap);
	}
};

Sprite spriteRestart;

void ShowRestartMenu() {
	textureRestart.loadFromFile("res/restart.png");
	spriteRestart.setTextureRect(IntRect(0, 0, restartMenuWidth, restartMenuHeight));
	spriteRestart.setTexture(textureRestart);
	spriteRestart.setPosition(worldMapWidth * sizeCharacter / 2 - restartMenuWidth / 2, worldMapHeight * sizeCharacter / 2 - restartMenuHeight / 2);
}

Hunter hunter1;
Hunter hunter2;
Animal animal1;
Trap trap1;

void PrintWorldMap(std::string msg) {
	std::cout << msg << std::endl;
	for (int i = 0; i < worldMapHeight; i++) {
		for (int j = 0; j < worldMapWidth; j++) {
			std::cout << worldMap[i][j];
		}
		std::cout << std::endl;
	}
	std::cout << "________" << std::endl;
	std::cout << std::endl;
}

// ������������� �����
void initWorldMap() {
	// ���������� ���������� ������� ������
	for (int i = 0; i < worldMapHeight; i++) {
		for (int j = 0; j < worldMapWidth; j++) {
			worldMap[i][j] = 0;
		}
	}

	// �� ��������� �������� ����� ��������������� ����� ���� ��� ��������� ���������� �������, 
	// �������� �����(������������������, ����(����������X, ����������Y)), 
	// ����� ���� ����������� ������� ���� � ������������ � �����
	std::map<std::pair<int, int>, std::string> mGameObjectPair;
	std::pair<int, int> tempCoordinate;

	tempCoordinate = std::make_pair(std::rand() % worldMapHeight, std::rand() % worldMapWidth);
	mGameObjectPair[tempCoordinate] = "hunter1";

	do
	{
		tempCoordinate = std::make_pair(std::rand() % worldMapHeight, std::rand() % worldMapWidth);
	} while (!mGameObjectPair[tempCoordinate].empty());
	mGameObjectPair[tempCoordinate] = "hunter2";

	do
	{
		tempCoordinate = std::make_pair(std::rand() % worldMapHeight, std::rand() % worldMapWidth);
	} while (!mGameObjectPair[tempCoordinate].empty());
	mGameObjectPair[tempCoordinate] = "animal1";

	do
	{
		tempCoordinate = std::make_pair(std::rand() % worldMapHeight, std::rand() % worldMapWidth);
	} while (!mGameObjectPair[tempCoordinate].empty());
	mGameObjectPair[tempCoordinate] = "trap1";

	// ������������� �������� ����� � �������� ���������
	for (std::map<std::pair<int, int>, std::string>::iterator it = mGameObjectPair.begin(); it != mGameObjectPair.end(); ++it) {
		/*std::cout << it->second << ": ";
		std::cout << it->first.first << " ";
		std::cout << it->first.second << "\n";*/
		if (it->second == "hunter1") {
			hunter1 = Hunter(it->first.first, it->first.second);
			worldMap[hunter1.yPos][hunter1.xPos] = hunter1.id;
		}
		else if (it->second == "hunter2") {
			hunter2 = Hunter(it->first.first, it->first.second);
			worldMap[hunter2.yPos][hunter2.xPos] = hunter2.id;
		}
		else if (it->second == "animal1") {
			animal1 = Animal(it->first.first, it->first.second);
			worldMap[animal1.yPos][animal1.xPos] = animal1.id;
		}
		else if (it->second == "trap1") {
			trap1 = Trap(it->first.first, it->first.second);
			worldMap[trap1.yPos][trap1.xPos] = trap1.id;
		}

	}

	std::cout << hunter1.id << " - Hunter; " << animal1.id << " - Animal; " << trap1.id << " - Trap" << std::endl << std::endl;

	// ����� ���������� �������
	PrintWorldMap("Map created:");
}

void loadPosition() {
	for (int i = 0; i < worldMapHeight; i++) {
		for (int j = 0; j < worldMapWidth; j++) {
			int n = worldMap[i][j]; // ��������� ������� �� �����
			if (!n) continue;
			if (n == 1) {
				if (i == hunter1.yPos && j == hunter1.xPos) {
					hunter1.sprite.setPosition(sizeCharacter*j, sizeCharacter*i);//
					hunter1.loaded = true;
					continue;
				}
				if (i == hunter2.yPos && j == hunter2.xPos) {
					hunter2.sprite.setPosition(sizeCharacter*j, sizeCharacter*i);//
					hunter2.loaded = true;
					continue;
				}
			}
			if (n == 2) {
				animal1.sprite.setPosition(sizeCharacter*j, sizeCharacter*i);//
			}
			if (n == 3) {
				trap1.sprite.setPosition(sizeCharacter*j, sizeCharacter*i);//
			}
		}
	}
}

float dx = 0, dy = 0;
Vector2f curPos;

// ��������� ����� ������� �������
template<typename T>
void SelectGameUnit(T &gameUnit, Vector2i mousePos) {
	if (gameUnit.sprite.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
		gameUnit.isMoveByPlayer = true;
		curPos = gameUnit.sprite.getPosition();
		dx = mousePos.x - gameUnit.sprite.getPosition().x;
		dy = mousePos.y - gameUnit.sprite.getPosition().y;
	}
}

// ��������� ������� �������� ������� �������
template<typename T>
void MoveGameUnitByPlayer(T &gameUnit, Vector2i mousePos) {
	if (gameUnit.sprite.getGlobalBounds().contains(mousePos.x, mousePos.y) && (gameUnit.isMoveByPlayer)) {
		gameUnit.isMoveByPlayer = false;

		// ������ ����� �������
		Vector2f p = gameUnit.sprite.getPosition() + Vector2f(sizeCharacter / 2, sizeCharacter / 2);
		int newArrayPosX = int(p.x / sizeCharacter);
		int newArrayPosY = int(p.y / sizeCharacter);

		// ������ ����� ������ �� ������ �������
		if (worldMap[newArrayPosY][newArrayPosX] != 0) {
			std::cout << "You can not move to this cage, there is already someone on it!" << std::endl;
			gameUnit.sprite.setPosition(curPos);
			return;
		}

		// �������� ������ �� ����� ������
		if (((gameUnit.xPos != newArrayPosX) && (gameUnit.yPos != newArrayPosY)) || ((abs(gameUnit.xPos - newArrayPosX) > 1) || (abs(gameUnit.yPos - newArrayPosY) > 1))) {
			gameUnit.sprite.setPosition(curPos);
			return;
		}

		// ����������� �������. ������������
		Vector2f newPos = Vector2f(sizeCharacter*newArrayPosX, sizeCharacter*newArrayPosY);
		gameUnit.sprite.setPosition(newPos);

		// ����������� ����� � �������
		worldMap[gameUnit.yPos][gameUnit.xPos] = 0; // �������� ������� �����
		gameUnit.xPos = newArrayPosX;
		gameUnit.yPos = newArrayPosY;
		worldMap[gameUnit.yPos][gameUnit.xPos] = gameUnit.id; // ������ ��������: Hunter(4, 3) = map[2][3]. ����� �������� ��� ���� ������ Hunter(5, 3) = map[2][4]

		humanTurn = false; // ����� ���� ������

		// ����� ���������� ������� ����� ���� ������
		PrintWorldMap("The Player made a move:");
	}
}
bool startGame() {
	gameOver = false;

	srand(static_cast <unsigned> (time(0)));

	RenderWindow window(VideoMode(worldMapWidth * sizeCharacter, worldMapHeight * sizeCharacter), "The Hunter", sf::Style::Titlebar | sf::Style::Close);

	Texture tCharacter, tWorldMap;
	tWorldMap.loadFromFile("res/map432.png");

	Sprite spriteMap(tWorldMap);

	initWorldMap();
	loadPosition();

	humanTurn = true;

	while (window.isOpen()) {
		Vector2i mousePos = Mouse::getPosition(window);

		Event e;
		while (window.pollEvent(e)) {
			if (e.type == Event::Closed)
				window.close();

			if (humanTurn) {
				// ��������������
				if (e.type == Event::MouseButtonPressed) {
					if (e.key.code == Mouse::Left) {
						// ����� ������� �������
						SelectGameUnit(hunter1, mousePos);
						SelectGameUnit(hunter2, mousePos);
						/*SelectGameUnit(animal1, mousePos);
						SelectGameUnit(trap1, mousePos);*/
					}
				}

				if (e.type == Event::MouseButtonReleased) {
					if (e.key.code == Mouse::Left) {
						MoveGameUnitByPlayer(hunter1, mousePos);
						MoveGameUnitByPlayer(hunter2, mousePos);
						/*MoveGameUnitByPlayer(animal1, mousePos);
						MoveGameUnitByPlayer(trap1, mousePos);*/
					}
				}
			}

			if (!humanTurn && !gameOver) {
				// 1. ���������� �������� �����
				int xLeft = animal1.xPos - 1;
				int xRight = animal1.xPos + 1;
				int yTop = animal1.yPos - 1;
				int yDown = animal1.yPos + 1;

				// 2. �������� ������ ��������� ����������
				std::vector<std::pair<int, int>> vAvailableCells; // ������ �������� ������
				std::vector<std::pair<int, int>> vCellsWithTrap; // ������ ������ � ��������, ������� ��������� � ���������������� �������� � ���������

				// �������� � ������� �� ���������
				std::cout << "Perimeter values:" << std::endl;
				std::cout << "map[animal1.yPos][animal1.xPos - 1]: " << worldMap[animal1.yPos][xLeft] << std::endl;
				if (worldMap[animal1.yPos][xLeft] == 3) vCellsWithTrap.push_back(std::pair<int, int>(xLeft, animal1.yPos));
				std::cout << "map[animal1.yPos][animal1.xPos + 1]: " << worldMap[animal1.yPos][xRight] << std::endl;
				if (worldMap[animal1.yPos][xRight] == 3) vCellsWithTrap.push_back(std::pair<int, int>(xRight, animal1.yPos));

				std::cout << "map[animal1.yPos - 1][animal1.xPos]: " << worldMap[yTop][animal1.xPos] << std::endl;
				if (worldMap[yTop][animal1.xPos] == 3) vCellsWithTrap.push_back(std::pair<int, int>(animal1.xPos, yTop));
				std::cout << "map[animal1.yPos + 1][animal1.xPos]: " << worldMap[yDown][animal1.xPos] << std::endl;
				if (worldMap[yDown][animal1.xPos] == 3) vCellsWithTrap.push_back(std::pair<int, int>(animal1.xPos, yDown));

				// ���������� ������ ����� �� ��������� ������ ���� �������������
				if (xLeft >= 0) {
					// ��������� ���������� � ������ ��������� ��� ���� � ������ ����: (�������� ����� �������, ������ ����� �� ������� � �� �������) ���� (�������� �� ����� �������, ������ ����� �� �������)
					if (((animalSeeTrap) && ((worldMap[animal1.yPos][xLeft] != 1) && (worldMap[animal1.yPos][xLeft] != 3))) ||
						((!animalSeeTrap) && (worldMap[animal1.yPos][xLeft] != 1))) {
						vAvailableCells.push_back(std::pair<int, int>(xLeft, animal1.yPos));
					}
				}
				if (xRight < worldMapWidth) {
					if (((animalSeeTrap) && ((worldMap[animal1.yPos][xRight] != 1) && (worldMap[animal1.yPos][xRight] != 3))) ||
						((!animalSeeTrap) && (worldMap[animal1.yPos][xRight] != 1))) {
						vAvailableCells.push_back(std::pair<int, int>(xRight, animal1.yPos));
					}
				}
				if (yTop >= 0) {
					if (((animalSeeTrap) && ((worldMap[yTop][animal1.xPos] != 1) && (worldMap[yTop][animal1.xPos] != 3))) ||
						((!animalSeeTrap) && (worldMap[yTop][animal1.xPos] != 1))) {
						vAvailableCells.push_back(std::pair<int, int>(animal1.xPos, yTop));
					}
				}
				if (yDown < worldMapHeight) {
					if (((animalSeeTrap) && ((worldMap[yDown][animal1.xPos] != 1) && (worldMap[yDown][animal1.xPos] != 3))) ||
						((!animalSeeTrap) && (worldMap[yDown][animal1.xPos] != 1))) {
						vAvailableCells.push_back(std::pair<int, int>(animal1.xPos, yDown));
					}
				}

				// ������� ��������� �����
				if (vAvailableCells.size() > 0) {
					// ����� �������
					std::cout << std::endl << "The Animal can go here:" << std::endl;
					for (int i = 0; i < vAvailableCells.size(); i++) {
						std::cout << vAvailableCells[i].first;
						std::cout << vAvailableCells[i].second;
						std::cout << std::endl;
					}
				}
				else {
					if (vCellsWithTrap.size() != 0) {
						vAvailableCells.push_back(vCellsWithTrap[0]);
					}
					std::cout << std::endl << "Rabbit has nowhere to go, he has to go to the trap:" << std::endl;
					for (int i = 0; i < vAvailableCells.size(); i++) {
						std::cout << vAvailableCells[i].first;
						std::cout << vAvailableCells[i].second;
						std::cout << std::endl;
					}
				}
				std::cout << std::endl;

				// 3. ��������� ������� ��������� ����� ������� ��������� ����� ������ �� ���������� ��������� ��� ���� ���������
				int randomInt = std::rand() % vAvailableCells.size();
				std::pair<int, int> botMoveCoordinate = vAvailableCells[randomInt];

				// 4. ������������� ��� �� �����
				Vector2f newPos = Vector2f(sizeCharacter*botMoveCoordinate.first, sizeCharacter*botMoveCoordinate.second);
				animal1.sprite.setPosition(newPos);

				// 5. ������������� ��� � ��������� �������
				worldMap[animal1.yPos][animal1.xPos] = 0;
				animal1.xPos = botMoveCoordinate.first;
				animal1.yPos = botMoveCoordinate.second;
				if (worldMap[botMoveCoordinate.second][botMoveCoordinate.first] == 3) {
					gameOver = true;
					std::cout << "You caught the rabbit!" << std::endl << std::endl;
					break;
				}
				worldMap[botMoveCoordinate.second][botMoveCoordinate.first] = animal1.id;

				// ����� ���������� ������� ����� ���� ����������
				PrintWorldMap("The Computer made a move:");

				humanTurn = true; // ����� ���� ����
			}

			// �������
			if (Keyboard::isKeyPressed(Keyboard::R)) {
				std::cout << "Restart pressed" << std::endl;
				std::cout << "________" << std::endl << std::endl;
				return true;
			}
		}

		// �������� ������� ������� � ������� ������� � ����������� ����� �������
		if (hunter1.isMoveByPlayer) {
			hunter1.sprite.setPosition(mousePos.x - dx, mousePos.y - dy);
		}
		if (hunter2.isMoveByPlayer) {
			hunter2.sprite.setPosition(mousePos.x - dx, mousePos.y - dy);
		}
		if (animal1.isMoveByPlayer) {
			animal1.sprite.setPosition(mousePos.x - dx, mousePos.y - dy);
		}
		if (trap1.isMoveByPlayer) {
			trap1.sprite.setPosition(mousePos.x - dx, mousePos.y - dy);
		}

		// ���������
		window.clear();
		window.draw(spriteMap);
		window.draw(hunter1.sprite);
		window.draw(hunter2.sprite);
		window.draw(animal1.sprite);
		window.draw(trap1.sprite);
		if (gameOver) {
			ShowRestartMenu();
			window.draw(spriteRestart);
		}
		window.display();
	}
}

void gameRunning() {
	// ������� startGame() ��������� ���� ������ � ��������� true ������ ��� ������� �� R, ���, � ���� ������� ����� �������� ������� gameRunning(), ������� ������ �������� startGame()
	if (startGame()) {
		gameRunning();
	}
}

int main(int argc, char *argv[]) {
	if (argc == 1) {
		std::cout << "Rabbit does not see the trap" << std::endl;
		animalSeeTrap = false;
	} else {
		for (int i = 1; i < argc; i++) {
			std::string param = argv[i];
			if (param == "AnimalSeeTrap") {
				animalSeeTrap = true;
				std::cout << "Rabbit sees traps" << std::endl;
			}
		}
	}

	gameRunning();

	return 0;
}
