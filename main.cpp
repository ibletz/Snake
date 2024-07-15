// Snake game using raylib
// 2024.07.12
// tutorial : youtube.com/watch?v=LGqsnM_WEK4

#include <iostream>
#include <deque>
#include "raylib.h"
#include "raymath.h"

Color green{ 173,204,96,255 };
Color darkGreen{ 43,51,24,255 };

// define size of game grid
int cellSize{ 30 };
int cellCount{ 25 };

// keep trak of time last snake update occurred
double lastUpdateTime{ 0 };

// limit snake updating to `interval` so that the speed is not to fast
bool eventTriggered(double interval)
{
	double currentTime{ GetTime() };
	if (currentTime - lastUpdateTime >= interval)
	{
		lastUpdateTime = currentTime;
		return true;
	}
	return false;
}

class Snake
{
public:
	// deque to hold all snake body segemnts
	std::deque<Vector2> body = { Vector2{6,9}, Vector2{5,9}, Vector2{4,9} };
	Vector2 direction{ 1, 0 };

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

	void update()
	{
		// move the snake in direction specified by direction vector
		body.pop_back();
		body.push_front(Vector2Add(body[0], direction));
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

		// updating
		if (eventTriggered(0.2))
			snake.update();

		// control the snake with keys
		// prevent the snake from moving in the opposite direction
		if (IsKeyPressed(KEY_W) && snake.direction.y != 1)
			snake.direction = { 0,-1 };
		if (IsKeyPressed(KEY_S) && snake.direction.y != -1)
			snake.direction = { 0, 1 };
		if (IsKeyPressed(KEY_A) && snake.direction.x != 1)
			snake.direction = { -1, 0 };
		if (IsKeyPressed(KEY_D) && snake.direction.x != -1)
			snake.direction = { 1, 0 };
		


		// drawing
		ClearBackground(green);
		food.draw();
		snake.draw();


		EndDrawing();
	}

	CloseWindow();
	return 0;
}