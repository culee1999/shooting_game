# include "raylib.h"
# include "raymath.h"
#include <iostream>
#include <cmath>
using namespace std;

typedef enum GameScreen {TITLE = 0, GAMEPLAY} GameScreen;

int main() {
	int left_ball = 5, cursor = 0, play_or_mute = 0, music_number = 0, game_stop = 0;
	int status = 0; // variable for check that ball bumps into target. 0 = not bump yet, 1 = bump, 2 = game clear, 3 = game over
	int hp = 100;
	float angle = 30, velocity = 10;
	float object_x = 800, object_y = 500;
	float target_x = 1150, target_y = 500;
	float time = 0, distance = 0, height = 0;
	float gravity = 9.80665;
	Vector2 ballPosition = { 0, 0 };
	Vector2 Shoot_Position = { 100, 600 };
	Vector2 Shoot_Position_for_line = { 110, 620 };
	Vector2 Shoot_Position_for_line2 = { 170, 615 };
	GameScreen current_screen = TITLE;

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

	Image image_object = LoadImage("resources/brick.png");
	Texture2D texture_object = LoadTextureFromImage(image_object);
	UnloadImage(image_object);
	image_object = LoadImageFromTexture(texture_object);
	UnloadTexture(texture_object);
	texture_object = LoadTextureFromImage(image_object);
	UnloadImage(image_object);

	Image image_target = LoadImage("resources/dog.png");
	Texture2D texture_target = LoadTextureFromImage(image_target);
	UnloadImage(image_target);
	image_target = LoadImageFromTexture(texture_target);
	UnloadTexture(texture_target);
	texture_target = LoadTextureFromImage(image_target);
	UnloadImage(image_target);

	Image image_stop_button = LoadImage("resources/stop.png");
	Texture2D texture_stop_button = LoadTextureFromImage(image_stop_button);
	UnloadImage(image_stop_button);
	image_stop_button = LoadImageFromTexture(texture_stop_button);
	UnloadTexture(texture_stop_button);
	texture_stop_button = LoadTextureFromImage(image_stop_button);
	UnloadImage(image_stop_button);

	Image image_pause = LoadImage("resources/pause.png");
	Texture2D texture_pause = LoadTextureFromImage(image_pause);
	UnloadImage(image_pause);
	image_pause = LoadImageFromTexture(texture_pause);
	UnloadTexture(texture_pause);
	texture_pause = LoadTextureFromImage(image_pause);
	UnloadImage(image_pause);

	Image image_game_clear = LoadImage("resources/clear.png");
	Texture2D texture_game_clear = LoadTextureFromImage(image_game_clear);
	UnloadImage(image_game_clear);
	image_game_clear = LoadImageFromTexture(texture_game_clear);
	UnloadTexture(texture_game_clear);
	texture_game_clear = LoadTextureFromImage(image_game_clear);
	UnloadImage(image_game_clear);

	Image image_game_over = LoadImage("resources/gameover.png");
	Texture2D texture_game_over = LoadTextureFromImage(image_game_over);
	UnloadImage(image_game_over);
	image_game_over = LoadImageFromTexture(texture_game_over);
	UnloadTexture(texture_game_over);
	texture_game_over = LoadTextureFromImage(image_game_over);
	UnloadImage(image_game_over);

	Image image_start_page = LoadImage("resources/start_page.png");
	Texture2D texture_start_page = LoadTextureFromImage(image_start_page);
	UnloadImage(image_start_page);
	image_start_page = LoadImageFromTexture(texture_start_page);
	UnloadTexture(texture_start_page);
	texture_start_page = LoadTextureFromImage(image_start_page);
	UnloadImage(image_start_page);

	Image image_slingshot = LoadImage("resources/slingshot.png");
	Texture2D texture_slingshot = LoadTextureFromImage(image_slingshot);
	UnloadImage(image_slingshot);
	image_slingshot = LoadImageFromTexture(texture_slingshot);
	UnloadTexture(texture_slingshot);
	texture_slingshot = LoadTextureFromImage(image_slingshot);
	UnloadImage(image_slingshot);
	//-----------------------------------------------------------------

	SetTargetFPS(60);
	while (!WindowShouldClose()) {
		//-------------------------------------------- Update part ------------------------------------------------
		switch (current_screen) {
		case TITLE:
		{
			if (IsKeyDown(KEY_SPACE)) current_screen = GAMEPLAY;
		} break;

		case GAMEPLAY:
		{
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

			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) { // decide ball position by mouse left click
				ballPosition = GetMousePosition();
				if (status == 3) { // when gameover
					if (ballPosition.x >= 435 && ballPosition.x <= 605 && ballPosition.y >= 210 && ballPosition.y <= 500) { // retry
						status = 0;
						hp = 100;
						left_ball = 5;
					}
					else if (ballPosition.x >= 660 && ballPosition.x <= 925 && ballPosition.y >= 210 && ballPosition.y <= 500) { // menu
						status = 0;
						left_ball = 5;
						hp = 100;
						current_screen = TITLE;
					}
				}

				if (status == 2) { // when game clear
					if (ballPosition.x >= 810 && ballPosition.x <= 880 && ballPosition.y >= 410 && ballPosition.y <= 430) { // menu
						status = 0;
						current_screen = TITLE;
					}
				}
				else {
					status = 0;
					distance = 0;
					height = 0;
					time = 0;

					// if click music sign, it will play or mute
					if (ballPosition.x >= 900 && ballPosition.x <= 969 && ballPosition.y >= 10 && ballPosition.y <= 76) {
						if (play_or_mute == 0)
							play_or_mute = 1;
						else
							play_or_mute = 0;
					}

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

					if (ballPosition.x >= 780 && ballPosition.x <= 850 && ballPosition.y >= 10 && ballPosition.y <= 80)
						game_stop = 1;

					if (game_stop == 1) {
						if (ballPosition.x >= 400 && ballPosition.x <= 610 && ballPosition.y >= 370 && ballPosition.y <= 395) // back to game
							game_stop = 0;
						else if (ballPosition.x >= 670 && ballPosition.x <= 765 && ballPosition.y >= 370 && ballPosition.y <= 395) { // retry
							game_stop = 0;
							left_ball = 5;
						}
						else if (ballPosition.x >= 820 && ballPosition.x <= 915 && ballPosition.y >= 370 && ballPosition.y <= 395) { // menu
							game_stop = 0;
							current_screen = TITLE;
						}
					}

					if (ballPosition.x < 200 && ballPosition.y > 600) {
						cursor = 1;
						angle = fabs(Vector2Angle(ballPosition, Shoot_Position));
						velocity = fabs(Vector2Distance(ballPosition, Shoot_Position));
					}
					else
						cursor = 0;
				}
			}

			if (IsKeyDown(KEY_S)) { // press s button -> shootint the ball, if ball meets the target -> hp decrease
				if (status == 0) {
					time += 0.5;
					distance = velocity * cos(angle) * time;
					height = (velocity * sin(angle) * time) - (gravity * pow(time, 2)) / 2;

					// collide with target
					if (ballPosition.x + distance + 15 >= target_x && ballPosition.x + distance - 15 <= 1250 && ballPosition.y - height - 15 >= target_y && ballPosition.y - height + 15 <= 780) {
						left_ball--;
						status = 1;
						hp -= 10;
					}

					// collide with object
					if (ballPosition.x + distance + 15 >= object_x && ballPosition.x + distance - 15 <= object_x + 10 && ballPosition.y - height - 15 >= object_y && ballPosition.y - height + 15 <= object_y + 500) {
						left_ball--;
						status = 1;
					}
				}
			}
			if (IsKeyDown(KEY_U)) // press n button -> wall(obstacle) move upper way
				object_y--;
			if (IsKeyDown(KEY_D)) // press n button -> wall(obstacle) move down way
				object_y++;
			if (IsKeyDown(KEY_R)) // press m button -> wall(obstacle) move right way
				object_x++;
			if (IsKeyDown(KEY_L)) // press n button -> wall(obstacle) move left way
				object_x--;
			if (IsKeyDown(KEY_C)) // press c button -> game clear
				hp = 0;
		} break;
		default: break;
		}

//---------------------------------------------------------------------------------------------------------

//---------------------------------------------- Draw Part ------------------------------------------------
		BeginDrawing();
		ClearBackground(RAYWHITE);
		switch (current_screen) {
		case TITLE:
		{
			DrawTexture(texture_start_page, 0, 0, WHITE);
		} break;
		case GAMEPLAY:
		{
			DrawTexture(texture, 0, 0, WHITE);

			if (hp == 0) {
				status = 2;
				if (status == 2)
					DrawTexture(texture_game_clear, 280, 150, WHITE);
			}

			if (left_ball == 0) {
				status = 3;
				if(status == 3)
					DrawTexture(texture_game_over, 280, 150, WHITE);
			}

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

			if (cursor == 1) {
				DrawCircle(ballPosition.x + distance, ballPosition.y - height, 15, GREEN); // ball
				DrawLineEx(Shoot_Position_for_line, ballPosition, 5, BLACK);
				DrawLineEx(Shoot_Position_for_line2, ballPosition, 5, BLACK);
			}
			DrawTexture(texture_slingshot, Shoot_Position.x, Shoot_Position.y, WHITE);

			// --------- Draw BGM part -----------
			if (play_or_mute == 0)
				DrawTexture(texture_music, 900, 10, WHITE);
			else
				DrawTexture(texture_mute, 900, 10, WHITE);
			DrawTriangle(Vector2{ 980, 7 }, Vector2{ 980, 70 }, Vector2{ 1000 ,43 }, BLACK);
			DrawTriangle(Vector2{ 869, 43 }, Vector2{ 889, 73 }, Vector2{ 889 , 10 }, BLACK);
			// -----------------------------------

			// ---------- Draw Stop butoon ------------
			DrawTexture(texture_stop_button, 780, 10, WHITE);
			if (game_stop == 1) {
				DrawTexture(texture_pause, 280, 150, WHITE);
			}
			// ----------------------------------------

			// ---------- Draw Object, Target part ----------
			DrawTexture(texture_object, object_x, object_y, WHITE); // object
			DrawTexture(texture_target, target_x, target_y, WHITE); // target
			// --------------------------------------
		} break;
		default: break;
		}
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