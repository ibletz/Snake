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
// offest for game border
int offset{ 75 };

// keep trak of time last snake update occurred
double lastUpdateTime{ 0 };

// check to prevent food appearing on snake body
bool elementInDeque(Vector2 element, std::deque<Vector2> deque)
{
	for (unsigned int i = 0; i < deque.size(); i++)
	{
		if (Vector2Equals(deque[i], element))
			return true;
	}
	return false;
}

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
	// decide whether to move snake or grow snake, based on food eaten
	bool addSegment = false;

	void draw()
	{
		// draw the segments of the snake
		for (unsigned int i{ 0 }; i < body.size(); i++)
		{
			float x = body[i].x;
			float y = body[i].y;
			Rectangle segment{ offset + x * cellSize, offset + y * cellSize, static_cast<float>(cellSize), static_cast<float>(cellSize) };
			DrawRectangleRounded(segment, 0.5, 6, darkGreen);
		}
	}

	void update()
	{
		// if the snake has eaten food add a segment to the body
		body.push_front(Vector2Add(body[0], direction));
		if (addSegment)
			addSegment = false;
		// else move the snake in direction specified by direction vector (pop the tail)
		else
			body.pop_back();
	}

	void reset()
	{
		body = { Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9} };
		direction = { 1, 0 };
	}
};

class Food
{
public:

	// position in cells
	Vector2 position{};
	Texture2D texture{};

	Food(std::deque<Vector2> snakeBody)
	{
		Image image{ LoadImage("graphics/food.png") };
		texture = LoadTextureFromImage(image);
		UnloadImage(image);
		position = generateRandomPos(snakeBody);
	}

	~Food()
	{
		UnloadTexture(texture);
	}

	void draw()
	{
		DrawTexture(texture, offset + position.x * static_cast<float>(cellSize), offset + position.y * static_cast<float>(cellSize), WHITE);
	}

	// randomly generate a cell position
	Vector2 generateRandomCell()
	{
		float x = static_cast<float>(GetRandomValue(0, static_cast<float>(cellCount) - 1.f));
		float y = static_cast<float>(GetRandomValue(0, static_cast<float>(cellCount) - 1.f));
		return Vector2{ x, y };
	}

	// randomly position the food
	Vector2 generateRandomPos(std::deque<Vector2> snakeBody)
	{
		Vector2 position{ generateRandomCell() };
		while (elementInDeque(position, snakeBody))
			position = generateRandomCell();
		return position;
	}
};

// class that handles game state
class Game
{
public:
	Snake snake{};
	Food food{ snake.body };
	bool running{ true };
	int score{ 0 };

	void draw()
	{
		food.draw();
		snake.draw();
	}

	void update()
	{
		if (running)
		{
			snake.update();
			checkCollisionWithFood();
			checkCollisionWithEdges();
			checkCollisionWithTail();
		}
	}

	// collision check snake head with food (snake eats food)
	void checkCollisionWithFood()
	{
		if (Vector2Equals(snake.body[0], food.position))
		{
			food.position = food.generateRandomPos(snake.body);
			snake.addSegment = true;
			score++;
		}
	}

	// check if the snake head has collided with the edge of the window
	void checkCollisionWithEdges()
	{
		if (snake.body[0].x == cellCount ||
			snake.body[0].x == -1 ||
			snake.body[0].y == cellCount ||
			snake.body[0].y == -1)
			gameOver();
	}

	// check if the snake collides with itself
	void checkCollisionWithTail()
	{
		std::deque<Vector2> headlessBody = snake.body;
		headlessBody.pop_front();
		if (elementInDeque(snake.body[0], headlessBody))
			gameOver();
	}

	void gameOver()
	{
		snake.reset();
		food.position = food.generateRandomPos(snake.body);
		running = false;
		score = 0;
	}

};

int main()
{
	InitWindow(2 * offset + cellSize * cellCount, 2 * offset + cellSize * cellCount, "Retro Snake");
	SetTargetFPS(60);

	Game game{};

	/*----------------------------------------------------*/
	/* game loop starts here							  */
	/*----------------------------------------------------*/
	while (WindowShouldClose() == false)
	{
		BeginDrawing();

		// updating
		if (eventTriggered(0.2))
			game.update();

		// control the snake with keys
		// prevent the snake from moving in the opposite direction
		if (IsKeyPressed(KEY_W) && game.snake.direction.y != 1)
		{
			game.snake.direction = { 0,-1 };
			game.running = true;
		}
		if (IsKeyPressed(KEY_S) && game.snake.direction.y != -1)
		{
			game.snake.direction = { 0, 1 };
			game.running = true;
		}
		if (IsKeyPressed(KEY_A) && game.snake.direction.x != 1)
		{
			game.snake.direction = { -1, 0 };
			game.running = true;
		}
		if (IsKeyPressed(KEY_D) && game.snake.direction.x != -1)
		{
			game.snake.direction = { 1, 0 };
			game.running = true;
		}


		// drawing
		ClearBackground(green);
		DrawRectangleLinesEx(Rectangle{ static_cast<float>(offset) - 5,
										static_cast<float>(offset) - 5,
										static_cast<float>(cellSize) * cellCount + 10,
										static_cast<float>(cellSize) * cellCount + 10 }, 5, darkGreen);
		DrawText("Retro Snake", offset - 5, 20, 40, darkGreen);
		DrawText(TextFormat("%i",game.score), offset - 5, offset + cellSize * cellCount + 10, 40, darkGreen);
		game.draw();

		EndDrawing();
	}

	CloseWindow();
	return 0;
}