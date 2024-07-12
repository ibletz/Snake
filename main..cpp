// Snake game using raylib
// 2024.07.12
// tutorial : youtube.com/watch?v=LGqsnM_WEK4

#include <iostream>
#include "raylib.h"

Color green{ 173,204,96,255 };
Color darkGreen{ 43,51,24,255 };

int main()
{
	std::cout << "Starting the game..." << '\n';

	const int window_width(750);
	const int window_height(750);

	InitWindow(window_width, window_height, "raylib Snake");

	SetTargetFPS(60);
	/*----------------------------------------------------*/
	/* game loop starts here							  */
	/*----------------------------------------------------*/
	while (WindowShouldClose() == false)
	{
		BeginDrawing();
		ClearBackground(green);
	
		
		
		EndDrawing();
	}

	CloseWindow();
	return 0;
}