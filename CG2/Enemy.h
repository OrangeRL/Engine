#pragma once
#include "GameObject3D.h"

class Enemy {
public:

	//�R���X�g���N�^
	Enemy();
	//�f�X�g���N�^
	~Enemy();

	void Initialize(ViewProjection* viewProjection, XMMATRIX* matProjection, const wchar_t* textureFileName);

	void Update();

	void Draw();

	WorldTransform GetWorldTransform();
	WorldTransform Settransform(float x,float y,float z);
	Vector3 SetSpeed(float x, float y, float z);
private:
	GameObject3D* gameObject = nullptr; // ���W��傫�����������Ă���

	Vector3 moveSpeed = {0,0,0}; //�ړ����x

};


