/*
 * mygame.h: 本檔案儲遊戲本身的class的interface
 * Copyright (C) 2002-2008 Woei-Kae Chen <wkc@csie.ntut.edu.tw>
 *
 * This file is part of game, a free game development framework for windows.
 *
 * game is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * game is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *	 2004-03-02 V4.0
 *      1. Add CGameStateInit, CGameStateRun, and CGameStateOver to
 *         demonstrate the use of states.
 *   2005-09-13
 *      Rewrite the codes for CBall and CEraser.
 *   2005-09-20 V4.2Beta1.
 *   2005-09-29 V4.2Beta2.
 *   2006-02-08 V4.2
 *      1. Rename OnInitialUpdate() -> OnInit().
 *      2. Replace AUDIO_CANYON as AUDIO_NTUT.
 *      3. Add help bitmap to CGameStateRun.
 *   2006-09-09 V4.3
 *      1. Rename Move() and Show() as OnMove and OnShow() to emphasize that they are
 *         event driven.
 *   2008-02-15 V4.4
 *      1. Add namespace game_framework.
 *      2. Replace the demonstration of animation as a new bouncing ball.
 *      3. Use ShowInitProgress(percent) to display loading progress.
*/

#include <vector>
#include "CEraser.h"
#include "CBall.h"
#include "CBouncingBall.h"

namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// Constants
	/////////////////////////////////////////////////////////////////////////////

	enum AUDIO_ID {				// 定義各種音效的編號
		AUDIO_DING,				// 0
		AUDIO_LAKE,				// 1
		AUDIO_NTUT				// 2
	};


	///////////////////////
	// 時間暫停! by17
	///////////////////////

	class Timer {
	public:
		bool ifRun = 0;
	};

	//class Ballitsa{};

	//////////////////////////////////////
	// 建立敵人一號的物件 by17
	//////////////////////////////////////
	class Enemy01 {
	public:
		Enemy01(int i);
		void LoadBitmap();
		void OnMove(Timer t);
		void OnShow();
	private:
		CMovingBitmap  pic;
		int x,y,state;
	};


	/////////////////////////
	// 控制怪物生成 by 17
	/////////////////////////

	class EnemyCrafting {
	public:
		vector<Enemy01> enemy01;
		void craft();
	};

	//////////////////////////////
	// 子彈 by17
	//////////////////////////////

	class Arrow {
	public:
		Arrow(int bx, int by, CPoint p);
		void LoadBitmap();
		void OnMove(Timer t);
		void OnShow();
		void Shoot(CPoint p);
	private:
		CMovingBitmap  pic;
		int x, y;
		double px, py, dis;
	};



	///////////////////////////////
	//建立弩炮物件 by17
	///////////////////////////////

	class Ballitsa {
	public:
		vector<Arrow> arrow;
		Ballitsa(CPoint p);
		bool state = 0; //0 = 建造中, 1 = 建造完成
		void LoadBitmap();
		void OnShow();
		void OnMove(CPoint p);
		void Click(CPoint p);
		void EnemySeraching(Enemy01 en);
	private:
		int x, y;
		CMovingBitmap pic;
	};

	///////////////////////////////////
	// 建立按鈕物件 by17
	///////////////////////////////////

	class Button {
	public:
		int getX();
		int getY();
	private:
		int x, y;
	};

	class Button_ballitsa : public Button { //弩炮按鈕
	public:
		vector<Ballitsa> ballitsa;
		Button_ballitsa();
		void newballitsa(CPoint p);
		void LoadBitmap();
		void OnShow();
		void Click();
		
	private:
		CMovingBitmap pic;
		int x = 500, y = 50;
	};

	class Button_start : public Button { //開始按鈕
	public:
		Button_start();
		void LoadBitmap();
		void OnShow();
		void OnMove();
	private:
		CMovingBitmap pic;
		int x, y;
		
	};

	class Cant {
	public:
		void LoadBitmap(); 
		void OnShow();
		void OnMove(CPoint p);
	private:
		CMovingBitmap pic;
		int x = 0, y = 0;
	};
	
	/////////////////////////////////////////////////////////////////////////////
	// 這個class為遊戲的遊戲開頭畫面物件
	// 每個Member function的Implementation都要弄懂
	/////////////////////////////////////////////////////////////////////////////

	class CGameStateInit : public CGameState {
	public:
		CGameStateInit(CGame *g);
		void OnInit();  								// 遊戲的初值及圖形設定
		void OnBeginState();							// 設定每次重玩所需的變數
		void OnKeyUp(UINT, UINT, UINT); 				// 處理鍵盤Up的動作
		void OnLButtonDown(UINT nFlags, CPoint point);  // 處理滑鼠的動作
	protected:
		void OnShow();									// 顯示這個狀態的遊戲畫面
	private:
		CMovingBitmap logo;								// csie的logo
	};

	/////////////////////////////////////////////////////////////////////////////
	// 這個class為遊戲的遊戲執行物件，主要的遊戲程式都在這裡
	// 每個Member function的Implementation都要弄懂
	/////////////////////////////////////////////////////////////////////////////

	class CGameStateRun : public CGameState {
	public:
		CGameStateRun(CGame *g);
		~CGameStateRun();
		void OnBeginState();							// 設定每次重玩所需的變數
		void OnInit();  								// 遊戲的初值及圖形設定
		void OnKeyDown(UINT, UINT, UINT);
		void OnKeyUp(UINT, UINT, UINT);
		void OnLButtonDown(UINT nFlags, CPoint point);  // 處理滑鼠的動作
		void OnMouseMove(UINT nFlags, CPoint point);	// 處理滑鼠的動作 
		bool Onclick(CPoint p, int y);					//判斷點擊時的滑鼠位子
		Timer timer;
		
	protected:
		void OnMove();									// 移動遊戲元素
		void OnShow();									// 顯示這個狀態的遊戲畫面
	private:
		const int		NUMBALLS;	// 球的總數
		CMovingBitmap	background;	// 背景圖
		CMovingBitmap	menu;		//選單背景 by17
		CMovingBitmap	help;		// 說明圖
		CBall			*ball;		// 球的陣列
		CMovingBitmap	corner;		// 角落圖
		CEraser			eraser;		// 拍子
		CInteger		hits_left;	// 剩下的撞擊數
		CBouncingBall   bball;		// 反覆彈跳的球
		EnemyCrafting	ENC;		// 敵人生成系統 by17
		Cant			cant;		//禁止符號 by17
		Button_ballitsa Button_Ballitsa;//弩炮按鈕 by17
		Button_start	Button_Start;//開始按鈕 by17
		bool			buliding = 0;//防禦塔建造中 by17
		bool			cantbulid = 0;//這裡不能蓋 by17
		int				level = 0;
	};

	/////////////////////////////////////////////////////////////////////////////
	// 這個class為遊戲的結束狀態(Game Over)
	// 每個Member function的Implementation都要弄懂
	/////////////////////////////////////////////////////////////////////////////

	class CGameStateOver : public CGameState {
	public:
		CGameStateOver(CGame *g);
		void OnBeginState();							// 設定每次重玩所需的變數
		void OnInit();
	protected:
		void OnMove();									// 移動遊戲元素
		void OnShow();									// 顯示這個狀態的遊戲畫面
	private:
		int counter;	// 倒數之計數器
	};

}