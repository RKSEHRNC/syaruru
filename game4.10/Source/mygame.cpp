/*
 * mygame.cpp: ���ɮ��x�C��������class��implementation
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

#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "mygame.h"

namespace game_framework {

/////////////////////////////////////////////////////////////////////////////
// �o��class���C�����C���}�Y�e������
/////////////////////////////////////////////////////////////////////////////

CGameStateInit::CGameStateInit(CGame *g)
: CGameState(g)
{
}

void CGameStateInit::OnInit()
{
	//
	// ��ϫܦh�ɡAOnInit���J�Ҧ����ϭn��ܦh�ɶ��C���קK���C�����H
	//     �������@�СA�C���|�X�{�uLoading ...�v�A���Loading���i�סC
	//
	ShowInitProgress(0);	// �@�}�l��loading�i�׬�0%
	//
	// �}�l���J���
	//
	logo.LoadBitmap(IDB_BACKGROUND);
	//Sleep(300);				// ��C�A�H�K�ݲM���i�סA��ڹC���ЧR����Sleep
	//
	// ��OnInit�ʧ@�|����CGameStaterRun::OnInit()�A�ҥH�i���٨S��100%
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
		GotoGameState(GAME_STATE_RUN);						// ������GAME_STATE_RUN
	else if (nChar == KEY_ESC)								// Demo �����C������k
		PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE,0,0);	// �����C��
}

void CGameStateInit::OnLButtonDown(UINT nFlags, CPoint point)
{
	GotoGameState(GAME_STATE_RUN);		// ������GAME_STATE_RUN
}

void CGameStateInit::OnShow()
{
	//
	// �K�Wlogo
	//
	logo.SetTopLeft((SIZE_X - logo.Width())/2, SIZE_Y/8);
	logo.ShowBitmap();
	//
	// Demo�ù��r�����ϥΡA���L�}�o�ɽкɶq�קK�����ϥΦr���A���CMovingBitmap����n
	//
	CDC *pDC = CDDraw::GetBackCDC();			// ���o Back Plain �� CDC 
	CFont f,*fp;
	f.CreatePointFont(160,"Times New Roman");	// ���� font f; 160���16 point���r
	fp=pDC->SelectObject(&f);					// ��� font f
	pDC->SetBkColor(RGB(0,0,0));
	pDC->SetTextColor(RGB(255,255,0));
	pDC->TextOut(120,220,"Please click mouse or press SPACE to begin.");
	pDC->TextOut(5,395,"Press Ctrl-F to switch in between window mode and full screen mode.");
	if (ENABLE_GAME_PAUSE)
		pDC->TextOut(5,425,"Press Ctrl-Q to pause the Game.");
	pDC->TextOut(5,455,"Press Alt-F4 or ESC to Quit.");
	pDC->SelectObject(fp);						// �� font f (�d�U���n�|�F��)
	CDDraw::ReleaseBackCDC();					// �� Back Plain �� CDC
}								

/////////////////////////////////////////////////////////////////////////////
// �o��class���C�����������A(Game Over)
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
	// ��ϫܦh�ɡAOnInit���J�Ҧ����ϭn��ܦh�ɶ��C���קK���C�����H
	//     �������@�СA�C���|�X�{�uLoading ...�v�A���Loading���i�סC
	//
	ShowInitProgress(66);	// ���ӫe�@�Ӫ��A���i�סA���B�i�׵���66%
	//
	// �}�l���J���
	//
	Sleep(300);				// ��C�A�H�K�ݲM���i�סA��ڹC���ЧR����Sleep
	//
	// �̲׶i�׬�100%
	//
	ShowInitProgress(100);
}

void CGameStateOver::OnShow()
{
	CDC *pDC = CDDraw::GetBackCDC();			// ���o Back Plain �� CDC 
	CFont f,*fp;
	f.CreatePointFont(160,"Times New Roman");	// ���� font f; 160���16 point���r
	fp=pDC->SelectObject(&f);					// ��� font f
	pDC->SetBkColor(RGB(0,0,0));
	pDC->SetTextColor(RGB(255,255,0));
	char str[80];								// Demo �Ʀr��r�ꪺ�ഫ
	sprintf(str, "Game Over ! (%d)", counter / 30);
	pDC->TextOut(240,210,str);
	pDC->SelectObject(fp);						// �� font f (�d�U���n�|�F��)
	CDDraw::ReleaseBackCDC();					// �� Back Plain �� CDC
}
///////////////////////////////////////////////
// �o�̬O�ĤH�@�����{���X by17
///////////////////////////////////////////////

Enemy01::Enemy01() {//�إ�
	x = 47, y = -20, state = 1;
}

void Enemy01::OnMove(Timer t) {//����
	if (t.ifRun == 1) {
		if (state == 1) { //�V�U
			if (y <= 122) y += 3;
			else state = 2;
		}
		if (state == 2) { //�V�k
			if (x <= 120) x += 3;
			else state = 3;
		}
		if (state == 3) { //�V�W
			if (y >= 48) y -= 3;
			else state = 4;
		}
		if (state == 4) { //�V�k
			if (x <= 407) x += 3;
			else state = 5;
		}
		if (state == 5) { //�V�U
			if (y <= 250) y += 3;
			else state = 6;
		}
		if (state == 6) { //�V��
			if (x >= 305) x -= 3;
			else state = 7;
		}
		if (state == 7) { //�V�U
			if (y <= 365) y += 3;
			else state = 8;
		}
		if (state == 8) { //�V��
			if (x >= 225) x -= 3;
			else state = 9;
		}
		if (state == 9) { //�V�W
			if (y >= 195) y -= 3;
			else state = 10;
		}
		if (state == 10) { //�V�k
			if (x <= 335) x += 3;
			else state = 11;
		}
		if (state == 11) { //�V�W
			if (y >= 120) y -= 3;
			else state = 12;
		}
		if (state == 12) { //�V��
			if (x >= 170) x -= 3;
			else state = 13;
		}
		if (state == 13) { //�V�U
			if (y <= 195) y += 3;
			else state = 14;
		}
		if (state == 14) { //�V��
			if (x >= 25) x -= 3;
			else state = 15;
		}
		if (state == 15) { //�V�U
			if (y <= 265) y += 3;
			else state = 16;
		}
		if (state == 16) { //�V�k
			if (x <= 145) x += 3;
			else state = 17;
		}
		if (state == 17) { //�V�U
			if (y <= 345) y += 3;
			else state = 18;
		}
		if (state == 18) { //�V��
			if (x >= 20) x -= 3;
			else state = 19;
		}
		if (state == 19) { //�V�U
			if (y <= 440) y += 3;
			else state = 20;
		}
		if (state == 20) { //�V�k
			if (x <= 385) x += 3;
			else state = 21;
		}
		if (state == 21) { //�V�W
			if (y >= 330) y -= 3;
			else state = 22;
		}
		if (state == 22) { //�V�k
			if (x <= 500) x += 3;
		}
	}
}

void Enemy01::LoadBitmap() {//���J�Ϲ�
	pic.LoadBitmap("Bitmaps\\enemy01.bmp");
}

void Enemy01::OnShow() {
	pic.SetTopLeft(x, y);
	pic.ShowBitmap();
}

///////////////////////////////////////////
// ���������� by17
///////////////////////////////////////////

Ballitsa::Ballitsa(CPoint p) {
	pic.LoadBitmap("Bitmaps\\ballista.bmp", RGB(255, 255, 255));
	pic.SetTopLeft(p.x - 20, p.y - 19);
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


///////////////////////
// button by17
///////////////////////
int Button::getX() {
	return x;
}

int Button::getY() {
	return y;
}


//////////////////////////////////////////
// �o�̬O�������s������ by17
//////////////////////////////////////////

Button_ballitsa::Button_ballitsa(){
}

void Button_ballitsa::newballitsa(CPoint p) {
	 ballitsa = new Ballitsa(p);
}

void Button_ballitsa::LoadBitmap() {
	pic.LoadBitmap("Bitmaps\\button_ballista.bmp");
}

void Button_ballitsa::OnShow() {
	pic.SetTopLeft(x, y);
	pic.ShowBitmap();
}

void Button_ballitsa::Click() {

}


//////////////////////////////////////////
// �}�l���s������ by17
//////////////////////////////////////////

Button_start::Button_start() {
	x = 500;
	y = 400;
}

void Button_start::LoadBitmap() {
	pic.LoadBitmap("Bitmaps\\button_start.bmp");
}

void Button_start::OnShow() {
	pic.SetTopLeft(x, y);
	pic.ShowBitmap();
}

void Button_start::OnMove() {
	x = x - 40;

}


/////////////////////////////////////////////////////////////////////////////
// �o��class���C�����C�����檫��A�D�n���C���{�����b�o��
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
	for (int i = 0; i < NUMBALLS; i++) {				// �]�w�y���_�l�y��
		int x_pos = i % BALL_PER_ROW;
		int y_pos = i / BALL_PER_ROW;
		ball[i].SetXY(x_pos * BALL_GAP + BALL_XY_OFFSET, y_pos * BALL_GAP + BALL_XY_OFFSET);
		ball[i].SetDelay(x_pos);
		ball[i].SetIsAlive(true);
	}
	eraser.Initialize();
	menu.SetTopLeft(MENU_X, -10);						// �]�w���I�����y�� by17
	help.SetTopLeft(0, SIZE_Y - help.Height());			// �]�w�����Ϫ��_�l�y��
	hits_left.SetInteger(HITS_LEFT);					// ���w�ѤU��������
	hits_left.SetTopLeft(HITS_LEFT_X,HITS_LEFT_Y);		// ���w�ѤU�����ƪ��y��
	CAudio::Instance()->Play(AUDIO_LAKE, 0);			// ���� WAVE
	CAudio::Instance()->Play(AUDIO_DING, false);		// ���� WAVE
	CAudio::Instance()->Play(AUDIO_NTUT, false);		// ���� MIDI
}

void CGameStateRun::OnMove()							// ���ʹC������
{
	//
	// �p�G�Ʊ�ק�cursor���˦��A�h�N�U���{����commment�����Y�i
	//
	// SetCursor(AfxGetApp()->LoadCursor(IDC_GAMECURSOR));

	enemy01.OnMove(timer);


	// �P�_���l�O�_�I��y
	//
	
	/*for (i=0; i < NUMBALLS; i++)
		if (ball[i].IsAlive() && ball[i].HitEraser(&eraser)) {
			ball[i].SetIsAlive(false);
			CAudio::Instance()->Play(AUDIO_DING);
			hits_left.Add(-1);
			//
			// �Y�Ѿl�I�����Ƭ�0�A�h����Game Over���A
			//
			if (hits_left.GetInteger() <= 0) {
				CAudio::Instance()->Stop(AUDIO_LAKE);	// ���� WAVE
				CAudio::Instance()->Stop(AUDIO_NTUT);	// ���� MIDI
				GotoGameState(GAME_STATE_OVER);
			}
		}*/

	//
	// ���ʼu�����y
	//
	bball.OnMove();
	eraser.OnMove();
}

void CGameStateRun::OnInit()  								// �C������Ȥιϧγ]�w
{
	//
	// ��ϫܦh�ɡAOnInit���J�Ҧ����ϭn��ܦh�ɶ��C���קK���C�����H
	//     �������@�СA�C���|�X�{�uLoading ...�v�A���Loading���i�סC
	//
	ShowInitProgress(33);	// ���ӫe�@�Ӫ��A���i�סA���B�i�׵���33%
	//
	// �}�l���J���
	//
	int i;
	for (i = 0; i < NUMBALLS; i++)	
		ball[i].LoadBitmap();								// ���J��i�Ӳy���ϧ�
	eraser.LoadBitmap();
	background.LoadBitmap("Bitmaps\\background.bmp");		// ���J�I�����ϧ�
	menu.LoadBitmap("Bitmaps\\menu.bmp");					// ���J��檺�ϧ�
	//
	// ��������Loading�ʧ@�A�����i��
	//
	ShowInitProgress(50);
	Sleep(300); // ��C�A�H�K�ݲM���i�סA��ڹC���ЧR����Sleep
	//
	// �~����J��L���
	//
	help.LoadBitmap(IDB_HELP,RGB(255,255,255));				// ���J�������ϧ�
	corner.LoadBitmap(IDB_CORNER);							// ���J�����ϧ�
	corner.ShowBitmap(background);							// �Ncorner�K��background
	enemy01.LoadBitmap();
	Button_Ballitsa.LoadBitmap();
	Button_Start.LoadBitmap();
	bball.LoadBitmap();										// ���J�ϧ�
	hits_left.LoadBitmap();									
	CAudio::Instance()->Load(AUDIO_DING,  "sounds\\ding.wav");	// ���J�s��0���n��ding.wav
	CAudio::Instance()->Load(AUDIO_LAKE,  "sounds\\lake.mp3");	// ���J�s��1���n��lake.mp3
	CAudio::Instance()->Load(AUDIO_NTUT,  "sounds\\ntut.mid");	// ���J�s��2���n��ntut.mid
	//
	// ��OnInit�ʧ@�|����CGameStaterOver::OnInit()�A�ҥH�i���٨S��100%
	//
}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	const char KEY_LEFT  = 0x25; // keyboard���b�Y
	const char KEY_UP    = 0x26; // keyboard�W�b�Y
	const char KEY_RIGHT = 0x27; // keyboard�k�b�Y
	const char KEY_DOWN  = 0x28; // keyboard�U�b�Y
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
	const char KEY_LEFT  = 0x25; // keyboard���b�Y
	const char KEY_UP    = 0x26; // keyboard�W�b�Y
	const char KEY_RIGHT = 0x27; // keyboard�k�b�Y
	const char KEY_DOWN  = 0x28; // keyboard�U�b�Y
	if (nChar == KEY_LEFT)
		eraser.SetMovingLeft(false);
	if (nChar == KEY_RIGHT)
		eraser.SetMovingRight(false);
	if (nChar == KEY_UP)
		eraser.SetMovingUp(false);
	if (nChar == KEY_DOWN)
		eraser.SetMovingDown(false);
}



bool CGameStateRun::Onclick(CPoint p, int y) { //�I���P�w by17
	HWND h = FindWindow(NULL, "game");
	GetCursorPos(&p);
	ScreenToClient(h, &p);
	return (500<p.x && p.x < 640 && y < (p.y-30) && (p.y-30) < (y+80));
}

void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point)  // �B�z�ƹ����ʧ@(������U) by17
{	
	if (Onclick(point, 50) == 1) { //����
		Button_Ballitsa.newballitsa(point);
		buliding = 1;
	}
	else if (buliding == 1&& point.x < 470) {
		Button_Ballitsa.ballitsa->state = 1;
	}
	if (Onclick(point, 400) == 1) { //start / pause
		timer.ifRun = !(timer.ifRun);
	} 
	
}


void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point)	// �B�z�ƹ����ʧ@(����) by17
{
	if(buliding==1 && Button_Ballitsa.ballitsa->state == 0) 	Button_Ballitsa.ballitsa->OnMove(point);
} 




void CGameStateRun::OnShow()
{
	//
	//  �`�N�GShow�̭��d�U���n���ʥ��󪫥󪺮y�СA���ʮy�Ъ��u�@����Move���~��A
	//        �_�h��������sø�Ϯ�(OnDraw)�A����N�|���ʡA�ݰ_�ӷ|�ܩǡC���ӳN�y
	//        ���AMove�t�dMVC����Model�AShow�t�dView�A��View�������Model�C
	//
	//
	//  �K�W�I���ϡB�����ơB�y�B���l�B�u�����y
	//
	background.ShowBitmap();			// �K�W�I����
	hits_left.ShowBitmap();
	enemy01.OnShow();
	
	//
	//  �K�W���W�Υk�U��������
	//
	corner.SetTopLeft(0,0);
	//corner.ShowBitmap();
	corner.SetTopLeft(SIZE_X-corner.Width(), SIZE_Y-corner.Height());
	//corner.ShowBitmap();

	//  �K�W��� by17
	menu.ShowBitmap();
	Button_Ballitsa.OnShow();
	Button_Start.OnShow();
	if(Button_Ballitsa.ballitsa != NULL ) Button_Ballitsa.ballitsa->OnShow();
	//ballitsa.OnShow();
	//eraser.OnShow();
	//bball.OnShow();

}
}