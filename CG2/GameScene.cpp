#include "GameScene.h"
#include <cassert>


GameScene::GameScene() {

}

GameScene::~GameScene() {
	
	soundManager_.SoundUnload(soundData1);
	soundManager_.SoundUnload(selectSound);
}

void GameScene::Initialize(WinApp* winApp) {

	//透視投影変換行列の計算
	matProjection_ = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(45.0) ,
		(float)winApp->window_width / winApp->window_height ,
		0.1f , 1000.0f
	);

	viewProjection_.Initialize();
	viewProjection_.eye = {0 , 0 , -100};

	player = new Player();
	player->Initialize(&viewProjection_, &matProjection_);
	loadEnemyPopData();
}

void GameScene::Update() {
	
	viewProjection_.UpdateView();
	//シーン管理
	player->Update();
	for (std::unique_ptr<Enemy>& enemy : enemys1) {
		enemy->Update();
	}
	for (std::unique_ptr<Enemy>& enemy : enemys2) {
		enemy->Update();
	}
	UpdateEnemyPopCommand();
	
}

void GameScene::Draw() {
	//3D描画
	player->Draw();
	for (std::unique_ptr<Enemy>& enemy : enemys1) {
		enemy->Draw();
	}
	for (std::unique_ptr<Enemy>& enemy : enemys2) {
		enemy->Draw();
	}
	//スプライト描画
	Sprite::PreDraw(dx12base_.GetCmdList().Get());


	Sprite::PostDraw();

}

void GameScene::Reset() {


}

void GameScene::loadEnemyPopData()
{
	//ファイルを開く
	std::ifstream file;
	file.open("Resources/enemyPop.csv");
	assert(file.is_open());
	//ファイルの内容を文字列ストリームにコピー
	enemyPopCommand << file.rdbuf();
	//ファイルを閉じる
	file.close();
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
	while (std::getline(enemyPopCommand,line)){
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
		if (world.find("POP_E1") == 0){
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

			//敵を発生させる
			//-------ここにEnemy発生関数---------//
			//複数化するためにuniq_ptrに変更
			std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
			newEnemy->Initialize(&viewProjection_, &matProjection_,L"Resources/white1x1.png");
			//上で書いてある物をEnemyの座標としてセットする
			newEnemy->Settransform(x, y, z);
			//敵を登録
			enemys1.push_back(std::move(newEnemy));
		}
		if (world.find("POP_E2") == 0) {
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
			float speed = (float)std::atof(world.c_str());
			//敵を発生させる
			//-------ここにEnemy発生関数---------//
			//複数化するためにuniq_ptrに変更
			std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
			newEnemy->Initialize(&viewProjection_, &matProjection_, L"Resources/e.png");
			//上で書いてある物をEnemyの座標としてセットする
			newEnemy->Settransform(x, y, z);
			newEnemy->SetSpeed(speed);
			//敵を登録
			enemys2.push_back(std::move(newEnemy));
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
