
// TestComPortDlg.cpp : файл реализации
//

#include "stdafx.h"
#include "TestComPort.h"
#include "TestComPortDlg.h"
#include "afxdialogex.h"
//#include "com_port.h"
#include "custom_com_port_class.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define SEND_UPDATE_DATA_MSG WM_USER+310

#define SQUARE_SIDE 10

#define Y_MIN 0

//#define Y_MAX 160
#define Y_MAX 320

#define X_MIN 0

//#define X_MAX 300
#define X_MAX 600

typedef struct {
  int x;
  int y;
} top_left, bottom_right;

typedef struct {
  top_left      top_left_corner_;
  bottom_right  bottom_right_corner_;
} area;

typedef struct {
  area top_area_;
  area right_area_;
  area bottom_area_;
} round_gesture_pattern;

area gesture_pattern[9];

void ZeroGesturePattern(area* gesture_pattern);

void FormGesturePattern(area* gesture_pattern, long x, long y);



void ZeroRoundGesturePattern(round_gesture_pattern* a);

void FormRoundGesturePattern(round_gesture_pattern* a, long x, long y);

bool IsPointInsideArea(area* area_, long x, long y);

bool flag_area1 = false;
bool flag_area2 = false;
bool flag_area3 = false;

void ProcessGesture2(round_gesture_pattern* a, long x, long y, CTestComPortDlg* dlg);


// диалоговое окно CTestComPortDlg


//ComPort g_com_port;
CustomComPort g_com;
round_gesture_pattern rnd_gstre_pat;

CTestComPortDlg::CTestComPortDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTestComPortDlg::IDD, pParent)
	, x_coor_out(0)
	, prev_x_coor_out(0)
	, diff_x(0)
	, start_x_coor(0)
	, end_x_coor(0)
	, y_coor_out(0)
	, prev_y_coor_out(0)
	, diff_y(0)
	, start_y_coor(0)
	, end_y_coor(0)
	, recv_coor_counter(0)
	, gesture_result(0)
	, curr_direction(kNoChange)
	, prev_direction(kNoChange)
	, curr_gesture_length(0)
	, gesture_goes_on(false)
	, gesture_finished(false)
	, curr_direction_x(kNoChange)
	, curr_direction_y(kNoChange)
	, prev_direction_x(kNoChange)
	, prev_direction_y(kNoChange)
	, round_flag1(0)
	, round_flag2(0)
	, round_flag3(0)
	, round_flag4(0)
	, round_flag5(0)
	, round_flag6(0)
	, long_click_done(false)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
  g_com.ComPortInit(this);
  ZeroRoundGesturePattern(&rnd_gstre_pat);
  area_flags[0] = false;
  area_flags[1] = false;
  area_flags[2] = false;
  area_flags[3] = false;
  area_flags[4] = false;
  area_flags[5] = false;
  area_flags[6] = false;
  area_flags[7] = false;
  area_flags[8] = false;
}

CTestComPortDlg::~CTestComPortDlg() {

}

void CTestComPortDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_COM_RECV1, x_coor_out);
	DDX_Text(pDX, IDC_PREV_X, prev_x_coor_out);
	DDX_Text(pDX, IDC_DIFF_X, diff_x);
	DDX_Text(pDX, IDC_COM_RECV2, y_coor_out);
	DDX_Text(pDX, IDC_PREV_Y, prev_y_coor_out);
	DDX_Text(pDX, IDC_DIFF_Y, diff_y);
	DDX_Text(pDX, IDC_RECV_COOR_COUNTER, recv_coor_counter);
	DDX_Text(pDX, IDC_START_X, start_x_coor);
	DDX_Text(pDX, IDC_END_X, end_x_coor);
	DDX_Text(pDX, IDC_START_Y, start_y_coor);
	DDX_Text(pDX, IDC_END_Y, end_y_coor);
	DDX_Text(pDX, IDC_GESTURE_RESULT, gesture_result);
	DDX_Text(pDX, AREA0, area_flags[0]);
	DDX_Text(pDX, AREA1, area_flags[1]);
	DDX_Text(pDX, AREA2, area_flags[2]);
	DDX_Text(pDX, AREA3, area_flags[3]);
	DDX_Text(pDX, AREA4, area_flags[4]);
	DDX_Text(pDX, AREA5, area_flags[5]);
	DDX_Text(pDX, AREA6, area_flags[6]);
	DDX_Text(pDX, AREA7, area_flags[7]);
	DDX_Text(pDX, AREA8, area_flags[8]);
}

BEGIN_MESSAGE_MAP(CTestComPortDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(SEND_UPDATE_DATA_MSG, SendUpdateData)
END_MESSAGE_MAP()


// обработчики сообщений CTestComPortDlg

BOOL CTestComPortDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Задает значок для этого диалогового окна. Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию
	g_com.ComPortOpen();
//	g_com.ComPortOpen();

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок. Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CTestComPortDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CTestComPortDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CTestComPortDlg::UpdData(bool a, CTestComPortDlg* dlg) {
  dlg->SendMessage(SEND_UPDATE_DATA_MSG, a);
}

LRESULT CTestComPortDlg::SendUpdateData(WPARAM wp, LPARAM lp) {
//  x_coor_out = x_coor_out * 2;
//  y_coor_out = y_coor_out * 2;
  if ((x_coor_out != 0)  || (y_coor_out != 0)) {  // Non zero coordinates came
    recv_coor_counter++;
    SetCursorPos((int) x_coor_out, (int) (1080 - y_coor_out));
    if ((prev_x_coor_out == 0) && (prev_y_coor_out == 0)) {
      gesture_goes_on = true;
      DoIfEnteredZone();
    }
    ProcessGesture4();
//    ProcessGesture2(&rnd_gstre_pat, x_coor_out, y_coor_out,this);
//    if (!(gesture_finished == true)) {
//      ProcessGesture3(x_coor_out, y_coor_out);
//    }
  } else if ((x_coor_out == 0)  && (y_coor_out == 0)) {
//    recv_coor_counter = 0;
    if ((prev_x_coor_out != 0) || (prev_y_coor_out != 0)) {
      gesture_goes_on = false;
      DoIfLeftZone();
      recv_coor_counter = 0;
    }
  }

  UpdateData(wp);
  return 0;
}

void CTestComPortDlg::DoIfEnteredZone() {
  long_click_done = false;
  gesture_result = 0;
  round_flag1 = 0;
  round_flag2 = 0;
  round_flag3 = 0;
  round_flag4 = 0;
  round_flag5 = 0;
  round_flag6 = 0;
  prev_direction_x = kNoChange;
  prev_direction_y = kNoChange;
  curr_direction_x = kNoChange;
  curr_direction_y = kNoChange;
  start_x_coor = x_coor_out;
  start_y_coor = y_coor_out;
  ZeroGesturePattern(gesture_pattern);
  FormGesturePattern(gesture_pattern, start_x_coor, start_y_coor);
  gesture_finished = false;
//  ZeroRoundGesturePattern(&rnd_gstre_pat);
//  FormRoundGesturePattern(&rnd_gstre_pat, start_x_coor, start_y_coor);
}

void CTestComPortDlg::DoIfLeftZone() {
  end_x_coor = prev_x_coor_out;
  end_y_coor = prev_y_coor_out;
//  curr_direction = kNoChange;  // Don't need right now
//  curr_gesture_length = 0;
  ClearAllZoneFlags();
  ZeroGesturePattern(gesture_pattern);
   if (gesture_result != kRound) {
     if (gesture_result != kRoundCounterClockWise) {
       if (gesture_result != kLongClick) {
       //       ComputeBasicGestures();
       ComputeBasicGestures1();
       }
     }
   }// else {
//     flag_area1 = false;
//     flag_area2 = false;
//     flag_area3 = false;
//   }
}

void CTestComPortDlg::ProcessGesture() {
  if ((diff_x > 0) && (gesture_goes_on == true)) {
    curr_direction = kRight;
    curr_gesture_length = curr_gesture_length + (x_coor_out - start_x_coor);
    if (curr_gesture_length >= 25) {
      gesture_result = kRight;
      curr_gesture_length = 0;
      gesture_goes_on = false;
    }
  }

  if ((diff_x < 0) && (gesture_goes_on == true)) {
    curr_direction = kLeft;
    curr_gesture_length = curr_gesture_length + (start_x_coor - x_coor_out);
    if (curr_gesture_length >= 25) {
      gesture_result = kLeft;
      curr_gesture_length = 0;
      gesture_goes_on = false;
    }
  }

  if ((diff_y > 0) && (gesture_goes_on == true)) {
    curr_direction = kUp;
    curr_gesture_length = curr_gesture_length + (y_coor_out - start_y_coor);
    if (curr_gesture_length >= 15) {
      gesture_result = kUp;
      curr_gesture_length = 0;
      gesture_goes_on = false;
    }
  }

  if ((diff_y < 0) && (gesture_goes_on == true)) {
    curr_direction = kDown;
    curr_gesture_length = curr_gesture_length + (start_y_coor - y_coor_out);
    if (curr_gesture_length >= 15) {
      gesture_result = kDown;
      curr_gesture_length = 0;
      gesture_goes_on = false;
    }
  }
}

void CTestComPortDlg::ProcessGesture1() {
  prev_direction = curr_direction;
  curr_direction = ComputeCurrentDirection();
  if (curr_direction == prev_direction) {
    gesture_goes_on = true;
    if (ComputePathLength(curr_direction) >= 20) {
    gesture_result = curr_direction;
    curr_gesture_length = 0;
    gesture_goes_on = false;
    }
  }
  
}

long CTestComPortDlg::ComputeCurrentDirection() {
  long result = kNoChange;
  if ((diff_x > 10) && (Abs(diff_y) < error_y)) {
    result = kRight;
  } else if ((diff_x < -10) && (Abs(error_y) < error_y)) {
    result = kLeft;
  } else if ((diff_y > 10) && (Abs(diff_x) < error_x)) {
    result = kUp;
  } else if ((diff_y < -10) && (Abs(diff_x) < error_x)) {
    result = kDown;
  }
  return result;
}

long CTestComPortDlg::ComputePathLength(long direction) {
  switch (direction)
  {
  case kUp:
    curr_gesture_length = curr_gesture_length + (y_coor_out - start_y_coor);
    break;
  case kDown:
    curr_gesture_length = curr_gesture_length + (start_y_coor - y_coor_out);
    break;
  case kLeft:
  curr_gesture_length = curr_gesture_length + (start_x_coor - x_coor_out);
    break;
  case kRight:
  curr_gesture_length = curr_gesture_length + (x_coor_out - start_x_coor);
    break;
  }
  return curr_gesture_length;
}

unsigned long CTestComPortDlg::Abs(long a) {
  if (a >= 0) {
    return a;
  } else {
    return (~a + 1);
  }
}


void ZeroArea(area* a) {
  a->top_left_corner_.x = 0;
  a->top_left_corner_.y = 0;
  a->bottom_right_corner_.x = 0;
  a->bottom_right_corner_.y = 0;
}

void ZeroRoundGesturePattern(round_gesture_pattern* a) {
  ZeroArea(&(a->top_area_));
  ZeroArea(&(a->right_area_));
  ZeroArea(&(a->bottom_area_));
}


//void FormRoundGesturePattern(round_gesture_pattern* a, long x, long y) {
//  a->top_area_.top_left_corner_.x     = x - SQUARE_SIDE / 2;
//  a->top_area_.top_left_corner_.y     = y + SQUARE_SIDE / 2;
//  a->top_area_.bottom_right_corner_.x = a->top_area_.top_left_corner_.x + SQUARE_SIDE;
//  a->top_area_.bottom_right_corner_.y = a->top_area_.top_left_corner_.y - SQUARE_SIDE;
///****************************************************************************************/
//  a->right_area_.top_left_corner_.x     = a->top_area_.bottom_right_corner_.x;
//  a->right_area_.top_left_corner_.y     = a->top_area_.bottom_right_corner_.y;
//  a->right_area_.bottom_right_corner_.x = a->right_area_.top_left_corner_.x + SQUARE_SIDE;
//  a->right_area_.bottom_right_corner_.y = a->right_area_.top_left_corner_.y - SQUARE_SIDE;
///****************************************************************************************/
//  a->bottom_area_.top_left_corner_.x     = a->top_area_.top_left_corner_.x;
//  a->bottom_area_.top_left_corner_.y     = a->right_area_.bottom_right_corner_.y;
//  a->bottom_area_.bottom_right_corner_.x = a->bottom_area_.top_left_corner_.x + SQUARE_SIDE;
//  a->bottom_area_.bottom_right_corner_.y = a->bottom_area_.top_left_corner_.y - SQUARE_SIDE;
//}

void FormRoundGesturePattern(round_gesture_pattern* a, long x, long y) {
  a->top_area_.top_left_corner_.x     = x - SQUARE_SIDE / 2;
  a->top_area_.top_left_corner_.y     = y + SQUARE_SIDE / 2;
  a->top_area_.bottom_right_corner_.x = a->top_area_.top_left_corner_.x + SQUARE_SIDE;
  a->top_area_.bottom_right_corner_.y = a->top_area_.top_left_corner_.y - SQUARE_SIDE;
  /****************************************************************************************/
  a->right_area_.top_left_corner_.x     = a->top_area_.bottom_right_corner_.x;
  a->right_area_.top_left_corner_.y     = a->top_area_.bottom_right_corner_.y;
  a->right_area_.bottom_right_corner_.x = X_MAX;//a->right_area_.top_left_corner_.x + SQUARE_SIDE;
  a->right_area_.bottom_right_corner_.y = a->right_area_.top_left_corner_.y - SQUARE_SIDE;
  /****************************************************************************************/
  a->bottom_area_.top_left_corner_.x     = X_MIN;//a->top_area_.top_left_corner_.x;
  a->bottom_area_.top_left_corner_.y     = a->right_area_.bottom_right_corner_.y;
  a->bottom_area_.bottom_right_corner_.x = a->top_area_.bottom_right_corner_.x;//a->bottom_area_.top_left_corner_.x + SQUARE_SIDE;
  a->bottom_area_.bottom_right_corner_.y = Y_MIN;//a->bottom_area_.top_left_corner_.y - SQUARE_SIDE;
}



bool IsPointInsideArea(area* area_, long x, long y) {
  if ((x > area_->top_left_corner_.x) && (x < area_->bottom_right_corner_.x) && (y > area_->bottom_right_corner_.y) && (y < area_->top_left_corner_.y)) {
    return true;
  } else return false;
}

void CTestComPortDlg::ComputeBasicGestures() {
// 	control_command_ = 0;
// 	delta_x_ = end_coordinates_.x - start_coordinates_.x;
// 	delta_y_ = ~(end_coordinates_.y - start_coordinates_.y) + 1;
// 
// 	if (delta_y_ > 0) {
// 		y_axis_direction_ = kUp;
// 	} else if (delta_y_ < 0) {
// 		y_axis_direction_ = kDown;
// 	} else {
// 		y_axis_direction_ = kNoChange;
// 	}
// 	if (delta_x_ > 0) {
// 		x_axis_direction_ = kRight;
// 	} else if (delta_x_ < 0) {
// 		x_axis_direction_ = kLeft;
// 	} else {
// 		x_axis_direction_ = kNoChange;
// 	}
// 
// 
// 
// 	if ((delta_x_ < error_x_) && (delta_x_ > -error_x_)) {
// 		if (y_axis_direction_ == kUp) {
// 			control_command_ = kGestureUp;
// 		} else if (y_axis_direction_ == kDown) {
// 			control_command_ = kGestureDown;
// 		}
// 	}
// 
// 	if ((delta_y_ < error_y_) && (delta_y_ > -error_y_)) {
// 		if (x_axis_direction_ == kRight) {
// 			control_command_ = kGestureRight;
// 		} else if (x_axis_direction_ == kLeft) {
// 			control_command_ = kGestureLeft;
// 		}
// 	}
// 	CString message = _T("");
// 	switch (control_command_)
// 	{
// 	case 1: message = _T("Gesture Up");
// 		break;
// 	case 2: message = _T("Gesture Down");
// 		break;
// 	case 3: message = _T("Gesture Left");
// 		break;
// 	case 4: message = _T("Gesture Right");
// 		break;
// 	case 0: message = _T("No Gesture");
// 		break;
// 	}
// 	AfxMessageBox(message);
// 	int a = 0;

  long control_command = 0;
  long delta_x = end_x_coor - start_x_coor;
  long delta_y = end_y_coor - start_y_coor;
  long x_axis_direction = kNoChange;
  long y_axis_direction = kNoChange;

  if (delta_y > 0) {
    y_axis_direction = kUp;
  } else if (delta_y < 0) {
    y_axis_direction = kDown;
  } else {
    y_axis_direction = kNoChange;
  }

  if (delta_x > 0) {
    x_axis_direction = kRight;
  } else if (delta_x < 0) {
    x_axis_direction = kLeft;
  } else {
    x_axis_direction = kNoChange;
  }

  if ((delta_x < error_x) && (delta_x > -error_x)) {
    if (y_axis_direction == kUp) {
//    gesture_result = kUp;
      control_command = kGestureUp;
    } else if (y_axis_direction == kDown) {
//    gesture_result = kDown;
      control_command = kGestureDown;
    }
  }

  if ((delta_y < error_y) && (delta_y > -error_y)) {
    if (x_axis_direction == kRight) {
//    gesture_result = kRight;
      control_command = kGestureRight;
    } else if (x_axis_direction == kLeft) {
//    gesture_result = kLeft;
      control_command = kGestureLeft;
    }
  }

  if ((delta_x < error_x) && (delta_x > -error_x)) {
    if ((delta_y < error_y) && (delta_y > -error_y)) {
      if (recv_coor_counter >= 8) {
        control_command = kGestureLongClick;
      } else {
        control_command = kGestureClick;
      }
    }
  }

  switch (control_command) {
    case kGestureUp:
      gesture_result = kUp;
      break;
    case kGestureDown:
      gesture_result = kDown;
      break;
    case kGestureLeft:
      gesture_result = kLeft;
      break;
    case kGestureRight:
      gesture_result = kRight;
      break;
    case kGestureClick:
      gesture_result = kClick;
      break;
    case kGestureLongClick:
      gesture_result = kLongClick;
      break;
    case 0:
      gesture_result = 0;
      break;
  }




}

void ProcessGesture2(round_gesture_pattern* a, long x, long y, CTestComPortDlg* dlg) {
  if (IsPointInsideArea(&(a->top_area_), x, y)) {
    flag_area1 = true;
  }
  if ((flag_area1 == true) && (IsPointInsideArea(&(a->right_area_), x, y))) {
    flag_area2 = true;
  }
  if ((flag_area1 == true) && (flag_area2 == true) && (IsPointInsideArea(&(a->bottom_area_), x, y))) {
    flag_area3 = true;
    dlg->gesture_result = kRound;
  }
}


void FormGesturePattern(area* gesture_pattern, long x, long y) {
//150 90
  gesture_pattern[0].top_left_corner_.x     = x - SQUARE_SIDE / 2;//145
  gesture_pattern[0].top_left_corner_.y     = y + SQUARE_SIDE / 2;//95
  gesture_pattern[0].bottom_right_corner_.x = gesture_pattern[0].top_left_corner_.x + SQUARE_SIDE;//155
  gesture_pattern[0].bottom_right_corner_.y = gesture_pattern[0].top_left_corner_.y - SQUARE_SIDE;//85
/************************************************************************************************/
  gesture_pattern[1].top_left_corner_.x     = gesture_pattern[0].top_left_corner_.x + (SQUARE_SIDE*2);
  gesture_pattern[1].top_left_corner_.y     = gesture_pattern[0].top_left_corner_.y;
  gesture_pattern[1].bottom_right_corner_.x = gesture_pattern[0].bottom_right_corner_.x + (SQUARE_SIDE*3);
  gesture_pattern[1].bottom_right_corner_.y = gesture_pattern[0].bottom_right_corner_.y;
/************************************************************************************************/
  gesture_pattern[2].top_left_corner_.x     = gesture_pattern[0].top_left_corner_.x + (SQUARE_SIDE*5);
  gesture_pattern[2].top_left_corner_.y     = gesture_pattern[0].top_left_corner_.y;
  gesture_pattern[2].bottom_right_corner_.x = gesture_pattern[0].bottom_right_corner_.x +(SQUARE_SIDE*6);
  gesture_pattern[2].bottom_right_corner_.y = gesture_pattern[0].bottom_right_corner_.y;
/************************************************************************************************/
  gesture_pattern[3].top_left_corner_.x     = gesture_pattern[0].top_left_corner_.x - (SQUARE_SIDE*3);
  gesture_pattern[3].top_left_corner_.y     = gesture_pattern[0].top_left_corner_.y;
  gesture_pattern[3].bottom_right_corner_.x = gesture_pattern[0].bottom_right_corner_.x - (SQUARE_SIDE*2);
  gesture_pattern[3].bottom_right_corner_.y = gesture_pattern[0].bottom_right_corner_.y;
  /************************************************************************************************/
  gesture_pattern[4].top_left_corner_.x     = gesture_pattern[0].top_left_corner_.x - (SQUARE_SIDE*6);
  gesture_pattern[4].top_left_corner_.y     = gesture_pattern[0].top_left_corner_.y;
  gesture_pattern[4].bottom_right_corner_.x = gesture_pattern[0].bottom_right_corner_.x - (SQUARE_SIDE*5);
  gesture_pattern[4].bottom_right_corner_.y = gesture_pattern[0].bottom_right_corner_.y;
  /************************************************************************************************/
  gesture_pattern[5].top_left_corner_.x     = gesture_pattern[0].top_left_corner_.x;
//  gesture_pattern[5].top_left_corner_.y     = gesture_pattern[0].top_left_corner_.y - (SQUARE_SIDE*2);
  gesture_pattern[5].top_left_corner_.y     = gesture_pattern[0].top_left_corner_.y - 15;
  gesture_pattern[5].bottom_right_corner_.x = gesture_pattern[0].bottom_right_corner_.x;
  gesture_pattern[5].bottom_right_corner_.y = Y_MIN;
  /************************************************************************************************/
  gesture_pattern[6].top_left_corner_.x     = gesture_pattern[0].top_left_corner_.x;
  gesture_pattern[6].top_left_corner_.y     = Y_MAX;
  gesture_pattern[6].bottom_right_corner_.x = gesture_pattern[0].bottom_right_corner_.x;
  gesture_pattern[6].bottom_right_corner_.y = gesture_pattern[0].bottom_right_corner_.y + (SQUARE_SIDE*2);
  /************************************************************************************************/
//  gesture_pattern[7].top_left_corner_.x     = gesture_pattern[0].top_left_corner_.x + (SQUARE_SIDE*2);
  gesture_pattern[7].top_left_corner_.x     = gesture_pattern[0].top_left_corner_.x + SQUARE_SIDE;
//  gesture_pattern[7].top_left_corner_.y     = gesture_pattern[0].top_left_corner_.y - (SQUARE_SIDE*2);
  gesture_pattern[7].top_left_corner_.y     = gesture_pattern[0].top_left_corner_.y - SQUARE_SIDE;
  gesture_pattern[7].bottom_right_corner_.x = X_MAX;
//  gesture_pattern[7].bottom_right_corner_.y = gesture_pattern[0].bottom_right_corner_.y - (SQUARE_SIDE*2);
  gesture_pattern[7].bottom_right_corner_.y = gesture_pattern[0].bottom_right_corner_.y - SQUARE_SIDE;
  /************************************************************************************************/
  gesture_pattern[8].top_left_corner_.x     = X_MIN;
//  gesture_pattern[8].top_left_corner_.y     = gesture_pattern[0].top_left_corner_.y - (SQUARE_SIDE*2);
  gesture_pattern[8].top_left_corner_.y     = gesture_pattern[0].top_left_corner_.y - SQUARE_SIDE;
//  gesture_pattern[8].bottom_right_corner_.x = gesture_pattern[0].bottom_right_corner_.x - (SQUARE_SIDE*2);
  gesture_pattern[8].bottom_right_corner_.x = gesture_pattern[0].bottom_right_corner_.x - SQUARE_SIDE;
//  gesture_pattern[8].bottom_right_corner_.y = gesture_pattern[0].bottom_right_corner_.y - (SQUARE_SIDE*2);
  gesture_pattern[8].bottom_right_corner_.y = gesture_pattern[0].bottom_right_corner_.y - SQUARE_SIDE;
  /************************************************************************************************/
}


void ZeroGesturePattern(area* gesture_pattern) {
  for (int i = 0; i < 9; i++) {
    ZeroArea(&(gesture_pattern[i]));
  }
}

void CTestComPortDlg::ClearAllZoneFlags() {
  for (int i = 0; i < 9; i++) {
    area_flags[i] = false;
  }
}

void CTestComPortDlg::ProcessGesture3(long x, long y) {
  for (int i = 0; i < 9; i++) {
    if (IsPointInsideArea(&(gesture_pattern[i]), x,y)) {
      area_flags[i] = true;
    }
  }
  gesture_result = DetermineGestureByFlags();
}


long CTestComPortDlg::DetermineGestureByFlags() {
  if ((area_flags[0] == true) && (area_flags[1] == true) && (area_flags[2] == true)) {
    ClearAllZoneFlags();
    gesture_finished = true;
    return kRight;
  }
/******************************************/
  if ((area_flags[0] == true) && (area_flags[3] == true) && (area_flags[4] == true)) {
    ClearAllZoneFlags();
    gesture_finished = true;
    return kLeft;
  }
/******************************************/
  if ((area_flags[0] == true) && (area_flags[6] == true)) {
    ClearAllZoneFlags();
    gesture_finished = true;
    return kUp;
  }
/******************************************/
  if ((area_flags[0] == true) && (area_flags[5] == true)) {
    if ((area_flags[7] != true) && (area_flags[8] != true)) {
      ClearAllZoneFlags();
      gesture_finished = true;
      return kDown;
    }
  }
/******************************************/
  if ((area_flags[0] == true) && (area_flags[7] == true) && (area_flags[5] == true) && (area_flags[8] == true)) {
    ClearAllZoneFlags();
    gesture_finished = true;
    return kRound;
  }
  return 0;
}

void CTestComPortDlg::ProcessGesture4() {
  prev_direction_x = curr_direction_x;
  prev_direction_y = curr_direction_y;
  long delta_x = x_coor_out - start_x_coor;
  long delta_y = y_coor_out - start_y_coor;

  if (recv_coor_counter == 3) {
    if ((delta_x < 5) && (delta_x > -5)) {
      if ((delta_y < 5) && (delta_y > -5)) {
      long_click_done = true;
      gesture_result = kLongClick;
      }
    }
  }

  if (long_click_done != true) {
	  ComputeCurrentDirection1();

	  if ((prev_direction_x == kRight) && (curr_direction_x == kRight) && (prev_direction_y == kUp) && (curr_direction_y == kDown)) {
		  round_flag1 = 1;
	  }
	  if (round_flag1 == 1) {
		  if ((prev_direction_x == kRight) && (curr_direction_x == kLeft) && (prev_direction_y == kDown) && (curr_direction_y == kDown)) {
			  round_flag2 = 1;
		  }
	  }
	  if (round_flag2 == 1){
		  if ((prev_direction_x == kLeft) && (curr_direction_x == kLeft) && (prev_direction_y == kDown) && (curr_direction_y == kUp)) {
			  round_flag3 = 1;
		  }
	  }
	  if (round_flag3 == 1) {
		  if ((prev_direction_x == kLeft) && (curr_direction_x == kRight) && (prev_direction_y == kUp) && (curr_direction_y == kUp)) {
			  gesture_result = kRound;
			  round_flag1 = 0;
			  round_flag2 = 0;
			  round_flag3 = 0;
		  }
	  }




	  if ((prev_direction_x == kLeft) && (curr_direction_x == kLeft) && (prev_direction_y == kUp) && (curr_direction_y == kDown)) {
		  round_flag4 = 1;
	  }

	  if (round_flag4 == 1) {
		  if ((prev_direction_x == kLeft) && (curr_direction_x == kRight) && (prev_direction_y == kDown) && (curr_direction_y == kDown)) {
			  round_flag5 = 1;
		  }
	  }

	  if (round_flag5 == 1){
		  if ((prev_direction_x == kRight) && (curr_direction_x == kRight) && (prev_direction_y == kDown) && (curr_direction_y == kUp)) {
			  round_flag6 = 1;
		  }
	  }

	  if (round_flag6 == 1) {
		  if ((prev_direction_x == kRight) && (curr_direction_x == kLeft) && (prev_direction_y == kUp) && (curr_direction_y == kUp)) {
			  gesture_result = kRoundCounterClockWise;
			  round_flag4 = 0;
			  round_flag5 = 0;
			  round_flag6 = 0;
		  }
	  }
  }

}

void CTestComPortDlg::ComputeCurrentDirection1() {

  if (diff_x > 0) {
    curr_direction_x = kRight;
  } else if (diff_x < 0) {
    curr_direction_x = kLeft;
  } else { //diff_x == 0
//    curr_direction_x = kNoChange;
  }

  if (diff_y > 0) {
    curr_direction_y = kUp;
  } else if (diff_y < 0) {
    curr_direction_y = kDown;
  } else { //diff_y == 0
//    curr_direction_y = kNoChange;
  }

}

void CTestComPortDlg::ComputeBasicGestures1() {
  long control_command = 0;
  long delta_x = end_x_coor - start_x_coor;
  long delta_y = end_y_coor - start_y_coor;
  long x_axis_direction = kNoChange;
  long y_axis_direction = kNoChange;

  if (Abs(delta_x) > Abs(delta_y)) {
    if (delta_x > 0 ) {
      x_axis_direction = kRight;
      gesture_result = kRight;
    } else if (delta_x < 0) {
      x_axis_direction = kLeft;
      gesture_result = kLeft;
    } else {
      x_axis_direction = kNoChange;
      gesture_result = 0;
    }
  } else if (Abs(delta_y) > Abs(delta_x)) {
    if (delta_y > 0 ) {
      y_axis_direction = kUp;
      gesture_result = kUp;
    } else if (delta_y < 0) {
      y_axis_direction = kDown;
      gesture_result = kDown;
    } else {
      y_axis_direction = kNoChange;
      gesture_result = 0;
    }
  }

  if ((delta_x < error_x) && (delta_x > -error_x)) {
    if ((delta_y < error_y) && (delta_y > -error_y)) {
//      if (recv_coor_counter >= 3) {
//        control_command = kGestureLongClick;
//        gesture_result = kLongClick;
//      } else {
        control_command = kGestureClick;
        gesture_result = kClick;
//      }
    }
  }


//  if ((delta_x < error_x) && (delta_x > -error_x)) {
//    if ((delta_y < error_y) && (delta_y > -error_y)) {
//      if (recv_coor_counter >= 8) {
//        control_command = kGestureLongClick;
//      } else {
//        control_command = kGestureClick;
//      }
//    }
//  }

//  switch (control_command) {
//    case kGestureUp:
//      gesture_result = kUp;
//      break;
//    case kGestureDown:
//      gesture_result = kDown;
//      break;
//    case kGestureLeft:
//      gesture_result = kLeft;
//      break;
//    case kGestureRight:
//      gesture_result = kRight;
//      break;
//    case kGestureClick:
//      gesture_result = kClick;
//      break;
//    case kGestureLongClick:
//      gesture_result = kLongClick;
//      break;
//    case 0:
//      gesture_result = 0;
//      break;
//  }

}

