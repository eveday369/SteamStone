#include "stdAfx.h"
#include "PacketBuffer.h"

CPacketBuffer::CPacketBuffer()
{
	head = 0;
	tail = 0;
	size = MAXIMUM_BUFFER_SIZE;
	use = 0;
	memset(buffer, 0, MAXIMUM_BUFFER_SIZE);
}
CPacketBuffer::~CPacketBuffer()
{
	head = 0;
	tail = 0;
	size = 0;
	use = 0;
	memset(buffer, 0, MAXIMUM_BUFFER_SIZE);
}

int
CPacketBuffer::push_back(__in BYTE* _data, __in_opt int _size)
{
	if(size<(_size+use))
	{
		printf("full\n");
		return -1;
	}

	int len = _size;
	while(len--)
	{
		*(buffer+tail) = *(_data++);
		++tail;
		tail %= size;
	}
	use += _size;

	return 0;
}

int
CPacketBuffer::pop(__out BYTE* _data, __out_opt int* _size)
{
	BYTE length[sizeof DWORD] = {0,};
	int calc = head;
	int len=0;
	int head_len = sizeof(DWORD);

	*(length+0) = *(buffer+calc); ++calc; calc %= size;
	*(length+1) = *(buffer+calc); ++calc; calc %= size;
	*(length+2) = *(buffer+calc); ++calc; calc %= size;
	*(length+3) = *(buffer+calc); ++calc; calc %= size;

	*_size = 0;
	len = *((DWORD*)length) + head_len;

	if(len>use || head_len == len)
		return -1;

	DWORD temp = len;
	while(len--)
	{
		if((head_len--) <= 0)
			*(_data++) = *(buffer+head);
		*(buffer+head) = 0;
		++head;
		head %= size;
	}
	use -= temp;
	*_size = temp - sizeof(DWORD);
	return 0;
}