#pragma once
#include "GameObject3D.h"
#include "Input.h"
#include "MathFunc.h"
#include "PlayerBullet.h"
#include "Pattern2.h"
#include "EnemyBullet.h"
#include <memory>
#include<list>


class Map;
class Goal;
class Enemy;
class GameScene;

class Player {
public:
	//�R���X�g���N�^
	Player();
	//�f�X�g���N�^
	~Player();

	void Initialize(ViewProjection* viewProjection, XMMATRIX* matProjection);

	void Update(WorldTransform wt);

	void Aim(Vector3 player, Vector3 enemy);

	void Draw();

	void Reset();

	void OnCollision();

	//�A�N�Z�b�T
	void SetMap(Map* map);
	void SetGoal(Goal* goal);
	void SetEnemy(Enemy* enemy);
	void NewBullet(ViewProjection* viewProjection, XMMATRIX* matProjection, Vector3 enemyPos,Vector3 playerPos);
	void NewBulletAim(ViewProjection* viewProjection, XMMATRIX* matProjection, Vector3 enemyPos,Vector3 playerPos);
	int GetIsGoal();
	void SetIsGoal(int flag);
	void SetPos(Vector3 pos) { gameObject->worldTransform.translation = pos; }

	int GetIsEnemy();
	void SetIsEnemy(int flag);

	int GetIsDead();

	Vector3 GetAngle();
	ViewProjection* GetViewProjection() { return viewProjection; }
	WorldTransform GetWorldTransform();
	Vector3 GetPos();

	Vector3 GetAimPos();
	int GetIsAimHit();
	void AimHit();

	Vector3 centerVec = { 0,0,0 };
	Vector3 angle = {};


	//�e���X�g���擾
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }
	const std::list<std::unique_ptr<Pattern2>>& GetAim() { return bulletsAim_; }
	//�����o�֐�
private:
	void Rotate();
	void Move();
	void Collision();


	//�����o�ϐ�
private:
	//�L�[����
	Input& input = Input::GetInstance();

	//�Q�[���I�u�W�F�N�g
	GameObject3D* gameObject = nullptr;
	GameObject3D* aimObject = nullptr;

	ViewProjection* viewProjection;

	Vector3 velocity = {};
	
	int life = 100;

	const float accelaration = 0.0025f;

	float moveSpeed = 0;
	float distance = 1;
	const float maxSpeed = 0.5;
	float ct = 0;
	const float g = 3;
	float timer = 0;
	int jumpFlag = 0;
	Vector3 target;

	int isDead = false;

	int isAimHit = false;

	int aimTouch = false;

	int isHitMap = false;

	int isGoal = false;
	int isEnemy = false;
	bool newBulletFired = false;
	Map* map = nullptr;
	Goal* goal = nullptr;
	Enemy* enemy = nullptr;
	
	Vector3 enemyPos = {};
	Vector3 playerPos = {};

	Vector3 move = {};

	ViewProjection* viewProjection_;
	XMMATRIX* matProjection_;
	//�e
	std::list<std::unique_ptr<PlayerBullet>>bullets_;
	PlayerBullet* playerBullet = nullptr;

	std::list<std::unique_ptr<Pattern2>>bulletsAim_;
	Pattern2* playerBulletAim = nullptr;

	//3D���e�B�N���p���[���h�g�����X�t�H�[��
	WorldTransform worldTransform3DReticle_;
	
	Vector3 posA;
	Vector3 posB;
	Vector3 posC;
};

