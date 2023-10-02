#include <SimpleMath.h>
#include "System/main.h"
#include "System/renderer.h"
#include "Component/animationModel.h"
#include "System/utility.h"

using namespace DirectX::SimpleMath;

void AnimationModel::Draw()
{
	// �v���~�e�B�u�g�|���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = Color(1.0f, 1.0f, 1.0f, 1.0f);
	material.Ambient = Color(1.0f, 1.0f, 1.0f, 1.0f);
	material.TextureEnable = true;
	Renderer::SetMaterial(material);

	// 20230909-02 ���W�X�^�T�ɃZ�b�g
	Renderer::GetDeviceContext()->VSSetConstantBuffers(5, 1, &m_BoneCombMtxCBuffer);

	for (unsigned int m = 0; m < m_AiScene->mNumMeshes; m++)
	{
		aiMesh* mesh = m_AiScene->mMeshes[m];

		aiMaterial* material = m_AiScene->mMaterials[mesh->mMaterialIndex];

		// �e�N�X�`���ݒ�
		aiString path;
		material->GetTexture(aiTextureType_DIFFUSE, 0, &path);
		Renderer::GetDeviceContext()->PSSetShaderResources(0, 1,
			&m_Texture[path.data]);

		// ���_�o�b�t�@�ݒ�
		UINT stride = sizeof(VERTEX_3D);
		UINT offset = 0;
		Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1,
			&m_VertexBuffer[m], &stride, &offset);

		// �C���f�b�N�X�o�b�t�@�ݒ�
		Renderer::GetDeviceContext()->IASetIndexBuffer(
			m_IndexBuffer[m], DXGI_FORMAT_R32_UINT, 0);

		// �|���S���`��
		Renderer::GetDeviceContext()->DrawIndexed(
			mesh->mNumFaces * 3, 0, 0);
	}
}

void AnimationModel::Load(const char* FileName)
{
	const std::string modelPath(FileName);

	m_AiScene = aiImportFile(FileName, aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_ConvertToLeftHanded);
	assert(m_AiScene);

	m_VertexBuffer = new ID3D11Buffer * [m_AiScene->mNumMeshes];
	m_IndexBuffer = new ID3D11Buffer * [m_AiScene->mNumMeshes];


	//�ό`�㒸�_�z�񐶐�
	m_DeformVertex = new std::vector<DEFORM_VERTEX>[m_AiScene->mNumMeshes];

	//�ċA�I�Ƀ{�[������
	CreateBone(m_AiScene->mRootNode);

	// �{�[���̔z��ʒu���i�[����				// 20230909
	unsigned int num = 0;						// 20230909
	for (auto& data : m_Bone) {					// 20230909
		data.second.idx = num;					// 20230909
		num++;									// 20230909
	}											// 20230909

	for (unsigned int m = 0; m < m_AiScene->mNumMeshes; m++)
	{
		aiMesh* mesh = m_AiScene->mMeshes[m];

		// ���_�o�b�t�@����
		{
			VERTEX_3D* vertex = new VERTEX_3D[mesh->mNumVertices];

			for (unsigned int v = 0; v < mesh->mNumVertices; v++)
			{
				vertex[v].Position = Vector3(mesh->mVertices[v].x, mesh->mVertices[v].y, mesh->mVertices[v].z);
				vertex[v].Normal = Vector3(mesh->mNormals[v].x, mesh->mNormals[v].y, mesh->mNormals[v].z);
				vertex[v].TexCoord = Vector2(mesh->mTextureCoords[0][v].x, mesh->mTextureCoords[0][v].y);
				vertex[v].Diffuse = Color(1.0f, 1.0f, 1.0f, 1.0f);
			}

			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DYNAMIC;
			bd.ByteWidth = sizeof(VERTEX_3D) * mesh->mNumVertices;
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

			D3D11_SUBRESOURCE_DATA sd;
			ZeroMemory(&sd, sizeof(sd));
			sd.pSysMem = vertex;

			Renderer::GetDevice()->CreateBuffer(&bd, &sd,
				&m_VertexBuffer[m]);

			delete[] vertex;
		}


		// �C���f�b�N�X�o�b�t�@����
		{
			unsigned int* index = new unsigned int[mesh->mNumFaces * 3];

			for (unsigned int f = 0; f < mesh->mNumFaces; f++)
			{
				const aiFace* face = &mesh->mFaces[f];

				assert(face->mNumIndices == 3);

				index[f * 3 + 0] = face->mIndices[0];
				index[f * 3 + 1] = face->mIndices[1];
				index[f * 3 + 2] = face->mIndices[2];
			}

			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(unsigned int) * mesh->mNumFaces * 3;
			bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bd.CPUAccessFlags = 0;

			D3D11_SUBRESOURCE_DATA sd;
			ZeroMemory(&sd, sizeof(sd));
			sd.pSysMem = index;

			Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_IndexBuffer[m]);

			delete[] index;
		}



		//�ό`�㒸�_�f�[�^������
		for (unsigned int v = 0; v < mesh->mNumVertices; v++)
		{
			DEFORM_VERTEX deformVertex;
			deformVertex.Position = mesh->mVertices[v];
			deformVertex.Normal = mesh->mNormals[v];
			deformVertex.BoneNum = 0;

			for (unsigned int b = 0; b < 4; b++)
			{
				deformVertex.BoneName[b] = "";
				deformVertex.BoneWeight[b] = 0.0f;
			}

			m_DeformVertex[m].push_back(deformVertex);
		}


		//�{�[���f�[�^������
		for (unsigned int b = 0; b < mesh->mNumBones; b++)
		{
			aiBone* bone = mesh->mBones[b];

			m_Bone[bone->mName.C_Str()].OffsetMatrix = bone->mOffsetMatrix;

			//�ό`�㒸�_�Ƀ{�[���f�[�^�i�[
			for (unsigned int w = 0; w < bone->mNumWeights; w++)
			{
				aiVertexWeight weight = bone->mWeights[w];

				int num = m_DeformVertex[m][weight.mVertexId].BoneNum;

				m_DeformVertex[m][weight.mVertexId].BoneWeight[num] = weight.mWeight;
				m_DeformVertex[m][weight.mVertexId].BoneName[num] = bone->mName.C_Str();

				// 20230909
				m_DeformVertex[m][weight.mVertexId].BoneIdx[num] = m_Bone[bone->mName.C_Str()].idx;

				m_DeformVertex[m][weight.mVertexId].BoneNum++;

				assert(m_DeformVertex[m][weight.mVertexId].BoneNum <= 4);
			}
		}
	}

	// ���_�o�b�t�@�Ƀ{�[���C���f�b�N�X�Əd�ݒl���Z�b�g����@	20230910-01
	// deformvertex�ɃZ�b�g����Ă���f�[�^�𒸓_�o�b�t�@�ɃZ�b�g����
	//���_�ϊ��iCPU�X�L�j���O�j
	for (unsigned int m = 0; m < m_AiScene->mNumMeshes; m++)
	{
		aiMesh* mesh = m_AiScene->mMeshes[m];

		// ���_�o�b�t�@���b�N
		D3D11_MAPPED_SUBRESOURCE ms;
		HRESULT hr = Renderer::GetDeviceContext()->Map(m_VertexBuffer[m], 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
		if (SUCCEEDED(hr)) {
			VERTEX_3D* vertex = (VERTEX_3D*)ms.pData;

			for (unsigned int v = 0; v < mesh->mNumVertices; v++)
			{
				DEFORM_VERTEX* deformVertex = &m_DeformVertex[m][v];

				vertex->Position = Vector3(mesh->mVertices[v].x, mesh->mVertices[v].y, mesh->mVertices[v].z);
				vertex->Normal = Vector3(mesh->mNormals[v].x, mesh->mNormals[v].y, mesh->mNormals[v].z);
				vertex->TexCoord = Vector2(mesh->mTextureCoords[0][v].x, mesh->mTextureCoords[0][v].y);
				vertex->Diffuse = Color(1.0f, 1.0f, 1.0f, 1.0f);

				vertex->BoneIndex[0] = deformVertex->BoneIdx[0];
				vertex->BoneIndex[1] = deformVertex->BoneIdx[1];
				vertex->BoneIndex[2] = deformVertex->BoneIdx[2];
				vertex->BoneIndex[3] = deformVertex->BoneIdx[3];

				vertex->BoneWeight[0] = deformVertex->BoneWeight[0];
				vertex->BoneWeight[1] = deformVertex->BoneWeight[1];
				vertex->BoneWeight[2] = deformVertex->BoneWeight[2];
				vertex->BoneWeight[3] = deformVertex->BoneWeight[3];

				vertex++;
			}

			// ���_�o�b�t�@���X�V
			Renderer::GetDeviceContext()->Unmap(m_VertexBuffer[m], 0);  // 20230909-02
		}
	}

	//�e�N�X�`���ǂݍ���
	for (int i = 0; i < m_AiScene->mNumTextures; i++)
	{
		ID3D11ShaderResourceView* texture;

		aiTexture* aitexture = m_AiScene->mTextures[i];

		// mHeight is 0, so try to load a compressed texture of mWidth bytes
		const size_t size = aitexture->mWidth;

		HRESULT hr = DirectX::CreateWICTextureFromMemory(
			Renderer::GetDevice(),
			Renderer::GetDeviceContext(),
			reinterpret_cast<const unsigned char*>(aitexture->pcData),
			aitexture->mWidth,
			nullptr,
			&texture);

		assert(texture);

		m_Texture[aitexture->mFilename.data] = texture;
	}

	// �萔�o�b�t�@�����@20230909-02
	bool sts = CreateConstantBufferWrite(											// 20230909 - 02
		Renderer::GetDevice(),					    // �f�o�C�X�I�u�W�F�N�g			// 20230909 - 02
		sizeof(CBBoneCombMatrx),					// �R���X�^���g�o�b�t�@�T�C�Y	// 20230909 - 02
		&m_BoneCombMtxCBuffer);						// �R���X�^���g�o�b�t�@			// 20230909 - 02

	assert(m_BoneCombMtxCBuffer);

}


void AnimationModel::LoadAnimation(const char* FileName, const char* Name)
{

	m_Animation[Name] = aiImportFile(FileName, aiProcess_ConvertToLeftHanded);
	assert(m_Animation[Name]);

}


void AnimationModel::CreateBone(aiNode* node)
{
	BONE bone;

	m_Bone[node->mName.C_Str()] = bone;

	for (unsigned int n = 0; n < node->mNumChildren; n++)
	{
		CreateBone(node->mChildren[n]);
	}

}


void AnimationModel::Uninit()
{
	for (unsigned int m = 0; m < m_AiScene->mNumMeshes; m++)
	{
		m_VertexBuffer[m]->Release();
		m_IndexBuffer[m]->Release();
	}

	delete[] m_VertexBuffer;
	delete[] m_IndexBuffer;

	delete[] m_DeformVertex;


	for (std::pair<const std::string, ID3D11ShaderResourceView*> pair : m_Texture)
	{
		pair.second->Release();
	}

	m_BoneCombMtxCBuffer->Release();						// 20230909-02

	aiReleaseImport(m_AiScene);


	for (std::pair<const std::string, const aiScene*> pair : m_Animation)
	{
		aiReleaseImport(pair.second);
	}
}

void AnimationModel::Update(const char* AnimationName1, int Frame1, const char* AnimationName2, int Frame2, float BlendRate)
{

	// �A�j���[�V�������肩�H
	if (m_Animation.count(AnimationName1) == 0)
		return;
	if (m_Animation.count(AnimationName2) == 0)
		return;

	if (!m_Animation[AnimationName1]->HasAnimations())
		return;
	if (!m_Animation[AnimationName2]->HasAnimations())
		return;

	//�A�j���[�V�����f�[�^����{�[���}�g���N�X�Z�o
	aiAnimation* animation1 = m_Animation[AnimationName1]->mAnimations[0];
	aiAnimation* animation2 = m_Animation[AnimationName2]->mAnimations[0];


	for (unsigned int c = 0; c < animation1->mNumChannels; c++)
	{
		aiNodeAnim* nodeAnim = animation1->mChannels[c];

		BONE* bone = &m_Bone[nodeAnim->mNodeName.C_Str()];

		int f;

		f = Frame1 % nodeAnim->mNumRotationKeys;				//�ȈՎ���
		aiQuaternion rot = nodeAnim->mRotationKeys[f].mValue;

		f = Frame1 % nodeAnim->mNumPositionKeys;				//�ȈՎ���
		aiVector3D pos = nodeAnim->mPositionKeys[f].mValue;

		bone->AnimationMatrix = aiMatrix4x4(aiVector3D(1.0f, 1.0f, 1.0f), rot, pos);
	}

	//�ċA�I�Ƀ{�[���}�g���N�X���X�V
//	aiMatrix4x4 rootMatrix = aiMatrix4x4(aiVector3D(1.0f, 1.0f, 1.0f), aiQuaternion(AI_MATH_PI, 0.0f, 0.0f), aiVector3D(0.0f, 0.0f, 0.0f));
	aiMatrix4x4 rootMatrix = aiMatrix4x4(aiVector3D(1.0f, 1.0f, 1.0f), aiQuaternion(AI_MATH_PI, 0.0f, 0.0f), aiVector3D(0.0f, 0.0f, 0.0f));

	UpdateBoneMatrix(m_AiScene->mRootNode, rootMatrix);

	// 20230909 �{�[���R���r�l�[�V�����s��̔z��𐶐�����
	std::vector<aiMatrix4x4> bonecombmtxcontainer;					// 20230909
	bonecombmtxcontainer.resize(m_Bone.size());						// 20230909
	for (auto data : m_Bone) {										// 20230909
		bonecombmtxcontainer[data.second.idx] = data.second.Matrix;	// 20230909
	}																// 20230909

	// 20230909 �]�u
	for (auto& bcmtx : bonecombmtxcontainer)
	{
		// �]�u����
		bcmtx.Transpose();
	}

	// 20230909-02 �萔�o�b�t�@�ɔ��f������
	D3D11_MAPPED_SUBRESOURCE MappedResource;
	CBBoneCombMatrx* pData = nullptr;

	HRESULT hr = Renderer::GetDeviceContext()->Map(
		m_BoneCombMtxCBuffer,
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&MappedResource);

	if (SUCCEEDED(hr)) {
		pData = (CBBoneCombMatrx*)MappedResource.pData;
		memcpy(pData->BoneCombMtx,
			bonecombmtxcontainer.data(),
			sizeof(aiMatrix4x4) * bonecombmtxcontainer.size());
		Renderer::GetDeviceContext()->Unmap(m_BoneCombMtxCBuffer, 0);
	}

	/*
		//���_�ϊ��iCPU�X�L�j���O�j
		for (unsigned int m = 0; m < m_AiScene->mNumMeshes; m++)
		{
			aiMesh* mesh = m_AiScene->mMeshes[m];

	//  20230909-02
	//		D3D11_MAPPED_SUBRESOURCE ms;
	//		Renderer::GetDeviceContext()->Map(m_VertexBuffer[m], 0,
	//			D3D11_MAP_WRITE_DISCARD, 0, &ms);

	//		VERTEX_3D *vertex = (VERTEX_3D*)ms.pData;


			for (unsigned int v = 0; v < mesh->mNumVertices; v++)
			{
				DEFORM_VERTEX *deformVertex = &m_DeformVertex[m][v];

				int bonematrixindex[4];

				// �ǉ� 20230909
				bonematrixindex[0] = deformVertex->BoneIdx[0];
				bonematrixindex[1] = deformVertex->BoneIdx[1];
				bonematrixindex[2] = deformVertex->BoneIdx[2];
				bonematrixindex[3] = deformVertex->BoneIdx[3];

				aiMatrix4x4 matrix[4];
				aiMatrix4x4 outMatrix;

	//	20230909			matrix[0] = m_Bone[deformVertex->BoneName[0]].Matrix;
	//	20230909			matrix[1] = m_Bone[deformVertex->BoneName[1]].Matrix;
	//	20230909			matrix[2] = m_Bone[deformVertex->BoneName[2]].Matrix;
	//	20230909			matrix[3] = m_Bone[deformVertex->BoneName[3]].Matrix;

				aiMatrix4x4 zeromatrix = {					//	20230909
					0.0f,0.0f,0.0f,0.0f,					//	20230909
					0.0f,0.0f,0.0f,0.0f,					//	20230909
					0.0f,0.0f,0.0f,0.0f,					//	20230909
					0.0f,0.0f,0.0f,0.0f						//	20230909
				};											//	20230909

				for (int i = 0; i < 4; i++) {				//	20230909
					matrix[i] = zeromatrix;					//	20230909
				}											//	20230909

				for (int i = 0; i < deformVertex->BoneNum; i++) {				//	20230909
					matrix[i] = bonecombmtxcontainer[bonematrixindex[i]];		//	20230909
				}																//	20230909

				{
					outMatrix.a1 = matrix[0].a1 * deformVertex->BoneWeight[0]
								+ matrix[1].a1 * deformVertex->BoneWeight[1]
								+ matrix[2].a1 * deformVertex->BoneWeight[2]
								+ matrix[3].a1 * deformVertex->BoneWeight[3];

					outMatrix.a2 = matrix[0].a2 * deformVertex->BoneWeight[0]
								+ matrix[1].a2 * deformVertex->BoneWeight[1]
								+ matrix[2].a2 * deformVertex->BoneWeight[2]
								+ matrix[3].a2 * deformVertex->BoneWeight[3];

					outMatrix.a3 = matrix[0].a3 * deformVertex->BoneWeight[0]
								+ matrix[1].a3 * deformVertex->BoneWeight[1]
								+ matrix[2].a3 * deformVertex->BoneWeight[2]
								+ matrix[3].a3 * deformVertex->BoneWeight[3];

					outMatrix.a4 = matrix[0].a4 * deformVertex->BoneWeight[0]
								+ matrix[1].a4 * deformVertex->BoneWeight[1]
								+ matrix[2].a4 * deformVertex->BoneWeight[2]
								+ matrix[3].a4 * deformVertex->BoneWeight[3];



					outMatrix.b1 = matrix[0].b1 * deformVertex->BoneWeight[0]
								+ matrix[1].b1 * deformVertex->BoneWeight[1]
								+ matrix[2].b1 * deformVertex->BoneWeight[2]
								+ matrix[3].b1 * deformVertex->BoneWeight[3];

					outMatrix.b2 = matrix[0].b2 * deformVertex->BoneWeight[0]
						+ matrix[1].b2 * deformVertex->BoneWeight[1]
						+ matrix[2].b2 * deformVertex->BoneWeight[2]
						+ matrix[3].b2 * deformVertex->BoneWeight[3];

					outMatrix.b3 = matrix[0].b3 * deformVertex->BoneWeight[0]
						+ matrix[1].b3 * deformVertex->BoneWeight[1]
						+ matrix[2].b3 * deformVertex->BoneWeight[2]
						+ matrix[3].b3 * deformVertex->BoneWeight[3];

					outMatrix.b4 = matrix[0].b4 * deformVertex->BoneWeight[0]
						+ matrix[1].b4 * deformVertex->BoneWeight[1]
						+ matrix[2].b4 * deformVertex->BoneWeight[2]
						+ matrix[3].b4 * deformVertex->BoneWeight[3];



					outMatrix.c1 = matrix[0].c1 * deformVertex->BoneWeight[0]
						+ matrix[1].c1 * deformVertex->BoneWeight[1]
						+ matrix[2].c1 * deformVertex->BoneWeight[2]
						+ matrix[3].c1 * deformVertex->BoneWeight[3];

					outMatrix.c2 = matrix[0].c2 * deformVertex->BoneWeight[0]
						+ matrix[1].c2 * deformVertex->BoneWeight[1]
						+ matrix[2].c2 * deformVertex->BoneWeight[2]
						+ matrix[3].c2 * deformVertex->BoneWeight[3];

					outMatrix.c3 = matrix[0].c3 * deformVertex->BoneWeight[0]
						+ matrix[1].c3 * deformVertex->BoneWeight[1]
						+ matrix[2].c3 * deformVertex->BoneWeight[2]
						+ matrix[3].c3 * deformVertex->BoneWeight[3];

					outMatrix.c4 = matrix[0].c4 * deformVertex->BoneWeight[0]
						+ matrix[1].c4 * deformVertex->BoneWeight[1]
						+ matrix[2].c4 * deformVertex->BoneWeight[2]
						+ matrix[3].c4 * deformVertex->BoneWeight[3];



					outMatrix.d1 = matrix[0].d1 * deformVertex->BoneWeight[0]
						+ matrix[1].d1 * deformVertex->BoneWeight[1]
						+ matrix[2].d1 * deformVertex->BoneWeight[2]
						+ matrix[3].d1 * deformVertex->BoneWeight[3];

					outMatrix.d2 = matrix[0].d2 * deformVertex->BoneWeight[0]
						+ matrix[1].d2 * deformVertex->BoneWeight[1]
						+ matrix[2].d2 * deformVertex->BoneWeight[2]
						+ matrix[3].d2 * deformVertex->BoneWeight[3];

					outMatrix.d3 = matrix[0].d3 * deformVertex->BoneWeight[0]
						+ matrix[1].d3 * deformVertex->BoneWeight[1]
						+ matrix[2].d3 * deformVertex->BoneWeight[2]
						+ matrix[3].d3 * deformVertex->BoneWeight[3];

					outMatrix.d4 = matrix[0].d4 * deformVertex->BoneWeight[0]
						+ matrix[1].d4 * deformVertex->BoneWeight[1]
						+ matrix[2].d4 * deformVertex->BoneWeight[2]
						+ matrix[3].d4 * deformVertex->BoneWeight[3];

				}

				deformVertex->Position = mesh->mVertices[v];
				deformVertex->Position *= outMatrix;						// ���_���W�~�s��


				//�@���ϊ��p�Ɉړ��������폜
				outMatrix.a4 = 0.0f;
				outMatrix.b4 = 0.0f;
				outMatrix.c4 = 0.0f;

				deformVertex->Normal = mesh->mNormals[v];
				deformVertex->Normal *= outMatrix;


				//���_�o�b�t�@�֏�������
	//			vertex[v].Position.x = deformVertex->Position.x;
	//			vertex[v].Position.y = deformVertex->Position.y;
	//			vertex[v].Position.z = deformVertex->Position.z;

	//			vertex[v].Normal.x = deformVertex->Normal.x;
	//			vertex[v].Normal.y = deformVertex->Normal.y;
	//			vertex[v].Normal.z = deformVertex->Normal.z;

	//			vertex[v].TexCoord.x = mesh->mTextureCoords[0][v].x;
	//			vertex[v].TexCoord.y = mesh->mTextureCoords[0][v].y;

	//			vertex[v].Diffuse = Color(1.0f, 1.0f, 1.0f, 1.0f);
			}
	//		Renderer::GetDeviceContext()->Unmap(m_VertexBuffer[m], 0);  20230909-02

		}
	*/

}

void AnimationModel::UpdateBoneMatrix(aiNode* node, aiMatrix4x4 matrix)
{
	// �����œn���ꂽ�m�[�h�����L�[�Ƃ��ă{�[�������擾����
	BONE* bone = &m_Bone[node->mName.C_Str()];

	//�}�g���N�X�̏�Z���Ԃɒ���
	aiMatrix4x4 worldMatrix;					// �����l�͒P�ʍs��

	//	worldMatrix *= matrix;						

	worldMatrix = matrix;						// �e�̈ʒu�A�p����������ԁi�t�{�[���I�t�Z�b�g�s��j
	worldMatrix *= bone->AnimationMatrix;		// �����œn���ꂽ�s����|���Z�i���m�[�h�̃A�j���[�V�����𔽉f������j�i�{�[����Ԃł̂��́j


	bone->Matrix = worldMatrix;					// �v���O�������ɗp�ӂ��Ă���s��ɔ��f������
	bone->Matrix *= bone->OffsetMatrix;			// �I�t�Z�b�g�s��𔽉f������

	for (unsigned int n = 0; n < node->mNumChildren; n++)
	{
		UpdateBoneMatrix(node->mChildren[n], worldMatrix);
	}
}

