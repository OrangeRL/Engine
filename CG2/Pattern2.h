#pragma once
#include "GameObject3D.h"
#include "MathFunc.h"
#include "Input.h"

class Pattern2 {
public:
	//�R���X�g���N�^
	Pattern2();
	//�f�X�g���N�^
	~Pattern2();

	//������
	void Initialize(ViewProjection* viewProjection, XMMATRIX* matProjection);
	//�X�V
	void Update(Vector3 enemyPos, Vector3 playerPos);
	//�`��
	void Draw();

	bool IsDead() const { return isDead_; }

	int GetIsDead();
	Vector3 GetAngle();
	WorldTransform GetWorldTransform();

	void OnCollision();

	//�U��
	void AttackPress();

	void Attack(Vector3 playerPos, Vector3 bossPos);
private:	//�����o�֐�

private:	//�����o�ϐ�
	//�e��
	const int bulletNum = 10;
	//�Q�[���I�u�W�F�N�g
	GameObject3D* gameObject;
	//�\���t���O
	bool isShot;
	//�J�E���g
	int bulletCount = 0;
	//�e�̈ړ����x
	const float speed = -10.0f;
	//�e�̃x�N�g��
	Vector3 velocity;
	//�e�̈ړ��͈�
	float canMoveArea = 120;

	//����
	static const int32_t kLifeTime = 60 * 5;
	//�f�X�^�C�}�[
	int32_t deathTimer_ = kLifeTime;
	//�f�X�t���O
	int isDead_ = false;
	Vector3 angle = {};

	Vector3 newEnemyPos = {};
	Vector3 newPlayerPos = {};
	
	//�L�[����
	Input& input = Input::GetInstance();
};