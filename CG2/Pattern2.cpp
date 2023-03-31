#include "Pattern2.h"

Pattern2::Pattern2() {
	//�������ȑO�̐ݒ�

	gameObject = nullptr;
	isShot = false;
	isDead_ = false;
}

Pattern2::~Pattern2() {
	delete gameObject;
}

void Pattern2::OnCollision() {
	isDead_ = true;
}

void Pattern2::Initialize(ViewProjection* viewProjection, XMMATRIX* matProjection) 
{
	gameObject = new GameObject3D();
	//gameObject->PreLoadModel("Resources/star/star.obj");
	gameObject->PreLoadTexture(L"Resources/red.png");
	gameObject->SetViewProjection(viewProjection);
	gameObject->SetMatProjection(matProjection);
	gameObject->Initialize();
	gameObject->worldTransform.scale = Vector3{ 0.3,0.3,1 };
	gameObject->Update();
	isDead_ = false;

}

void Pattern2::Update(Vector3 enemyPos, Vector3 playerPos) {

	Attack(enemyPos, playerPos);

	if (isShot) 
	{
		gameObject->Update();
	}
	
}
void Pattern2::Attack(Vector3 playerPos, Vector3 bossPos) {
	if (!isShot) 
	{
		//���̔ԍ��̒e�̔��˃t���O��false�Ȃ�true�ɂ���
		isShot = true;
		isDead_ = false;
		//����Ɠ����ɓ��ԍ��̃I�u�W�F�N�g�̍��W���{�X�̍��W�Ɏ����Ă���
		gameObject->worldTransform.translation = bossPos;
		//�{�X�Ǝ��@�̍����x�N�g�������߂�
		velocity = { 0.0f,0.0f,0.0f };
	}

	if (isShot)
	{
		//�{�X�Ǝ��@�̍����x�N�g�������߂�
		velocity = { 0.0f,0.0f,1.0f };

		//���˃t���O��true�Ȃ炻�̎��_�ł̎��@�̍��W�Ɍ������Ĉړ�����
		gameObject->worldTransform.translation.z += velocity.z;
		gameObject->worldTransform.translation.x = bossPos.x;
		gameObject->worldTransform.translation.y = bossPos.y;

		if (gameObject->worldTransform.translation.x < -canMoveArea ||
			gameObject->worldTransform.translation.x > canMoveArea ||
			gameObject->worldTransform.translation.y < -canMoveArea ||
			gameObject->worldTransform.translation.y > canMoveArea ||
			gameObject->worldTransform.translation.z < -canMoveArea ||
			gameObject->worldTransform.translation.z > playerPos.z + playerPos.z + 500)
		{
			//���͈̔͊O�ŏ���
			isShot = false;
			isDead_ = true;

		}
	}
}

void Pattern2::Draw() 
{
	if (isShot) 
	{
		gameObject->Draw();
	}
}

void Pattern2::AttackPress()
{
	isShot = true;
}

int Pattern2::GetIsDead() {
	return isDead_;
}

WorldTransform Pattern2::GetWorldTransform() {
	return gameObject->worldTransform;
}

Vector3 Pattern2::GetAngle() {
	return angle;
}