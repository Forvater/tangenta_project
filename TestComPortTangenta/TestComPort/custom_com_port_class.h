#include "TestComPortDlg.h"
#include <string>

enum ConnectionErrors {
  kOK = 0,
  kSendError,
  kOpenPortFailed,
  kDcbReadFailed,
  kDcbSetFailed,
  kTimeoutsSetFailed,
};

class CustomComPort {
 public:
  CustomComPort(): com_port_name_(L"COM1"), baud_rate_(57600), bytes_received_(0)
  {
	  buffer_ = NULL;
	  int_buffer_ = NULL;
	  buffer_ = new unsigned char[12];
	  memset(buffer_, 0, 12);
	  memset(receive_buffer1_, 0, kReceiveBufferSize1);
  }
  CustomComPort(std::wstring com_port_name, unsigned long baud_rate)
  : com_port_name_(com_port_name)
  , baud_rate_(baud_rate)
  , bytes_received_(0)
  {}
  ~CustomComPort() {}

  int ComPortOpen();
  int ComPortConfigure();
  void ComPortClose();
  virtual void OnComPortReceiveData();
  void ComPortInit(CTestComPortDlg* parent_dlg);

  CTestComPortDlg* parent_dlg_;
  unsigned char* buffer_;
  unsigned int* int_buffer_;
  unsigned int buffer_position_;

 protected:
  static const unsigned long kReceiveBufferSize = 256;
  static const unsigned long kReceiveBufferSize1 = 50;

  // Default values for the DCB structure
  static const unsigned long kfBinary = true;
  static const unsigned long kfOutxCtsFlow = false;
  static const unsigned long kfOutxDsrFlow = false;
  static const unsigned long kfDtrControl = DTR_CONTROL_DISABLE;
  static const unsigned long kfDsrSensitivity = false;
  static const unsigned long kfNull = false;
  static const unsigned long kfRtsControl = RTS_CONTROL_DISABLE;
  static const unsigned long kfAbortOnError = false;
  static const unsigned char kByteSize = 8;
  static const unsigned char kParity = 1;
  static const unsigned char kStopBits = 0;

  // ComPort internal buffer size
  static const unsigned long kInputBufferSize = 2000;
  static const unsigned long kOutputBufferSize = 2000;

  // Set default values for the DCB structure
  void set_device_control_block();
  void set_timeouts();
  static unsigned long __stdcall ReadFromComPortInThread(void* p_param);
  static unsigned long __stdcall WriteToComPortInThread(void* p_param);

  unsigned char receive_buffer_[kReceiveBufferSize];
  unsigned char receive_buffer1_[kReceiveBufferSize1];
  unsigned long bytes_received_;
  HANDLE com_port_handle_;
  HANDLE read_tread_handle_;
  HANDLE write_tread_handle_;
  std::wstring com_port_name_;
  unsigned long baud_rate_;
  DCB device_control_block_;
  COMMTIMEOUTS timeouts_;
};