#include "common.hlsl"

void main(in VSONESKIN_IN In, out PS_IN Out)
{
        // モーション計算
    {
     	// ワンスキン頂点ブレンドの処理
        float4x4 comb = (float4x4) 0;
        for (int i = 0; i < 4; i++)
        {
		// 重みを計算しながら行列生成
            comb += BoneMatrix[In.BoneIndex[i]] * In.BoneWeight[i];
        }

        float4 Pos;
    
        Pos = mul(comb, In.Position);
        In.Position = Pos;

	//
        matrix wvp;
        wvp = mul(World, View);
        wvp = mul(wvp, Projection);
	
        float4 worldNormal, normal;
        normal = float4(In.Normal.xyz, 0.0);
        worldNormal = mul(normal, World);
        worldNormal = normalize(worldNormal);

        float light = -dot(Light.Direction.xyz, worldNormal.xyz) * 0.5 + 0.5;
        light = saturate(light);

        Out.Diffuse = In.Diffuse * Material.Diffuse * light * Light.Diffuse;
        Out.Diffuse += In.Diffuse * Material.Ambient * Light.Ambient;
        Out.Diffuse += Material.Emission;
        Out.Diffuse.a = In.Diffuse.a * Material.Diffuse.a;

        Out.Position = mul(In.Position, wvp);
        Out.TexCoord = In.TexCoord;
    }
    
    // アウトライン計算
    {
        float4x4 world, view, proj;
        world = World;
        view = View;
        proj = Projection;
        
        // ローカル座標をスクリーン座標へ変換
        float4 position = In.Position;
        position.w = 1.0f;
        Out.Position = position; // ローカル座標
    
        // 輪郭線として表示するために、法線方向へ頂点を移動
        // 単純に拡張で大きくすると見た目がおかしくなる
        // 正規化された法線は１（メートル）で拡大が大きくなりすぎるため、補正を入れる
        Out.Position.xyz += normalize(In.Normal) * 0.03f;
    
        Out.Position = mul(Out.Position, world); // ワールド座標
        Out.Position = mul(Out.Position, view); // ビュー座標
        Out.Position = mul(Out.Position, proj); // プロジェクション座標
    
        // ピクセルシェーダーで利用するUV座標を渡す
        Out.TexCoord = In.TexCoord;
    
        // ピクセルシェーダーで利用する法線を渡す
        // 法線はオブジェクトの回転に合わせて回転させないとおかしな見た目になる
        // ワールド行列には移動成分も含まれるが移動を法線に適応してもおかしくなるので、
        // 回転成分のみの行列となるように縮小して計算する
        Out.Normal = mul((float3)In.Normal, (float3x3) world);
    }
}