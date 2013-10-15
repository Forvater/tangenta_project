#include "Resource.h"
// TestComPortDlg.h : файл заголовка
//

enum Directions {
  kUp = 1,
  kDown,
  kLeft,
  kRight,
  kNoChange,
  kRound,
  kRoundCounterClockWise,
  kClick,
  kLongClick
};

enum ControlCommands {
  kGestureUp = 1,
  kGestureDown,
  kGestureLeft,
  kGestureRight,
  kGestureNoChange,
  kGestureClick,
  kGestureLongClick
};

#pragma once


// диалоговое окно CTestComPortDlg
class CTestComPortDlg : public CDialogEx
{
// Создание
 public:
	CTestComPortDlg(CWnd* pParent = NULL);	// стандартный конструктор
	~CTestComPortDlg();
	void UpdData(bool a, CTestComPortDlg* dlg);
  afx_msg LRESULT SendUpdateData(WPARAM wp, LPARAM lp);


// Данные диалогового окна
	enum { IDD = IDD_TESTCOMPORT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
 protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
 public:
  long x_coor_out;
  long prev_x_coor_out;
  long start_x_coor;
  long end_x_coor;
  long y_coor_out;
  long prev_y_coor_out;
  long start_y_coor;
  long end_y_coor;
  long recv_coor_counter;
  long gesture_result;
  long diff_x;
  long diff_y;
 public:
  void DoIfEnteredZone();  // Remember start coordinates
  void DoIfLeftZone();  // Remember end coordinates
  void ProcessGesture();
  void ProcessGesture1();
  long ComputeCurrentDirection();
  long ComputePathLength(long direction);
 public:
  long curr_direction;
  long prev_direction;
  long curr_gesture_length;
  bool gesture_goes_on;
  static const int error_x = 5;
  static const int error_y = 5;  // was 20
  unsigned long Abs(long a);
  void ComputeBasicGestures();
  void ComputeBasicGestures1();
  int area_flags[9];
  void ClearAllZoneFlags();
  void ProcessGesture3(long x, long y);
  long DetermineGestureByFlags();
  bool gesture_finished;
  void ProcessGesture4();
  void ComputeCurrentDirection1();
  long curr_direction_x;
  long curr_direction_y;
  long prev_direction_x;
  long prev_direction_y;
  long round_flag1;
  long round_flag2;
  long round_flag3;
  long round_flag4;
  long round_flag5;
  long round_flag6;
  bool long_click_done;
};
