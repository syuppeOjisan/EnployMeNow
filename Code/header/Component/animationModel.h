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

	ID3D11Buffer**	m_VertexBuffer;			// ���_�o�b�t�@
	ID3D11Buffer**	m_IndexBuffer;			// �C���f�b�N�X�o�b�t�@
	ID3D11Buffer*	m_BoneCombMtxCBuffer;	// �萔�o�b�t�@�@20230909-02

	std::unordered_map<std::string, ID3D11ShaderResourceView*> m_Texture;

	std::vector<DEFORM_VERTEX>* m_DeformVertex;				//�ό`�㒸�_�f�[�^
	std::unordered_map<std::string, BONE> m_Bone;			//�{�[���f�[�^�i���O�ŎQ�Ɓj

	const char*		m_NowAnimation;				// �Đ����̃A�j���[�V����
	const char*		m_PrevAnimation;			// �O��Đ����̃A�j���[�V����
	int				m_NowAnimationFrame = 0;	// �Đ����̃A�j���[�V�����t���[��
	int				m_PrevAnimationFrame = 0;	// �O�Đ����Ă����A�j���[�V�����t���[��
	float			m_BlendRate;				// �u�����h���[�g
	float			m_BlendSpeed = 0.1f;		// �u�����h���x
	bool			m_isAnimBlendOver = false;	// �A�j���[�V�����u�����h���I��������


	void CreateBone(aiNode* Node);
	void UpdateBoneMatrix(aiNode* Node, aiMatrix4x4 Matrix);

public:
	using Component::Component;

	void Load( const char *FileName );
	void LoadAnimation( const char *FileName, const char *Name );
	void Uninit() override;

	void Update(const char *AnimationName1, int Frame1, const char *AnimationName2, int Frame2, float BlendRate);		// �f�t�H���g
	void Update(int Frame1, int Frame2, float BlendRate);																// ���̃A�j���[�V�������w�肵�ău�����h
	void Update();																										// �u�����h���[�g�������o�Ŏ������o�[�W����

	void GPU_Update(const char* AnimationName1, int Frame1, const char* m_NowAnimation, int Frame2, float BlendRate);	// GPU�v�Z�f�t�H���g
	void GPU_Update(int Frame1, int Frame2, float BlendRate);															// �F�X�e�X�g

	void Draw() override;

	void SetFirstAnimation(const char* _firstAnimation);
	bool SetNextAnimation(const char* _nextAnimation);		// ���̃A�j���[�V������ݒ�

	bool GetIsAnimBlend(void) { return m_BlendRate; }		// �A�j���[�V�����u�����h���I��������ǂ������擾

	void SetBlendSpeed(float _speed) { this->m_BlendSpeed = _speed; }	// �u�����h���x��ݒ�

	const char* GetNowAnimName(void) { return m_NowAnimation; }
	const char* GetPrevAnimName(void) { return m_PrevAnimation; }
	const float* GetBlendRate(void) { return &m_BlendRate; }
};