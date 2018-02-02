#include <SFML/Graphics.hpp>
#include <time.h>
#include <iostream>
#include <vector>

using namespace sf;

const int sizeCharacter = 54; // Размер текстуры персонажа
const int restartMenuHeight = 54; // Высота спрайта рестарта игры
const int restartMenuWidth = 162; // Ширина спрайта рестарта игры
bool animalSeeTrap = false; // Животное видит ловушки
bool humanTurn = false; // True - ход игрока. False - ход компьютера
bool gameOver = false; // Маркер конца игры

Texture textureHunter;
Texture textureAnimal;
Texture textureTrap;
Texture textureRestart;

const int worldMapHeight = 4; // Высота карты
const int worldMapWidth = 4; // Ширина карты
int worldMap[worldMapWidth][worldMapHeight]; // Размер карты

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
		sprite.setTextureRect(IntRect(0, 0, sizeCharacter, sizeCharacter)); // Резка текстуры на спрайты
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

// Инициализация карты
void initWorldMap() {
	// Заполнение двумерного массива нулями
	for (int i = 0; i < worldMapHeight; i++) {
		for (int j = 0; j < worldMapWidth; j++) {
			worldMap[i][j] = 0;
		}
	}

	// Во избежание ситуаций когда сгенерированная новая пара уже присвоена некоторому объекту, 
	// создаётся Карта(ИмяИгровогоОбьекта, Пара(координатаX, координатаY)), 
	// после чего проверяется наличие пары с координатами в карте
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

	// Сопоставление значения карты с игровыми объектами
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

	// Вывод двумерного массива
	PrintWorldMap("Map created:");
}

void loadPosition() {
	for (int i = 0; i < worldMapHeight; i++) {
		for (int j = 0; j < worldMapWidth; j++) {
			int n = worldMap[i][j]; // Некоторая позиция на доске
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

// Получение точки захвата спрайта
template<typename T>
void SelectGameUnit(T &gameUnit, Vector2i mousePos) {
	if (gameUnit.sprite.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
		gameUnit.isMoveByPlayer = true;
		curPos = gameUnit.sprite.getPosition();
		dx = mousePos.x - gameUnit.sprite.getPosition().x;
		dy = mousePos.y - gameUnit.sprite.getPosition().y;
	}
}

// Изменение позиции игрового обьекта игроком
template<typename T>
void MoveGameUnitByPlayer(T &gameUnit, Vector2i mousePos) {
	if (gameUnit.sprite.getGlobalBounds().contains(mousePos.x, mousePos.y) && (gameUnit.isMoveByPlayer)) {
		gameUnit.isMoveByPlayer = false;

		// Расчёт новой позиции
		Vector2f p = gameUnit.sprite.getPosition() + Vector2f(sizeCharacter / 2, sizeCharacter / 2);
		int newArrayPosX = int(p.x / sizeCharacter);
		int newArrayPosY = int(p.y / sizeCharacter);

		// Ходить можно только по пустым клеткам
		if (worldMap[newArrayPosY][newArrayPosX] != 0) {
			std::cout << "You can not move to this cage, there is already someone on it!" << std::endl;
			gameUnit.sprite.setPosition(curPos);
			return;
		}

		// Движение только на одину клетку
		if (((gameUnit.xPos != newArrayPosX) && (gameUnit.yPos != newArrayPosY)) || ((abs(gameUnit.xPos - newArrayPosX) > 1) || (abs(gameUnit.yPos - newArrayPosY) > 1))) {
			gameUnit.sprite.setPosition(curPos);
			return;
		}

		// Перемещение спрайта. Выравнивание
		Vector2f newPos = Vector2f(sizeCharacter*newArrayPosX, sizeCharacter*newArrayPosY);
		gameUnit.sprite.setPosition(newPos);

		// Перемещение числа в массиве
		worldMap[gameUnit.yPos][gameUnit.xPos] = 0; // Удаление старого места
		gameUnit.xPos = newArrayPosX;
		gameUnit.yPos = newArrayPosY;
		worldMap[gameUnit.yPos][gameUnit.xPos] = gameUnit.id; // Старые значения: Hunter(4, 3) = map[2][3]. Новые значения при ходе вправо Hunter(5, 3) = map[2][4]

		humanTurn = false; // Конец хода игрока

		// Вывод двумерного массива после хода игрока
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
				// Перетаскивание
				if (e.type == Event::MouseButtonPressed) {
					if (e.key.code == Mouse::Left) {
						// Точка захвата спрайта
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
				// 1. Координаты соседних ячеек
				int xLeft = animal1.xPos - 1;
				int xRight = animal1.xPos + 1;
				int yTop = animal1.yPos - 1;
				int yDown = animal1.yPos + 1;

				// 2. Отобрать только доступные координаты
				std::vector<std::pair<int, int>> vAvailableCells; // Список достуных клеток
				std::vector<std::pair<int, int>> vCellsWithTrap; // Список клеток с ловушкой, которые находится в непосредственной близости к животному

				// Значения в ячейках по периметру
				std::cout << "Perimeter values:" << std::endl;
				std::cout << "map[animal1.yPos][animal1.xPos - 1]: " << worldMap[animal1.yPos][xLeft] << std::endl;
				if (worldMap[animal1.yPos][xLeft] == 3) vCellsWithTrap.push_back(std::pair<int, int>(xLeft, animal1.yPos));
				std::cout << "map[animal1.yPos][animal1.xPos + 1]: " << worldMap[animal1.yPos][xRight] << std::endl;
				if (worldMap[animal1.yPos][xRight] == 3) vCellsWithTrap.push_back(std::pair<int, int>(xRight, animal1.yPos));

				std::cout << "map[animal1.yPos - 1][animal1.xPos]: " << worldMap[yTop][animal1.xPos] << std::endl;
				if (worldMap[yTop][animal1.xPos] == 3) vCellsWithTrap.push_back(std::pair<int, int>(animal1.xPos, yTop));
				std::cout << "map[animal1.yPos + 1][animal1.xPos]: " << worldMap[yDown][animal1.xPos] << std::endl;
				if (worldMap[yDown][animal1.xPos] == 3) vCellsWithTrap.push_back(std::pair<int, int>(animal1.xPos, yDown));

				// Координата клетки слева от животного должна быть положительной
				if (xLeft >= 0) {
					// Поместить координату в список доступных для хода в случае если: (Животное видит ловушки, ячейка слева не охотник и не ловушка) либо (Животное не видит ловушки, ячейка слева не охотник)
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

				// Наличие возможных ходов
				if (vAvailableCells.size() > 0) {
					// Вывод вектора
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

				// 3. Используя функцию случайных чисел выбрать случайное число изходя из количества доступных для хода вариантов
				int randomInt = std::rand() % vAvailableCells.size();
				std::pair<int, int> botMoveCoordinate = vAvailableCells[randomInt];

				// 4. Зафиксировать ход на карте
				Vector2f newPos = Vector2f(sizeCharacter*botMoveCoordinate.first, sizeCharacter*botMoveCoordinate.second);
				animal1.sprite.setPosition(newPos);

				// 5. Зафиксировать ход в двумерном массиве
				worldMap[animal1.yPos][animal1.xPos] = 0;
				animal1.xPos = botMoveCoordinate.first;
				animal1.yPos = botMoveCoordinate.second;
				if (worldMap[botMoveCoordinate.second][botMoveCoordinate.first] == 3) {
					gameOver = true;
					std::cout << "You caught the rabbit!" << std::endl << std::endl;
					break;
				}
				worldMap[botMoveCoordinate.second][botMoveCoordinate.first] = animal1.id;

				// Вывод двумерного массива после хода компьютера
				PrintWorldMap("The Computer made a move:");

				humanTurn = true; // Конец хода бота
			}

			// Рестрат
			if (Keyboard::isKeyPressed(Keyboard::R)) {
				std::cout << "Restart pressed" << std::endl;
				std::cout << "________" << std::endl << std::endl;
				return true;
			}
		}

		// Привязка позиции спрайта к позиции курсора с сохранением точки захвата
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

		// Отрисовка
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
	// Функция startGame() завершает свою работу и возращает true только при нажатии на R, что, в свою очередь снова вызывает функцию gameRunning(), которая заново вызывает startGame()
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
