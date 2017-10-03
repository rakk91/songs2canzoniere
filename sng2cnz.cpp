#include <fstream>
#include <iostream>
#include <deque>
#include "Song.h"


using namespace std;


string findSubString ( string where2search, string str1, string str2 = "" );
string findSubString ( string where2search, size_t &pos, string str1, string str2 = "");




int main (int argc, char* argv[]){
    string fileName = argv[1];
    string fileNameOut = argv[2];
    fileNameOut += "/" + fileName;
//    string root = fileName.substr(0, (fileName.find_last_of(".tex")-3));
//    string fileNameOut = root + "2" + ".tex";
    ifstream inFile  (fileName);    //apro il file da modificare
    ofstream outFile (fileNameOut);
    Song song;
    song.read_from_file(inFile);
    // ora che ho memorizzato tutto analizzo.
    song.analyze();

    string tmp;
    cout << "_________________" << endl;
    cout << "Canzone elaborata: " <<  song.getName() << endl;
    cout << "Salvata in: "      <<  fileNameOut << endl;
    cout << "Gli autori sono: " <<  song.getAuthor() << endl;
//    cout << "La canzone presenta linee n.: " << song.line.size()<< endl;
    cout << "Album: "           << endl;
//    getline(cin, tmp);
//    song.fillAlbum(tmp);
//    cout << "Famiglia: ";
//    getline(cin, tmp);
    song.fillFamiglia("altre");
//    cout << "Gruppo: ";
//    getline(cin, tmp);
//    song.fillGruppo(tmp);
//    cout << "Momenti: ";
//    getline(cin, tmp);
//    song.fillMomenti(tmp);
    song.fillIdentificatore();
    cout << "_________________" << endl;
    outFile << "%titolo{"   << song.getName()     << "}" << endl;
    outFile << "%autore{"   << song.getAuthor()   << "}" << endl;
    outFile << "%album{"    << song.getAlbum()    << "}" << endl;
    outFile << "%tonalita{" << song.getTonalita() << "}" << endl;
    outFile << "%famiglia{" << song.getFamiglia() << "}" << endl;
    outFile << "%gruppo{"   << song.getGruppo()   << "}" << endl;
    outFile << "%momenti{"  << song.getMomenti()  << "}" << endl;
    outFile << "%identificatore{"<<song.getIndentificatore()<< "}"<< endl;
    outFile << "%data_revisione{"<<song.getDataRevisione()<< "}"<< endl;
    outFile << "%trascrittore{Francesco Raccanello}"<< endl;
    deque<string>::iterator pos;

    for (pos=song.newline.begin(); pos!=song.newline.end(); pos++){
         outFile << *pos <<endl;
    }

    return 0;
}
