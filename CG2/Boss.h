#pragma once
#include "GameObject3D.h"
#include "MathFunc.h"
#include <memory>
#include <list>
#include <random>

enum BossPhase {
	spown,
	attack,
	attack2,
	defence,
};

class Boss
{
public:
	void Initialize(ViewProjection* viewProjection, XMMATRIX* matProjection);
	void Update();
	void Draw();
	//���s���w���p
	void Attack();
	void Attack2();
	void Defence();

	float Random(float minValue, float maxValue);

	//���W�擾�p
	WorldTransform GetWorldTransform();
	BossPhase GetPhase();
	
	float GetAttackSpeed();
	float SetAttackSpeed(float speed);
	bool GetIsAttack();
	bool SetIsAttack(bool isAttack);
	bool GetIsDead();
private:
	GameObject3D* gameObject = nullptr;

	Vector3 moveSpeed = { 0,0,0 };

	//�s���ω�
	float phaseTimer = 3000.0f;
	BossPhase phase = BossPhase::spown;

	bool isDead = true;
	int HP = 10;

	float attackSpeed = 200.0f;
	bool isAttack = false;
};

