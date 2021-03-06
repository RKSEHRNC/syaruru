/*
 * mygame.cpp: 本檔案儲遊戲本身的class的implementation
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
 * History:
 *   2002-03-04 V3.1
 *          Add codes to demostrate the use of CMovingBitmap::ShowBitmap(CMovingBitmap &).
 *	 2004-03-02 V4.0
 *      1. Add CGameStateInit, CGameStateRun, and CGameStateOver to
 *         demonstrate the use of states.
 *      2. Demo the use of CInteger in CGameStateRun.
 *   2005-09-13
 *      Rewrite the codes for CBall and CEraser.
 *   2005-09-20 V4.2Beta1.
 *   2005-09-29 V4.2Beta2.
 *      1. Add codes to display IDC_GAMECURSOR in GameStateRun.
 *   2006-02-08 V4.2
 *      1. Revise sample screens to display in English only.
 *      2. Add code in CGameStateInit to demo the use of PostQuitMessage().
 *      3. Rename OnInitialUpdate() -> OnInit().
 *      4. Fix the bug that OnBeginState() of GameStateInit is not called.
 *      5. Replace AUDIO_CANYON as AUDIO_NTUT.
 *      6. Add help bitmap to CGameStateRun.
 *   2006-09-09 V4.3
 *      1. Rename Move() and Show() as OnMove and OnShow() to emphasize that they are
 *         event driven.
 *   2006-12-30
 *      1. Bug fix: fix a memory leak problem by replacing PostQuitMessage(0) as
 *         PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE,0,0).
 *   2008-02-15 V4.4
 *      1. Add namespace game_framework.
 *      2. Replace the demonstration of animation as a new bouncing ball.
 *      3. Use ShowInitProgress(percent) to display loading progress. 
 *   2010-03-23 V4.6
 *      1. Demo MP3 support: use lake.mp3 to replace lake.wav.
*/

#include <vector>
#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include <math.h>
#include "audio.h"
#include "gamelib.h"
#include "mygame.h"

namespace game_framework {

/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的遊戲開頭畫面物件
/////////////////////////////////////////////////////////////////////////////

CGameStateInit::CGameStateInit(CGame *g)
: CGameState(g)
{
}

void CGameStateInit::OnInit()
{
	//
	// 當圖很多時，OnInit載入所有的圖要花很多時間。為避免玩遊戲的人
	//     等的不耐煩，遊戲會出現「Loading ...」，顯示Loading的進度。
	//
	ShowInitProgress(0);	// 一開始的loading進度為0%
	//
	// 開始載入資料
	//
	logo.LoadBitmap(IDB_BACKGROUND);
	//Sleep(300);				// 放慢，以便看清楚進度，實際遊戲請刪除此Sleep
	//
	// 此OnInit動作會接到CGameStaterRun::OnInit()，所以進度還沒到100%
	//
}

void CGameStateInit::OnBeginState()
{
}

void CGameStateInit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	const char KEY_ESC = 27;
	const char KEY_SPACE = ' ';
	if (nChar == KEY_SPACE)
		GotoGameState(GAME_STATE_RUN);						// 切換至GAME_STATE_RUN
	else if (nChar == KEY_ESC)								// Demo 關閉遊戲的方法
		PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE,0,0);	// 關閉遊戲
}

void CGameStateInit::OnLButtonDown(UINT nFlags, CPoint point)
{
	GotoGameState(GAME_STATE_RUN);		// 切換至GAME_STATE_RUN
}

void CGameStateInit::OnShow()
{
	//
	// 貼上logo
	//
	logo.SetTopLeft((SIZE_X - logo.Width())/2, SIZE_Y/8);
	logo.ShowBitmap();
	//
	// Demo螢幕字型的使用，不過開發時請盡量避免直接使用字型，改用CMovingBitmap比較好
	//
	CDC *pDC = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC 
	CFont f,*fp;
	f.CreatePointFont(160,"Times New Roman");	// 產生 font f; 160表示16 point的字
	fp=pDC->SelectObject(&f);					// 選用 font f
	pDC->SetBkColor(RGB(0,0,0));
	pDC->SetTextColor(RGB(255,255,0));
	pDC->TextOut(120,220,"Please click mouse or press SPACE to begin.");
	pDC->TextOut(5,395,"Press Ctrl-F to switch in between window mode and full screen mode.");
	if (ENABLE_GAME_PAUSE)
		pDC->TextOut(5,425,"Press Ctrl-Q to pause the Game.");
	pDC->TextOut(5,455,"Press Alt-F4 or ESC to Quit.");
	pDC->SelectObject(fp);						// 放掉 font f (千萬不要漏了放掉)
	CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC
}								

/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的結束狀態(Game Over)
/////////////////////////////////////////////////////////////////////////////

CGameStateOver::CGameStateOver(CGame *g)
: CGameState(g)
{
}

void CGameStateOver::OnMove()
{
	counter--;
	if (counter < 0)
		GotoGameState(GAME_STATE_INIT);
}

void CGameStateOver::OnBeginState()
{
	counter = 30 * 5; // 5 seconds
}

void CGameStateOver::OnInit()
{
	//
	// 當圖很多時，OnInit載入所有的圖要花很多時間。為避免玩遊戲的人
	//     等的不耐煩，遊戲會出現「Loading ...」，顯示Loading的進度。
	//
	ShowInitProgress(66);	// 接個前一個狀態的進度，此處進度視為66%
	//
	// 開始載入資料
	//
	Sleep(300);				// 放慢，以便看清楚進度，實際遊戲請刪除此Sleep
	//
	// 最終進度為100%
	//
	ShowInitProgress(100);
}

void CGameStateOver::OnShow()
{
	CDC *pDC = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC 
	CFont f,*fp;
	f.CreatePointFont(160,"Times New Roman");	// 產生 font f; 160表示16 point的字
	fp=pDC->SelectObject(&f);					// 選用 font f
	pDC->SetBkColor(RGB(0,0,0));
	pDC->SetTextColor(RGB(255,255,0));
	char str[80];								// Demo 數字對字串的轉換
	sprintf(str, "Game Over ! (%d)", counter / 30);
	pDC->TextOut(240,210,str);
	pDC->SelectObject(fp);						// 放掉 font f (千萬不要漏了放掉)
	CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC
}
///////////////////////////////////////////////
// 這裡是敵人一號的程式碼 by17
///////////////////////////////////////////////

Enemy01::Enemy01() {
	x = 0, y = 0;
}

Enemy01::Enemy01(int i) {//建立
	x = 47, y = (-20 - i), state = 1;
	pic.LoadBitmap("Bitmaps\\enemy01.bmp");
}

void Enemy01::OnMove(Timer t) {//移動
	if (t.ifRun == 1) {
		if (state == 1) { //向下
			if (y <= 122) y += 3;
			else state = 2;
		}
		if (state == 2) { //向右
			if (x <= 120) x += 3;
			else state = 3;
		}
		if (state == 3) { //向上
			if (y >= 48) y -= 3;
			else state = 4;
		}
		if (state == 4) { //向右
			if (x <= 407) x += 3;
			else state = 5;
		}
		if (state == 5) { //向下
			if (y <= 250) y += 3;
			else state = 6;
		}
		if (state == 6) { //向左
			if (x >= 305) x -= 3;
			else state = 7;
		}
		if (state == 7) { //向下
			if (y <= 365) y += 3;
			else state = 8;
		}
		if (state == 8) { //向左
			if (x >= 225) x -= 3;
			else state = 9;
		}
		if (state == 9) { //向上
			if (y >= 195) y -= 3;
			else state = 10;
		}
		if (state == 10) { //向右
			if (x <= 335) x += 3;
			else state = 11;
		}
		if (state == 11) { //向上
			if (y >= 120) y -= 3;
			else state = 12;
		}
		if (state == 12) { //向左
			if (x >= 170) x -= 3;
			else state = 13;
		}
		if (state == 13) { //向下
			if (y <= 195) y += 3;
			else state = 14;
		}
		if (state == 14) { //向左
			if (x >= 25) x -= 3;
			else state = 15;
		}
		if (state == 15) { //向下
			if (y <= 265) y += 3;
			else state = 16;
		}
		if (state == 16) { //向右
			if (x <= 145) x += 3;
			else state = 17;
		}
		if (state == 17) { //向下
			if (y <= 345) y += 3;
			else state = 18;
		}
		if (state == 18) { //向左
			if (x >= 20) x -= 3;
			else state = 19;
		}
		if (state == 19) { //向下
			if (y <= 440) y += 3;
			else state = 20;
		}
		if (state == 20) { //向右
			if (x <= 385) x += 3;
			else state = 21;
		}
		if (state == 21) { //向上
			if (y >= 330) y -= 3;
			else state = 22;
		}
		if (state == 22) { //向右
			if (x <= 500) x += 3;
		}
	}
}

void Enemy01::OnShow() {
	pic.SetTopLeft(x, y);
	pic.ShowBitmap();
}

int Enemy01::Getmidx() {
	return x + 10;
}

int Enemy01::Getmidy() {
	return y + 10;
}

/////////////////////////////
// 敵人生成系統 by17
/////////////////////////////

void EnemyCrafting::craft() {
	int i = 0;
	for (int a = 0; a < 5; a++) {
		enemy01.push_back(Enemy01(i));
		i = i +30;
	}
}

////////////////////////////
// 子彈的物件 by17
////////////////////////////

Arrow::Arrow(int bx, int by, Enemy01* EN,double distence) {
	pic.LoadBitmap("Bitmaps\\arrow.bmp", RGB(255, 255, 255));
	x = bx + 15, y = by + 5, dis = distence;
	en = *EN;
	px = (en.Getmidx - x) / dis;
	py = (en.Getmidy - y) / dis;
}
void Arrow::OnShow() {
	pic.SetTopLeft(x, y);
	pic.ShowBitmap();
}

void Arrow::OnMove(Timer t) {
	if (t.ifRun == 1) {
		x = x + int(px * 10);
		y = y + int(py * 10);
	}
}

///////////////////////////////////////////
// 弩炮的物件 by17
///////////////////////////////////////////

Ballitsa::Ballitsa(CPoint p) {
	pic.LoadBitmap("Bitmaps\\ballista.bmp", RGB(255, 255, 255));
	pic.SetTopLeft(p.x - 20, p.y - 19);
	colddown = 0;
	range = 50;
}

void Ballitsa::LoadBitmap() {
	pic.LoadBitmap("Bitmaps\\ballista.bmp", RGB(255, 255, 255));
}

void Ballitsa::OnShow() {
	pic.ShowBitmap();
}

void Ballitsa::OnMove(CPoint p) {
	pic.SetTopLeft(p.x - 20, p.y - 19);
}

void Ballitsa::EnemySeraching(Enemy01 en) {
	if (colddown == 0) {
		distence = sqrt(pow(en.Getmidx - (x + 20), 2) + pow(en.Getmidy - (y + 19), 2));
		if (distence < range) {
			arrow.push_back(Arrow(x, y, &en, distence));
			colddown = 30;
		}
	}

}

///////////////////////
// button by17
///////////////////////
Button::Button() {
	x = 500;
	y = 400;
}

int Button::getX() {
	return x;
}

int Button::getY() {
	return y;
}

void Button::OnShow() {
	pic.SetTopLeft(x, y);
	pic.ShowBitmap();
}

void Button::Set(char* f) {
	pic.LoadBitmap(f);
}



//////////////////////////////////////////
// 這裡是弩炮按鈕的物件 by17
//////////////////////////////////////////

Button_ballitsa::Button_ballitsa(){
}

void Button_ballitsa::newballitsa(CPoint p) {
	ballitsa.push_back(Ballitsa(p));
}

void Button_ballitsa::LoadBitmap() {
	pic.LoadBitmap("Bitmaps\\button_ballista.bmp");
}

void Button_ballitsa::OnShow() {
	pic.SetTopLeft(x, y);
	pic.ShowBitmap();
}

//////////////////////////////////////////
// 按鈕的物件 by17
//////////////////////////////////////////

/*Button_start::Button_start() {
	x = 500;
	y = 400;
}

void Button_start::LoadBitmap() {
	pic.LoadBitmap("Bitmaps\\button_start.bmp");
}*/


///////////////////////////////////
// cant      by17
///////////////////////////////////


void Cant::LoadBitmap() {
	pic.LoadBitmap("Bitmaps\\cant.bmp", RGB(255, 255, 255));
}

void Cant::OnShow() {
	pic.ShowBitmap();
}

void Cant::OnMove(CPoint p) {
	pic.SetTopLeft(p.x - 25, p.y - 25);
}

///////////////////////////////////
// 射程      by17
///////////////////////////////////


void Range::LoadBitmap() {
	pic.LoadBitmap("Bitmaps\\range.bmp", RGB(255, 255, 255));
}

void Range::OnShow() {
	pic.ShowBitmap();
}

void Range::OnMove(CPoint p) {
	pic.SetTopLeft(p.x - 100, p.y - 100);
}


/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的遊戲執行物件，主要的遊戲程式都在這裡
/////////////////////////////////////////////////////////////////////////////

CGameStateRun::CGameStateRun(CGame *g)
: CGameState(g), NUMBALLS(28)
{
	ball = new CBall [NUMBALLS];
}

CGameStateRun::~CGameStateRun()
{
	delete [] ball;
}

void CGameStateRun::OnBeginState()
{
	const int BALL_GAP = 90;
	const int BALL_XY_OFFSET = 45;
	const int BALL_PER_ROW = 7;
	const int HITS_LEFT = 10;
	const int HITS_LEFT_X = 590;
	const int HITS_LEFT_Y = 0;
	const int ANIMATION_SPEED = 15;
	const int MENU_X = 485;
	for (int i = 0; i < NUMBALLS; i++) {				// 設定球的起始座標
		int x_pos = i % BALL_PER_ROW;
		int y_pos = i / BALL_PER_ROW;
		ball[i].SetXY(x_pos * BALL_GAP + BALL_XY_OFFSET, y_pos * BALL_GAP + BALL_XY_OFFSET);
		ball[i].SetDelay(x_pos);
		ball[i].SetIsAlive(true);
	}
	eraser.Initialize();
	menu.SetTopLeft(MENU_X, -10);						// 設定選單背景的座標 by17
	help.SetTopLeft(0, SIZE_Y - help.Height());			// 設定說明圖的起始座標
	hits_left.SetInteger(HITS_LEFT);					// 指定剩下的撞擊數
	hits_left.SetTopLeft(HITS_LEFT_X,HITS_LEFT_Y);		// 指定剩下撞擊數的座標
	//CAudio::Instance()->Play(AUDIO_LAKE, 0);			// 撥放 WAVE
	//CAudio::Instance()->Play(AUDIO_DING, false);		// 撥放 WAVE
	//CAudio::Instance()->Play(AUDIO_NTUT, false);		// 撥放 MIDI
}

void CGameStateRun::OnMove()							// 移動遊戲元素
{
	//
	// 如果希望修改cursor的樣式，則將下面程式的commment取消即可
	//
	// SetCursor(AfxGetApp()->LoadCursor(IDC_GAMECURSOR));
	if (ENC.enemy01.size() != 0) {
		for (int a = 0; a < 5; a++) ENC.enemy01.at(a).OnMove(timer);
		if (Button_Ballitsa.ballitsa.size() != 0) {
			for (int a = 0; a < Button_Ballitsa.ballitsa.size(); a++) {
				for (int b = 0; b < ENC.enemy01.size(); b++) {
					Button_Ballitsa.ballitsa.at(a).EnemySeraching(ENC.enemy01.at(b));
				}
			}
		}
	}
	if (Button_Ballitsa.ballitsa.size() != 0) {
		for (int a = 0; a < Button_Ballitsa.ballitsa.size(); a++) {
			if (Button_Ballitsa.ballitsa.at(a).colddown > 0) Button_Ballitsa.ballitsa.at(a).colddown -= 1;
		}
	}

	//if (Button_Ballitsa.ballitsa!=NULL && Button_Ballitsa.ballitsa->arrow != NULL) Button_Ballitsa.ballitsa->arrow->OnMove(timer);

	// 判斷擦子是否碰到球
	//
	
	/*for (i=0; i < NUMBALLS; i++)
		if (ball[i].IsAlive() && ball[i].HitEraser(&eraser)) {
			ball[i].SetIsAlive(false);
			CAudio::Instance()->Play(AUDIO_DING);
			hits_left.Add(-1);
			//
			// 若剩餘碰撞次數為0，則跳到Game Over狀態
			//
			if (hits_left.GetInteger() <= 0) {
				CAudio::Instance()->Stop(AUDIO_LAKE);	// 停止 WAVE
				CAudio::Instance()->Stop(AUDIO_NTUT);	// 停止 MIDI
				GotoGameState(GAME_STATE_OVER);
			}
		}*/

	//
	// 移動彈跳的球
	//
	bball.OnMove();
	eraser.OnMove();
}

void CGameStateRun::OnInit()  								// 遊戲的初值及圖形設定
{
	//
	// 當圖很多時，OnInit載入所有的圖要花很多時間。為避免玩遊戲的人
	//     等的不耐煩，遊戲會出現「Loading ...」，顯示Loading的進度。
	//
	ShowInitProgress(33);	// 接個前一個狀態的進度，此處進度視為33%
	//
	// 開始載入資料
	//
	int i;
	for (i = 0; i < NUMBALLS; i++)	
		ball[i].LoadBitmap();								// 載入第i個球的圖形
	eraser.LoadBitmap();
	background.LoadBitmap("Bitmaps\\background.bmp");		// 載入背景的圖形
	menu.LoadBitmap("Bitmaps\\menu.bmp");					// 載入選單的圖形
	//
	// 完成部分Loading動作，提高進度
	//
	ShowInitProgress(50);
	Sleep(300); // 放慢，以便看清楚進度，實際遊戲請刪除此Sleep
	//
	// 繼續載入其他資料
	//
	help.LoadBitmap(IDB_HELP,RGB(255,255,255));				// 載入說明的圖形
	corner.LoadBitmap(IDB_CORNER);							// 載入角落圖形
	corner.ShowBitmap(background);							// 將corner貼到background
	Button_Ballitsa.LoadBitmap();
	Button_Start.Set("Bitmaps\\button_start.bmp");
	Button_Pause.Set("Bitmaps\\button_pause.bmp");
	Button_Cannel.Set("Bitmaps\\button_cannel.bmp");
	bball.LoadBitmap();										// 載入圖形
	cant.LoadBitmap();
	range.LoadBitmap();
	hits_left.LoadBitmap();									
	CAudio::Instance()->Load(AUDIO_DING,  "sounds\\ding.wav");	// 載入編號0的聲音ding.wav
	CAudio::Instance()->Load(AUDIO_LAKE,  "sounds\\lake.mp3");	// 載入編號1的聲音lake.mp3
	CAudio::Instance()->Load(AUDIO_NTUT,  "sounds\\ntut.mid");	// 載入編號2的聲音ntut.mid
	//
	// 此OnInit動作會接到CGameStaterOver::OnInit()，所以進度還沒到100%
	//
}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	const char KEY_LEFT  = 0x25; // keyboard左箭頭
	const char KEY_UP    = 0x26; // keyboard上箭頭
	const char KEY_RIGHT = 0x27; // keyboard右箭頭
	const char KEY_DOWN  = 0x28; // keyboard下箭頭
	if (nChar == KEY_LEFT)
		eraser.SetMovingLeft(true);
	if (nChar == KEY_RIGHT)
		eraser.SetMovingRight(true);
	if (nChar == KEY_UP)
		eraser.SetMovingUp(true);
	if (nChar == KEY_DOWN)
		eraser.SetMovingDown(true);
}

void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	const char KEY_LEFT  = 0x25; // keyboard左箭頭
	const char KEY_UP    = 0x26; // keyboard上箭頭
	const char KEY_RIGHT = 0x27; // keyboard右箭頭
	const char KEY_DOWN  = 0x28; // keyboard下箭頭
	if (nChar == KEY_LEFT)
		eraser.SetMovingLeft(false);
	if (nChar == KEY_RIGHT)
		eraser.SetMovingRight(false);
	if (nChar == KEY_UP)
		eraser.SetMovingUp(false);
	if (nChar == KEY_DOWN)
		eraser.SetMovingDown(false);
}



bool CGameStateRun::Onclick(CPoint p, int y) { //點擊判定 by17
	HWND h = FindWindow(NULL, "game");
	GetCursorPos(&p);
	ScreenToClient(h, &p);
	return (500<p.x && p.x < 640 && y < (p.y-30) && (p.y-30) < (y+80));
}

void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point)  // 處理滑鼠的動作(左鍵按下) by17
{	
	//if (Button_Ballitsa.ballitsa.size() != 0 && Button_Ballitsa.ballitsa.rbegin()->state == 1) Button_Ballitsa.ballitsa.rbegin()->Click(point);
	
	if (Onclick(point, 50) == 1) { //弩炮
		Button_Ballitsa.newballitsa(point);
		buliding = 1;
	}
	else if (buliding == 1&& cantbulid==0) { 
		Button_Ballitsa.ballitsa.rbegin()->state = 1;
		buliding = 0;
	}
	if (Onclick(point, 400) == 1) { //start / pause
		timer.ifRun = !(timer.ifRun);
		if (level == 0) level = 1, ENC.craft();
		//for (int a = 0; a < int(ENC.enemy01.size()); a++) Button_Start.OnMove();
	} 
	
}


void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point)	// 處理滑鼠的動作(移動) by17
{
	if (buliding == 1 && Button_Ballitsa.ballitsa.rbegin()->state == 0) {
		Button_Ballitsa.ballitsa.rbegin()->OnMove(point);
		cant.OnMove(point);
		range.OnMove(point);
	}
	if (point.x > 470 || (point.x > 30 && point.x < 85 && point.y < 160)) { //把防禦塔能不能蓋寫在這裡面 by 17
			cantbulid = 1;
			cant.OnMove(point);
	}
	else if (point.x > 78 && point.x < 152 && point.y < 160 && point.y>116) cantbulid = 1;
	else if (point.x > 111 && point.x < 152 && point.y < 157 && point.y>35) cantbulid = 1;
	else if (point.x > 111 && point.x < 440 && point.y < 75 && point.y>35) cantbulid = 1;
	else if (point.x > 400 && point.x < 440 && point.y < 284 && point.y>75) cantbulid = 1;
	else if (point.x >294 && point.x < 440 && point.y < 286 && point.y>245) cantbulid = 1;
	else if (point.x > 294 && point.x < 331 && point.y < 400 && point.y>286) cantbulid = 1;
	else if (point.x > 215 && point.x < 331 && point.y < 400 && point.y>360) cantbulid = 1;
	else if (point.x > 215 && point.x < 250 && point.y < 400 && point.y>185) cantbulid = 1;
	else if (point.x > 215 && point.x < 369 && point.y < 224 && point.y>184) cantbulid = 1;
	else if (point.x > 330 && point.x < 369 && point.y < 224 && point.y>107) cantbulid = 1;
	else if (point.x > 159 && point.x < 369 && point.y < 148 && point.y>107) cantbulid = 1;
	else if (point.x > 159 && point.x < 197 && point.y < 222 && point.y>107) cantbulid = 1;
	else if (point.x > 10 && point.x < 197 && point.y < 224 && point.y>182) cantbulid = 1;
	else if (point.x > 10 && point.x < 50 && point.y < 302 && point.y>184) cantbulid = 1;
	else if (point.x > 10 && point.x < 176 && point.y < 303 && point.y>263) cantbulid = 1;
	else if (point.x > 178 && point.x < 138 && point.y < 377 && point.y>263) cantbulid = 1;
	else if (point.x > 8 && point.x < 177 && point.y < 377 && point.y>337) cantbulid = 1 ;
	else if (point.x > 8 && point.x < 48 && point.y>337) cantbulid = 1;
	else if (point.x > 10 && point.x < 416 && point.y>434) cantbulid = 1;
	else if (point.x > 376 && point.x < 416 && point.y>317) cantbulid = 1;
	else if (point.x > 376 && point.y < 358 && point.y>319) cantbulid = 1;
	else cantbulid = 0;
} 


void CGameStateRun::OnShow()
{
	//
	//  注意：Show裡面千萬不要移動任何物件的座標，移動座標的工作應由Move做才對，
	//        否則當視窗重新繪圖時(OnDraw)，物件就會移動，看起來會很怪。換個術語
	//        說，Move負責MVC中的Model，Show負責View，而View不應更動Model。
	//
	//
	//  貼上背景圖、撞擊數、球、擦子、彈跳的球
	//
	background.ShowBitmap();			// 貼上背景圖
	hits_left.ShowBitmap();
	if (ENC.enemy01.size() != 0){
		for (int a = 0; a < 5; a++) ENC.enemy01.at(a).OnShow();
	}
	//
	//  貼上左上及右下角落的圖
	//
	corner.SetTopLeft(0,0);
	//corner.ShowBitmap();
	corner.SetTopLeft(SIZE_X-corner.Width(), SIZE_Y-corner.Height());
	//corner.ShowBitmap();

	//  貼上選單 by17
	menu.ShowBitmap();
	Button_Ballitsa.OnShow();
	if (buliding == 1) Button_Cannel.OnShow();
	else if (timer.ifRun == 0) Button_Start.OnShow();
	else if (timer.ifRun == 1) Button_Pause.OnShow();


	if (Button_Ballitsa.ballitsa.size() != 0) {
		for (int a = 0;a < int(Button_Ballitsa.ballitsa.size()); a++) {
			Button_Ballitsa.ballitsa.at(a).OnShow();
		}
		Button_Ballitsa.ballitsa.rbegin()->OnShow();
		if (buliding == 1) { 
			if(cantbulid == 1) cant.OnShow();
			else range.OnShow();
		}
		//if (Button_Ballitsa.ballitsa.rbegin()->arrow != NULL) Button_Ballitsa.ballitsa->arrow->OnShow();
	}
	//eraser.OnShow();
	//bball.OnShow();

}
}