#include <iostream>
#include <thread>
#include <Windows.h>
#include <algorithm>
#include <vector>


using namespace std;

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN

#define VK_LEFT           0x41 // A button
#define VK_RIGHT          0x44 // D button

#define VK_UP			  0x57 // W button
#define VK_DOWN			  0x53 // S button

const int height = 20;
const int width = 20;
char board[20][20] = {};

void ClearScreen()
{
	COORD cursorPosition;
	cursorPosition.X = 0;
	cursorPosition.Y = 0;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPosition);
}

void HideCursor()
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(consoleHandle, &info);
}

void RenderBoard()
{
	for (size_t i = 0; i < width + 2; i++)
	{
		cout << '#';
	}
	cout << '\n';
	for (size_t i = 0; i < height; i++)
	{
		std::cout << '#';
		for (size_t j = 0; j < width; j++)
		{
			std::cout << board[i][j];
		}
		std::cout << '#' << '\n';

	}
	for (size_t i = 0; i < width + 2; i++)
	{
		cout << '#';
	}
}

void GenerateApple()
{
	//create new seed
	srand(time(NULL));
	int randomX = (rand() % width - 1) + 1;
	int randomY = (rand() % height - 1) + 1;
	while (board[randomY][randomX] == 'S')
	{
		randomX = (rand() % width - 1) + 1;
		randomY = (rand() % height - 1) + 1;
	}
	board[randomY][randomX] = 'A';

}

void AddBody(int y, int x, vector<pair<int, int>>* snake)
{
	board[y][x] = 'S';
	pair<int, int> body = { y,x };
	(*snake).push_back(body);
}

int main()
{
	HideCursor();
	char direction = 'r';

	for (size_t i = 0; i < height; i++)
	{
		fill(board[i], board[i] + 20, ' ');
	}
	board[10][10] = 'S';

	vector<pair<int, int>> snake;
	pair<int, int> head = { 10,10 };//y,x
	snake.push_back(head);
	GenerateApple();
	int LastPosX = 0, LastPosY = 0,score=0;

	while (true)
	{
		bool interupt = false;

		int y = 0, x = 0;
		//input
		if (GetAsyncKeyState(VK_RIGHT) != 0 && GetAsyncKeyState(VK_RIGHT) != 1 && direction != 'l')
		{
			interupt = true;
			direction = 'r';
		}

		if (GetAsyncKeyState(VK_LEFT) != 0 && GetAsyncKeyState(VK_LEFT) != 1 && !interupt && direction != 'r')
		{
			interupt = true;
			direction = 'l';
		}

		if (GetAsyncKeyState(VK_UP) != 0 && GetAsyncKeyState(VK_UP) != 1 && !interupt && direction != 'd')
		{
			interupt = true;
			direction = 'u';
		}

		if (GetAsyncKeyState(VK_DOWN) != 0 && GetAsyncKeyState(VK_DOWN) != 1 && !interupt && direction != 'u')
		{
			interupt = true;
			direction = 'd';
		}


		
		switch (direction)
		{
		case 'u':
			y--;
			break;
		case 'd':
			y++;
			break;
		case 'r':
			x++;
			break;
		case 'l':
			x--;
			break;

		default:
			break;
		}

		int prevPosY, prevPosX, prevVarY = 0, prevVarX = 0;

		//Head Logic

		prevPosY = snake.at(0).first;
		prevPosX = snake.at(0).second;

		
		snake.at(0).first += y;
		snake.at(0).second += x;
		if (snake.at(0).second < 0)
		{
			snake.at(0).second = width - 1;
		}
		else if (snake.at(0).second > width - 1)
		{
			snake.at(0).second = 0;
		}
		if (snake.at(0).first < 0)
		{
			snake.at(0).first = height - 1;
		}
		else if (snake.at(0).first > height - 1)
		{
			snake.at(0).first = 0;
		}

		if (board[snake.at(0).first][snake.at(0).second] == 'A')
		{
			score += 100;
			AddBody(LastPosY, LastPosX, &snake);
			board[snake.at(0).first][snake.at(0).second] = ' ';
			GenerateApple();
		}
		//Game Over
		else if (board[snake.at(0).first][snake.at(0).second] == 'S')
		{
			break;
		}
		std::swap(board[prevPosY][prevPosX], board[snake.at(0).first][snake.at(0).second]);


		//Body Logic
		for (size_t i = 1; i < snake.size(); i++)
		{
			prevVarY = prevPosY;
			prevVarX = prevPosX;

			std::swap(board[snake.at(i).first][snake.at(i).second], board[prevPosY][prevPosX]);


			prevPosY = snake.at(i).first;
			prevPosX = snake.at(i).second;

			snake.at(i).first = prevVarY;
			snake.at(i).second = prevVarX;


		}
		LastPosX = prevPosX;
		LastPosY = prevPosY;

		cout << "Score: " << score << endl;
		RenderBoard();
		std::this_thread::sleep_for(std::chrono::milliseconds(150));
		ClearScreen();
	}
	system("cls");
	cout << "Game Over! Your score is " << score << endl;

}