#include "Enemy.h"

Enemy::Enemy() {

}

Enemy::~Enemy() {
	delete gameObject;
}

void Enemy::Initialize(ViewProjection* viewProjection, XMMATRIX* matProjection) {

	gameObject = new GameObject3D();
	gameObject->PreLoadTexture(L"Resources/white1x1.png");
	gameObject->SetViewProjection(viewProjection);
	gameObject->SetMatProjection(matProjection);
	gameObject->Initialize();

	gameObject->worldTransform.translation = { 0 , 0 , 100 };
	gameObject->worldTransform.scale = { 2 , 2 , 2 };

}

void Enemy::Update(Vector3 playerPos, Vector3 bossPos) {


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

	gameObject->Update();

}

void Enemy::Draw() {
	gameObject->Draw();
}

void Enemy::Reset() {
	gameObject->worldTransform.translation = { 0 , 0 , 100 };
}

WorldTransform Enemy::GetWorldTransform() {
	return gameObject->worldTransform;
}