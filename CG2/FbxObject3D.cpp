#include "FbxObject3D.h"
#include "FbxLoader.h"
#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

using namespace Microsoft::WRL;
using namespace DirectX;

ComPtr<ID3D12RootSignature>FbxObject3D::rootsignature;
ComPtr<ID3D12PipelineState>FbxObject3D::pipelinestate;

ComPtr<ID3D12Device> FbxObject3D::device = nullptr;
ViewProjection* FbxObject3D::camera = nullptr;
ID3D12GraphicsCommandList* FbxObject3D::cmdList = nullptr;
void FbxObject3D::Initialize()
{
	//1�t���[�����̎��Ԃ�60FPS�Őݒ�
	frameTime.SetTime(0, 0, 0, 1, 0, FbxTime::EMode::eFrames60);

	HRESULT result;
	//�萔�o�b�t�@�̐���
	CD3DX12_HEAP_PROPERTIES v1 = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	CD3DX12_RESOURCE_DESC v2 = CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataTransform) + 0xff) & ~0xff);
	CD3DX12_RESOURCE_DESC v3 = CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataSkin) + 0xff) & ~0xff);
	result = device->CreateCommittedResource(
		&v1,
		D3D12_HEAP_FLAG_NONE,
		&v2,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffTransform)
	);
	result = device->CreateCommittedResource(
		&v1,	//�A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&v3,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffSkin)
	);
	InitializeConstMapTransform();
	InitializeConstMapMaterial();
	//���[���h�ϊ��̏�����
	worldTransform.initialize();

	//�萔�o�b�t�@�փf�[�^�]��
	ConstBufferDataSkin* constMapSkin = nullptr;
	result = constBuffSkin->Map(0, nullptr, (void**)&constMapSkin);
	for (int i = 0; i < MAX_BONES; i++)
	{
		constMapSkin->bones[i] = XMMatrixIdentity();
	}
	constBuffSkin->Unmap(0, nullptr);
}

void FbxObject3D::Update()
{
	HRESULT result;

	//�A�j���[�V����
	if (isPlay)
	{
		//1�t���[���i�߂�
		currentTime += frameTime;
		//�Ō�܂ōĐ�������擪�ɖ߂�
		if (currentTime > endTime)
		{
			currentTime = startTime;
		}
	}

	//�{�[���z��
	std::vector<FbxModel::Bone>& bones = model->GetBones();

	//�萔�o�b�t�@�փf�[�^�]��
	ConstBufferDataSkin* constMapSkin = nullptr;
	result = constBuffSkin->Map(0, nullptr, (void**)&constMapSkin);
	for (int i = 0; i < bones.size(); i++)
	{
		//���̎p���s��
		XMMATRIX matCurrentPose;
		//���̎p���s����擾
		FbxAMatrix fbxCurrentPose = bones[i].fbxCluster->GetLink()->EvaluateGlobalTransform(currentTime);
		//XMMATRIX1�ɕϊ�
		FbxLoader::ConvertMatrixFromFbx(&matCurrentPose, fbxCurrentPose);
		//�������ăX�L�j���O�s���
		constMapSkin->bones[i] = model->GetModelTransform() * bones[i].invInitialPose * matCurrentPose * XMMatrixInverse(nullptr, model->GetModelTransform());
		//XMMatrixInverse(nullptr, model->GetModelTransform());
		
	}
	constBuffSkin->Unmap(0, nullptr);

	worldTransform.UpdateMatWorld();
	//�萔�o�b�t�@�փf�[�^�]��
	constMapTransform->world = worldTransform.matWorld;
	constMapTransform->viewproj = camera->matView;
	constMapTransform->viewproj *= MathFunc::Utility::ConvertXMMATRIXtoMatrix4(*matProjection);
	//constMapTransform->viewproj *= MathFunc::Utility::ConvertXMMATRIXtoMatrix4(*matProjection);
	
	//���f���̃��b�V���g�����X�t�H�[��
	const XMMATRIX& modelTransform = model->GetModelTransform();
	//constMapMaterial->color = color;

	//�萔�o�b�t�@�փf�[�^�]��
	ConstBufferDataTransform* constMap = nullptr;
	result = constBuffTransform->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result))
	{
		constMap->viewproj = camera->matView;
		constMap->world = worldTransform.matWorld;
		constMap->viewproj *= MathFunc::Utility::ConvertXMMATRIXtoMatrix4(*matProjection);
		constBuffTransform->Unmap(0, nullptr);
	}

}

void FbxObject3D::Draw(ID3D12GraphicsCommandList* CmdList)
{
	//���f���̊��蓖�Ă��Ȃ���Ε`��s��
	if (model == nullptr)
	{
		return;
	}

	//�p�C�v���C���X�e�[�g�̐ݒ�
	dx12base.GetCmdList()->SetPipelineState(pipelinestate.Get());
	//���[�g�V�O�l�`���̐ݒ�
	dx12base.GetCmdList()->SetGraphicsRootSignature(rootsignature.Get());
	//�v���~�e�B�u�`��̐ݒ�
	dx12base.GetCmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//�萔�o�b�t�@�r���[���Z�b�g
	dx12base.GetCmdList()->SetGraphicsRootConstantBufferView(0,constBuffTransform->GetGPUVirtualAddress());
	dx12base.GetCmdList()->SetGraphicsRootConstantBufferView(2, constBuffSkin->GetGPUVirtualAddress());

	//���f���`��
	model->Draw(CmdList);
}


void FbxObject3D::CreateGraphicsPipeline()
{
	HRESULT result = S_FALSE;
	ComPtr<ID3DBlob> vsBlob; // ���_�V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> psBlob;    // �s�N�Z���V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> errorBlob; // �G���[�I�u�W�F�N�g

	assert(device);

	// ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"FbxVertexShader.hlsl",    // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "vs_5_0",    // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&vsBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"FbxPixelShader.hlsl",    // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "ps_5_0",    // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&psBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// ���_���C�A�E�g
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{ // xy���W(1�s�ŏ������ق������₷��)
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // �@���x�N�g��(1�s�ŏ������ق������₷��)
			"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // uv���W(1�s�ŏ������ق������₷��)
			"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{//�e�����󂯂�{�[���ԍ�(4��)
			"BONEINDICES",0,DXGI_FORMAT_R32G32B32A32_UINT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
		{//�{�[���̃X�L���E�F�C�g(4��)
			"BONEWEIGHTS",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
	};

	// �O���t�B�b�N�X�p�C�v���C���̗����ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	// �T���v���}�X�N
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�
	// ���X�^���C�U�X�e�[�g
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	//gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	//gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	// �f�v�X�X�e���V���X�e�[�g
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	// �����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;    // RBGA�S�Ẵ`�����l����`��
	blenddesc.BlendEnable = true;
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// �u�����h�X�e�[�g�̐ݒ�
	gpipeline.BlendState.RenderTarget[0] = blenddesc;
	gpipeline.BlendState.RenderTarget[1] = blenddesc;

	// �[�x�o�b�t�@�̃t�H�[�}�b�g
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// ���_���C�A�E�g�̐ݒ�
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	// �}�`�̌`��ݒ�i�O�p�`�j
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 2;    // �`��Ώۂ�1��
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0�`255�w���RGBA
	gpipeline.RTVFormats[1] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0�`255�w���RGBA
	gpipeline.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O

	// �f�X�N���v�^�����W
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 ���W�X�^

	// ���[�g�p�����[�^
	//CD3DX12_ROOT_PARAMETER rootparams[2];
	CD3DX12_ROOT_PARAMETER rootparams[3];
	// CBV�i���W�ϊ��s��p�j
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	// SRV�i�e�N�X�`���j
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);
	//CBV(�X�L�j���O)
	rootparams[2].InitAsConstantBufferView(3, 0, D3D12_SHADER_VISIBILITY_ALL);

	// �X�^�e�B�b�N�T���v���[
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	// ���[�g�V�O�l�`���̐ݒ�
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	// �o�[�W������������̃V���A���C�Y
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	// ���[�g�V�O�l�`���̐���
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(rootsignature.ReleaseAndGetAddressOf()));
	if (FAILED(result)) { assert(0); }

	gpipeline.pRootSignature = rootsignature.Get();

	// �O���t�B�b�N�X�p�C�v���C���̐���
	result = device->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(pipelinestate.ReleaseAndGetAddressOf()));
	if (FAILED(result)) { assert(0); }
}

void FbxObject3D::InitializeConstMapTransform() {
	HRESULT result;
	//�q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES cbTransformHeapProp{};
	D3D12_HEAP_PROPERTIES cbMaterialHeapProp{};
	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC cbTransformResourceDesc{};
	D3D12_RESOURCE_DESC cbMaterialResourceDesc{};
	//�萔�o�b�t�@�̐���
	//�q�[�v�ݒ�
	cbTransformHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//GPU�ւ̓]���p

	//���\�[�X�ݒ�
	cbTransformResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbTransformResourceDesc.Width = (sizeof(ConstBufferDataTransform) + 0xff) & ~0xff;	//256�o�C�g�A���C�������g
	cbTransformResourceDesc.Height = 1;
	cbTransformResourceDesc.DepthOrArraySize = 1;
	cbTransformResourceDesc.MipLevels = 1;
	cbTransformResourceDesc.SampleDesc.Count = 1;
	cbTransformResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//�萔�o�b�t�@�̐���
	result = dx12base.GetDevice()->CreateCommittedResource(
		&cbTransformHeapProp,	//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&cbTransformResourceDesc,	//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffTransform)
	);
	assert(SUCCEEDED(result));


	//�萔�o�b�t�@�̃}�b�s���O
	result = constBuffTransform->Map(0, nullptr, (void**)&constMapTransform); // �}�b�s���O
	//assert(SUCCEEDED(result));

}

void FbxObject3D::InitializeConstMapMaterial() {

	HRESULT result;
	//�q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES cbTransformHeapProp{};
	D3D12_HEAP_PROPERTIES cbMaterialHeapProp{};
	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC cbTransformResourceDesc{};
	D3D12_RESOURCE_DESC cbMaterialResourceDesc{};

	//�萔�o�b�t�@�̐���
//�q�[�v�ݒ�
	cbMaterialHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//GPU�ւ̓]���p

	//���\�[�X�ݒ�
	cbMaterialResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbMaterialResourceDesc.Width = (sizeof(ConstBufferDataTransform) + 0xff) & ~0xff;	//256�o�C�g�A���C�������g
	cbMaterialResourceDesc.Height = 1;
	cbMaterialResourceDesc.DepthOrArraySize = 1;
	cbMaterialResourceDesc.MipLevels = 1;
	cbMaterialResourceDesc.SampleDesc.Count = 1;
	cbMaterialResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//�萔�o�b�t�@�̐���
	result = dx12base.GetDevice()->CreateCommittedResource(
		&cbMaterialHeapProp,	//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&cbMaterialResourceDesc,	//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffMaterial)
	);
	//assert(SUCCEEDED(result));

	//�萔�o�b�t�@�̃}�b�s���O
	//ConstBufferDataMaterial* constMapMaterial = nullptr;
	result = constBuffMaterial->Map(0, nullptr, (void**)&constMapMaterial); // �}�b�s���O
	//assert(SUCCEEDED(result));

	//�萔�o�b�t�@�ւ̃f�[�^�]��
	//�l���������ނƎ����I�ɓ]�������
	constMapMaterial->color = color;

}

void FbxObject3D::PlayAnimation()
{
	FbxScene* fbxScene = model->GetFbxScene();
	//0�ԃA�j���[�V�����擾
	FbxAnimStack* animstack = fbxScene->GetSrcObject<FbxAnimStack>(0);
	//�A�j���[�V�����̖��O�擾
	const char* animstackname = animstack->GetName();
	//�A�j���[�V�����̎��ԏ��
	FbxTakeInfo* takeinfo = fbxScene->GetTakeInfo(animstackname);

	//�J�n���Ԏ擾
	startTime = takeinfo->mLocalTimeSpan.GetStart();
	//�I�����Ԏ擾
	endTime = takeinfo->mLocalTimeSpan.GetStop();
	//�J�n���Ԃɍ��킹��
	currentTime = startTime;
	//�Đ�����Ԃɂ���
	isPlay = true;
}


void FbxObject3D::SetViewProjection(ViewProjection* viewProjection) {
	this->viewProjection = viewProjection;
}

void FbxObject3D::SetMatProjection(XMMATRIX* matProjection) {
	this->matProjection = matProjection;
}