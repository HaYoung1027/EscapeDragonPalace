#pragma once

#define ESC 27 // ESC Ű ��

// �Լ�
void GameStartScreen();
void GameOverScreen();
void GameClearSceen();
void DrawStartScreen();
void ReturnStartScreen();
void DrawGameClearScreen();
bool GetGameStart();
bool GetIsGameOver();
void SetIsGameOver(int src);
bool GetTextE();
void SetTextE(int src);
bool GetControlScreen();
void SetControlScreen(bool src);
void DrawControlsScreen();


static char Controls[SCREEN_HEIGHT][SCREEN_WIDTH] = {
"                     ���۹� ����",
"",
"",
"",
"",
" ��  ��  ����",
"        _____",
"       |  W  |                     _______________",
"       |_____|                    |    SPACEBAR   |",
"  _____ _____ _____               |_______________|",
" |  A  |  S  |  D  |",		      
" |_____|_____|_____|                    ����",
" ",
" ����  �Ʒ�  ������",
};
