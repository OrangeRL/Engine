#pragma once
#include "GameObject3D.h"
#include "EnemyBullet.h"

class Enemy {
public:

	//�R���X�g���N�^
	Enemy();
	//�f�X�g���N�^
	~Enemy();

	void Initialize(ViewProjection* viewProjection, XMMATRIX* matProjection, const wchar_t* textureFileName);

	void Update(ViewProjection* viewProjection, XMMATRIX* matProjection, const wchar_t* textureFileName);

	void Draw();

	WorldTransform GetWorldTransform();
	WorldTransform Settransform(float x,float y,float z);
	float SetSpeed(float speed);
private:
	GameObject3D* gameObject = nullptr; // ���W��傫�����������Ă���

	float moveSpeed = 0; //�ړ����x
	float attackSpeed = 100.0f;
	bool isAttack = false;
};


