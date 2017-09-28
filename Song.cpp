#include "Song.h"
#include <iostream>
#include <locale>         // std::locale, std::tolower


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
    name_tmp = name;
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
