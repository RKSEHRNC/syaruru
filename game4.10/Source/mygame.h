/*
 * mygame.h: ���ɮ��x�C��������class��interface
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

	enum AUDIO_ID {				// �w�q�U�ح��Ī��s��
		AUDIO_DING,				// 0
		AUDIO_LAKE,				// 1
		AUDIO_NTUT				// 2
	};


	///////////////////////
	// �ɶ��Ȱ�! by17
	///////////////////////

	class Timer {
	public:
		bool ifRun = 0;
	};

	//class Ballitsa{};

	//////////////////////////////////////
	// �إ߼ĤH�@�������� by17
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
	// ����Ǫ��ͦ� by 17
	/////////////////////////

	class EnemyCrafting {
	public:
		vector<Enemy01> enemy01;
		void craft();
	};

	//////////////////////////////
	// �l�u by17
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
	//�إߩ������� by17
	///////////////////////////////

	class Ballitsa {
	public:
		vector<Arrow> arrow;
		Ballitsa(CPoint p);
		bool state = 0; //0 = �سy��, 1 = �سy����
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
	// �إ߫��s���� by17
	///////////////////////////////////

	class Button {
	public:
		int getX();
		int getY();
	private:
		int x, y;
	};

	class Button_ballitsa : public Button { //�������s
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

	class Button_start : public Button { //�}�l���s
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
	// �o��class���C�����C���}�Y�e������
	// �C��Member function��Implementation���n����
	/////////////////////////////////////////////////////////////////////////////

	class CGameStateInit : public CGameState {
	public:
		CGameStateInit(CGame *g);
		void OnInit();  								// �C������Ȥιϧγ]�w
		void OnBeginState();							// �]�w�C�������һݪ��ܼ�
		void OnKeyUp(UINT, UINT, UINT); 				// �B�z��LUp���ʧ@
		void OnLButtonDown(UINT nFlags, CPoint point);  // �B�z�ƹ����ʧ@
	protected:
		void OnShow();									// ��ܳo�Ӫ��A���C���e��
	private:
		CMovingBitmap logo;								// csie��logo
	};

	/////////////////////////////////////////////////////////////////////////////
	// �o��class���C�����C�����檫��A�D�n���C���{�����b�o��
	// �C��Member function��Implementation���n����
	/////////////////////////////////////////////////////////////////////////////

	class CGameStateRun : public CGameState {
	public:
		CGameStateRun(CGame *g);
		~CGameStateRun();
		void OnBeginState();							// �]�w�C�������һݪ��ܼ�
		void OnInit();  								// �C������Ȥιϧγ]�w
		void OnKeyDown(UINT, UINT, UINT);
		void OnKeyUp(UINT, UINT, UINT);
		void OnLButtonDown(UINT nFlags, CPoint point);  // �B�z�ƹ����ʧ@
		void OnMouseMove(UINT nFlags, CPoint point);	// �B�z�ƹ����ʧ@ 
		bool Onclick(CPoint p, int y);					//�P�_�I���ɪ��ƹ���l
		Timer timer;
		
	protected:
		void OnMove();									// ���ʹC������
		void OnShow();									// ��ܳo�Ӫ��A���C���e��
	private:
		const int		NUMBALLS;	// �y���`��
		CMovingBitmap	background;	// �I����
		CMovingBitmap	menu;		//���I�� by17
		CMovingBitmap	help;		// ������
		CBall			*ball;		// �y���}�C
		CMovingBitmap	corner;		// ������
		CEraser			eraser;		// ��l
		CInteger		hits_left;	// �ѤU��������
		CBouncingBall   bball;		// ���мu�����y
		EnemyCrafting	ENC;		// �ĤH�ͦ��t�� by17
		Cant			cant;		//�T��Ÿ� by17
		Button_ballitsa Button_Ballitsa;//�������s by17
		Button_start	Button_Start;//�}�l���s by17
		bool			buliding = 0;//���m��سy�� by17
		bool			cantbulid = 0;//�o�̤���\ by17
		int				level = 0;
	};

	/////////////////////////////////////////////////////////////////////////////
	// �o��class���C�����������A(Game Over)
	// �C��Member function��Implementation���n����
	/////////////////////////////////////////////////////////////////////////////

	class CGameStateOver : public CGameState {
	public:
		CGameStateOver(CGame *g);
		void OnBeginState();							// �]�w�C�������һݪ��ܼ�
		void OnInit();
	protected:
		void OnMove();									// ���ʹC������
		void OnShow();									// ��ܳo�Ӫ��A���C���e��
	private:
		int counter;	// �˼Ƥ��p�ƾ�
	};

}