#include <SimpleMath.h>
#include "System/main.h"
#include "System/renderer.h"
#include "Component/animationModel.h"
#include "System/utility.h"

using namespace DirectX::SimpleMath;

void AnimationModel::Draw()
{
	// プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = Color(1.0f, 1.0f, 1.0f, 1.0f);
	material.Ambient = Color(1.0f, 1.0f, 1.0f, 1.0f);
	material.TextureEnable = true;
	Renderer::SetMaterial(material);

	// 20230909-02 レジスタ５にセット
	Renderer::GetDeviceContext()->VSSetConstantBuffers(5, 1, &m_BoneCombMtxCBuffer);

	for (unsigned int m = 0; m < m_AiScene->mNumMeshes; m++)
	{
		aiMesh* mesh = m_AiScene->mMeshes[m];

		aiMaterial* material = m_AiScene->mMaterials[mesh->mMaterialIndex];

		// テクスチャ設定
		aiString path;
		material->GetTexture(aiTextureType_DIFFUSE, 0, &path);
		Renderer::GetDeviceContext()->PSSetShaderResources(0, 1,
			&m_Texture[path.data]);

		// 頂点バッファ設定
		UINT stride = sizeof(VERTEX_3D);
		UINT offset = 0;
		Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1,
			&m_VertexBuffer[m], &stride, &offset);

		// インデックスバッファ設定
		Renderer::GetDeviceContext()->IASetIndexBuffer(
			m_IndexBuffer[m], DXGI_FORMAT_R32_UINT, 0);

		// ポリゴン描画
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


	//変形後頂点配列生成
	m_DeformVertex = new std::vector<DEFORM_VERTEX>[m_AiScene->mNumMeshes];

	//再帰的にボーン生成
	CreateBone(m_AiScene->mRootNode);

	// ボーンの配列位置を格納する				// 20230909
	unsigned int num = 0;						// 20230909
	for (auto& data : m_Bone) {					// 20230909
		data.second.idx = num;					// 20230909
		num++;									// 20230909
	}											// 20230909

	for (unsigned int m = 0; m < m_AiScene->mNumMeshes; m++)
	{
		aiMesh* mesh = m_AiScene->mMeshes[m];

		// 頂点バッファ生成
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


		// インデックスバッファ生成
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



		//変形後頂点データ初期化
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


		//ボーンデータ初期化
		for (unsigned int b = 0; b < mesh->mNumBones; b++)
		{
			aiBone* bone = mesh->mBones[b];

			m_Bone[bone->mName.C_Str()].OffsetMatrix = bone->mOffsetMatrix;

			//変形後頂点にボーンデータ格納
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

	// 頂点バッファにボーンインデックスと重み値をセットする　	20230910-01
	// deformvertexにセットされているデータを頂点バッファにセットする
	//頂点変換（CPUスキニング）
	for (unsigned int m = 0; m < m_AiScene->mNumMeshes; m++)
	{
		aiMesh* mesh = m_AiScene->mMeshes[m];

		// 頂点バッファロック
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

			// 頂点バッファを更新
			Renderer::GetDeviceContext()->Unmap(m_VertexBuffer[m], 0);  // 20230909-02
		}
	}

	//テクスチャ読み込み
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

	// 定数バッファ生成　20230909-02
	bool sts = CreateConstantBufferWrite(											// 20230909 - 02
		Renderer::GetDevice(),					    // デバイスオブジェクト			// 20230909 - 02
		sizeof(CBBoneCombMatrx),					// コンスタントバッファサイズ	// 20230909 - 02
		&m_BoneCombMtxCBuffer);						// コンスタントバッファ			// 20230909 - 02

	assert(m_BoneCombMtxCBuffer);
}

void AnimationModel::LoadAnimation(const char* FileName, const char* Name)
{

	m_Animation[Name] = aiImportFile(FileName, aiProcess_ConvertToLeftHanded);
	assert(m_Animation[Name]);

}

void AnimationModel::LoadAnimation(const char* _fileName, int _animID)
{
	m_AnimationNew[_animID] = aiImportFile(_fileName, aiProcess_ConvertToLeftHanded);
	assert(m_AnimationNew[_animID]);
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

void AnimationModel::Update(const char* _prevAnimation, float _prevAnimationFrame, const char* _nowAnimation, float _nowAnimationFrame, float BlendRate)
{
	// アニメーションありか？
	if (m_Animation.count(_prevAnimation) == 0)
		return;
	if (m_Animation.count(_nowAnimation) == 0)
		return;

	if (!m_Animation[_prevAnimation]->HasAnimations())
		return;
	if (!m_Animation[_nowAnimation]->HasAnimations())
		return;


	// CPUスキニングでのアニメーションブレンド処理
	{
		//アニメーションデータからボーンマトリクス算出
		aiAnimation* animation1 = m_Animation[_prevAnimation]->mAnimations[0];
		aiAnimation* animation2 = m_Animation[_nowAnimation]->mAnimations[0];

		// 現在のアニメーションについて関連するボーンを全て更新
		for (unsigned int c = 0; c < animation1->mNumChannels; c++)
		{
			aiNodeAnim* nodeAnim = animation1->mChannels[c];

			BONE* bone = &m_Bone[nodeAnim->mNodeName.C_Str()];

			int f;

			f = (int)_prevAnimationFrame % nodeAnim->mNumRotationKeys;//簡易実装
			aiQuaternion rot = nodeAnim->mRotationKeys[f].mValue;

			f = (int)_prevAnimationFrame % nodeAnim->mNumPositionKeys;//簡易実装
			aiVector3D pos = nodeAnim->mPositionKeys[f].mValue;

			bone->BlendPosFrom = pos;
			bone->BlendRFrom = rot;
		}

		// 現在のアニメーション2について関連するボーンを全て更新
		for (unsigned int c = 0; c < animation2->mNumChannels; c++)
		{
			aiNodeAnim* nodeAnim = animation2->mChannels[c];

			BONE* bone = &m_Bone[nodeAnim->mNodeName.C_Str()];

			int f;

			f = (int)_nowAnimationFrame % nodeAnim->mNumRotationKeys;//簡易実装
			aiQuaternion rot = nodeAnim->mRotationKeys[f].mValue;

			f = (int)_nowAnimationFrame % nodeAnim->mNumPositionKeys;//簡易実装
			aiVector3D pos = nodeAnim->mPositionKeys[f].mValue;

			bone->BlendPosTo = pos;
			bone->BlendRTo = rot;
		}

		// ブレンド
		for (unsigned int c = 0; c < animation2->mNumChannels; c++)
		{
			aiNodeAnim* nodeAnim = animation2->mChannels[c];

			BONE* bone = &m_Bone[nodeAnim->mNodeName.C_Str()];

			// 位置のブレンド
			aiVector3D pos1 = bone->BlendPosFrom;
			aiVector3D pos2 = bone->BlendPosTo;

			aiVector3D pos = pos1 * (1.0f - BlendRate) + pos2 * BlendRate;//線形補間

			// 姿勢のブレンド
			aiQuaternion rot1 = bone->BlendRFrom;
			aiQuaternion rot2 = bone->BlendRTo;

			aiQuaternion rot;
			aiQuaternion::Interpolate(rot, rot1, rot2, BlendRate);//球面線形補間

			bone->AnimationMatrix = aiMatrix4x4(aiVector3D(1.0f, 1.0f, 1.0f), rot, pos);
		}


		//再帰的にボーンマトリクスを更新
		aiMatrix4x4 rootMatrix = aiMatrix4x4(aiVector3D(1.0f, 1.0f, 1.0f), aiQuaternion(AI_MATH_PI, 0.0f, 0.0f), aiVector3D(0.0f, 0.0f, 0.0f));

		UpdateBoneMatrix(m_AiScene->mRootNode, rootMatrix);

		// 20230909 ボーンコンビネーション行列の配列を生成する
		std::vector<aiMatrix4x4> bonecombmtxcontainer;					// 20230909
		bonecombmtxcontainer.resize(m_Bone.size());						// 20230909
		for (auto data : m_Bone) {										// 20230909
			bonecombmtxcontainer[data.second.idx] = data.second.Matrix;	// 20230909
		}																// 20230909

		// 20230909 転置
		for (auto& bcmtx : bonecombmtxcontainer)
		{
			// 転置する
			bcmtx.Transpose();
		}

		// 20230909-02 定数バッファに反映させる
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
	}

}

void AnimationModel::Update(int _prevAnimationID, float _prevAnimationFrame, int _nowAnimationID, float _nowAnimationFrame, float BlendRate)
{
	// アニメーションありか？
	if (m_AnimationNew.count(_prevAnimationID) == 0)
		return;
	if (m_AnimationNew.count(_nowAnimationID) == 0)
		return;

	if (!m_AnimationNew[_prevAnimationID]->HasAnimations())
		return;
	if (!m_AnimationNew[_nowAnimationID]->HasAnimations())
		return;


	// CPUスキニングでのアニメーションブレンド処理
	{
		//アニメーションデータからボーンマトリクス算出
		aiAnimation* animation1 = m_AnimationNew[_prevAnimationID]->mAnimations[0];
		aiAnimation* animation2 = m_AnimationNew[_nowAnimationID]->mAnimations[0];

		// 現在のアニメーションについて関連するボーンを全て更新
		for (unsigned int c = 0; c < animation1->mNumChannels; c++)
		{
			aiNodeAnim* nodeAnim = animation1->mChannels[c];

			BONE* bone = &m_Bone[nodeAnim->mNodeName.C_Str()];

			int f;

			f = (int)_prevAnimationFrame % nodeAnim->mNumRotationKeys;//簡易実装
			aiQuaternion rot = nodeAnim->mRotationKeys[f].mValue;

			f = (int)_prevAnimationFrame % nodeAnim->mNumPositionKeys;//簡易実装
			aiVector3D pos = nodeAnim->mPositionKeys[f].mValue;

			bone->BlendPosFrom = pos;
			bone->BlendRFrom = rot;
		}

		// 現在のアニメーション2について関連するボーンを全て更新
		for (unsigned int c = 0; c < animation2->mNumChannels; c++)
		{
			aiNodeAnim* nodeAnim = animation2->mChannels[c];

			BONE* bone = &m_Bone[nodeAnim->mNodeName.C_Str()];

			int f;

			f = (int)_nowAnimationFrame % nodeAnim->mNumRotationKeys;//簡易実装
			aiQuaternion rot = nodeAnim->mRotationKeys[f].mValue;

			f = (int)_nowAnimationFrame % nodeAnim->mNumPositionKeys;//簡易実装
			aiVector3D pos = nodeAnim->mPositionKeys[f].mValue;

			bone->BlendPosTo = pos;
			bone->BlendRTo = rot;
		}

		// ブレンド
		for (unsigned int c = 0; c < animation2->mNumChannels; c++)
		{
			aiNodeAnim* nodeAnim = animation2->mChannels[c];

			BONE* bone = &m_Bone[nodeAnim->mNodeName.C_Str()];

			// 位置のブレンド
			aiVector3D pos1 = bone->BlendPosFrom;
			aiVector3D pos2 = bone->BlendPosTo;

			aiVector3D pos = pos1 * (1.0f - BlendRate) + pos2 * BlendRate;//線形補間

			// 姿勢のブレンド
			aiQuaternion rot1 = bone->BlendRFrom;
			aiQuaternion rot2 = bone->BlendRTo;

			aiQuaternion rot;
			aiQuaternion::Interpolate(rot, rot1, rot2, BlendRate);//球面線形補間

			bone->AnimationMatrix = aiMatrix4x4(aiVector3D(1.0f, 1.0f, 1.0f), rot, pos);
		}


		//再帰的にボーンマトリクスを更新
		aiMatrix4x4 rootMatrix = aiMatrix4x4(aiVector3D(1.0f, 1.0f, 1.0f), aiQuaternion(AI_MATH_PI, 0.0f, 0.0f), aiVector3D(0.0f, 0.0f, 0.0f));

		UpdateBoneMatrix(m_AiScene->mRootNode, rootMatrix);

		// 20230909 ボーンコンビネーション行列の配列を生成する
		std::vector<aiMatrix4x4> bonecombmtxcontainer;					// 20230909
		bonecombmtxcontainer.resize(m_Bone.size());						// 20230909
		for (auto data : m_Bone) {										// 20230909
			bonecombmtxcontainer[data.second.idx] = data.second.Matrix;	// 20230909
		}																// 20230909

		// 20230909 転置
		for (auto& bcmtx : bonecombmtxcontainer)
		{
			// 転置する
			bcmtx.Transpose();
		}

		// 20230909-02 定数バッファに反映させる
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
	}
}

void AnimationModel::UpdateBoneMatrix(aiNode* node, aiMatrix4x4 matrix)
{
	// 引数で渡されたノード名をキーとしてボーン情報を取得する
	BONE* bone = &m_Bone[node->mName.C_Str()];

	//マトリクスの乗算順番に注意
	aiMatrix4x4 worldMatrix;					// 初期値は単位行列

	//	worldMatrix *= matrix;						

	worldMatrix = matrix;						// 親の位置、姿勢が初期状態（逆ボーンオフセット行列）
	worldMatrix *= bone->AnimationMatrix;		// 引数で渡された行列を掛け算（自ノードのアニメーションを反映させる）（ボーン空間でのもの）


	bone->Matrix = worldMatrix;					// プログラム内に用意している行列に反映させる
	bone->Matrix *= bone->OffsetMatrix;			// オフセット行列を反映させる

	for (unsigned int n = 0; n < node->mNumChildren; n++)
	{
		UpdateBoneMatrix(node->mChildren[n], worldMatrix);
	}
}

bool AnimationModel::CheckIsAnimation(const char* _animName)
{
	// 指定されたアニメーションがロードされているかをチェック
	if (m_Animation.count(_animName) == 0)
		return false;
	if (!m_Animation[_animName]->HasAnimations())
		return false;

	return true;
}

bool AnimationModel::CheckIsAnimation(int _animID)
{
	// 指定されたアニメーションがロードされているかをチェック
	if (m_AnimationNew.count(_animID) == 0)
		return false;
	if (!m_AnimationNew[_animID]->HasAnimations())
		return false;

	return true;
}
