#include "Player.h"

class Map {
public:
	int GetObjectCount();
	int GetObjectID(int x, int y);
	WorldTransform GetWorldTransform(int ObjectNum);
};

class Goal {
public:
	WorldTransform GetWorldTransform();
};

class Enemy {
public:
	WorldTransform GetWorldTransform();
	void Reset();
};

class GameScene {
public:
	const std::list<std::unique_ptr<Enemy>>& GetEnemies();
	void Reset();
};

Player::Player() {

}

Player::~Player() {
	delete gameObject;
}

void Player::Initialize(ViewProjection* viewProjection, XMMATRIX* matProjection) {
	viewProjection_ = viewProjection;

	gameObject = new GameObject3D();
	gameObject->PreLoadModel("Resources/tofu/tofu.obj");
	gameObject->PreLoadTexture(L"Resources/star/star.jpg");
	gameObject->SetViewProjection(viewProjection_);
	gameObject->SetMatProjection(matProjection);
	gameObject->Initialize();

	aimObject = new GameObject3D();
	//aimObject->PreLoadModel("Resources/tofu/tofu.obj");
	aimObject->PreLoadTexture(L"Resources/crosshair.png");
	aimObject->SetViewProjection(viewProjection_);
	aimObject->SetMatProjection(matProjection);
	aimObject->Initialize();

	Reset();

	//3Dレティクルのワールドトランスフォーム初期化
	worldTransform3DReticle_.initialize();
}

void Player::Update(WorldTransform wt) {
	playerPos = GetWorldTransform().translation;

	Move();
	//Aim();
	//enemyPos = enemy->GetWorldTransform().translation;
	//デスフラグの立った弾を削除
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
		return bullet->IsDead();
		});

	//弾更新

	//for (std::unique_ptr<PlayerBullet>& bullet : bullets_) { bullet->Update(); }

	if (isDead == false)
	{
		aimObject->worldTransform.parent = &wt;
		gameObject->worldTransform.parent = &wt;
		gameObject->Update();
		aimObject->Update();
	}

	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Update();
	}
	Collision();

	//SetPos({ 0.0f,0.0f,5.0f });
}

void Player::AimHit() {
	isAimHit = true;
}

void Player::Aim(Vector3 player, Vector3 enemy) {

	//自機から3Dレティクルへのオフセット(Z+向き)
	Vector3 offset = { 0,0,1.0f };

	float speed = -0.2f;
	posA = player;
	posB = enemy;
	posC = posA - posB;
	//posC.nomalize();
	//posC *= speed;
	 // Calculate the distance between the player and enemy vectors
	distance++;
	//自機から3Dレティクルへの距離
	float kDistancePlayerTo3DReticle = distance;

	aimObject->worldTransform.scale = { 1.0f, 1.0f, distance };

	aimObject->worldTransform.translation = gameObject->worldTransform.translation + offset * kDistancePlayerTo3DReticle;

	if (isAimHit == true) {
		distance = 0;
	}

}
//
//void Player::Aim(Vector3 player, Vector3 enemy) {
//	//自機から3Dレティクルへの距離
//	const float kDistancePlayerTo3DReticle = 50.0f;
//	//自機から3Dレティクルへのオフセット(Z+向き)
//	Vector3 offset = { 0,0,1.0f };
//	//自機のワールド行列の回転を反映
//	//offset = MathFunc::Utility::MulVector3AndMatrix4(offset, gameObject->worldTransform.matWorld);
//	////ベクトルの長さを整える
//	//offset.nomalize();
//	//offset * kDistancePlayerTo3DReticle;
//	//3Dレティクルの座標を設定
//	
//	float speed = -0.2f;
//	posA = player;
//	posB = enemy;
//	posC = posA - posB;
//	//posC.nomalize();
//	//posC *= speed;
//
//	aimObject->worldTransform.scale = { 0.1, 0.1, 50 };
//
//	aimObject->worldTransform.translation = gameObject->worldTransform.translation + offset*kDistancePlayerTo3DReticle;
//}

void Player::Draw() {
	if (isDead == false) {
		gameObject->Draw();
		aimObject->Draw();
		//弾描画
		for (std::unique_ptr<PlayerBullet>& bullet : bullets_) { bullet->Draw(); }
	}
}

void Player::Reset() {
	//gameObject->worldTransform.translation = { -10 , 0 , -10 };
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

	if (input.PushKey(DIK_W) || input.PushKey(DIK_S) || input.PushKey(DIK_D) || input.PushKey(DIK_A) || input.PushKey(DIK_E) || input.PushKey(DIK_Q))
	{
		// 移動後の座標を計算
		if (input.PushKey(DIK_W)) { move = { 0,moveSpeed,0 }; }
		else if (input.PushKey(DIK_S)) { move = { 0,-moveSpeed,0 }; }
		if (input.PushKey(DIK_D)) { move = { moveSpeed,0,0 }; }
		else if (input.PushKey(DIK_A)) { move = { -moveSpeed,0,0 }; }

		if (input.PushKey(DIK_E)) { move = { 0,0,0.1 }; }
		else if (input.PushKey(DIK_Q)) { move = { 0,0,-0.1 }; }

		if (input.PushKey(DIK_D) && input.PushKey(DIK_W)) { move = { moveSpeed,moveSpeed,0 }; }
		else if (input.PushKey(DIK_D) && input.PushKey(DIK_S)) { move = { moveSpeed,-moveSpeed,0 }; }

		if (input.PushKey(DIK_A) && input.PushKey(DIK_W)) { move = { -moveSpeed,moveSpeed,0 }; }
		if (input.PushKey(DIK_A) && input.PushKey(DIK_S)) { move = { -moveSpeed,-moveSpeed,0 }; }
	}


	gameObject->worldTransform.translation += move;
}
void Player::NewBullet(ViewProjection* viewProjection, XMMATRIX* matProjection, Vector3 enemyPos, Vector3 playerPos) {
	playerPos = GetPos();
	//enemyPos = enemy->GetWorldTransform().translation;
	//弾を生成し、初期化
	std::unique_ptr<PlayerBullet>newBullet = std::make_unique<PlayerBullet>();
	newBullet->Initialize(viewProjection, matProjection, enemyPos, playerPos);

	//弾を登録する
	bullets_.push_back(std::move(newBullet));

	timer--;
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		//bullet->AttackPress();
		bullet->Update();
	}
	const std::list < std::unique_ptr<PlayerBullet>>& playerBullets = GetBullets();
	for (const std::unique_ptr<PlayerBullet>& bulletA : playerBullets) {
		if (input.PushKey(DIK_P)) {
			isDead = true;
			bulletA->OnCollision();
		}
	}
}
void Player::Collision() {

	//const std::list < std::unique_ptr<Enemy>>& enemyLoads = GetEnemyBullets();
	//for (const std::unique_ptr<Enemy>& enemy : enemyLoads) {

	////enemy-player
	//if (enemy->GetWorldTransform().translation.x - gameObject->worldTransform.translation.x < 2 &&
	//	-2 < enemy->GetWorldTransform().translation.x - gameObject->worldTransform.translation.x) {
	//	if (enemy->GetWorldTransform().translation.y - gameObject->worldTransform.translation.y < 3 &&
	//		-3 < enemy->GetWorldTransform().translation.y - gameObject->worldTransform.translation.y) {
	//		if (enemy->GetWorldTransform().translation.z - gameObject->worldTransform.translation.z < 3 &&
	//			-3 < enemy->GetWorldTransform().translation.z - gameObject->worldTransform.translation.z) {
	//			if (life > 0) {
	//				life -= 10;
	//			}
	//			else {
	//				isDead = true;
	//			}
	//		}
	//	}
	//}
	////bullet-enemy
	//
	//
	//

	//const std::list < std::unique_ptr<PlayerBullet>>& playerBullets = GetBullets();
	//for (const std::unique_ptr<PlayerBullet>& bulletA : playerBullets) {
	//		if (input.PushKey(DIK_P)) {
	//			isDead = true;
	//			bulletA->OnCollision();
	//		}
	//	if (enemy->GetWorldTransform().translation.x - bulletA->GetWorldTransform().translation.x < 2 &&
	//		-2 < enemy->GetWorldTransform().translation.x - bulletA->GetWorldTransform().translation.x) {
	//		if (enemy->GetWorldTransform().translation.y - bulletA->GetWorldTransform().translation.y < 3 &&
	//			-3 < enemy->GetWorldTransform().translation.y - bulletA->GetWorldTransform().translation.y) {
	//			if (enemy->GetWorldTransform().translation.z - bulletA->GetWorldTransform().translation.z < 3 &&
	//				-3 < enemy->GetWorldTransform().translation.z - bulletA->GetWorldTransform().translation.z) {
	//
	//				bulletA->OnCollision();
	//				enemy->Reset();
	//				isDead=true;
	//			}
	//		}
	//	}
	//}
	//}

}

//アクセッサ
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
	enemy = enemy;
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

int Player::GetIsAimHit() {
	return isAimHit;
}

Vector3 Player::GetAngle() {
	return angle;
}

WorldTransform Player::GetWorldTransform() {
	return gameObject->worldTransform;
}

Vector3 Player::GetPos() {
	return {
		gameObject->worldTransform.matWorld.m[3][0],
		gameObject->worldTransform.matWorld.m[3][1],
		gameObject->worldTransform.matWorld.m[3][2],
	};
}

Vector3 Player::GetAimPos() {
	return {
		aimObject->worldTransform.matWorld.m[3][0],
		aimObject->worldTransform.matWorld.m[3][1],
		aimObject->worldTransform.matWorld.m[3][2],
	};
}

void Player::OnCollision() {
	isDead = true;
}