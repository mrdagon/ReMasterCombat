//Copyright © 2017 (´･@･)
//[License]GNU Affero General Public License, version 3
//[Contact]http://tacoika.blog87.fc2.com/
#pragma once
#include "../System/EnumType.h"
#include "../Struct/LegionData.h"

namespace SDX_RMC
{
	using namespace SDX;

	class BattleAI
	{
	private:
		LegionData* 軍団[2];
		WeatherType* 天気;
		double 両軍距離;
		/*AIの個性*/
		double 攻撃強制ライン = 180;//A
		double 後退不可ライン = 120;//B
		double 後退終了ライン = 60;//C
		double 回復完了モラル = 90;//A
		double 回復解除モラル =75;//B
		double 攻撃続行モラル = 50;//C
		double 反撃モラル差 = 0;//D
		double 追撃解除モラル = 20;//E

		double 対攻撃目標ダメージ = 0.5;
		double 対防御目標ダメージ = 1;
		int 逃走残存部隊数 = 20;
		double 突撃モラル差 = 50;
	public:
		void Init(LegionData* 軍団A, LegionData* 軍団B, WeatherType* 天気)
		{
			this->軍団[0] = 軍団A;
			this->軍団[1] = 軍団B;
			this->天気 = 天気;
		}

		void 基本計算()
		{
			//LegionDataの平均〇〇関数で大半は計算済み
			両軍距離 = std::abs(軍団[0]->列平均位置[軍団[0]->戦闘列] - 軍団[1]->列平均位置[軍団[1]->戦闘列]);
			
			//is分断
			if (軍団[0]->現在戦術 == TacticsType::分断作戦) { 軍団[1]->is分断判定; }
			if (軍団[1]->現在戦術 == TacticsType::分断作戦) { 軍団[0]->is分断判定; }

			//列の距離でAI用モラル計算
			for (int a = 0; a < 2; a++)
			{
				//距離が近い場合
				if (std::abs(軍団[a]->列平均位置[RowType::前列] - 軍団[a]->列平均位置[RowType::後列]))
				{
					軍団[a]->AIモラル = 軍団[a]->平均モラル;
				}
				else
				{
					軍団[a]->AIモラル = 軍団[a]->列平均モラル[軍団[a]->戦闘列];
				}
			}

			//ダメ予測値
			予測ダメージ計算();
		}

		virtual TacticsType 戦術選択(int AI)
		{
			TacticsType 攻撃用戦術;
			TacticsType 士気用戦術;

			/*突撃他例外的な処理*/
			//突撃判断
			if (軍団[AI]->戦術回数[TacticsType::全軍突撃] > 0 &&
					(
					軍団[AI]->AIモラル + 50 < 軍団[1-AI]->AIモラル ||
					軍団[1 - AI]->現在戦術 == TacticsType::全軍突撃
					)
				)
			{
				return TacticsType::全軍突撃;
			}			
			/*攻撃用戦術と士気用戦術の選択*/
			攻撃用戦術 = TacticsType::全軍前進;
			士気用戦術 = TacticsType::全軍後退;

			/*共通戦術選択処理*/
			return 攻防選択(AI,攻撃用戦術, 士気用戦術);
		}

	private:

		void 予測ダメージ計算()
		{
			//複列で距離が離れている場合、片方のモラルだけ参照
			double 攻撃予測値[2];
			double 防御予測値[2];
			
			//基礎ステ、戦術補正

			//戦闘状態による変化

			//陣形補正

			//天候補正

			//モラル補正、突撃補正
		}

		TacticsType 攻防選択(int AI,TacticsType 攻撃用戦術, TacticsType 回復用戦術)
		{
			int BI = 1 - AI;//相手軍

			//FBSのAI_VScheck()相当
			//★回復 VS 回復
			if (軍団[AI]->戦闘態勢 == TacticsBattleType::回復 && 軍団[BI]->戦闘態勢 == TacticsBattleType::回復)
			{
				//モラル水準以上、or、後退しすぎで解除して攻撃
				if (軍団[AI]->AIモラル > 回復完了モラル || 軍団[AI]->最後尾位置 < 後退終了ライン)
				{
					return 攻撃用戦術;
				}
			}
			//★回復 VS 攻撃
			if (軍団[AI]->戦闘態勢 == TacticsBattleType::回復 && 軍団[BI]->戦闘態勢 == TacticsBattleType::攻撃)
			{
				if (軍団[AI]->AIモラル > 回復解除モラル || 軍団[AI]->最後尾位置 < 後退終了ライン)
				{
					return 攻撃用戦術;
				}

			}
			//★回復 VS 防御
			if (軍団[AI]->戦闘態勢 == TacticsBattleType::回復 && 軍団[BI]->戦闘態勢 == TacticsBattleType::防御)
			{
				if (軍団[AI]->AIモラル > 回復完了モラル || 軍団[AI]->最後尾位置 < 後退終了ライン)
				{
					return 攻撃用戦術;
				}
			}
			//▲攻撃 VS 回復
			if (軍団[AI]->戦闘態勢 == TacticsBattleType::攻撃 && 軍団[BI]->戦闘態勢 == TacticsBattleType::回復)
			{
				if (軍団[AI]->AIモラル < 追撃解除モラル && 軍団[AI]->最後尾位置 > 攻撃強制ライン)
				{
					return 回復用戦術;
				}
			}
			//▲攻撃 VS 攻撃
			if (軍団[AI]->戦闘態勢 == TacticsBattleType::攻撃 && 軍団[BI]->戦闘態勢 == TacticsBattleType::攻撃)
			{
				//与えるダメージが相手より低く
				//モラルがとても低く
				//押し込まれすぎていなければ回復
				if (軍団[AI]->与ダメ予測値 - 軍団[BI]->与ダメ予測値 < 対攻撃目標ダメージ &&
					軍団[AI]->AIモラル < 攻撃続行モラル &&
					軍団[AI]->最後尾位置 > 攻撃強制ライン)
				{
					return 回復用戦術;
				}
			}
			//▲攻撃 VS 防御
			if (軍団[AI]->戦闘態勢 == TacticsBattleType::攻撃 && 軍団[BI]->戦闘態勢 == TacticsBattleType::防御)
			{
				//与えるダメージが低く
				//モラルがとても低く
				//押し込まれすぎていなければ回復
				if (軍団[AI]->与ダメ予測値 < 対防御目標ダメージ &&
					軍団[AI]->AIモラル < 攻撃続行モラル &&
					軍団[AI]->最後尾位置 > 攻撃強制ライン)
				{
					return 回復用戦術;
				}
			}
			//自発的には防御にしないので防御は絶対解除する
			//●防御 VS 回復
			if (軍団[AI]->戦闘態勢 == TacticsBattleType::防御 && 軍団[BI]->戦闘態勢 == TacticsBattleType::回復)
			{
				if (軍団[AI]->AIモラル < 追撃解除モラル &&
					軍団[AI]->最後尾位置 > 攻撃強制ライン)
				{
					return 回復用戦術;
				} else {
					return 攻撃用戦術;
				}
			}
			//●防御 VS 攻撃
			if (軍団[AI]->戦闘態勢 == TacticsBattleType::防御 && 軍団[BI]->戦闘態勢 == TacticsBattleType::攻撃)
			{
				if (軍団[AI]->AIモラル && 軍団[AI]->最後尾位置)
				{
					return 回復用戦術;
				} else {
					return 攻撃用戦術;
				}
			}
			//●防御 VS 防御
			if (軍団[AI]->戦闘態勢 == TacticsBattleType::防御 && 軍団[BI]->戦闘態勢 == TacticsBattleType::防御)
			{
				if (軍団[AI]->AIモラル < 軍団[BI]->AIモラル &&
					軍団[AI]->最後尾位置 > 攻撃強制ライン)
				{
					return 回復用戦術;
				} else {
					return 攻撃用戦術;
				}
			}

			return TacticsType::空き;
		}


	};

}