#include "Player.h"

class Enemy {
public:
	WorldTransform GetWorldTransform();
};

Player::Player() {

}

Player::~Player() {
	delete gameObject;
}

void Player::Initialize(ViewProjection* viewProjection, XMMATRIX* matProjection) {

	gameObject = new GameObject3D();
	gameObject->PreLoadModel("Resources/tofu/tofu.obj");
	gameObject->PreLoadTexture(L"Resources/star/star.jpg");
	gameObject->SetViewProjection(viewProjection);
	gameObject->SetMatProjection(matProjection);
	gameObject->Initialize();

	Reset();

}

void Player::Update() {

	Move();

	//�f�X�t���O�̗������e���폜
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
		return bullet->IsDead();
		});

	//�e�X�V
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) { bullet->Update({ 0,0,0 },GetWorldTransform().translation); }

	if (isDead == false) 
	{
		gameObject->Update();
	}

}

void Player::Draw() {
	if (isDead == false) {
		gameObject->Draw();
		//�e�`��
		for (std::unique_ptr<PlayerBullet>& bullet : bullets_) { bullet->Draw(); }
	}
}

void Player::Reset() {
	gameObject->worldTransform.translation = { -40 , 0 , -40 };
	moveSpeed = 0;
	isDead = false;
}

void Player::Rotate() {

	const float rotationSpeed = MathFunc::Utility::Deg2Rad(6.0f);

	Vector3 rotation = { 0 , 0 , 0 };

	rotation.y = rotationSpeed;

	gameObject->worldTransform.rotation += rotation;

}

void Player::Move() {

	moveSpeed = 0.5f;
	move = { 0,0,0 };


	if (input.PushKey(DIK_W) || input.PushKey(DIK_S) || input.PushKey(DIK_D) || input.PushKey(DIK_A))
	{

		// �ړ���̍��W���v�Z
		if (input.PushKey(DIK_W)) { move = { 0,moveSpeed,0 }; }
		else if (input.PushKey(DIK_S)) { move = { 0,-moveSpeed,0 }; }
		if (input.PushKey(DIK_D)) { move = { moveSpeed,0,0 }; }
		else if (input.PushKey(DIK_A)) { move = { -moveSpeed,0,0 }; }

		if (input.PushKey(DIK_D) && input.PushKey(DIK_W)) { move = { moveSpeed,moveSpeed,0 }; }
		else if (input.PushKey(DIK_D) && input.PushKey(DIK_S)) { move = { moveSpeed,-moveSpeed,0 }; }

		if (input.PushKey(DIK_A) && input.PushKey(DIK_W)) { move = { -moveSpeed,moveSpeed,0 }; }
		if (input.PushKey(DIK_A) && input.PushKey(DIK_S)) { move = { -moveSpeed,-moveSpeed,0 }; }
	}

	gameObject->worldTransform.translation += move;
}
void Player::NewBullet(ViewProjection* viewProjection, XMMATRIX* matProjection) {
	if (input.PushKey(DIK_SPACE))
	{
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);
	
		//�e�𐶐����A������
		std::unique_ptr<PlayerBullet>newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(viewProjection, matProjection);
		//�e��o�^����
		bullets_.push_back(std::move(newBullet));
		
	}
}
void Player::Collision() {
	////enemy
	//if (enemy->GetWorldTransform().translation.x - gameObject->worldTransform.translation.x < 40 &&
	//	-40 < enemy->GetWorldTransform().translation.x - gameObject->worldTransform.translation.x) {
	//	if (enemy->GetWorldTransform().translation.y - gameObject->worldTransform.translation.y < 3 &&
	//		-3 < enemy->GetWorldTransform().translation.y - gameObject->worldTransform.translation.y) {
	//		if (enemy->GetWorldTransform().translation.z - gameObject->worldTransform.translation.z < 3 &&
	//			-3 < enemy->GetWorldTransform().translation.z - gameObject->worldTransform.translation.z) {

	//			//isEnemy = true;
	//			isDead = true;
	//		}
	//	}
	//}
}

//�A�N�Z�b�T
void Player::SetMap(Map* map) {
	this->map = map;
}

void Player::SetGoal(Goal* goal) {
	this->goal = goal;
}

int Player::GetIsGoal() {
	return isGoal;
}

void Player::SetIsGoal(int flag) {
	isGoal = flag;
}

void Player::SetEnemy(Enemy* enemy) {
	this->enemy = enemy;
}

int Player::GetIsEnemy() {
	return isGoal;
}

void Player::SetIsEnemy(int flag) {
	isEnemy = flag;
}

int Player::GetIsDead() {
	return isDead;
}

Vector3 Player::GetAngle() {
	return angle;
}

WorldTransform Player::GetWorldTransform() {
	return gameObject->worldTransform;
}