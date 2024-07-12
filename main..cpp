// Snake game using raylib
// 2024.07.12
// tutorial : youtube.com/watch?v=LGqsnM_WEK4

#include <iostream>
#include <deque>
#include "raylib.h"

Color green{ 173,204,96,255 };
Color darkGreen{ 43,51,24,255 };

// define size of game grid
int cellSize{ 30 };
int cellCount{ 25 };

class Snake
{
public:
	// deque to hold all snake body segemnts
	std::deque<Vector2> body = { Vector2{6,9}, Vector2{5,9}, Vector2{4,9} };

	void draw()
	{
		// draw the segments of the snake
		for (unsigned int i{ 0 }; i < body.size(); i++)
		{
			float x = body[i].x;
			float y = body[i].y;
			Rectangle segment{ x * cellSize, y * cellSize, static_cast<float>(cellSize), static_cast<float>(cellSize) };
			DrawRectangleRounded(segment, 0.5, 6, darkGreen);
		}
	}
};

class Food
{
public:

	// position in cells
	Vector2 position{};
	Texture2D texture{};

	Food()
	{
		Image image{ LoadImage("graphics/food.png") };
		texture = LoadTextureFromImage(image);
		UnloadImage(image);
		position = generateRandomPos();
	}

	~Food()
	{
		UnloadTexture(texture);
	}

	void draw()
	{
		DrawTexture(texture, position.x * cellSize, position.y * cellSize, WHITE);
	}

	// randomly position the food
	Vector2 generateRandomPos()
	{
		float x = GetRandomValue(0, cellCount - 1);
		float y = GetRandomValue(0, cellCount - 1);
		return Vector2{ x, y };
	}
};

int main()
{
	std::cout << "Starting the game..." << '\n';

	InitWindow(cellSize * cellCount, cellSize * cellCount, "raylib Snake");

	Food food{};
	Snake snake{};

	SetTargetFPS(60);
	/*----------------------------------------------------*/
	/* game loop starts here							  */
	/*----------------------------------------------------*/
	while (WindowShouldClose() == false)
	{
		BeginDrawing();
		ClearBackground(green);

		food.draw();
		snake.draw();


		EndDrawing();
	}

	CloseWindow();
	return 0;
}