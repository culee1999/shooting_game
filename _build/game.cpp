# include "raylib.h"
# include "raymath.h"
#include <iostream>
#include <cmath>
using namespace std;

int main() {
	int left_ball = 5, cursor = 0, play_or_mute = 0, music_number = 0;
	int bump = 0; // variable for check that ball bumps into target. 0 = not bump yet, 1 = bump, 2 = game clear
	int hp = 100;
	float angle = 30, velocity = 10;
	float brick_x = 800, brick_y = 500;
	float time = 0, distance = 0, height = 0;
	float gravity = 9.80665;
	Vector2 ballPosition = { 0, 0 };
	Vector2 Shoot_Position = { 30, 700 };

	angle *= 3.1415926536 / 180;

	const int WindowWidth = 1280;
	const int WindowHeight = 720;
	
	InitWindow(WindowWidth, WindowHeight, "Shooting Game");

//------------------ bgm variable -----------------------------
	InitAudioDevice();
	Music music = LoadMusicStream("resources/bgm.mp3");
	Music music2 = LoadMusicStream("resources/bgm2.mp3");
	PlayMusicStream(music);
	PlayMusicStream(music2);
//---------------------------------------------------------

//----------------- image variable --------------------------------
	Image image = LoadImage("resources/window.png");
	Texture2D texture = LoadTextureFromImage(image);
	UnloadImage(image);
	image = LoadImageFromTexture(texture);
	UnloadTexture(texture);
	texture = LoadTextureFromImage(image);
	UnloadImage(image);

	Image image_music = LoadImage("resources/music.png");
	Texture2D texture_music = LoadTextureFromImage(image_music);
	UnloadImage(image_music);
	image_music = LoadImageFromTexture(texture_music);
	UnloadTexture(texture_music);
	texture_music = LoadTextureFromImage(image_music);
	UnloadImage(image_music);

	Image image_mute = LoadImage("resources/music_mute.png");
	Texture2D texture_mute = LoadTextureFromImage(image_mute);
	UnloadImage(image_mute);
	image_mute = LoadImageFromTexture(texture_mute);
	UnloadTexture(texture_mute);
	texture_mute = LoadTextureFromImage(image_mute);
	UnloadImage(image_mute);
//-----------------------------------------------------------------

	SetTargetFPS(60);

	while (!WindowShouldClose()) {
//---------------------- bgm ---------------------------
		if (play_or_mute == 1) {
			StopMusicStream(music);
			StopMusicStream(music2);
		}
		else if (music_number == 0) {
			UpdateMusicStream(music);
			PlayMusicStream(music);
		}
		else {
			UpdateMusicStream(music2);
			PlayMusicStream(music2);
		}
//------------------------------------------------------

		BeginDrawing();
		ClearBackground(RAYWHITE);
		DrawTexture(texture, 0, 0, WHITE);

		if (hp == 0) {
			DrawRectangle(200, 100, 900, 400, WHITE);
			DrawText("Game Clear!!", 350, 200, 100, GRAY);
			bump = 2;
		}

		if (bump != 2 && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) { // decide ball position by mouse left click
			bump = 0;
			distance = 0;
			height = 0;
			time = 0;
			ballPosition = GetMousePosition();

			// if click musig sign, it will play or mute
			if (ballPosition.x >= 900 && ballPosition.x <= 969 && ballPosition.y >= 10 && ballPosition.y <= 76) {
				if (play_or_mute == 0)
					play_or_mute = 1;
				else
					play_or_mute = 0;
			}

//------------------------------------ bgm click part --------------------------------------------------------------
			if (ballPosition.x >= 869 && ballPosition.x <= 889 && ballPosition.y >= 10 && ballPosition.y <= 73)
				if (music_number > 0) {
					music_number--;
					if (music_number == 1 && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
						StopMusicStream(music2);
						PlayMusicStream(music2);
					}

					if (music_number == 0 && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
						StopMusicStream(music);
						PlayMusicStream(music);
					}
				}
			if (ballPosition.x >= 980 && ballPosition.x <= 1000 && ballPosition.y >= 7 && ballPosition.y <= 70)
				if (music_number <= 0) {
					music_number++;
					if (music_number == 1 && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
						StopMusicStream(music2);
						PlayMusicStream(music2);
					}

					if (music_number == 0 && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
						StopMusicStream(music);
						PlayMusicStream(music);
					}
				}
//---------------------------------------------------------------------------------------------------------------------
			

			if (ballPosition.x < 400 && ballPosition.y > 600) {
				cursor = 1;
				angle = Vector2Angle(ballPosition, Shoot_Position);
				velocity = fabs(Vector2Distance(ballPosition, Shoot_Position));
			}
			else
				cursor = 0;
		}

		if (IsKeyDown(KEY_S)) { // press s button -> shootint the ball, if ball meets the target -> hp decrease
			if (bump == 0 && ballPosition.y + height <= 680 && ballPosition.x + distance <= 1220) {
				time += 0.5;
				distance = -velocity * cos(angle) * time;
				height = -(velocity * sin(angle) * time) + (gravity * pow(time, 2)) / 2;

				if (ballPosition.x + distance + 15 >= 1150 && ballPosition.x + distance - 15 <= 1250 && ballPosition.y + height - 15 >= 580 && ballPosition.y + height + 15 <= 780) {
					bump = 1;
					hp -= 10;
				}
			}
		}

		// ------------------- for test -----------------------------
		if (IsKeyDown(KEY_A)) // press a button -> decrese hp
			if(left_ball <= 5 && left_ball > 0)
				left_ball--;

		if (IsKeyDown(KEY_M)) // press m button -> wall(obstacle) move
			brick_x++;

		DrawRectangleLines(0, 600, 200, 200, RED);
	
		//------------------------------------------------------------

		DrawRectangle(0, 0, 250, 60, BLUE); // ball number rectangle
		DrawRectangle(1030, 0, 250, 60, BLUE); // hp rectangle
		DrawRectangle(1080, 10, 2 * hp, 30, RED); // hp bar

		// --------------- Draw how many balls are left and hp bar---------------------
		for (int i = 4; i >= 5 - left_ball; i--)
			DrawCircle(20 + i * 50, 30, 15, GREEN);
		for (int i = 0; i < 5 - left_ball; i++) {
			DrawCircle(20 + i * 50, 30, 15, GREEN);
			DrawLine(10 + i * 50, 13, 40 + i * 50, 47, RED);
		}
		DrawText("HP", 1035, 10, 30, GRAY);
		// -----------------------------------------------------------------
	
		if(cursor == 1)
			DrawCircle(ballPosition.x + distance, height + ballPosition.y, 15, GREEN);
		DrawRectangle(brick_x, brick_y, 10, 500, BLUE);
		DrawRectangle(30, 700, 15, 15, RED);

		DrawRectangle(1150, 580, 100, 200, GREEN);

		// --------- Draw BGM part -----------
		if (play_or_mute == 0)
			DrawTexture(texture_music, 900, 10, WHITE);
		else
			DrawTexture(texture_mute, 900, 10, WHITE);
		DrawTriangle(Vector2{ 980, 7 }, Vector2{ 980, 70 }, Vector2{ 1000 ,43 }, BLACK);
		DrawTriangle(Vector2{ 869, 43 }, Vector2{ 889, 73 }, Vector2{ 889 , 10 }, BLACK);
		// -----------------------------------

		EndDrawing();
	}

	UnloadTexture(texture);
	UnloadTexture(texture_music);
	UnloadTexture(texture_mute);
	UnloadMusicStream(music);
	UnloadMusicStream(music2);
	CloseAudioDevice();
	CloseWindow();
	return 0;
}