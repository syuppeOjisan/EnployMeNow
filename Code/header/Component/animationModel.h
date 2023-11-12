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
	std::unordered_map<int, const aiScene*> m_AnimationNew;

	ID3D11Buffer**	m_VertexBuffer;			// 頂点バッファ
	ID3D11Buffer**	m_IndexBuffer;			// インデックスバッファ
	ID3D11Buffer*	m_BoneCombMtxCBuffer;	// 定数バッファ　20230909-02

	std::unordered_map<std::string, ID3D11ShaderResourceView*> m_Texture;

	std::vector<DEFORM_VERTEX>* m_DeformVertex;				//変形後頂点データ
	std::unordered_map<std::string, BONE> m_Bone;			//ボーンデータ（名前で参照）

	void CreateBone(aiNode* Node);
	void UpdateBoneMatrix(aiNode* Node, aiMatrix4x4 Matrix);

public:
	using Component::Component;

	void Load( const char *FileName );
	void LoadAnimation( const char *FileName, const char *Name );
	void LoadAnimation(const char* _fileName, int _animID);
	void Uninit() override;
	void Update(const char *_prevAnimation, float _prevAnimationFrame, const char *_nowAnimation, float _nowAnimationFrame, float BlendRate);
	void Update(int _prevAnimationID, float _prevAnimationFrame, int _nowAnimationID, float _nowAnimationFrame, float BlendRate);


	void Draw() override;

	// 指定されたアニメーションがロードされているかをチェック
	bool CheckIsAnimation(const char* _animName);
	bool CheckIsAnimation(int _animID);
};