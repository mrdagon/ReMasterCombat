//Copyright © 2017 (´･@･)
//[License]GNU Affero General Public License, version 3
//[Contact]http://tacoika.blog87.fc2.com/
#pragma once
#include "../System/EnumType.h"

namespace SDX_RMC
{
	using namespace SDX;

	//各兵種の性能
	class ISceneBattele
	{
	public:
		LegionData 軍団[2];//0側が右、1側が左

		WeatherType 天気 = WeatherType::晴れ;
		int 天気継続時間 = 0;//0になると晴れに戻る

		TacticsType 演出戦術種;
		int 演出軍団;
		int 戦闘時間 = 0;
		int 編成時間 = 0;

		//オブジェクト
		std::vector<FireData> 火炎;
		std::vector<HoleData> 落とし穴;
		std::vector<FenceData> 防御柵;
		std::vector<DamageEffectData> ダメージエフェクト;

		//ダメージエフェクト
		virtual void Draw(int 描画時間) = 0;

		/*指揮官にしゃべらせる*/
		void Set台詞(std::string& 台詞, LegionData* 軍団, int 台詞時間 = 50)
		{
			//すでに指揮官がいない場合台詞スキップ
			if (軍団->指揮官->戦闘状態 != UnitStateType::戦闘中) { return; }

			軍団->指揮官->台詞 = 台詞;
			軍団->指揮官->台詞時間 = 50;
			Draw(台詞時間);
		}
	};


}