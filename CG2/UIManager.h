#pragma once
#include "WinApp.h"
#include "Sprite.h"
#include "Rhythm.h"
#include <string>

class UIManager
{
public:
	static const int fontSize = 64;
	static const int maxDegree = 360;
	const float maxSpeed = 30.0f;
	static const int maxCharCount = 256;	// �ő啶����
	static const int kBufferSize = 512;   // �����t��������W�J�p�o�b�t�@�T�C�Y
	static const int fontLineCount = 10;

	~UIManager();

	void Initialize(UINT texnumber);

	void Update(Rhythm* rhythm, int isDead);

	void Draw(Rhythm* rhythm);

	void UIPrintf(XMFLOAT2 pos,XMFLOAT2 scale, XMFLOAT4 color, int size, const char* fmt, ...);

private:
	int score = 5000;
	int countDown = 3;

	XMFLOAT4 color = {0.0f,1.0f,1.0f,1.0f};
	XMFLOAT4 flashColor = { 10.0f,10.0f,10.0f,1.0f };
	float lectPoint = 0.0f;

	XMFLOAT2 position = { 0.0f,0.0f };
	float rotation = 0.0f;

	float moveSpeed = 0.1f;

	Sprite* clearSprite;
	Sprite* numbersSprite = nullptr;

	// �X�v���C�g�f�[�^�̔z��
	Sprite* spriteDatas[maxCharCount] = {};
	// �X�v���C�g�f�[�^�z��̓Y�����ԍ�
	int spriteIndex = 0;

	// �����t��������W�J�p�o�b�t�@
	char buffer[kBufferSize];

	int j;
};

