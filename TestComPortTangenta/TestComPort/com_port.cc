#include "stdafx.h"
#include "com_port.h"

void ComPort::ComPortInit(std::wstring com_port_name, unsigned long baud_rate, CTestComPortDlg* parent_dlg) {
  com_port_name_ = com_port_name;
  baud_rate_ = baud_rate;
  bytes_received_ = 0;
  parent_dlg_ = parent_dlg;
}

ComPort::ComPort() {
  buffer_ = NULL;
  int_buffer_ = NULL;
  buffer_ = new unsigned char[56];
  memset(buffer_, 0, 56);

}

ComPort::~ComPort() {
  delete[] buffer_;
}

void ComPort::OnComPortReceiveData() { //working from thread
//  parent_dlg_->x_coor_out = receive_buffer_[1];
  memset(this->buffer_, 0, 56);
  for (int i = 0; i < 200; i++) {
    if (receive_buffer_[i] == 0xF4){
      if (receive_buffer_[i + 1] == 1) {
        if (receive_buffer_[i + 2] == 0) {
          if (receive_buffer_[i + 3] == 0) {
            memcpy(this->buffer_, &(receive_buffer_[i]), 56);
//            parent_dlg_->int_buffer_ = reinterpret_cast<int*>(parent_dlg_->buffer_);
            this->int_buffer_ = (unsigned int*) this->buffer_;
//            parent_dlg_->x_coor_out = parent_dlg_->int_buffer_[0];
			parent_dlg_->x_coor_out = this->int_buffer_[11];
            parent_dlg_->UpdData(false, parent_dlg_);
			break;
//            void * memcpy ( void * destination, const void * source, size_t num );
//            Copy block of memory
//            Copies the values of num bytes from the location pointed by source directly to the memory block pointed by destination.
          }
        }
      }
    }
  }
}