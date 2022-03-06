#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
#include<utility>
#include<stdio.h>
#include<fstream>
#include <sys/stat.h>
#include <sys/types.h>

using namespace std;




string const mainDirctory = "/Kamat/DataBases";
vector<string> dblist;
string CurntlyDB;


void loadDBList() {
	dblist.clear();
	ifstream dblistfile(mainDirctory + ".km");
	int num;
	dblistfile >> num;
	string exdb;
	while (num--) {
		dblistfile >> exdb;
		dblist.push_back(exdb);
	}
	dblistfile.close();
}

bool Createdb(string name) {


	for (string DBName : dblist) {
		if (name == DBName) {
			return false;
		}
	}
	ofstream dblistfile(mainDirctory + ".km");
	dblistfile << dblist.size() + 1 << "\n";
	for (string DBName : dblist) {
		dblistfile << DBName << "\n";
	}
	dblist.push_back(name);
	dblistfile << name << "\n";
	dblistfile.close();
	string str = "/Kamat/DataBases/" + name;
	char* arr;
	arr = &str[0];
	mkdir(arr,0777);
	ofstream dbFile(mainDirctory + "/" + name + "/" + name + ".db.km");
	dbFile << "0\n";
	dbFile.close();
	return true;
}

bool CREATE() {

	string Type;
	cin >> Type;
	for (int i = 0; i < Type.size(); i++) {
		Type[i] = toupper(Type[i]);
	}

	if (Type == "DATABASE") {
		string Suffix;
		cin >> Suffix;
		for (int i = 0; i < Suffix.size(); i++) {
			Suffix[i] = tolower(Suffix[i]);
		}
		bool b = 1;

		for (int i = 0; i < Suffix.size(); i++) {
			if (Suffix[i] != '_' && !isalpha(Suffix[i]) && !isdigit(Suffix[i])) {
				b = 0;
				break;
			}

		}

		if (b == 0) {
			cout << "Database name should not to contain special character\n";
			return 0;
		}
		else {
			if (Createdb(Suffix)) {
				return 1;
			}
			else {
				cout << "can not create database!!\n";
				return 0;
			}
		}
	}
	else if (Type == "TABLE") {
		return 0;
	}
	else {
		cout << "syntax error\n";
	}
	return 0;
}

bool SELECT() {


	return 0;
}

bool KMDELETE() {


	return 0;
}
bool UPDATE() {


	return 0;
}

bool INSERT() {


	return 0;
}

bool DROP() {


	return 0;
}

bool USE() {


	return 0;
}

bool ShTables() {


	return 0;
}

void setup() {

	ifstream ifile;
	ifile.open(mainDirctory + ".km");
	if (ifile) {
		ifile.close();
	}
	else {

		mkdir("/Kamat",0777);
		mkdir("/Kamat/DataBases",0777);

		ofstream MyFile(mainDirctory + ".km");

		MyFile << "0\n";

		MyFile.close();
	}

	loadDBList();
	CurntlyDB = "any";
}
int main() {
	setup();



	string Quary;


	while (true) {

		cin >> Quary;


		for (int i = 0; i < Quary.size(); i++) {
			Quary[i] = toupper(Quary[i]);
		}
		//cout << Quary << "\n";


		if (Quary == "SELECT") {
			SELECT();
		}
		else if (Quary == "DELETE") {
			KMDELETE();
		}
		else if (Quary == "INSERT") {
			string Suffix;
			cin >> Suffix;
			for (int i = 0; i < Suffix.size(); i++) {
				Suffix[i] = toupper(Suffix[i]);
			}
			if (Suffix == "INTO") {
				INSERT();
			}
			else {
				cout << "syntax error\n";
			}
		}
		else if (Quary == "DROP") {
			DROP();
		}
		else if (Quary == "UPDATE") {
			UPDATE();
		}
		else if (Quary == "CREATE") {
			CREATE();
		}
		//SHOW DATABASES;
		else if (Quary == "SHOW") {
			string Suffix;
			cin >> Suffix;
			for (int i = 0; i < Suffix.size(); i++) {
				Suffix[i] = toupper(Suffix[i]);
			}
			if (Suffix == "DATABASES") {
				for (string str : dblist) {
					cout << str << "\n";
				}
			}
			else if (Suffix == "TABLES") {
				ShTables();
			}
			else {
				
				cout << "syntax error\n";
				
			}
		}
		else if (Quary == "USE") {
			if (USE()) {
				cout << CurntlyDB << " is in use now\n";
			}
			else {
				cout << "Not in the databases list !!\n";
			}
		}
		else {
			cout << "syntax error\n";
		}
	}

}