#pragma once
#include "GameObject3D.h"
#include "EnemyBullet.h"
#include <memory>
#include <list>

class Enemy {
public:

	//�R���X�g���N�^�\
	Enemy();
	//�f�X�g���N�^
	~Enemy();

	void Initialize(ViewProjection* viewProjection, XMMATRIX* matProjection, const wchar_t* textureFileName);


	void Update(ViewProjection* viewProjection, XMMATRIX* matProjection, const wchar_t* textureFileName, int bulletNum);


	void Draw();
	
	void Reset();
	//���W�擾�p
	WorldTransform GetWorldTransform();
	//���������ꏊ��ݒ�
	WorldTransform Settransform(float x,float y,float z);
	float GetAttackSpeed();
	float SetAttackSpeed(float speed);
	//���x�ݒ�
	float SetSpeed(float speed);

	bool GetIsAttack();
	bool SetIsAttack(bool isAttack);
private:
	GameObject3D* gameObject = nullptr;


	float moveSpeed = 0;
	float attackSpeed = 100.0f;
	bool isAttack = false;
};


