#pragma once
#include "common.h"

class CPacketBuffer
{
#define MAXIMUM_BUFFER_SIZE 4096
public:
	CPacketBuffer();
	virtual ~CPacketBuffer();
	int push_back(__in BYTE* _data, __in_opt int _size);
	int pop(__out BYTE* _data, __out_opt int* _size);
private:
	int head;
	int tail;
	int size;
	int use;
	BYTE buffer[MAXIMUM_BUFFER_SIZE];
protected:
};