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

void PlayerBullet::OnCollision() {
	isDead_ = true;
}

void PlayerBullet::Initialize(ViewProjection* viewProjection, XMMATRIX* matProjection, Vector3 playerPos, Vector3 bossPos) 
{
	gameObject = new GameObject3D();
	//gameObject->PreLoadModel("Resources/star/star.obj");
	gameObject->PreLoadTexture(L"Resources/red.png");
	gameObject->SetViewProjection(viewProjection);
	gameObject->SetMatProjection(matProjection);
	gameObject->Initialize();
	gameObject->worldTransform.scale = Vector3{ 2,2,2 };
	gameObject->Update();
	isDead_ = false;
	newPlayerPos = playerPos;
	newEnemyPos = bossPos;
	gameObject->worldTransform.translation = bossPos;
}

void PlayerBullet::Update() {
	const float rotationSpeed = MathFunc::Utility::Deg2Rad(0.1f);

	Vector3 rotation = { 0 , 0 , 0 };
	//newPlayerPos = playerPos;*/
	rotation.y = rotationSpeed;
	rotation.x = rotationSpeed;
	rotation.z = rotationSpeed;
	angle = gameObject->worldTransform.rotation;
	gameObject->worldTransform.rotation += rotation;

	Attack(newPlayerPos, newEnemyPos);
	if (isShot) 
	{
		gameObject->Update();
	}
	
}
void PlayerBullet::Attack(Vector3 playerPos, Vector3 bossPos) {
	
	if (!isShot) 
	{
		//���̔ԍ��̒e�̔��˃t���O��false�Ȃ�true�ɂ���
		isShot = true;
		isDead_ = false;
		//����Ɠ����ɓ��ԍ��̃I�u�W�F�N�g�̍��W���{�X�̍��W�Ɏ����Ă���
		newPlayerPos = playerPos;
		newEnemyPos = bossPos;
		//gameObject->worldTransform.translation = bossPos;
		//�{�X�Ǝ��@�̍����x�N�g�������߂�
		//velocity = newPlayerPos - newEnemyPos;
		velocity = { 0.0f,0.0f,0.0f };

		//�x�N�g���̐��K��
		velocity.nomalize();
		//�x�N�g���̒����𑬂��ɍ��킹��
		velocity.x *= speed;
		velocity.y *= speed;
		velocity.z *= speed;
	}

	if (isShot) 
	{
		//�{�X�Ǝ��@�̍����x�N�g�������߂�
		//velocity = newPlayerPos - newEnemyPos;
		velocity = { 0.0f,0.0f,5.0f };
		
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
			gameObject->worldTransform.translation.z < -canMoveArea - 00 ||
			gameObject->worldTransform.translation.z > canMoveArea + playerPos.z + 400) 
		{
			//���͈̔͊O�ŏ���
			isShot = false;
			isDead_ = true;

		}
	}
}

void PlayerBullet::Draw() 
{
	if (isShot) 
	{
		gameObject->Draw();
	}
}

void PlayerBullet::AttackPress()
{
	isShot = true;
}

int PlayerBullet::GetIsDead() {
	return isDead_;
}

WorldTransform PlayerBullet::GetWorldTransform() {
	return gameObject->worldTransform;
}

Vector3 PlayerBullet::GetAngle() {
	return angle;
}