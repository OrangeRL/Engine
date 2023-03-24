#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "MathFunc.h"
#include "Input.h"

class ReilCamera
{
public:
	void Initialize(Vector3 pos, Vector3 rot);

	void Update(Input* input);

	ViewProjection GetViewProjection() { return viewProjection_; }

	WorldTransform GetWorldTransform() { return worldTransform_; }

	Vector3 GetRotation() { return rotation; }

private:
	const float PI = 3.14159f;
	Input* input = nullptr;

	//���[���h�s��
	WorldTransform worldTransform_;

	//�r���[�v���W�F�N�V����
	ViewProjection viewProjection_;

	//��]
	Vector3 rotation = { 0,0.5f,0 };
	//��]�̑��x
	Vector3 rotSpeeed = { 0.005f,0.005f,0.005f };
	
	//�C�[�W���O
	float easeInOutSine(float number) {
		return -(cos(PI * number) - 1) / 2;
		//return sin((number * PI) / 2);
	}
	const float MAX_FLAME = 50.0f;
	float flameLeft = 0;
	float flameRight = 0;
	float flameUp = 0;
	float flameDown = 0;

	float a = 0;
};

