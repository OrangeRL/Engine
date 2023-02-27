#include "Rhythm.h"

Rhythm::~Rhythm() {
	SoundUnload(guideSound1);
	SoundUnload(guideSound2);
	SoundUnload(guideSound3);
	SoundUnload(shotSound);
	SoundUnload(laserSound);
	SoundUnload(explosionSound);
	SoundUnload(missSound);
	SoundUnload(mainBGM);
	delete soundManager_;
}

void Rhythm::Initialize() {
	soundManager_ = new SoundManager();
	soundManager_->Initialize();
}

void Rhythm::Update(Input* input) {

	//����ύX
	if (input->TriggerKey(DIK_0)) {
		weapon = Weapons::Normal;
	}
	else if (input->TriggerKey(DIK_1)) {
		weapon = Weapons::Rapid;
	}
	else if (input->TriggerKey(DIK_2)) {
		weapon = Weapons::Explosion;
	}
	else if (input->TriggerKey(DIK_3)) {
		weapon = Weapons::ThreeWay;
	}
	else if (input->TriggerKey(DIK_4)) {
		weapon = Weapons::Laser;
	}

	if (isPause == false) {
		//�I�t�Z�b�g�ݒ�
		if (input->TriggerKey(DIK_A) && offset > -5.0f) {
			offset--;
		}
		else if (input->TriggerKey(DIK_D) && offset < 5.0f) {
			offset++;
		}

		//�^�C�}�[����������烌�[�U�[���ˉ\��
		if (timer == 30.0f) {
			if (isLaserActive == 0) {
				isLaserActive = 1;
			}
			else {
				isLaserActive = 0;
			}
		}

		//�ʏ�e�̃��Y��
		if (weapon == Weapons::Normal) {
			shotTiming1 = 30.0f;	//�e���˃^�C�~���O1
			reloadTiming = 15.0f;	//�e�����U�����^�C�~���O

			//���̃^�C�~���O�ŉ���炷
			if (timer == 0.0f || timer == shotTiming1) {
				SoundPlayWave(guideSound2, guideSEVolume);
			}
			//���̃^�C�~���O�Œe�𑕓U����
			if (timer == reloadTiming || timer == shotTiming1 + reloadTiming) {
				isActive = 1;
			}

			//�e�����ˉ\�ȂƂ�
			if (isActive == 1) {
				if (input->TriggerKey(DIK_SPACE)) {
					//���x�����^�C�~���O�ŃL�[�������Ɣ���
					if (((timer <= judgeBreadth + offset) ||
						(timer >= shotTiming1 - judgeBreadth + offset && timer <= shotTiming1 + judgeBreadth + offset) ||
						(timer >= maxTimer - judgeBreadth + offset))) {
						SoundPlayWave(shotSound, normalSEVolume);
					}
					//����ȊO�̃^�C�~���O�͎��s
					else {
						SoundPlayWave(missSound, normalSEVolume);
					}

					isActive = 0;
				}
			}
		}
		//���˒e�̃��Y��
		else if (weapon == Weapons::Rapid) {
			shotTiming1 = 15.0f;	//�e���˃^�C�~���O1
			shotTiming2 = 30.0f;	//�e���˃^�C�~���O2
			shotTiming3 = 45.0f;	//�e���˃^�C�~���O3
			reloadTiming = 7.0f;	//�e�����U�����^�C�~���O

			//���̃^�C�~���O�ŉ���炷
			if (timer == 0.0f || timer == shotTiming1 || timer == shotTiming2 || timer == shotTiming3) {
				SoundPlayWave(guideSound2, guideSEVolume);
			}
			//���̃^�C�~���O�Œe�𑕓U����
			if (timer == reloadTiming || timer == shotTiming1 + reloadTiming ||
				timer == shotTiming2 + reloadTiming || timer == shotTiming3 + reloadTiming) {
				isActive = 1;
			}

			//�e�����ˉ\�ȂƂ�
			if (isActive == 1) {
				if (input->TriggerKey(DIK_SPACE)) {
					//���x�����^�C�~���O�ŃL�[�������Ɣ���
					if (((timer <= judgeBreadth + offset) ||
						(timer >= shotTiming1 - judgeBreadth + offset && timer <= shotTiming1 + judgeBreadth + offset) ||
						(timer >= shotTiming2 - judgeBreadth + offset && timer <= shotTiming2 + judgeBreadth + offset) ||
						(timer >= shotTiming3 - judgeBreadth + offset && timer <= shotTiming3 + judgeBreadth + offset) ||
						(timer >= maxTimer - judgeBreadth + offset))) {
						SoundPlayWave(shotSound, normalSEVolume);
					}
					//����ȊO�̃^�C�~���O�͎��s
					else {
						SoundPlayWave(missSound, normalSEVolume);
					}

					isActive = 0;
				}
			}
		}

		//�����e�̃��Y��
		else if (weapon == Weapons::Explosion) {
			shotTiming1 = 30.0f;	//�e���˃^�C�~���O1(���j�^�C�~���O)
			reloadTiming = 45.0f;	//�e�����U�����^�C�~���O

			//���̃^�C�~���O�ŉ���炷
			if (timer == 0.0f) {
				SoundPlayWave(guideSound2, guideSEVolume);
			}
			//���̃^�C�~���O�Œe�𑕓U����
			if (timer == reloadTiming) {
				isSuccess = 0;
				isActive = 1;
			}

			//�e�����ˉ\�ȂƂ�
			if (isActive == 1) {
				if (input->TriggerKey(DIK_SPACE)) {
					//���x�����^�C�~���O�ŃL�[�������Ɣ���
					if (((timer <= judgeBreadth + offset) ||
						(timer >= maxTimer - judgeBreadth + offset))) {
						SoundPlayWave(shotSound, normalSEVolume);
						isSuccess = 1;
					}
					//����ȊO�̃^�C�~���O�͎��s
					else {
						SoundPlayWave(missSound, normalSEVolume);
						isSuccess = 0;
					}

					isActive = 0;
				}
			}
			//���������甚�j����炷
			if (isSuccess == 1 && timer == shotTiming1) {
				SoundPlayWave(explosionSound, normalSEVolume);
			}
		}

		//3way�e�̃��Y��
		else if (weapon == Weapons::ThreeWay) {
			shotTiming1 = 20.0f;	//�e���˃^�C�~���O1
			shotTiming2 = 40.0f;	//�e���˃^�C�~���O2
			reloadTiming = 10.0f;	//�e�����U�����^�C�~���O

			//���̃^�C�~���O�ŉ���炷
			if (timer == 0.0f || timer == shotTiming1 || timer == shotTiming2) {
				SoundPlayWave(guideSound2, guideSEVolume);

			}
			//���̃^�C�~���O�Œe�𑕓U����
			if (timer == reloadTiming || timer == shotTiming1 + reloadTiming || timer == shotTiming2 + reloadTiming) {
				isActive = 1;
			}

			//�e�����ˉ\�ȂƂ�
			if (isActive == 1) {
				if (input->TriggerKey(DIK_SPACE)) {
					//���x�����^�C�~���O�ŃL�[�������Ɣ���
					if (((timer <= judgeBreadth + offset) ||
						(timer >= shotTiming1 - judgeBreadth + offset && timer <= shotTiming1 + judgeBreadth + offset) ||
						(timer >= shotTiming2 - judgeBreadth + offset && timer <= shotTiming2 + judgeBreadth + offset) ||
						(timer >= maxTimer - judgeBreadth + offset))) {
						SoundPlayWave(shotSound, normalSEVolume);
					}
					//����ȊO�̃^�C�~���O�͎��s
					else {
						SoundPlayWave(missSound, normalSEVolume);
					}

					isActive = 0;

				}
			}
		}

		//���[�U�[�e�̃��Y��
		else if (weapon == Weapons::Laser) {
			reloadTiming = 0.0f;	//�e�����U�����^�C�~���O

			//���̃^�C�~���O�ŉ���炷
			if (timer == 0.0f && isLaserActive == 1) {
				SoundPlayWave(guideSound3, guideSEVolume);
			}
			//���̃^�C�~���O�Œe�𑕓U����
			if (timer == reloadTiming && isLaserActive == 0) {
				isActive = 1;
			}

			//�e�����ˉ\�ȂƂ�
			if (isActive == 1) {
				if (input->TriggerKey(DIK_SPACE)) {
					//���x�����^�C�~���O�ŃL�[�������Ɣ���
					if (((timer <= judgeBreadth + offset && isLaserActive == 1) ||
						(timer >= maxTimer - judgeBreadth + offset && isLaserActive == 1))) {
						soundManager_->SoundPlayWave(soundManager_->xAudio2.Get(), laserSound, false, normalSEVolume);

					}
					//����ȊO�̃^�C�~���O�͎��s
					else {
						SoundPlayWave(missSound, normalSEVolume);
					}

					isActive = 0;
				}
			}

			//�{�^���𗣂��Ǝ~�܂�
			if (input->PushKey(DIK_SPACE) == 0 && laserSound.pSourceVoice != nullptr) {
				laserSound.pSourceVoice->Stop(0);
			}
		}

		//�Ȃ����������ŏ�����Đ�(��)
		if (measureCount == 0 && timer == 0) {
			//SoundPlayWave(mainBGM);
			soundManager_->SoundPlayWave(soundManager_->xAudio2.Get(), mainBGM, false, BGMVolume);
		}

		//�ڈ��Ƃ���2���ߖڂő傫�߂̉���炷
		if (timer == maxTimer / 2) {
			SoundPlayWave(guideSound1, BGMVolume);
		}

		//�^�C�}�[�𑝂₷
		timer++;

		//�^�C�}�[���ő�܂ł�������0�ɂ��A���߂�1�i�߂�
		if (timer >= maxTimer) {
			timer = 0;
			measureCount++;
		}

		//���߂����܂ł�������Ȃ��ŏ�����ɂ���(��)
		if (measureCount >= 16) {
			measureCount = 0;
		}
	}

	//�ꎞ��~
	if (input->TriggerKey(DIK_P)) {

		if (isPause == 0) {
			isPause = 1;
			mainBGM.pSourceVoice->Stop(0);
		}
		else {
			isPause = 0;
			mainBGM.pSourceVoice->Start();

		}
	}

	//���ʒ���
	//BGM
	if (input->PushKey(DIK_UP) && BGMVolume < 2.0f) {
		BGMVolume += 0.01;
	}
	if (input->PushKey(DIK_DOWN) && BGMVolume >= 0.0f) {
		BGMVolume -= 0.01;
		if (BGMVolume <= 0.0f) {
			BGMVolume = 0;
		}
	}
	//SE
	if (input->PushKey(DIK_RIGHT) && guideSEVolume < 2.0f) {
		guideSEVolume += 0.01;
	}
	if (input->PushKey(DIK_LEFT) && guideSEVolume >= 0.0f) {
		guideSEVolume -= 0.01;
		if (guideSEVolume <= 0.0f) {
			guideSEVolume = 0;
		}
	}

	mainBGM.pSourceVoice->SetVolume(BGMVolume);
}

void Rhythm::SoundPlayWave(SoundData soundData, float volume) {
	soundManager_->StopWave(soundData);
	soundManager_->SoundPlayWave(soundManager_->xAudio2.Get(), soundData, false, volume);
}

void Rhythm::SoundUnload(SoundData soundData) {
	soundManager_->StopWave(soundData);
	soundManager_->SoundUnload(soundData);
}