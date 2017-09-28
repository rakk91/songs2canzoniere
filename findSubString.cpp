#include <string>

using namespace std;

string findSubString ( string where2search, string str1, string str2 = "" ){
    size_t pos=where2search.find(str1);
    if ( pos!=string::npos ){ // start find search
        size_t end = where2search.find(str2);
        pos = pos + str1.length();
        string sub_string = where2search.substr(pos, end - pos);
        return sub_string;
    };
    return string();
}

string findSubString ( string where2search, size_t &pos, string str1, string str2 = ""){
    pos=where2search.find(str1,pos);
    if ( pos!=string::npos ){ // start find search
        size_t end = where2search.find(str2,pos);
        pos = pos + str1.length();
        string sub_string = where2search.substr(pos, end - pos);
        return sub_string;
    };
    return string();
}
