//Copyright © 2017 (´･@･)
//[License]GNU Affero General Public License, version 3
//[Contact]http://tacoika.blog87.fc2.com/
#pragma once
#include "../System/EnumType.h"

namespace SDX_RMC
{
	using namespace SDX;

	class IFormatin
	{
	public:
		FormationType 陣形種;
		std::string 陣形名;
		std::string 説明文;
		bool is複列;

		void 整列(LegionData& 軍団, bool 右軍)
		{
			//現在の状態を代入
			for (int a = 0; a < (int)軍団.兵士.size(); a++)
			{
				軍団.兵士[a].開始地点 = 軍団.兵士[a].位置;
			}
			//整列処理
			整列処理(軍団);

			//後処理
			if (右軍 == true)
			{
				//線対象位置に座標を計算
				for (int a = 0; a < (int)軍団.兵士.size(); a++)
				{
					軍団.兵士[a].着地地点.x = 640 - 軍団.兵士[a].着地地点.x;
				}
			}

			//陣形回復用位置計算
			double 平均X = 0;
			for (auto&it : 軍団.兵士)
			{
				平均X += it.着地地点.x;
			}
			平均X /= 軍団.兵士.size();
			for (auto&it : 軍団.兵士)
			{
				it.陣形位置差分 = 平均X - it.着地地点.x;
			}

			return;
		}

		void Set性能(bool is複列)
		{
			this->is複列 = is複列;
		}

		void Set説明(const char*陣形名,const char*説明文)
		{
			this->陣形名 = 陣形名;
			this->説明文 = 説明文;
		}

	private:
		virtual void 整列処理(LegionData& 軍団) = 0;
		//メニュー画面用の陣形表示関数
		//ステータス補正計算関数
	};

	//ちゃんとsingletonにしてない警察注意!
	class F縦列陣形 :public IFormatin
	{
	private:

		void 整列処理(LegionData& 軍団) override
		{
			const int max = 軍団.兵士.size() - 1;

			//指揮官
			軍団.兵士[max].現在列 = 軍団.兵士[max].初期列 = RowType::前列;
			軍団.兵士[max].着地地点 = { 80 , 150 };
			//単列
			for (int a = 0; a < max; a++)
			{
				double div = (double)a / max;
				軍団.兵士[a].現在列 = 軍団.兵士[a].初期列 = RowType::前列;
				軍団.兵士[a].着地地点 = { 90 + a % 2 * 10 , div * 300 };
			}
		}
	};
	class F防壁の陣 :public IFormatin
	{
	private:

		void 整列処理(LegionData& 軍団) override
		{
			const int 前列数 = (軍団.兵士.size() - 1) / 2;
			const int 後列数 = (軍団.兵士.size() - 1) - 前列数;
			const int max = 軍団.兵士.size() - 1;

			//指揮官
			軍団.兵士[max].現在列 = 軍団.兵士[max].初期列 = RowType::後列;
			軍団.兵士[max].着地地点 = { 60 , 80 };
			//前列
			for (int a = 0; a < 前列数; a++)
			{
				double div = (double)a / 前列数;

				軍団.兵士[a].現在列 = 軍団.兵士[a].初期列 = RowType::前列;
				軍団.兵士[a].着地地点 = { 140 + abs(50 - 100 * div) , div * 300 };
			}
			//後列
			for (int a = 前列数; a < 前列数 + 後列数; a++)
			{
				double div = (double)(a - 前列数) / 後列数;

				軍団.兵士[a].現在列 = 軍団.兵士[a].初期列 = RowType::後列;
				軍団.兵士[a].着地地点 = { 40 + abs(50 - 100 * div) , div * 300 };
			}
		}
	};
	class F攻戦の陣 :public IFormatin
	{
	private:
		void 整列処理(LegionData& 軍団) override
		{
			const int 前列数 = (軍団.兵士.size() - 1) / 2;
			const int 後列数 = (軍団.兵士.size() - 1) - 前列数;
			const int max = 軍団.兵士.size() - 1;

			//指揮官
			軍団.兵士[max].現在列 = 軍団.兵士[max].初期列 = RowType::後列;
			軍団.兵士[max].着地地点 = { 60 , 80 };

			//後列
			for (int a = 0; a < 前列数; a++)
			{
				double div = (double)a / 前列数;

				軍団.兵士[a].現在列 = 軍団.兵士[a].初期列 = RowType::後列;
				軍団.兵士[a].着地地点 = { 90 - abs(50 - 100 * div) , div * 300 };
			}
			//前列
			for (int a = 前列数; a < 前列数 + 後列数; a++)
			{
				double div = (double)(a - 前列数) / 後列数;

				軍団.兵士[a].現在列 = 軍団.兵士[a].初期列 = RowType::前列;
				軍団.兵士[a].着地地点 = { 190 - abs(50 - 100 * div) , div * 300 };
			}
		}
	};
	class F上翼突破の陣 :public IFormatin
	{
	private:
		void 整列処理(LegionData& 軍団) override
		{
			const int 前列数 = (軍団.兵士.size() - 1) * 3 / 4;
			const int 後列数 = (軍団.兵士.size() - 1) - 前列数;
			const int max = 軍団.兵士.size() - 1;

			//指揮官
			軍団.兵士[max].現在列 = 軍団.兵士[max].初期列 = RowType::後列;
			軍団.兵士[max].着地地点 = { 130 , 50 };

			for (int a = 0; a < 前列数; a++)
			{
				double div = (double)a / 前列数;

				軍団.兵士[a].現在列 = 軍団.兵士[a].初期列 = RowType::前列;
				軍団.兵士[a].着地地点 = { 120 + abs(50 - 100 * div) , 300 - div * 300 };
			}

			for (int a = 前列数; a < 前列数 + 後列数; a++)
			{
				double div = (double)(a - 前列数) / 後列数;

				int tmp = int(300 * div);
				軍団.兵士[a].現在列 = 軍団.兵士[a].初期列 = RowType::後列;
				軍団.兵士[a].着地地点 = { 110 - (tmp / 100) * 30 , 60 - tmp % 60 };
			}

		}
	};
	class F下翼突破の陣 :public IFormatin
	{
	private:
		void 整列処理(LegionData& 軍団) override
		{
			const int 前列数 = (軍団.兵士.size() - 1) * 3 / 4;
			const int 後列数 = (軍団.兵士.size() - 1) - 前列数;
			const int max = 軍団.兵士.size() - 1;

			//指揮官
			軍団.兵士[max].現在列 = 軍団.兵士[max].初期列 = RowType::後列;
			軍団.兵士[max].着地地点 = { 130 , 250 };

			for (int a = 0; a < 前列数; a++)
			{
				double div = (double)a / 前列数;

				軍団.兵士[a].現在列 = 軍団.兵士[a].初期列 = RowType::前列;
				軍団.兵士[a].着地地点 = { 120 + abs(50 - 100 * div) , div * 300 };
			}

			for (int a = 前列数; a < 前列数 + 後列数; a++)
			{
				double div = (double)(a - 前列数) / 後列数;

				int tmp = int(300 * div);
				軍団.兵士[a].現在列 = 軍団.兵士[a].初期列 = RowType::後列;
				軍団.兵士[a].着地地点 = { 110 - (tmp / 100) * 30 , 240 + tmp % 60 };
			}

		}
	};
	class F中央突破の陣 :public IFormatin
	{
	private:
		void 整列処理(LegionData& 軍団) override
		{
			const int 前列数 = (軍団.兵士.size() - 1) * 3 / 4;
			const int 後列数 = (軍団.兵士.size() - 1) - 前列数;
			const int max = 軍団.兵士.size() - 1;

			//指揮官
			軍団.兵士[max].現在列 = 軍団.兵士[max].初期列 = RowType::後列;
			軍団.兵士[max].着地地点 = { 60 , 150 };

			for (int a = 0; a < 前列数; a++)
			{
				double div = (double)a / 前列数;

				軍団.兵士[a].現在列 = 軍団.兵士[a].初期列 = RowType::後列;
				軍団.兵士[a].着地地点 = { 40 + abs(50 - 100 * div) , div * 300 };
			}

			for (int a = 前列数; a < 前列数 + 後列数; a++)
			{
				double div = (double)(a - 前列数) / 後列数;

				int tmp = int(100 * div);
				軍団.兵士[a].現在列 = 軍団.兵士[a].初期列 = RowType::前列;
				軍団.兵士[a].着地地点 = { 130 - abs(50 - 100 * div) , 100 + tmp };
			}

		}
	};
	class F三段構えの陣 :public IFormatin
	{
	private:
		void 整列処理(LegionData& 軍団) override
		{
			const int 前列数 = (軍団.兵士.size() - 1) * 1 / 3;
			const int 後列数 = (軍団.兵士.size() - 1) - 前列数;
			const int max = 軍団.兵士.size() - 1;

			//指揮官
			軍団.兵士[max].現在列 = 軍団.兵士[max].初期列 = RowType::後列;
			軍団.兵士[max].着地地点 = { 40 , 30 };

			for (int a = 0; a < 前列数; a++)
			{
				double div = (double)a / 前列数;

				軍団.兵士[a].現在列 = 軍団.兵士[a].初期列 = RowType::前列;
				軍団.兵士[a].着地地点 = { 120 , div * 300 };
			}

			for (int a = 前列数; a < 前列数 + 後列数; a++)
			{
				double div = (double)(a - 前列数) / 後列数;

				int tmp = int(2 * div);
				軍団.兵士[a].現在列 = 軍団.兵士[a].初期列 = RowType::後列;
				軍団.兵士[a].着地地点 = { 80 - tmp * 40 , div * 600 - tmp * 300 };
			}

		}
	};
	class F疾風陣 :public IFormatin
	{
	private:
		void 整列処理(LegionData& 軍団) override
		{
			const int 前列数 = (軍団.兵士.size() - 1) / 2;
			const int 後列数 = (軍団.兵士.size() - 1) - 前列数;
			const int max = 軍団.兵士.size() - 1;

			//指揮官
			軍団.兵士[max].現在列 = 軍団.兵士[max].初期列 = RowType::前列;
			軍団.兵士[max].着地地点 = { 90 , 50 };

			for (int a = 0; a < 前列数; a++)
			{
				double div = (double)a / 前列数;

				軍団.兵士[a].現在列 = 軍団.兵士[a].初期列 = RowType::前列;
				軍団.兵士[a].着地地点 = { 190 - abs(150 - 300 * div) , div * 300 };

			}

			for (int a = 前列数; a < 前列数 + 後列数; a++)
			{
				double div = (double)(a + 0.5 - 前列数) / 後列数;

				軍団.兵士[a].現在列 = 軍団.兵士[a].初期列 = RowType::前列;
				軍団.兵士[a].着地地点 = { 190 - abs(150 - 300 * div) , div * 300 };
			}

		}
	};
	class F背水の陣 :public IFormatin
	{
	private:
		void 整列処理(LegionData& 軍団) override
		{
			const int 前列数 = (軍団.兵士.size() - 1) / 2;
			const int 後列数 = (軍団.兵士.size() - 1) - 前列数;
			const int max = 軍団.兵士.size() - 1;

			//指揮官
			軍団.兵士[max].現在列 = 軍団.兵士[max].初期列 = RowType::前列;
			軍団.兵士[max].着地地点 = { 60 , 60 + 80 };

			for (int a = 0; a < 前列数; a++)
			{
				double div = (double)a / 前列数;

				軍団.兵士[a].現在列 = 軍団.兵士[a].初期列 = RowType::前列;
				軍団.兵士[a].着地地点 = { 40 , div * 300 };
			}

			for (int a = 前列数; a < 前列数 + 後列数; a++)
			{
				double div = (double)(a - 前列数) / 後列数;

				軍団.兵士[a].現在列 = 軍団.兵士[a].初期列 = RowType::前列;
				軍団.兵士[a].着地地点 = { 70 + (20 * (a % 3)) , div * 260 };
				if (div > 0.5) { 軍団.兵士[a].着地地点.y += 40; }

			}

		}
	};
	class F強撃陣 :public IFormatin
	{
	private:
		void 整列処理(LegionData& 軍団) override
		{
			const int 前列数 = (軍団.兵士.size() - 1) / 2;
			const int 後列数 = (軍団.兵士.size() - 1) - 前列数;
			const int max = 軍団.兵士.size() - 1;

			//指揮官
			軍団.兵士[max].現在列 = 軍団.兵士[max].初期列 = RowType::前列;
			軍団.兵士[max].着地地点 = { 40 , 150 };
			for (int a = 0; a < 前列数; a++)
			{
				double div = (double)a / 前列数;

				軍団.兵士[a].現在列 = 軍団.兵士[a].初期列 = RowType::前列;
				軍団.兵士[a].着地地点 = { 65 , div * 300 };

			}

			for (int a = 前列数; a < 前列数 + 後列数; a++)
			{
				double div = (double)(a - 前列数) / 後列数;

				軍団.兵士[a].現在列 = 軍団.兵士[a].初期列 = RowType::前列;
				軍団.兵士[a].着地地点 = { 75 , div * 300 };
			}
		}
	};
	class F盾の陣 :public IFormatin
	{
	private:
		void 整列処理(LegionData& 軍団) override
		{
			const int 前列数 = (軍団.兵士.size() - 1) / 2;
			const int 後列数 = (軍団.兵士.size() - 1) - 前列数;
			const int max = 軍団.兵士.size() - 1;

			//指揮官
			軍団.兵士[max].現在列 = 軍団.兵士[max].初期列 = RowType::後列;
			軍団.兵士[max].着地地点 = { 40 , 150 };
			//前列
			for (int a = 0; a < 前列数; a++)
			{
				double div = (double)a / 前列数;

				軍団.兵士[a].現在列 = 軍団.兵士[a].初期列 = RowType::前列;
				軍団.兵士[a].着地地点 = { 140 + abs(50 - 100 * div) , div * 300 };
			}
			//後列
			for (int a = 前列数; a < 前列数 + 後列数; a++)
			{
				double div = (double)(a - 前列数) / 後列数;

				軍団.兵士[a].現在列 = 軍団.兵士[a].初期列 = RowType::後列;
				if (div < 0.66)
				{
					div = div * 3 / 2;
					軍団.兵士[a].着地地点 = { 80 + abs(50 - 100 * div) , div * 280 + 10 };
				}
				else {
					div = (div - 0.66) * 3;
					軍団.兵士[a].着地地点 = { 65 - abs(25 - 50 * div) , 120 + div * 60 };
				}


			}

		}
	};
	class F虎穴陣 :public IFormatin
	{
	private:
		void 整列処理(LegionData& 軍団) override
		{
			const int max = 軍団.兵士.size() - 1;

			//指揮官
			軍団.兵士[max].現在列 = 軍団.兵士[max].初期列 = RowType::前列;
			軍団.兵士[max].着地地点 = { 40 , 150 };

			for (int a = 0; a < max; a++)
			{
				double div = (double)a / max;
				軍団.兵士[a].現在列 = 軍団.兵士[a].初期列 = RowType::前列;
				軍団.兵士[a].着地地点 = { 40 + abs(150 - div * 300) , div * 300 };
			}

		}
	};
	class F修羅の陣 :public IFormatin
	{
	private:
		void 整列処理(LegionData& 軍団) override
		{
			const int 前列数 = (軍団.兵士.size() - 1) / 2;
			const int 後列数 = (軍団.兵士.size() - 1) - 前列数;
			const int max = 軍団.兵士.size() - 1;

			//指揮官
			軍団.兵士[max].現在列 = 軍団.兵士[max].初期列 = RowType::前列;
			軍団.兵士[max].着地地点 = { 40 , 150 };

			for (int a = 0; a < 前列数; a++)
			{
				double div = (double)a / 前列数;

				軍団.兵士[a].現在列 = 軍団.兵士[a].初期列 = RowType::前列;
				軍団.兵士[a].着地地点 = { 40 + (a % 3) * 40 , div * 120 };
			}

			for (int a = 前列数; a < 前列数 + 後列数; a++)
			{
				double div = (double)(a - 前列数) / 前列数;

				軍団.兵士[a].現在列 = 軍団.兵士[a].初期列 = RowType::前列;
				軍団.兵士[a].着地地点 = { 40 + (a % 3) * 40 , 180 + div * 120 };
			}

		}
	};
	class F龍翼の陣 :public IFormatin
	{
	private:
		void 整列処理(LegionData& 軍団) override
		{
			const int 前列数 = (軍団.兵士.size() - 1) / 2;
			const int 後列数 = (軍団.兵士.size() - 1) - 前列数;
			const int max = 軍団.兵士.size() - 1;

			//指揮官
			軍団.兵士[max].現在列 = 軍団.兵士[max].初期列 = RowType::前列;
			軍団.兵士[max].着地地点 = { 40 , 150 };

			for (int a = 0; a < 前列数; a++)
			{
				double div = (double)a / 前列数;

				軍団.兵士[a].現在列 = 軍団.兵士[a].初期列 = RowType::前列;
				軍団.兵士[a].着地地点 = { 190 - abs(150 - 300 * div) , div * 130 };

			}

			for (int a = 前列数; a < 前列数 + 後列数; a++)
			{
				double div = (double)(a + 0.5 - 前列数) / 後列数;

				軍団.兵士[a].現在列 = 軍団.兵士[a].初期列 = RowType::前列;
				軍団.兵士[a].着地地点 = { 190 - abs(150 - 300 * div) , div * 130 + 170 };
			}

		}
	};
	class F戦神の陣 :public IFormatin
	{
	private:

		void 整列処理(LegionData& 軍団) override
		{
			const int 前列数 = (軍団.兵士.size() - 1) / 2;
			const int 後列数 = (軍団.兵士.size() - 1) - 前列数;
			const int max = 軍団.兵士.size() - 1;

			//指揮官
			軍団.兵士[max].現在列 = 軍団.兵士[max].初期列 = RowType::前列;
			軍団.兵士[max].着地地点 = { 190 , 150 };

			for (int a = 0; a < 前列数; a++)
			{
				double div = (double)a / 前列数;

				軍団.兵士[a].現在列 = 軍団.兵士[a].初期列 = RowType::前列;
				軍団.兵士[a].着地地点 = { 40 + abs(150 - 300 * div) , div * 130 };

			}

			for (int a = 前列数; a < 前列数 + 後列数; a++)
			{
				double div = (double)(a + 0.5 - 前列数) / 後列数;

				軍団.兵士[a].現在列 = 軍団.兵士[a].初期列 = RowType::前列;
				軍団.兵士[a].着地地点 = { 40 + abs(150 - 300 * div) , div * 130 + 170 };
			}

		}
	};
	class F海王の陣 :public IFormatin
	{
	private:
		void 整列処理(LegionData& 軍団) override
		{
			const int 前列数 = (軍団.兵士.size() - 1) / 2;
			const int 後列数 = (軍団.兵士.size() - 1) - 前列数;
			const int max = 軍団.兵士.size() - 1;

			//指揮官
			軍団.兵士[max].現在列 = 軍団.兵士[max].初期列 = RowType::前列;
			軍団.兵士[max].着地地点 = { 190 , 150 };

			for (int a = 0; a < 前列数; a++)
			{
				double div = (double)a / 前列数;

				軍団.兵士[a].現在列 = 軍団.兵士[a].初期列 = RowType::後列;
				軍団.兵士[a].着地地点 = { 40 + abs(150 - 300 * div) , div * 300 };
			}

			for (int a = 前列数; a < 前列数 + 後列数; a++)
			{
				double div = (double)(a - 前列数) / 後列数;

				軍団.兵士[a].現在列 = 軍団.兵士[a].初期列 = RowType::前列;
				if (div < 0.66) {
					div = div * 3 / 2;
					軍団.兵士[a].着地地点 = { 75 + abs(115 - 220 * div) , 40 + div * 220 };
				}
				else {
					div = (div - 0.66) * 3;
					軍団.兵士[a].着地地点 = { 132.5 + abs(57.5 - 110 * div) , 95 + div * 110 };
				}
			}

		}
	};
	class F皇帝十字陣 :public IFormatin
	{
	private:
		void 整列処理(LegionData& 軍団) override
		{
			const int 前列数 = (軍団.兵士.size() - 1) / 2;
			const int 後列数 = (軍団.兵士.size() - 1) - 前列数;
			const int max = 軍団.兵士.size() - 1;

			//指揮官
			軍団.兵士[max].現在列 = 軍団.兵士[max].初期列 = RowType::後列;
			軍団.兵士[max].着地地点 = { 100 , 150 };

			for (int a = 0; a < 前列数; a++)
			{
				double div = (double)a / 前列数;

				軍団.兵士[a].現在列 = 軍団.兵士[a].初期列 = RowType::前列;
				軍団.兵士[a].着地地点 = { 110 , div * 300 };
				if (軍団.兵士[a].着地地点.y >= 100 && 軍団.兵士[a].着地地点.y <= 200) { 軍団.兵士[a].着地地点.x += 50; };
			}

			for (int a = 前列数; a < 前列数 + 後列数; a++)
			{
				double div = (double)(a - 前列数) / 後列数;

				軍団.兵士[a].現在列 = 軍団.兵士[a].初期列 = RowType::後列;
				軍団.兵士[a].着地地点 = { 90 , div * 300 };
				if (軍団.兵士[a].着地地点.y >= 100 && 軍団.兵士[a].着地地点.y <= 200) { 軍団.兵士[a].着地地点.x -= 50; };
			}

			return;
		}
	};
	class F方陣 :public IFormatin
	{
	private:
		void 整列処理(LegionData& 軍団) override
		{
			const int 前列数 = (軍団.兵士.size() - 1) / 2;
			const int 後列数 = (軍団.兵士.size() - 1) - 前列数;
			const int max = 軍団.兵士.size() - 1;

			//指揮官
			軍団.兵士[max].現在列 = 軍団.兵士[max].初期列 = RowType::後列;
			軍団.兵士[max].着地地点 = { 100 , 150 };

			for (int a = 0; a < 前列数; a++)
			{
				double div = (double)a / 前列数;

				軍団.兵士[a].現在列 = 軍団.兵士[a].初期列 = RowType::前列;
				軍団.兵士[a].着地地点 = { 150 - abs(50 - 100 * div) , div * 300 };
			}

			for (int a = 前列数; a < 前列数 + 後列数; a++)
			{
				double div = (double)(a - 前列数) / 後列数;

				軍団.兵士[a].現在列 = 軍団.兵士[a].初期列 = RowType::後列;
				軍団.兵士[a].着地地点 = { 50 + abs(50 - 100 * div) , div * 300 };
			}

		}
	};
	class F獣の群れ :public IFormatin
	{
	private:
		void 整列処理(LegionData& 軍団) override
		{
			const int max = 軍団.兵士.size() - 1;

			//指揮官
			軍団.兵士[max].現在列 = 軍団.兵士[max].初期列 = RowType::前列;
			軍団.兵士[max].着地地点 = { 100 , 150 };

			for (int a = 0; a < max; a++)
			{
				double div = (double)a / max;
				軍団.兵士[a].現在列 = 軍団.兵士[a].初期列 = RowType::前列;
				軍団.兵士[a].着地地点 = { 100 + sin(div*PAI * 2) * 80 , 150 + cos(div*PAI * 2) * (30 + a % 3 * 60) };
			}

		}
	};
	class F一点突破の陣 :public IFormatin
	{
	private:
		void 整列処理(LegionData& 軍団) override
		{
			const int 前列数 = (軍団.兵士.size() - 1) / 2;
			const int 後列数 = (軍団.兵士.size() - 1) - 前列数;
			const int max = 軍団.兵士.size() - 1;

			//指揮官
			軍団.兵士[max].現在列 = 軍団.兵士[max].初期列 = RowType::前列;
			軍団.兵士[max].着地地点 = { 60 , 150 };

			for (int a = 0; a < 前列数; a++)
			{
				double div = (double)a / 前列数;

				軍団.兵士[a].現在列 = 軍団.兵士[a].初期列 = RowType::前列;
				軍団.兵士[a].着地地点 = { 90+ div*120 , 150 };
			}

			for (int a = 前列数; a < 前列数 + 後列数; a++)
			{
				double div = (double)(a - 前列数) / 後列数;

				軍団.兵士[a].現在列 = 軍団.兵士[a].初期列 = RowType::後列;
				軍団.兵士[a].着地地点 = { 30 , div * 300 };
			}
		}
	};
	class F横列陣形 :public IFormatin
	{
	private:
		void 整列処理(LegionData& 軍団) override
		{
			const int max = 軍団.兵士.size() - 1;

			//指揮官
			軍団.兵士[max].現在列 = 軍団.兵士[max].初期列 = RowType::前列;
			軍団.兵士[max].着地地点 = { 85 , 150 };

			for (int a = 0; a < max; a++)
			{
				double div = (double)a / max;
				軍団.兵士[a].現在列 = 軍団.兵士[a].初期列 = RowType::前列;
				軍団.兵士[a].着地地点 = { 30 + div*130 , 140 + (a % 2)*20 };
			}
		}
	};
	class F魚鱗の陣 :public IFormatin
	{
	private:
		void 整列処理(LegionData& 軍団) override
		{
			const int 前列数 = (軍団.兵士.size() - 1) / 2;
			const int 後列数 = (軍団.兵士.size() - 1) - 前列数;
			const int max = 軍団.兵士.size() - 1;

			//指揮官
			軍団.兵士[max].現在列 = 軍団.兵士[max].初期列 = RowType::後列;
			軍団.兵士[max].着地地点 = { 50 , 150 };

			for (int a = 0; a < 前列数; a++)
			{
				double div = (double)a / 前列数;

				軍団.兵士[a].現在列 = 軍団.兵士[a].初期列 = RowType::前列;
				軍団.兵士[a].着地地点 = { 190 - abs(150 - 300 * div) , div * 300 };
			}

			for (int a = 前列数; a < 前列数 + 後列数; a++)
			{
				double div = (double)(a - 前列数) / 後列数;

				軍団.兵士[a].現在列 = 軍団.兵士[a].初期列 = RowType::後列;
				if (div < 0.66) {
					div = div * 3 / 2;
					軍団.兵士[a].着地地点 = { 150 - abs(115 - 220 * div) , 40 + div * 220 };
				}
				else {
					div = (div - 0.66) * 3;
					軍団.兵士[a].着地地点 = { 110 - abs(57.5 - 110 * div) , 95 + div * 110 };
				}
			}
		}
	};
	class Fぐるぐるの陣 :public IFormatin
	{
	private:
		void 整列処理(LegionData& 軍団) override
		{
			const int max = 軍団.兵士.size() - 1;

			//指揮官
			軍団.兵士[max].現在列 = 軍団.兵士[max].初期列 = RowType::前列;
			軍団.兵士[max].着地地点 = { 100 , 150 };

			for (int a = 0; a < max; a++)
			{
				double div = (double)a / max;
				軍団.兵士[a].現在列 = 軍団.兵士[a].初期列 = RowType::前列;
				軍団.兵士[a].着地地点 = { 100 + sin(div*PAI * 4) * 40 * (div + 1) , 150 + cos(div*PAI * 4) * 75 * (div + 1) };
			}

		}
	};
	class F天の翼 :public IFormatin
	{
	private:
		void 整列処理(LegionData& 軍団) override
		{
			const int 前列数 = (軍団.兵士.size() - 1) / 2;
			const int 後列数 = (軍団.兵士.size() - 1) - 前列数;
			const int max = 軍団.兵士.size() - 1;

			//指揮官
			軍団.兵士[max].現在列 = 軍団.兵士[max].初期列 = RowType::後列;
			軍団.兵士[max].着地地点 = { 120 , 150 };

			for (int a = 0; a < 前列数; a++)
			{
				double div = (double)a / 前列数;
				int tmp_y = int(div * 280);
				軍団.兵士[a].現在列 = 軍団.兵士[a].初期列 = RowType::前列;
				軍団.兵士[a].着地地点 = { 190 - abs(70 - tmp_y % 140) , div * 280 };
				if (div > 0.5) { 軍団.兵士[a].着地地点.y += 20; }
			}

			for (int a = 前列数; a < 前列数 + 後列数; a++)
			{
				double div = (double)(a - 前列数) / 後列数;

				軍団.兵士[a].現在列 = 軍団.兵士[a].初期列 = RowType::後列;
				軍団.兵士[a].着地地点 = { 100 - abs(50 - 100 * div) , div * 300 };
			}

		}
	};
	class F冥の翼 :public IFormatin
	{
	private:
		void 整列処理(LegionData& 軍団) override
		{
			const int 前列数 = (軍団.兵士.size() - 1) / 2;
			const int 後列数 = (軍団.兵士.size() - 1) - 前列数;
			const int max = 軍団.兵士.size() - 1;

			//指揮官
			軍団.兵士[max].現在列 = 軍団.兵士[max].初期列 = RowType::後列;
			軍団.兵士[max].着地地点 = { 120 , 150 };

			for (int a = 前列数; a < 前列数 + 後列数; a++)
			{
				double div = (double)(a - 前列数) / 後列数;
				int tmp_y = int(div * 280);
				軍団.兵士[a].現在列 = 軍団.兵士[a].初期列 = RowType::前列;
				軍団.兵士[a].着地地点 = { 120 + abs(70 - tmp_y % 140) , div * 280 };
				if (div > 0.5) { 軍団.兵士[a].着地地点.y += 20; }
			}

			for (int a = 0; a < 前列数; a++)
			{
				double div = (double)a / 前列数;

				軍団.兵士[a].現在列 = 軍団.兵士[a].初期列 = RowType::後列;
				軍団.兵士[a].着地地点 = { 100 - abs(50 - 100 * div) , div * 300 };
			}
		}
	};

	EnumArray<IFormatin*, FormationType> 陣形データ;

	//各種隊列処理
	void LoadFormationS()
	{
		陣形データ[FormationType::縦列陣形] = &Singleton<F縦列陣形>::Get();
		陣形データ[FormationType::防壁の陣] = &Singleton<F防壁の陣>::Get();
		陣形データ[FormationType::攻戦の陣] = &Singleton<F攻戦の陣>::Get();
		陣形データ[FormationType::上翼突破の陣] = &Singleton<F上翼突破の陣>::Get();
		陣形データ[FormationType::下翼突破の陣] = &Singleton<F下翼突破の陣>::Get();
		陣形データ[FormationType::中央突破の陣] = &Singleton<F中央突破の陣>::Get();
		陣形データ[FormationType::三段構えの陣] = &Singleton<F三段構えの陣>::Get();
		陣形データ[FormationType::疾風陣] = &Singleton<F疾風陣>::Get();
		陣形データ[FormationType::背水の陣] = &Singleton<F背水の陣>::Get();
		陣形データ[FormationType::強撃陣] = &Singleton<F強撃陣>::Get();
		陣形データ[FormationType::盾の陣] = &Singleton<F盾の陣>::Get();
		陣形データ[FormationType::虎穴陣] = &Singleton<F虎穴陣>::Get();
		陣形データ[FormationType::修羅の陣] = &Singleton<F修羅の陣>::Get();
		陣形データ[FormationType::龍翼の陣] = &Singleton<F龍翼の陣>::Get();
		陣形データ[FormationType::戦神の陣] = &Singleton<F戦神の陣>::Get();
		陣形データ[FormationType::海王の陣] = &Singleton<F海王の陣>::Get();
		陣形データ[FormationType::皇帝十字陣] = &Singleton<F皇帝十字陣>::Get();
		陣形データ[FormationType::方陣] = &Singleton<F方陣>::Get();
		陣形データ[FormationType::獣の群れ] = &Singleton<F獣の群れ>::Get();
		陣形データ[FormationType::一点突破の陣] = &Singleton<F一点突破の陣>::Get();
		陣形データ[FormationType::横列陣形] = &Singleton<F横列陣形>::Get();
		陣形データ[FormationType::魚鱗の陣] = &Singleton<F魚鱗の陣>::Get();
		陣形データ[FormationType::ぐるぐるの陣] = &Singleton<Fぐるぐるの陣>::Get();
		陣形データ[FormationType::天の翼] = &Singleton<F天の翼>::Get();
		陣形データ[FormationType::冥の翼] = &Singleton<F冥の翼>::Get();

		for (int a = 0; a < (int)FormationType::COUNT; a++)
		{
			陣形データ[FormationType(a)]->陣形種 = FormationType(a);
		}

		陣形データ[FormationType::縦列陣形]->Set説明("縦列陣形", "");
		陣形データ[FormationType::防壁の陣]->Set説明("防壁の陣", "");
		陣形データ[FormationType::攻戦の陣]->Set説明("攻戦の陣", "");
		陣形データ[FormationType::上翼突破の陣]->Set説明("上翼突破の陣", "");
		陣形データ[FormationType::下翼突破の陣]->Set説明("下翼突破の陣", "");
		陣形データ[FormationType::中央突破の陣]->Set説明("中央突破の陣", "");
		陣形データ[FormationType::三段構えの陣]->Set説明("三段構えの陣", "");
		陣形データ[FormationType::疾風陣]->Set説明("疾風陣", "");
		陣形データ[FormationType::背水の陣]->Set説明("背水の陣", "");
		陣形データ[FormationType::強撃陣]->Set説明("強撃陣", "");
		陣形データ[FormationType::盾の陣]->Set説明("盾の陣", "");
		陣形データ[FormationType::虎穴陣]->Set説明("虎穴陣", "");
		陣形データ[FormationType::修羅の陣]->Set説明("修羅の陣", "");
		陣形データ[FormationType::龍翼の陣]->Set説明("龍翼の陣", "");
		陣形データ[FormationType::戦神の陣]->Set説明("戦神の陣", "");
		陣形データ[FormationType::海王の陣]->Set説明("海王の陣", "");
		陣形データ[FormationType::皇帝十字陣]->Set説明("皇帝十字陣", "");
		陣形データ[FormationType::方陣]->Set説明("方陣", "");
		陣形データ[FormationType::獣の群れ]->Set説明("獣の群れ", "");
		陣形データ[FormationType::一点突破の陣]->Set説明("一点突破の陣", "");
		陣形データ[FormationType::横列陣形]->Set説明("横列陣形", "");
		陣形データ[FormationType::魚鱗の陣]->Set説明("魚鱗の陣", "");
		陣形データ[FormationType::ぐるぐるの陣]->Set説明("ぐるぐるの陣", "");
		陣形データ[FormationType::天の翼]->Set説明("天の翼", "");
		陣形データ[FormationType::冥の翼]->Set説明("冥の翼", "");

		陣形データ[FormationType::縦列陣形]->Set性能(false);
		陣形データ[FormationType::防壁の陣]->Set性能(true);
		陣形データ[FormationType::攻戦の陣]->Set性能(true);
		陣形データ[FormationType::上翼突破の陣]->Set性能(true);
		陣形データ[FormationType::下翼突破の陣]->Set性能(true);
		陣形データ[FormationType::中央突破の陣]->Set性能(true);
		陣形データ[FormationType::三段構えの陣]->Set性能(true);
		陣形データ[FormationType::疾風陣]->Set性能(false);
		陣形データ[FormationType::背水の陣]->Set性能(false);
		陣形データ[FormationType::強撃陣]->Set性能(false);
		陣形データ[FormationType::盾の陣]->Set性能(true);
		陣形データ[FormationType::虎穴陣]->Set性能(false);
		陣形データ[FormationType::修羅の陣]->Set性能(false);
		陣形データ[FormationType::龍翼の陣]->Set性能(false);
		陣形データ[FormationType::戦神の陣]->Set性能(false);
		陣形データ[FormationType::海王の陣]->Set性能(true);
		陣形データ[FormationType::皇帝十字陣]->Set性能(true);
		陣形データ[FormationType::方陣]->Set性能( true);
		陣形データ[FormationType::獣の群れ]->Set性能( false);
		陣形データ[FormationType::一点突破の陣]->Set性能( false);
		陣形データ[FormationType::横列陣形]->Set性能( false);
		陣形データ[FormationType::魚鱗の陣]->Set性能( true);
		陣形データ[FormationType::ぐるぐるの陣]->Set性能( false);
		陣形データ[FormationType::天の翼]->Set性能(true);
		陣形データ[FormationType::冥の翼]->Set性能(true);
	}
}