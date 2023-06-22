#include "GameScene.h"
#include <cassert>
#include <chrono>
#include <thread>
#include <json.hpp>
#include <fstream>

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
	reilCamera->Initialize({ 0,0,-50 }, { 0,0,0 }, winApp);

	//アイテム
	//item = new Item();
	//item->Initialize(&viewProjection_, &matProjection_, L"Resources/white1x1.png", {0.0f,0.0f,50.0f},2);

	player = new Player();
	player->Initialize(&viewProjection_, &matProjection_);
	player->SetPos({ 0.0f, 0.0f, 20.0f });

	viewProjection_ = reilCamera->GetViewProjection();

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

	//std::map<std::string, Model*> models;
	//std::vector<FbxObject3D*> objects;
	//連結してフルパスを得る
	const std::string fullpath = std::string("Resources/levels/") + "untitled.json";
	//ファイルストリーム
	std::ifstream file;

	//ファイルを開く
	file.open(fullpath);
	//ファイルオープン失敗をチェック
	if (file.fail())
	{
		assert(0);
	}

	//JSON文字列から解凍したデータ
	nlohmann::json deserialized;

	//解凍
	file >> deserialized;

	//正しいレベルデータファイルかチェック
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	//"name"を文字列として取得
	std::string name = deserialized["name"].get<std::string>();
	//正しいレベルデータファイルかチェック
	assert(name.compare("scene") == 0);

	//レベルデータ格納用インスタンスを生成
	levelData = new LevelData();
	//"objects"の全オブジェクトを走査
	for (nlohmann::json& object : deserialized["objects"])
	{
		assert(object.contains("type"));
		//種別を取得
		std::string type = object["type"].get<std::string>();

		//MESH
		if (type.compare("MESH") == 0)
		{
			//要素追加
			levelData->objects.emplace_back(LevelData::ObjectData{});
			//今追加した要素の参照を得る
			LevelData::ObjectData& objectData = levelData->objects.back();
			if (object.contains("file_name"))
			{
				//ファイル名
				objectData.fileName = object["file_name"];
			}
			//トランスフォームのパラメータ読み込み
			nlohmann::json& transform = object["transform"];
			//平行移動
			objectData.translation.m128_f32[0] = (float)transform["translation"][1];
			objectData.translation.m128_f32[1] = (float)transform["translation"][2];
			objectData.translation.m128_f32[2] = -(float)transform["translation"][0];
			objectData.translation.m128_f32[3] = 1.0f;
			//回転角
			objectData.rotation.m128_f32[0] = -(float)transform["rotation"][1];
			objectData.rotation.m128_f32[1] = -(float)transform["rotation"][2];
			objectData.rotation.m128_f32[2] = (float)transform["rotation"][0];
			objectData.rotation.m128_f32[3] = 0.0f;
			//スケーリング
			objectData.scaling.m128_f32[0] = (float)transform["scaling"][1];
			objectData.scaling.m128_f32[1] = (float)transform["scaling"][2];
			objectData.scaling.m128_f32[2] = (float)transform["scaling"][0];
			objectData.scaling.m128_f32[3] = 0.0f;
			//TODO:コライダーのパラメータ読み込み

			if (object.contains("children"))
			{

			}
		}

		////レベルデータに出現するモデルの読み込み　
		//for (auto& objectData : levelData->objects)
		//{
		//	decltype(models)::iterator it = models.find(objectData.fileName);
		//	if (it != models.end())
		//	{
		//		FbxModel* model = FbxLoader::GetInstance()->LoadModelFromFile(objectData.fileName);
		//		models[objectData.fileName] = model;
		//	}
		//}

		//レベルデータからオブジェクトを生成、配置
		for (auto& objectData : levelData->objects) {
		
			//3Dオブジェクトを生成
			FbxObject3D* newObject = new FbxObject3D;

			decltype(models)::iterator it = models.find(objectData.fileName);
			if (it != models.end())
			{
				FbxModel* model = FbxLoader::GetInstance()->LoadModelFromFile(objectData.fileName);
				models[objectData.fileName] = model;
				newObject->SetModel(model);
			}
			
			//座標
			DirectX::XMFLOAT3 pos;
			DirectX::XMStoreFloat3(&pos,objectData.translation);
			newObject->SetPosition(pos);
			//回転角
			DirectX::XMFLOAT3 rot;
			DirectX::XMStoreFloat3(&rot,objectData.rotation);
			newObject->SetRotation(rot);
			//座標
			DirectX::XMFLOAT3 scale;
			DirectX::XMStoreFloat3(&scale,objectData.scaling);
			newObject->SetScale(scale);

			
			newObject->Initialize();

			//配列に登録
			objects.push_back(newObject);
		}

		}
	}

	void GameScene::Update()
	{
		for (FbxObject3D* object : objects)
		{
			//object->Update();
		}
		std::default_random_engine engine(seed_gen());

		viewProjection_ = reilCamera->GetViewProjection();

		viewProjection_.UpdateView();

		player->Update(reilCamera->GetWorldTransform(), reilCamera->GetWorldTransform().rotation);

		skydome->Update();

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
		//プレイヤー描画


		//rhythm->Draw(player->GetIsDead());
		//particle->Draw();
		//particle2->Draw();

		/*for (int i = 0; i < enemyVal; i++) {
			pManager[i].Draw();
		}*/
		//敵の描画
		//for (std::unique_ptr<Enemy>& enemy : enemys1) {
		//	enemy->Draw();
		//}

		//if (rhythm->GetSoundState().wave == 3) {
		//	boss->Draw();
		//	for (std::unique_ptr<BossBullet>& bullet : bossBullet1) {
		//		bullet->Draw();
		//	}

		//	for (std::unique_ptr<BossBullet>& bullet : bossBullet2) {
		//		bullet->Draw();
		//	}
		//}

		//for (std::unique_ptr<EnemyBullet>& bullet : bullets1) {
		//	bullet->Draw();
		//}
		//for (std::unique_ptr<EnemyBullet>& bullet : bullets2) {
		//	bullet->Draw();
		//}
		//int count = 0;
		//for (auto& objectData : levelData->objects)
		//{
		//	//ファイル名から登録済みモデルを検索
		//	Model* model = nullptr;
		//	decltype(models)::iterator it = models.find(objectData.fileName);
		//	if (it != models.end()) { model = it->second; }

		//	model->Draw(objects[i],);

		//	objects.push_back(newObject);
		//	count++;
		//}
		player->Draw();
		//アイテム描画
		//for (std::unique_ptr<Item>& item : items_) { item->Draw(); }
		skydome->Draw();
		object1->Draw(dx12base_.GetCmdList().Get());
		//スプライト描画
		Sprite::PreDraw(dx12base_.GetCmdList().Get());

		//crosshair->Draw();
		//
		//UIManager.Draw(rhythm);

		// デバッグテキストの描画
		debugText.DrawAll(dx12base_.GetCmdList().Get());

		Sprite::PostDraw();

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
		for (std::unique_ptr<Enemy>& enemy : enemys1) {
			if (enemy->GetWorldTransform().translation.x - player->GetAimPos().x < 1 &&
				-1 < enemy->GetWorldTransform().translation.x - player->GetAimPos().x) {
				if (enemy->GetWorldTransform().translation.y - player->GetAimPos().y < 1 &&
					-1 < enemy->GetWorldTransform().translation.y - player->GetAimPos().y) {
					if (enemy->GetWorldTransform().translation.z - player->GetAimPos().z < 100 &&
						-100 < enemy->GetWorldTransform().translation.z - player->GetAimPos().z) {
						player->AimHit();
					}
					else
					{
						player->NotAimHit();
					}
				}
			}
		}
#pragma region PlayerToEnemyCollision
		const std::list < std::unique_ptr<PlayerBullet>>& playerBullets = player->GetBullets();
		for (std::unique_ptr<Enemy>& enemy : enemys1) {
			//enemy-player
			if (enemy->GetWorldTransform().translation.x - player->GetPos().x < 2 &&
				-2 < enemy->GetWorldTransform().translation.x - player->GetPos().x) {
				if (enemy->GetWorldTransform().translation.y - player->GetPos().y < 2 &&
					-2 < enemy->GetWorldTransform().translation.y - player->GetPos().y) {
					if (enemy->GetWorldTransform().translation.z - player->GetPos().z < 2 &&
						-2 < enemy->GetWorldTransform().translation.z - player->GetPos().z) {

						player->OnCollision(rhythm);

					}
				}
			}
#pragma endregion

#pragma region bulletToEnemyCollisions
			const std::list < std::unique_ptr<PlayerBullet>>& playerBullets = player->GetBullets();
			for (const std::unique_ptr<PlayerBullet>& bulletA : playerBullets) {
				if (enemy->GetWorldTransform().translation.x - bulletA->GetWorldTransform().translation.x < 2 &&
					-2 < enemy->GetWorldTransform().translation.x - bulletA->GetWorldTransform().translation.x) {
					if (enemy->GetWorldTransform().translation.y - bulletA->GetWorldTransform().translation.y < 2 &&
						-2 < enemy->GetWorldTransform().translation.y - bulletA->GetWorldTransform().translation.y) {
						if (enemy->GetWorldTransform().translation.z - bulletA->GetWorldTransform().translation.z < 2 &&
							-2 < enemy->GetWorldTransform().translation.z - bulletA->GetWorldTransform().translation.z) {

							bulletA->OnCollision();
							enemy->OnCollision(rhythm);
							player->NotAimHit();

						}
					}
				}
			}
		}
#pragma endregion

#pragma region bulletToEnemyBulletCollisions
		//bullet-enemyBullet
		for (const std::unique_ptr<EnemyBullet>& bulletB : bullets1) {
			for (const std::unique_ptr<PlayerBullet>& bulletA : playerBullets) {
				if (input_.PushKey(DIK_P)) {
					//player->OnCollision();
					bulletA->OnCollision();
				}
				if (bulletB->GetWorldTransform().translation.x - bulletA->GetWorldTransform().translation.x < 2 &&
					-2 < bulletB->GetWorldTransform().translation.x - bulletA->GetWorldTransform().translation.x) {
					if (bulletB->GetWorldTransform().translation.y - bulletA->GetWorldTransform().translation.y < 2 &&
						-2 < bulletB->GetWorldTransform().translation.y - bulletA->GetWorldTransform().translation.y) {
						if (bulletB->GetWorldTransform().translation.z - bulletA->GetWorldTransform().translation.z < 2 &&
							-2 < bulletB->GetWorldTransform().translation.z - bulletA->GetWorldTransform().translation.z) {

							bulletA->OnCollision();
							bulletB->OnCollision();

						}
					}
				}
			}
		}

		//bullet-enemyBullet
		for (const std::unique_ptr<EnemyBullet>& bulletB : bullets2) {
			for (const std::unique_ptr<PlayerBullet>& bulletA : playerBullets) {
				if (input_.PushKey(DIK_P)) {
					//player->OnCollision();
					bulletA->OnCollision();
				}
				if (bulletB->GetWorldTransform().translation.x - bulletA->GetWorldTransform().translation.x < 2 &&
					-2 < bulletB->GetWorldTransform().translation.x - bulletA->GetWorldTransform().translation.x) {
					if (bulletB->GetWorldTransform().translation.y - bulletA->GetWorldTransform().translation.y < 2 &&
						-2 < bulletB->GetWorldTransform().translation.y - bulletA->GetWorldTransform().translation.y) {
						if (bulletB->GetWorldTransform().translation.z - bulletA->GetWorldTransform().translation.z < 2 &&
							-2 < bulletB->GetWorldTransform().translation.z - bulletA->GetWorldTransform().translation.z) {

							bulletA->OnCollision();
							bulletB->OnCollision();

						}
					}
				}
			}
		}
#pragma endregion

#pragma region enemyBulletToPlayerCollisions
		//player-enemybullet
		for (const std::unique_ptr<EnemyBullet>& bulletB : bullets1) {

			if (player->GetPos().x - bulletB->GetWorldTransform().translation.x < 2 &&
				-2 < player->GetPos().x - bulletB->GetWorldTransform().translation.x) {
				if (player->GetPos().y - bulletB->GetWorldTransform().translation.y < 2 &&
					-2 < player->GetPos().y - bulletB->GetWorldTransform().translation.y) {
					if (player->GetPos().z - bulletB->GetWorldTransform().translation.z < 2 &&
						-2 < player->GetPos().z - bulletB->GetWorldTransform().translation.z) {

						//bulletB->OnCollision();
						//enemy->Reset();
						player->OnCollision(rhythm);
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
						player->OnCollision(rhythm);
					}
				}
			}
		}
#pragma endregion

		const std::list < std::unique_ptr<Pattern2>>& playerAim = player->GetAim();

		for (std::unique_ptr<Enemy>& enemy : enemys1) {
			for (const std::unique_ptr<Pattern2>& bulletA : playerAim) {
				if (enemy->GetWorldTransform().translation.x - bulletA->GetWorldTransform().translation.x < 4 &&
					-4 < enemy->GetWorldTransform().translation.x - bulletA->GetWorldTransform().translation.x) {
					if (enemy->GetWorldTransform().translation.y - bulletA->GetWorldTransform().translation.y < 4 &&
						-4 < enemy->GetWorldTransform().translation.y - bulletA->GetWorldTransform().translation.y) {
						if (enemy->GetWorldTransform().translation.z - bulletA->GetWorldTransform().translation.z < 5 &&
							-5 < enemy->GetWorldTransform().translation.z - bulletA->GetWorldTransform().translation.z) {

							bulletA->OnCollision();
						}
					}
				}
			}
		}

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