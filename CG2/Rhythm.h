#pragma once
#include "Input.h"
#include "Audio.h"

enum class Weapons {
	Normal,		//�ʏ�e
	Rapid,		//���˒e
	ThreeWay,	//3way�e
	Explosion,	//�����e
	Laser,		//���[�U�[�e
};

struct SoundState {
	//�^�C�}�[
	float timer = 0.0f;
	//���߃J�E���g(��)
	int measureCount = 0;

	//�ꎞ��~����
	bool isPause = 0;
	//����̎��
	Weapons weapon = Weapons::Normal;
	//���ː���������
	bool isFireSucces;
	//���[�U�[�e�̔��ˉ\�t���O
	bool isLaserActive = 0;

	//����̂���(�I�t�Z�b�g)
	float offset = 0.0f;
	//BGM����
	float BGMVolume = 1.0f;
	//�m�[�}��SE����
	float normalSEVolume = 1.0f;
	//�K�C�hSE����
	float guideSEVolume = 1.0f;
};

class Rhythm
{
public:

	~Rhythm();

	void Initialize();

	void Update(Input* input);

	void SoundUnload(SoundData soundData);

	void SoundPlayWave(SoundData soundData, float volume);

	void NormalShot(SoundState s, Input* input);
	void RapidShot(SoundState s, Input* input);
	void ThreeWayShot(SoundState s, Input* input);
	void ExplosionShot(SoundState s, Input* input);
	void LaserShot(SoundState s, Input* input);

	void SetWeapon(Weapons weapon) { this->soundState.weapon = weapon; }

	SoundState GetSoundState() { return soundState; }

private:
	SoundState soundState;

	//�ꏬ�߂̎���
	const float maxTimer = 60.0f;
	//����̍L��
	const float judgeBreadth = 3.0f;

	//�e���˃^�C�~���O1
	float shotTiming1;
	//�e���˃^�C�~���O2
	float shotTiming2;
	//�e���˃^�C�~���O3
	float shotTiming3;
	//�e�����U�����^�C�~���O
	float reloadTiming;
	//���̒e�����Ă�܂ł̃N�[���^�C���̃t���O
	bool isFireActive = 1;
	//�����e�̔��˂������������ǂ���(��)
	int isSuccess = 0;

	//�T�E���h�}�l�[�W���[
	SoundManager* soundManager_ = nullptr;

	//�����ǂݍ���
	SoundData guideSound1 = soundManager_->SoundLoadWave("Resources/rhythm2.wav");
	SoundData guideSound2 = soundManager_->SoundLoadWave("Resources/rhythm3.wav");
	SoundData guideSound3 = soundManager_->SoundLoadWave("Resources/rhythm4.wav");
	SoundData shotSound = soundManager_->SoundLoadWave("Resources/shot2.wav");
	SoundData missSound = soundManager_->SoundLoadWave("Resources/death.wav");
	SoundData laserSound = soundManager_->SoundLoadWave("Resources/shot4.wav");
	SoundData explosionSound = soundManager_->SoundLoadWave("Resources/shot3.wav");
	SoundData demoBGM = soundManager_->SoundLoadWave("Resources/demo.wav");
	SoundData stage1BGM = soundManager_->SoundLoadWave("Resources/stage1.wav");



};