//Copyright © 2017 (´･@･)
//[License]GNU Affero General Public License, version 3
//[Contact]http://tacoika.blog87.fc2.com/
#pragma once
#include "../System/EnumType.h"

namespace SDX_RMC
{
	using namespace SDX;

	class LegionData
	{
	public:
		std::vector<UnitData> 兵士;
		UnitData* 指揮官;
		FormationType 陣形;
		TacticsType 戦術[8];
		EnumArray<int,TacticsType> 残り戦術回数;//AIの都合上装備してない戦術も残り使用数をカウント
		TacticsType 現在戦術;//補助戦術使用時は変化しない場合あり
		DirectionType 前進方向;
		DirectionType 後退方向;
		int 前進正負;//前進する時の向き 1が左向き、-1が右向き
		LegionData* 敵軍;

		bool is交代前進;//交代中-後列が前列位置へ
		bool is交代後退;//交代中-前列が後列位置へ
		bool is指揮官生存;
		int 活動兵士数;//死亡、退却、突破していない兵士の数
		double 交代前の後列位置;//交代開始時の後列ライン

		double 陣形回復座標;
		int 陣形回復残り時間;
		int 死んだふり発動待ち時間;
		double 投石座標;//X座標だけ

		TacticsType 発動待ち戦術;
		int 発動待ち時間;
		int 発動待ち最大;

		//AI用パラメータ
		bool isAI = false;
		double 平均モラル;
		EnumArray<double,RowType> 列平均モラル;
		int 火災時間;
		EnumArray<double, RowType> 列人数;
		EnumArray<double, RowType> 列人数割合;//0.0～1.0
		double 上翼割合;
		double 下翼割合;

		double 平均位置;//X座標
		EnumArray<double, RowType> 列平均位置;
		double 最前線位置;
		double 最後尾位置;
		bool is交戦;
		bool is分断判定;//死んだフリ,分断作戦による

		double 与ダメ予測値;//被ダメは敵軍のを参照する

		int 前回戦術経過時間;//前に戦術を使ってから経過した時間

		/*指揮官をポインタに代入*/
		void Set指揮官()
		{
			for (auto& it : 兵士)
			{
				if (it.is指揮官 == true)
				{
					指揮官 = &it;
					return;
				}
			}
		}

		void 平均値計算()
		{
			平均モラル計算();
			平均位置計算();
		}

		void 突破判定(double X座標)
		{

		}

		void 退却判定(double X座標)
		{

		}

	private:
		void 平均モラル計算()
		{
			活動兵士数 = 0;
			平均モラル = 0;
			列平均モラル[RowType::前列] = 0;
			列平均モラル[RowType::後列] = 0;
			列人数[RowType::前列] = 0;
			列人数[RowType::後列] = 0;
			列人数割合[RowType::前列] = 0;
			列人数割合[RowType::後列] = 0;
			上翼割合 = 0;
			下翼割合 = 0;

			for (auto& it : 兵士)
			{
				if (it.戦闘状態 != UnitStateType::戦闘中) { continue; }

				平均モラル += it.士気;
				列平均モラル[it.現在列] += it.士気;
				列人数[it.現在列]++;
				活動兵士数++;
				if (it.位置.y <= 150) { 上翼割合++; }
				else { 下翼割合++; }

			}

			if (活動兵士数 == 0) { return; }
				
			平均モラル /= 活動兵士数;
			if (列人数[RowType::前列] != 0)
			{
				列平均モラル[RowType::前列] /= 列人数割合[RowType::前列];
				列人数割合[RowType::前列] = 活動兵士数 / 列人数[RowType::前列];
			}

			if (列人数[RowType::後列] != 0)
			{
				列平均モラル[RowType::後列] /= 列人数割合[RowType::後列];
				列人数割合[RowType::後列] = 活動兵士数 / 列人数[RowType::後列];
			}

			if (上翼割合 != 0) { 上翼割合 /= 活動兵士数; }
			if (下翼割合 != 0) { 下翼割合 /= 活動兵士数; }

		}

		void 平均位置計算()
		{
			//軍団=0 右側、軍団=1左側
			最前線位置 = 0;
			最後尾位置 = 640;
			平均位置 = 0;
			列平均位置[RowType::前列] = 0;
			列平均位置[RowType::後列] = 0;

			for (auto& it : 兵士)
			{
				if (it.戦闘状態 != UnitStateType::戦闘中) { continue; }
				if (最前線位置 < it.位置.x) { 最前線位置 = it.位置.x; }
				if (最後尾位置 > it.位置.x) { 最後尾位置 = it.位置.x; }
				列平均位置[it.現在列] += it.位置.x;
				平均位置 += it.位置.x;
			}
			if (列人数[RowType::前列] == 0 && 列人数[RowType::後列] == 0) { return; };

			if (列人数[RowType::前列] > 0) { 列平均位置[RowType::前列] /= 列人数[RowType::前列]; }
			if (列人数[RowType::後列] > 0) { 列平均位置[RowType::後列] /= 列人数[RowType::後列]; }
			平均位置 /= 列人数[RowType::前列] + 列人数[RowType::後列];

			//左側(初期進行方向＝右)の軍は最前と最後尾を交換
			if (前進方向 == DirectionType::右)
			{
				std::swap(最前線位置, 最後尾位置);
			}
		}
	};
}