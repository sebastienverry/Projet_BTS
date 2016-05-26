/* c__rs232.hpp
*/

#ifndef C__RS232_HPP_INCLUDED
#define C__RS232_HPP_INCLUDED

#include <Windows.h>
#include <iostream>
#include <string>

class c__rs232
{
public:
	//c__rs232(void);
	c__rs232(LPCWSTR port);
	~c__rs232(void);
	
	void confCom(int baud_rate = CBR_9600, BOOL f_parity = TRUE, int byte_size = 8, int parity = ODDPARITY, int stop_bit = TWOSTOPBITS);
	BOOL TxData(char *buffer, unsigned long number_bytes_buffer);
	BOOL TxData(std::string &buffer, unsigned long number_bytes_buffer);
	//BOOL TxData(char*buffer, char [11],unsigned long number_bytes_buffer);
	unsigned int RxData(char *buffer, unsigned long number_bytes_buffer);


private:
	BOOL setTimeout(int TO);
	HANDLE hCom;
};

#endif // C__RS232_HPP_INCLUDED