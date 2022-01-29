/*
* this is a lexical for Kamat Structured Query Language (SQL)
* it was writen by Najm Adin
*/


#include<iostream>
#include<string>
#include<vector>
#include<utility>
#include<direct.h>
#include<stdio.h>
#include"CreateTable.cpp"



using namespace std;


string const mainDirctory = "C:/Kamat/DataBases";
vector<string> dblist;
string CurntlyDB;


bool Createdb(string name) {
	

	for (string DBName : dblist) {
		if (name == DBName) {
			return false;
		}
	}
	ofstream dblistfile(mainDirctory+".km");
	dblistfile << dblist.size() + 1 << "\n";
	for (string DBName : dblist) {
		dblistfile << DBName << "\n";
	}
	dblist.push_back(name);
	dblistfile << name << "\n";
	dblistfile.close();
	string str= "C:/Kamat/DataBases/" + name;
	char* arr;
	arr = &str[0];
	_mkdir(arr);
	ofstream dbFile(mainDirctory + "/" + name + "/" + name + ".db.km");
	dbFile << "0\n";
	dbFile.close();
	return true;
}


bool SELECT() {
	

	return 0;
}

bool DROP( ) {
	

	return 0;
}
bool CREATE( ) {
	
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
		int ln = Suffix.size();
		if (Suffix[Suffix.size() - 1] == ';') {
			ln--;
		}
		
		for (int i = 0; i < ln; i++) {
			if (Suffix[i] != '_' && !isalpha(Suffix[i]) && !isdigit(Suffix[i])) {
				b = 0;
				break;
			}
			
		}
		
		if (b == 0) {
			cout << "Databese name should not to contain special character\n";
			return 0;
		}
		else {
			if (Createdb(Suffix)) {
				return 0;
			}
			else {
				cout << "can not create database!!\n";
				return 0;
			}
		}
	}
	if (Type == "TABLE") {

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
			cout << "Table name should not to contain special character\n";
			return 0;
		}
		else {
			if (IfTableExist(Suffix)) {
				cout << "Table name is not available\n";
				return 0;
			}
			else {
				Create_Table(Suffix);
			}
		}
	}

	return 0;
}
bool UPDATE( ) {
	

	return 0;
}
bool INSERT( ) {
	

	return 0;
}
bool DELETE( ) {


	return 0;
}
bool USE() {
	string Suffix;
	cin >> Suffix;
	for (int i = 0; i < Suffix.size(); i++) {
		Suffix[i] = tolower(Suffix[i]);
	}
	for (string str : dblist) {
		if (str == Suffix) {
			CurntlyDB = Suffix;
			return true;
		}
	}
	return 0;
}

void loadDBList() {
	ifstream dblistfile(mainDirctory + ".km");
	int num;
	dblistfile >> num;
	string indb;
	while (num--) {
		dblistfile >> indb;
		dblist.push_back(indb);
	}
	dblistfile.close();
}


void stup() {
	ifstream ifile;
	ifile.open(mainDirctory + ".km");
	if (ifile) { }
	else {

		_mkdir("C:/Kamat");
		_mkdir("C:/Kamat/DataBases");

		ofstream MyFile(mainDirctory + ".km");

		MyFile << "0\n";

		MyFile.close();
	}

	loadDBList();
	CurntlyDB = "any";
}

int main() {
	stup();



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
		else if (Quary == "UPDATE") {
			UPDATE();
		}
		else if (Quary == "DELETE") {
			DELETE();
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
			else {
				cout << "syntax error\n";
			}
		}
		else if (Quary == "USE") {
			if (USE()) {
				cout << CurntlyDB << " is in use now\n";
			}
			else {
				cout << CurntlyDB << " is not in databases list !!\n";
			}
		}
		else {
			cout << "syntax error\n";
		}
	}

}

