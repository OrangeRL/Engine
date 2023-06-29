#pragma once
#include "Input.h"
#include "DX12base.h"
#include"GameObject3D.h"
#include "WinApp.h"
#include "ViewProjection.h"
#include "Audio.h"
#include "Collision.h"
//#include <xaudio2.h>
#pragma comment(lib,"xaudio2.lib")
#include <sstream>
#include "Sprite.h"
#include <memory>
#include <list>
#include <random>
#include "Player.h"
#include "Boss.h"
#include "Map.h"
#include "Goal.h"
#include "Particle.h"
#include "Enemy.h"
#include "Rhythm.h"
#include "DebugText.h"
#include "PlayerBullet.h"
#include "BossBullet.h"
#include "ReilCamera.h"
#include "Pattern2.h"
#include "Particle2.h"
#include "DirectXMath.h"
#include "Item.h"
#include "UIManager.h"
#include"FbxLoader.h"
#include"FbxObject3D.h"
#include"JsonLoader.h"
#include <vector>
#include <map>
class GameScene {

public: // メンバ関数

	//コンストラクタ
	GameScene();
	/// デストラクタ
	~GameScene();

	//初期化
	void Initialize(WinApp* winApp);

	//毎フレーム処理
	void Update();

	//描画処理
	void Draw();

	void Reset();

	void Collisions();

	/// <summary>
	/// 敵発生データの読み込み
	/// </summary>
	void loadEnemyPopData(int stageNum);
	/// <summary>
	/// 敵発生コマンドの更新
	/// </summary>
	void UpdateEnemyPopCommand();
	void LoadCsv(const wchar_t* fileName, int obstacleVal);
	const std::list<std::unique_ptr<Enemy>>& GetEnemies() { return enemys1; }

	//セッター
//変形行列セット
	void SetPosition0(Vector3 pos) { position0 = pos; }
	void SetRotation0(Vector3 rot) { rotation0 = rot; }
	void SetScale0(Vector3 sca) { scale0 = sca; }
	void SetPosition1(Vector3 pos) { position1 = pos; }
	void SetRotation1(Vector3 rot) { rotation1 = rot; }
	void SetScale1(Vector3 sca) { scale1 = sca; }

	//当たり判定セット
	void SetCollisionPlayer(Vector3 position, Vector3 scale);
	void SetCollisionEnemy(Vector3 position, Vector3 scale);
	void SetCollisionBullet(Vector3 position, Vector3 scale);
	void SetCollisionEnemyBullet(Vector3 position, Vector3 scale);
	void SetCollisionAim(Vector3 position, Vector3 scale);

	//ゲッター
//変形行列
	Vector3 GetPosition0() { return position0; }
	Vector3 GetRotation0() { return rotation0; }
	Vector3 GetScale0() { return scale0; }
	Vector3 GetPosition1() { return position1; }
	Vector3 GetRotation1() { return rotation1; }
	Vector3 GetScale1() { return scale1; }

	//hitbox
	Vector3 GetHitboxPosition0() { return hitboxPosition0; }
	Vector3 GetHitboxRotation0() { return hitboxRotation0; }
	Vector3 GetHitboxScale0() { return hitboxScale0; }
	Vector3 GetHitboxPosition1() { return hitboxPosition1; }
	Vector3 GetHitboxRotation1() { return hitboxRotation1; }
	Vector3 GetHitboxScale1() { return hitboxScale1; }

	/// <summary>
	/// 敵発生データの読み込み
	/// </summary>
	void loadBossPopData(int stageNum);
	/// <summary>
	/// 敵発生コマンドの更新
	/// </summary>
	void UpdateBossPopCommand();

private: // メンバ変数
	WinApp* winApp_ = nullptr;
	DX12base& dx12base_ = DX12base::GetInstance();
	Input& input_ = Input::GetInstance();
	XMMATRIX matProjection_ = {};
	DebugText debugText;

	//ランダムな整数
	std::random_device seed_gen;

	Rhythm* rhythm = nullptr;
	//SoundManager* soundManager_ = nullptr;
	ViewProjection viewProjection_;

	bool isPlayingBGM = false;

	Player* player = nullptr;
	PlayerBullet* playerBullet = nullptr;
	Pattern2* pattern2 = nullptr;

	Boss* boss = nullptr;
	std::list<std::unique_ptr<BossBullet>> bossBullet1;
	std::list<std::unique_ptr<BossBullet>> bossBullet2;

	Particle* particle = nullptr;
	Particle* particle2 = nullptr;
	Particle* particle3 = nullptr;
	//敵
	std::list<std::unique_ptr<Enemy>> enemys1;
	std::list<std::unique_ptr<EnemyBullet>> bullets1;
	std::list<std::unique_ptr<EnemyBullet>> bullets2;

	int useBullet;

	//天球
	GameObject3D* skydome = nullptr;

	//アイテム
	std::list<std::unique_ptr<Item>>items_;

	FbxModel* model1 = nullptr;
	FbxModel* model2 = nullptr;
	FbxModel* TestFBX = nullptr;
	FbxObject3D* object1 = nullptr;

	Vector3 enemyPos = {};
	//シーン管理
	enum class Scene
	{
		Title,//タイトル
		Stage,//ステージ
	};

	Scene scene_ = Scene::Title;

	int stage = 1;

	int gameoverTimer = 0;

	ReilCamera* reilCamera = nullptr;


	Sprite* num_[10];
	Sprite* crosshair = nullptr;

	Particle2 pManager[40];
	//変形行列
	Vector3 position0 = { 0.0f,0.0f,0.0f };
	Vector3 rotation0 = { 0.0f,0.0f,0.0f };
	Vector3 scale0 = { 0.01f,0.01f,0.01f };
	Vector3 position1;
	Vector3 rotation1;
	Vector3 scale1;

	//hitboxの変形行列
	Vector3 hitboxPosition0 = { 0.0f,0.0f,0.0f };
	Vector3 hitboxRotation0 = { 0.0f,0.0f,0.0f };
	Vector3 hitboxScale0 = { 0.01f,0.01f,0.01f };
	Vector3 hitboxPosition1;
	Vector3 hitboxRotation1;
	Vector3 hitboxScale1;

	//当たり判定
	std::list<std::unique_ptr<Collision>> collisionsPlayer;
	std::list<std::unique_ptr<Collision>> collisionsEnemy;
	std::list<std::unique_ptr<Collision>> collisionsBullet;
	std::list<std::unique_ptr<Collision>> collisionsEnemyBullet;
	std::list<std::unique_ptr<Collision>> collisionsAim;

		int32_t spawntime = 0;

	//UI関連
	UIManager UIManager;

// 敵コマンド関係
//------------------------------------
	std::stringstream enemyPopCommand;
	bool waitFlag = true;
	float waitTime_=10;
	size_t enemyVal = 80;
//------------------------------------
//ボスコマンド関係
//------------------------------------
	std::stringstream bossPopCommand;
	bool bossWaitFlag = false;
	float bossWaitTime_;
//------------------------------------

//JsonLoader

	//レベルエディタ
	JsonLoader* jsonLoader = nullptr;
	//オブジェクト
	std::list<std::unique_ptr<FbxObject3D>> objects;
	//オブジェクトの数
	size_t objectNum = 4;
	std::list<std::unique_ptr<FbxModel>> models;

};