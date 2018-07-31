//Copyright © 2017 (´･@･)
//[License]GNU Affero General Public License, version 3
//[Contact]http://tacoika.blog87.fc2.com/
#pragma once
#include "../System/EnumType.h"

namespace SDX_RMC
{
	using namespace SDX;

	//各兵種の性能
	class UnitData
	{
	public:
		JobType 職種;
		UnitImageType 画像;
		int 部隊数 = 100;
		double 生命力;//0になると死亡
		double 攻撃力;//与ダメージを増加
		double 防御力;//被ダメージを軽減
		double 機動力;//移動速度
		double 重量;
		double 火耐性;//0無効, 1.0等倍,2なら倍ダメージ
		double 雨耐性;//0無効、1通常、マイナスなら逆にモラル回復&速度上昇
		double 雪耐性;//0無効、1通常、マイナスなら逆にモラル回復&速度上昇

		double 士気 = CV::最大士気;//最大値は定数参照=100
		double 蓄積ダメージ = 0;//これが生命力を超えると部隊数-1

		RowType 現在列;
		RowType 初期列;

		double 攻撃補正 = 0;//戦術による補正
		double 防御補正 = 0;
		double 士気変化 = 0;

		Point 位置;
		double 移動速度 = 0;//方向＆速度
		DirectionType 向き;
		EmoteType 噴き出しフラグ;
		UnitStateType 戦闘状態 = UnitStateType::戦闘中;
		int 突撃時間 = 0;
		bool is逃走中 = false;
		bool is描画 = true;
		int  自爆時間 = 0;//1で自爆
		bool is指揮官 = false;
		bool is巨大 = false;//ボス的用の
		double 重量補正 = 0;//連接触吹き飛び補正、1Fで0.01増加、最大1.00
		double 陣形位置差分 = 0;//平均X座標との差分
		double 吹き飛び量 = 0;//X方向の吹き飛び残量

		int 落とし穴時間 = 0;

		//演出用
		Point 開始地点;
		Point 着地地点;
		double ジャンプ高さ = 0;
		int ジャンプ回数 = 0;
		int ジャンプ待ち = 0;
		double ジャンプ加速度 = 0;
		int フラッシュ時間 = 0;
		Color フラッシュ色 = Color::White;
		int 退場時間 = 0;//死亡、突破演出用
		EmoteType 感情;
		int 感情時間 = 0;
		std::string 台詞;
		int 台詞時間 = 0;

		void Set(JobType 職種,UnitImageType 画像)
		{
			this->職種 = 職種;
			this->画像 = 画像;
		}

		//これはテキストファイルから読み込みとかでも
		void Set性能(int 生命力, double 攻撃力, double 防御力,  double 機動力, double 重量, double 炎耐性, double 雨耐性 ,double 雪耐性)
		{
			this->生命力 = 生命力;
			this->攻撃力 = 攻撃力;
			this->防御力 = 防御力;
			this->重量 = 重量;
			this->機動力 = 機動力;
			this->火耐性 = 炎耐性;
			this->雨耐性 = 雨耐性;
			this->雪耐性 = 雪耐性;
		}


	};

	struct FireData
	{
		Point 位置;
		int 残り時間;
		double 移動速度;
	};

	struct HoleData
	{
		Point 位置;
	};

	struct FenceData
	{
		Point 位置;
		double 残り耐久;
	};

	EnumArray<UnitData, JobType> 職種基礎ステ;//この数値を初期化時に代入する

	void LoadUnitS()
	{
		職種基礎ステ[JobType::歩兵].Set(JobType::歩兵, UnitImageType::歩兵);
		職種基礎ステ[JobType::ゼラルー].Set(JobType::ゼラルー, UnitImageType::ゼラルー);
		職種基礎ステ[JobType::ゴブリン].Set(JobType::ゴブリン, UnitImageType::ゴブリン);
		職種基礎ステ[JobType::オーガ].Set(JobType::オーガ, UnitImageType::オーガ);
		職種基礎ステ[JobType::歩兵].Set(JobType::歩兵, UnitImageType::歩兵);
		職種基礎ステ[JobType::竜王].Set(JobType::竜王, UnitImageType::竜王);
		職種基礎ステ[JobType::重装兵].Set(JobType::重装兵, UnitImageType::重装兵);
		職種基礎ステ[JobType::機動兵].Set(JobType::機動兵, UnitImageType::機動兵);
		職種基礎ステ[JobType::僧兵].Set(JobType::僧兵, UnitImageType::僧兵);
		職種基礎ステ[JobType::魔法兵].Set(JobType::魔法兵, UnitImageType::魔法兵);
		職種基礎ステ[JobType::妖精].Set(JobType::妖精, UnitImageType::妖精);
		職種基礎ステ[JobType::忍者].Set(JobType::忍者, UnitImageType::忍者);
		職種基礎ステ[JobType::くノ一].Set(JobType::くノ一, UnitImageType::くノ一);
		職種基礎ステ[JobType::女エルフ].Set(JobType::女エルフ, UnitImageType::女エルフ);
		職種基礎ステ[JobType::男エルフ].Set(JobType::男エルフ, UnitImageType::男エルフ);
		職種基礎ステ[JobType::魔族].Set(JobType::魔族, UnitImageType::魔族);
		職種基礎ステ[JobType::格闘家].Set(JobType::格闘家, UnitImageType::格闘家);
		職種基礎ステ[JobType::傭兵].Set(JobType::傭兵, UnitImageType::傭兵);
		職種基礎ステ[JobType::魔貴族].Set(JobType::魔貴族, UnitImageType::魔貴族);
		職種基礎ステ[JobType::斧兵].Set(JobType::斧兵, UnitImageType::斧兵);
		職種基礎ステ[JobType::勇者].Set(JobType::勇者, UnitImageType::勇者);
		職種基礎ステ[JobType::王子].Set(JobType::王子, UnitImageType::王子);
		職種基礎ステ[JobType::王].Set(JobType::王, UnitImageType::王);
		職種基礎ステ[JobType::王女].Set(JobType::王女, UnitImageType::王女);
		職種基礎ステ[JobType::老兵].Set(JobType::老兵, UnitImageType::老兵);
		職種基礎ステ[JobType::主人公].Set(JobType::主人公, UnitImageType::主人公);
		職種基礎ステ[JobType::エルフ王].Set(JobType::エルフ王, UnitImageType::エルフ王);
		職種基礎ステ[JobType::騎士].Set(JobType::騎士, UnitImageType::騎士);
		職種基礎ステ[JobType::司祭].Set(JobType::司祭, UnitImageType::司祭);
		職種基礎ステ[JobType::骸骨].Set(JobType::骸骨, UnitImageType::骸骨);
		職種基礎ステ[JobType::海人].Set(JobType::海人, UnitImageType::海人);
		職種基礎ステ[JobType::ゴーレム].Set(JobType::ゴーレム, UnitImageType::ゴーレム);
		職種基礎ステ[JobType::獣人].Set(JobType::獣人, UnitImageType::獣人);
		職種基礎ステ[JobType::ゴブリン王].Set(JobType::ゴブリン王, UnitImageType::ゴブリン王);
		職種基礎ステ[JobType::竜].Set(JobType::竜, UnitImageType::竜);
		職種基礎ステ[JobType::悪魔].Set(JobType::悪魔, UnitImageType::悪魔);
		職種基礎ステ[JobType::樹人].Set(JobType::樹人, UnitImageType::樹人);
		職種基礎ステ[JobType::怪鳥].Set(JobType::怪鳥, UnitImageType::怪鳥);

		職種基礎ステ[JobType::ゼラルー].Set性能(100,  8, 10, 0.6, 1.5, 1, 1, 1);//0
		職種基礎ステ[JobType::ゴブリン].Set性能(110,  9,  9, 1.0, 0.9, 1, 1, 1);
		職種基礎ステ[JobType::オーガ].Set性能(	120, 12,  9, 0.9, 1.0, 1, 1, 1);
		職種基礎ステ[JobType::歩兵].Set性能(	100, 10, 10, 1.0, 1.0, 1, 1, 1);
		職種基礎ステ[JobType::竜王].Set性能(   3000, 12, 10, 1.0, 1.5, 1, 1, 1);
		職種基礎ステ[JobType::重装兵].Set性能(	100, 10, 11, 0.9, 1.1, 1, 1, 1);//5
		職種基礎ステ[JobType::機動兵].Set性能(	100, 12, 10, 1.1, 0.9, 1, 1, 1);
		職種基礎ステ[JobType::僧兵].Set性能(	 90,  8,  8, 0.9, 0.9, 1, 1, 1);
		職種基礎ステ[JobType::魔法兵].Set性能(	500, 14, 10, 0.8, 1.5, 1, 1, 1);
		職種基礎ステ[JobType::妖精].Set性能(	100, 10, 12, 0.9, 1.0, 1, 1, 1);
		職種基礎ステ[JobType::忍者].Set性能(	110, 12, 10, 1.1, 1.2, 1, 1, 1);//10
		職種基礎ステ[JobType::くノ一].Set性能(	110, 11, 12, 1.5, 0.9, 1, 1, 1);
		職種基礎ステ[JobType::女エルフ].Set性能(100, 11, 12, 1.1, 1.0, 1, 1, 1);
		職種基礎ステ[JobType::男エルフ].Set性能(100, 12, 11, 1.1, 1.0, 1, 1, 1);
		職種基礎ステ[JobType::魔族].Set性能(	110, 12, 11, 1.0, 1.0, 1, 1, 1);
		職種基礎ステ[JobType::格闘家].Set性能(	150, 10,  8, 1.1, 1.0, 1, 1, 1);//15
		職種基礎ステ[JobType::傭兵].Set性能(	110, 12, 11, 1.0, 1.2, 1, 1, 1);
		職種基礎ステ[JobType::魔貴族].Set性能(	130, 14, 13, 1.1, 1.5, 1, 1, 1);
		職種基礎ステ[JobType::斧兵].Set性能(	110, 11, 10, 1.0, 1.0, 1, 1, 1);
		職種基礎ステ[JobType::勇者].Set性能(	150, 15, 15, 1.2, 1.3, 1, 1, 1);
		職種基礎ステ[JobType::王子].Set性能(	130, 14, 14, 1.1, 1.3, 1, 1, 1);
		職種基礎ステ[JobType::王].Set性能(		140, 13, 13, 1.1, 1.3, 1, 1, 1);
		職種基礎ステ[JobType::王女].Set性能(	130, 13, 14, 1.5, 1.3, 1, 1, 1);
		職種基礎ステ[JobType::老兵].Set性能(	 80,  8,  8, 0.8, 0.8, 1, 1, 1);
		職種基礎ステ[JobType::主人公].Set性能(	110, 11, 11, 1.0, 1.0, 1, 1, 1);
		職種基礎ステ[JobType::エルフ王].Set性能(120, 13, 13, 1.2, 1.1, 1, 1, 1);
		職種基礎ステ[JobType::騎士].Set性能(	110, 11, 11, 1.0, 1.1, 1, 1, 1);
		職種基礎ステ[JobType::司祭].Set性能(	120, 12, 12, 0.9, 1.2, 1, 1, 1);
		職種基礎ステ[JobType::骸骨].Set性能(	 70, 10,  5, 0.7, 0.8, 1, 1, 1);
		職種基礎ステ[JobType::海人].Set性能(	110, 10, 11, 0.8, 1.2, 1, 1, 1);
		職種基礎ステ[JobType::ゴーレム].Set性能( 80, 11, 15, 0.7, 1.5, 1, 1, 1);
		職種基礎ステ[JobType::獣人].Set性能(	100, 12, 10, 1.1, 1.0, 1, 1, 1);
		職種基礎ステ[JobType::ゴブリン王].Set性能(100,10,10, 0.9, 1.2, 1, 1, 1);
		職種基礎ステ[JobType::竜].Set性能(		130, 13, 13, 1.0, 1.3, 1, 1, 1);
		職種基礎ステ[JobType::悪魔].Set性能(	120, 12, 12, 1.2, 1.2, 1, 1, 1);
		職種基礎ステ[JobType::樹人].Set性能(	150, 11, 10, 0.8, 1.3, 1, 1, 1);
		職種基礎ステ[JobType::怪鳥].Set性能(	140, 14, 10, 1.4, 1.4, 1, 1, 1);
	}

}