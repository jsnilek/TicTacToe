#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <string>

enum class player { empty, player1, player2 };
typedef player game[9];
const auto number_of_sectors = 3;
// Override base class with your custom functionality
class TicTacToe : public olc::PixelGameEngine
{

public:
	//game all_solutions[362880];
	//bool all_winner[362880] = { };
	void DrawCross(int32_t x, int32_t y, int32_t w, int32_t h, olc::Pixel p);
	void DrawCross(int32_t x, int32_t y, int32_t r, olc::Pixel p);
	void Reset_game();
	[[nodiscard]] player current_player() const { return (round % 2) ? player::player1 : player::player2; }
	void SolveTicTacToe();
	void IA_play_rand();
	void IA_play();
	player who_win_game(const game& actual_game_field);
	int round = 0;
	player winner;
	game game_field;

	TicTacToe()
	{
		sAppName = "TicTacToe";
	}
public:
	bool OnUserCreate() override
	{

			Reset_game();
		//SolveTicTacToe();
		// Called once at the start, so create things here
		return true;
	}
	bool OnUserUpdate(float fElapsedTime) override
	{
		winner = who_win_game(game_field);

		const olc::vi2d mouse = { GetMouseX() / (ScreenWidth() / number_of_sectors), GetMouseY() / (ScreenWidth() / number_of_sectors) };
		Clear(olc::BLACK);
		FillRect(ScreenWidth() / number_of_sectors - 1, 0, 2, ScreenHeight(), olc::WHITE);
		FillRect(2 * ScreenWidth() / number_of_sectors - 1, 0, 2, ScreenHeight(), olc::WHITE);

		FillRect(0, ScreenHeight() / number_of_sectors - 1, ScreenWidth(), 2, olc::WHITE);
		FillRect(0, 2 * ScreenHeight() / number_of_sectors - 1, ScreenWidth(), 2, olc::WHITE);
		const olc::vi2d screen_sector = { 0,0 };

		//mouse input
		if (GetMouse(0).bPressed && (game_field[mouse.y * number_of_sectors + mouse.x] == player::empty) && winner == player::empty)
		{
			game_field[mouse.y * number_of_sectors + mouse.x] = current_player();
			round++;
		}
		//key board input
		if (GetKey(olc::R).bReleased)
		{
			Reset_game();
		}
		if (GetKey(olc::A).bReleased && winner == player::empty)
		{
			IA_play_rand();
		}

		if (GetKey(olc::Q).bReleased)
		{
			IA_play();
		}


		// display winner
		if (winner != player::empty)
		{
			const std::string temp = winner == player::player1 ? "player O" : "player X";

			DrawString(0, 0, std::string("winner is " + temp), olc::RED);
		}

		//display game state
		for (auto x = 0; x < number_of_sectors; ++x)
		{
			for (auto y = 0; y < number_of_sectors; ++y)
			{
				if (game_field[y * number_of_sectors + x] == player::player1)
					DrawCircle(x * ScreenWidth() / number_of_sectors + (ScreenWidth() / 2 / number_of_sectors),
						y * ScreenHeight() / number_of_sectors + (ScreenHeight() / 2 / number_of_sectors), 12, olc::YELLOW);

				if (game_field[y * number_of_sectors + x] == player::player2)
					DrawCross(x * ScreenWidth() / number_of_sectors + (ScreenWidth() / 2 / number_of_sectors),
						y * ScreenHeight() / number_of_sectors + (ScreenHeight() / 2 / number_of_sectors), 12, olc::BLUE);
			}
		}
		return true;
	}

	bool OnUserDestroy() override
	{

		return true;
	}
};
int main()
{
	TicTacToe demo;
	if (demo.Construct(256, 240, 4, 4))
		demo.Start();
	return 0;
}

void TicTacToe::DrawCross(int32_t x, int32_t y, int32_t w, int32_t h, olc::Pixel p)
{
	DrawLine(x, y, x + w, y + h, olc::WHITE);
	DrawLine(x + w, y, y + h, x, olc::WHITE);
}

void TicTacToe::DrawCross(int32_t x, int32_t y, int32_t r, olc::Pixel p)
{
	DrawLine(x - r, y - r, x + r, y + r, p);
	DrawLine(x + r, y - r, x - r, y + r, p);
}

void TicTacToe::Reset_game()
{
	for (auto& i : game_field)
	{
		i = player::empty;
	}
	winner = player::empty;
	round = 0;
}
/*
void TicTacToe::SolveTicTacToe()
{
	for (auto start_position = 0; start_position < 9; ++start_position)
	{
		for (auto start_position = 0; start_position < 8; ++start_position)
		{
			for (auto start_position = 0; start_position < 7; ++start_position)
			{
				for (auto start_position = 0; start_position < 6; ++start_position)
				{
					for (auto start_position = 0; start_position < 5; ++start_position)
					{
						for (auto start_position = 0; start_position < 4; ++start_position)
						{
							for (auto start_position = 0; start_position < 3; ++start_position)
							{
								for (auto start_position = 0; start_position < 2; ++start_position)
								{

								}
							}
						}
					}
				}
			}
		}
	}

	for (auto attempt = 0; attempt < 362880; ++attempt)
	{

	}
	for (auto x = 0; x < 3 ; ++x)
		for (auto y = 0; y < 3; ++y)
		{
			game_field[y * 3 + x] = current_player();
			round++;
			if (who_win_game() != player::empty) return;
		}




}
*/
void TicTacToe::IA_play()
{}
void TicTacToe::IA_play_rand()
{
	auto is_turn_valid = false;
	while (!is_turn_valid)
	{
		const auto ia_play = rand() % 9;
		if (game_field[ia_play] == player::empty)
		{
			is_turn_valid = true;
			game_field[ia_play] = current_player();
			round++;
		}
	}


}

player TicTacToe::who_win_game(const game& actual_game_field)
{
	auto temp = player::empty;
	for (auto p = 1; p < 3; ++p)
	{
		const auto actual_player = player(p);
		for (auto i = 0; i < 3; i++)
		{
			if (actual_game_field[i * 3] == actual_player && actual_game_field[i * 3 + 1] == actual_player && actual_game_field[i * 3 + 2] == actual_player) { temp = actual_player; }
			if (actual_game_field[i] == actual_player && actual_game_field[3 + i] == actual_player && actual_game_field[i + 6] == actual_player) { temp = actual_player; }
		}

		if (actual_game_field[0] == actual_player && actual_game_field[4] == actual_player && actual_game_field[8] == actual_player) { temp = actual_player; }
		if (actual_game_field[2] == actual_player && actual_game_field[4] == actual_player && actual_game_field[6] == actual_player) { temp = actual_player; }
	}
	return temp;
}

