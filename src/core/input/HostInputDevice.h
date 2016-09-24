#ifndef INPUT_HOSTINPUTDEVICE
#define INPUT_HOSTINPUTDEVICE



class HostInputDevice 
{
	
public:
	HostInputDevice();
	~HostInputDevice();

	virtual bool isKeyDown(int key) = 0;

private:


};


#endif