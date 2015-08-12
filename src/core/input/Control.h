#ifndef INPUT_CONTROL_H
#define INPUT_CONTROL_H

#include <string>


class InputControl
{
public:
	InputControl(const char* controlName) : name(controlName)
	{

	}
	~InputControl()
	{

	}

	std::string& getName()
	{
		return name;
	}

	const std::string& getType()
	{
		return type;
	}

protected:
	std::string name;
	const std::string type;
};


class ButtonControl : public InputControl
{
public:
	ButtonControl(const char* controlName) : InputControl(controlName)
	{
		this->pressed = false;
	}
	~ButtonControl();

	void setState(bool isPressed);

	bool isPressed();

private:
	bool pressed;

};


//class AnalogueControl : public InputControl
//{
//public:
//	AnalogueControl(int min, int max);
//	~AnalogueControl();
//
//	void setState(int value);
//
//	int getValue();
//
//	int getMinValue();
//	int getMaxValue();
//};

#endif