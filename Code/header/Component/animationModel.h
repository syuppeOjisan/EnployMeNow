#pragma once

#include <unordered_map>

#include "assimp/cimport.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/matrix4x4.h"

#pragma comment (lib, "assimp-vc142-mtd.lib")

#include "Base/component.h"

//変形後頂点構造体
struct DEFORM_VERTEX
{
	aiVector3D		Position;
	aiVector3D		Normal;
	int				BoneNum;
	std::string		BoneName[4];			// 本来はボーンインデックスで管理するべき
	float			BoneWeight[4];
	int				BoneIdx[4];				// ボーン情報へのインデックス 20230909
};

struct CBBoneCombMatrx {
	DirectX::XMFLOAT4X4 BoneCombMtx[400];			// ボーンコンビネーション行列
};

//ボーン構造体
struct BONE
{
	aiMatrix4x4		Matrix;						// 座標変換に使用する行列
	aiMatrix4x4		AnimationMatrix;			// 自分の事だけ考えた行列
	aiMatrix4x4		OffsetMatrix;				// ボーンオフセット行列
	aiQuaternion	BlendRFrom;					// モーションブレンドする際のFROM側
	aiQuaternion	BlendRTo;					// モーションブレンドする際のTO側
	aiVector3D		BlendPosFrom;				// モーションブレンドする際のFROM側
	aiVector3D		BlendPosTo;					// モーションブレンドする際のTO側
	int				idx;						// GPUに送る際に必要なボーンのインデックス
};

class AnimationModel : public Component
{
private:
	const aiScene* m_AiScene = nullptr;
	std::unordered_map<std::string, const aiScene*> m_Animation;

	ID3D11Buffer**	m_VertexBuffer;			// 頂点バッファ
	ID3D11Buffer**	m_IndexBuffer;			// インデックスバッファ
	ID3D11Buffer*	m_BoneCombMtxCBuffer;	// 定数バッファ　20230909-02

	std::unordered_map<std::string, ID3D11ShaderResourceView*> m_Texture;

	std::vector<DEFORM_VERTEX>* m_DeformVertex;				//変形後頂点データ
	std::unordered_map<std::string, BONE> m_Bone;			//ボーンデータ（名前で参照）

	const char*		m_NowAnimation;				// 再生中のアニメーション
	const char*		m_PrevAnimation;			// 前回再生中のアニメーション
	int				m_NowAnimationFrame = 0;	// 再生中のアニメーションフレーム
	int				m_PrevAnimationFrame = 0;	// 前再生していたアニメーションフレーム
	float			m_BlendRate;				// ブレンドレート
	float			m_BlendSpeed = 0.1f;		// ブレンド速度
	bool			m_isAnimBlendOver = false;	// アニメーションブレンドが終了したか


	void CreateBone(aiNode* Node);
	void UpdateBoneMatrix(aiNode* Node, aiMatrix4x4 Matrix);

public:
	using Component::Component;

	void Load( const char *FileName );
	void LoadAnimation( const char *FileName, const char *Name );
	void Uninit() override;

	void Update(const char *AnimationName1, int Frame1, const char *AnimationName2, int Frame2, float BlendRate);		// デフォルト
	void Update(int Frame1, int Frame2, float BlendRate);																// 次のアニメーションを指定してブレンド
	void Update();																										// ブレンドレートをメンバで持ったバージョン

	void GPU_Update(const char* AnimationName1, int Frame1, const char* m_NowAnimation, int Frame2, float BlendRate);	// GPU計算デフォルト
	void GPU_Update(int Frame1, int Frame2, float BlendRate);															// 色々テスト

	void Draw() override;

	void SetFirstAnimation(const char* _firstAnimation);
	bool SetNextAnimation(const char* _nextAnimation);		// 次のアニメーションを設定

	bool GetIsAnimBlend(void) { return m_BlendRate; }		// アニメーションブレンドが終わったかどうかを取得

	void SetBlendSpeed(float _speed) { this->m_BlendSpeed = _speed; }	// ブレンド速度を設定

	const char* GetNowAnimName(void) { return m_NowAnimation; }
	const char* GetPrevAnimName(void) { return m_PrevAnimation; }
	const float* GetBlendRate(void) { return &m_BlendRate; }
};