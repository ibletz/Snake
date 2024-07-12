// Snake game using raylib
// 2024.07.12
// tutorial : youtube.com/watch?v=LGqsnM_WEK4

#include <iostream>
#include "raylib.h"

Color green{ 173,204,96,255 };
Color darkGreen{ 43,51,24,255 };

// define size of game grid
int cellSize{ 30 };
int cellCount{ 25 };

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
		position = GenerateRandomPos();
	}

	~Food()
	{
		UnloadTexture(texture);
	}

	void Draw()
	{
		DrawTexture(texture, position.x * cellSize, position.y * cellSize, WHITE);
	}

	// randomly position the food
	Vector2 GenerateRandomPos()
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

	SetTargetFPS(60);
	/*----------------------------------------------------*/
	/* game loop starts here							  */
	/*----------------------------------------------------*/
	while (WindowShouldClose() == false)
	{
		BeginDrawing();
		ClearBackground(green);
		
		food.Draw();



		EndDrawing();
	}

	CloseWindow();
	return 0;
}