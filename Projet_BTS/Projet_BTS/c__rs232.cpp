/* c__rs232.hpp
*/

#include "c__rs232.hpp"

using namespace std;

c__rs232::c__rs232(LPCWSTR port)
{
	hCom = CreateFile(
		port,                           // Choix du port « COMx »
		GENERIC_READ | GENERIC_WRITE, 	// accès pour lire et écrire sur le port
		0,                            	// accès exclusif au port de COM
		NULL,                         	// sécurité par défaut
		OPEN_EXISTING,                	// Fichier spécial associé à la ressource COM1 déjà existant.
		0, 															// mode synchrone (BLOQUANT)
		NULL);
	if (hCom == INVALID_HANDLE_VALUE)
	{
		cout << "Erreur dans la connextion du port " << port << endl;
		exit(1);
	}
	cout << "Succes dans la connextion du port " << port << endl;
}

void c__rs232::confCom(int baud_rate, BOOL f_parity, int byte_size, int parity, int stop_bit)
{
	DCB sConfig;
	if (GetCommState(hCom, &sConfig) == 0)
	{
		cout << "Erreur dans l'obtention de la configuration: " << GetLastError() << endl;

	}
	else
	{
		sConfig.BaudRate = baud_rate;
		sConfig.fParity = f_parity;
		sConfig.ByteSize = byte_size;
		sConfig.Parity = parity;
		sConfig.StopBits = stop_bit;

		if (SetCommState(hCom, &sConfig) == 0)
		{
			cout << "Erreur de réglage dans la configuration: " << GetLastError() << endl;
		}
	}
	Sleep(500);
}

BOOL c__rs232::TxData(char *buffer, unsigned long number_bytes_buffer)
{
	unsigned int i;
	unsigned long size_1 = 1;
	for (i = 0; i < number_bytes_buffer; i++)
	{
		if (WriteFile(hCom, &buffer[i], 1, &size_1, NULL) == 0)
		{
			cout << "pb" << endl;
			return FALSE;
		}
	}

	Sleep(500);
	return TRUE;
}

BOOL c__rs232::TxData(string &buffer, unsigned long number_bytes_buffer)
{
	unsigned int i;
	unsigned long size_1 = 1;
	char byte;
	for (i = 0; i < number_bytes_buffer; ++i)
	{
		byte = buffer.at(i);
		if (WriteFile(hCom, &byte, size_1, &size_1, NULL) == 0)
			return FALSE;
	}
	return TRUE;
}

unsigned int c__rs232::RxData(char *buffer, unsigned long number_bytes_buffer)
{
	unsigned int i = 0;
	unsigned long number_bytes_read = 1;
	setTimeout(0);
	do
	{
		if (ReadFile(hCom, (buffer + i), number_bytes_read, &number_bytes_read, NULL) == 0)
			return 0;
		if (number_bytes_read == 1)
			++i;
		if (i == 1)
		{
			setTimeout(20);
		}
	} while ((number_bytes_read == 1) && (i < number_bytes_buffer));
	return i;
}

BOOL c__rs232::setTimeout(int TO)
{
	COMMTIMEOUTS time_out;
	time_out.ReadIntervalTimeout = 0;
	time_out.ReadTotalTimeoutMultiplier = 0;
	time_out.ReadTotalTimeoutConstant = TO;
	if (SetCommTimeouts(hCom, &time_out) == 0)
		return FALSE;
	return TRUE;
}

c__rs232::~c__rs232(void)
{
	CloseHandle(hCom);
}
