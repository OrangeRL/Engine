#pragma once
#include "Input.h"
#include "Audio.h"
#include "GameObject3D.h"

//武器の種類
enum class Weapons {
	Normal,		//通常弾
	Rapid,		//速射弾
	ThreeWay,	//3way弾
	Explosion,	//爆裂弾
	Laser,		//レーザー弾
};

//判定
enum class Judge {
	None,
	Miss,
	Good,
};

struct SoundState {
	//タイマー
	float timer = 0.0f;
	//小節カウント(仮)
	int measureCount = 0;
	//コンボ
	int combo = 0;

	//一時停止中か
	bool isPause = 0;
	//武器の種類
	Weapons weapon = Weapons::Normal;
	//発射成功したか
	bool isFireSucces;
	//次の弾が撃てるまでのクールタイムのフラグ
	bool isFireActive = 1;
	//レーザー弾の発射可能フラグ
	bool isLaserActive = 0;
	//判定
	Judge judge = Judge::None;

	//判定のずれ(オフセット)
	float offset = 0.0f;
	//BGM音量
	float BGMVolume = 1.0f;
	//ノーマルSE音量
	float normalSEVolume = 1.0f;
	//ガイドSE音量
	float guideSEVolume = 1.0f;
	//現在のウェーブ数
	float wave = 1.0f;
};

class Rhythm
{
public:

	~Rhythm();

	void Initialize(ViewProjection* viewProjection, XMMATRIX* matProjection);

	void Update(Input* input, Vector3 pos, Vector3 rot, int isDead,int stage);

	void Draw(int isDead);

	void SoundUnload(SoundData soundData);

	void SoundPlayWave(SoundData soundData, float volume);

	void ItemSoundPlay(float volume){ soundManager_->SoundPlayWave(soundManager_->xAudio2.Get(), itemSound, false, volume); };
	void damageSoundPlay(float volume){ soundManager_->SoundPlayWave(soundManager_->xAudio2.Get(), damageSound, false, volume); };
	void knockSoundPlay(float volume){ soundManager_->SoundPlayWave(soundManager_->xAudio2.Get(), knockSound, false, volume); };

	void NormalShot(SoundState s, Input* input);
	void RapidShot(SoundState s, Input* input);
	void ThreeWayShot(SoundState s, Input* input);
	void ExplosionShot(SoundState s, Input* input);
	void LaserShot(SoundState s, Input* input);

	void SetWeapon(Weapons weapon) { this->soundState.weapon = weapon; }

	SoundState GetSoundState() { return soundState; }

private:
	static const int circleNum = 2;
	GameObject3D* circle[circleNum];

	SoundState soundState;

	//一小節の時間
	const float maxTimer = 60.0f;
	//判定の広さ
	const float judgeBreadth = 5.0f;

	//弾発射タイミング1
	float shotTiming1;
	//弾発射タイミング2
	float shotTiming2;
	//弾発射タイミング3
	float shotTiming3;
	//弾が装填されるタイミング
	float reloadTiming;
	
	//爆裂弾の発射が成功したかどうか(仮)
	int isSuccess = 0;

	Vector3 shrinkSpeed = { 0.1f,0.1f,0.0f };

	float colorChange = 0.0f;

	float pitch;

	//サウンドマネージャー
	SoundManager* soundManager_ = nullptr;

	//音声読み込み
	//SE
	SoundData guideSound1 = soundManager_->SoundLoadWave("Resources/rhythm2.wav");
	SoundData guideSound2 = soundManager_->SoundLoadWave("Resources/rhythm3.wav");
	SoundData guideSound3 = soundManager_->SoundLoadWave("Resources/rhythm4.wav");
	SoundData shotSound = soundManager_->SoundLoadWave("Resources/shot2.wav");
	SoundData missSound = soundManager_->SoundLoadWave("Resources/death.wav");
	SoundData laserSound = soundManager_->SoundLoadWave("Resources/shot4.wav");
	SoundData explosionSound = soundManager_->SoundLoadWave("Resources/shot3.wav");
	SoundData itemSound = soundManager_->SoundLoadWave("Resources/item.wav");
	SoundData damageSound = soundManager_->SoundLoadWave("Resources/damage.wav");
	SoundData knockSound = soundManager_->SoundLoadWave("Resources/knock.wav");

	//BGM
	SoundData demoBGM = soundManager_->SoundLoadWave("Resources/demo.wav");
	SoundData stage1_1BGM = soundManager_->SoundLoadWave("Resources/stage1-1.wav");
	SoundData stage1_2BGM = soundManager_->SoundLoadWave("Resources/stage1-2.wav");
	SoundData stage1_bossBGM = soundManager_->SoundLoadWave("Resources/stage1-boss.wav");

};