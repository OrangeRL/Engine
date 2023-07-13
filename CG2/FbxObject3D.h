#pragma once
#include"FbxModel.h"
#include"ViewProjection.h"
#include "WorldTransform.h"
#include<Windows.h>
#include<wrl.h>
#include<d3d12.h>
#include<d3dx12.h>
#include<string>
#include "DX12base.h"

class FbxObject3D
{
protected:	//�G�C���A�X
//Microsoft::WRL::���ȗ�
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
	//DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	//�萔
	static const int MAX_BONES = 32;

	struct ConstBufferDataTransform
	{
		Matrix4 viewproj;
		Matrix4 world;
		//XMFLOAT3 cameraPos;
		Matrix4 mat; //3D�ϊ��s��
	};
	//�萔�o�b�t�@�p�f�[�^�\����(�X�L�j���O)
	struct ConstBufferDataSkin
	{
		XMMATRIX bones[MAX_BONES];
	};
	struct ConstBufferDataMaterial {
		Vector4 color; //�F(RGBA)
	};
//�Z�b�^�[
	static void SetDevice(ComPtr<ID3D12Device> device) { FbxObject3D::device = device; }
	static void SetCamera(ViewProjection* camera) { FbxObject3D::camera = camera; }
private://�ÓI�����o�ϐ�
	static ComPtr<ID3D12Device> device;
	static ViewProjection* camera;
	static ID3D12GraphicsCommandList* cmdList;
public://�����o�֐�
	//������
	void Initialize();
	//�X�V
	void Update();
	//�`��
	void Draw(ID3D12GraphicsCommandList* CmdList);

	void SetModel(FbxModel* model) { this->model = model; }
	//�O���t�B�b�N�X�p�C�v���C���̐���
	static void CreateGraphicsPipeline();

	void InitializeConstMapTransform();
	void InitializeConstMapMaterial();

	void SetViewProjection(ViewProjection* viewProjection);
	void SetMatProjection(XMMATRIX* matProjection);

	//�A�j���[�V�����J�n
	void PlayAnimation();

	//�Z�b�^�[
	void SetPosition(XMFLOAT3 pos) { position = pos; }
	void SetRotation(Vector3 rot) { rotation = rot; }
	void SetScale(XMFLOAT3 sca) { scale = sca; }
private://�����o�ϐ�
	//�萔�o�b�t�@
	//ComPtr<ID3D12Resource>constBuffTransform;
	//�萔�o�b�t�@(�s��p)
	ComPtr<ID3D12Resource> constBuffTransform = nullptr;
	//�萔�o�b�t�@�}�b�s���O(�s��p)
	ConstBufferDataTransform* constMapTransform = nullptr;

	ComPtr<ID3D12Resource> constBuffMaterial = nullptr;
	//���[�g�V�O�l�`��
	static ComPtr<ID3D12RootSignature>rootsignature;
	//�p�C�v���C���X�e�[�g�I�u�W�F�N�g
	static ComPtr<ID3D12PipelineState>pipelinestate;
	ConstBufferDataMaterial* constMapMaterial = nullptr;
private:
	//���[�J���X�P�[��
	XMFLOAT3 scale = { 1,1,1 };
	//X,Y,Z�����̃��[�J���s��
	Vector3 rotation = { 0,0,0 };
	//���[�J�����W
	XMFLOAT3 position = { 0,0,0 };
	//���[�J�����[���h�ϊ��s��
	XMMATRIX matWorld;
	//���f��
	FbxModel* model = nullptr;

	//�萔�o�b�t�@
	ComPtr<ID3D12Resource>constBuffSkin;

	//1�t���[���̎���
	FbxTime frameTime;
	//�A�j���[�V�����J�n����
	FbxTime startTime;
	//�A�j���[�V�����I������
	FbxTime endTime;
	//���ݎ���
	FbxTime currentTime;
	//�A�j���[�V�����Đ���
	bool isPlay = false;

	//���[���h�ϊ�
	WorldTransform worldTransform;
	XMMATRIX* matProjection;
	//�r���[�v���W�F�N�V����
	ViewProjection* viewProjection;
	DX12base& dx12base = DX12base::GetInstance();
	Vector4 color = { 1.0f,1.0f,1.0f,1.0f };
};