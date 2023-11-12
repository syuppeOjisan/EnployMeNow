#pragma once

#include <unordered_map>

#include "assimp/cimport.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/matrix4x4.h"

#pragma comment (lib, "assimp-vc142-mtd.lib")

#include "Base/component.h"

//�ό`�㒸�_�\����
struct DEFORM_VERTEX
{
	aiVector3D		Position;
	aiVector3D		Normal;
	int				BoneNum;
	std::string		BoneName[4];			// �{���̓{�[���C���f�b�N�X�ŊǗ�����ׂ�
	float			BoneWeight[4];
	int				BoneIdx[4];				// �{�[�����ւ̃C���f�b�N�X 20230909
};

struct CBBoneCombMatrx {
	DirectX::XMFLOAT4X4 BoneCombMtx[400];			// �{�[���R���r�l�[�V�����s��
};

//�{�[���\����
struct BONE
{
	aiMatrix4x4		Matrix;						// ���W�ϊ��Ɏg�p����s��
	aiMatrix4x4		AnimationMatrix;			// �����̎������l�����s��
	aiMatrix4x4		OffsetMatrix;				// �{�[���I�t�Z�b�g�s��
	aiQuaternion	BlendRFrom;					// ���[�V�����u�����h����ۂ�FROM��
	aiQuaternion	BlendRTo;					// ���[�V�����u�����h����ۂ�TO��
	aiVector3D		BlendPosFrom;				// ���[�V�����u�����h����ۂ�FROM��
	aiVector3D		BlendPosTo;					// ���[�V�����u�����h����ۂ�TO��
	int				idx;						// GPU�ɑ���ۂɕK�v�ȃ{�[���̃C���f�b�N�X
};

class AnimationModel : public Component
{
private:
	const aiScene* m_AiScene = nullptr;
	std::unordered_map<std::string, const aiScene*> m_Animation;
	std::unordered_map<int, const aiScene*> m_AnimationNew;

	ID3D11Buffer**	m_VertexBuffer;			// ���_�o�b�t�@
	ID3D11Buffer**	m_IndexBuffer;			// �C���f�b�N�X�o�b�t�@
	ID3D11Buffer*	m_BoneCombMtxCBuffer;	// �萔�o�b�t�@�@20230909-02

	std::unordered_map<std::string, ID3D11ShaderResourceView*> m_Texture;

	std::vector<DEFORM_VERTEX>* m_DeformVertex;				//�ό`�㒸�_�f�[�^
	std::unordered_map<std::string, BONE> m_Bone;			//�{�[���f�[�^�i���O�ŎQ�Ɓj

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

	// �w�肳�ꂽ�A�j���[�V���������[�h����Ă��邩���`�F�b�N
	bool CheckIsAnimation(const char* _animName);
	bool CheckIsAnimation(int _animID);
};