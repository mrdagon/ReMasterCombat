//Copyright © 2017 (´･@･)
//[License]GNU Affero General Public License, version 3
//[Contact]http://tacoika.blog87.fc2.com/
#pragma once

namespace SDX_RMC
{
	using namespace SDX;


	struct AreaData
	{
		AreaType 地域種;
		std::string 地域名;
		std::string 指揮官;
		AreaStateType 状態;
		Point 座標;
		int 友好度;
		int 戦力;
		int 背景;
		JobType 大将;
		JobType 前衛;
		JobType 後衛;
		FormationType 使用陣形;//連戦の場合、個別に設定?
		TacticsType 使用戦術[8];

	};

	EnumArray<AreaData, AreaType> 地域データ;//この数値を初期化時に代入する

	void SetArea(AreaType 地域種, const std::string 地域名, Point 座標, int 友好, int 戦力,int 背景)
	{
		地域データ[地域種].地域種 = 地域種;
		地域データ[地域種].地域名 = 地域名;
		地域データ[地域種].座標 = 座標;
		地域データ[地域種].友好度 = 友好;
		地域データ[地域種].戦力 = 戦力;
		地域データ[地域種].背景 = 背景;
	}

	void SetBattle(AreaType 地域種, const std::string 指揮官, JobType 大将, FormationType 使用陣形)
	{
		地域データ[地域種].指揮官 = 指揮官;
		地域データ[地域種].大将 = 大将;
		地域データ[地域種].使用陣形 = 使用陣形;
	}

	void LoadAreaS()
	{
		SetArea(AreaType::ロア, "ロア", {255,140},0,0,0);
		SetArea(AreaType::永久凍土, "永久凍土", { 20,20 }, 0, 70, 0);//23
		SetArea(AreaType::飛竜の山, "飛竜の山", { 115,35 }, 0, 80, 7);//28
		SetArea(AreaType::飛竜の山2, "飛竜の山", { 115,35 }, 0, 80, 7);//38
		SetArea(AreaType::ユルテム, "ユルテム", { 70,90 }, 75, 30, 0);//21
		SetArea(AreaType::双槍峰, "双槍峰", { 25,180 }, 0, 100, 4);//33
		SetArea(AreaType::プドニ, "プドニ", { 75,220 }, 25, 50, 4);//4
		SetArea(AreaType::ベア, "ベア", { 135,155 }, 25, 100, 6);//35
		SetArea(AreaType::ベア2, "ベア", { 135,155 }, 25, 100, 6);//30
		SetArea(AreaType::ベア3, "ベア", { 135,155 }, 25, 100, 6);//2
		SetArea(AreaType::子鬼の巣窟, "子鬼の巣窟", { 225,240 }, 0, 10, 6);//1
		SetArea(AreaType::ツィガフ, "ツィガフ", { 205,180 }, 0, 20, 6);//3
		SetArea(AreaType::ボロンカ, "ボロンカ", { 225,240 }, 75, 20, 6);//5
		SetArea(AreaType::ローブルグ, "ローブルグ", { 145,300 }, 0, 50, 8);//11
		SetArea(AreaType::カトマ, "カトマ", { 220,320 }, 50, 80, 5);//20
		SetArea(AreaType::カトマ2, "カトマ", { 220,320 }, 50, 80,  5);//17
		SetArea(AreaType::朱雀城跡, "朱雀城跡", { 135,365 }, 0, 100, 8);//25
		SetArea(AreaType::パネイル, "パネイル", { 230,380 }, 100, 10, 1);//31
		SetArea(AreaType::精霊の森, "精霊の森", { 275,40 }, 75, 50, 5);//13
		SetArea(AreaType::精霊の森2, "精霊の森2", { 275,40 }, 75, 50,  5);//22
		SetArea(AreaType::魔神殿, "魔神殿", { 360,30 }, 0, 100, 2);//24
		SetArea(AreaType::スピア, "スピア", { 295,100 }, 50, 40, 6);//9
		SetArea(AreaType::バガダ, "バガダ", { 355,150 }, 25, 60, 5);//14
		SetArea(AreaType::東の森, "東の森", { 285,200 }, 0, 30, 6);//36
		SetArea(AreaType::魔女の森, "東の森", { 285,200 }, 0, 30,  6);//37
		SetArea(AreaType::アントラ, "アントラ", { 440,160 }, 25, 90, 2);//10
		SetArea(AreaType::アントラ2, "アントラ2", { 440,160 }, 25, 90,  2);//18
		SetArea(AreaType::テリザロ, "テリザロ", { 405,205 }, 0, 75, 2);//19
		SetArea(AreaType::テリザロ2, "テリザロ2", { 405,205 }, 0, 75,  2);//12
		SetArea(AreaType::クィンティン, "クィンティン", { 375,250 }, 50, 50, 6);//7
		SetArea(AreaType::バルバード, "バルバード", { 410,365 }, 25, 70, 1);//6
		SetArea(AreaType::スバイツア, "スバイツア", { 470,220 }, 50, 80, 2);//8
		SetArea(AreaType::スバイツア2, "スバイツア2", { 470,220 }, 50, 80,  29);//29
		SetArea(AreaType::アロンダイ, "アロンダイ", { 510,250 }, 25, 30, 5);//15
		SetArea(AreaType::深淵, "深淵", { 540,290 }, 0, 100, 10);//34
		SetArea(AreaType::冥王の塔, "冥王の塔", { 535,360 }, 0, 100, 10);//34
		SetArea(AreaType::冥王の塔2, "冥王の塔2", { 535,360 }, 0, 100,  10);//34
		SetArea(AreaType::サイズヅ, "サイズヅ", { 595,255 }, 0, 60, 3);//26
		SetArea(AreaType::コウガイ, "コウガイ", { 495,125 }, 75, 50, 3);//27
		SetArea(AreaType::海帝島, "海帝島", { 600,125 }, 0, 100, 4);//39

		SetBattle(AreaType::ロア, "主人公", JobType::主人公, FormationType::縦列陣形);
		SetBattle(AreaType::永久凍土,"氷の巨人タイダーン",JobType::主人公, FormationType::縦列陣形);//23
		SetBattle(AreaType::飛竜の山,"鬼将ドビン",JobType::主人公, FormationType::縦列陣形);//28
		SetBattle(AreaType::飛竜の山2,"龍皇ドルーラ",JobType::主人公, FormationType::縦列陣形);//38
		SetBattle(AreaType::ユルテム,"町長ドラント",JobType::主人公, FormationType::縦列陣形);//21
		SetBattle(AreaType::双槍峰, "魔龍将ヴォーリク",JobType::主人公, FormationType::縦列陣形);//33
		SetBattle(AreaType::プドニ, "双璧のスラック",JobType::主人公, FormationType::縦列陣形);//4
		SetBattle(AreaType::ベア, "篭城のコメテオ",JobType::主人公, FormationType::縦列陣形);//35
		SetBattle(AreaType::ベア2, "鉄槌のネイムフ",JobType::主人公, FormationType::縦列陣形);//30
		SetBattle(AreaType::ベア3, "将軍グレィトス",JobType::主人公, FormationType::縦列陣形);//2
		SetBattle(AreaType::子鬼の巣窟, "鬼王ゴブキング", JobType::主人公, FormationType::縦列陣形);//1
		SetBattle(AreaType::ツィガフ,"鉄壁のブロクス", JobType::主人公, FormationType::縦列陣形);//3
		SetBattle(AreaType::ボロンカ, "攻撃のアラン", JobType::主人公, FormationType::縦列陣形);//5
		SetBattle(AreaType::ローブルグ, "魔炎軍アチーチ", JobType::主人公, FormationType::縦列陣形);//11
		SetBattle(AreaType::カトマ, "蹂躙のウェイブ", JobType::主人公, FormationType::縦列陣形);//20
		SetBattle(AreaType::カトマ2, "破壊のモロモン", JobType::主人公, FormationType::縦列陣形);//17
		SetBattle(AreaType::朱雀城跡, "魔炎将フェルネクス",  JobType::主人公, FormationType::縦列陣形);//25
		SetBattle(AreaType::パネイル, "横向きのヨーコ",  JobType::主人公, FormationType::縦列陣形);//31
		SetBattle(AreaType::精霊の森, "妖精長エリス",  JobType::主人公, FormationType::縦列陣形);//13
		SetBattle(AreaType::精霊の森2, "族長エルウィ",  JobType::主人公, FormationType::縦列陣形);//22
		SetBattle(AreaType::魔神殿, "魔戦将イリゴス", JobType::主人公, FormationType::縦列陣形);//24
		SetBattle(AreaType::スピア, "集中のラゴル",   JobType::主人公, FormationType::縦列陣形);//9
		SetBattle(AreaType::バガダ, "波動のツカオ",   JobType::主人公, FormationType::縦列陣形);//14
		SetBattle(AreaType::東の森, "粘液軍団ゼラルー",   JobType::主人公, FormationType::縦列陣形);//36
		SetBattle(AreaType::魔女の森, "もりのうぃっち",  JobType::主人公, FormationType::縦列陣形);//37
		SetBattle(AreaType::アントラ, "防衛のユリーザ",   JobType::主人公, FormationType::縦列陣形);//10
		SetBattle(AreaType::アントラ2, "猛将スマーグ",   JobType::主人公, FormationType::縦列陣形);//18
		SetBattle(AreaType::テリザロ, "殲滅のアラス",   JobType::主人公, FormationType::縦列陣形);//19
		SetBattle(AreaType::テリザロ2, "智将ゴリアレフ",  JobType::主人公, FormationType::縦列陣形);//12
		SetBattle(AreaType::クィンティン, "突破のムニトラ", JobType::主人公, FormationType::縦列陣形);//7
		SetBattle(AreaType::バルバード, "飛翔のガリン", JobType::主人公, FormationType::縦列陣形);//6
		SetBattle(AreaType::スバイツア, "猛攻のストラト", JobType::主人公, FormationType::縦列陣形);//8
		SetBattle(AreaType::スバイツア2, "策将トーフ", JobType::主人公, FormationType::縦列陣形);//29
		SetBattle(AreaType::アロンダイ, "疾風のセイザー", JobType::主人公, FormationType::縦列陣形);//15
		SetBattle(AreaType::深淵, "深淵の魔王", JobType::主人公, FormationType::縦列陣形);//34
		SetBattle(AreaType::冥王の塔, "冥教司祭アンナ", JobType::主人公, FormationType::縦列陣形);//34
		SetBattle(AreaType::冥王の塔2, "冥教法王テリウス",  JobType::主人公, FormationType::縦列陣形);//27
		SetBattle(AreaType::サイズヅ, "深淵の魔海兵",  JobType::主人公, FormationType::縦列陣形);//39
		SetBattle(AreaType::コウガイ, "忍頭サルライヤ",  JobType::主人公, FormationType::縦列陣形);//16
		SetBattle(AreaType::海帝島, "魔海将ヴェパール",  JobType::主人公, FormationType::縦列陣形);//32

		//忍頭サルライヤ 16?
		//深淵の魔海兵 39
	}

}