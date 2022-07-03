#ifndef Kamat_h
#define Kamat_h

class KMT
{
public:
    KMT(string DataBaseName);
    int USE(string DataBaseName);
    string getError(int Error);
    void ShTables(vector<string> &TablesList);
    int Createdb(string name);
    int INSERT(string Table, vector<string> v);
    int SELECT(string Table, vector<vector<string>> &ResultData, string condition, string printerChk);
    int DEL(string Table,  string condition, string printerChk);
    int DROP(string Type,string name);
    int Create_Table(string Name,vector<pair<string, string>> ColumnAndType,unsigned int primKeyIdx);

private:
    string const userHome = getenv("HOME");
    string mainDirctory = userHome + "/Kamat/DataBases";
    vector<string> dblist;
    string CurntlyDB;
    vector<string> DataTypes = {"BOOL", "BOOLEAN", "INT", "INTEGER", "FLOAT"};
    string VarCh = "VARCHAR";
    string PKey[2] = {"primary", "key"};
    vector<string> ErrorList = {
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
        "Table name is duplicated","Prime Key out of range","Field name error","Data Type Name Error"
        };

    int HashIt(string s);
    void loadDBList();
    string RegularDataType(string TY);
    bool IfTableExist(string name);
    string inputDataType(string input, string Type);
    bool DeleteSubTable(string table, string db);
    bool addTable(string name);
};

#endif