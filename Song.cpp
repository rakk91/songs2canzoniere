#include "Song.h"
#include <iostream>
#include <fstream>
#include <locale>         // std::locale, std::tolower

std::string findSubString ( std::string where2search, std::string str1, std::string str2 = "" );
std::string findSubString ( std::string where2search, size_t &pos, std::string str1, std::string str2 = "");



Song::Song():
    notFindName     (true),
    notFindAuthor   (true),
    notFindTonalita (true),
    findIntro       (true) {}

Song::Song( std::string song_name ){
    name   = song_name;
    notFindAuthor   = true;
    notFindTonalita = true;
    findIntro       = true;
    fillIdentificatore();
}

Song::Song( std::string song_name, std::string song_author){
    name   = song_name;
    author = song_author;
    notFindAuthor   = true;
    notFindTonalita = true;
    findIntro       = true;
    fillAuthor(song_author);
    fillIdentificatore();
}

Song::~Song(){}

//void member

void Song::fillName(std::string song_name){name=song_name;}
void Song::fillAuthor(std::string song_author){author=song_author;}
void Song::fillAlbum(std::string song_album){album=song_album;}
void Song::fillTonalita(std::string song_tonalita){tonalita=song_tonalita;}
void Song::fillFamiglia(std::string song_famiglia){famiglia=song_famiglia;}
void Song::fillGruppo(std::string song_gruppo){gruppo=song_gruppo;}
void Song::fillMomenti(std::string song_momenti){momenti=song_momenti;}
void Song::fillIdentificatore(){
    std::string name_tmp;
    name_tmp = removeAccented(name);
    size_t pos = name_tmp.find(" ");
    while (pos != std::string::npos) {
        name_tmp.replace(pos, 1, "_");
        pos = name_tmp.find(" ",pos+1);
    }
    std::locale loc;
    for (std::string::size_type i=0; i<name_tmp.length();i++)
        identificatore+=std::tolower(name_tmp[i],loc);
}
void Song::fillIdentificatore(std::string song_identificatore){identificatore=song_identificatore;}
void Song::fillDataRevisione( std::string song_data_revisione){data_revisione=song_data_revisione;}
void switch_find_name();



void Song::switch_find_name(){
    notFindName = !notFindName;
}

void Song::switch_find_author(){
    notFindAuthor = !notFindAuthor;
}

void Song::switch_find_tonalita(){
    notFindTonalita = ! notFindTonalita;
}

void Song::switch_find_intro(){
    findIntro = !findIntro;
}



// typed member

std::string Song::getName(){return name;}
std::string Song::getAuthor(){return author;}
std::string Song::getAlbum(){return album;}
std::string Song::getTonalita(){return tonalita;}
std::string Song::getFamiglia(){return famiglia;}
std::string Song::getGruppo(){return gruppo;}
std::string Song::getMomenti(){return momenti;}
std::string Song::getIndentificatore(){return identificatore;}
std::string Song::getDataRevisione(){return data_revisione;}


bool Song::chk_find_name(){
    return notFindName;
}

bool Song::chk_find_author(){
    return notFindAuthor;
}

bool Song::chk_find_tonalita(){
    return notFindTonalita;
}

bool Song::chk_find_intro(){
    return findIntro;
}


void Song::read_from_file( std::ifstream &inFile){
    std::string linea;
    int i=0;
    while ( getline(inFile, linea) ){
        line.push_back(linea);
        std::string temp_name;
        if ( (chk_find_name()) && (findSubString( line[i], "\\beginsong{", "}") != "" ) ){    // find song name
            fillName( findSubString( line[i], "\\beginsong{", "}") );
            switch_find_name();
        }
        if ( chk_find_author() ){  // find song author(s)
            fillAuthor(findSubString( line[i], "by={", "}" ));
            if (getAuthor() != "" ){
                switch_find_author();
            }
        }
        if ( chk_find_tonalita() ){ //trova la tonalita'
            if ( line[i].find("\\[")!= std::string::npos ){ //se trova un accordo
                std::string tmp_chrd = findSubString( line[i], "\\[","]"); // trova il primo accordo completo di variazioni
                std::string rootChord;
                std::string chords[] = {"DO","RE","MI", "FA", "SOL", "LA", "SI"};
                size_t pos;
                if( (pos = tmp_chrd.find("&"))!=std::string::npos )
                    tmp_chrd.replace(pos, 1, "b");  //sosituisci & -> b
                for (int i=0; i < 7; i++)           // ricerca i possibili accordi (senza variazioni)
                    if (tmp_chrd.find(chords[i])!=std::string::npos){
                        rootChord = tmp_chrd.substr(0, chords[i].length() );    // estrae la radice del nome
                        break;
                    }
                tmp_chrd = tmp_chrd.replace(0, rootChord.length(), convertChords_lowercase( rootChord )); // sostituisci il nome minuscolo
                fillTonalita( tmp_chrd );
                switch_find_tonalita();
            }
        }
        i++;
    }
}

void Song::analyze(){
    std::deque<std::string>::iterator pos;
    pos = line.begin();
    bool not_nolyrics;
    for (pos=line.begin(); pos < line.end(); pos++){
        not_nolyrics = (((pos->find("\\beginverse")!=std::string::npos)&&(pos->find("*")==std::string::npos))||(pos->find("\\beginchorus")!=std::string::npos));
        // se l'intro e' gia' stato trovato disattiva la ricerca dell'intro.
        if (chk_find_intro()&&not_nolyrics)
            switch_find_intro();
        // se trova un \beginverse*
        if (pos->find("\\beginverse*")!=std::string::npos){
            std::deque<std::string> chords;
            while (pos->find("\\endverse")==std::string::npos){   //finché non arrivo alla fine di \endverse
                *pos = replaceChords (*pos);
                for (size_t chr = pos->find("\\["); chr < std::string::npos; chr=pos->find("\\[",chr) )
                    chords.push_back( "\\["+findSubString(*pos, chr, "\\[", "]")+"]" );    //memorizzo gli accordi
                pos++;
            }
        std::string nolyrics = "{\\nolyrics";
        std::string chordsline;
        for (int i=0; i < chords.size(); i++)
            chordsline = chordsline +chords[i] + " ";
        nolyrics = nolyrics + " " + chordsline + "}";
        newline.push_back("\\ifchorded");
        newline.push_back("\\beginverse*");
        if(chk_find_intro())
            newline.push_back("\\vspace*{-0.8\\versesep}");
        else
            newline.push_back("\\vspace*{-\\versesep}");
        newline.push_back(nolyrics);
        if(chk_find_intro())
            newline.push_back("\\vspace*{-\\versesep}");
        newline.push_back("\\endverse");
        newline.push_back("\\fi");
        switch_find_intro();
        //pos++;
        }
        // se trova una strofa od un ritornello -> copia tutto il suo contenuto fino a \endverse (compreso)
        else if( ((pos->find("\\beginverse")!=std::string::npos)&&(pos->find("*")==std::string::npos)) || (pos->find("\\beginchorus")!=std::string::npos) ){
            while ((pos->find("\\endverse")==std::string::npos) && (pos->find("\\endchorus")==std::string::npos) ){   //finché non arrivo alla fine di \endverse o \endchorus
                newline.push_back(replaceChords(*pos));
                pos++;
            }
            newline.push_back(*pos);
        }
        // se non è nessun'altra cosa, copia e basta.
        else newline.push_back(*pos);
    }
}





std::string Song::convertChords (std::string accordo){
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
    
    return std::string();
}

std::string Song::convertChords_lowercase (std::string accordo){
    if ( accordo == "DO"  ) return "Do";
    if ( accordo == "RE"  ) return "Re";
    if ( accordo == "MI"  ) return "Mi";
    if ( accordo == "FA"  ) return "Fa";
    if ( accordo == "SOL" ) return "Sol";
    if ( accordo == "LA"  ) return "La";
    if ( accordo == "SI"  ) return "Si";
    
    if ( accordo == "A"   ) return "La";
    if ( accordo == "B"   ) return "Si";
    if ( accordo == "C"   ) return "Do";
    if ( accordo == "D"   ) return "Re";
    if ( accordo == "E"   ) return "Mi";
    if ( accordo == "F"   ) return "Fa";
    if ( accordo == "G"   ) return "Sol";
    
    return std::string();
}

std::string Song::replaceChords( std::string line ){
    size_t ch_pos;
    std::string delim1 = "\\[";
    std::string delim2 = "]";
    std::string oldChord, newChord;
    std::string chords[] = {"DO","RE","MI", "FA", "SOL", "LA", "SI"};
    for ( ch_pos=line.find(delim1); ch_pos<std::string::npos; ch_pos=line.find(delim1,ch_pos) ){ //spanna la line cercando l'inizio di un accordo
        ch_pos=line.find(delim1,ch_pos);
        if ( ch_pos!=std::string::npos ){ // start find search
            size_t end = line.find(delim2,ch_pos);
            ch_pos = ch_pos + delim1.length();
            oldChord = line.substr(ch_pos, end - ch_pos);
        }
        for (int i=0; i < 7; i++)
            if (oldChord.find(chords[i])!=std::string::npos){
                oldChord = oldChord.substr(0, chords[i].length() );
                newChord = convertChords( oldChord );
//                std::cout << "oldChord= " << oldChord << " size " <<oldChord.length() << "\tnewChord= " << newChord << std::endl;
                line = line.replace(ch_pos, oldChord.length(), newChord);
//                std::cout << line << std::endl;
                break;
            }
    }
    return line;
}


std::string Song::removeAccented( std::string str ) {
    std::string newString;
    std::string::iterator chr = str.begin();
    const char*
    //   "ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖ×ØÙÚÛÜÝÞßàáâãäåæçèéêëìíîïðñòóôõö÷øùúûüýþÿ"
    tr = "AAAAAAECEEEEIIIIDNOOOOOx0UUUUYPsaaaaaaeceeeeiiiiOnooooo/0uuuuypy";
    while ( chr!=str.end() ) {
        unsigned char ch = *chr;
        std::cout << (int) ch << "-";
        if ( ch == 195 ) {                     // rimozione accenti
            ch = *(++chr);
            std::cout << (int) ch << std::endl;
            newString.push_back(tr[ ch-128 ]);
        }
        else if ( ch == 39){ ++chr; continue;} // rimozione apostrofo
        else newString.push_back(*chr);        // altrimenti copia inalterata
        ++chr;
    }
    return newString;
}
