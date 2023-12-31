#include "Rhythm.h"

Rhythm::~Rhythm() {
	SoundUnload(guideSound1);
	SoundUnload(guideSound2);
	SoundUnload(guideSound3);
	SoundUnload(shotSound);
	SoundUnload(laserSound);
	SoundUnload(explosionSound);
	SoundUnload(itemSound);
	SoundUnload(missSound);
	SoundUnload(demoBGM);
	SoundUnload(stage1_1BGM);
	SoundUnload(stage1_2BGM);
	SoundUnload(stage1_bossBGM);
	delete soundManager_;
	for (int i = 0; i < circleNum; i++) {
		delete circle[i];
	}
}

void Rhythm::Initialize(ViewProjection* viewProjection, XMMATRIX* matProjection) {

	soundManager_ = new SoundManager();
	soundManager_->Initialize();
	soundState.wave = 1;
	soundState.measureCount = 0;

	for (int i = 0; i < circleNum; i++) {
		circle[i] = new GameObject3D();
		circle[i]->PreLoadModel("Resources/circle/circle.obj");
		circle[i]->PreLoadTexture(L"Resources/white1x1.png");
		circle[i]->SetViewProjection(viewProjection);
		circle[i]->SetMatProjection(matProjection);
		circle[i]->Initialize();

		circle[i]->worldTransform.scale = { 2.0f,2.0f,1.0f };
	}

	pitch = 1.0003f;

}

void Rhythm::Update(Input* input, Vector3 pos, Vector3 rot, int isDead, int stage) {

	//最初の音声再生処理
	if (stage == 1) {
		if (soundState.timer == 0 && soundState.measureCount <= 0) {
			if (soundState.wave == 1) {
				soundManager_->SoundPlayWave(soundManager_->xAudio2.Get(), stage1_1BGM, false, soundState.BGMVolume);
			}
			else if (soundState.wave == 2) {
				soundManager_->SoundPlayWave(soundManager_->xAudio2.Get(), stage1_2BGM, false, soundState.BGMVolume);
			}
		}

		if (soundState.wave == 3 && soundState.timer == 0 && soundState.measureCount % 72 == 0) {
			soundManager_->SoundPlayWave(soundManager_->xAudio2.Get(), stage1_bossBGM, false, soundState.BGMVolume);
		}
	}
	else if (stage == 2) {

	}
	else if (stage == 3) {

	}

	if (isDead == 0) {

		pitch = 1.0003f;

		//武器変更
		if (input->TriggerKey(DIK_0)) {
			soundState.weapon = Weapons::Normal;
		}
		else if (input->TriggerKey(DIK_1)) {
			soundState.weapon = Weapons::Rapid;
		}
		else if (input->TriggerKey(DIK_2)) {
			soundState.weapon = Weapons::ThreeWay;
		}
		else if (input->TriggerKey(DIK_3)) {
			soundState.weapon = Weapons::Explosion;
		}
		else if (input->TriggerKey(DIK_4)) {
			soundState.weapon = Weapons::Laser;
		}

		if (soundState.isPause == false) {
			if (soundState.measureCount >= 8 &&
				((soundState.measureCount < 72 && soundState.wave == 1) ||
				(soundState.measureCount < 80 && soundState.wave == 2) ||
					soundState.wave == 3)) {

				//オフセット設定
				if (input->TriggerKey(DIK_Q) && soundState.offset > -5.0f) {
					soundState.offset--;
				}
				else if (input->TriggerKey(DIK_E) && soundState.offset < 5.0f) {
					soundState.offset++;
				}

				//タイマーが一周したらレーザー発射可能に
				if (soundState.timer == 30.0f) {
					if (soundState.isLaserActive == 0) {
						soundState.isLaserActive = 1;
					}
					else {
						soundState.isLaserActive = 0;
					}
				}

				soundState.isFireSucces = 0;

				//通常弾のリズム
				if (soundState.weapon == Weapons::Normal) {
					NormalShot(soundState, input);
					shrinkSpeed = { 0.1f,0.1f,0.0f };
					circle[1]->color.z -= 0.05f;
				}
				//速射弾のリズム
				else if (soundState.weapon == Weapons::Rapid) {
					RapidShot(soundState, input);
					shrinkSpeed = { 0.2f,0.2f,0.0f };
					circle[1]->color.z -= 0.1f;
				}
				//3way弾のリズム
				else if (soundState.weapon == Weapons::ThreeWay) {
					ThreeWayShot(soundState, input);
					shrinkSpeed = { 0.15f,0.15f,0.0f };
					circle[1]->color.z -= 0.075f;
				}
				//爆裂弾のリズム
				else if (soundState.weapon == Weapons::Explosion) {
					shrinkSpeed = { 0.05f,0.05f,0.0f };
					circle[1]->color.z -= 0.025f;
					ExplosionShot(soundState, input);
				}
				//レーザー弾のリズム
				else if (soundState.weapon == Weapons::Laser) {
					shrinkSpeed = { 0.0f,0.0f,0.0f };
					circle[1]->color.z -= 0.025f;
					LaserShot(soundState, input);
				}

				//UIの円を徐々に小さくする
				circle[1]->worldTransform.scale -= shrinkSpeed;

			}

			//タイマーを増やす
			soundState.timer++;

			//タイマーが最大までいったら0にし、小節を1進める
			if (soundState.timer >= maxTimer) {
				soundState.timer = 0;
				soundState.measureCount++;
			}

			//小節が一定までいったらリセット
			if (soundState.wave == 1 && soundState.measureCount >= 80) {
				soundState.measureCount = 0;
				soundState.wave++;
			}
			else if (soundState.wave == 2 && soundState.measureCount >= 90) {
				soundState.measureCount = 0;
				soundState.wave++;
			}
		}

		//一時停止
		if (input->TriggerKey(DIK_P)) {

			if (soundState.isPause == 0) {
				soundState.isPause = 1;

				if (soundState.wave == 1) {
					stage1_1BGM.pSourceVoice->Stop(0);
				}
				else if (soundState.wave == 2) {
					stage1_2BGM.pSourceVoice->Stop(0);
				}
				else if (soundState.wave == 3) {
					stage1_bossBGM.pSourceVoice->Stop(0);
				}
			}
			else {
				soundState.isPause = 0;

				if (soundState.wave == 1) {
					stage1_1BGM.pSourceVoice->Start();
				}else if(soundState.wave == 2) {
					stage1_2BGM.pSourceVoice->Start();
				}else if (soundState.wave == 3) {
					stage1_bossBGM.pSourceVoice->Start();
				}

			}
		}

		//音量調節
		//BGM
		if (input->PushKey(DIK_UP) && soundState.BGMVolume < 2.0f) {
			soundState.BGMVolume += 0.01;
		}
		if (input->PushKey(DIK_DOWN) && soundState.BGMVolume >= 0.0f) {
			soundState.BGMVolume -= 0.01;
			if (soundState.BGMVolume <= 0.0f) {
				soundState.BGMVolume = 0;
			}
		}
		//SE
		if (input->PushKey(DIK_RIGHT) && soundState.guideSEVolume < 2.0f) {
			soundState.guideSEVolume += 0.01;
		}
		if (input->PushKey(DIK_LEFT) && soundState.guideSEVolume >= 0.0f) {
			soundState.guideSEVolume -= 0.01;
			if (soundState.guideSEVolume <= 0.0f) {
				soundState.guideSEVolume = 0;
			}
		}


		//demoBGM.pSourceVoice->SetVolume(BGMVolume);
		//stage1BGM.pSourceVoice->SetVolume(soundState.BGMVolume);

		//UI(円)の更新
		if (input->TriggerKey(DIK_SPACE)) {
			colorChange = 0.0f;	
		}

		colorChange += 0.05f;

		if (soundState.judge == Judge::Good) {
			circle[0]->color = { 1.0f,1.0f,colorChange,0.0f };
		}
		else if (soundState.judge == Judge::Miss) {
			circle[0]->color = { 1.0f,colorChange,colorChange,0.0f };
		}

		for (int i = 0; i < circleNum; i++) {
			circle[i]->worldTransform.translation = pos;
			circle[i]->worldTransform.rotation = rot;
			circle[i]->Update();
		}
	}
	else {
	//死亡時、ピッチを段々下げる
		pitch -= 0.01;
	}

	if (soundState.wave == 1) {
		if (stage1_1BGM.pSourceVoice) {
			stage1_1BGM.pSourceVoice->SetFrequencyRatio(pitch);
			stage1_1BGM.pSourceVoice->SetVolume(soundState.BGMVolume);
		}
	}
	else if (soundState.wave == 2) {
		if (stage1_2BGM.pSourceVoice) {
			stage1_2BGM.pSourceVoice->SetFrequencyRatio(pitch);
			stage1_2BGM.pSourceVoice->SetVolume(soundState.BGMVolume);
		}
	}
	else if (soundState.wave == 3) {
		if (stage1_bossBGM.pSourceVoice) {
			stage1_bossBGM.pSourceVoice->SetFrequencyRatio(pitch);
			stage1_bossBGM.pSourceVoice->SetVolume(soundState.BGMVolume);
		}
	}

}

void Rhythm::Draw(int isDead) {
	if (isDead == 0) {
		for (int i = 0; i < circleNum; i++) {
			circle[i]->Draw();
		}
	}
}

void Rhythm::SoundPlayWave(SoundData soundData, float volume) {
	soundManager_->StopWave(soundData);
	soundManager_->SoundPlayWave(soundManager_->xAudio2.Get(), soundData, false, volume);
}

void Rhythm::SoundUnload(SoundData soundData) {
	soundManager_->StopWave(soundData);
	soundManager_->SoundUnload(soundData);
}

//通常弾
void Rhythm::NormalShot(SoundState s, Input* input) {
	shotTiming1 = 30.0f;	//弾発射タイミング1
	reloadTiming = 15.0f;	//弾が装填されるタイミング

	//一定のタイミングで音を鳴らす
	if (s.timer == 0.0f || s.timer == shotTiming1) {
		circle[1]->worldTransform.scale = { 5.0f,5.0f,0.0f };
		circle[1]->color.z = 1.0f;
		SoundPlayWave(guideSound2, s.guideSEVolume);
	}
	//一定のタイミングで弾を装填する
	if (s.timer == reloadTiming || s.timer == shotTiming1 + reloadTiming) {
		soundState.isFireActive = 1;
	}

	//弾が発射可能なとき
	if (soundState.isFireActive == 1) {
		if (input->TriggerKey(DIK_SPACE)) {
			//丁度いいタイミングでキーを押すと発射
			if (((s.timer <= judgeBreadth + s.offset) ||
				(s.timer >= shotTiming1 - judgeBreadth + s.offset && s.timer <= shotTiming1 + judgeBreadth + s.offset) ||
				(s.timer >= maxTimer - judgeBreadth + s.offset))) {

				soundState.isFireSucces = 1;
				soundState.judge = Judge::Good;
				soundState.combo++;
				SoundPlayWave(shotSound, s.normalSEVolume);
			}
			//それ以外のタイミングは失敗
			else {
				soundState.judge = Judge::Miss;
				soundState.combo = 0;
				SoundPlayWave(missSound, s.normalSEVolume);
			}

			soundState.isFireActive = 0;
		}
	}
}

//速射弾
void Rhythm::RapidShot(SoundState s, Input* input) {
	shotTiming1 = 15.0f;	//弾発射タイミング1
	shotTiming2 = 30.0f;	//弾発射タイミング2
	shotTiming3 = 45.0f;	//弾発射タイミング3
	reloadTiming = 7.0f;	//弾が装填されるタイミング

	//一定のタイミングで音を鳴らす
	if (s.timer == 0.0f || s.timer == shotTiming1 || s.timer == shotTiming2 || s.timer == shotTiming3) {
		SoundPlayWave(guideSound2, s.guideSEVolume);
		circle[1]->worldTransform.scale = { 5.0f,5.0f,0.0f };
		circle[1]->color.z = 1.0f;
	}
	//一定のタイミングで弾を装填する
	if (s.timer == reloadTiming || s.timer == shotTiming1 + reloadTiming ||
		s.timer == shotTiming2 + reloadTiming || s.timer == shotTiming3 + reloadTiming) {
		soundState.isFireActive = 1;
	}

	//弾が発射可能なとき
	if (soundState.isFireActive == 1) {
		if (input->TriggerKey(DIK_SPACE)) {
			//丁度いいタイミングでキーを押すと発射
			if (((s.timer <= judgeBreadth + s.offset) ||
				(s.timer >= shotTiming1 - judgeBreadth + s.offset && s.timer <= shotTiming1 + judgeBreadth + s.offset) ||
				(s.timer >= shotTiming2 - judgeBreadth + s.offset && s.timer <= shotTiming2 + judgeBreadth + s.offset) ||
				(s.timer >= shotTiming3 - judgeBreadth + s.offset && s.timer <= shotTiming3 + judgeBreadth + s.offset) ||
				(s.timer >= maxTimer - judgeBreadth + s.offset))) {

				soundState.isFireSucces = 1;
				soundState.judge = Judge::Good;
				SoundPlayWave(shotSound, s.normalSEVolume);
			}
			//それ以外のタイミングは失敗
			else {
				soundState.judge = Judge::Miss;
				SoundPlayWave(missSound, s.normalSEVolume);
			}

			soundState.isFireActive = 0;
		}
	}
}

//3way弾
void Rhythm::ThreeWayShot(SoundState s, Input* input) {
	shotTiming1 = 20.0f;	//弾発射タイミング1
	shotTiming2 = 40.0f;	//弾発射タイミング2
	reloadTiming = 10.0f;	//弾が装填されるタイミング

	//一定のタイミングで音を鳴らす
	if (s.timer == 0.0f ||s. timer == shotTiming1 || s.timer == shotTiming2) {
		circle[1]->worldTransform.scale = { 5.0f,5.0f,0.0f };
		circle[1]->color.z = 1.0f;
		SoundPlayWave(guideSound2, s.guideSEVolume);

	}
	//一定のタイミングで弾を装填する
	if (s.timer == reloadTiming || s.timer == shotTiming1 + reloadTiming || s.timer == shotTiming2 + reloadTiming) {
		soundState.isFireActive = 1;
	}

	//弾が発射可能なとき
	if (soundState.isFireActive == 1) {
		if (input->TriggerKey(DIK_SPACE)) {
			//丁度いいタイミングでキーを押すと発射
			if (((s.timer <= judgeBreadth + s.offset) ||
				(s.timer >= shotTiming1 - judgeBreadth + s.offset && s.timer <= shotTiming1 + judgeBreadth + s.offset) ||
				(s.timer >= shotTiming2 - judgeBreadth + s.offset && s.timer <= shotTiming2 + judgeBreadth + s.offset) ||
				(s.timer >= maxTimer - judgeBreadth + s.offset))) {

				soundState.isFireSucces = 1;
				soundState.judge = Judge::Good;
				SoundPlayWave(shotSound, s.normalSEVolume);
			}
			//それ以外のタイミングは失敗
			else {
				soundState.judge = Judge::Miss;
				SoundPlayWave(missSound, s.normalSEVolume);
			}

			soundState.isFireActive = 0;

		}
	}
}

//爆裂弾
void Rhythm::ExplosionShot(SoundState s, Input* input) {
	shotTiming1 = 30.0f;	//弾発射タイミング1(爆破タイミング)
	reloadTiming = 45.0f;	//弾が装填されるタイミング

	//一定のタイミングで音を鳴らす
	if (s.timer == 0.0f) {
		circle[1]->worldTransform.scale = { 5.0f,5.0f,0.0f };
		circle[1]->color.z = 1.0f;
		SoundPlayWave(guideSound2, s.guideSEVolume);
	}
	//一定のタイミングで弾を装填する
	if (s.timer == reloadTiming) {
		isSuccess = 0;
		soundState.isFireActive = 1;
	}

	//弾が発射可能なとき
	if (soundState.isFireActive == 1) {
		if (input->TriggerKey(DIK_SPACE)) {
			//丁度いいタイミングでキーを押すと発射
			if (((s.timer <= judgeBreadth + s.offset) ||
				(s.timer >= maxTimer - judgeBreadth + s.offset))) {

				soundState.isFireSucces = 1;
				isSuccess = 1;
				soundState.judge = Judge::Good;
				SoundPlayWave(shotSound, s.normalSEVolume);
			}
			//それ以外のタイミングは失敗
			else {
				soundState.judge = Judge::Miss;
				SoundPlayWave(missSound, s.normalSEVolume);
				isSuccess = 0;
			}

			soundState.isFireActive = 0;
		}
	}
	//成功したら爆破音を鳴らす
	if (isSuccess == 1 && s.timer == shotTiming1) {
		SoundPlayWave(explosionSound, s.normalSEVolume);
	}
}

//レーザー弾
void Rhythm::LaserShot(SoundState s, Input* input) {
	reloadTiming = 0.0f;	//弾が装填されるタイミング}

	//一定のタイミングで音を鳴らす
	if (s.timer == 0.0f && s.isLaserActive == 1) {
		SoundPlayWave(guideSound3, s.guideSEVolume);
	}
	//一定のタイミングで弾を装填する
	if (s.timer == reloadTiming && s.isLaserActive == 0) {
		circle[1]->worldTransform.scale = { 5.0f,5.0f,0.0f };
		circle[1]->color.z = 1.0f;
		soundState.isFireActive = 1;
	}

	//弾が発射可能なとき
	if (soundState.isFireActive == 1) {
		if (input->TriggerKey(DIK_SPACE)) {
			//丁度いいタイミングでキーを押すと発射
			if (((s.timer <= judgeBreadth + s.offset && s.isLaserActive == 1) ||
				(s.timer >= maxTimer - judgeBreadth + s.offset && s.isLaserActive == 1))) {

				soundState.isFireSucces = 1;
				soundState.judge = Judge::Good;
				soundManager_->SoundPlayWave(soundManager_->xAudio2.Get(), laserSound, false, s.normalSEVolume);

			}
			//それ以外のタイミングは失敗
			else {
				soundState.judge = Judge::Miss;
				SoundPlayWave(missSound, s.normalSEVolume);
			}

			soundState.isFireActive = 0;
		}
		shrinkSpeed = { 0.05f,0.05f,0.0f };

	}

	if (circle[1]->worldTransform.scale.x <= 0) {
		shrinkSpeed = { 0.0f,0.0f,0.0f };
	}
	//ボタンを離すと止まる
	if (input->PushKey(DIK_SPACE) == 0 && laserSound.pSourceVoice != nullptr) {
		laserSound.pSourceVoice->Stop(0);
	}

}
