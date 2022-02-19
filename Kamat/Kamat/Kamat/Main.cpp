/*
* this is The Kamat Structured Query Language (SQL)
* it was writen by :
*						ENG:- Najm Adin Othman Sayed     CS Dep.
*                       ENG:- Ahmed Saad AbdulAziz       IS Dep.
*/


#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
#include<utility>
#include<direct.h>
#include<stdio.h>
#include<windows.h>
#include<fstream>





using namespace std;
/*
	console Handler
					to write read
					SetConsoleTextAttribute(hConsole, 12);
				cout << "syntax error\n";
				to write white
				SetConsoleTextAttribute(hConsole, 15);
				green
				SetConsoleTextAttribute(hConsole, 10);
				yallow
				SetConsoleTextAttribute(hConsole, 14);
*/
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);


string const mainDirctory = "C:/Kamat/DataBases";
vector<string> dblist;
string CurntlyDB;
vector<string>DataTypes = { "BOOL","BOOLEAN","INT","INTEGER","FLOAT" };
string VarCh = "VARCHAR";
string PKey[2] = { "primary","key" };


int HashIt(string s) {
	long long res = 0;
	for (int i = 0; i < s.size(); i++) {
		res += s[i];
	}
	return (res % 101);
}

void loadDBList() {
	dblist.clear();
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


string RegularDataType(string TY) {
	if (TY.size() < 3) {
		return "non";
	}


	int stop = 0;
	if (TY[TY.size() - 1] == ','|| TY[TY.size() - 1] == ')') {
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
	int idx = 0;
	for (int i = 0; i < TY.size(); i++) {
		if (idx < 7) {
			if (VarCh[idx] != toupper(TY[i])) {
				return "non";
			}
			idx++;
		}
		else {
			if (TY[i] != '(') {
				return "non";
			}
			i++;
			int num = 0;
			string strnum = "";
			for (; i < TY.size(); i++) {
				if (isdigit(TY[i])) {
					num *= 10;
					num += (TY[i] - '0');
				}
				else if (TY[i] == ')') {
					while (num) {
						strnum += ((num % 10)+'0');
						num /= 10;
					}
					reverse(strnum.begin(),strnum.end());
					return (VarCh + '(' + strnum + ')');
				}
			}
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
			if (TY == PKey[1]|| TY == (PKey[1]+',') || TY == (PKey[1]+')')) {
				if (primKeyIdx != -1) {
					cout << "In this version you cannot use more than one primary key\n";
					return 0;
				}
				else {
					primKeyIdx = idx;
				}
				if(TY[TY.size()-1]==')'){
					break;
				}
			}
		}
		else {
			idx++;
			string TYBack = RegularDataType(TY);
			if (TYBack == "non") {
				cout << "syntax error\n";
				return 0;
			}
			else {
				ColumnAndType.push_back({NM, TYBack});
				if (TY[TY.size() - 1] == ')') {
					if (tolower(TY[0]) != 'v') {
						break;
					}
					else {
						if (TY[TY.size() - 2] == ')') {
							break;
						}
					}
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


string inputDataType(string input,string Type) {
	
	if (Type == DataTypes[0] || Type == DataTypes[1]) {
		
		if ( input == "0"|| input == "false") {
			return"0";
		}
		else if (input == "1"|| input == "true") {
			return"1";
		}
		else {
			SetConsoleTextAttribute(hConsole, 12);
			cout << "input error\n";
			SetConsoleTextAttribute(hConsole, 15);
			return "";
		}
	}
	else if (Type == DataTypes[2] || Type == DataTypes[3]) {
		string sol = "";
		int i = 0;
		if (input[0] == '-') { 
			i = 1;
			sol += '-';
		}
		for (; i < input.size(); i++) {
			if (!isdigit(input[i])) {
				SetConsoleTextAttribute(hConsole, 12);
				cout << "syntax or input error\n";
				SetConsoleTextAttribute(hConsole, 15);
				return "";
			}
			else {
				sol += input[i];
			}
		}
		return sol;
	}
	else if (Type == DataTypes[4]) {
		int point = 0;
		string sol = "";
		int i = 0;
		if (input[0] == '-') {
			i = 1;
			sol += '-';
		}
		for (; i < input.size() ; i++) {
			if (!isdigit(input[i])) {
				if (input[i] == '.'&&point==0) {
					point++;
				}
				else {
					SetConsoleTextAttribute(hConsole, 12);
					cout << "syntax or input error\n";
					SetConsoleTextAttribute(hConsole, 15);
					return "";
				}
			}
			else {
				sol += input[i];
			}
		}
		return sol;
	}
	else if (Type[0] == 'V') {
		//VARCHAR
		if (input.size()<3||input[0] != '\'' || input[input.size() - 1] != '\'') {
			SetConsoleTextAttribute(hConsole, 12);
			cout << "Varchar input error\n";
			SetConsoleTextAttribute(hConsole, 15);
			return "";
		}
		int len = 0;
		for (int i = 7; i < Type.size() - 1; i++) {
			len *= 10;
			len += (Type[i] - '0');
		}
		if (input.size() > (len + 2)) {
			SetConsoleTextAttribute(hConsole, 12);
			cout << "input length is more than allow\n";
			SetConsoleTextAttribute(hConsole, 15);
			return "";
		}
		string sol="";
		for (int i = 1; i < input.size() - 1; i++) {
			sol += input[i];
		}
		return sol;
	}
	return "";
}

bool DeleteSubTable(string table, string db) {
	ifstream ifile;
	string str = " ";
	char* ch = &str[0];
	for (int i = 0; i < 101; i++) {
		str = mainDirctory + "/" + db + "/" + table + "." + to_string(i) + ".t.km";
		ch = &str[0];
		remove(ch);
	}
	return 1;
}

bool SELECT() {
	//SELECT * FROM Customers;
	string who;
	cin >> who;
	if (who != "*") {
		SetConsoleTextAttribute(hConsole, 12);
		cout << "syntax error\n";
		SetConsoleTextAttribute(hConsole, 14);
		cout << "Showing results for certain rows is not available in this version of the program\n";
		SetConsoleTextAttribute(hConsole, 15);
		return 0;
	}
	string FROM;
	cin >> FROM;
	for (int i = 0; i < FROM.size(); i++) {
		FROM[i] = toupper(FROM[i]);
	}
	if (FROM != "FROM") {
		SetConsoleTextAttribute(hConsole, 12);
		cout << "syntax error\n";
		SetConsoleTextAttribute(hConsole, 15);
		return 0;
	}
	string Table = "", strin;
	cin >> strin;
	int b = 0;
	if (strin[strin.size() - 1] == ';') {
		b = 1;
	}
	for (int i = 0; i < strin.size() - b; i++) {
		Table += tolower(strin[i]);
	}
	if (!IfTableExist(Table)) {
		SetConsoleTextAttribute(hConsole, 12);
		cout << "Table is not Exist\n";
		SetConsoleTextAttribute(hConsole, 15);
		return 0;
	}
	int sz;
	ifstream TbFile(mainDirctory + "/" + CurntlyDB + "/" + Table + ".t.km");
	TbFile >> sz;
	vector<pair<string, string>>TYlist(sz);
	for (int i = 0; i < sz; i++) {
		TbFile >> TYlist[i].first >> TYlist[i].second;
	}
	int PrimIdx;
	TbFile >> PrimIdx;
	TbFile.close();


	if (b == 0) {
		string Where;
		cin >> Where;
		if (Where != ";") {
			int CondIdx = -1;
			for (int i = 0; i < Where.size(); i++) {
				Where[i] = toupper(Where[i]);
			}
			if (Where == "WHERE") {
				string condition;
				cin >> condition;
				for (int i = 0; i < condition.size(); i++) {
					condition[i] = tolower(condition[i]);
				}
				for (int i = 0; i < sz; i++) {
					if (TYlist[i].first == condition) {
						CondIdx = i;
					}
				}

				if (CondIdx == -1) {
					SetConsoleTextAttribute(hConsole, 12);
					cout << "Field name not found\n";
					SetConsoleTextAttribute(hConsole, 15);
					return 0;
				}
				if (CondIdx != PrimIdx) {
					SetConsoleTextAttribute(hConsole, 14);
					cout << "In this version of the program, you cannot choose anything other than the primary key or show all in the table\n";
					SetConsoleTextAttribute(hConsole, 15);
					return 0;
				}
				char eq;
				cin >> eq;
				if (eq != '=') {
					SetConsoleTextAttribute(hConsole, 12);
					cout << "syntax error\n";
					SetConsoleTextAttribute(hConsole, 15);
					return 0;
				}
				string printerChk = "";
				cin >> printerChk;
				printerChk = inputDataType(printerChk, TYlist[CondIdx].second);
				if (printerChk == "") {
					SetConsoleTextAttribute(hConsole, 12);
					cout << "condition input error\n";
					SetConsoleTextAttribute(hConsole, 15);
					return 0;
				}
				int packNum = HashIt(printerChk);
				vector<string>sol, inSol(sz);
				int datalen = 0;
				ifstream subfile;
				subfile.open(mainDirctory + "/" + CurntlyDB + "/" + Table + "." + to_string(packNum) + ".t.km");
				if (subfile) {
					subfile >> datalen;
					for (int i = 0; i < datalen; i++) {
						for (int j = 0; j < sz; j++) {
							subfile >> inSol[j];
						}
						if (inSol[PrimIdx] == printerChk) {
							sol = inSol;
							break;
						}
					}
					subfile.close();
					if (sol.size() == 0) {
						SetConsoleTextAttribute(hConsole, 14);
						cout << "No data\n";
						SetConsoleTextAttribute(hConsole, 15);
						return 0;
					}
					for (int j = 0; j < sz; j++) {
						cout << TYlist[j].first << '\t';
					}
					cout << "\t\n";
					SetConsoleTextAttribute(hConsole, 9);
					for (int j = 0; j < sz; j++) {
						cout << sol[j] << '\t';
					}
					cout << "\t\n";
					SetConsoleTextAttribute(hConsole, 15);
					return 1;

				}
				SetConsoleTextAttribute(hConsole, 14);
				cout << "No data\n";
				SetConsoleTextAttribute(hConsole, 15);
				return 0;

			}
			SetConsoleTextAttribute(hConsole, 12);
			cout << "syntax error\n";
			SetConsoleTextAttribute(hConsole, 15);
			return 0;
		}

	}

	vector<string>solin(sz);
	vector<vector<string>>solPrint;

	for (int i = 0; i < 101; i++) {
		ifstream SubTbFile(mainDirctory + "/" + CurntlyDB + "/" + Table+"."+to_string(i) + ".t.km");
		if (SubTbFile) {
			int x;
			SubTbFile >> x;
			for (int k = 0; k < x; k++) {
				for (int j = 0; j < sz; j++) {
					SubTbFile >> solin[j];
				}
				solPrint.push_back(solin);
			}
			SubTbFile.close();
		}
	}
	for (int i = 0; i < sz; i++) {
		cout << TYlist[i].first << '\t';
	}
	cout << "\t\n";
	for (int i = 0; i < solPrint.size(); i++) {
		SetConsoleTextAttribute(hConsole, (9+(i%2)));
		for (int j = 0; j < sz; j++) {
			cout << solPrint[i][j]<<'\t';
		}
		cout << "\t\n";
	}
	SetConsoleTextAttribute(hConsole, 15);

	
	

	return 1;
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
			int sz;
			ifstream dbfile(ch);
			dbfile >> sz;
			string Table;
				
			for (int i = 0; i < sz; i++) {
				dbfile >> Table;
				DeleteSubTable(Table, Suffix);
				str = mainDirctory + "/" + Suffix + "/" + Table + ".t.km";
				ch = &str[0];
				remove(ch);

			}
			dbfile.close();
			str = mainDirctory + "/" + Suffix + "/" + Suffix + ".db.km";
			ch = &str[0];
			remove(ch);
			str = mainDirctory + "/" + Suffix;
			ch = &str[0];
			_rmdir(ch);
			ofstream dblistFile(mainDirctory + ".km");
			dblistFile << dblist.size() - 1 << "\n";
			for (string str : dblist) {
				if (str != Suffix) {
					dblistFile << str << "\n";
				}
			}
			dblistFile.close();
			loadDBList();
			return 1;
		}
		return 0;
	}
	else if (Type == "TABLE") {
		string Suffix;
		cin >> Suffix;
		for (int i = 0; i < Suffix.size(); i++) {
			Suffix[i] = tolower(Suffix[i]);
		}
		if (!IfTableExist(Suffix)) {
			cout << " Table is Not Created Already!!\n";
			return 0;
		}
		else {
			ifstream dbFile(mainDirctory + "/" + CurntlyDB + "/" + CurntlyDB + ".db.km");
			int num;
			dbFile >> num;
			vector<string> ExTable(num);
			for (int i = 0; i < num; i++) {
				dbFile >> ExTable[i];
			}
			dbFile.close();
			ofstream odbFile(mainDirctory + "/" + CurntlyDB + "/" + CurntlyDB + ".db.km");
			odbFile << num - 1 << "\n";
			for (int i = 0; i < num - 1; i++) {
				if (ExTable[i] != Suffix) {
					odbFile << ExTable[i]<<"\n";
				}
			}
			odbFile.close();
			string str = mainDirctory + "/" + CurntlyDB + "/" + Suffix + ".t.km";
			char* ch = &str[0];
			LPCSTR Pth = ch;
			DeleteFileA(Pth);
			DeleteSubTable(Suffix, CurntlyDB);
			return 1;

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
				return 1;
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

bool INSERT() {

	string Suffix;
	cin >> Suffix;
	
	for (int j = 0; j < Suffix.size();j++) {
		Suffix[j] = tolower(Suffix[j]);
		
	}

	if (IfTableExist(Suffix)) {
		string val;
		cin >> val;
		if (val.size() != 6) {

			SetConsoleTextAttribute(hConsole, 12);
			cout << "syntax error\n";
			SetConsoleTextAttribute(hConsole, 15);
			return 0;
		}
	
		for (int j = 0; j < val.size();j++) {
			val[j] = toupper(val[j]);
		}
		if (val == "VALUES") {
			char ch;
			cin >> ch;
			if (ch != '(') {
				SetConsoleTextAttribute(hConsole, 12);
				cout << "syntax error\n";
				SetConsoleTextAttribute(hConsole, 15);
				return 0;
			}
			int sz;
			ifstream TbFile(mainDirctory + "/" + CurntlyDB + "/" + Suffix + ".t.km");
			TbFile >> sz;
			vector<string>v, TYlist(sz);
			string str;
			for (int i = 0; i < sz; i++) {
				TbFile >> str >> TYlist[i];
			}
			while (v.size() < sz-1) {
				cin >> str;
				if (str[str.size() - 1] != ',') {
					SetConsoleTextAttribute(hConsole, 12);
					cout << "syntax or input error\n";
					SetConsoleTextAttribute(hConsole, 15);
					return 0;
				}
				str.pop_back();
				v.push_back(str);
			}
			cin >> str;
			v.push_back(str);
			if (v[v.size() - 1][v[v.size() - 1].size() - 1] != ')') {
				SetConsoleTextAttribute(hConsole, 12);
				cout << "syntax error\n";
				SetConsoleTextAttribute(hConsole, 15);
				return 0;
			}
			v[v.size()-1].pop_back();
			for (int i = 0; i < sz; i++) {

				v[i]=inputDataType(v[i], TYlist[i]);
				if (v[i] == "") {
					return 0;
				}
			}
			int primIdx;
			TbFile >> primIdx;
			TbFile.close();
			int packNum=HashIt(v[primIdx]);
			vector<vector<string>>oldData;
			int datalen = 0;
			ifstream subfile;
			subfile.open(mainDirctory + "/" + CurntlyDB + "/" + Suffix+ "." + to_string(packNum) + ".t.km");
			if (subfile) {
				subfile >> datalen;
				vector<vector<string>>inoldData(datalen, vector<string>(sz));
				for (int i = 0; i < datalen; i++) {
					for (int j = 0; j < sz; j++) {
						subfile >> inoldData[i][j];
					}
				}
				oldData = inoldData;
			}
			subfile.close();
			for (int i = 0; i < datalen; i++) {
				if (oldData[i][primIdx] == v[primIdx]) {

					SetConsoleTextAttribute(hConsole, 12);
					cout << "Duplicate primary key\n";
					SetConsoleTextAttribute(hConsole, 15);
					return 0;
				}
			}
			ofstream Osubfile;
			Osubfile.open(mainDirctory + "/" + CurntlyDB + "/" + Suffix + "." + to_string(packNum) + ".t.km");
			Osubfile << oldData.size() + 1 << '\n';
			if (oldData.size()!=0) {
				for (int i = 0; i < datalen; i++) {
					for (int j = 0; j < sz; j++) {
						Osubfile <<oldData[i][j]<<" ";
					}
					Osubfile << "\n";
				}
			}

			for (int j = 0; j < sz; j++) {
				Osubfile << v[j] << " ";
			}
			Osubfile << "\n";
			Osubfile.close();

			SetConsoleTextAttribute(hConsole, 10);
			cout << "Done\n";
			SetConsoleTextAttribute(hConsole, 15);

			return 1;

		}

		SetConsoleTextAttribute(hConsole, 12);
		cout << "syntax error\n";
		SetConsoleTextAttribute(hConsole, 15);
		return 0;
	}

	return 0;
}
bool KMDELETE( ) {


	return 0;
}
bool UPDATE() {


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

void ShTables() {
	ifstream dbFile(mainDirctory + "/" + CurntlyDB + "/" + CurntlyDB + ".db.km");
	int num;
	dbFile >> num;
	if (num != 0) {
		cout << "Tables List\n";
		string ExTable;
		for (int i = 0; i < num; i++) {
			dbFile >> ExTable;
			cout << ExTable << "\n";
		}
	}
	else {
		cout << "No Tables\n";
	}
}

void setup() {
	SetConsoleTextAttribute(hConsole, 15);
	ifstream ifile;
	ifile.open(mainDirctory + ".km");
	if (ifile) {
		ifile.close();
	}
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
			else if (Suffix=="TABLES") {
				ShTables();
			}
			else {
				SetConsoleTextAttribute(hConsole, 12);
				cout << "syntax error\n";
				SetConsoleTextAttribute(hConsole, 15);
			}
		}
		else if (Quary == "USE") {
			if (USE()) {
				cout << CurntlyDB << " is in use now\n";
			}
			else {
				cout <<"Not in the databases list !!\n";
			}
		}
		else {
			cout << "syntax error\n";
		}
	}

}

