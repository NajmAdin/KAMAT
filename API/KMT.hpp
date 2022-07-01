#ifndef Kamat_h
#define Kamat_h


class KMT
{
    public:
    KMT(string DataBaseName);
    int USE(string DataBaseName);
    string getError(int Error);
    private:
    string const mainDirctory = "/Kamat/DataBases";
    vector<string> dblist;
    string CurntlyDB;
    vector<string> DataTypes = {"BOOL", "BOOLEAN", "INT", "INTEGER", "FLOAT"};
    string VarCh = "VARCHAR";
    string PKey[2] = {"primary", "key"};
    //                          0           1           
    vector<string>ErrorList={"non","DataBase not found"};
};

#endif