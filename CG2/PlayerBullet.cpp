#include "PlayerBullet.h"

PlayerBullet::PlayerBullet() {
	//�������ȑO�̐ݒ�
	for (int i = 0; i < bulletNum; i++) {
		gameObject[i] = nullptr;
		isShot[i] = false;
	}
}

PlayerBullet::~PlayerBullet() {
	delete gameObject;
}

void PlayerBullet::Initialize(ViewProjection* viewProjection, XMMATRIX* matProjection) {
	for (int i = 0; i < bulletNum; i++) {
		gameObject[i] = new GameObject3D();
		gameObject[i]->PreLoadModel("Resources/tofu/tofu.obj");
		gameObject[i]->SetViewProjection(viewProjection);
		gameObject[i]->SetMatProjection(matProjection);
		gameObject[i]->Initialize();
		gameObject[i]->worldTransform.scale = Vector3{ 1,1,1 };
		gameObject[i]->Update();
	}
}

void PlayerBullet::Update(Vector3 playerPos, Vector3 bossPos) {
	Attack(playerPos, bossPos);
	for (int i = 0; i < bulletNum; i++) {
		if (isShot[i]) {
			gameObject[i]->Update();
		}
	}
}

void PlayerBullet::Draw() {
	for (int i = 0; i < bulletNum; i++) {
		if (isShot[i]) {
			gameObject[i]->Draw();
		}
	}
}

void PlayerBullet::Attack(Vector3 playerPos, Vector3 bossPos) {
	if (bulletCount > bulletNum) {
		//�J�E���g�������̍ő�l���I�[�o�[���Ă����烊�Z�b�g
		bulletCount = 0;
	}
	if (!isShot[bulletCount]) {
		//���̔ԍ��̒e�̔��˃t���O��false�Ȃ�true�ɂ���
		isShot[bulletCount] = true;
		//����Ɠ����ɓ��ԍ��̃I�u�W�F�N�g�̍��W���{�X�̍��W�Ɏ����Ă���
		gameObject[bulletCount]->worldTransform.translation = bossPos;
		//�{�X�Ǝ��@�̍����x�N�g�������߂�
		velocity[bulletCount] = playerPos - bossPos;
		//�x�N�g���̐��K��
		velocity[bulletCount].nomalize();
		//�x�N�g���̒����𑬂��ɍ��킹��
		velocity[bulletCount].x *= speed;
		velocity[bulletCount].y *= speed;
		velocity[bulletCount].z *= speed;
	}
	for (int i = 0; i < bulletNum; i++) {
		if (isShot[i]) {
			//�{�X�Ǝ��@�̍����x�N�g�������߂�
			velocity[i] = playerPos - bossPos;
			//�x�N�g���̐��K��
			velocity[i].nomalize();
			//�x�N�g���̒����𑬂��ɍ��킹��
			velocity[i].x *= speed;
			velocity[i].y *= speed;
			velocity[i].z *= speed;
			//���˃t���O��true�Ȃ炻�̎��_�ł̎��@�̍��W�Ɍ������Ĉړ�����
			gameObject[i]->worldTransform.translation -= velocity[i];
			if (gameObject[i]->worldTransform.translation.x < -canMoveArea ||
				gameObject[i]->worldTransform.translation.x > canMoveArea ||
				gameObject[i]->worldTransform.translation.y < -canMoveArea ||
				gameObject[i]->worldTransform.translation.y > canMoveArea ||
				gameObject[i]->worldTransform.translation.z < -canMoveArea ||
				gameObject[i]->worldTransform.translation.z > canMoveArea) {
				//���͈̔͊O�ŏ���
				isShot[i] = false;
			}
		}
	}
	bulletCount++;
}