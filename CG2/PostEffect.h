#include "Sprite.h"
class PostEffect :
	public Sprite
{
public:
	///<summary>
	///コンストラクタ
	///</summary>
	PostEffect();

	///<summary>
	///初期化
	///</summary>
	void Initialize();

	///<summary>
	///描画コマンドの発行
	///</summary>
	///<paramname="cmdList">コマンドリスト</param>
	void Draw(ID3D12GraphicsCommandList* cmdList);

	///<summary>
	///シーン描画前処理
	///</summary>
	///<paramname="cmdList">コマンドリスト</param>
	void PreDrawScene(ID3D12GraphicsCommandList* cmdList);
	///<summary>
	///シーン描画後処理
	///</summary>
	///<paramname="cmdList">コマンドリスト</param>
	void PostDrawScene(ID3D12GraphicsCommandList* cmdList);

	///<summary>///パイプライン生成///</summary>
	void CreateGraphicsPipelineState();
private:
	//テクスチャバッファ
	ComPtr<ID3D12Resource> texBuff;
	//SRV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeapSRV;
	//深度バッファ
	ComPtr<ID3D12Resource>depthBuff;
	//RTV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap>descHeapRTV;
	//DSV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap>descHeapDSV;
	//画面クリアカラー
	static const float clearColor[4];

	//グラフィックスパイプライン
	ComPtr<ID3D12PipelineState> pipelineState;
	//ルートシグネチャ
	ComPtr<ID3D12RootSignature> rootSignature;
};
