#include "UIManager.h"

UIManager::~UIManager() {

	delete clearSprite;
	delete numbersSprite;
}

void UIManager::Initialize(UINT texnumber) {

	clearSprite = new Sprite(2, { 0.0f,0.0f }, { 1000,500 }, { 1.0f,1.0f,1.0f,1.0f }, { 0,0 }, 0, 0);
	clearSprite->Initialize();
	clearSprite->SetAnchorPoint({ 0.4f, 0.5f });
	
	moveSpeed = maxSpeed;
	position = { 0.0f,-250.0f };
	countDown = 3;

	// �S�ẴX�v���C�g�f�[�^�ɂ���
	for (int i = 0; i < _countof(spriteDatas); i++)
	{
		// �X�v���C�g�𐶐�����
		spriteDatas[i] = Sprite::Create(texnumber, { 0,0 });
	}

}

void UIManager::Update(Rhythm* rhythm, Input* input, int isDead) {

	//�ŏ��́uREADY?�v�̏���
	if (rhythm->GetSoundState().measureCount < 4) {	

		if (rhythm->GetSoundState().measureCount < 2) {

			if (moveSpeed > 0) {
				moveSpeed -= 1.0f;
				position.y += moveSpeed;

			}
		}
		else if (rhythm->GetSoundState().measureCount == 3) {
			if (moveSpeed <= maxSpeed) {
				moveSpeed += 1.0f;
				position.y -= moveSpeed;

			}
		}

		if (rhythm->GetSoundState().measureCount < 4) {
			UIPrintf({ window_width / 2 - 200, position.y }, { 1.7f,2.0f }, { 0.0f,1.0f,1.0f,1.0f }, 5, " WAVE");
			UIPrintf({ window_width / 2 + 40, position.y - 40}, { 2.5f,2.8f }, { 0.0f,1.0f,1.0f,1.0f }, 2, " %f",rhythm->GetSoundState().wave);
			UIPrintf({ window_width / 2 - 250,position.y + 100 }, { 2.2f,2.5f }, { 0.0f,1.0f,1.0f,1.0f }, 7, " READY?");
		}
	}
	else if (rhythm->GetSoundState().measureCount > 4 && isDead == false)
	{
		position = { 0.0f,-250.0f };
		moveSpeed = maxSpeed;
	}

	//�J�E���g�_�E���̏���
	if (rhythm->GetSoundState().wave <= 2) {

	}
	//�����x�����Z�b�g
	if (rhythm->GetSoundState().timer == 0) {
		color.w = 1.0f;
	}

	//�J�E���g�_�E���u3,2,1�v�̏���
	if (rhythm->GetSoundState().measureCount >= 4 && rhythm->GetSoundState().measureCount < 7) {

		UIPrintf({ window_width / 2 - 300,window_height / 2 - 150 },{ 6.0f,5.0f }, color, 2, " %d",countDown);
		if (rhythm->GetSoundState().timer == 59) {
			countDown -= 1;
		}

	}
	//�J�E���g�_�E����uSTART!�v�̏���
	else if (rhythm->GetSoundState().measureCount == 7) {
		UIPrintf({ window_width / 2 - 500,window_height / 2 - 100 }, { 4.0f,3.5f }, color, 7, " START!");
	}
	//1wave���I������烊�Z�b�g
	else if (rhythm->GetSoundState().measureCount >= 79) {

		countDown = 3;
	}	

	//�N���A���̏���
	if ((rhythm->GetSoundState().measureCount >= 72 && rhythm->GetSoundState().wave == 1) ||
		(rhythm->GetSoundState().measureCount >= 80 && rhythm->GetSoundState().wave == 2)) {
		if (rotation < maxDegree) {
			rotation += 6.0f;
			clearSprite->SetSize({ 800 + maxDegree - rotation,150 + maxDegree - rotation });
			clearSprite->SetColor({ 1.0f,1.0f,1.0f,rotation / maxDegree });
		}
		else {
			if (flashColor.x > 1.0f) {

				flashColor.x -= 0.1f;
				flashColor.y -= 0.1f;
				flashColor.z -= 0.1f;

			}
			clearSprite->SetColor(flashColor);
		}

		clearSprite->SetTextureRect({ 0,80 }, { 512,80 });
		clearSprite->SetPosition({ window_width / 2,window_height / 2 });

		clearSprite->SetRotation(rotation);

	}
	else {
		rotation = 0.0f;
		flashColor = { 10.0f,10.0f,10.0f,1.0f };
		color.w -= 0.02f;
		clearSprite->SetColor(color);
	}

	//�Q�[���I�[�o�[���̏���
	if (isDead == true) {
		if (moveSpeed > 0) {
			moveSpeed -= 1.0f;
			position.y += moveSpeed;
		}
		UIPrintf({ window_width / 2 - 600, position.y }, { 3.5f,4.0f }, { 0.0f,1.0f,1.0f,1.0f }, 9, " GAMEOVER");
	}
	
	//�|�[�Y��ʂ̏���
	if (rhythm->GetSoundState().isPause == true) {
		UIPrintf({ window_width / 2 - 100, window_height / 2 - 50}, { 1.0f,1.0f }, { 0.0f,0.5f,1.0f,1.0f }, 6, " PAUSE");
	}

	//
	if (input->TriggerKey(DIK_SPACE)) {
		if (rhythm->GetSoundState().isFireActive) {
			judgeAlpha = 1.0f;
		}
	}

	if (rhythm->GetSoundState().judge == Judge::Good) {
		UIPrintf({ window_width / 2 - 200, 100 }, { 2.0f,2.0f }, { 1.0f,1.0f,0.0f,judgeAlpha }, 5, " Good");
	}
	else if (rhythm->GetSoundState().judge == Judge::Miss) {
		UIPrintf({ window_width / 2 - 200, 100 }, { 2.0f,2.0f }, { 0.3f,0.3f,0.3f,judgeAlpha }, 5, " Miss");
	}
	judgeAlpha -= 0.05f;
}

void UIManager::Draw(Rhythm* rhythm) {

	if (rhythm->GetSoundState().measureCount >= 72 && rhythm->GetSoundState().wave == 1 ||
		rhythm->GetSoundState().measureCount >= 80 && rhythm->GetSoundState().wave == 2) {
		clearSprite->Draw();
	}

	// �S�Ă̕����̃X�v���C�g�ɂ���
	for (int i = 0; i < spriteIndex; i++)
	{
		// �X�v���C�g�`��
		spriteDatas[i]->Draw();
	}

	spriteIndex = 0;
}

void UIManager::UIPrintf(XMFLOAT2 pos, XMFLOAT2 scale, XMFLOAT4 color, int size, const char* fmt, ...) {
	// �S�Ă̕����ɂ���
	for (int i = 0; i < size; i++)
	{
		// �ő啶��������
		if (spriteIndex >= maxCharCount) {
			break;
		}
		// 1�������o��(��ASCII�R�[�h�ł������藧���Ȃ�)
		const unsigned char& character = buffer[i];

		va_list arg_ptr;

		va_start(arg_ptr, fmt);
		j = vsnprintf(buffer, 64, fmt, arg_ptr);
		va_end(arg_ptr);
		buffer[i] = j;

		int fontIndex = character - 32;
		if (character >= 0x7f) {
			fontIndex = 0;
		}

		int fontIndexY = fontIndex / fontLineCount;
		int fontIndexX = fontIndex % fontLineCount;

		// ���W�v�Z
		spriteDatas[spriteIndex]->SetPosition({ pos.x  + scale.x * (i * (fontSize - 30)), pos.y });
		spriteDatas[spriteIndex]->SetTextureRect({ (float)fontIndexX * (float)fontSize, (float)fontIndexY * (float)fontSize }, { (float)fontSize, (float)fontSize });
		spriteDatas[spriteIndex]->SetSize({ (fontSize - 30) * scale.x, fontSize * scale.y });
		spriteDatas[spriteIndex]->SetColor(color);

		// �������P�i�߂�
		spriteIndex++;

	}
}