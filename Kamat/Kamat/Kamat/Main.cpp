/*
* this is a lexical for Kamat Structured Query Language (SQL)
* it was writen by Najm Adin
*/


#include<iostream>
#include<string>
#include<vector>
#include<utility>
#include"CreateTable.cpp"



using namespace std;

bool SELECT() {
	

	return 0;
}

bool DROP( ) {
	

	return 0;
}
bool CREATE( ) {
	
	string Type;
	cin >> Type;
	if (Type == "DATABASE") {

	}
	if (Type == "TABLE") {

		string Suffix;
		cin >> Suffix;
		bool b = 1;
		for (int i = 0; i < Suffix.size(); i++) {
			if (Suffix[i] != '.' && Suffix[i] != '_' && !isalpha(Suffix[i]) && !isdigit(Suffix[i])) {
				b = 0;
				break;
			}
		}
		if (b == 0) {
			cout << "Table name should not to contain special character\n";
			return 0;
		}
		else {
			vector<pair<string, string>> ColumnAndType;
			
			Create_Table();
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






int main() {



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
		else {
			cout << "syntax error\n";
		}
	}

}

