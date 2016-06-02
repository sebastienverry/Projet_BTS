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
	string s;
	string delimiter = ":";
	string arr[6];
	string sql;
	int i = 0;
	int debuSubStr;
	int finSubStr;
	//\r\n+CMGL: 9,\"REC UNREAD\",\"+33780573989\",\"\",\"16/06/02,15:00:00+08\"\r\nCell_Id:
	ligne = "3DFD Fix:43.773121,7.212598\r\n";            

	// latitude
	debuSubStr = ligne.find_last_of( "Fix:") + 1; 
	finSubStr = ligne.find_last_of(",");
	longi = ligne.substr(debuSubStr, finSubStr - debuSubStr);
	//strcpy (lati , s);       

	// longitude
	debuSubStr= finSubStr+1;
	//s = ;
	finSubStr = ligne.find("\ ", 100);
	//debuSubStr = ligne.find_last_of(":") + 1;								/* On prend toujours la trame par la fin et on va demander de garder que ce qu'il y a entre la première virgule et les : */
	lati = ligne.substr(debuSubStr+10, finSubStr - debuSubStr - 1);


	//finSubStr = debuSubStr;
	//debuSubStr = ligne.find_last_of(":", finSubStr - 2) + 1;				/* Ici on reprend ce qui a été fait dans l'étape au dessus */
	//arr[4] = ligne.substr(debuSubStr, finSubStr - debuSubStr - 6);			/*On prend ce qu'il y a entre l'étape du dessuset les : */




	/* Display SQL */
	//sql = "INSERT INTO markers VALUES('', " + arr[i] + "," + arr[1] + ");";
}

void EcrireDansBDD(string lat, string  lng)
{
	
	int result;
	string requete;
	//char *r[100];

	MYSQL *mysql = mysql_init(NULL);
	

	mysql_options(mysql, MYSQL_READ_DEFAULT_GROUP, "option");
	if (mysql_real_connect(mysql, "localhost", "root", "", "localisation", 0, NULL, 0))
	{
		//sprintf_s(
		requete = "INSERT INTO `markers` ( lat  , lng) VALUES ( '" + lat + "',  '" + lng + "');";

		//mysql_query(mysql, "TRUNCATE TABLE `markers`");

		
		result=mysql_query(mysql, requete.c_str());  

		
		mysql_close(mysql);
		if (result)
		{
			cout << "Les données n'ont pas été enregistrés dans la base" << endl;
		}
	}
	else
	{
		printf("Une erreur s'est produite");
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


		comman.TxData((char *)"AT+CMGL\r", strlen("AT+CMGL\r"));
		//sprintf(dest,"AT+CMGL\r");

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
		EcrireDansBDD(lat, lng);

		
	}
	system("pause");

}
