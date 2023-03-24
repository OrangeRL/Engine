#include "GameScene.h"
#include <cassert>

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
	//delete UIManager;
	//画像の解放
	for (int i = 0; i < 10; i++) {
		delete num_[i];
	}
}

void GameScene::Initialize(WinApp* winApp) 
{
	// デバッグテキスト用テクスチャ読み込み
	Sprite::LoadTexture(10, L"Resources/debugfont.png");
	// デバッグテキスト初期化
	debugText.Initialize(10);


	//透視投影変換行列の計算
	matProjection_ = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(45.0),
		(float)winApp->window_width / winApp->window_height,
		0.1f, 1000.0f
	);

	num_[0]->LoadTexture(0, L"Resources/0.png");
	num_[1]->LoadTexture(1, L"Resources/1.png");
	num_[2]->LoadTexture(2, L"Resources/2.png");
	num_[3]->LoadTexture(3, L"Resources/3.png");
	num_[4]->LoadTexture(4, L"Resources/4.png");
	num_[5]->LoadTexture(5, L"Resources/5.png");
	num_[6]->LoadTexture(6, L"Resources/6.png");
	num_[7]->LoadTexture(7, L"Resources/7.png");
	num_[8]->LoadTexture(8, L"Resources/8.png");
	num_[9]->LoadTexture(9, L"Resources/9.png");



	for (int i = 0; i < 10; i++) {
		num_[i] = new Sprite(i, { 0,0 }, { 64,64 }, { 1.0f,1.0f,1.0f,0.5f }, { 0,0 }, 0, 0);
		num_[i]->Initialize();
	}

	//UI初期化
	UIManager.Initialize();

	viewProjection_.Initialize();

	//XAudioエンジンのインスタンスを生成
	//soundManager_ = new SoundManager();
	//soundManager_->Initialize();

	//天球
	skydome = new GameObject3D();
	skydome->PreLoadModel("Resources/skydome/skydome.obj");
	skydome->PreLoadTexture(L"Resources/skydome/Fine_Basin.jpg");
	skydome->SetViewProjection(&viewProjection_);
	skydome->SetMatProjection(&matProjection_);
	skydome->Initialize();
	skydome->worldTransform.scale = { 1000.0f,1000.0f,1000.0f };

	//レールカメラ
	reilCamera = new ReilCamera();
	reilCamera->Initialize({ 0,0,-50 }, { 0,0,0 });

	//アイテム
	//item = new Item();
	//item->Initialize(&viewProjection_, &matProjection_, L"Resources/white1x1.png", {0.0f,0.0f,50.0f},2);

	player = new Player();
	player->Initialize(&viewProjection_, &matProjection_);
	player->SetPos({ 0.0f, 0.0f, 20.0f });

	boss = new Boss();
	boss->Initialize(&viewProjection_, &matProjection_);

	viewProjection_ = reilCamera->GetViewProjection();

	//player->SetMap(map);
	//player->SetGoal(goal);

	particle = new Particle;
	particle->Initialize(&viewProjection_, &matProjection_, player);

	particle2 = new Particle;
	particle2->Initialize(&viewProjection_, &matProjection_, player);

	loadEnemyPopData(1);
	loadBossPopData(1);

	rhythm = new Rhythm();
	rhythm->Initialize(&viewProjection_, &matProjection_);
}

void GameScene::Update()
{
	//ランダムな整数
	std::default_random_engine engine(seed_gen());

	viewProjection_ = reilCamera->GetViewProjection();

	viewProjection_.UpdateView();

	if (rhythm->GetSoundState().isPause == 0) {


		player->Update(reilCamera->GetWorldTransform(), reilCamera->GetWorldTransform().rotation);
		reilCamera->Update(&input_);

		particle->Update();
		particle2->Update2();

		skydome->Update();

		//UI更新
		UIManager.Update(rhythm);


		//アイテムの更新処理
		for (std::unique_ptr<Item>& item : items_) {
			item->Update();
		}

		//デスフラグの立ったアイテムを削除
		items_.remove_if([](std::unique_ptr<Item>& item) {
			return item->GetIsDead();
			});

		//アイテム生成
		if (input_.TriggerKey(DIK_T)) {
			std::uniform_int_distribution<> dist(0, 4);
			int value = dist(engine);
			//アイテムを生成し、初期化
			std::unique_ptr<Item>item = std::make_unique<Item>();
			item->Initialize(&viewProjection_, &matProjection_, L"Resources/white1x1.png", { player->GetPos().x,player->GetPos().y,player->GetPos().z + 20.0f }, value);

			//アイテムを登録する
			items_.push_back(std::move(item));
		}

		//敵の更新処理
		for (std::unique_ptr<Enemy>& enemy : enemys1) {
			enemy->Update(&viewProjection_, &matProjection_, 0);
#pragma region makeEnemyBullet
			if (enemy->GetAttackSpeed() <= 0.0f && enemy->GetPhase() == Phase::move) {
				//弾を生成
				std::unique_ptr<EnemyBullet> bullet = std::make_unique<EnemyBullet>();
				//初期化
				bullet->Initialize(&viewProjection_, &matProjection_, L"Resources/white1x1.png", player->GetPos(), enemy->GetWorldTransform().translation);
				bullet->SetTransform(enemy->GetWorldTransform().translation);
				//使う弾の設定
				bullet->SetBullet(0);
				bullets1.push_back(std::move(bullet));
				//攻撃頻度の設定 1(速い)~ >1(遅い)
				enemy->SetAttackSpeed(150.0f);

				if (enemy->GetIsAttack() == false) {
					enemy->SetIsAttack(true);
				}
			}
			if (enemy->GetIsAttack() == true) {

				for (std::unique_ptr<EnemyBullet>& bullet : bullets1) {
					bullet->Update();
				}
			}

			//弾&敵を削除する
			bullets1.remove_if([](std::unique_ptr<EnemyBullet>& bullet) { return bullet->IsDead(); });
#pragma endregion
		enemyPos = enemy->GetWorldTransform().translation;
		//player->SetEnemy(enemy);
		//player->NewBullet(&viewProjection_, &matProjection_, enemyPos, player->GetWorldTransform().translation);
	}
	//敵1の削除
	enemys1.remove_if([](std::unique_ptr<Enemy>& enemy) {return enemy->IsDead(); });

	for (std::unique_ptr<Enemy>& enemy : enemys2) {
		enemy->Update(&viewProjection_, &matProjection_, 1);
	}
	//敵の削除
	enemys2.remove_if([](std::unique_ptr<Enemy>& enemy) {return enemy->IsDead(); });

	for (std::unique_ptr<Enemy>& enemy : enemys3) {
		enemy->Update(&viewProjection_, &matProjection_, 2);
	}
	//敵の削除
	enemys3.remove_if([](std::unique_ptr<Enemy>& enemy) {return enemy->IsDead(); });

	//ボス関連
	boss->Update();
#pragma region made BossBullet
	if (boss->GetIsDead()==false) {
		if (boss->GetPhase() == BossPhase::attack2 && boss->GetAttackSpeed() <= 0.0f) {
			//弾を生成
			std::unique_ptr<BossBullet> bullet = std::make_unique<BossBullet>();
			bullet->Initialize(&viewProjection_, &matProjection_, boss->Random(-0.5f, 0.1f));
			bullet->SetTransform(boss->GetWorldTransform().translation);
			bossBullet.push_back(std::move(bullet));
			boss->SetAttackSpeed(200.0f);
			if (boss->GetIsAttack() == false) {
				boss->SetIsAttack(true);
			}
		}
		if (boss->GetIsAttack() == true) {
			for (std::unique_ptr<BossBullet>& bullet : bossBullet) {
				bullet->Update();
			}
		}
	}	
#pragma endregion

	UpdateEnemyPopCommand();
	if (boss->GetPhase() == BossPhase::defence) {
		UpdateBossPopCommand();
	}


	if (player->GetIsDead() == false) {
		//enemy->Update(player->GetWorldTransform().translation, enemy->GetWorldTransform().translation);
	}

	if (player->GetIsDead() == false) {
		//enemy->Update(player->GetWorldTransform().translation, enemy->GetWorldTransform().translation);
	}

	if (input_.PushKey(DIK_R)) {
		Reset();
    
	}

	/*if (player->GetIsDead() == true && particle->GetIsDead() == true) {
		if (gameoverTimer <= 0) {
			gameoverTimer = 5;
		}
		else {
			gameoverTimer--;
			if (gameoverTimer <= 0) {
				stage = 1;
				Reset();
				scene_ = Scene::Title;
			}
		}
	}*/
  
}
	else {
		debugText.Printf(window_width/2, window_height/2, 1.0f, 6, " PAUSE");
	}

	rhythm->Update(&input_, player->GetPos(), reilCamera->GetWorldTransform().rotation);

	//プレイヤーの弾発射処理
	if (input_.TriggerKey(DIK_SPACE) && rhythm->GetSoundState().isFireSucces) {
		player->NewBullet(&viewProjection_, &matProjection_, { 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f });
	}

	Collision();

	debugText.Printf(0, 100, 1.0f, 18, " Q,E...offset:%f", rhythm->GetSoundState().offset);
	debugText.Printf(0, 140, 1.0f, 25, " Up,Dawn...BGMVolume:%f", rhythm->GetSoundState().BGMVolume);
	debugText.Printf(0, 160, 1.0f, 32, " Left,Right...guideSEVolume:%f", rhythm->GetSoundState().guideSEVolume);
	debugText.Printf(0, 120, 1.0f, 10, " Timer:%f", rhythm->GetSoundState().timer);
	debugText.Printf(0, 180, 1.0f, 17, " measureCount:%d", rhythm->GetSoundState().measureCount);
	debugText.Printf(0, 200, 1.0f, 9, " weapon:%d", rhythm->GetSoundState().weapon);
	debugText.Printf(0, 220, 1.0f, 7, " wave:%f", rhythm->GetSoundState().wave);
	debugText.Printf(0, 240, 1.0f, 11, " rotY:%f", reilCamera->GetRotation().x);
  
}

void GameScene::Draw() {
	//3D描画
	//プレイヤー描画
	player->Draw();

	rhythm->Draw();
	particle->Draw();
	particle2->Draw();
	//敵の描画
	for (std::unique_ptr<Enemy>& enemy : enemys1) {
		enemy->Draw();
	}
	for (std::unique_ptr<Enemy>& enemy : enemys2) {
		enemy->Draw();
	}
	for (std::unique_ptr<Enemy>& enemy : enemys3) {
		enemy->Draw();
	}

	boss->Draw(); for (std::unique_ptr<BossBullet>& bullet : bossBullet) {
		bullet->Draw();
	}

	for (std::unique_ptr<EnemyBullet>& bullet : bullets1) {
		bullet->Draw();
	}
	for (std::unique_ptr<EnemyBullet>& bullet : bullets2) {
		bullet->Draw();
	}

	//アイテム描画
	for (std::unique_ptr<Item>& item : items_) { item->Draw(); }
	skydome->Draw();

	//スプライト描画
	Sprite::PreDraw(dx12base_.GetCmdList().Get());

	for (int i = 0; i < 10; i++) {
		num_[i]->Draw();
	}
	UIManager.Draw(rhythm);

	// デバッグテキストの描画
	debugText.DrawAll(dx12base_.GetCmdList().Get());

	Sprite::PostDraw();

}

void GameScene::loadEnemyPopData(int stageNum)
{

	if (stageNum == 1) {
		//ファイルを開く
		std::ifstream file;
		file.open("Resources/csv/enemy.csv");
		assert(file.is_open());
		//ファイルの内容を文字列ストリームにコピー
		enemyPopCommand << file.rdbuf();
		//ファイルを閉じる
		file.close();
	}
	
	if (stageNum == 2) {
		//ファイルを開く
		std::ifstream file;
		file.open("Resources/csv/enemy2.csv");
		assert(file.is_open());
		//ファイルの内容を文字列ストリームにコピー
		enemyPopCommand << file.rdbuf();
		//ファイルを閉じる
		file.close();
	}
}

void GameScene::UpdateEnemyPopCommand()
{
	if (waitFlag) {
		waitTime_--;
		if (waitTime_ <= 0.0f) {
			//待機完了
			waitFlag = false;
		}
		return;
	}
	//1行分の文字列を入れる
	std::string line;
	//コマンド実行
	while (std::getline(enemyPopCommand, line)) {
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
		if (world.find("Enemy2") == 0) {	//移動のみ
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
			newEnemy->Initialize(&viewProjection_, &matProjection_, L"Resources/star/star.jpg");
			//上で書いてある物をEnemyの座標としてセットする
			newEnemy->Settransform(x, y, z);
			newEnemy->SetSpeed(speedX, speedY, speedZ);
			//敵を登録
			enemys2.push_back(std::move(newEnemy));
		}
		if (world.find("Enemy3") == 0) {	//移動も攻撃もしない
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
			newEnemy->Initialize(&viewProjection_, &matProjection_, L"Resources/star/star.jpg");
			//上で書いてある物をEnemyの座標としてセットする
			newEnemy->Settransform(x, y, z);
			newEnemy->SetSpeed(speedX, speedY, speedZ);
			//敵を登録
			enemys3.push_back(std::move(newEnemy));
		}
		//WAITコマンド
		else if (world.find("WAIT") == 0)
		{
			std::getline(line_stream, world, ',');
			//待ち時間
			int32_t waitTime = atoi(world.c_str());
			//待ち時間
			waitFlag = true;
			waitTime_ = waitTime;
			//コマンドループを抜ける
			break;
		}
	}
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
			waitFlag = true;
			waitTime_ = waitTime;
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

void GameScene::Collision() {

	const std::list < std::unique_ptr<PlayerBullet>>& playerBullets = player->GetBullets();
	for (const std::unique_ptr<PlayerBullet>& bulletA : playerBullets) {
		if (input_.PushKey(DIK_P)) {
			//player->OnCollision();
			//bulletA->OnCollision();
			break;
		}
	}
	for (std::unique_ptr<Enemy>& enemy : enemys1) {

		//enemy-player
		if (enemy->GetWorldTransform().translation.x - player->GetPos().x < 2 &&
			-2 < enemy->GetWorldTransform().translation.x - player->GetPos().x) {
			if (enemy->GetWorldTransform().translation.y - player->GetPos().y < 3 &&
				-3 < enemy->GetWorldTransform().translation.y - player->GetPos().y) {
				if (enemy->GetWorldTransform().translation.z - player->GetPos().z < 3 &&
					-3 < enemy->GetWorldTransform().translation.z - player->GetPos().z) {

					player->OnCollision();

				}
			}
		}
		//bullet-enemy

		const std::list < std::unique_ptr<PlayerBullet>>& playerBullets = player->GetBullets();
		for (const std::unique_ptr<PlayerBullet>& bulletA : playerBullets) {
			if (input_.PushKey(DIK_P)) {
				//player->OnCollision();
				//bulletA->OnCollision();
			}
			if (enemy->GetWorldTransform().translation.x - bulletA->GetWorldTransform().translation.x < 2 &&
				-2 < enemy->GetWorldTransform().translation.x - bulletA->GetWorldTransform().translation.x) {
				if (enemy->GetWorldTransform().translation.y - bulletA->GetWorldTransform().translation.y < 2 &&
					-2 < enemy->GetWorldTransform().translation.y - bulletA->GetWorldTransform().translation.y) {
					if (enemy->GetWorldTransform().translation.z - bulletA->GetWorldTransform().translation.z < 2 &&
						-2 < enemy->GetWorldTransform().translation.z - bulletA->GetWorldTransform().translation.z) {

						bulletA->OnCollision();
						//enemy->Reset();
						//player->OnCollision();
					}
				}
			}
		}
	}
		//player-enemybullet
		for (const std::unique_ptr<EnemyBullet>& bulletB : bullets1) {

			if (player->GetPos().x - bulletB->GetWorldTransform().translation.x < 2 &&
				-2 < player->GetPos().x - bulletB->GetWorldTransform().translation.x) {
				if (player->GetPos().y - bulletB->GetWorldTransform().translation.y < 3 &&
					-3 < player->GetPos().y - bulletB->GetWorldTransform().translation.y) {
					if (player->GetPos().z - bulletB->GetWorldTransform().translation.z < 3 &&
						-3 < player->GetPos().z - bulletB->GetWorldTransform().translation.z) {

						//bulletB->OnCollision();
						//enemy->Reset();
						player->OnCollision();
					}
				}
			}
		}
		for (const std::unique_ptr<EnemyBullet>& bulletC : bullets2) {
			if (player->GetPos().x - bulletC->GetWorldTransform().translation.x < 2 &&
				-2 < player->GetPos().x - bulletC->GetWorldTransform().translation.x) {
				if (player->GetPos().y - bulletC->GetWorldTransform().translation.y < 2 &&
					-2 < player->GetPos().y - bulletC->GetWorldTransform().translation.y) {
					if (player->GetPos().z - bulletC->GetWorldTransform().translation.z < 2 &&
						-2 < player->GetPos().z - bulletC->GetWorldTransform().translation.z) {

						//bulletB->OnCollision();
						//enemy->Reset();
						player->OnCollision();
					}
				}
			}
		}

		//自機とアイテムの当たり判定
		for (const std::unique_ptr<Item>& item : items_) {
			if (player->GetPos().x - item->GetPos().x < 2 &&
				-2 < player->GetPos().x - item->GetPos().x) {
				if (player->GetPos().y - item->GetPos().y < 2 &&
					-2 < player->GetPos().y - item->GetPos().y) {
					if (player->GetPos().z - item->GetPos().z < 2 &&
						-2 < player->GetPos().z - item->GetPos().z) {

						if (item->GetIsDead() == false) {
							item->OnCollision();

							if (item->GetWeapon() == 0) {
								rhythm->SetWeapon(Weapons::Normal);
							}
							else if (item->GetWeapon() == 1) {
								rhythm->SetWeapon(Weapons::Rapid);
							}
							else if (item->GetWeapon() == 2) {
								rhythm->SetWeapon(Weapons::ThreeWay);
							}
							else if (item->GetWeapon() == 3) {
								rhythm->SetWeapon(Weapons::Explosion);
							}
							else if (item->GetWeapon() == 4) {
								rhythm->SetWeapon(Weapons::Laser);
							}

							rhythm->ItemSoundPlay(1.0f);
						}
					}
				}
			}
		}
}
