#ifndef CPU_READWRITELISTENER_H
#define CPU_READWRITELISTENER_H


class CPUReadWriteListener
{
public:
	CPUReadWriteListener();
	~CPUReadWriteListener();

	virtual unsigned char onCPURead(unsigned short cpuAddress) = 0;

	virtual void onCPUWrite(unsigned short cpuAddress, unsigned char value) = 0;

};



#endif