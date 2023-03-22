#pragma once
#include "GameObject3D.h"
#include "MathFunc.h"

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

	void Attack();
	void Attack2();
	void Defence();
private:
	GameObject3D* gameObject = nullptr;

	Vector3 moveSpeed = { 0,0,0 };

	//�s���ω�
	float phaseTimer = 300.0f;
	BossPhase phase = BossPhase::spown;

	bool isDead = true;
	int HP = 10;
};

