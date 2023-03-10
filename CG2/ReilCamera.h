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

private:
	Input* input = nullptr;

	//���[���h�s��
	WorldTransform worldTransform_;

	//�r���[�v���W�F�N�V����
	ViewProjection viewProjection_;

	//��]
	Vector3 rotation = { 0,0.5f,0 };
	//��]�̑��x
	Vector3 rotSpeeed = { 0.005f,0.005f,0.005f };
};
