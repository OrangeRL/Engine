#pragma once
#include "Input.h"
#include "Audio.h"

enum class Weapons {
	Normal,		//通常弾
	Rapid,		//速射弾
	ThreeWay,	//3way弾
	Explosion,	//爆裂弾
	Laser,		//レーザー弾
};

class Rhythm
{
public:

	~Rhythm();

	void Initialize();

	void Update(Input* input);

	void SoundUnload(SoundData soundData);

	void SoundPlayWave(SoundData soundData, float volume);

	void SetWeapon(Weapons weapon) { this->weapon = weapon; }

private:
	//一小節の時間
	const float maxTimer = 60.0f;
	//判定の広さ
	const float judgeBreadth = 3.0f;

	//サウンドマネージャー
	SoundManager* soundManager_ = nullptr;

	//音声読み込み
	SoundData guideSound1 = soundManager_->SoundLoadWave("Resources/rhythm2.wav");
	SoundData guideSound2 = soundManager_->SoundLoadWave("Resources/rhythm3.wav");
	SoundData guideSound3 = soundManager_->SoundLoadWave("Resources/rhythm4.wav");
	SoundData shotSound = soundManager_->SoundLoadWave("Resources/shot2.wav");
	SoundData missSound = soundManager_->SoundLoadWave("Resources/death.wav");
	SoundData laserSound = soundManager_->SoundLoadWave("Resources/shot4.wav");
	SoundData explosionSound = soundManager_->SoundLoadWave("Resources/shot3.wav");
	SoundData mainBGM = soundManager_->SoundLoadWave("Resources/demo.wav");

	//タイマー
	float timer = 0.0f;
	//小節カウント(仮)
	int measureCount = 0;
	//次の弾が撃てるまでのクールタイムのフラグ
	bool isActive = 1;
	//一時停止中か
	bool isPause = 0;
	//武器の種類
	Weapons weapon = Weapons::Normal;
	//爆裂弾の発射が成功したかどうか(仮)
	int isSuccess = 0;
	//レーザー弾の発射可能フラグ
	bool isLaserActive = 0;
	//弾発射タイミング1
	float shotTiming1;
	//弾発射タイミング2
	float shotTiming2;
	//弾発射タイミング3
	float shotTiming3;
	//弾が装填されるタイミング
	float reloadTiming;
	//判定のずれ(オフセット)
	float offset = 0.0f;
	//BGM音量
	float BGMVolume = 1.0f;
	//ノーマルSE音量
	float normalSEVolume = 1.0f;
	//ガイドSE音量
	float guideSEVolume = 1.0f;
};