#include "PlayerBullet.h"

PlayerBullet::PlayerBullet() {
	//�������ȑO�̐ݒ�

	gameObject = nullptr;
	isShot = false;
	isDead_ = false;

}

PlayerBullet::~PlayerBullet() {
	delete gameObject;
}

void PlayerBullet::Initialize(ViewProjection* viewProjection, XMMATRIX* matProjection) {

	gameObject = new GameObject3D();
	gameObject->PreLoadModel("Resources/tofu/tofu.obj");
	gameObject->SetViewProjection(viewProjection);
	gameObject->SetMatProjection(matProjection);
	gameObject->Initialize();
	gameObject->worldTransform.scale = Vector3{ 1,1,1 };
	gameObject->Update();

}

void PlayerBullet::Update(Vector3 playerPos, Vector3 bossPos) {
	Attack(playerPos, bossPos);

	if (isShot) {
		gameObject->Update();
	}

}

void PlayerBullet::Draw() {

	if (isShot) {
		gameObject->Draw();

	}
}

void PlayerBullet::Attack(Vector3 playerPos, Vector3 bossPos) {

	if (!isShot) {
		//���̔ԍ��̒e�̔��˃t���O��false�Ȃ�true�ɂ���
		isShot = true;
		isDead_ = false;
		//����Ɠ����ɓ��ԍ��̃I�u�W�F�N�g�̍��W���{�X�̍��W�Ɏ����Ă���
		gameObject->worldTransform.translation = bossPos;
		//�{�X�Ǝ��@�̍����x�N�g�������߂�
		velocity = playerPos - bossPos;
		//�x�N�g���̐��K��
		velocity.nomalize();
		//�x�N�g���̒����𑬂��ɍ��킹��
		velocity.x *= speed;
		velocity.y *= speed;
		velocity.z *= speed;
	}

	if (isShot) {
		//�{�X�Ǝ��@�̍����x�N�g�������߂�
		velocity = playerPos - bossPos;
		//�x�N�g���̐��K��
		velocity.nomalize();
		//�x�N�g���̒����𑬂��ɍ��킹��
		velocity.x *= speed;
		velocity.y *= speed;
		velocity.z *= speed;
		//���˃t���O��true�Ȃ炻�̎��_�ł̎��@�̍��W�Ɍ������Ĉړ�����
		gameObject->worldTransform.translation -= velocity;
		if (gameObject->worldTransform.translation.x < -canMoveArea ||
			gameObject->worldTransform.translation.x > canMoveArea ||
			gameObject->worldTransform.translation.y < -canMoveArea ||
			gameObject->worldTransform.translation.y > canMoveArea ||
			gameObject->worldTransform.translation.z < -canMoveArea ||
			gameObject->worldTransform.translation.z > canMoveArea) {
			//���͈̔͊O�ŏ���
			isShot = false;
			isDead_ = true;

		}
	}
	if (bulletCount < 10) {
		bulletCount++;
	}
}