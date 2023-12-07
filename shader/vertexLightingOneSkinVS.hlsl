#include "common.hlsl"

void main(in VSONESKIN_IN In, out PS_IN Out)
{
        // ���[�V�����v�Z
    {
     	// �����X�L�����_�u�����h�̏���
        float4x4 comb = (float4x4) 0;
        for (int i = 0; i < 4; i++)
        {
		// �d�݂��v�Z���Ȃ���s�񐶐�
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
    
    // �A�E�g���C���v�Z
    {
        float4x4 world, view, proj;
        world = World;
        view = View;
        proj = Projection;
        
        // ���[�J�����W���X�N���[�����W�֕ϊ�
        float4 position = In.Position;
        position.w = 1.0f;
        Out.Position = position; // ���[�J�����W
    
        // �֊s���Ƃ��ĕ\�����邽�߂ɁA�@�������֒��_���ړ�
        // �P���Ɋg���ő傫������ƌ����ڂ����������Ȃ�
        // ���K�����ꂽ�@���͂P�i���[�g���j�Ŋg�傪�傫���Ȃ肷���邽�߁A�␳������
        Out.Position.xyz += normalize(In.Normal) * 0.03f;
    
        Out.Position = mul(Out.Position, world); // ���[���h���W
        Out.Position = mul(Out.Position, view); // �r���[���W
        Out.Position = mul(Out.Position, proj); // �v���W�F�N�V�������W
    
        // �s�N�Z���V�F�[�_�[�ŗ��p����UV���W��n��
        Out.TexCoord = In.TexCoord;
    
        // �s�N�Z���V�F�[�_�[�ŗ��p����@����n��
        // �@���̓I�u�W�F�N�g�̉�]�ɍ��킹�ĉ�]�����Ȃ��Ƃ������Ȍ����ڂɂȂ�
        // ���[���h�s��ɂ͈ړ��������܂܂�邪�ړ���@���ɓK�����Ă����������Ȃ�̂ŁA
        // ��]�����݂̂̍s��ƂȂ�悤�ɏk�����Čv�Z����
        Out.Normal = mul((float3)In.Normal, (float3x3) world);
    }
}