#ifndef INESFILEFORMATEXCEPTION_H
#define INESFILEFORMATEXCEPTION_H

#include <exception>


class iNesFileFormatException : public std::exception
{
public:
	iNesFileFormatException(const char* message);
	~iNesFileFormatException();

	virtual const char* what() const throw()
	{
		return this->msg;
	}
private:
	const char* msg;
};


#endif