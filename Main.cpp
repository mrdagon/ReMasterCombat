//#include <SDXFramework.h>
//#include <SDXafx.h>
#include "stdafx.h"

using namespace SDX;
#include "System/ConstValue.h"
#include "System/MCSystem.h"
#include "System/Material.h"
#include "System/SaveAndLoad.h"
#include "System/MCSystem.h"
#include "Struct/DataS.h"
#include "System/InitData.h"
#include "Scene/SceneS.h"
using namespace SDX_RMC;

int main(int argc, char* argv[])
{
	System::Initialise("sample", 640, 360);//���C�u�����̏�����

	//�摜�Ɖ����̓ǂݍ���
	LoadMaterial();
	//�e�퐔�l�̑���A�ǂݍ���
	LoadUnitS();
	LoadFormationS();
	LoadTacticsS();
	
	SceneBattle �e�X�g;
	�e�X�g.Init();
	�e�X�g.Run();

	System::End();//���C�u�����̏I������
	return 0;
}