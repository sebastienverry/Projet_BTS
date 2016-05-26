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

void transform() //(string line)
{
	string s;
	string delimiter = ":";
	string line = "FIX CellID:0612345678 Time:2016:04:12 10:44:22 Fix:48.255568,2.78955";            /*\r\n+CMGL: 9,\"REC UNREAD\",\"+33665071903\",\"\",\"16/05/25,18:16:07+08\"\r\nsss\r\n\r\nOK\r\n";*/
	string arr[6];
	string sql;
	int i = 0;
	int BegSubStr;
	int EndSubStr;
	//stringstream ssin(line);


	/*while (ssin.good() && i < 6)
	{
	ssin >> arr[i];
	++i;
	}*/

	BegSubStr = line.find_last_of(",") + 1;								/* On prend la trame par la fin et on va aller rechercher la première virgule */
	arr[0] = line.substr(BegSubStr, line.length() - BegSubStr);       

	EndSubStr = BegSubStr;												/* Ici on reprend ce qui a été fait dans l'étape au dessus */
	BegSubStr = line.find_last_of(":") + 1;								/* On prend toujours la trame par la fin et on va demander de garder que ce qu'il y a entre la première virgule et les : */
	arr[1] = line.substr(BegSubStr, EndSubStr - BegSubStr);

	EndSubStr = line.find_last_of(" ");
	BegSubStr = line.find_last_of(" ", EndSubStr - 1) + 1;				/* On prend toujours la trame par la fin et on va demander de garder que ce qu'il y a entre le premier espace et le deuxième espace :*/
	arr[2] = line.substr(BegSubStr, EndSubStr - BegSubStr);     

	EndSubStr = BegSubStr;
	BegSubStr = line.find_last_of(":", EndSubStr - 1) + 1;				/* Ici on reprend ce qui a été fait dans l'étape au dessus */
	arr[3] = line.substr(BegSubStr, EndSubStr - BegSubStr);				/* On prend la trame à partir de l'espace précédent jusqu'à les dernier :*/

	EndSubStr = BegSubStr;
	BegSubStr = line.find_last_of(":", EndSubStr - 2) + 1;				/* Ici on reprend ce qui a été fait dans l'étape au dessus */
	arr[4] = line.substr(BegSubStr, EndSubStr - BegSubStr - 6);			/*On prend ce qu'il y a entre l'étape du dessuset les : */




	/* Display SQL */
	sql = "INSERT INTO  VALUES(";
	for (i = 0; i < 5; i++) {
		sql += "'" + arr[i] + "',";
	}
	sql += "'" + arr[5] + "');";

	cout << sql << endl;
}

void EcrireDansBDD(char* lat, char*  lng)
{
	//MYSQL mysql;
	int result;
	char requete[150];

	MYSQL *mysql = mysql_init(NULL);
	//"MYSQL my_sql;" suivi de "mysql_init(&my_sql);" par "MYSQL *my_sql = mysql_init(NULL);"

	//mysql_init(&mysql);

	mysql_options(mysql, MYSQL_READ_DEFAULT_GROUP, "option");
	if (mysql_real_connect(mysql, "localhost", "root", "", "localisation", 0, NULL, 0))
	{
		sprintf_s(requete, "INSERT INTO `markers` ( lat  , lng) VALUES ( '%s',  '%s');", lat, lng);

		mysql_query(mysql, "TRUNCATE TABLE `markers`");
		result=mysql_query(mysql, requete);  //"INSERT INTO `markers` ( lat  ", " + + *lng + ") VALUES ('33', '1');"

		//mysql_free_result(result);
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
/* //Déclaration du pointeur de structure de type MYSQL
MYSQL mysql;

//Initialisation de MySQL
mysql_init(&mysql);

//Options de connexion
mysql_options(&mysql,MYSQL_READ_DEFAULT_GROUP,"option");

//Si la connexion réussie...
if(mysql_real_connect(&mysql,"127.0.0.1","root","","abo",0,NULL,0))
{
//Requête qui sélectionne tout dans ma table scores
mysql_query(&mysql, "SELECT * FROM informations_completes");

//Déclaration des objets
MYSQL_RES *result = NULL;

MYSQL_ROW row;

int i = 1;

//On met le jeu de résultat dans le pointeur result
result = mysql_use_result(&mysql);

//Tant qu'il y a encore un résultat ...
while ((row = mysql_fetch_row(result)))
{
printf("Resultat %ld\n", i);
i++;
}

//Libération du jeu de résultat
mysql_free_result(result);

//Fermeture de MySQL
mysql_close(&mysql);

}
else  //Sinon ...
{
printf("Une erreur s'est produite lors de la connexion à la BDD!");
}
*/

int main(void)
{
	
	transform();

	char RxBuf[256];
	int NbRecus;
	char dest[256];
	string msg;
	char caract;
	char lat[10], lng[10];
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

		strcpy_s(dest, "AT+CPIN=1234\r");									// ici on envoie la commande au Modem
		comman.TxData(dest, strlen(dest));
		NbRecus = comman.RxData(RxBuf, sizeof RxBuf);						// ici on stock dans une variable NbRecus La réponse du modem
		RxBuf[NbRecus] = '\0';
		cout << "NbRecus : " << NbRecus << " Data: " << RxBuf << endl;		// NbRecus stock la réponse du modem et on l'affiche

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

		//transform(RxBuf);
		strcpy_s(lat , "10.33");
		strcpy_s(lng , "20.45");
		EcrireDansBDD(lat, lng);

		/*	ofstream fichier("test.txt", ios::out| ios::trunc);
		if (fichier)
		{
		fichier<<RxBuf<<endl;
		fichier.close();
		}
		else {
		cout <<"Imopossible d'ouvrir le fichier !!"<<endl;
		}*/
	}
	system("pause");

}
