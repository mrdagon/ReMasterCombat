//Copyright © 2017 (´･@･)
//[License]GNU Affero General Public License, version 3
//[Contact]http://tacoika.blog87.fc2.com/
#pragma once
namespace SDX_RMC
{
	using namespace SDX;

	class ITactics
	{
	public:
		TacticsType 戦術種;
		std::string 戦術名;
		std::string 説明文;
		int 使用可能回数;//無限に使える回数はConstValue.hで定義
		bool is単列使用可 = true;
		bool is複列使用可 = true;
		bool is攻防変化 = true;//現在戦術を変えるかどうか,AI用

		//singletonクラスの様な感じに
		template<typename T>
		static T& GetS()
		{
			static T single;
			return single;
		}

		//使用時効果
		virtual void 発動(ISceneBattele* 戦場,LegionData* 軍団)
		{
			//共通処理
			軍団->前回戦術経過時間 = 0;
			if (is攻防変化 == true)
			{
				軍団->発動待ち戦術 = 戦術種;
			}
			軍団->発動待ち時間 = 0;
			軍団->is交代前進 = false;
			軍団->is交代後退 = false;
			軍団->陣形回復残り時間 = 0;
			軍団->現在戦術 = 戦術種;
			//個別処理
			発動効果(戦場, 軍団);
		};
	
		virtual void 時間差効果(ISceneBattele* 戦場, LegionData* 軍団){}

		void Set性能(int 使用回数, bool is単列, bool is複列, bool 攻防変化)
		{

		}
	
		void Set説明(const char* 名前,const char* 説明)
		{
			this->戦術名 = 名前;
			this->説明文 = 説明;
		}
	private:
		virtual void 発動効果(ISceneBattele* 戦場, LegionData* 軍団) {}
	};

	class T空き : public ITactics
	{
	private:
		void 発動効果(ISceneBattele* 戦場, LegionData* 軍団) override
		{
			//使用不可能＆効果なし
		};

	};
	class T全軍前進 : public ITactics
	{
	private:
		void 発動効果(ISceneBattele* 戦場,LegionData* 軍団) override
		{
			戦場->Set台詞(戦術名,軍団);

			for (auto& it :軍団->兵士)
			{
				it.攻撃補正 = 2;
				it.防御補正 = 0;
				it.向き = 軍団->前進方向;
				it.士気変化 = -0.05;
				it.移動速度 = it.機動力  * 0.5;
				it.フラッシュ時間 = 50;
				it.フラッシュ色 = Color::Red;
				it.is逃走中 = false;
			}
		};

	};
	class T全軍後退 : public ITactics
	{
	private:
		void 発動効果(ISceneBattele* 戦場, LegionData* 軍団) override
		{
			戦場->Set台詞(戦術名, 軍団);

			for (auto& it : 軍団->兵士)
			{
				it.攻撃補正 = 0;
				it.防御補正 = 0;
				it.向き = 軍団->後退方向;
				it.士気変化 = +0.12;
				it.移動速度 = it.機動力  * -0.3;
				it.フラッシュ時間 = 50;
				it.フラッシュ色 = Color::Green;
				it.is逃走中 = false;
			}
		};
	};
	class T全軍防御 : public ITactics
	{
	private:
		void 発動効果(ISceneBattele* 戦場, LegionData* 軍団) override
		{
			戦場->Set台詞(戦術名, 軍団);

			for (auto& it : 軍団->兵士)
			{
				it.攻撃補正 = 0;
				it.防御補正 = 4;
				it.向き = 軍団->前進方向;
				it.士気変化 = -0.01;
				it.移動速度 = 0;
				it.フラッシュ時間 = 50;
				it.フラッシュ色 = Color::Blue;
				it.is逃走中 = false;
			}
		};
	};
	class T全軍突撃 : public ITactics
	{
	private:
		void 発動効果(ISceneBattele* 戦場, LegionData* 軍団) override
		{
			戦場->Set台詞(戦術名, 軍団);

			//ぴょんぴょんはねる&戦術効果
			for (auto& it : 軍団->兵士)
			{
				it.攻撃補正 = 3;
				it.防御補正 = 0;
				it.向き = 軍団->前進方向;
				it.士気変化 = -0.05;
				it.移動速度 = it.機動力  * 0.5;
				it.士気 += CV::最大士気 * 0.6;
				it.ジャンプ回数 = 3;
				it.ジャンプ待ち = Rand::Get(10);
				it.フラッシュ色 = Color::Red;
				it.突撃時間 += 300;
				it.is逃走中 = false;
			}
			戦場->Draw(100);
			//!マーク出す
			for (auto& it : 軍団->兵士)
			{
				it.感情 = EmoteType::驚き;
				it.感情時間 = 50;
			}
			戦場->Draw(70);
			//相手に焦りマーク
			for (auto& it : 軍団->敵軍->兵士)
			{
				if (Rand::Coin(0.6))
				{
					it.感情 = EmoteType::焦り;
					it.感情時間 = 50;
				}
				it.士気 -= CV::最大士気 * 0.4;
			}
			戦場->Draw(70);

			//フラッシュ
			for (auto& it : 軍団->敵軍->兵士)
			{
				it.フラッシュ時間 = 50;
			}
		};
	};
	class T全軍退却 : public ITactics
	{
	private:
		void 発動効果(ISceneBattele* 戦場, LegionData* 軍団) override
		{
			戦場->Set台詞(戦術名, 軍団);

			for (auto& it : 軍団->兵士)
			{
				it.攻撃補正 = 0;
				it.防御補正 = 0;
				it.向き = 軍団->後退方向;
				it.士気変化 = -0.05;
				it.移動速度 = it.機動力  * -0.45;
				//フラッシュしない
				it.is逃走中 = true;
			}
		};
	};
	class T前列前進 : public ITactics
	{
	private:
		void 発動効果(ISceneBattele* 戦場, LegionData* 軍団) override
		{
			戦場->Set台詞(戦術名, 軍団);

			for (auto& it : 軍団->兵士)
			{
				if (it.現在列 == RowType::前列)
				{
					it.攻撃補正 = 3;
					it.防御補正 = 0;
					it.士気変化 = -0.05;
					it.移動速度 = it.機動力  * 0.5;
					it.フラッシュ色 = Color::Red;
				} else {
					it.攻撃補正 = 0;
					it.防御補正 = 4;
					it.士気変化 = -0.01;
					it.移動速度 = 0;
					it.フラッシュ色 = Color::Blue;
				}
				it.向き = 軍団->前進方向;
				it.フラッシュ時間 = 50;
				it.is逃走中 = false;
			}
		};
	};
	class T後列前進 : public ITactics
	{
	private:
		void 発動効果(ISceneBattele* 戦場, LegionData* 軍団) override
		{
			戦場->Set台詞(戦術名, 軍団);

			for (auto& it : 軍団->兵士)
			{
				if (it.現在列 == RowType::前列)
				{
					it.攻撃補正 = 0;
					it.防御補正 = 4;
					it.士気変化 = -0.01;
					it.移動速度 = 0;
					it.フラッシュ色 = Color::Blue;
				} else {
					it.攻撃補正 = 3;
					it.防御補正 = 0;
					it.士気変化 = -0.05;
					it.移動速度 = it.機動力  * 0.5;
					it.フラッシュ色 = Color::Red;
				}

				it.向き = 軍団->前進方向;
				it.フラッシュ時間 = 50;
				it.is逃走中 = false;
			}
		};
	};
	class T速攻前進 : public ITactics
	{
	private:
		void 発動効果(ISceneBattele* 戦場, LegionData* 軍団) override
		{
			戦場->Set台詞(戦術名, 軍団);

			for (auto& it : 軍団->兵士)
			{
				if (it.現在列 == RowType::前列)
				{
					it.攻撃補正 = 3;
					it.防御補正 = 0;
					it.士気変化 = -0.05;
					it.移動速度 = it.機動力  * 0.75;
					it.フラッシュ色 = Color::Red;
				} else {
					it.攻撃補正 = 0;
					it.防御補正 = 0;
					it.士気変化 = 0.12;
					it.移動速度 = 0;
					it.フラッシュ色 = Color::Green;
				}
				it.向き = 軍団->前進方向;
				it.フラッシュ時間 = 50;
				it.is逃走中 = false;
			}
		};
	};
	class T強撃前進 : public ITactics
	{
	private:
		void 発動効果(ISceneBattele* 戦場, LegionData* 軍団) override
		{
			戦場->Set台詞(戦術名, 軍団);

			for (auto& it : 軍団->兵士)
			{
				if (it.現在列 == RowType::前列)
				{
					it.攻撃補正 = 4;
					it.防御補正 = 0;
					it.士気変化 = -0.05;
					it.移動速度 = it.機動力  * 0.25;
					it.フラッシュ色 = Color::Red;
				} else {
					it.攻撃補正 = 0;
					it.防御補正 = 0;
					it.士気変化 = 0.12;
					it.移動速度 = 0;
					it.フラッシュ色 = Color::Green;
				}
				it.向き = 軍団->前進方向;
				it.フラッシュ時間 = 50;
				it.is逃走中 = false;
			}
		};
	};
	class T上翼攻撃 : public ITactics
	{
	private:
		void 発動効果(ISceneBattele* 戦場, LegionData* 軍団) override
		{
			戦場->Set台詞(戦術名, 軍団);

			for (auto& it : 軍団->兵士)
			{
				if (it.位置.y <= 150)
				{
					it.攻撃補正 = 4;
					it.防御補正 = 0;
					it.士気変化 = -0.05;
					it.移動速度 = it.機動力  * 0.5;
					it.フラッシュ色 = Color::Red;
				} else {
					it.攻撃補正 = 0;
					it.防御補正 = 5;
					it.士気変化 = -0.02;
					it.移動速度 = 0;
					it.フラッシュ色 = Color::Blue;
				}
				it.向き = 軍団->前進方向;
				it.フラッシュ時間 = 50;
				it.is逃走中 = false;
			}
		};
	};
	class T下翼攻撃 : public ITactics
	{
	private:
		void 発動効果(ISceneBattele* 戦場, LegionData* 軍団) override
		{
			戦場->Set台詞(戦術名, 軍団);

			for (auto& it : 軍団->兵士)
			{
				if (it.位置.y >= 150)
				{
					it.攻撃補正 = 4;
					it.防御補正 = 0;
					it.士気変化 = -0.05;
					it.移動速度 = it.機動力  * 0.5;
					it.フラッシュ色 = Color::Red;
				} else {
					it.攻撃補正 = 0;
					it.防御補正 = 5;
					it.士気変化 = -0.02;
					it.移動速度 = 0;
					it.フラッシュ色 = Color::Blue;
				}
				it.向き = 軍団->前進方向;
				it.フラッシュ時間 = 50;
				it.is逃走中 = false;
			}
		};
	};
	class T全軍前進･強 : public ITactics
	{
	private:
		void 発動効果(ISceneBattele* 戦場, LegionData* 軍団) override
		{
			戦場->Set台詞(戦術名, 軍団);

			for (auto& it : 軍団->兵士)
			{
				it.攻撃補正 = 4;
				it.防御補正 = 0;
				it.士気変化 = -0.06;
				it.移動速度 = it.機動力  * 0.5;
				it.向き = 軍団->前進方向;
				it.フラッシュ時間 = 50;
				it.フラッシュ色 = Color::Red;
				it.is逃走中 = false;
			}
		};
	};
	class T全軍休息 : public ITactics
	{
	private:
		void 発動効果(ISceneBattele* 戦場, LegionData* 軍団) override
		{
			戦場->Set台詞(戦術名, 軍団);

			for (auto& it : 軍団->兵士)
			{
				it.攻撃補正 = 0;
				it.防御補正 = 0;
				it.士気変化 = 0.12;
				it.移動速度 = 0;
				it.向き = 軍団->前進方向;
				it.フラッシュ時間 = 50;
				it.フラッシュ色 = Color::Green;
				it.is逃走中 = false;
			}
		};
	};
	class T速攻後退 : public ITactics
	{
	private:
		void 発動効果(ISceneBattele* 戦場, LegionData* 軍団) override
		{
			戦場->Set台詞(戦術名, 軍団);

			for (auto& it : 軍団->兵士)
			{
				it.攻撃補正 = 0;
				it.防御補正 = 0;
				it.士気変化 = 0.12;
				it.移動速度 = it.機動力  * -0.45;
				it.向き = 軍団->後退方向;
				it.フラッシュ時間 = 50;
				it.フラッシュ色 = Color::Green;
				it.is逃走中 = false;
			}
		};
	};
	class T後退攻撃 : public ITactics
	{
	private:
		void 発動効果(ISceneBattele* 戦場, LegionData* 軍団) override
		{
			戦場->Set台詞(戦術名, 軍団);

			for (auto& it : 軍団->兵士)
			{
				it.攻撃補正 = 1;
				it.防御補正 = 0;
				it.向き = 軍団->前進方向;
				it.士気変化 = 0.04;
				it.移動速度 = it.機動力  * -0.1;
				it.フラッシュ時間 = 50;
				it.フラッシュ色 = Color::Red;
				it.is逃走中 = false;
			}
		};
	};
	class T後退防御 : public ITactics
	{
	private:
		void 発動効果(ISceneBattele* 戦場, LegionData* 軍団) override
		{
			戦場->Set台詞(戦術名, 軍団);

			for (auto& it : 軍団->兵士)
			{
				it.攻撃補正 = 0;
				it.防御補正 = 2;
				it.士気変化 = 0.05;
				it.移動速度 = it.機動力  * -0.1;
				it.向き = 軍団->前進方向;
				it.フラッシュ時間 = 50;
				it.フラッシュ色 = Color::Blue;
				it.is逃走中 = false;
			}
		};
	};
	class T前列防御 : public ITactics
	{
	private:
		void 発動効果(ISceneBattele* 戦場, LegionData* 軍団) override
		{
			戦場->Set台詞(戦術名, 軍団);

			for (auto& it : 軍団->兵士)
			{
				if (it.現在列 == RowType::前列)
				{
					it.攻撃補正 = 0;
					it.防御補正 = 5;
					it.士気変化 = -0.01;
					it.移動速度 = 0;
					it.フラッシュ色 = Color::Blue;
				} else {
					it.攻撃補正 = 0;
					it.防御補正 = 0;
					it.士気変化 = 0.12;
					it.移動速度 = 0;
					it.フラッシュ色 = Color::Green;
				}

				it.向き = 軍団->前進方向;
				it.フラッシュ時間 = 50;
				it.is逃走中 = false;
			}
		};
	};
	class T後列後退 : public ITactics
	{
	private:
		void 発動効果(ISceneBattele* 戦場, LegionData* 軍団) override
		{
			戦場->Set台詞(戦術名, 軍団);

			for (auto& it : 軍団->兵士)
			{
				if (it.現在列 == RowType::前列)
				{
					it.攻撃補正 = 0;
					it.防御補正 = 5;
					it.士気変化 = -0.01;
					it.移動速度 = 0;
					it.向き = 軍団->前進方向;
					it.フラッシュ色 = Color::Blue;
				} else {
					it.攻撃補正 = 0;
					it.防御補正 = 0;
					it.士気変化 = 0.12;
					it.移動速度 = it.機動力  * -0.3;
					it.向き = 軍団->後退方向;
					it.フラッシュ色 = Color::Green;
				}

				it.フラッシュ時間 = 50;
				it.is逃走中 = false;
			}
		};
	};
	class T前列死守 : public ITactics
	{
	private:
		void 発動効果(ISceneBattele* 戦場, LegionData* 軍団) override
		{
			戦場->Set台詞(戦術名, 軍団);

			for (auto& it : 軍団->兵士)
			{
				if (it.現在列 == RowType::前列)
				{
					it.攻撃補正 = 0;
					it.防御補正 = 9;
					it.士気変化 = -0.03;
					it.移動速度 = 0;
					it.フラッシュ色 = Color::Blue;
				} else {
					it.攻撃補正 = 0;
					it.防御補正 = 0;
					it.士気変化 = 0.12;
					it.移動速度 = 0;
					it.フラッシュ色 = Color::Green;
				}

				it.向き = 軍団->前進方向;
				it.フラッシュ時間 = 50;
				it.is逃走中 = false;
			}
		}
	};
	class T全軍防御･強 : public ITactics
	{
	private:
		void 発動効果(ISceneBattele* 戦場, LegionData* 軍団) override
		{
			戦場->Set台詞(戦術名, 軍団);

			for (auto& it : 軍団->兵士)
			{
				it.攻撃補正 = 0;
				it.防御補正 = 6;
				it.士気変化 = -0.03;
				it.移動速度 = 0;
				it.向き = 軍団->前進方向;
				it.フラッシュ時間 = 50;
				it.フラッシュ色 = Color::Blue;
				it.is逃走中 = false;
			}
		};
	};
	class T前列突撃 : public ITactics
	{
	private:
		void 発動効果(ISceneBattele* 戦場, LegionData* 軍団) override
		{
			戦場->Set台詞(戦術名, 軍団);

			//ぴょんぴょんはねる&戦術効果
			for (auto& it : 軍団->兵士)
			{
				if (it.現在列 == RowType::前列)
				{
					it.攻撃補正 = 3;
					it.防御補正 = 0;
					it.士気変化 = -0.05;
					it.移動速度 = it.機動力  * 0.5;
					it.士気 += CV::最大士気 * 0.5;
					it.フラッシュ色 = Color::Red;
					it.突撃時間 += 250;
					it.ジャンプ回数 = 3;
					it.ジャンプ待ち = Rand::Get(10);
				} else {
					it.攻撃補正 = 0;
					it.防御補正 = 4;
					it.士気変化 = -0.01;
					it.移動速度 = 0;
					it.フラッシュ色 = Color::Blue;
				}

				it.向き = 軍団->前進方向;
				it.is逃走中 = false;
			}
			戦場->Draw(100);
			//!マーク出す
			for (auto& it : 軍団->兵士)
			{
				if (it.現在列 == RowType::前列)
				{
					it.感情 = EmoteType::驚き;
					it.感情時間 = 50;
				}
			}
			戦場->Draw(70);
			//相手に焦りマーク
			for (auto& it : 軍団->敵軍->兵士)
			{
				if (Rand::Coin(0.4))
				{
					it.感情 = EmoteType::焦り;
					it.感情時間 = 50;
				}
				it.士気 -= CV::最大士気 * 0.25;
			}
			戦場->Draw(70);

			//フラッシュ
			for (auto& it : 軍団->兵士)
			{
				it.フラッシュ時間 = 50;
			}
		};
	};
	class T後列突撃 : public ITactics
	{
	private:
		void 発動効果(ISceneBattele* 戦場, LegionData* 軍団) override
		{
			戦場->Set台詞(戦術名, 軍団);

			//ぴょんぴょんはねる&戦術効果
			for (auto& it : 軍団->兵士)
			{
				if (it.現在列 == RowType::後列)
				{
					it.攻撃補正 = 3;
					it.防御補正 = 0;
					it.士気変化 = -0.05;
					it.移動速度 = it.機動力  * 0.5;
					it.士気 += CV::最大士気 * 0.5;
					it.突撃時間 = 250;
					it.フラッシュ色 = Color::Red;
					it.ジャンプ回数 = 3;
					it.ジャンプ待ち = Rand::Get(10);
				} else {
					it.攻撃補正 = 0;
					it.防御補正 = 4;
					it.士気変化 = -0.01;
					it.移動速度 = 0;
					it.フラッシュ色 = Color::Blue;
				}

				it.向き = 軍団->前進方向;
				it.is逃走中 = false;
			}
			戦場->Draw(100);
			//!マーク出す
			for (auto& it : 軍団->兵士)
			{
				if (it.現在列 == RowType::後列)
				{
					it.感情 = EmoteType::驚き;
					it.感情時間 = 50;
				}
			}
			戦場->Draw(70);
			//相手に焦りマーク
			for (auto& it : 軍団->敵軍->兵士)
			{
				if (Rand::Coin(0.4))
				{
					it.感情 = EmoteType::焦り;
					it.感情時間 = 50;
				}
				it.士気 -= CV::最大士気 * 0.25;
			}
			戦場->Draw(70);

			//フラッシュ
			for (auto& it : 軍団->兵士)
			{
				it.フラッシュ時間 = 50;
			}
		};
	};
	class T上翼突撃 : public ITactics
	{
	private:
		void 発動効果(ISceneBattele* 戦場, LegionData* 軍団) override
		{
			戦場->Set台詞(戦術名, 軍団);

			//ぴょんぴょんはねる&戦術効果
			for (auto& it : 軍団->兵士)
			{
				if (it.位置.y <= 150)
				{
					it.攻撃補正 = 4;
					it.防御補正 = 0;
					it.士気変化 = -0.05;
					it.移動速度 = it.機動力  * 0.5;
					it.士気 += CV::最大士気 * 0.5;
					it.フラッシュ色 = Color::Red;
					it.突撃時間 = 250;
					it.ジャンプ回数 = 3;
					it.ジャンプ待ち = Rand::Get(10);
				} else {
					it.攻撃補正 = 0;
					it.防御補正 = 5;
					it.士気変化 = -0.02;
					it.移動速度 = 0;
					it.フラッシュ色 = Color::Blue;
				}

				it.向き = 軍団->前進方向;
				it.is逃走中 = false;
			}
			戦場->Draw(100);
			//!マーク出す
			for (auto& it : 軍団->兵士)
			{
				if (it.位置.y <= 150)
				{
					it.感情 = EmoteType::驚き;
					it.感情時間 = 50;
				}
			}
			戦場->Draw(70);
			//相手に焦りマーク
			for (auto& it : 軍団->敵軍->兵士)
			{
				if (Rand::Coin(0.4))
				{
					it.感情 = EmoteType::焦り;
					it.感情時間 = 50;
				}
				it.士気 -= CV::最大士気 * 0.25;
			}
			戦場->Draw(70);

			//フラッシュ
			for (auto& it : 軍団->兵士)
			{
				it.フラッシュ時間 = 50;
			}
		};
	};
	class T下翼突撃 : public ITactics
	{
	private:
		void 発動効果(ISceneBattele* 戦場, LegionData* 軍団) override
		{
			戦場->Set台詞(戦術名, 軍団);

			//ぴょんぴょんはねる&戦術効果
			for (auto& it : 軍団->兵士)
			{
				if (it.位置.y >= 150)
				{
					it.攻撃補正 = 4;
					it.防御補正 = 0;
					it.士気変化 = -0.07;
					it.移動速度 = it.機動力  * 0.5;
					it.士気 += CV::最大士気 * 0.5;
					it.フラッシュ色 = Color::Red;
					it.突撃時間 = 250;
					it.ジャンプ回数 = 3;
					it.ジャンプ待ち = Rand::Get(10);
				} else {
					it.攻撃補正 = 0;
					it.防御補正 = 5;
					it.士気変化 = -0.02;
					it.移動速度 = 0;
					it.フラッシュ色 = Color::Blue;
				}
				it.向き = 軍団->前進方向;
				it.is逃走中 = false;
			}
			戦場->Draw(100);
			//!マーク出す
			for (auto& it : 軍団->兵士)
			{
				if (it.位置.y >= 150)
				{
					it.感情 = EmoteType::驚き;
					it.感情時間 = 50;
				}
			}
			戦場->Draw(70);
			//相手に焦りマーク
			for (auto& it : 軍団->敵軍->兵士)
			{
				if (Rand::Coin(0.4))
				{
					it.感情 = EmoteType::焦り;
					it.感情時間 = 50;
				}
				it.士気 -= CV::最大士気 * 0.25;
			}
			戦場->Draw(70);

			//フラッシュ
			for (auto& it : 軍団->兵士)
			{
				it.フラッシュ時間 = 50;
			}
		};
	};
	class T指揮官突撃 : public ITactics
	{
	private:
		void 発動効果(ISceneBattele* 戦場, LegionData* 軍団) override
		{
			戦場->Set台詞(戦術名, 軍団);
			//!マーク出す
			for (auto& it : 軍団->兵士)
			{
				if (it.is指揮官 == true)
				{
					it.攻撃補正 = 5;
					it.防御補正 = 0;
					it.士気変化 = -0.05;
					it.移動速度 = it.機動力  * 0.75;
					it.士気 = 100;
					it.フラッシュ色 = Color::Red;
					it.感情 = EmoteType::驚き;
					it.感情時間 = 80;
				} else {
					it.攻撃補正 = 0;
					it.防御補正 = 4;
					it.士気変化 = 0.04;
					it.移動速度 = 0;
					it.フラッシュ色 = Color::Blue;
				}
				it.向き = 軍団->前進方向;
				it.is逃走中 = false;
			}
			戦場->Draw(80);

			//フラッシュ
			for (auto& it : 軍団->兵士)
			{
				it.フラッシュ時間 = 50;
			}
		}

	};
	class T前列交代 : public ITactics
	{
	public:
		void 前後交代(ISceneBattele* 戦場, LegionData* 軍団)
		{
			軍団->is交代前進 = false;
			軍団->is交代後退 = true;

			for (auto& it : 軍団->兵士)
			{
				if (it.現在列 == RowType::前列)
				{
					it.攻撃補正 = 0;
					it.防御補正 = 0;
					it.士気変化 = +0.12;
					it.移動速度 = it.機動力  * -0.3;
					it.向き = 軍団->後退方向;
					it.現在列 = RowType::後列;
					it.フラッシュ色 = Color::Green;
				} else {
					it.攻撃補正 = 2;
					it.防御補正 = 0;
					it.士気変化 = -0.05;
					it.移動速度 = 0;
					it.向き = 軍団->前進方向;
					it.現在列 = RowType::前列;
					it.フラッシュ色 = Color::Red;
				}

				it.フラッシュ時間 = 50;

				it.is逃走中 = false;
			}
		}
		void 交代完了(ISceneBattele* 戦場, LegionData* 軍団)
		{
			軍団->is交代後退 = false;

			for (auto& it : 軍団->兵士)
			{
				if (it.現在列 == RowType::前列)
				{
					it.攻撃補正 = 3;
					it.防御補正 = 0;
					it.士気変化 = -0.05;
					it.移動速度 = it.機動力  * 0.5;
					it.向き = 軍団->前進方向;
					it.フラッシュ色 = Color::Red;
				} else {
					it.攻撃補正 = 0;
					it.防御補正 = 0;
					it.士気変化 = +0.12;
					it.移動速度 = 0;
					it.向き = 軍団->前進方向;
					it.フラッシュ色 = Color::Green;
				}

				it.フラッシュ時間 = 50;

				it.is逃走中 = false;
			}


		}

	private:
		void 発動効果(ISceneBattele* 戦場, LegionData* 軍団) override
		{
			戦場->Set台詞(戦術名, 軍団);
			軍団->is交代前進 = true;
			軍団->交代前の後列位置 = 軍団->列平均位置[RowType::後列];

			for (auto& it : 軍団->兵士)
			{
				if (it.現在列 == RowType::前列)
				{
					it.攻撃補正 = 0;
					it.防御補正 = 4;
					it.士気変化 = -0.01;
					it.移動速度 = 0;
					it.フラッシュ色 = Color::Blue;
				} else {
					it.攻撃補正 = 2;
					it.防御補正 = 0;
					it.士気変化 = -0.05;
					it.移動速度 = it.機動力  * 0.5;
					it.フラッシュ色 = Color::Red;
				}

				it.向き = 軍団->前進方向;
				it.フラッシュ時間 = 50;
				it.is逃走中 = false;
			}
		};

		void 時間差効果(ISceneBattele* 戦場, LegionData* 軍団) override
		{
			//前列前進使用と同じ状態になる
			軍団->現在戦術 = TacticsType::前列前進;
		
		}
	};
	class T速攻交代 : public ITactics
	{
	public:
		void 前後交代(ISceneBattele* 戦場, LegionData* 軍団)
		{

		}
		void 交代完了(ISceneBattele* 戦場, LegionData* 軍団)
		{

		}
	private:
		void 発動効果(ISceneBattele* 戦場, LegionData* 軍団) override
		{
			//todo
			for (auto& it : 軍団->兵士)
			{
				if (it.現在列 == RowType::前列)
				{
					it.攻撃補正 = 0;
					it.防御補正 = 4;
					it.士気変化 = -0.01;
					it.移動速度 = 0;
					it.フラッシュ色 = Color::Blue;
				}
				else {
					it.攻撃補正 = 2;
					it.防御補正 = 0;
					it.士気変化 = -0.05;
					it.移動速度 = it.機動力  * 0.75;
					it.フラッシュ色 = Color::Red;
				}

				it.向き = 軍団->前進方向;
				it.フラッシュ時間 = 50;
				it.is逃走中 = false;
			}
		};
		
		void 時間差効果(ISceneBattele* 戦場, LegionData* 軍団) override
		{
			//前列前進から

			//前列前進使用と同じ状態になる
			軍団->現在戦術 = TacticsType::前列前進;
		}
	};
	class T陣形回復 : public ITactics
	{
	private:
		void 発動効果(ISceneBattele* 戦場, LegionData* 軍団) override
		{
			軍団->発動待ち最大 = 360;
			軍団->発動待ち時間 = 360;
			軍団->陣形回復残り時間 = 360;
			軍団->陣形回復座標 = 軍団->平均位置;
			//todo 6秒
			for (auto& it : 軍団->兵士)
			{
				it.攻撃補正 = 0;
				it.防御補正 = 0;
				it.士気変化 = 0.06;
				it.移動速度 = 0;
				it.向き = 軍団->前進方向;
				it.フラッシュ色 = Color::Green;
				it.フラッシュ時間 = 50;
				it.is逃走中 = false;
			}
		};

		void 時間差効果(ISceneBattele* 戦場, LegionData* 軍団) override
		{
			//フラッシュして＆モラル回復
			戦場->Set台詞(戦術名, 軍団);
			for (auto& it : 軍団->兵士)
			{
				it.士気 = CV::最大士気;
				it.フラッシュ時間 = 50;
				it.フラッシュ色 = Color::White;
			}
			戦場->Draw(50);
			//音符マーク
			for (auto& it : 軍団->兵士)
			{
				it.感情 = EmoteType::音符;
				it.感情時間 = 50;
			}
			戦場->Draw(70);
			//全軍前進
			for (auto& it : 軍団->兵士)
			{
				it.攻撃補正 = 2;
				it.防御補正 = 0;
				it.向き = 軍団->前進方向;
				it.士気変化 = -0.05;
				it.移動速度 = it.機動力  * 0.5;
				it.フラッシュ時間 = 50;
				it.フラッシュ色 = Color::Red;
				it.is逃走中 = false;
			}
		}
	};
	class T陣形回復･速 : public ITactics
	{
	private:
		void 発動効果(ISceneBattele* 戦場, LegionData* 軍団) override
		{
			軍団->発動待ち最大 = 300;
			軍団->発動待ち時間 = 300;
			軍団->陣形回復残り時間 = 300;
			軍団->陣形回復座標 = 軍団->平均位置;

			for (auto& it : 軍団->兵士)
			{
				it.攻撃補正 = 0;
				it.防御補正 = 0;
				it.向き = 軍団->前進方向;
				it.士気変化 = 0.06;
				it.移動速度 = 0;
				it.フラッシュ色 = Color::Green;
				it.フラッシュ時間 = 50;
				it.is逃走中 = false;
			}
		}

		void 時間差効果(ISceneBattele* 戦場, LegionData* 軍団) override
		{
			//フラッシュして＆モラル回復
			戦場->Set台詞(戦術名, 軍団);
			for (auto& it : 軍団->兵士)
			{
				it.士気 = CV::最大士気;
				it.フラッシュ時間 = 50;
				it.フラッシュ色 = Color::White;
			}
			戦場->Draw(50);
			//音符マーク
			for (auto& it : 軍団->兵士)
			{
				it.感情 = EmoteType::音符;
				it.感情時間 = 50;
			}
			戦場->Draw(70);
			//全軍前進
			for (auto& it : 軍団->兵士)
			{
				it.攻撃補正 = 2;
				it.防御補正 = 0;
				it.向き = 軍団->前進方向;
				it.士気変化 = -0.05;
				it.移動速度 = it.機動力  * 0.5;
				it.フラッシュ時間 = 50;
				it.フラッシュ色 = Color::Red;
				it.is逃走中 = false;
			}
		}
				
		};
	class T統率回復･弱 : public ITactics
	{
	private:
		void 発動効果(ISceneBattele* 戦場, LegionData* 軍団) override
		{
			軍団->発動待ち最大 = 120;
			軍団->発動待ち時間 = 120;
		};

		void 時間差効果(ISceneBattele* 戦場, LegionData* 軍団) override
		{
			//モラル10%回復、白く光る
			戦場->Set台詞(戦術名, 軍団);
			for (auto& it : 軍団->兵士)
			{
				it.士気 += CV::最大士気 * 0.1;
				it.フラッシュ時間 = 50;
				it.フラッシュ色 = Color::White;
			}
			戦場->Draw(50);
			//音符マーク
			for (auto& it : 軍団->兵士)
			{
				it.感情 = EmoteType::音符;
				it.感情時間 = 50;
			}
			戦場->Draw(70);
		}
	};
	class T統率回復･強 : public ITactics
	{
	private:
		void 発動効果(ISceneBattele* 戦場, LegionData* 軍団) override
		{
			軍団->発動待ち最大 = 180;
			軍団->発動待ち時間 = 180;
		}

		void 時間差効果(ISceneBattele* 戦場, LegionData* 軍団) override
		{
			//モラル20%回復、白く光る
			//モラル10%回復、白く光る
			戦場->Set台詞(戦術名, 軍団);
			for (auto& it : 軍団->兵士)
			{
				it.士気 += CV::最大士気 * 0.2;
				it.フラッシュ時間 = 50;
				it.フラッシュ色 = Color::White;
			}
			戦場->Draw(50);
			//音符マーク
			for (auto& it : 軍団->兵士)
			{
				it.感情 = EmoteType::音符;
				it.感情時間 = 50;
			}
			戦場->Draw(70);
		}
	};
	class T速攻退却 : public ITactics
	{
	private:
		void 発動効果(ISceneBattele* 戦場, LegionData* 軍団) override
		{
			戦場->Set台詞(戦術名, 軍団);

			for (auto& it : 軍団->兵士)
			{
				it.攻撃補正 = 0;
				it.防御補正 = 0;
				it.士気変化 = 0.0;
				it.移動速度 = it.機動力  * -1.0;
				it.向き = 軍団->後退方向;
				it.is逃走中 = true;
			}
		};
	};
	class T死んだフリ : public ITactics
	{
	private:
		void 発動効果(ISceneBattele* 戦場, LegionData* 軍団) override
		{
			//todo
			for (auto& it : 軍団->兵士)
			{
				if ((it.現在列 == RowType::前列 || it.is指揮官 == true) && it.戦闘状態 == UnitStateType::戦闘中)
				{
					it.戦闘状態 = UnitStateType::死亡;
					it.攻撃補正 = 0;
					it.防御補正 = 0;
					it.士気変化 = 0.0;
					it.移動速度 = 0;
				} else {
					it.攻撃補正 = 0;
					it.防御補正 = 0;
					it.士気変化 = 0.12;
					it.移動速度 = 0;
					it.フラッシュ色 = Color::Green;
				}
				it.向き = 軍団->前進方向;
				it.フラッシュ時間 = 50;
				it.is逃走中 = false;
			}
			軍団->死んだふり発動待ち時間 = 300;
		}

		void 時間差効果(ISceneBattele* 戦場, LegionData* 軍団) override
		{
			//死んだふり発動
		}
	};
	class T雨ごい : public ITactics
	{
	private:
		void 発動効果(ISceneBattele* 戦場, LegionData* 軍団) override
		{
			//todo
			//防御命令効果削除
			戦場->天気 = WeatherType::雨;
			戦場->天気継続時間 = 600;
		}
	};
	class T冬将軍 : public ITactics
	{
	private:
		void 発動効果(ISceneBattele* 戦場, LegionData* 軍団) override
		{
			for (auto& it : 軍団->兵士)
			{
				//todo
				//防御命令効果削除
				戦場->天気 = WeatherType::雨;
				戦場->天気継続時間 = 600;
			}
		};
	};
	class T分断作戦 : public ITactics
	{
	private:
		void 発動効果(ISceneBattele* 戦場, LegionData* 軍団) override
		{
			//todo
			for (auto& it : 軍団->兵士)
			{
				it.攻撃補正 = 3;
				it.防御補正 = 0;
				it.士気変化 = -0.05;
				it.移動速度 = it.機動力  * 0.5;
				it.向き = 軍団->前進方向;
				it.フラッシュ時間 = 50;
				it.is逃走中 = false;
			}
		};
	};
	class T爆裂部隊 : public ITactics
	{
	private:
		void 発動効果(ISceneBattele* 戦場, LegionData* 軍団) override
		{
			//todo
			int count = 0;

			for (auto& it : 軍団->兵士)
			{
				count++;
				if (it.現在列 == RowType::前列 && count >= 2 && it.is指揮官 == false)
				{
					count = 0;
					it.攻撃補正 = 4;
					it.防御補正 = 0;
					it.士気変化 = -0.05;
					it.移動速度 = it.機動力  * 0.5;
					it.自爆時間 = 300;
				} else {
					it.攻撃補正 = 4;
					it.防御補正 = 0;
					it.士気変化 = -0.01;
					it.移動速度 = 0;
				}

				it.向き = 軍団->前進方向;
				it.フラッシュ時間 = 50;
				it.is逃走中 = false;
			}
		}
		
		void 時間差効果(ISceneBattele* 戦場, LegionData* 軍団) override
		{}
	};
	class T火攻め : public ITactics
	{
	private:
		void 発動効果(ISceneBattele* 戦場, LegionData* 軍団) override
		{
			//todo
			for (auto& it : 軍団->兵士)
			{
				it.攻撃補正 = 0;
				it.防御補正 = 0;
				it.士気変化 = 0.12;
				it.移動速度 = 0;
				it.向き = 軍団->前進方向;
				it.フラッシュ時間 = 50;
				it.is逃走中 = false;
			}
		};
	};
	class Tおとり作戦 : public ITactics
	{
	private:
		void 発動効果(ISceneBattele* 戦場, LegionData* 軍団) override
		{
			//todo
			for (auto& it : 軍団->兵士)
			{
				if (it.is指揮官 == true)
				{
					it.攻撃補正 = 0;
					it.防御補正 = 0;
					it.士気変化 = 0.12;
					it.移動速度 = it.機動力  * -0.4;
					it.向き = 軍団->後退方向;
				} else {
					it.攻撃補正 = 0;
					it.防御補正 = 4;
					it.士気変化 = -0.01;
					it.移動速度 = it.機動力  * 0.5;
					it.向き = 軍団->前進方向;
				}
				it.フラッシュ時間 = 50;
				it.is逃走中 = false;
			}
		};
	};
	class T穴掘り作戦 : public ITactics
	{
	private:
		void 発動効果(ISceneBattele* 戦場, LegionData* 軍団) override
		{
			//todo
		};
	};
	class T防柵作戦 : public ITactics
	{
	private:
		void 発動効果(ISceneBattele* 戦場, LegionData* 軍団) override
		{
			//todo
			for (auto& it : 軍団->兵士)
			{
				it.攻撃補正 = 0;
				it.防御補正 = 0;
				it.士気変化 = 0.12;
				it.移動速度 = 0;
				it.向き = 軍団->前進方向;
				it.フラッシュ時間 = 50;
				it.is逃走中 = false;
			}
		};
	};
	class T水際作戦 : public ITactics
	{
	private:
		void 発動効果(ISceneBattele* 戦場, LegionData* 軍団) override
		{
			//todo-後に柵を置いて
		}
	};
	class T背火の計 : public ITactics
	{
	private:
		void 発動効果(ISceneBattele* 戦場, LegionData* 軍団) override
		{
			//todo
			for (auto& it : 軍団->兵士)
			{
				it.攻撃補正 = 4;
				it.防御補正 = 0;
				it.士気変化 = -0.05;
				it.移動速度 = it.機動力  * 0.25;
				it.向き = 軍団->前進方向;
				it.フラッシュ時間 = 50;
				it.is逃走中 = false;
			}
		}
	};
	class T戦場の風 : public ITactics
	{
	private:
		void 発動効果(ISceneBattele* 戦場, LegionData* 軍団) override
		{
			//todo
			for (auto& it : 軍団->兵士)
			{
				it.攻撃補正 = 3;
				it.防御補正 = 0;
				it.士気変化 = -0.05;
				it.移動速度 = it.機動力  * 0.9;
				it.向き = 軍団->前進方向;
				it.フラッシュ時間 = 50;
				it.is逃走中 = false;
			}
		};
	};
	class T情報操作L1 : public ITactics
	{
	private:
		void 発動効果(ISceneBattele* 戦場, LegionData* 軍団) override
		{
			//todo

		};
	};
	class T情報操作L2 : public ITactics
	{
	private:
		void 発動効果(ISceneBattele* 戦場, LegionData* 軍団) override
		{
			//todo

		};
	};
	class T情報操作L3 : public ITactics
	{
	private:
		void 発動効果(ISceneBattele* 戦場, LegionData* 軍団) override
		{
			//todo		
		};
	};
	class T情報操作L4 : public ITactics
	{
	private:
		void 発動効果(ISceneBattele* 戦場, LegionData* 軍団) override
		{
			//todo
		};
	};
	class T情報操作L5 : public ITactics
	{
	private:
		void 発動効果(ISceneBattele* 戦場, LegionData* 軍団) override
		{
			//todo
		};
	};
	class T投石部隊 : public ITactics
	{
	private:
		void 発動効果(ISceneBattele* 戦場, LegionData* 軍団) override
		{
			//todo
			//投石予告
		}

		void 時間差効果(ISceneBattele* 戦場, LegionData* 軍団) override
		{
			//投石落下
		}
	};
	class T信じる者達 : public ITactics
	{
	private:
		void 発動効果(ISceneBattele* 戦場, LegionData* 軍団) override
		{
			//todo
			//爆裂部隊の人数少ない版
		};
	};
	class T生命回復 : public ITactics
	{
	private:
		void 発動効果(ISceneBattele* 戦場, LegionData* 軍団) override
		{
			for (auto& it : 軍団->兵士)
			{
				if (it.戦闘状態 != UnitStateType::戦闘中)
				{
					it.部隊数 = 100;
				}
				it.士気 += 50;
				it.攻撃補正 = 2;
				it.防御補正 = 0;
				it.士気変化 = -0.05;
				it.移動速度 = it.機動力  * 0.5;
				it.向き = 軍団->前進方向;
				it.フラッシュ時間 = 50;
				it.is逃走中 = false;
			}
		};
	};
	class Tｽﾗｲﾑﾌｨｰﾊﾞｰ : public ITactics
	{
	private:
		void 発動効果(ISceneBattele* 戦場, LegionData* 軍団) override
		{
			//todo
			for (auto& it : 軍団->兵士)
			{
				if (it.戦闘状態 == UnitStateType::退却 || it.戦闘状態 == UnitStateType::突破) { continue; }
				it.蓄積ダメージ = 0;
				it.部隊数 = 100;
				it.士気 = 100;
				it.攻撃力 += 3;
				it.戦闘状態 = UnitStateType::戦闘中;//死んでたら生き返る
				it.攻撃補正 = 2;
				it.防御補正 = 0;
				it.士気変化 = -0.05;
				it.移動速度 = it.機動力  * 0.5;
				it.向き = 軍団->前進方向;
				it.フラッシュ時間 = 50;
				it.is逃走中 = false;
			}
		};
	};
	class T大熱波 : public ITactics
	{
	private:
		void 発動効果(ISceneBattele* 戦場, LegionData* 軍団) override
		{
			//todo
			for (auto& it : 軍団->兵士)
			{
				it.攻撃補正 = 0;
				it.防御補正 = 0;
				it.士気変化 = 0.12;
				it.移動速度 = 0;
				it.向き = 軍団->前進方向;
				it.フラッシュ時間 = 50;
				it.is逃走中 = false;
			}
		};
	};
	class T大あまごい : public ITactics
	{
	private:
		void 発動効果(ISceneBattele* 戦場, LegionData* 軍団) override
		{
			//todo
			
		};
	};
	class T超高速デルタ : public ITactics
	{
	private:
		void 発動効果(ISceneBattele* 戦場, LegionData* 軍団) override
		{
			//todo
			for (auto& it : 軍団->兵士)
			{
				it.攻撃補正 = 5;
				it.防御補正 = 0;
				it.士気変化 = -0.05;
				it.移動速度 = it.機動力  * 1.0;
				it.向き = 軍団->前進方向;
				it.フラッシュ時間 = 50;
				it.is逃走中 = false;
			}
		};
	};
	class T大地震 : public ITactics
	{
	private:
		void 発動効果(ISceneBattele* 戦場, LegionData* 軍団) override
		{
			//todo
			
		};
	};
	class Tナラクの風 : public ITactics
	{
	private:
		void 発動効果(ISceneBattele* 戦場, LegionData* 軍団) override
		{	
			//戦術名を出す
			戦場->Draw(50);
			//ぴょんぴょんはねる&戦術効果
			for (auto& it : 軍団->兵士)
			{
				it.攻撃補正 = 5;
				it.防御補正 = 0;
				it.士気変化 = -0.10;
				it.士気 = CV::最大士気;
				it.移動速度 = it.機動力  * 0.75;
				it.向き = 軍団->前進方向;
				it.突撃時間 = 500;
				it.is逃走中 = false;
				it.ジャンプ回数 = 3;
				it.ジャンプ待ち = Rand::Get(10);
			}
			戦場->Draw(80);
			//!マーク出す
			for (auto& it : 軍団->兵士)
			{
				it.感情 = EmoteType::驚き;
				it.感情時間 = 50;
			}
			戦場->Draw(80);
			//相手に焦りマーク
			for (auto& it : 軍団->敵軍->兵士)
			{
				it.感情 = EmoteType::焦り;
				it.感情時間 = 50;
				it.士気 = 0;
			}
			戦場->Draw(80);

			//フラッシュ
			for (auto& it : 軍団->兵士)
			{
				it.フラッシュ時間 = 50;
			}
		};
	};

	EnumArray<ITactics*, TacticsType> 戦術データ;

	void LoadTacticsS()
	{
		//EnumTypeで型を参照できるようにしておく
		戦術データ[TacticsType::空き] = &Singleton<T空き>::Get();
		戦術データ[TacticsType::全軍前進] = &Singleton<T全軍前進>::Get();
		戦術データ[TacticsType::全軍後退] = &Singleton<T全軍後退>::Get();
		戦術データ[TacticsType::全軍防御] = &Singleton<T全軍防御>::Get();
		戦術データ[TacticsType::全軍突撃] = &Singleton<T全軍突撃>::Get();
		戦術データ[TacticsType::全軍退却] = &Singleton<T全軍退却>::Get();
		戦術データ[TacticsType::前列前進] = &Singleton<T前列前進>::Get();
		戦術データ[TacticsType::後列前進] = &Singleton<T後列前進>::Get();
		戦術データ[TacticsType::速攻前進] = &Singleton<T速攻前進>::Get();
		戦術データ[TacticsType::強撃前進] = &Singleton<T強撃前進>::Get();
		戦術データ[TacticsType::後退攻撃] = &Singleton<T後退攻撃>::Get();
		戦術データ[TacticsType::上翼攻撃] = &Singleton<T上翼攻撃>::Get();
		戦術データ[TacticsType::下翼攻撃] = &Singleton<T下翼攻撃>::Get();
		戦術データ[TacticsType::前列突撃] = &Singleton<T前列突撃>::Get();
		戦術データ[TacticsType::後列突撃] = &Singleton<T後列突撃>::Get();
		戦術データ[TacticsType::上翼突撃] = &Singleton<T上翼突撃>::Get();
		戦術データ[TacticsType::下翼突撃] = &Singleton<T下翼突撃>::Get();
		戦術データ[TacticsType::前列防御] = &Singleton<T前列防御>::Get();
		戦術データ[TacticsType::全軍防御･強] = &Singleton<T全軍防御･強>::Get();
		戦術データ[TacticsType::後退防御] = &Singleton<T後退防御>::Get();
		戦術データ[TacticsType::速攻後退] = &Singleton<T速攻後退>::Get();
		戦術データ[TacticsType::前列交代] = &Singleton<T前列交代>::Get();
		戦術データ[TacticsType::速攻交代] = &Singleton<T速攻交代>::Get();
		戦術データ[TacticsType::統率回復･弱] = &Singleton<T統率回復･弱>::Get();
		戦術データ[TacticsType::陣形回復] = &Singleton<T陣形回復>::Get();
		戦術データ[TacticsType::前列死守] = &Singleton<T前列死守>::Get();
		戦術データ[TacticsType::統率回復･強] = &Singleton<T統率回復･強>::Get();
		戦術データ[TacticsType::陣形回復･速] = &Singleton<T陣形回復･速>::Get();
		戦術データ[TacticsType::死んだフリ] = &Singleton<T死んだフリ>::Get();
		戦術データ[TacticsType::雨ごい] = &Singleton<T雨ごい>::Get();
		戦術データ[TacticsType::冬将軍] = &Singleton<T冬将軍>::Get();
		戦術データ[TacticsType::分断作戦] = &Singleton<T分断作戦>::Get();
		戦術データ[TacticsType::投石部隊] = &Singleton<T投石部隊>::Get();
		戦術データ[TacticsType::指揮官突撃] = &Singleton<T指揮官突撃>::Get();
		戦術データ[TacticsType::爆裂部隊] = &Singleton<T爆裂部隊>::Get();
		戦術データ[TacticsType::火攻め] = &Singleton<T火攻め>::Get();
		戦術データ[TacticsType::おとり作戦] = &Singleton<Tおとり作戦>::Get();
		戦術データ[TacticsType::情報操作L1] = &Singleton<T情報操作L1>::Get();
		戦術データ[TacticsType::情報操作L2] = &Singleton<T情報操作L2>::Get();
		戦術データ[TacticsType::情報操作L3] = &Singleton<T情報操作L3>::Get();
		戦術データ[TacticsType::情報操作L4] = &Singleton<T情報操作L4>::Get();
		戦術データ[TacticsType::情報操作L5] = &Singleton<T情報操作L5>::Get();
		戦術データ[TacticsType::穴掘り作戦] = &Singleton<T穴掘り作戦>::Get();
		戦術データ[TacticsType::防柵作戦] = &Singleton<T防柵作戦>::Get();
		戦術データ[TacticsType::水際作戦] = &Singleton<T水際作戦>::Get();
		戦術データ[TacticsType::背火の計] = &Singleton<T背火の計>::Get();
		戦術データ[TacticsType::速攻退却] = &Singleton<T速攻退却>::Get();
		戦術データ[TacticsType::全軍前進･強] = &Singleton<T全軍前進･強>::Get();
		戦術データ[TacticsType::全軍休息] = &Singleton<T全軍休息>::Get();
		戦術データ[TacticsType::戦場の風] = &Singleton<T戦場の風>::Get();
		戦術データ[TacticsType::後列後退] = &Singleton<T後列後退>::Get();
		戦術データ[TacticsType::ｽﾗｲﾑﾌｨｰﾊﾞｰ] = &Singleton<Tｽﾗｲﾑﾌｨｰﾊﾞｰ>::Get();
		戦術データ[TacticsType::信じる者達] = &Singleton<T信じる者達>::Get();
		戦術データ[TacticsType::生命回復] = &Singleton<T生命回復>::Get();
		戦術データ[TacticsType::ナラクの風] = &Singleton<Tナラクの風>::Get();
		戦術データ[TacticsType::大熱波] = &Singleton<T大熱波>::Get();
		戦術データ[TacticsType::大あまごい] = &Singleton<T大あまごい>::Get();
		戦術データ[TacticsType::超高速デルタ] = &Singleton<T超高速デルタ>::Get();
		戦術データ[TacticsType::大地震] = &Singleton<T大地震>::Get();
		//戦術種代入
		for (int a = 0; a < (int)TacticsType::COUNT; a++)
		{
			戦術データ[TacticsType(a)]->戦術種 = TacticsType(a);
		}

		戦術データ[TacticsType::空き]->Set説明("――――","");//"――――"表示
		戦術データ[TacticsType::全軍前進]->Set説明("全軍前進","");
		戦術データ[TacticsType::全軍後退]->Set説明("全軍後退","");
		戦術データ[TacticsType::全軍防御]->Set説明("全軍防御","");
		戦術データ[TacticsType::全軍突撃]->Set説明("全軍突撃","");
		戦術データ[TacticsType::全軍退却]->Set説明("全軍退却","");
		戦術データ[TacticsType::前列前進]->Set説明("前列前進","");
		戦術データ[TacticsType::後列前進]->Set説明("後列前進","");
		戦術データ[TacticsType::速攻前進]->Set説明("速攻前進","");
		戦術データ[TacticsType::強撃前進]->Set説明("強撃前進","");
		戦術データ[TacticsType::上翼攻撃]->Set説明("上翼攻撃","");
		戦術データ[TacticsType::下翼攻撃]->Set説明("下翼攻撃","");
		戦術データ[TacticsType::全軍前進･強]->Set説明("全軍前進･強","");
		戦術データ[TacticsType::全軍休息]->Set説明("全軍休息","");
		戦術データ[TacticsType::速攻後退]->Set説明("速攻後退","");
		戦術データ[TacticsType::後退攻撃]->Set説明("後退攻撃","");
		戦術データ[TacticsType::後退防御]->Set説明("後退防御","");
		戦術データ[TacticsType::前列防御]->Set説明("前列防御","");
		戦術データ[TacticsType::後列後退]->Set説明("後列後退","");
		戦術データ[TacticsType::前列死守]->Set説明("前列死守","");
		戦術データ[TacticsType::全軍防御･強]->Set説明("全軍防御･強","");
		戦術データ[TacticsType::前列突撃]->Set説明("前列突撃","");
		戦術データ[TacticsType::後列突撃]->Set説明("後列突撃","");
		戦術データ[TacticsType::上翼突撃]->Set説明("上翼突撃","");
		戦術データ[TacticsType::下翼突撃]->Set説明("下翼突撃","");
		戦術データ[TacticsType::指揮官突撃]->Set説明("指揮官突撃","");
		戦術データ[TacticsType::前列交代]->Set説明("前列交代","");
		戦術データ[TacticsType::速攻交代]->Set説明("速攻交代","");
		戦術データ[TacticsType::統率回復･弱]->Set説明("統率回復･弱","");
		戦術データ[TacticsType::統率回復･強]->Set説明("統率回復･強","");
		戦術データ[TacticsType::陣形回復]->Set説明("陣形回復","");
		戦術データ[TacticsType::陣形回復･速]->Set説明("陣形回復･速","");
		戦術データ[TacticsType::速攻退却]->Set説明("速攻退却","");
		戦術データ[TacticsType::死んだフリ]->Set説明("死んだフリ","");
		戦術データ[TacticsType::雨ごい]->Set説明("雨ごい","");
		戦術データ[TacticsType::冬将軍]->Set説明("冬将軍","");
		戦術データ[TacticsType::分断作戦]->Set説明("分断作戦","");
		戦術データ[TacticsType::爆裂部隊]->Set説明("爆裂部隊","");
		戦術データ[TacticsType::火攻め]->Set説明("火攻め","");
		戦術データ[TacticsType::おとり作戦]->Set説明("おとり作戦","");
		戦術データ[TacticsType::穴掘り作戦]->Set説明("穴掘り作戦","");
		戦術データ[TacticsType::防柵作戦]->Set説明("防柵作戦","");
		戦術データ[TacticsType::水際作戦]->Set説明("水際作戦","");
		戦術データ[TacticsType::背火の計]->Set説明("背火の計","");
		戦術データ[TacticsType::戦場の風]->Set説明("戦場の風","");
		戦術データ[TacticsType::情報操作L1]->Set説明("情報操作L1","");
		戦術データ[TacticsType::情報操作L2]->Set説明("情報操作L2","");
		戦術データ[TacticsType::情報操作L3]->Set説明("情報操作L3","");
		戦術データ[TacticsType::情報操作L4]->Set説明("情報操作L4","");
		戦術データ[TacticsType::情報操作L5]->Set説明("情報操作L5","");
		戦術データ[TacticsType::投石部隊]->Set説明("投石部隊","");
		戦術データ[TacticsType::信じる者達]->Set説明("信じる者達","");
		戦術データ[TacticsType::生命回復]->Set説明("生命回復","");
		戦術データ[TacticsType::ｽﾗｲﾑﾌｨｰﾊﾞｰ]->Set説明("ｽﾗｲﾑﾌｨｰﾊﾞｰ","");
		戦術データ[TacticsType::ナラクの風]->Set説明("ナラクの風","");
		戦術データ[TacticsType::大熱波]->Set説明("大熱波","");
		戦術データ[TacticsType::大あまごい]->Set説明("大あまごい","");
		戦術データ[TacticsType::超高速デルタ]->Set説明("超高速デルタ","");
		戦術データ[TacticsType::大地震]->Set説明("大地震","");

		戦術データ[TacticsType::空き]->Set性能(CV::戦術回数無限, true, true, false);
		戦術データ[TacticsType::全軍前進]->Set性能(CV::戦術回数無限, true, true, true);
		戦術データ[TacticsType::全軍後退]->Set性能(CV::戦術回数無限, true, true, true);
		戦術データ[TacticsType::全軍防御]->Set性能(CV::戦術回数無限, true, true, true);
		戦術データ[TacticsType::全軍突撃]->Set性能(CV::戦術回数無限, true, true, true);
		戦術データ[TacticsType::全軍退却]->Set性能(CV::戦術回数無限, true, true, true);
		戦術データ[TacticsType::前列前進]->Set性能(CV::戦術回数無限,false, true, true);
		戦術データ[TacticsType::後列前進]->Set性能(CV::戦術回数無限,false, true, true);
		戦術データ[TacticsType::速攻前進]->Set性能(4, true, true, true);
		戦術データ[TacticsType::強撃前進]->Set性能(4, true, true, true);
		戦術データ[TacticsType::上翼攻撃]->Set性能(4, true, true, true);
		戦術データ[TacticsType::下翼攻撃]->Set性能(4, true, true, true);
		戦術データ[TacticsType::全軍前進･強]->Set性能(2, true, true, true);
		戦術データ[TacticsType::全軍休息]->Set性能(4, true, true, true);
		戦術データ[TacticsType::速攻後退]->Set性能(4, true, true, true);
		戦術データ[TacticsType::後退攻撃]->Set性能(3, true, true, true);
		戦術データ[TacticsType::後退防御]->Set性能(3, true, true, true);
		戦術データ[TacticsType::前列防御]->Set性能(CV::戦術回数無限,false, true, true);
		戦術データ[TacticsType::後列後退]->Set性能(CV::戦術回数無限,false, true, true);
		戦術データ[TacticsType::前列死守]->Set性能(2,false, true, true);
		戦術データ[TacticsType::全軍防御･強]->Set性能(2, true, true, true);
		戦術データ[TacticsType::前列突撃]->Set性能(1,false, true, true);
		戦術データ[TacticsType::後列突撃]->Set性能(1,false, true, true);
		戦術データ[TacticsType::上翼突撃]->Set性能(1, true, true, true);
		戦術データ[TacticsType::下翼突撃]->Set性能(1, true, true, true);
		戦術データ[TacticsType::指揮官突撃]->Set性能(1, true, true, true);
		戦術データ[TacticsType::前列交代]->Set性能(CV::戦術回数無限, false, true, true);
		戦術データ[TacticsType::速攻交代]->Set性能(3, false, true, true);
		戦術データ[TacticsType::統率回復･弱]->Set性能(3, true, true, false);
		戦術データ[TacticsType::統率回復･強]->Set性能(1, true, true, false);
		戦術データ[TacticsType::陣形回復]->Set性能(3, true, true, true);
		戦術データ[TacticsType::陣形回復･速]->Set性能(2, true, true, true);
		戦術データ[TacticsType::速攻退却]->Set性能(4, true, true, true);
		戦術データ[TacticsType::死んだフリ]->Set性能(1, true, true, true);
		戦術データ[TacticsType::雨ごい]->Set性能(1, true, true, false);
		戦術データ[TacticsType::冬将軍]->Set性能(1, true, true, false);
		戦術データ[TacticsType::分断作戦]->Set性能(1, true, true, true);
		戦術データ[TacticsType::爆裂部隊]->Set性能(1, true, true, true);
		戦術データ[TacticsType::火攻め]->Set性能(1, true, true, true);
		戦術データ[TacticsType::おとり作戦]->Set性能(1, true, true, true);
		戦術データ[TacticsType::穴掘り作戦]->Set性能(1, true, true, true);
		戦術データ[TacticsType::防柵作戦]->Set性能(1, true, true, true);
		戦術データ[TacticsType::水際作戦]->Set性能(1, true, true, true);
		戦術データ[TacticsType::背火の計]->Set性能(1, true, true, true);
		戦術データ[TacticsType::戦場の風]->Set性能(1, true, true, true);
		戦術データ[TacticsType::情報操作L1]->Set性能(1, true, true, false);
		戦術データ[TacticsType::情報操作L2]->Set性能(1, true, true, false);
		戦術データ[TacticsType::情報操作L3]->Set性能(1, true, true, false);
		戦術データ[TacticsType::情報操作L4]->Set性能(1, true, true, false);
		戦術データ[TacticsType::情報操作L5]->Set性能(1, true, true, false);
		戦術データ[TacticsType::投石部隊]->Set性能(CV::戦術回数無限, true, true, false);
		戦術データ[TacticsType::信じる者達]->Set性能(4, true, true, true);
		戦術データ[TacticsType::生命回復]->Set性能(1, true, true, true);
		戦術データ[TacticsType::ｽﾗｲﾑﾌｨｰﾊﾞｰ]->Set性能(1, true, true, true);
		戦術データ[TacticsType::ナラクの風]->Set性能(1, true, true, true);
		戦術データ[TacticsType::大熱波]->Set性能(2, true, true, true);
		戦術データ[TacticsType::大あまごい]->Set性能(2, true, true, false);
		戦術データ[TacticsType::超高速デルタ]->Set性能(2, true, true, true);
		戦術データ[TacticsType::大地震]->Set性能(2, true, true, false);

	}
}