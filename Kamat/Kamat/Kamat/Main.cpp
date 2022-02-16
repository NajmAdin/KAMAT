/*
* this is The Kamat Structured Query Language (SQL)
* it was writen by :
*						ENG:- Najm Adin Othman Sayed     CS Dep.
*                       ENG:- Ahmed Saad AbdulAziz       IS Dep.
*/


#include<iostream>
#include<string>
#include<vector>
#include<utility>
#include<direct.h>
#include<stdio.h>
#include<windows.h>
#include<fstream>




using namespace std;


string const mainDirctory = "C:/Kamat/DataBases";
vector<string> dblist;
string CurntlyDB;
vector<string>DataTypes = { "BOOL","BOOLEAN","INT","INTEGER","FLOAT","DATE","TIME","YEAR","DATETIME" };
string VarCh = "VARCHAR";
string PKey[2] = { "primary","key" };

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



bool IfTableExist(string name) {
	ifstream dbFile(mainDirctory + "/" + CurntlyDB + "/" + CurntlyDB + ".db.km");
	int num;
	dbFile >> num;
	string ExTable;
	for (int i = 0; i < num; i++) {
		dbFile >> ExTable;
		if (ExTable == name) {
			dbFile.close();
			return 1;
		}
	}
	dbFile.close();
	return 0;
}

bool addTable(string name) {
	ifstream dbFile(mainDirctory + "/" + CurntlyDB + "/" + CurntlyDB + ".db.km");
	int num;
	dbFile >> num;
	vector<string> ExTable(num,"");
	for (int i = 0; i < num; i++) {
		dbFile >> ExTable[i];	
	}
	dbFile.close();
	ofstream dbFileW(mainDirctory + "/" + CurntlyDB + "/" + CurntlyDB + ".db.km");
	dbFileW <<ExTable.size()+1<<"\n";
	for (int i = 0; i < num; i++) {
		dbFileW<< ExTable[i]<<"\n";
	}
	dbFileW << name;
	dbFileW.close();
	return 0;
}


string RegularDataTaype(string TY) {
	if (TY.size() < 3) {
		return "non";
	}


	int stop = 0;
	if (TY[TY.size() - 1] == ',') {
		stop++;
		if (TY[TY.size() - 2] == ')') {
			stop++;
		}
	}
	else if (TY[TY.size() - 1] == ')') {
		stop++;
	}

	string DTY = "";
	for (int i = 0; i < TY.size()-stop; i++) {
		DTY += toupper(TY[i]);
	}
	for (int i = 0; i < DataTypes.size(); i++) {
		if (DTY == DataTypes[i]) {
			return DataTypes[i];
		}
	}
	return "non";
}


bool Create_Table(string Name) {

	vector<pair<string, string>> ColumnAndType;
	int primKeyIdx = -1;
	char ch;
	cin >> ch;
	if (ch != '(') {
		cout << "syntax error\n";
		return 0;
	}

	int idx = -1;
	string NM, TY;
	while (true) {
		cin >> NM >> TY;
		if (!isalpha(NM[0]) && NM[0] != '_') {
			cout << "syntax error\n";
			return 0;
		}
		for (int i = 1; i < NM.size(); i++) {
			if (!isalpha(NM[i]) && NM[i] != '_' && !isdigit(NM[i])) {
				cout << "syntax error\n";
				return 0;
			}
		}
		for (int i = 0; i < NM.size(); i++) {
			NM[i] = tolower(NM[i]);
		}
		if (NM == PKey[0]) {
			for (int i = 0; i < TY.size(); i++) {
				TY[i] = tolower(TY[i]);
			}
			if (TY == PKey[1]) {
				if (primKeyIdx != -1) {
					cout << "In this version you cannot use more than one primary key";
					return 0;
				}
				else {
					primKeyIdx = idx;
				}
			}
		}
		else {
			idx++;
			string TYBack = RegularDataTaype(TY);
			if (TYBack == "non") {
				cout << "non\n";//we need to write VarChar(n) Fun
				return 0;
			}
			else {
				ColumnAndType.push_back({NM, TYBack});
				if (TY[TY.size() - 1] == ')') {
					break;
				}
			}
		}

	}
	addTable(Name);
	ofstream TbFile(mainDirctory + "/" + CurntlyDB + "/" + Name + ".t.km");
	TbFile <<ColumnAndType.size()<< "\n";
	for (int i = 0; i < ColumnAndType.size(); i++) {
		TbFile << ColumnAndType[i] .first<<" " << ColumnAndType[i].second << "\n";
	}
	if (primKeyIdx == -1) {
		TbFile << 0;
	}
	else {
		TbFile << primKeyIdx;
	}
	TbFile.close();
	return 1;
}


bool SELECT() {
	

	return 0;
}

bool DROP( ) {
	/*remove_all("myDirectory");
	remove(fileName);
	*/

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

		bool b = 0;
		for (string str : dblist) {
			if (str == Suffix) {
				b = 1;
			}
		}
		
		if (!b) {
			cout << " DataBase is Not Created Already!!\n";
			return 0;
		}
		else {
			string str = mainDirctory + "/" + Suffix + "/" + Suffix + ".db.km";
			char* ch = &str[0];
			LPCSTR Pth = ch;
			DeleteFileA(Pth);
			ofstream dblistFile(mainDirctory + ".km");
			dblistFile << dblist.size() - 1 << "\n";
			for (string str : dblist) {
				if (str != Suffix) {
					dblistFile << str << "\n";
				}
			}
			dblistFile.close();
			loadDBList();

		}

	}
	

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
		
		for (int i = 0; i < Suffix.size(); i++) {
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
	else if (Type == "TABLE") {

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
bool KMDELETE( ) {


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

