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
	// アニメーションありか？
	if (m_Animation.count(AnimationName1) == 0)
		return;
	if (m_Animation.count(AnimationName2) == 0)
		return;

	if (!m_Animation[AnimationName1]->HasAnimations())
		return;
	if (!m_Animation[AnimationName2]->HasAnimations())
		return;

	// CPUスキニングでのアニメーションブレンド処理
	{
		//アニメーションデータからボーンマトリクス算出
		aiAnimation* animation1 = m_Animation[AnimationName1]->mAnimations[0];
		aiAnimation* animation2 = m_Animation[AnimationName2]->mAnimations[0];

		// 現在のアニメーションについて関連するボーンを全て更新
		for (unsigned int c = 0; c < animation1->mNumChannels; c++)
		{
			aiNodeAnim* nodeAnim = animation1->mChannels[c];

			BONE* bone = &m_Bone[nodeAnim->mNodeName.C_Str()];

			int f;

			f = Frame1 % nodeAnim->mNumRotationKeys;//簡易実装
			aiQuaternion rot = nodeAnim->mRotationKeys[f].mValue;

			f = Frame1 % nodeAnim->mNumPositionKeys;//簡易実装
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

			f = Frame2 % nodeAnim->mNumRotationKeys;//簡易実装
			aiQuaternion rot = nodeAnim->mRotationKeys[f].mValue;

			f = Frame2 % nodeAnim->mNumPositionKeys;//簡易実装
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
		rootMatrix = aiMatrix4x4(aiVector3D(1.0f, 1.0f, 1.0f), aiQuaternion(AI_MATH_PI, 0.0f, 0.0f), aiVector3D(0.0f, 0.0f, 0.0f));

		UpdateBoneMatrix(m_AiScene->mRootNode, rootMatrix);

		//頂点変換（CPUスキニング）
		for (unsigned int m = 0; m < m_AiScene->mNumMeshes; m++)
		{
			aiMesh* mesh = m_AiScene->mMeshes[m];

			D3D11_MAPPED_SUBRESOURCE ms;
			Renderer::GetDeviceContext()->Map(m_VertexBuffer[m], 0,
				D3D11_MAP_WRITE_DISCARD, 0, &ms);

			VERTEX_3D* vertex = (VERTEX_3D*)ms.pData;

			for (unsigned int v = 0; v < mesh->mNumVertices; v++)
			{
				DEFORM_VERTEX* deformVertex = &m_DeformVertex[m][v];

				aiMatrix4x4 matrix[4];
				aiMatrix4x4 outMatrix;
				matrix[0] = m_Bone[deformVertex->BoneName[0]].Matrix;
				matrix[1] = m_Bone[deformVertex->BoneName[1]].Matrix;
				matrix[2] = m_Bone[deformVertex->BoneName[2]].Matrix;
				matrix[3] = m_Bone[deformVertex->BoneName[3]].Matrix;

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
				deformVertex->Position *= outMatrix;						// 頂点座標×行列


				//法線変換用に移動成分を削除
				outMatrix.a4 = 0.0f;
				outMatrix.b4 = 0.0f;
				outMatrix.c4 = 0.0f;

				deformVertex->Normal = mesh->mNormals[v];
				deformVertex->Normal *= outMatrix;


				//頂点バッファへ書き込み
				vertex[v].Position.x = deformVertex->Position.x;
				vertex[v].Position.y = deformVertex->Position.y;
				vertex[v].Position.z = deformVertex->Position.z;

				vertex[v].Normal.x = deformVertex->Normal.x;
				vertex[v].Normal.y = deformVertex->Normal.y;
				vertex[v].Normal.z = deformVertex->Normal.z;

				vertex[v].TexCoord.x = mesh->mTextureCoords[0][v].x;
				vertex[v].TexCoord.y = mesh->mTextureCoords[0][v].y;

				vertex[v].Diffuse = Color(1.0f, 1.0f, 1.0f, 1.0f);
			}
			Renderer::GetDeviceContext()->Unmap(m_VertexBuffer[m], 0);
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

