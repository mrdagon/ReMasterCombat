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
	System::Initialise("sample", 640, 360);//ライブラリの初期化

	//画像と音声の読み込み
	LoadMaterial();
	//各種数値の代入、読み込み
	LoadUnitS();
	LoadFormationS();
	LoadTacticsS();
	
	SceneBattle テスト;
	テスト.Init();
	テスト.Run();

	System::End();//ライブラリの終了処理
	return 0;
}