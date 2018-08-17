//Copyright © 2017 (´･@･)
//[License]GNU Affero General Public License, version 3
//[Contact]http://tacoika.blog87.fc2.com/
#pragma once

//各種定数
namespace SDX_RMC
{
	namespace CV
	{
		constexpr double バージョン = 0.001;
		//システム
		constexpr int 最大戦術数 = 8;
		constexpr double 最大士気 = 100;
		constexpr int 当たり判定サイズ = 32;
		constexpr int 最大セーブ数 = 3;

		constexpr double 雨士気低下 = 0.08;
		constexpr double 雪士気低下 = 0.12;
		constexpr double 雨移動低下 = 0.3;//この割合分低下
		constexpr double 雪移動低下 = 0.6;

		constexpr double 指揮官士気ペナルティ = 0.06;

		constexpr int 戦術回数無限 = 100;

		constexpr double 戦場左端 = 16.0;
		constexpr double 戦場右端 = 624.0;

		constexpr int ダメージ表示時間 = 60;
		//陣形
		//戦術
		constexpr double 交代閾値 = 10.0;
		//兵種
		//地方
		//各種性能や効果,座標等は数が多いのでとりあえず直打ち
		//外部ファイルから配列に読み込むなどした方が調整する時楽なので変えたい
		//Enum
	}

}