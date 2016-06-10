//main.cpp

#include <iostream>
#include <string>
#include <cstdio>
#include <conio.h>
#include <fstream>
#include <sstream>   
#include <winsock.h>
#include <mysql/mysql.h>
#include "c__rs232.hpp"

using namespace std;

void transform(string ligne, string &lati, string &longi) //()
{

	string arr[6];
	string sql;
	int i = 0;
	int p_start;
	int p_end; // position


	// Lattitude
	p_start = ligne.find_last_of(":") + 1;
	p_end = ligne.find_last_of(",") - p_start;

	lati = ligne.substr(p_start, p_end);

	

	// Latitude
	p_start = ligne.find_last_of(",") + 1;
	p_end = ligne.length() - (p_start + 5);


	cout << p_start << "\n";
	cout << p_end << "\n";

	longi = ligne.substr(p_start, p_end);


	// Display

	cout << "Latitude:\t" << lati << "\n";
	cout << "Longitude:\t" << longi<< "\n";
	//system("pause");


	/* Display SQL */
	sql = "INSERT INTO markers VALUES('', " + arr[i] + "," + arr[1] + ");";
}



/*                                                

				Partie permettant l'envoie du SMS transformer dans la base de donnée


*/


void EcrireDansBDD(string lat, string  lng)
{
	
	int result;
	string requete;
	//char *r[100];

	MYSQL *mysql = mysql_init(NULL);
	

	mysql_options(mysql, MYSQL_READ_DEFAULT_GROUP, "option");
	if (mysql_real_connect(mysql,"192.168.241.237", "test1", "test1", "localisation", 0, NULL, 0))
	{
		//sprintf_s(
		
		requete = "INSERT INTO `markers` ( lat  , lng) VALUES ('" + lat + "',  '" + lng + "');";

		mysql_query(mysql, "TRUNCATE TABLE `markers`");

		
		result=mysql_query(mysql, requete.c_str());  

		
		mysql_close(mysql);
		if (result)
		{
			cout << "Les donnees n'ont pas ete enregistres dans la base" << endl;
		}
	}
	else
	{
		cout<<"Une erreur s'est produite"<<endl;
	}

	 
}


int main(void)
{
	char RxBuf[256];
	int NbRecus;
	char dest[256];
	string msg;
	char caract;
	string lat, lng;
	LPCWSTR com = L"\\\\.\\COM8";


	while (1)
	{
		c__rs232 comman(com);


		comman.confCom(CBR_9600, FALSE, 8, NOPARITY, ONESTOPBIT);

		strcpy_s(dest, "AT\r");
		comman.TxData(dest, strlen(dest));									// ici on envoie la commande au Modem
		NbRecus = comman.RxData(RxBuf, sizeof RxBuf);						// ici on stock dans une variable NbRecus La réponse du modem
		RxBuf[NbRecus] = '\0';												//	
		cout << "NbRecus : " << NbRecus << " Data: " << RxBuf << endl;		// NbRecus stock la réponse du modem et on l'affiche

		//strcpy_s(dest, "AT+CPIN=1234\r");									// ici on envoie la commande au Modem
		//comman.TxData(dest, strlen(dest));
		//NbRecus = comman.RxData(RxBuf, sizeof RxBuf);						// ici on stock dans une variable NbRecus La réponse du modem
		//RxBuf[NbRecus] = '\0';
		//cout << "NbRecus : " << NbRecus << " Data: " << RxBuf << endl;		// NbRecus stock la réponse du modem et on l'affiche

		sprintf_s(dest, "AT+CMGF=1\r");
		comman.TxData(dest, strlen(dest));									// ici on envoie la commande au Modem
		NbRecus = comman.RxData(RxBuf, sizeof RxBuf);						// ici on stock dans une variable NbRecus La réponse du modem
		RxBuf[NbRecus] = '\0';
		cout << "NbRecus : " << NbRecus << " Data: " << RxBuf << endl;		// NbRecus stock la réponse du modem et on l'affiche

		do{
		comman.TxData((char *)"AT+CMGL\r", strlen("AT+CMGL\r"));
		/*sprintf(dest,"AT+CMGL\r");*/
		

			NbRecus = comman.RxData((char *)RxBuf, sizeof RxBuf);				// ici on stock dans une variable NbRecus La réponse du modem
			RxBuf[NbRecus] = '\0';
			cout << "NbRecus : " << NbRecus << " Data: " << RxBuf << endl;
			NbRecus = comman.RxData((char *)RxBuf, sizeof RxBuf);
			RxBuf[NbRecus] = '\0';
			cout << "NbRecus : " << NbRecus << " Data: " << RxBuf << endl;		// NbRecus stock la réponse du modem et on l'affiche

			//sprintf_s(dest, "AT+CMGD=1,4\r");
			//comman.TxData(dest, strlen(dest));									// ici on envoie la commande au Modem
			//NbRecus = comman.RxData(RxBuf, sizeof RxBuf);						// ici on stock dans une variable NbRecus La réponse du modem
			//RxBuf[NbRecus] = '\0';
			//cout << "NbRecus : " << NbRecus << " Data: " << RxBuf << endl;

			//transform(RxBuf);
			/*strcpy_s(lat , "10.33");
			strcpy_s(lng , "20.45");*/


			transform(RxBuf, lat, lng);

		} while (lat == "\r\nOK\r\n");
		EcrireDansBDD(lat, lng);

		
	}
	//system("pause");

}
