#include "init.h"
#include "map.h"
#include "screens.h"
#include "weapon.h"
#include "turtle.h"
#include "Rabbit.h"

bool GameStart = false;		// ���� ���� ����
bool IsGameOver = false;	// ���� ���� ����
bool textE = true;			// ���� ����Ʈ ȿ�� (���, ȸ������ �����Ÿ�)

bool ControlScreen = false; // ���� ȭ�� ����

// ���ӿ��� ȭ��
void GameOverScreen() {
	_DrawText(28, 4, "�����   ���   ��   ��  �����");
	_DrawText(28, 5, "��     ��  ��  ��� ���  ��      ");
	_DrawText(28, 6, "�� ���  �����  �� �� ��  �����");
	_DrawText(28, 7, "��  ��  ��  ��  ��   ��  ��      ");
	_DrawText(28, 8, "�����  ��  ��  ��   ��  �����   ");
			   
	_DrawText(28, 10, "�����  ��   ��  �����  �����");
	_DrawText(28, 11, "��  ��  ��   ��  ��     ��  ��");
	_DrawText(28, 12, "��  ��  ��   ��  �����  �����");
	_DrawText(28, 13, "��  ��   �� ��   ��     ��  ��");
	_DrawText(28, 14, "�����    ��    �����  ��   ��");

	_DrawText(36, 16, " (\\(\\ ");
	_DrawText(36, 17, " (x-x)");
	_DrawText(36, 18, "o(   )");
}

// ���ӽ��� ȭ��
void GameStartScreen()
{

	_DrawText(23, 2, "�����  �����  �����   ���   �����  �����");
	_DrawText(23, 3, "��     ��     ��     ��  ��  ��  ��  ��");
	_DrawText(23, 4, "�����  �����  ��     �����  �����  �����");
	_DrawText(23, 5, "��        ��  ��     ��  ��  ��     ��");
	_DrawText(23, 6, "�����  �����  �����  ��  ��  ��     �����");
			   
	_DrawText(23, 8, "����   �����   ���   �����  �����  ��  ��");
	_DrawText(23, 9, "��  ��  ��  ��  ��  ��  ��     ��  ��  ��� ��");
	_DrawText(23, 10, "��  ��  �����  �����  �� ���  ��  ��  �� ���");
	_DrawText(23, 11, "��  ��  �� ��   ��  ��  ��  ��  ��  ��  ��  ��");
	_DrawText(23, 12, "����   ��  ��  ��  ��  �����  �����  ��  ��");
			   
	_DrawText(23, 14, "�����   ���   ��      ���   �����  �����");
	_DrawText(23, 15, "��  ��  ��  ��  ��     ��  ��  ��     ��");
	_DrawText(23, 16, "�����  �����  ��     �����  ��     �����");
	_DrawText(23, 17, "��     ��  ��  ��     ��  ��  ��     ��");
	_DrawText(23, 18, "��     ��  ��  �����  ��  ��  �����  �����");
}

// ���� Ŭ���� ȭ��
void GameClearSceen()
{
	_DrawText(26, 3, "�����    ���    ��   ��   �����");
	_DrawText(26, 4, "��      ��  ��   ��� ���   ��      ");
	_DrawText(26, 5, "�� ���   �����   �� �� ��   �����");
	_DrawText(26, 6, "��  ��   ��  ��   ��   ��   ��      ");
	_DrawText(26, 7, "�����   ��  ��   ��   ��   �����   ");

	_DrawText(25, 9, "�����  ��     �����   ���   �����");
	_DrawText(25, 10, "��     ��     ��     ��  ��  ��  ��");
	_DrawText(25, 11, "��     ��     �����  �����  �����");
	_DrawText(25, 12, "��     ��     ��     ��  ��  �� ��");
	_DrawText(25, 13, "�����  �����  �����  ��  ��  ��  ��");

	_DrawText(29, 15, "       ______");
	_DrawText(29, 16, " ___ _/ \\__/ \\_   /|");
	_DrawText(29, 17, "(_x / \\ /  \\ / \\_/ |");
	_DrawText(29, 18, " \\__ -----------__/");
	_DrawText(29, 19, " (___\\_|_|_|_|_/___)");
}

// ���� ���� ȭ�� ��� �Լ�
void DrawStartScreen() {
	// ���� ���� ���� ��
	while (!GameStart) {
		// Ű �Է��� ������
		if (_kbhit()) {
			GameStart = true;   // ���� ����
			_getch();			// �Է� ���� ����
		}
		textE = !textE; // ���� ����Ʈ ȿ�� (���, ȸ������ �����Ÿ�)
		_Invalidate();
		Sleep(500);
	}
}

// ���� ���� ȭ�� ��� �Լ�
void ReturnStartScreen() {
	// ���� ������ ��
	while (IsGameOver) {
		// Ű �Է��� ������
		if (_kbhit()) {
			if (_getch() == ESC) {	// ESC Ű �Է½�
				// ESC Ű�� ������ ���� ����
				system("cls"); // ȭ�� �����
				exit(0); // ���α׷� ����
			}
			else {
				IsGameOver = false;		// ���ӿ��� ���� false ����
				GameStart = false;		// ���ӽ��� ���� false ����
				SetWeaponChosen(false);	// ���� ���ÿ��� false�� ����
				SetMapSetting(false);	// ������ ���� �ʱ�ȭ
				SetIsNearLadder(false); // ��ٸ� ��ó ���� false�� ����
				SetInvincibleTime(false); // �÷��̾� ���� �ð� false�� ����
				SetMapStatus(E_Jail);	// ���� ������ �̵�
				SetPlusX(0);			// X ��ǥ ������ 0���� ����
			}
		}
		textE = !textE; // ���� ����Ʈ ȿ�� (���, ȸ������ �����Ÿ�)
		_Invalidate();
		Sleep(500);
	}

}

// ���� Ŭ���� ȭ�� ��� �Լ�
void DrawGameClearScreen() {
	// ���� Ŭ���� �� ( ���� �ڶ� ������ ü���� 0 ������ �� )
	while (GetTurtleHp() <= 0) {
		// Ű �Է��� ������
		if (_kbhit()) {
			if (_getch() == ESC) {	// ESC Ű �Է½�
				// ESC Ű�� ������ ���� ����
				system("cls"); // ȭ�� �����
				exit(0); // ���α׷� ����
			}
			else {
				GameStart = false;		// ���ӽ��� ���� false ����
				SetWeaponChosen(false); // ���� ���ÿ��� false�� ����
				SetMapSetting(false);	// ������ ���� �ʱ�ȭ
				SetMapStatus(E_Jail);	// ���� ������ �̵�
				SetIsNearLadder(false); // ��ٸ� ��ó ���� false�� ����
				SetInvincibleTime(false); // �÷��̾� ���� �ð� false�� ����
				SetPlusX(0);			// X ��ǥ ������ 0���� ����
				ResetTurtleHp();		// �ڶ� ���� ü�� �ʱ�ȭ	
			}
		}
		textE = !textE; // ���� ����Ʈ ȿ�� (���, ȸ������ �����Ÿ�)
		_Invalidate();
		Sleep(500);
	}

}

// ���� ���� ���� ��������
bool GetGameStart()
{
	return GameStart;
}

// ���� ���� ���� ��������
bool GetIsGameOver()
{
	return IsGameOver;
}

// ���� ���� ���� ����
void SetIsGameOver(int src)
{
	IsGameOver = src;
}

// ���� ����Ʈ ȿ���� ���� ��������
bool GetTextE()
{
	return textE;
}

// ���� ����Ʈ ȿ����	���� ����
void SetTextE(int src)
{
	textE = src;
}

bool GetControlScreen()
{
	return ControlScreen;
}

void SetControlScreen(bool src)
{
	ControlScreen = src;
}

void DrawControlsScreen() {
	while (!ControlScreen) {
		if (_kbhit()) {
			ControlScreen = true;
			_getch();
		}
		textE = !textE;
		_Invalidate();
		Sleep(500);
	}
}