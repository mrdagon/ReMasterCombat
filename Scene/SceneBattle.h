//Copyright © 2017 (´･@･)
//[License]GNU Affero General Public License, version 3
//[Contact]http://tacoika.blog87.fc2.com/
#pragma once
namespace SDX_RMC
{
	using namespace SDX;
	
	class SceneBattle : public ISceneBattele
	{
	private:
		enum class BattlePhase
		{
			Format,//画面端から歩いてきて陣形選択まで
			Combat,//戦闘中
			Result,//戦闘後の処理
		}処理状態;

		//描画管理用
		int 明るさ;
		int 時間 = 0;
		int アニメ時間 = 0;
		int ゲームスピード = 1;

		//デバッグ用
		int 選択陣形 = 0;
		int 陣形テスト = 0;
		int 戦術テスト = 0;		

		//操作状態保存
		bool is戦術選択中 = false;

		const Rect B陣形選択上 = Rect( 320 - 140,100,280,40 );
		const Rect B陣形選択下 = Rect( 320 - 140,160,280,40 );
		const Rect B戦闘開始 = Rect( 320 - 100,230,200,40 );
		const Rect B戦術選択 = Rect(320 - 140, 20, 280, 320);
		const Rect B戦術 = Rect(320 - 140, 20, 280, 40);
	public:
		/*外部呼出し用*/
		void Draw(int 描画時間) override
		{
			for (int t = 0; t < 描画時間; t++)
			{
				Draw();
				if (System::Update() == false) { break; }
			}
		}
		
		void Init()
		{
			//まず両軍の状態を初期化して陣形を選択する
			処理状態 = BattlePhase::Format;
			戦闘時間 = 0;
			明るさ = 0;
			時間 = 0;
			軍団[0].敵軍 = &軍団[1];
			軍団[1].敵軍 = &軍団[0];

			軍団[0].前進方向 = DirectionType::左;
			軍団[1].前進方向 = DirectionType::右;
			軍団[0].後退方向 = DirectionType::右;
			軍団[1].後退方向 = DirectionType::左;
			軍団[0].前進正負 = -1;
			軍団[1].前進正負 = 1;

			//とりあえず30+1人ずつの軍団でテスト
			for (int b = 0; b < 2; b++)
			{
				for (int a = 0; a < 15; a++)
				{
					軍団[b].兵士.push_back(職種基礎ステ[JobType::機動兵]);
				}
				for (int a = 0; a < 15; a++)
				{
					軍団[b].兵士.push_back(職種基礎ステ[JobType::重装兵]);
				}
				for (int a = 0; a < 30; a++)
				{
					軍団[b].兵士[a].位置.SetPos(650-b*660, a * 300 / 30);//60～260
				}
			}
			軍団[0].兵士.push_back(職種基礎ステ[JobType::騎士]);
			軍団[1].兵士.push_back(職種基礎ステ[JobType::勇者]);

			for (int a = 0; a < 2; a++)
			{
				軍団[a].兵士.back().is指揮官 = true;
				軍団[a].兵士.back().位置.SetPos(640*(1-a), 150);
				軍団[a].指揮官 = &軍団[1].兵士.back();
			}

			//全軍前進相当
			
			for (int a = 0; a < 2; a++)
			{
				for(auto& it:軍団[a].兵士)
				{
					it.向き = 軍団[a].前進方向;
					it.攻撃補正 = 2;
					it.防御補正 = 0;
					it.士気変化 = -0.05;
					it.移動速度 = it.機動力  * 0.5;
					it.is逃走中 = false;
				}
			}

		}

		bool Run()
		{

			while (System::Update())
			{
				if (Update() == false) { break; }

				Draw();
			}

			return true;
		}
	
	private:
		//共通処理
		void Draw()
		{
			アニメ時間 += ゲームスピード;
			//明るさ設定と背景
			Screen::SetBright({ 明るさ,明るさ,明るさ });
			MSystem::背景[1]->DrawExtend({ 0,0,640,300 });
			//兵士
			DrawUnit();
			//柵

			//炎

			//天候描画

			//ダメージ

			//士気表示-前後 上下、長さ308 616 24
			//MSystem::モラルバー.Draw({   6, 300 });
			//MSystem::モラルバー.Draw({ 326, 300 });

			//戦術ボタン
			//Rect 戦術ボタン = { 2,282,76,76 };
			//Point 戦術名 = {40,300};
			//for (int a = 0; a < 8; a++)
			//{
			//	Drawing::Rect(戦術ボタン, Color::White);
			//	MFont::ゴシック小.DrawRotate(戦術名, 1,0,Color::Black, 戦術データ[TacticsType(a+戦術テスト)]->戦術名);
			//
			//	戦術ボタン.x += 80;
			//	戦術名.x += 80;
			//}


			//個別処理
			switch (処理状態)
			{
				case BattlePhase::Format:DrawFormat();break;
				case BattlePhase::Combat:DrawCombat(); break;
				case BattlePhase::Result:break;
			}

		}

		void DrawUnit()
		{
			int モーション値 = アニメ時間 / 10 % 3 + 6;

			for (int a = 0; a < 2; a++)
			{
				for (auto& it : 軍団[a].兵士)
				{
					if (it.is描画 == false) { continue; }

					const double px = it.位置.x;
					const double py = (it.位置.y + it.ジャンプ高さ) * 2 / 3 + 80;
					bool is反転 = (it.向き == DirectionType::右);//右に向くように反転

					switch (it.戦闘状態)
					{
					case UnitStateType::死亡:
						if (軍団[a].前進方向 == DirectionType::左)
						{

						} else {

						}
						MUnit::ユニット画像[a][it.画像][モーション値]->DrawRotate({ px , py }, 2, 0, is反転);
						it.退場時間++;
						continue;
					case UnitStateType::突破:
						it.退場時間++;
						break;
					}

					//ジャンプ処理
					if (it.ジャンプ待ち > 0)
					{
						it.ジャンプ待ち--;
					}
					else if (it.ジャンプ高さ == 0 && it.ジャンプ回数 > 0)
					{
						it.ジャンプ回数--;
						it.ジャンプ加速度 = -5.0;
						it.ジャンプ高さ = -3;
					}
					else if (it.ジャンプ高さ < 0)
					{
						it.ジャンプ高さ += it.ジャンプ加速度;
						it.ジャンプ加速度 += 0.5;
						if (it.ジャンプ高さ > 0) { it.ジャンプ高さ = 0; }
					}

					//落とし穴状態
					if (it.落とし穴時間 > 0)
					{
						MSystem::穴.DrawRotate({ it.位置.x,py + 10 }, 2, 0);
					}
					//基本描画
					MUnit::ユニット画像[a][it.画像][モーション値]->DrawRotate({ px , py }, 2, 0, is反転);
					//フラッシュ描画
					if (it.フラッシュ時間 > 0)
					{
						int 透明度;
						if (it.フラッシュ時間 > 25) { 透明度 = 500 - it.フラッシュ時間 * 10; }
						else { 透明度 = it.フラッシュ時間 * 10; }
						it.フラッシュ色.SetColor(it.フラッシュ色.GetRed(), it.フラッシュ色.GetGreen(), it.フラッシュ色.GetBlue(), 透明度);

						MUnit::ユニット画像[2][it.画像][モーション値]->SetColor(it.フラッシュ色);
						MUnit::ユニット画像[2][it.画像][モーション値]->DrawRotate({ px, py }, 2, 0, is反転);
						MUnit::ユニット画像[2][it.画像][モーション値]->SetColor(Color::White);
						it.フラッシュ時間--;
					}
					//セリフ
					if (it.台詞時間 > 0)
					{
						const int len = MFont::ゴシック大.GetDrawStringWidth(it.台詞);

						Drawing::Rect({ px - len / 2 - 3 , py - 40, len + 6 , 20 }, { 175,135,92 });//淵175,135,92
						Drawing::Rect({ px - len / 2 - 2 , py - 40 + 1 , len + 4 , 18 }, { 225,205,172 });//225,205,172、明るい部分
						MSystem::吹き出し.Draw({ px - 15, py - 40 + 3 });
						MFont::ゴシック大.Draw({ px - len / 2 ,py - 40 + 1 }, Color::Black, it.台詞);
						it.台詞時間--;
					}
					//エモート
					if (it.感情時間 > 0)
					{
						MSystem::感情[(int)it.感情]->DrawRotate({ px,py - 20 }, 2, 0);
						it.感情時間--;
					}
					//発動待ち・・・
					if (軍団[a].発動待ち時間 > 0 && it.is指揮官 )
					{
						int 点数 = (int)EmoteType::思考1 + 軍団[a].発動待ち時間 * 3 / 軍団[a].発動待ち最大;
						MSystem::感情[点数]->DrawRotate({ px,py - 20 }, 2, 0);
					}
				}
			}
		}

		bool Update()
		{
			//デバッグ用
			if (Input::mouse.Right.on)
			{
				陣形テスト++;
				陣形テスト = 陣形テスト % (int(FormationType::COUNT) - 1);
			}
			if (Input::key.Left.on) { 戦術テスト -= 8; }
			if (Input::key.Right.on) { 戦術テスト += 8; }
			戦術テスト = std::max(0, 戦術テスト);
			戦術テスト = std::min((int)TacticsType::COUNT - 8, 戦術テスト);
			//デバッグここまで


			//共通処理
			if (明るさ < 253)
			{
				明るさ += 2;
			} else {
				明るさ = 255;
			}
			時間++;

			//個別処理
			switch (処理状態)
			{
			case BattlePhase::Format:UpdateFormat(); break;
			case BattlePhase::Combat:UpdateCombat(); break;
			case BattlePhase::Result:break;
			}

			return true;
		}

		//Formation
		void DrawFormat()
		{
			//最初画面外から両軍行進、相手が陣形選択、その後自軍陣形選択ウィンドウ表示
			//陣形に整列、陣形を変更するとアニメーションしながら位置変更
			if (時間 > 180)
			{
				Drawing::Rect({ 320 - 150,90,300,120 }, { 0,0,255,64 });

				if (選択陣形 == 0) {
					Drawing::Rect(B陣形選択上, { 0,0,255,128 });
				}
				else {
					Drawing::Rect(B陣形選択下, { 0,0,255,128 });
				}
				Drawing::Rect( B戦闘開始, { 0,0,255,128 });

				MFont::ゴシック大.DrawRotate({ 320 , 120 }, 2, 0, Color::White, {陣形データ[FormationType(陣形テスト)]->陣形名 });
				MFont::ゴシック大.DrawRotate({ 320 , 180 }, 2, 0, Color::White, {陣形データ[FormationType(陣形テスト+1)]->陣形名 });
				MFont::ゴシック大.DrawRotate({ 320 , 250 }, 2, 0, Color::White, { "戦闘開始！" });
			}
		}

		void UpdateFormat()
		{
			//最初画面外から両軍行進
			if( 時間 < 180)
			{
				for (int a = 0; a < 2; a++)
				{
					double 移動速度 = (a * 2 - 1)*0.5;

					for(int b = 0; b < (int)軍団[a].兵士.size();b++)
					{ 
						軍団[a].兵士[b].位置.x += 移動速度;
					}
				}
			}
			else
			{
				//その後陣形選択画面
				//陣形選択ウィンドウを表示
				//陣形に整列、陣形を変更するとアニメーションしながら位置変更
				//クリックで仮確定、決定で確定＆決定
				if (Input::mouse.Left.on)
				{
					if (B陣形選択上.Hit(&Input::mouse.GetPoint()))
					{
						選択陣形 = 陣形テスト;
						陣形データ[FormationType(陣形テスト)]->整列(軍団[0], true);
						陣形データ[FormationType(陣形テスト)]->整列(軍団[1], false);
						編成時間 = 30;
					}
					else if (B陣形選択下.Hit(&Input::mouse.GetPoint()))
					{
						選択陣形 = 陣形テスト+1;
						陣形データ[FormationType(陣形テスト+1)]->整列(軍団[0], true);
						陣形データ[FormationType(陣形テスト+1)]->整列(軍団[1], false);
						編成時間 = 30;
					}
					else if (B戦闘開始.Hit(&Input::mouse.GetPoint()))
					{
						//描画順が正しくなるように並び変え
						auto 並び式 = [](const UnitData& a, const UnitData& b)->bool 
						{
							if (a.is指揮官 || b.is指揮官) { return false; }
							return (a.位置.GetY() < b.位置.GetY()); 
						};

						std::sort(軍団[0].兵士.begin(), 軍団[0].兵士.end(), 並び式 );
						std::sort(軍団[1].兵士.begin(), 軍団[1].兵士.end(), 並び式 );
						軍団[0].Set指揮官();
						軍団[1].Set指揮官();

						処理状態 = BattlePhase::Combat;
					}
				}

				if (編成時間 > 0)
				{
					編成時間--;
					for (int a = 0; a < 2; a++)
					{
						for (int b = 0; b < (int)軍団[a].兵士.size(); b++)
						{
							軍団[a].兵士[b].位置.x = 軍団[a].兵士[b].開始地点.x * 編成時間 / 30 + 軍団[a].兵士[b].着地地点.x * (30 - 編成時間) / 30;
							軍団[a].兵士[b].位置.y = 軍団[a].兵士[b].開始地点.y * 編成時間 / 30 + 軍団[a].兵士[b].着地地点.y * (30 - 編成時間) / 30;
						}
					}
				}
			}

		}
		//■■Combat■■
		void DrawCombat()
		{
			if (is戦術選択中 == true)
			{
				Drawing::Rect(B戦術選択, { 0,0,255,128 });
				Rect 選択枠 = B戦術;

				for (int a = 0; a < 8; ++a)
				{
					if (a + 戦術テスト >= (int)TacticsType::COUNT) { break; }

					if (選択枠.Hit(&Input::mouse.GetPoint()))
					{
						選択枠.Draw({ 0,0,255,128 });
					}
					MFont::ゴシック大.DrawRotate({ 320 , 40 + 40*a }, 2, 0, Color::White, 戦術データ[TacticsType(a+ 戦術テスト)]->戦術名);
					選択枠.Move(0, 40);
				}
			}
		}

		void UpdateCombat()
		{
			//AI戦術選択
			TacticsAI();
			//操作処理
			PlayerInput();

			//戦術選択中は戦闘処理等をスキップ
			if (is戦術選択中 == true)
			{
				return;
			}

			//
			戦闘時間++;

			//ユニットの更新
			UpdateUnit();
			//オブジェクトの更新
			UpdateObject();
			//当たり判定処理、兵火柵穴
			HitUnit();
			//戦術効果
			TacticsEffect();
		}
		
		void UpdateUnit()
		{
			for(int a=0;a<2;a++)
			{
				for (auto& it : 軍団[a].兵士)
				{
					if (it.戦闘状態 != UnitStateType::戦闘中) { continue; }

					double 移動補正 = 軍団[a].前進正負;
					//天候効果
					switch (天気)
					{
					case WeatherType::雨:
						it.士気 -= CV::雨士気低下 * it.雨耐性;
						if (it.雨耐性 > 0)
						{
							移動補正 *= CV::雨移動低下;
						}
						break;
					case WeatherType::雪:
						it.士気 -= CV::雪士気低下 * it.雪耐性;
						if (it.雪耐性 > 0)
						{
							移動補正 *= CV::雪移動低下;
						}
						break;
					}

					if (it.落とし穴時間 > 0)
					{ 
						移動補正 = 0;
						it.落とし穴時間 = 0;
					}

					//移動処理&吹き飛び処理
					it.位置.x += (it.移動速度+ it.吹き飛び量/8) * 移動補正;
					it.吹き飛び量 -= it.吹き飛び量 / 8;
					//陣形回復
					if (軍団[a].陣形回復残り時間 > 0 && 移動補正 != 0)
					{
						double 位置差 = 軍団[a].陣形回復座標 - it.位置.x;

						if (位置差 > it.陣形位置差分) it.位置.x += it.機動力 * 0.5;
						else if (位置差 < it.陣形位置差分) it.位置.x -= it.機動力 * 0.5;
					}
					//連戦補正計算
					if (it.重量補正 < 1.0)
					{
						it.重量補正 += 0.01;
					}
					//モラル変化
					it.士気 += it.士気変化;
					if (it.突撃時間 > 0)
					{
						it.士気 -= 0.02;
						it.突撃時間--;
					}
					//自爆処理
					if (it.自爆時間 > 0)
					{
						it.自爆時間--;
						if (it.自爆時間 == 0)
						{

						}
					}
					//指揮官死亡デバフ
					if (軍団[a].is指揮官生存 == false)
					{
						it.士気 -= CV::指揮官士気ペナルティ;
					}
					//死亡判定
					if (it.部隊数 <= 0)
					{
						it.戦闘状態 = UnitStateType::死亡;
						it.退場時間 = 0;
					}
					//突破判定
					if (it.位置.x < CV::戦場左端 && 軍団[a].前進方向 == DirectionType::左 ||
						it.位置.x > CV::戦場右端 && 軍団[a].前進方向 == DirectionType::右 )
					{
						it.戦闘状態 = UnitStateType::突破;
						it.退場時間 = 0;
					}
					//逃走、衝突判定
					if (it.位置.x < CV::戦場左端 && 軍団[a].前進方向 == DirectionType::右 ||
						it.位置.x > CV::戦場右端 && 軍団[a].前進方向 == DirectionType::左 )
					{
						if (it.is逃走中)
						{
							it.戦闘状態 = UnitStateType::退却;
							it.is描画 = false;
						} else {
							it.蓄積ダメージ += 10;
							it.吹き飛び量 = 0;
							if (it.位置.x < 320) { it.位置.x = CV::戦場左端; }
							if (it.位置.x > 320) { it.位置.x = CV::戦場右端; }
						}
					}
					//士気を0～最大値に収める
					it.士気 = std::max(0.0, it.士気);
					it.士気 = std::min(CV::最大士気, it.士気);
				}
			}
		}

		void UpdateObject()
		{
			//火の移動,消滅判定

			//柵の消滅判定

		}

		void HitUnit()
		{
			for (auto& itA : 軍団[0].兵士)
			{
				if (itA.戦闘状態 != UnitStateType::戦闘中) { continue; }
				for (auto& itB : 軍団[1].兵士)
				{
					if (itB.戦闘状態 != UnitStateType::戦闘中 || itB.重量補正 == 0) { continue; }
					double x差 = std::abs(itA.位置.x - itB.位置.x);
					double y差 = std::abs(itA.位置.y - itB.位置.y);
					if (itA.is巨大 || itB.is巨大){ y差 = 0;}

					if (x差 < CV::当たり判定サイズ && y差 < CV::当たり判定サイズ )
					{
						FightUnit( itA , itB );
						break;
					}
				}
			}
		}

		void FightUnit(UnitData& 兵士A,UnitData& 兵士B)
		{
			UnitData* 兵士[2] = {&兵士A,&兵士B};
			double 攻撃[2] = { 0, 0};//Aが0,Bが1
			double 防御[2] = { 0, 0};
			bool is戦闘[2] = {true,true};//敵に向いているかどうか
			double 押し値[2];
			double 重量値[2] = { 0,0 };

			//攻撃防御計算
			for (int a = 0; a < 2; a++)
			{
				//向きチェック
				if (兵士[a]->向き == 軍団[a].後退方向)
				{
					is戦闘[a] = false;
				}
	
				if( is戦闘[a] == true)
				{
					if (兵士[a]->攻撃補正 > 0)
					{
						攻撃[a] = 兵士[a]->攻撃力 + 兵士[a]->攻撃補正;
						攻撃[a] += Rand::Get(兵士[a]->士気 / 20 + 1);
					}
					防御[a] = 兵士[a]->攻撃力 + 兵士[a]->防御補正;
					防御[a] += Rand::Get(兵士[a]->士気 / 20 + 1);
				} else {
					防御[a] = Rand::Get(兵士[a]->士気 / 40 + 0.5);
				}

				//指揮官死亡デバフ
				if (軍団[a].is指揮官生存 = false)
				{
					攻撃[a] -= 1; 
					防御[a] -= 1;
				}
				//天気補正
				if (天気継続時間 > 0 && 天気 == WeatherType::雨)
				{
					if(兵士[a]->雨耐性 <= 0)
					{
						攻撃[a] += 1;
						防御[a] += 1;
					}
					if(兵士[a]->雨耐性 >= 2)
					{
						攻撃[a] -= 1;
						防御[a] -= 1;
					}
				}
				if (天気継続時間 > 0 && 天気 == WeatherType::雪 )
				{
					if (兵士[a]->雪耐性 <= 0)
					{
						攻撃[a] += 1;
						防御[a] += 1;
					}
					if (兵士[a]->雪耐性 >= 2)
					{
						攻撃[a] -= 1;
						防御[a] -= 1;
					}
				}
			}
			//ダメージ計算
			for (int a = 0,b; a < 2; a++)
			{
				b = 1 - a;
				if (攻撃[b] <= 0) { continue; };

				double ダメージ量;

				ダメージ量 = std::max( 0.0, 攻撃[b] - 防御[a]) + Rand::Get(1.0);
				兵士[a]->蓄積ダメージ += ダメージ量;
				重量値[b] += ダメージ量/10;

				if (兵士[a]->蓄積ダメージ >= 1)
				{
					int ダメージ = int(兵士[a]->蓄積ダメージ);
					兵士[a]->部隊数 -= ダメージ;
					兵士[a]->蓄積ダメージ -= ダメージ;
					AddDamageEffect(ダメージ, a, 兵士[a]->位置);
				}

			}
			//吹き飛び計算
			for (int a = 0; a < 2; a++)
			{
				押し値[a] = std::abs(兵士[a]->機動力 * 兵士[a]->移動速度);
				重量値[a] += 兵士[a]->重量 + 兵士[a]->重量補正;
				//防御中は重さ3倍判定
				if (is戦闘[a] == true && 兵士[a]->防御補正 > 0)
				{
					重量値[a] *= 4;
				}
			}

			for (int a = 0,b; a < 2; a++)
			{
				b = 1 - a;
				//後ろ向きに吹き飛ぶ、80%は吹き飛び量に追加、20%は加算
				兵士[a]->吹き飛び量 -= (押し値[a] * 19 + 押し値[b]) * 10 * 重量値[b] / (重量値[a]+重量値[b]);
				兵士[a]->重量補正 = 0;
			}
		}

		void AddDamageEffect(int ダメージ量,int 軍,Point& 座標)
		{

		}

		void TacticsEffect()
		{
			for (auto& it : 軍団)
			{
				//平均モラル、平均座標等計算
				it.平均値計算();
				//時間差発動等の特殊戦術の処理
				//交代系
				if (it.is交代前進 && std::abs(it.列平均位置[RowType::前列] - it.列平均位置[RowType::後列]) <= CV::交代閾値)
				{
					if (it.現在戦術 == TacticsType::前列交代) Singleton<T前列交代>::Get().前後交代(this,&it);
					if(it.現在戦術 == TacticsType::速攻交代)Singleton<T速攻交代>::Get().前後交代(this, &it);
				}
				else if (it.is交代後退 && std::abs(it.交代前の後列位置 - it.列平均位置[RowType::後列]) <= CV::交代閾値)
				{
					if (it.現在戦術 == TacticsType::前列交代) Singleton<T前列交代>::Get().交代完了(this, &it);
					if (it.現在戦術 == TacticsType::速攻交代)Singleton<T速攻交代>::Get().交代完了(this, &it);
				}
				//時間差発動チェック
				//陣形回復,統率回復,投石部隊
				if (it.発動待ち時間 > 0)
				{
					it.発動待ち時間--;
					it.陣形回復残り時間--;
					if (it.発動待ち時間 == 0)
					{
						戦術データ[it.現在戦術]->時間差効果(this, &it);
					}

				}
				//死んだフリ、これは途中で別命令使ってもOK
			}
		}

		void PlayerInput()
		{
			//ゲームスピード変更


			//戦術選択ウィンドウを出す-閉じる
			if (Input::mouse.Right.on)
			{
				is戦術選択中 = !is戦術選択中;
				return;
			}
			
			//戦術を選ぶ
			if (is戦術選択中 == true)
			{
				Rect 選択枠 = B戦術;
				for (int a = 0; a < 8; a++)
				{
					if (選択枠.Hit(&Input::mouse.GetPoint()) && Input::mouse.Left.on)
					{
						is戦術選択中 = false;
						選択枠.Draw({ 0,0,255,128 });
						戦術データ[TacticsType(a + 戦術テスト)]->発動(this,&軍団[0]);

						return;
					}
					選択枠.Move(0, 40);
				}
			}
		}

		void TacticsAI()
		{

		}
		//●AIサブ処理
		void AI計算()
		{
			//平均モラル<-TacticsEffect()関数で計算
			//前後列平均位置<-TacticsEffect()関数で計算
			//上翼,下翼,前列,後列割合計算

			//交戦中チェック
			//分断チェック
			//最後尾位置計算
			//交戦中部隊モラル計算
			//ダメージ計算予測
			//火による分断
		}
		
		void AIダメージ予測()
		{
		}

		void AI個別戦術選択()
		{
			
		}
		
		void AI基本戦術選択()
		{


		}
		//■■EndCombat■■
	};
}