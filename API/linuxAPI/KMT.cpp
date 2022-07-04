#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <utility>
#include <stdio.h>
#include <fstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <filesystem>
#include <unistd.h>

using namespace std;
class KMT
{
public:
	int USE(string DataBaseName)
	{

		for (int i = 0; i < DataBaseName.size(); i++)
		{
			DataBaseName[i] = tolower(DataBaseName[i]);
		}
		for (string str : dblist)
		{
			if (str == DataBaseName)
			{
				CurntlyDB = DataBaseName;
				return 0;
			}
		}
		return 1;
	}

	KMT(string DataBaseName)
	{
		userHome = getenv("HOME");
		mainDirctory = userHome + "/Kamat/DataBases";
		DataTypes = {"BOOL", "BOOLEAN", "INT", "INTEGER", "FLOAT"};
		VarCh = "VARCHAR";
		PKey[0] = "primary";
		PKey[1] = "key";
		ErrorList = {
			// 0         1                       2
			"non", "DataBase not found", "DataBase Name is duplicated",
			//                      3
			"Databese name should not to contain special character!!",
			//      4               5                    6
			"Table not Exist", "Input Type Error", "Duplicate primary key",
			//      7                       8                           9
			"Field name not found", "not capable in this version", "No data to delete",
			//                          10
			"Table name should not to contain special character",
			//           11                     12                      13                  14
			"Table name is duplicated", "Prime Key out of range", "Field name error", "Data Type Name Error"};

		loadDBList();

		int x = USE(DataBaseName);
		if (x)
		{
			cout << DataBaseName + " is not found use app to create it\n";
			exit(1);
		}
	}

	string getError(int Error)
	{
		return ErrorList[Error];
	}

	void ShowTables(vector<string> &TablesList)
	{
		ifstream dbFile(mainDirctory + "/" + CurntlyDB + "/" + CurntlyDB + ".db.km");
		int num;
		dbFile >> num;
		string ExTable;
		for (int i = 0; i < num; i++)
		{
			dbFile >> ExTable;
			TablesList.push_back(ExTable);
		}
		dbFile.close();
	}

	int Createdb(string name)
	{
		for (int i = 0; i < name.size(); i++)
		{
			name[i] = tolower(name[i]);
		}

		for (int i = 0; i < name.size(); i++)
		{
			if (name[i] != '_' && !isalpha(name[i]) && !isdigit(name[i]))
			{
				return 3;
			}
		}

		for (string DBName : dblist)
		{
			if (name == DBName)
			{
				return 2;
			}
		}
		ofstream dblistfile(mainDirctory + ".km");
		dblistfile << dblist.size() + 1 << "\n";
		for (string DBName : dblist)
		{
			dblistfile << DBName << "\n";
		}
		dblist.push_back(name);
		dblistfile << name << "\n";
		dblistfile.close();
		string str = mainDirctory + "/" + name;
		char *arr;
		arr = &str[0];
		mkdir(arr, 0777);
		ofstream dbFile(mainDirctory + "/" + name + "/" + name + ".db.km");
		dbFile << "0\n";
		dbFile.close();
		return 0;
	}

	int INSERT(string Table, vector<string> v)
	{

		for (int j = 0; j < Table.size(); j++)
		{
			Table[j] = tolower(Table[j]);
		}

		if (IfTableExist(Table))
		{

			int sz;
			ifstream TbFile(mainDirctory + "/" + CurntlyDB + "/" + Table + ".t.km");
			TbFile >> sz;
			vector<string> TYlist(sz);
			string str;
			for (int i = 0; i < sz; i++)
			{
				TbFile >> str >> TYlist[i];
			}
			for (int i = 0; i < sz; i++)
			{

				v[i] = inputDataType(v[i], TYlist[i]);
				if (v[i] == "")
				{
					return 5;
				}
			}
			int primIdx;
			TbFile >> primIdx;
			TbFile.close();
			int packNum = HashIt(v[primIdx]);
			vector<vector<string>> oldData;
			int datalen = 0;
			ifstream subfile;
			subfile.open(mainDirctory + "/" + CurntlyDB + "/" + Table + "." + to_string(packNum) + ".t.km");
			if (subfile)
			{
				subfile >> datalen;
				vector<vector<string>> inoldData(datalen, vector<string>(sz));
				for (int i = 0; i < datalen; i++)
				{
					for (int j = 0; j < sz; j++)
					{
						subfile >> inoldData[i][j];
					}
				}
				oldData = inoldData;
			}
			subfile.close();
			for (int i = 0; i < datalen; i++)
			{
				if (oldData[i][primIdx] == v[primIdx])
				{
					return 6;
				}
			}
			ofstream Osubfile;
			Osubfile.open(mainDirctory + "/" + CurntlyDB + "/" + Table + "." + to_string(packNum) + ".t.km");
			Osubfile << oldData.size() + 1 << '\n';
			if (oldData.size() != 0)
			{
				for (int i = 0; i < datalen; i++)
				{
					for (int j = 0; j < sz; j++)
					{
						Osubfile << oldData[i][j] << " ";
					}
					Osubfile << "\n";
				}
			}

			for (int j = 0; j < sz; j++)
			{
				Osubfile << v[j] << " ";
			}
			Osubfile << "\n";
			Osubfile.close();
			return 0;
		}

		return 4;
	}

	int SELECT(string Table, vector<vector<string>> &ResultData, string condition, string printerChk)
	{
		// SELECT * FROM Customers;

		for (int i = 0; i < Table.size(); i++)
		{
			Table[i] = tolower(Table[i]);
		}
		if (!IfTableExist(Table))
		{
			return 4;
		}
		int sz;
		ifstream TbFile(mainDirctory + "/" + CurntlyDB + "/" + Table + ".t.km");
		TbFile >> sz;
		vector<pair<string, string>> TYlist(sz);
		vector<string> TY(sz), NM(sz);
		for (int i = 0; i < sz; i++)
		{
			TbFile >> TYlist[i].first >> TYlist[i].second;
			NM[i] = TYlist[i].first;
			TY[i] = TYlist[i].second;
		}
		ResultData.push_back(NM);
		ResultData.push_back(TY);
		int PrimIdx;
		TbFile >> PrimIdx;
		TbFile.close();

		if (condition != "non")
		{

			int CondIdx = -1;

			for (int i = 0; i < condition.size(); i++)
			{
				condition[i] = tolower(condition[i]);
			}
			for (int i = 0; i < sz; i++)
			{
				if (TYlist[i].first == condition)
				{
					CondIdx = i;
				}
			}

			if (CondIdx == -1)
			{
				return 7;
			}
			if (CondIdx != PrimIdx)
			{
				return 8;
			}

			printerChk = inputDataType(printerChk, TYlist[CondIdx].second);
			if (printerChk == "")
			{
				return 5;
			}
			int packNum = HashIt(printerChk);
			vector<string> inSol(sz);
			int datalen = 0;
			ifstream subfile;
			subfile.open(mainDirctory + "/" + CurntlyDB + "/" + Table + "." + to_string(packNum) + ".t.km");
			if (subfile)
			{
				subfile >> datalen;
				for (int i = 0; i < datalen; i++)
				{
					for (int j = 0; j < sz; j++)
					{
						subfile >> inSol[j];
					}
					if (inSol[PrimIdx] == printerChk)
					{
						ResultData.push_back(inSol);
						break;
					}
				}
				subfile.close();
				sort(ResultData.begin()+2,ResultData.end());
				return 0;
			}
			sort(ResultData.begin()+2,ResultData.end());
			return 0;
		}

		vector<string> solin(sz);

		for (int i = 0; i < 101; i++)
		{
			ifstream SubTbFile(mainDirctory + "/" + CurntlyDB + "/" + Table + "." + to_string(i) + ".t.km");
			if (SubTbFile)
			{
				int x;
				SubTbFile >> x;
				for (int k = 0; k < x; k++)
				{
					for (int j = 0; j < sz; j++)
					{
						SubTbFile >> solin[j];
					}
					ResultData.push_back(solin);
				}
				SubTbFile.close();
			}
		}
		sort(ResultData.begin()+2,ResultData.end());
		return 0;
	}

	int DEL(string Table, string condition, string printerChk)
	{
		for (int i = 0; i < Table.size(); i++)
		{
			Table[i] = tolower(Table[i]);
		}
		if (!IfTableExist(Table))
		{
			return 4;
		}
		int sz;
		ifstream TbFile(mainDirctory + "/" + CurntlyDB + "/" + Table + ".t.km");
		TbFile >> sz;
		vector<pair<string, string>> TYlist(sz);
		for (int i = 0; i < sz; i++)
		{
			TbFile >> TYlist[i].first >> TYlist[i].second;
		}
		int PrimIdx;
		TbFile >> PrimIdx;
		TbFile.close();

		int CondIdx = -1;
		for (int i = 0; i < condition.size(); i++)
		{
			condition[i] = tolower(condition[i]);
		}
		for (int i = 0; i < sz; i++)
		{
			if (TYlist[i].first == condition)
			{
				CondIdx = i;
			}
		}

		if (CondIdx == -1)
		{
			return 7;
		}
		if (CondIdx != PrimIdx)
		{
			return 8;
		}

		printerChk = inputDataType(printerChk, TYlist[CondIdx].second);
		if (printerChk == "")
		{
			return 5;
		}
		int packNum = HashIt(printerChk);
		int datalen = 0, delIdx = -1;
		ifstream subfile;
		subfile.open(mainDirctory + "/" + CurntlyDB + "/" + Table + "." + to_string(packNum) + ".t.km");
		if (subfile)
		{
			subfile >> datalen;
			vector<vector<string>> oldData(datalen, vector<string>(sz));
			for (int i = 0; i < datalen; i++)
			{
				for (int j = 0; j < sz; j++)
				{
					subfile >> oldData[i][j];
				}
				if (oldData[i][PrimIdx] == printerChk)
				{
					delIdx = i;
				}
			}
			subfile.close();
			if (delIdx == -1)
			{
				return 9;
			}
			ofstream Osubfile;
			Osubfile.open(mainDirctory + "/" + CurntlyDB + "/" + Table + "." + to_string(packNum) + ".t.km");
			Osubfile << oldData.size() - 1 << '\n';
			if (oldData.size() != 0)
			{
				for (int i = 0; i < datalen; i++)
				{
					for (int j = 0; j < sz; j++)
					{
						Osubfile << oldData[i][j] << " ";
					}
					Osubfile << "\n";
				}
			}
			Osubfile.close();
			return 0;
		}
		return 9;
	}

	int DROP(string Type, string name)
	{
		/*remove_all("myDirectory");
		remove(fileName);
		*/
		for (int i = 0; i < Type.size(); i++)
		{
			Type[i] = toupper(Type[i]);
		}

		if (Type == "DATABASE")
		{

			for (int i = 0; i < name.size(); i++)
			{
				name[i] = tolower(name[i]);
			}

			bool b = 0;
			for (string str : dblist)
			{
				if (str == name)
				{
					b = 1;
				}
			}

			if (!b)
			{
				return 1;
			}
			else
			{
				string str = mainDirctory + "/" + name + "/" + name + ".db.km";
				char *ch = &str[0];
				int sz;
				ifstream dbfile(ch);
				dbfile >> sz;
				string Table;

				for (int i = 0; i < sz; i++)
				{
					dbfile >> Table;
					DeleteSubTable(Table, name);
					str = mainDirctory + "/" + name + "/" + Table + ".t.km";
					ch = &str[0];
					remove(ch);
				}
				dbfile.close();
				str = mainDirctory + "/" + name + "/" + name + ".db.km";
				ch = &str[0];
				remove(ch);
				str = mainDirctory + "/" + name;
				ch = &str[0];
				rmdir(ch);
				ofstream dblistFile(mainDirctory + ".km");
				dblistFile << dblist.size() - 1 << "\n";
				for (string str : dblist)
				{
					if (str != name)
					{
						dblistFile << str << "\n";
					}
				}
				dblistFile.close();
				loadDBList();
				return 0;
			}
		}
		else if (Type == "TABLE")
		{

			for (int i = 0; i < name.size(); i++)
			{
				name[i] = tolower(name[i]);
			}
			if (!IfTableExist(name))
			{
				return 4;
			}
			else
			{
				ifstream dbFile(mainDirctory + "/" + CurntlyDB + "/" + CurntlyDB + ".db.km");
				int num;
				dbFile >> num;
				vector<string> ExTable(num);
				for (int i = 0; i < num; i++)
				{
					dbFile >> ExTable[i];
				}
				dbFile.close();
				ofstream odbFile(mainDirctory + "/" + CurntlyDB + "/" + CurntlyDB + ".db.km");
				odbFile << num - 1 << "\n";
				for (int i = 0; i < num - 1; i++)
				{
					if (ExTable[i] != name)
					{
						odbFile << ExTable[i] << "\n";
					}
				}
				odbFile.close();
				string str = mainDirctory + "/" + CurntlyDB + "/" + name + ".t.km";
				char *ch = &str[0];

				remove(ch);
				DeleteSubTable(name, CurntlyDB);
				return 0;
			}
		}

		return 5;
	}

	int Create_Table(string Name, vector<pair<string, string>> ColumnAndType, unsigned int primKeyIdx)
	{

		for (int i = 0; i < Name.size(); i++)
		{
			Name[i] = tolower(Name[i]);
		}
		bool b = 1;
		for (int i = 0; i < Name.size(); i++)
		{
			if (Name[i] != '_' && !isalpha(Name[i]) && !isdigit(Name[i]))
			{
				b = 0;
				break;
			}
		}
		if (b == 0)
		{
			return 10;
		}
		else
		{
			if (IfTableExist(Name))
			{
				return 11;
			}
			else
			{
				if (primKeyIdx >= ColumnAndType.size())
				{
					return 12;
				}

				string NM, TY;
				for (int k = 0; k < ColumnAndType.size(); k++)
				{
					NM = ColumnAndType[k].first;
					TY = ColumnAndType[k].second;
					if (!isalpha(NM[0]) && NM[0] != '_')
					{
						return 13;
					}
					for (int i = 1; i < NM.size(); i++)
					{
						if (!isalpha(NM[i]) && NM[i] != '_' && !isdigit(NM[i]))
						{
							return 13;
						}
					}
					for (int i = 0; i < NM.size(); i++)
					{
						NM[i] = tolower(NM[i]);
					}

					TY = RegularDataType(TY);
					if (TY == "non")
					{
						return 14;
					}

					ColumnAndType[k].first = NM;
					ColumnAndType[k].second = TY;
				}
				addTable(Name);
				ofstream TbFile(mainDirctory + "/" + CurntlyDB + "/" + Name + ".t.km");
				TbFile << ColumnAndType.size() << "\n";
				for (int i = 0; i < ColumnAndType.size(); i++)
				{
					TbFile << ColumnAndType[i].first << " " << ColumnAndType[i].second << "\n";
				}

				TbFile << primKeyIdx;

				TbFile.close();
				return 0;
			}
		}
	}

	void ShowDataBases(vector<string> &DataBasesList){
		loadDBList();
		DataBasesList=dblist;
	}

private:
	string userHome;
	string mainDirctory;
	vector<string> dblist;
	string CurntlyDB;
	vector<string> DataTypes;
	string VarCh;
	string PKey[2];
	vector<string> ErrorList;

	int HashIt(string s)
	{
		long long res = 0;
		for (int i = 0; i < s.size(); i++)
		{
			res += s[i];
		}
		return (res % 101);
	}

	void loadDBList()
	{
		dblist.clear();
		ifstream dblistfile(mainDirctory + ".km");
		int num;
		dblistfile >> num;
		string exdb;
		while (num--)
		{
			dblistfile >> exdb;
			dblist.push_back(exdb);
		}
		dblistfile.close();
	}

	string RegularDataType(string TY)
	{
		if (TY.size() < 3)
		{
			return "non";
		}

		string DTY = "";
		for (int i = 0; i < TY.size(); i++)
		{
			DTY += toupper(TY[i]);
		}
		for (int i = 0; i < DataTypes.size(); i++)
		{
			if (DTY == DataTypes[i])
			{
				return DataTypes[i];
			}
		}
		int idx = 0;
		for (int i = 0; i < TY.size(); i++)
		{
			if (idx < 7)
			{
				if (VarCh[idx] != toupper(TY[i]))
				{
					return "non";
				}
				idx++;
			}
			else
			{
				if (TY[i] != '(')
				{
					return "non";
				}
				i++;
				int num = 0;
				string strnum = "";
				for (; i < TY.size(); i++)
				{
					if (isdigit(TY[i]))
					{
						num *= 10;
						num += (TY[i] - '0');
					}
					else if (TY[i] == ')')
					{
						strnum = to_string(num);
						return (VarCh + '(' + strnum + ')');
					}
				}
			}
		}

		return "non";
	}

	bool IfTableExist(string name)
	{
		ifstream dbFile(mainDirctory + "/" + CurntlyDB + "/" + CurntlyDB + ".db.km");
		int num;
		dbFile >> num;
		string ExTable;
		for (int i = 0; i < num; i++)
		{
			dbFile >> ExTable;
			if (ExTable == name)
			{
				dbFile.close();
				return 1;
			}
		}
		dbFile.close();
		return 0;
	}

	string inputDataType(string input, string Type)
	{

		if (Type == DataTypes[0] || Type == DataTypes[1])
		{

			if (input == "0" || input == "false")
			{
				return "0";
			}
			else if (input == "1" || input == "true")
			{
				return "1";
			}
			else
			{
				return "";
			}
		}
		else if (Type == DataTypes[2] || Type == DataTypes[3])
		{
			string sol = "";
			int i = 0;
			if (input[0] == '-')
			{
				i = 1;
				sol += '-';
			}
			for (; i < input.size(); i++)
			{
				if (!isdigit(input[i]))
				{
					return "";
				}
				else
				{
					sol += input[i];
				}
			}
			return sol;
		}
		else if (Type == DataTypes[4])
		{
			int point = 0;
			string sol = "";
			int i = 0;
			if (input[0] == '-')
			{
				i = 1;
				sol += '-';
			}
			for (; i < input.size(); i++)
			{
				if (!isdigit(input[i]))
				{
					if (input[i] == '.' && point == 0)
					{
						point++;
					}
					else
					{
						return "";
					}
				}
				else
				{
					sol += input[i];
				}
			}
			return sol;
		}
		else if (Type[0] == 'V')
		{
			// VARCHAR
			if (input.size() < 3 || input[0] != '\'' || input[input.size() - 1] != '\'')
			{
				return "";
			}
			int len = 0;
			for (int i = 7; i < Type.size() - 1; i++)
			{
				len *= 10;
				len += (Type[i] - '0');
			}
			if (input.size() > (len + 2))
			{
				return "";
			}
			string sol = "";
			for (int i = 1; i < input.size() - 1; i++)
			{
				sol += input[i];
			}
			return sol;
		}
		return "";
	}

	bool DeleteSubTable(string table, string db)
	{
		string str = " ";
		char *ch = &str[0];
		for (int i = 0; i < 101; i++)
		{
			str = mainDirctory + "/" + db + "/" + table + "." + to_string(i) + ".t.km";
			ch = &str[0];
			remove(ch);
		}
		return 1;
	}

	bool addTable(string name)
	{
		ifstream dbFile(mainDirctory + "/" + CurntlyDB + "/" + CurntlyDB + ".db.km");
		int num;
		dbFile >> num;
		vector<string> ExTable(num, "");
		for (int i = 0; i < num; i++)
		{
			dbFile >> ExTable[i];
		}
		dbFile.close();
		ofstream dbFileW(mainDirctory + "/" + CurntlyDB + "/" + CurntlyDB + ".db.km");
		dbFileW << ExTable.size() + 1 << "\n";
		for (int i = 0; i < num; i++)
		{
			dbFileW << ExTable[i] << "\n";
		}
		dbFileW << name;
		dbFileW.close();
		return 0;
	}
};