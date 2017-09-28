#include <fstream>
#include <iostream>
#include <deque>
#include "Song.h"


using namespace std;


string findSubString ( string where2search, string str1, string str2 = "" );
string findSubString ( string where2search, size_t &pos, string str1, string str2 = "");




int main (int argc, char* argv[]){
    string fileName = argv[1];
    string root = fileName.substr(0, (fileName.find_last_of(".tex")-3));
    string fileNameOut = root + "2" + ".tex";
    ifstream inFile  (fileName);    //apro il file da modificare
    ofstream outFile (fileNameOut);
    Song song;
    string line;
    int i=0;
    while ( getline(inFile, line) ){
        song.line.push_back(line);
        string temp_name;
        if ( (song.chk_find_name()) && (findSubString( song.line[i], "\\beginsong{", "}") != "" ) ){    // find song name
            song.fillName( findSubString( song.line[i], "\\beginsong{", "}") );
            song.switch_find_name();
        }
        if ( song.chk_find_author() ){  // find song author(s)
            song.fillAuthor(findSubString( song.line[i], "by={", "}" ));
            if (song.getAuthor() != "" ){
                song.switch_find_author();
            }
        }
        if ( song.chk_find_tonalita() ){ //trova la tonalita'
            if ( song.line[i].find("\\[")!= string::npos ){ //se trova un accordo
                song.fillTonalita( findSubString( song.line[i], "\\[","]") );
                song.switch_find_tonalita();
            }
        }
        i++;
    }
    
    // ora che ho memorizzato tutto analizzo.
    deque<string>::iterator pos;
    pos = song.line.begin();
    bool not_nolyrics;
    for (pos=song.line.begin(); pos < song.line.end(); pos++){
        not_nolyrics = (((pos->find("\\beginverse")!=string::npos)&&(pos->find("*")==string::npos))||(pos->find("\\beginchorus")!=string::npos));
        if (song.chk_find_intro()&&not_nolyrics)
            song.switch_find_intro();   //disattiva la sostituzione per l'intro
        if (pos->find("\\beginverse*")!=string::npos){
//            cout<<"trovato un \\beginverse*"<<endl;
           deque<string> chords;
           while (pos->find("\\endverse")==string::npos){   //finché non arrivo alla fine di \endverse
               *pos = song.replaceChords (*pos);
               for (size_t chr = pos->find("\\["); chr < string::npos; chr=pos->find("\\[",chr) ){
                   chords.push_back( "\\["+findSubString(*pos, chr, "\\[", "]")+"]" );    //memorizzo gli accordi
               }
//               cout << "\t\t\t Analizzato (if \\beginverse* -> while \\endverse) " << *pos <<endl;
               pos++;
           }
//            cout << "Uscito dal while" << endl;
           string nolyrics = "{\\nolyrics";
           string chordsline;
           for (int i=0; i < chords.size(); i++)
               chordsline = chordsline +chords[i] + " ";
           nolyrics = nolyrics + " " + chordsline + "}";

           song.newline.push_back("\\ifchorded");
           song.newline.push_back("\\beginverse*");
           if(song.chk_find_intro())
               song.newline.push_back("\\vspace*{-0.8\\versesep}");
           else
               song.newline.push_back("\\vspace*{-\\versesep}");
           song.newline.push_back(nolyrics);
           if(song.chk_find_intro())
               song.newline.push_back("\\vspace*{-\\versesep}");
           song.newline.push_back("\\endverse");
           song.newline.push_back("\\fi");
           song.switch_find_intro();
//           cout << "\t\t\t Analizzato (if): " << *pos <<endl;
           pos++;
        }
        else if( ((pos->find("\\beginverse")!=string::npos)&&(pos->find("*")==string::npos)) || (pos->find("\\beginchorus")!=string::npos) ){
            while ((pos->find("\\endverse")==string::npos) && (pos->find("\\endchorus")==string::npos) ){   //finché non arrivo alla fine di \endverse o \endchorus
                song.newline.push_back(song.replaceChords(*pos));
//                cout << "\t\t\t Analizzato: " << *pos <<endl;
                pos++;
            }
        }
       song.newline.push_back(*pos);
//        cout << "\t\t\t NonAnalizzato: " << *pos <<endl;
//        cout << *pos << endl;
    }
    string tmp;
    cout << "_________________" << endl;
    cout << "Canzone elaborata: " <<  song.getName() << endl;
    cout << "Salvata in: "      <<  fileNameOut << endl;
    cout << "Gli autori sono: " <<  song.getAuthor() << endl;
    cout << "La canzone presenta linee n.: " << song.line.size()<< endl;
    cout << "Album: ";
    getline(cin, tmp);
    song.fillAlbum(tmp);
    cout << "Famiglia: ";
    getline(cin, tmp);
    song.fillFamiglia(tmp);
    cout << "Gruppo: ";
    getline(cin, tmp);
    song.fillGruppo(tmp);
    cout << "Momenti: ";
    getline(cin, tmp);
    song.fillMomenti(tmp);
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
    for (pos=song.newline.begin(); pos!=song.newline.end(); pos++){
         outFile << *pos <<endl;
    }

    return 0;
}
