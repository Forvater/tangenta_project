#include "stdafx.h"
#include "custom_com_port_class.h"

void CustomComPort::set_timeouts() {
  timeouts_.ReadIntervalTimeout = 0;
  timeouts_.ReadTotalTimeoutMultiplier = 0;
  timeouts_.ReadTotalTimeoutConstant = 0;
  timeouts_.WriteTotalTimeoutMultiplier = 0;
  timeouts_.WriteTotalTimeoutConstant = 0;
}

void CustomComPort::set_device_control_block() {
  device_control_block_.BaudRate = baud_rate_;
  device_control_block_.fBinary = kfBinary;
  device_control_block_.fOutxCtsFlow = kfOutxCtsFlow;
  device_control_block_.fOutxDsrFlow = kfOutxDsrFlow;
  device_control_block_.fDtrControl = kfDtrControl;
  device_control_block_.fDsrSensitivity = kfDsrSensitivity;
  device_control_block_.fNull = kfNull;
  device_control_block_.fRtsControl = kfRtsControl;
  device_control_block_.fAbortOnError = kfAbortOnError;
  device_control_block_.ByteSize = kByteSize;
  device_control_block_.Parity = kParity;
  device_control_block_.StopBits = kStopBits;
}

int CustomComPort::ComPortOpen() {
  com_port_handle_ = CreateFile(com_port_name_.c_str(),
                                GENERIC_READ | GENERIC_WRITE,
                                0,
                                NULL,
                                OPEN_EXISTING,
                                FILE_FLAG_OVERLAPPED,
                                NULL);
  if (com_port_handle_ == INVALID_HANDLE_VALUE) {
    return kOpenPortFailed;
  } else {
    return ComPortConfigure();
  }
}

int CustomComPort::ComPortConfigure() {
  device_control_block_.DCBlength = sizeof(DCB);
  // Read DCB Structure from port
  if (!GetCommState(com_port_handle_, &device_control_block_)) {
    ComPortClose();
    return kDcbReadFailed;
  }
  set_device_control_block();
  if (!SetCommState(com_port_handle_, &device_control_block_)) {
    ComPortClose();
    return kDcbSetFailed;
  }
  set_timeouts();
  if (!SetCommTimeouts(com_port_handle_, &timeouts_)) {
    ComPortClose();
    return kTimeoutsSetFailed;
  }
  // Set mask on EV_RXCHAR event
  unsigned long event_mask_to_set = 1;
  SetCommMask(com_port_handle_, event_mask_to_set);
  // Set size of the receive and transmission queue
  SetupComm(com_port_handle_,kInputBufferSize,kOutputBufferSize);
  // Purge the receive buffer
  PurgeComm(com_port_handle_, PURGE_RXCLEAR);
  read_tread_handle_ = CreateThread(NULL, 0, ReadFromComPortInThread, this, 0,
                                    NULL);
  write_tread_handle_ = CreateThread(NULL, 0, WriteToComPortInThread, this,
                                     CREATE_SUSPENDED, NULL);
  return kOK;
}

unsigned long __stdcall CustomComPort::ReadFromComPortInThread(void* p_param) {
  CustomComPort* p_this = static_cast<CustomComPort*>(p_param);
  unsigned long event_mask = 0;
  _OVERLAPPED overlapped_read_structure;
  unsigned long number_of_bytes_transferred = 0;
  unsigned long errors = 0;
  COMSTAT com_status;
  overlapped_read_structure.hEvent = CreateEvent(NULL, true, false, NULL);
  unsigned long wait_result = 1;
  while (true) {
    WaitCommEvent(p_this->com_port_handle_, &event_mask,
                  &overlapped_read_structure);
    wait_result = WaitForSingleObject(overlapped_read_structure.hEvent,
                                    INFINITE);
    if (wait_result == WAIT_OBJECT_0) {
      if (GetOverlappedResult(p_this->com_port_handle_,
                              &overlapped_read_structure,
                              &number_of_bytes_transferred,
                              true)) {
        if (event_mask == 1) {
          ClearCommError(p_this->com_port_handle_, &errors, &com_status);
          if (errors == 0) {
            overlapped_read_structure.Offset = 0;
            overlapped_read_structure.OffsetHigh = 0;
            if (com_status.cbInQue) {
              memset(p_this->receive_buffer_, 0, p_this->kReceiveBufferSize);
              ReadFile(p_this->com_port_handle_, p_this->receive_buffer_,
                       com_status.cbInQue, NULL, &overlapped_read_structure);
              p_this->bytes_received_ = com_status.cbInQue;
//			  if (com_status.cbInQue > 200) {
//				  p_this->OnComPortReceiveData();
//			  } 
//			  Sleep(500);


			  if (com_status.cbInQue >= p_this->kReceiveBufferSize1) { //Filling the buffer
				  memcpy(p_this->receive_buffer1_, p_this->receive_buffer_, p_this->kReceiveBufferSize1);
				  p_this->OnComPortReceiveData();
				  p_this->buffer_position_ = 0;
			  } else if (com_status.cbInQue < p_this->kReceiveBufferSize1) {
				  if ( (p_this->kReceiveBufferSize1 - p_this->buffer_position_) > com_status.cbInQue ) {
					  memcpy(&(p_this->receive_buffer1_[p_this->buffer_position_]), p_this->receive_buffer_, com_status.cbInQue);
					  p_this->buffer_position_ = p_this->buffer_position_ + com_status.cbInQue;
					  if (p_this->buffer_position_ == p_this->kReceiveBufferSize1) {
						  p_this->OnComPortReceiveData();
						  p_this->buffer_position_ = 0;
					  }
				  } else if ( (p_this->kReceiveBufferSize1 - p_this->buffer_position_) <= com_status.cbInQue ) {
					  memcpy(&(p_this->receive_buffer1_[p_this->buffer_position_]), p_this->receive_buffer_, p_this->kReceiveBufferSize1 - p_this->buffer_position_);
					  p_this->buffer_position_ = p_this->buffer_position_ + (p_this->kReceiveBufferSize1 - p_this->buffer_position_);
					  if (p_this->buffer_position_ == p_this->kReceiveBufferSize1) {
						  p_this->OnComPortReceiveData();
						  p_this->buffer_position_ = 0;
					  }
				  }
			  }


            }
          }
        }
      }
    }
  }
  return EXIT_SUCCESS;
}

unsigned long __stdcall CustomComPort::WriteToComPortInThread(void* p_param) {
  CustomComPort* p_this = static_cast<CustomComPort*>(p_param);
  return EXIT_SUCCESS;
}

void CustomComPort::ComPortClose() {
}

void CustomComPort::OnComPortReceiveData() {
	memset(this->buffer_, 0, 12);
	for (int i = 0; i < 38; i++) {
		if (receive_buffer1_[i] == 0xF4){
			if (receive_buffer1_[i + 1] == 1) {
				if (receive_buffer1_[i + 2] == 0) {
					if (receive_buffer1_[i + 3] == 0) {
						memcpy(this->buffer_, &(receive_buffer1_[i]), 12);
						//            parent_dlg_->int_buffer_ = reinterpret_cast<int*>(parent_dlg_->buffer_);
						this->int_buffer_ = (unsigned int*) this->buffer_;
						//            parent_dlg_->x_coor_out = parent_dlg_->int_buffer_[0];
						parent_dlg_->prev_x_coor_out = parent_dlg_->x_coor_out;
						parent_dlg_->prev_y_coor_out = parent_dlg_->y_coor_out;
						parent_dlg_->x_coor_out = this->int_buffer_[1];
						parent_dlg_->y_coor_out = this->int_buffer_[2];
						parent_dlg_->x_coor_out = parent_dlg_->x_coor_out / 100;
						parent_dlg_->y_coor_out = parent_dlg_->y_coor_out / 100;
						parent_dlg_->diff_x = parent_dlg_->x_coor_out - parent_dlg_->prev_x_coor_out;
						parent_dlg_->diff_y = parent_dlg_->y_coor_out - parent_dlg_->prev_y_coor_out;
						parent_dlg_->UpdData(false, parent_dlg_);
						memset(receive_buffer1_, 0, kReceiveBufferSize1);
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

void CustomComPort::ComPortInit(CTestComPortDlg* parent_dlg) {
  parent_dlg_ = parent_dlg;
}