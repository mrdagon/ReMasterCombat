//Copyright © 2017 (´･@･)
//[License]GNU Affero General Public License, version 3
//[Contact]http://tacoika.blog87.fc2.com/
#pragma once
//#include "../Struct/DataS.h"
#include "EnumType.h"

namespace SDX_RMC
{
	using namespace SDX;
	namespace MCSystem
	{
		
		EnumArray<bool, TacticsType> 所持戦術;
		EnumArray<bool, FormationType> 所持陣形;
		EnumArray<bool, EventType> イベントフラグ;

		//内政関連
		int 傭兵レベル;
		int 忍者レベル;
		int 資金;
		int 幸福;
		int 産業;
		int 日数;

		//
		int 勝利回数;
		int 敗北回数;
		int 逃走回数;
		int 完勝回数;
		int 現プレイ時間;
		GameType ゲームモード;

		int 武器レベル;
		int 防具レベル;
		int 竜鱗所持数;
		bool 魔槍;
		bool 鳳凰の羽;
		bool 生命の石;

		//陣形２つと、陣形毎に8つずつプリセットする
		FormationType 装備陣形[2];
		TacticsType 装備戦術[2][CV::最大戦術数];
		//褒章解除関連
	}

	/*例外的な事はあまり考えない実装*/
	template <typename T>
	class Singleton
	{
	public:
		static T& Get()
		{
			static T instance;
			return instance;
		}
	};

}