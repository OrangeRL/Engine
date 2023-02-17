#pragma once
#include "GameObject3D.h"
#include "MathFunc.h"

class PlayerBullet {
public:
	//�R���X�g���N�^
	PlayerBullet();
	//�f�X�g���N�^
	~PlayerBullet();

	//������
	void Initialize(ViewProjection* viewProjection, XMMATRIX* matProjection);
	//�X�V
	void Update(Vector3 playerPos, Vector3 bossPos);
	//�`��
	void Draw();
private:	//�����o�֐�
	//�U��
	void Attack(Vector3 playerPos, Vector3 bossPos);
private:	//�����o�ϐ�
	//�e��
	const int bulletNum = 300;
	//�Q�[���I�u�W�F�N�g
	GameObject3D* gameObject[300];
	//�\���t���O
	bool isShot[300];
	//�J�E���g
	int bulletCount = 0;
	//�e�̈ړ����x
	const float speed = -1.0f;
	//�e�̃x�N�g��
	Vector3 velocity[300];
	//�e�̈ړ��͈�
	float canMoveArea = 120;
};