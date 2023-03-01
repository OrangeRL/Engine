#include "Rhythm.h"

Rhythm::~Rhythm() {
	SoundUnload(guideSound1);
	SoundUnload(guideSound2);
	SoundUnload(guideSound3);
	SoundUnload(shotSound);
	SoundUnload(laserSound);
	SoundUnload(explosionSound);
	SoundUnload(missSound);
	SoundUnload(demoBGM);
	delete soundManager_;
}

void Rhythm::Initialize() {

	soundManager_ = new SoundManager();
	soundManager_->Initialize();

	soundManager_->SoundPlayWave(soundManager_->xAudio2.Get(), stage1BGM, false, soundState.BGMVolume);
}

void Rhythm::Update(Input* input) {


	//����ύX
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

			//�I�t�Z�b�g�ݒ�
			if (input->TriggerKey(DIK_A) && soundState.offset > -5.0f) {
				soundState.offset--;
			}
			else if (input->TriggerKey(DIK_D) && soundState.offset < 5.0f) {
				soundState.offset++;
			}

			//�^�C�}�[����������烌�[�U�[���ˉ\��
			if (soundState.timer == 30.0f) {
				if (soundState.isLaserActive == 0) {
					soundState.isLaserActive = 1;
				}
				else {
					soundState.isLaserActive = 0;
				}
			}

			//�ʏ�e�̃��Y��
			if (soundState.weapon == Weapons::Normal) {
				NormalShot(soundState, input);
				if (input->PushKey(DIK_SPACE)) {
					//soundState.isActive = 0;
				}
			}
			//���˒e�̃��Y��
			else if (soundState.weapon == Weapons::Rapid) {
				RapidShot(soundState, input);
			}
			//3way�e�̃��Y��
			else if (soundState.weapon == Weapons::ThreeWay) {
				ThreeWayShot(soundState, input);
			}
			//�����e�̃��Y��
			else if (soundState.weapon == Weapons::Explosion) {
				ExplosionShot(soundState, input);
			}
			//���[�U�[�e�̃��Y��
			else if (soundState.weapon == Weapons::Laser) {
				LaserShot(soundState, input);
			}

			//�Ȃ����������ŏ�����Đ�(��)
			if (soundState.measureCount == 0 && soundState.timer == 0) {
				//SoundPlayWave(mainBGM);
				//soundManager_->SoundPlayWave(soundManager_->xAudio2.Get(), demoBGM, false, BGMVolume);
			}

			//�ڈ��Ƃ���2���ߖڂő傫�߂̉���炷
			if (soundState.timer == maxTimer / 2) {
				//SoundPlayWave(guideSound1, BGMVolume);
			}
		}

		//�^�C�}�[�𑝂₷
		soundState.timer++;

		//�^�C�}�[���ő�܂ł�������0�ɂ��A���߂�1�i�߂�
		if (soundState.timer >= maxTimer) {
			soundState.timer = 0;
			soundState.measureCount++;
		}

		//���߂����܂ł�������Ȃ��ŏ�����ɂ���(��)
		if (soundState.measureCount >= 16) {
			//soundState.measureCount = 0;
		}
	}

	//�ꎞ��~
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

	//���ʒ���
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

void Rhythm::NormalShot(SoundState s, Input* input) {
	shotTiming1 = 30.0f;	//�e���˃^�C�~���O1
	reloadTiming = 15.0f;	//�e�����U�����^�C�~���O

	//���̃^�C�~���O�ŉ���炷
	if (s.timer == 0.0f || s.timer == shotTiming1) {
		SoundPlayWave(guideSound2, s.guideSEVolume);
	}
	//���̃^�C�~���O�Œe�𑕓U����
	if (s.timer == reloadTiming || s.timer == shotTiming1 + reloadTiming) {
		isActive = 1;
	}

	//�e�����ˉ\�ȂƂ�
	if (isActive == 1) {
		if (input->TriggerKey(DIK_SPACE)) {
			//���x�����^�C�~���O�ŃL�[�������Ɣ���
			if (((s.timer <= judgeBreadth + s.offset) ||
				(s.timer >= shotTiming1 - judgeBreadth + s.offset && s.timer <= shotTiming1 + judgeBreadth + s.offset) ||
				(s.timer >= maxTimer - judgeBreadth + s.offset))) {
				SoundPlayWave(shotSound, s.normalSEVolume);
			}
			//����ȊO�̃^�C�~���O�͎��s
			else {
				SoundPlayWave(missSound, s.normalSEVolume);
			}

			isActive = 0;
		}
	}
}

void Rhythm::RapidShot(SoundState s, Input* input) {
	shotTiming1 = 15.0f;	//�e���˃^�C�~���O1
	shotTiming2 = 30.0f;	//�e���˃^�C�~���O2
	shotTiming3 = 45.0f;	//�e���˃^�C�~���O3
	reloadTiming = 7.0f;	//�e�����U�����^�C�~���O

	//���̃^�C�~���O�ŉ���炷
	if (s.timer == 0.0f || s.timer == shotTiming1 || s.timer == shotTiming2 || s.timer == shotTiming3) {
		SoundPlayWave(guideSound2, s.guideSEVolume);
	}
	//���̃^�C�~���O�Œe�𑕓U����
	if (s.timer == reloadTiming || s.timer == shotTiming1 + reloadTiming ||
		s.timer == shotTiming2 + reloadTiming || s.timer == shotTiming3 + reloadTiming) {
		isActive = 1;
	}

	//�e�����ˉ\�ȂƂ�
	if (isActive == 1) {
		if (input->TriggerKey(DIK_SPACE)) {
			//���x�����^�C�~���O�ŃL�[�������Ɣ���
			if (((s.timer <= judgeBreadth + s.offset) ||
				(s.timer >= shotTiming1 - judgeBreadth + s.offset && s.timer <= shotTiming1 + judgeBreadth + s.offset) ||
				(s.timer >= shotTiming2 - judgeBreadth + s.offset && s.timer <= shotTiming2 + judgeBreadth + s.offset) ||
				(s.timer >= shotTiming3 - judgeBreadth + s.offset && s.timer <= shotTiming3 + judgeBreadth + s.offset) ||
				(s.timer >= maxTimer - judgeBreadth + s.offset))) {
				SoundPlayWave(shotSound, s.normalSEVolume);
			}
			//����ȊO�̃^�C�~���O�͎��s
			else {
				SoundPlayWave(missSound, s.normalSEVolume);
			}

			isActive = 0;
		}
	}
}

void Rhythm::ThreeWayShot(SoundState s, Input* input) {
	shotTiming1 = 20.0f;	//�e���˃^�C�~���O1
	shotTiming2 = 40.0f;	//�e���˃^�C�~���O2
	reloadTiming = 10.0f;	//�e�����U�����^�C�~���O

	//���̃^�C�~���O�ŉ���炷
	if (s.timer == 0.0f ||s. timer == shotTiming1 || s.timer == shotTiming2) {
		SoundPlayWave(guideSound2, s.guideSEVolume);

	}
	//���̃^�C�~���O�Œe�𑕓U����
	if (s.timer == reloadTiming || s.timer == shotTiming1 + reloadTiming || s.timer == shotTiming2 + reloadTiming) {
		isActive = 1;
	}

	//�e�����ˉ\�ȂƂ�
	if (isActive == 1) {
		if (input->TriggerKey(DIK_SPACE)) {
			//���x�����^�C�~���O�ŃL�[�������Ɣ���
			if (((s.timer <= judgeBreadth + s.offset) ||
				(s.timer >= shotTiming1 - judgeBreadth + s.offset && s.timer <= shotTiming1 + judgeBreadth + s.offset) ||
				(s.timer >= shotTiming2 - judgeBreadth + s.offset && s.timer <= shotTiming2 + judgeBreadth + s.offset) ||
				(s.timer >= maxTimer - judgeBreadth + s.offset))) {
				SoundPlayWave(shotSound, s.normalSEVolume);
			}
			//����ȊO�̃^�C�~���O�͎��s
			else {
				SoundPlayWave(missSound, s.normalSEVolume);
			}

			isActive = 0;

		}
	}
}

void Rhythm::ExplosionShot(SoundState s, Input* input) {
	shotTiming1 = 30.0f;	//�e���˃^�C�~���O1(���j�^�C�~���O)
	reloadTiming = 45.0f;	//�e�����U�����^�C�~���O

	//���̃^�C�~���O�ŉ���炷
	if (s.timer == 0.0f) {
		SoundPlayWave(guideSound2, s.guideSEVolume);
	}
	//���̃^�C�~���O�Œe�𑕓U����
	if (s.timer == reloadTiming) {
		isSuccess = 0;
		isActive = 1;
	}

	//�e�����ˉ\�ȂƂ�
	if (isActive == 1) {
		if (input->TriggerKey(DIK_SPACE)) {
			//���x�����^�C�~���O�ŃL�[�������Ɣ���
			if (((s.timer <= judgeBreadth + s.offset) ||
				(s.timer >= maxTimer - judgeBreadth + s.offset))) {
				SoundPlayWave(shotSound, s.normalSEVolume);
				isSuccess = 1;
			}
			//����ȊO�̃^�C�~���O�͎��s
			else {
				SoundPlayWave(missSound, s.normalSEVolume);
				isSuccess = 0;
			}

			isActive = 0;
		}
	}
	//���������甚�j����炷
	if (isSuccess == 1 && s.timer == shotTiming1) {
		SoundPlayWave(explosionSound, s.normalSEVolume);
	}
}

void Rhythm::LaserShot(SoundState s, Input* input) {
	reloadTiming = 0.0f;	//�e�����U�����^�C�~���O

	//���̃^�C�~���O�ŉ���炷
	if (s.timer == 0.0f && s.isLaserActive == 1) {
		SoundPlayWave(guideSound3, s.guideSEVolume);
	}
	//���̃^�C�~���O�Œe�𑕓U����
	if (s.timer == reloadTiming && s.isLaserActive == 0) {
		isActive = 1;
	}

	//�e�����ˉ\�ȂƂ�
	if (isActive == 1) {
		if (input->TriggerKey(DIK_SPACE)) {
			//���x�����^�C�~���O�ŃL�[�������Ɣ���
			if (((s.timer <= judgeBreadth + s.offset && s.isLaserActive == 1) ||
				(s.timer >= maxTimer - judgeBreadth + s.offset && s.isLaserActive == 1))) {
				soundManager_->SoundPlayWave(soundManager_->xAudio2.Get(), laserSound, false, s.normalSEVolume);

			}
			//����ȊO�̃^�C�~���O�͎��s
			else {
				SoundPlayWave(missSound, s.normalSEVolume);
			}

			isActive = 0;
		}
	}

	//�{�^���𗣂��Ǝ~�܂�
	if (input->PushKey(DIK_SPACE) == 0 && laserSound.pSourceVoice != nullptr) {
		laserSound.pSourceVoice->Stop(0);
	}
}
