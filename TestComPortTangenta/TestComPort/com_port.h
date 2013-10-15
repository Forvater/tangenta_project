#include "TestComPortDlg.h"
#include "custom_com_port_class.h"


class ComPort :public CustomComPort {
 public:
  ComPort();
  ~ComPort();
  void ComPortInit(std::wstring com_port_name, unsigned long baud_rate, CTestComPortDlg* parent_dlg);
  void OnComPortReceiveData();
  CTestComPortDlg* parent_dlg_;
  unsigned char* buffer_;
  unsigned int* int_buffer_;
};