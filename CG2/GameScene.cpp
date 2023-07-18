#include "GameScene.h"
#include <cassert>
#include <chrono>
#include <thread>

GameScene::GameScene() {

}

GameScene::~GameScene() {
	delete rhythm;
	delete player;
	delete playerBullet;
	delete skydome;
	delete particle;
	delete particle2;
	delete reilCamera;
	delete object1;

	//delete UIManager;
}

void GameScene::Initialize(WinApp* winApp)
{
	// デバッグテキスト用テクスチャ読み込み
	Sprite::LoadTexture(0, L"Resources/debugfont.png");
	// デバッグテキスト初期化
	debugText.Initialize(0);

	//透視投影変換行列の計算
	matProjection_ = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(45.0),
		(float)winApp->window_width / winApp->window_height,
		0.1f, 1000.0f
	);

	// UI用テクスチャ読み込み
	Sprite::LoadTexture(1, L"Resources/gamefont.png");
	Sprite::LoadTexture(2, L"Resources/text.png");

	crosshair->LoadTexture(11, L"Resources/crosshair.png");
	crosshair = Sprite::Create(11, { 0,0 });

	//UI初期化
	UIManager.Initialize(1);


	viewProjection_.Initialize(winApp);

	//XAudioエンジンのインスタンスを生成
	//soundManager_ = new SoundManager();
	//soundManager_->Initialize();

	//天球
	skydome = new GameObject3D();
	skydome->PreLoadModel("Resources/skydome/skydome.obj");
	skydome->PreLoadTexture(L"Resources/skydome/skydome.png");
	skydome->SetViewProjection(&viewProjection_);
	skydome->SetMatProjection(&matProjection_);
	skydome->Initialize();
	skydome->worldTransform.scale = { 2000.0f,2000.0f,2000.0f };

	//レールカメラ
	reilCamera = new ReilCamera();
	//reilCamera->Initialize({ 0,0,-50 }, { 0,0,0 },winApp);
	reilCamera->Initialize({ 0,0,0 }, { 0,0,0 }, winApp);

	//アイテム
	//item = new Item();
	//item->Initialize(&viewProjection_, &matProjection_, L"Resources/white1x1.png", {0.0f,0.0f,50.0f},2);

	player = new Player();
	player->Initialize(&viewProjection_, &matProjection_);
	player->SetPos({ 0.0f, 0.0f, 20.0f });

	//boss = new Boss();
	//boss->Initialize(&viewProjection_, &matProjection_);

	viewProjection_ = reilCamera->GetViewProjection();

	//player->SetMap(map);
	//player->SetGoal(goal);

	//particle = new Particle;
	//particle->Initialize(&viewProjection_, &matProjection_, player);

	//particle2 = new Particle;
	//particle2->Initialize(&viewProjection_, &matProjection_, player);

	//particle3 = new Particle;
	//particle3->Initialize(&viewProjection_, &matProjection_, player);
	//loadEnemyPopData();
	//モデル名を指定してファイル読み込み
	model1 = FbxLoader::GetInstance()->LoadModelFromFile("boneTest");

	//デバイスをセット
	FbxObject3D::SetDevice(dx12base_.GetDevice());
	FbxObject3D::SetCamera(&viewProjection_);
	FbxObject3D::CreateGraphicsPipeline();

	object1 = new FbxObject3D;
	object1->SetViewProjection(&viewProjection_);
	object1->SetMatProjection(&matProjection_);
	object1->Initialize();
	object1->SetModel(model1);
	object1->PlayAnimation();
	object1->SetRotation({ 0.0f, 90.0f, 0.0f });
	
	//rhythm = new Rhythm();
	//rhythm->Initialize(&viewProjection_, &matProjection_);
//	spawntime = 0;
//#pragma region enemy初期化
//
//	for (int i = 1; i <= enemyVal; i++)
//	{
//		std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
//		newEnemy->Initialize(&viewProjection_, &matProjection_, L"Resources/red1x1.png");
//		newEnemy->Settransform({ 0.0f, 0.0f, -1000.0f });
//		//pManager[i].Initialize(&viewProjection_, &matProjection_, L"Resources/purple1x1.png");
//		//敵を登録
//		enemys1.push_back(std::move(newEnemy));
//	}
//	//loadEnemyPopData(1);
//	//ボスの雑魚敵の配置
//	loadBossPopData(1);
}

void GameScene::Update()
{
	//spawntime += 1;
	//LoadCsv(L"Resources/enemyPos.csv", enemyVal);
	////ランダムな整数
	std::default_random_engine engine(seed_gen());

	viewProjection_ = reilCamera->GetViewProjection();

	viewProjection_.UpdateView();
	skydome->Update();
	////UI更新
	//UIManager.Update(rhythm, &input_, player->GetIsDead());

	//if (rhythm->GetSoundState().isPause == 0) {

		player->Update(reilCamera->GetWorldTransform(), reilCamera->GetWorldTransform().rotation);

	//	if (player->GetIsDead() == false) {
	//		reilCamera->Update(&input_, rhythm->GetSoundState().wave);
	//	}

	//	particle->Update();
	//	particle2->Update2();

		skydome->Update();
		
//
//		crosshair->SetPosition({ player->GetWorldTransform().translation.x, player->GetWorldTransform().translation.y });
//		crosshair->Sprite::SetSize({ 1,1 });
//
//		//アイテムの更新処理
//		for (std::unique_ptr<Item>& item : items_) {
//					item->Update();
//		}
//
//		//デスフラグの立ったアイテムを削除
//		items_.remove_if([](std::unique_ptr<Item>& item) {
//			return item->GetIsDead();
//		});
//
//		//アイテム生成
//		if (input_.TriggerKey(DIK_T)) {
//			std::uniform_int_distribution<> dist(0, 4);
//			int value = dist(engine);
//			//アイテムを生成し、初期化
//			std::unique_ptr<Item>item = std::make_unique<Item>();
//			item->Initialize(&viewProjection_, &matProjection_, L"Resources/white1x1.png", { player->GetPos().x,player->GetPos().y,player->GetPos().z + 20.0f }, value);
//
//			//アイテムを登録する
//			items_.push_back(std::move(item));
//		}
// 
//
//		//敵の更新処理
//		for (std::unique_ptr<Enemy>& enemy : enemys1) {
//			enemy->Update(&viewProjection_, &matProjection_, 0);
//			if (enemy->IsDead()) {
//				player->NotAimHit();
//			}
//#pragma region makeEnemyBullet
//			if (enemy->GetAttackSpeed() <= 0.0f && enemy->GetPhase() == Phase::move) {
//				//弾を生成
//				std::unique_ptr<EnemyBullet> bullet = std::make_unique<EnemyBullet>();
//				//初期化
//				bullet->Initialize(&viewProjection_, &matProjection_, L"Resources/white1x1.png", player->GetPos(), enemy->GetWorldTransform().translation);
//				bullet->SetTransform(enemy->GetWorldTransform().translation);
//				//使う弾の設定
//				bullet->SetBullet(enemy->GetBulletNum());
//				bullets1.push_back(std::move(bullet));
//				//攻撃頻度の設定 1(速い)~ >1(遅い)
//				enemy->SetAttackSpeed(100.0f);
//
//				if (enemy->GetIsAttack() == false) {
//					enemy->SetIsAttack(true);
//				}
//			}
//			
//			if (enemy->GetIsAttack() == true) {
//				for (std::unique_ptr<EnemyBullet>& bullet : bullets1) {
//					bullet->Update();
//				}
//			}
//
//
//			//弾&敵を削除する
//			bullets1.remove_if([](std::unique_ptr<EnemyBullet>& bullet) { return bullet->IsDead(); });
//#pragma endregion
//			enemyPos = enemy->GetWorldTransform().translation;
//		}
//		//敵1の削除
//		enemys1.remove_if([](std::unique_ptr<Enemy>& enemy) {return enemy->IsDead(); });
//		
//		//ボス関連
//
//		if (rhythm->GetSoundState().wave == 3) {
//			boss->Update();
//#pragma region made BossBullet
//			if (boss->GetIsDead() == false) {
//				if (boss->GetPhase() == BossPhase::attack && boss->GetAttackSpeed() <= 0.0f) {
//					//弾を生成
//					std::unique_ptr<BossBullet> bullet = std::make_unique<BossBullet>();
//					bullet->Initialize(&viewProjection_, &matProjection_, player->GetPos(), boss->GetWorldTransform().translation);
//					bullet->SetTransform(boss->GetWorldTransform().translation);
//					bossBullet1.push_back(std::move(bullet));
//					boss->SetAttackSpeed(50.0f);
//					if (boss->GetIsAttack() == false) {
//						boss->SetIsAttack(true);
//					}
//				}
//				if (boss->GetPhase() == BossPhase::attack2 && boss->GetAttackSpeed() <= 0.0f) {
//					//弾を生成
//					std::unique_ptr<BossBullet> bullet = std::make_unique<BossBullet>();
//					bullet->Initialize(&viewProjection_, &matProjection_, player->GetPos(), boss->GetWorldTransform().translation);
//					bullet->SetTransform(boss->GetWorldTransform().translation);
//					bossBullet2.push_back(std::move(bullet));
//					boss->SetAttackSpeed(150.0f);
//					if (boss->GetIsAttack() == false) {
//						boss->SetIsAttack(true);
//					}
//				}
//
//				if (boss->GetIsAttack() == true) {
//					for (std::unique_ptr<BossBullet>& bullet : bossBullet1) {
//						bullet->Update(boss->GetPhase());
//					}
//					for (std::unique_ptr<BossBullet>& bullet : bossBullet2) {
//						bullet->Update(boss->GetPhase());
//					}
//				}
//				//弾&敵を削除する
//				bossBullet1.remove_if([](std::unique_ptr<BossBullet>& bullet) { return bullet->IsDead(); });
//				bossBullet2.remove_if([](std::unique_ptr<BossBullet>& bullet) { return bullet->IsDead(); });
//
//			}
//#pragma endregion
//			if (boss->GetPhase() == BossPhase::defence) {
//				UpdateBossPopCommand();
//			}
//		}
		//if (player->GetIsDead() == false) {
		//	//UpdateEnemyPopCommand();
		//}

		//	//UpdateEnemyPopCommand();
		//	if (player->GetIsDead() == false) {
		//		//enemy->Update(player->GetWorldTransform().translation, enemy->GetWorldTransform().translation);
		//	}

		//	if (player->GetIsDead() == false) {
		//		//enemy->Update(player->GetWorldTransform().translation, enemy->GetWorldTransform().translation);
		//	}

		//	if (input_.PushKey(DIK_R)) {
		//		Reset();
		//	}

		//	/*if (player->GetIsDead() == true && particle->GetIsDead() == true) {
		//		if (gameoverTimer <= 0) {
		//			gameoverTimer = 5;
		//		}
		//		else {
		//			gameoverTimer--;
		//			if (gameoverTimer <= 0) {
		//				stage = 1;
		//				Reset();
		//				scene_ = Scene::Title;
		//			}
		//		}
		//	}*/
		//	rhythm->Update(&input_, player->GetPos(), reilCamera->GetWorldTransform().rotation, player->GetIsDead(), stage);
		//	//プレイヤーの弾発射処理
		//	if (input_.TriggerKey(DIK_SPACE) && rhythm->GetSoundState().isFireSucces) {
		//		player->NewBullet(&viewProjection_, &matProjection_, { 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f }, rhythm->GetSoundState().weapon);
		//		//player->NewBulletAim(&viewProjection_, &matProjection_, enemyPos, player->GetWorldTransform().translation);
		//		
		//	}
		//	Collisions();
		//	player->Aim(player->GetWorldTransform().translation, { 0,0,100 }, reilCamera->GetWorldTransform().rotation, 0.0f);

		object1->Update();
	}	

#pragma region DebugText
	//debugText.Printf(0, 100, 1.0f, 18, " Q,E...offset:%f", rhythm->GetSoundState().offset);
	//debugText.Printf(0, 140, 1.0f, 25, " Up,Dawn...BGMVolume:%f", rhythm->GetSoundState().BGMVolume);
	//debugText.Printf(0, 160, 1.0f, 32, " Left,Right...guideSEVolume:%f", rhythm->GetSoundState().guideSEVolume);
	//debugText.Printf(0, 120, 1.0f, 10, " Timer:%f", rhythm->GetSoundState().timer);
	//debugText.Printf(0, 180, 1.0f, 17, " measureCount:%d", rhythm->GetSoundState().measureCount);
	//debugText.Printf(0, 200, 1.0f, 9, " weapon:%d", rhythm->GetSoundState().weapon);
	//debugText.Printf(0, 280, 1.0f, 9, " spawn:%d", spawntime);
	//debugText.Printf(0, 220, 1.0f, 7, " wave:%f", rhythm->GetSoundState().wave);
	//debugText.Printf(0, 240, 1.0f, 11, " rotY:%f", reilCamera->GetRotation().x);
	//debugText.Printf(0, 260, 1.0f, 27, " %f,%f,%f",
	//	player->GetWorldTransform().matWorld.m[3][0],
	//	player->GetWorldTransform().matWorld.m[3][1],
	//	player->GetWorldTransform().matWorld.m[3][2]);

//	debugText.Printf(0, 280, 1.0f, 11, " Phase : %d", boss->GetPhase());
//#pragma endregion
//}

void GameScene::Draw() {
	//3D描画
	player->Draw();
	//アイテム描画
	//for (std::unique_ptr<Item>& item : items_) { item->Draw(); }
	skydome->Draw();

	object1->Draw(dx12base_.GetCmdList().Get());

	//スプライト描画


	//crosshair->Draw();
	//
	//UIManager.Draw(rhythm);
  
	// デバッグテキストの描画
	debugText.DrawAll(dx12base_.GetCmdList().Get());



}

void GameScene::loadBossPopData(int stageNum)
{
	if (stageNum == 1) {
		//ファイルを開く
		std::ifstream file;
		file.open("Resources/csv/bossAttack.csv");
		assert(file.is_open());
		//ファイルの内容を文字列ストリームにコピー
		bossPopCommand << file.rdbuf();
		//ファイルを閉じる
		file.close();
	}
}

void GameScene::UpdateBossPopCommand()
{
	if (bossWaitFlag) {
		bossWaitTime_--;
		if (bossWaitTime_ <= 0.0f) {
			//待機完了
			bossWaitFlag = false;
		}
		return;
	}
	//1行分の文字列を入れる
	std::string line;
	//コマンド実行
	while (std::getline(bossPopCommand, line)) {
		//1行分の文字列をストリームに変換
		std::istringstream line_stream(line);
		std::string world;
		// ,区切りで行の先頭文字列を取得
		std::getline(line_stream, world, ',');
		//"//"から始まる行はコメント
		if (world.find("//") == 0) {
			continue;
		}
		//POPコマンド
		if (world.find("Enemy1") == 0) {	//固定砲台
			//CSVに書いてある値を変数に入れる
			//x座標
			std::getline(line_stream, world, ',');
			float x = (float)std::atof(world.c_str());
			//y座標
			std::getline(line_stream, world, ',');
			float y = (float)std::atof(world.c_str());
			//z座標
			std::getline(line_stream, world, ',');
			float z = (float)std::atof(world.c_str());
			//移動速度
			std::getline(line_stream, world, ',');
			float speedX = (float)std::atof(world.c_str());
			std::getline(line_stream, world, ',');
			float speedY = (float)std::atof(world.c_str());
			std::getline(line_stream, world, ',');
			float speedZ = (float)std::atof(world.c_str());
			//敵を発生させる
			//-------ここにEnemy発生関数---------//
			//複数化するためにuniq_ptrに変更
			std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
			newEnemy->Initialize(&viewProjection_, &matProjection_, L"Resources/red.png");
			//上で書いてある物をEnemyの座標としてセットする
			newEnemy->Settransform(x, y, z);
			newEnemy->SetSpeed(speedX, speedY, speedZ);
			//敵を登録
			enemys1.push_back(std::move(newEnemy));
		}
		//WAITコマンド
		else if (world.find("WAIT") == 0)
		{
			std::getline(line_stream, world, ',');
			//待ち時間
			int32_t waitTime = atoi(world.c_str());
			//待ち時間
			bossWaitFlag = true;
			bossWaitTime_ = waitTime;
			//コマンドループを抜ける
			break;
		}
	}
}

void GameScene::Reset() {

	player->Reset();
	particle->Reset();
	particle2->Reset();

}

void GameScene::Collisions() {
	//	for (std::unique_ptr<Enemy>& enemy : enemys1) {
	//			if (enemy->GetWorldTransform().translation.x - player->GetAimPos().x < 1 &&
	//				-1 < enemy->GetWorldTransform().translation.x - player->GetAimPos().x) {
	//				if (enemy->GetWorldTransform().translation.y - player->GetAimPos().y < 1 &&
	//					-1 < enemy->GetWorldTransform().translation.y - player->GetAimPos().y) {
	//					if (enemy->GetWorldTransform().translation.z - player->GetAimPos().z < 100 &&
	//						-100 < enemy->GetWorldTransform().translation.z - player->GetAimPos().z) {
	//						player->AimHit();
	//					}
	//					else 
	//					{
	//						player->NotAimHit();
	//					}
	//				}
	//			}
	//		}
	//#pragma region PlayerToEnemyCollision
	//	const std::list < std::unique_ptr<PlayerBullet>>& playerBullets = player->GetBullets();
	//		for (std::unique_ptr<Enemy>& enemy : enemys1) {
	//			//enemy-player
	//			if (enemy->GetWorldTransform().translation.x - player->GetPos().x < 2 &&
	//				-2 < enemy->GetWorldTransform().translation.x - player->GetPos().x) {
	//				if (enemy->GetWorldTransform().translation.y - player->GetPos().y < 2 &&
	//					-2 < enemy->GetWorldTransform().translation.y - player->GetPos().y) {
	//					if (enemy->GetWorldTransform().translation.z - player->GetPos().z < 2 &&
	//						-2 < enemy->GetWorldTransform().translation.z - player->GetPos().z) {
	//
	//						player->OnCollision(rhythm);
	//
	//					}
	//				}
	//			}
	//#pragma endregion
	//
	//#pragma region bulletToEnemyCollisions
	//			const std::list < std::unique_ptr<PlayerBullet>>& playerBullets = player->GetBullets();
	//			for (const std::unique_ptr<PlayerBullet>& bulletA : playerBullets) {
	//				if (enemy->GetWorldTransform().translation.x - bulletA->GetWorldTransform().translation.x < 2 &&
	//					-2 < enemy->GetWorldTransform().translation.x - bulletA->GetWorldTransform().translation.x) {
	//					if (enemy->GetWorldTransform().translation.y - bulletA->GetWorldTransform().translation.y < 2 &&
	//						-2 < enemy->GetWorldTransform().translation.y - bulletA->GetWorldTransform().translation.y) {
	//						if (enemy->GetWorldTransform().translation.z - bulletA->GetWorldTransform().translation.z < 2 &&
	//							-2 < enemy->GetWorldTransform().translation.z - bulletA->GetWorldTransform().translation.z) {
	//
	//							bulletA->OnCollision();
	//							enemy->OnCollision(rhythm);
	//							player->NotAimHit();
	//
	//						}
	//					}
	//				}
	//			}
	//		}
	//#pragma endregion
	//
	//#pragma region bulletToEnemyBulletCollisions
	//		//bullet-enemyBullet
	//		for (const std::unique_ptr<EnemyBullet>& bulletB : bullets1) {
	//			for (const std::unique_ptr<PlayerBullet>& bulletA : playerBullets) {
	//				if (input_.PushKey(DIK_P)) {
	//					//player->OnCollision();
	//					bulletA->OnCollision();
	//				}
	//				if (bulletB->GetWorldTransform().translation.x - bulletA->GetWorldTransform().translation.x < 2 &&
	//					-2 < bulletB->GetWorldTransform().translation.x - bulletA->GetWorldTransform().translation.x) {
	//					if (bulletB->GetWorldTransform().translation.y - bulletA->GetWorldTransform().translation.y < 2 &&
	//						-2 < bulletB->GetWorldTransform().translation.y - bulletA->GetWorldTransform().translation.y) {
	//						if (bulletB->GetWorldTransform().translation.z - bulletA->GetWorldTransform().translation.z < 2 &&
	//							-2 < bulletB->GetWorldTransform().translation.z - bulletA->GetWorldTransform().translation.z) {
	//
	//							bulletA->OnCollision();
	//							bulletB->OnCollision();
	//
	//						}
	//					}
	//				}
	//			}
	//		}
	//
	//		//bullet-enemyBullet
	//		for (const std::unique_ptr<EnemyBullet>& bulletB : bullets2) {
	//			for (const std::unique_ptr<PlayerBullet>& bulletA : playerBullets) {
	//				if (input_.PushKey(DIK_P)) {
	//					//player->OnCollision();
	//					bulletA->OnCollision();
	//				}
	//				if (bulletB->GetWorldTransform().translation.x - bulletA->GetWorldTransform().translation.x < 2 &&
	//					-2 < bulletB->GetWorldTransform().translation.x - bulletA->GetWorldTransform().translation.x) {
	//					if (bulletB->GetWorldTransform().translation.y - bulletA->GetWorldTransform().translation.y < 2 &&
	//						-2 < bulletB->GetWorldTransform().translation.y - bulletA->GetWorldTransform().translation.y) {
	//						if (bulletB->GetWorldTransform().translation.z - bulletA->GetWorldTransform().translation.z < 2 &&
	//							-2 < bulletB->GetWorldTransform().translation.z - bulletA->GetWorldTransform().translation.z) {
	//
	//							bulletA->OnCollision();
	//							bulletB->OnCollision();
	//
	//						}
	//					}
	//				}
	//			}
	//		}
	//#pragma endregion
	//
	//#pragma region enemyBulletToPlayerCollisions
	//		//player-enemybullet
	//		for (const std::unique_ptr<EnemyBullet>& bulletB : bullets1) {
	//
	//			if (player->GetPos().x - bulletB->GetWorldTransform().translation.x < 2 &&
	//				-2 < player->GetPos().x - bulletB->GetWorldTransform().translation.x) {
	//				if (player->GetPos().y - bulletB->GetWorldTransform().translation.y < 2 &&
	//					-2 < player->GetPos().y - bulletB->GetWorldTransform().translation.y) {
	//					if (player->GetPos().z - bulletB->GetWorldTransform().translation.z < 2 &&
	//						-2 < player->GetPos().z - bulletB->GetWorldTransform().translation.z) {
	//
	//						//bulletB->OnCollision();
	//						//enemy->Reset();
	//						player->OnCollision(rhythm);
	//					}
	//				}
	//			}
	//		}
	//		for (const std::unique_ptr<EnemyBullet>& bulletC : bullets2) {
	//			if (player->GetPos().x - bulletC->GetWorldTransform().translation.x < 2 &&
	//				-2 < player->GetPos().x - bulletC->GetWorldTransform().translation.x) {
	//				if (player->GetPos().y - bulletC->GetWorldTransform().translation.y < 2 &&
	//					-2 < player->GetPos().y - bulletC->GetWorldTransform().translation.y) {
	//					if (player->GetPos().z - bulletC->GetWorldTransform().translation.z < 2 &&
	//						-2 < player->GetPos().z - bulletC->GetWorldTransform().translation.z) {
	//
	//						//bulletB->OnCollision();
	//						//enemy->Reset();
	//						player->OnCollision(rhythm);
	//					}
	//				}
	//			}
	//		}
	//#pragma endregion
	//
	//		const std::list < std::unique_ptr<Pattern2>>& playerAim = player->GetAim();
	//		
	//		for (std::unique_ptr<Enemy>& enemy : enemys1) {
	//			for (const std::unique_ptr<Pattern2>& bulletA : playerAim) {
	//				if (enemy->GetWorldTransform().translation.x - bulletA->GetWorldTransform().translation.x < 4 &&
	//					-4 < enemy->GetWorldTransform().translation.x - bulletA->GetWorldTransform().translation.x) {
	//					if (enemy->GetWorldTransform().translation.y - bulletA->GetWorldTransform().translation.y < 4 &&
	//						-4 < enemy->GetWorldTransform().translation.y - bulletA->GetWorldTransform().translation.y) {
	//						if (enemy->GetWorldTransform().translation.z - bulletA->GetWorldTransform().translation.z < 5 &&
	//							-5 < enemy->GetWorldTransform().translation.z - bulletA->GetWorldTransform().translation.z) {
	//
	//							bulletA->OnCollision();
	//						}
	//					}
	//				}
	//			}
	//		}

}

void GameScene::LoadCsv(const wchar_t* fileName, int obstacleVal)
{
	// open file
	std::ifstream file;
	file.open(fileName);
	std::stringstream obstaclePosList;    // string stream
	obstaclePosList << file.rdbuf();
	file.close();

	//1行分の文字列
	std::string line;
	std::vector<Vector3> obstaclePos;
	std::vector<int32_t> spawntimer;
	std::vector<int32_t> bulletNum;

	//コマンド実行
	while (std::getline(obstaclePosList, line)) {
		//1行分の文字列をストリームに変換
		std::istringstream line_stream(line);
		//std::string first_value;
		std::string word;
		std::getline(line_stream, word, ',');
		if (word.find("//") == 0) {
			continue;
		}
		if (word.find("{") == 0) {
			std::getline(line_stream, word, ',');
			int x = (int)std::atof(word.c_str());
			std::getline(line_stream, word, ',');
			int y = (int)std::atof(word.c_str());
			std::getline(line_stream, word, ',');
			int z = (int)std::atof(word.c_str());
			std::getline(line_stream, word, ',');
			int32_t timer = (int)std::atof(word.c_str());
			std::getline(line_stream, word, ',');
			useBullet = (int)std::atof(word.c_str());
			//座標
			Vector3 pos(x, y, z);
			//待ち時間
			int32_t waitTime = timer;
			obstaclePos.push_back(pos);
			spawntimer.push_back(waitTime);
			bulletNum.push_back(useBullet);
		}
	}

	int i = 0;
	spawntime += 1;
	for (std::unique_ptr<Enemy>& newEnemy : enemys1) {
		if (spawntime == spawntimer[1]) {
			if (i < obstaclePos.size() && i < 12) {
				if (newEnemy->GetSpownFlag() == false && newEnemy->IsDead() == false) {
					newEnemy->Settransform(obstaclePos[i]);
					newEnemy->Spawn();
				}
				newEnemy->SetBulletNum(bulletNum[i]);
				newEnemy->SetSpeed(0, 0, 0);
				
			}
			i++;
		}
		else if (spawntime == spawntimer[22]) {
			if (i < obstaclePos.size() && i > 12 && i < 23) {
				if (newEnemy->GetSpownFlag() == false && newEnemy->IsDead() == false) {
					newEnemy->Settransform(obstaclePos[i]);
					newEnemy->Spawn();
				}
				newEnemy->SetBulletNum(bulletNum[i]);
				//newEnemy->SetSpeed(0, 100, -100);
				
			}
			i++;
		}
		else if (spawntime == spawntimer[27]) {
			if (i < obstaclePos.size() && i > 23 && i < 28) {
				if (newEnemy->GetSpownFlag() == false && newEnemy->IsDead() == false) {
					newEnemy->Settransform(obstaclePos[i]);
					newEnemy->Spawn();
				}
				newEnemy->SetBulletNum(bulletNum[i]);
				//newEnemy->SetSpeed(0, 100, -100);
				
			}
			i++;
		}
		else if (spawntime == spawntimer[32]) {
			if (i < obstaclePos.size() && i > 28 && i < 33) {
				if (newEnemy->GetSpownFlag() == false && newEnemy->IsDead() == false) {
					newEnemy->Settransform(obstaclePos[i]);
					newEnemy->Spawn();
				}
				newEnemy->SetBulletNum(bulletNum[i]);
				//newEnemy->SetSpeed(0, 100, -100);
				
			}
			i++;
		}
		else if (spawntime == spawntimer[40]) {
			if (i < obstaclePos.size() && i > 33 && i < obstacleVal) {
				if (newEnemy->GetSpownFlag() == false && newEnemy->IsDead() == false) {
					newEnemy->Settransform(obstaclePos[i]);
					newEnemy->Spawn();
				}
				newEnemy->SetBulletNum(bulletNum[i]);
				//newEnemy->SetSpeed(0, 100, -100);
				
			}
			i++;
		}
	}
}