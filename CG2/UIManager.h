#pragma once
#include "WinApp.h"
#include "Sprite.h"
#include "Rhythm.h"
#include <string>

class UIManager
{
public:
	static const int fontSize = 80;
	static const int maxDegree = 360;
	const float maxSpeed = 30.0f;
	static const int maxCharCount = 256;	// �ő啶����
	static const int kBufferSize = 512;   // �����t��������W�J�p�o�b�t�@�T�C�Y

	~UIManager();

	void Initialize();

	void Update(Rhythm* rhythm);

	void Draw(Rhythm* rhythm);

	void InitializeScore(UINT texnumber);

	void ScorePrint(float x, float y, float scale, int size, const char* fmt, ...);

private:

	XMFLOAT4 color = {1.0f,1.0f,1.0f,1.0f};
	XMFLOAT4 flashColor = { 10.0f,10.0f,10.0f,1.0f };
	float lectPoint = 0.0f;

	XMFLOAT2 position = { 0.0f,0.0f };
	float rotation = 0.0f;

	float moveSpeed = 0.1f;

	Sprite* textSprite[3];
	Sprite* numbersSprite = nullptr;

	// �X�v���C�g�f�[�^�̔z��
	Sprite* spriteDatas[maxCharCount] = {};
	// �X�v���C�g�f�[�^�z��̓Y�����ԍ�
	int spriteIndex = 0;

	// �����t��������W�J�p�o�b�t�@
	char buffer[kBufferSize];

	int j;
};

