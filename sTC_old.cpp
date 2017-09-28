#include <fstream>
#include <iostream>
#include <deque>


using namespace std;

struct songStruct{
    string name;
    string author;
    string album;
    string tonalita;
    string famiglia;
    string gruppo;
    string momenti;
    string identificatore;
    string data_revisione;
    bool notFindName;
    bool notFindAuthor;
    bool notFindTonalita;
    bool findIntro;
    deque<string> line;
    deque<string> newline;
};

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





//deque<string> findMultiSubString ( string where2search, string str1, string str2 = ""){
//    deque<string> dqString;
//    for (size_t pos=where2search.find(str1); pos!=string::npos; pos=where2search.find(str1, pos+1)){
//        pos+=2; // mi posizione sull'inizio dell'accordo
//        size_t end = where2search.find(str2,pos);
//        dqString.push_back( where2search.substr(pos, end-pos) );
//    }
//    return dqString;
//}

string convertChords (string accordo){
    if ( accordo == "DO"  ) return "C";
    if ( accordo == "RE"  ) return "D";
    if ( accordo == "MI"  ) return "E";
    if ( accordo == "FA"  ) return "F";
    if ( accordo == "SOL" ) return "G";
    if ( accordo == "LA"  ) return "A";
    if ( accordo == "SI"  ) return "B";

    if ( accordo == "A"   ) return "LA";
    if ( accordo == "B"   ) return "SI";
    if ( accordo == "C"   ) return "DO";
    if ( accordo == "D"   ) return "RE";
    if ( accordo == "E"   ) return "MI";
    if ( accordo == "F"   ) return "FA";
    if ( accordo == "G"   ) return "SOL";

    return string();

}

string replaceChords( string line ){
    size_t ch_pos;
    string delim1 = "\\[";
    string delim2 = "]";
    string oldChord, newChord;
    string chords[] = {"DO","RE","MI", "FA", "SOL", "LA", "SI"};
    for ( ch_pos=line.find(delim1); ch_pos < string::npos; ch_pos=line.find(delim1,ch_pos)){ //spanna la line cercando l'inizio di un accordo
        oldChord = findSubString( line, ch_pos, delim1, delim2 );
        for (int i=0; i < 7; i++)
            if (oldChord.find(chords[i])!=string::npos){
                oldChord = oldChord.substr(0, chords[i].length() );
                newChord = convertChords( oldChord );
//                cout << "oldChord= " << oldChord << " newChord= " << newChord << endl;
                line = line.replace(ch_pos, oldChord.length(), newChord);
                break;
            }
    }
    return line;
}

int main (int argc, char* argv[]){
    songStruct song;
    song.notFindName     = true;
    song.notFindAuthor   = true;
    song.notFindTonalita = true;
    song.findIntro=true;
    string fileName = argv[1];
    string root = fileName.substr(0, (fileName.find_last_of(".tex")-3));
    string fileNameOut = root + "2" + ".tex";
    ifstream inFile  (fileName);    //apro il file da modificare
    ofstream outFile (fileNameOut);
    string line;
    int i=0;
    while ( getline(inFile, line) ){
        song.line.push_back(line);
        if ( song.notFindName ){    // find song name
            song.name = findSubString( song.line[i], "\\beginsong{", "}" );
            if (song.name != "" ){
                song.notFindName = false;
            }
        }
        
        if ( song.notFindAuthor ){  // find song author(s)
            song.author = findSubString( song.line[i], "by={", "}" );
            if (song.author != "" ){
                song.notFindAuthor = false;
            }
        }
        if ( song.notFindTonalita ){ //trova la tonalita'
            if ( song.line[i].find("\\[")!= string::npos ){ //se trova un accordo
                song.tonalita = findSubString( song.line[i], "\\[","]");
                song.notFindTonalita = false;
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
        if (song.findIntro&&not_nolyrics)
            song.findIntro=false;   //disattiva la sostituzione per l'intro
        if (pos->find("\\beginverse*")!=string::npos){
//            cout<<"trovato un \\beginverse*"<<endl;
           deque<string> chords;
           while (pos->find("\\endverse")==string::npos){   //finché non arrivo alla fine di \endverse
               *pos = replaceChords (*pos);
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
           if(song.findIntro)
               song.newline.push_back("\\vspace*{-0.8\\versesep}");
           else
               song.newline.push_back("\\vspace*{-\\versesep}");
           song.newline.push_back(nolyrics);
           if(song.findIntro)
               song.newline.push_back("\\vspace*{-\\versesep}");
           song.newline.push_back("\\endverse");
           song.newline.push_back("\\fi");
           song.findIntro=false;
//           cout << "\t\t\t Analizzato (if): " << *pos <<endl;
           pos++;
        }
        else if( ((pos->find("\\beginverse")!=string::npos)&&(pos->find("*")==string::npos)) || (pos->find("\\beginchorus")!=string::npos) ){
            while ((pos->find("\\endverse")==string::npos) && (pos->find("\\endchorus")==string::npos) ){   //finché non arrivo alla fine di \endverse o \endchorus
                song.newline.push_back(replaceChords(*pos));
//                cout << "\t\t\t Analizzato: " << *pos <<endl;
                pos++;
            }
        }
       song.newline.push_back(*pos);
//        cout << "\t\t\t NonAnalizzato: " << *pos <<endl;
//        cout << *pos << endl;
    }
    
    cout << endl;
    cout << "_________________" << endl;
    cout << "Canzone elaborata: " <<  song.name << endl;
    cout << "Salvata in: "      <<  fileNameOut << endl;
    cout << "Gli autori sono: " <<  song.author << endl;
    cout << "La canzone presenta linee n.: " << song.line.size()<< endl;
    cout << "Album: ";
    cin  >> song.album;
    cout << "Famiglia: ";
    cin  >> song.famiglia;
    cout << "Gruppo: ";
    cin  >> song.gruppo;
    cout << "Momenti: ";
    cin  >> song.momenti;
    
    cout << "_________________" << endl;
    outFile << "%titolo{" << song.name << "}"   << endl;
    outFile << "%autore{" << song.author << "}" << endl;
    outFile << "%album{"  << "}"                << endl;
    outFile << "%tonalita{" << song.tonalita << "}" << endl;
    outFile << "%famiglia{}"                    << endl;
    outFile << "%gruppo{}"                      << endl;
    outFile << "%momenti{}"                     << endl;
    outFile << "%identificatore{}"              << endl;
    outFile << "%data_revisione{}"              << endl;
    outFile << "%trascrittore{Francesco Raccanello}" << endl;
    for (pos=song.newline.begin(); pos!=song.newline.end(); pos++){
         outFile << *pos <<endl;
    }

    return 0;
}
