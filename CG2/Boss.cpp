#include "Boss.h"

void Boss::Initialize(ViewProjection* viewProjection, XMMATRIX* matProjection)
{
	gameObject = new GameObject3D();
	gameObject->PreLoadTexture(L"Resources/red.png");
	gameObject->SetViewProjection(viewProjection);
	gameObject->SetMatProjection(matProjection);
	gameObject->Initialize();


	gameObject->worldTransform.translation = { 0 , 0 , 600 };
	gameObject->worldTransform.scale = { 10 , 15 , 10 };
}

void Boss::Update()
{
	phaseTimer--;
	switch (phase)
	{
	case BossPhase::spown:	// �a��
		if (phaseTimer <= 0.0f) {
			phase = BossPhase::attack;
			isDead = false;
			phaseTimer = 400.0f;
		}
		break;
	case BossPhase::attack:	//�U��1 ���C��(�})�𒣂肻�����]������
		// ���U������
		Attack();
		//-----------------------
		if (phaseTimer <= 0.0f) {
			phase = BossPhase::attack2;
			phaseTimer = 400.0f;
		}
		break;
	case BossPhase::attack2:	//�U��2
		// ���U������2�@�ړ��ꏊ���i�点�� & �����_���V���b�g
		Attack2();
		//-----------------------
		if (phaseTimer <= 0.0f) {
			phase = BossPhase::defence;
			phaseTimer = 400.0f;
		}
		break;
	case BossPhase::defence:	//���,�h��
		// ���h�䏈��	�_���[�W�y�����ʂ��o��
		Defence();
		//-----------------------
		if (phaseTimer <= 0.0f) {
			phase = BossPhase::attack;
			phaseTimer = 200.0f;
		}
		break;
	}

	gameObject->Update();
}

void Boss::Draw()
{
	if (isDead == false) {
		gameObject->Draw();
	}
}

void Boss::Attack()	//���C����ɒe��W�J��]������
{

}

void Boss::Attack2()	//�ړ��ꏊ�𐧌�����&�����_���V���b�g
{

}

void Boss::Defence()	//�_���[�W���y��������
{

}
