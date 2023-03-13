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
	delete soundManager_;
}

void Rhythm::Initialize() {

	soundManager_ = new SoundManager();
	soundManager_->Initialize();
}

void Rhythm::Update(Input* input) {

	if (soundState.timer == 0 && soundState.measureCount <= 0) {
		soundManager_->SoundPlayWave(soundManager_->xAudio2.Get(), stage1BGM, false, soundState.BGMVolume);
		stage1BGM.pSourceVoice->SetFrequencyRatio(1.0003f);
	}

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
		if (soundState.measureCount >= 8) {

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
				if (input->PushKey(DIK_SPACE)) {
					//soundState.isActive = 0;
				}
			}
			//速射弾のリズム
			else if (soundState.weapon == Weapons::Rapid) {
				RapidShot(soundState, input);
			}
			//3way弾のリズム
			else if (soundState.weapon == Weapons::ThreeWay) {
				ThreeWayShot(soundState, input);
			}
			//爆裂弾のリズム
			else if (soundState.weapon == Weapons::Explosion) {
				ExplosionShot(soundState, input);
			}
			//レーザー弾のリズム
			else if (soundState.weapon == Weapons::Laser) {
				LaserShot(soundState, input);
			}

			//曲が一周したら最初から再生(仮)
			if (soundState.measureCount == 0 && soundState.timer == 0) {
				//SoundPlayWave(mainBGM);
				//soundManager_->SoundPlayWave(soundManager_->xAudio2.Get(), demoBGM, false, BGMVolume);
			}

			//目安として2小節目で大きめの音を鳴らす
			if (soundState.timer == maxTimer / 2) {
				//SoundPlayWave(guideSound1, BGMVolume);
			}
		}

		//タイマーを増やす
		soundState.timer++;

		//タイマーが最大までいったら0にし、小節を1進める
		if (soundState.timer >= maxTimer) {
			soundState.timer = 0;
			soundState.measureCount++;
		}

		//小節が一定までいったら曲を最初からにする(仮)
		if (soundState.measureCount >= 16) {
			//soundState.measureCount = 0;
		}
	}

	//一時停止
	if (input->TriggerKey(DIK_P)) {

		if (soundState.isPause == 0) {
			soundState.isPause = 1;
			//demoBGM.pSourceVoice->Stop(0);
			stage1BGM.pSourceVoice->Stop(0);
		}
		else {
			soundState.isPause = 0;
			//demoBGM.pSourceVoice->Start();
			stage1BGM.pSourceVoice->Start();

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
	stage1BGM.pSourceVoice->SetVolume(soundState.BGMVolume);
}

void Rhythm::SoundPlayWave(SoundData soundData, float volume) {
	soundManager_->StopWave(soundData);
	soundManager_->SoundPlayWave(soundManager_->xAudio2.Get(), soundData, false, volume);
}

void Rhythm::SoundUnload(SoundData soundData) {
	soundManager_->StopWave(soundData);
	soundManager_->SoundUnload(soundData);
}

void Rhythm::ItemSoundPlay(float volume) {
	soundManager_->StopWave(itemSound);
	soundManager_->SoundPlayWave(soundManager_->xAudio2.Get(), itemSound, false, volume);
}

//通常弾
void Rhythm::NormalShot(SoundState s, Input* input) {
	shotTiming1 = 30.0f;	//弾発射タイミング1
	reloadTiming = 15.0f;	//弾が装填されるタイミング

	//一定のタイミングで音を鳴らす
	if (s.timer == 0.0f || s.timer == shotTiming1) {
		SoundPlayWave(guideSound2, s.guideSEVolume);
	}
	//一定のタイミングで弾を装填する
	if (s.timer == reloadTiming || s.timer == shotTiming1 + reloadTiming) {
		isFireActive = 1;
	}

	//弾が発射可能なとき
	if (isFireActive == 1) {
		if (input->TriggerKey(DIK_SPACE)) {
			//丁度いいタイミングでキーを押すと発射
			if (((s.timer <= judgeBreadth + s.offset) ||
				(s.timer >= shotTiming1 - judgeBreadth + s.offset && s.timer <= shotTiming1 + judgeBreadth + s.offset) ||
				(s.timer >= maxTimer - judgeBreadth + s.offset))) {

				soundState.isFireSucces = 1;

				SoundPlayWave(shotSound, s.normalSEVolume);
			}
			//それ以外のタイミングは失敗
			else {
				SoundPlayWave(missSound, s.normalSEVolume);
			}

			isFireActive = 0;
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
	}
	//一定のタイミングで弾を装填する
	if (s.timer == reloadTiming || s.timer == shotTiming1 + reloadTiming ||
		s.timer == shotTiming2 + reloadTiming || s.timer == shotTiming3 + reloadTiming) {
		isFireActive = 1;
	}

	//弾が発射可能なとき
	if (isFireActive == 1) {
		if (input->TriggerKey(DIK_SPACE)) {
			//丁度いいタイミングでキーを押すと発射
			if (((s.timer <= judgeBreadth + s.offset) ||
				(s.timer >= shotTiming1 - judgeBreadth + s.offset && s.timer <= shotTiming1 + judgeBreadth + s.offset) ||
				(s.timer >= shotTiming2 - judgeBreadth + s.offset && s.timer <= shotTiming2 + judgeBreadth + s.offset) ||
				(s.timer >= shotTiming3 - judgeBreadth + s.offset && s.timer <= shotTiming3 + judgeBreadth + s.offset) ||
				(s.timer >= maxTimer - judgeBreadth + s.offset))) {

				soundState.isFireSucces = 1;

				SoundPlayWave(shotSound, s.normalSEVolume);
			}
			//それ以外のタイミングは失敗
			else {
				SoundPlayWave(missSound, s.normalSEVolume);
			}

			isFireActive = 0;
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
		SoundPlayWave(guideSound2, s.guideSEVolume);

	}
	//一定のタイミングで弾を装填する
	if (s.timer == reloadTiming || s.timer == shotTiming1 + reloadTiming || s.timer == shotTiming2 + reloadTiming) {
		isFireActive = 1;
	}

	//弾が発射可能なとき
	if (isFireActive == 1) {
		if (input->TriggerKey(DIK_SPACE)) {
			//丁度いいタイミングでキーを押すと発射
			if (((s.timer <= judgeBreadth + s.offset) ||
				(s.timer >= shotTiming1 - judgeBreadth + s.offset && s.timer <= shotTiming1 + judgeBreadth + s.offset) ||
				(s.timer >= shotTiming2 - judgeBreadth + s.offset && s.timer <= shotTiming2 + judgeBreadth + s.offset) ||
				(s.timer >= maxTimer - judgeBreadth + s.offset))) {

				soundState.isFireSucces = 1;

				SoundPlayWave(shotSound, s.normalSEVolume);
			}
			//それ以外のタイミングは失敗
			else {
				SoundPlayWave(missSound, s.normalSEVolume);
			}

			isFireActive = 0;

		}
	}
}

//爆裂弾
void Rhythm::ExplosionShot(SoundState s, Input* input) {
	shotTiming1 = 30.0f;	//弾発射タイミング1(爆破タイミング)
	reloadTiming = 45.0f;	//弾が装填されるタイミング

	//一定のタイミングで音を鳴らす
	if (s.timer == 0.0f) {
		SoundPlayWave(guideSound2, s.guideSEVolume);
	}
	//一定のタイミングで弾を装填する
	if (s.timer == reloadTiming) {
		isSuccess = 0;
		isFireActive = 1;
	}

	//弾が発射可能なとき
	if (isFireActive == 1) {
		if (input->TriggerKey(DIK_SPACE)) {
			//丁度いいタイミングでキーを押すと発射
			if (((s.timer <= judgeBreadth + s.offset) ||
				(s.timer >= maxTimer - judgeBreadth + s.offset))) {

				soundState.isFireSucces = 1;
				isSuccess = 1;

				SoundPlayWave(shotSound, s.normalSEVolume);
			}
			//それ以外のタイミングは失敗
			else {
				SoundPlayWave(missSound, s.normalSEVolume);
				isSuccess = 0;
			}

			isFireActive = 0;
		}
	}
	//成功したら爆破音を鳴らす
	if (isSuccess == 1 && s.timer == shotTiming1) {
		SoundPlayWave(explosionSound, s.normalSEVolume);
	}
}

//レーザー弾
void Rhythm::LaserShot(SoundState s, Input* input) {
	reloadTiming = 0.0f;	//弾が装填されるタイミング

	//一定のタイミングで音を鳴らす
	if (s.timer == 0.0f && s.isLaserActive == 1) {
		SoundPlayWave(guideSound3, s.guideSEVolume);
	}
	//一定のタイミングで弾を装填する
	if (s.timer == reloadTiming && s.isLaserActive == 0) {
		isFireActive = 1;
	}

	//弾が発射可能なとき
	if (isFireActive == 1) {
		if (input->TriggerKey(DIK_SPACE)) {
			//丁度いいタイミングでキーを押すと発射
			if (((s.timer <= judgeBreadth + s.offset && s.isLaserActive == 1) ||
				(s.timer >= maxTimer - judgeBreadth + s.offset && s.isLaserActive == 1))) {

				soundState.isFireSucces = 1;

				soundManager_->SoundPlayWave(soundManager_->xAudio2.Get(), laserSound, false, s.normalSEVolume);

			}
			//それ以外のタイミングは失敗
			else {
				SoundPlayWave(missSound, s.normalSEVolume);
			}

			isFireActive = 0;
		}
	}

	//ボタンを離すと止まる
	if (input->PushKey(DIK_SPACE) == 0 && laserSound.pSourceVoice != nullptr) {
		laserSound.pSourceVoice->Stop(0);
	}

}
