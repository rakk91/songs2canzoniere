#ifndef Song_h
#define Song_h
#include <deque>
#include <string>
#include <fstream>


class Song {
public:
    std::deque<std::string> line;
    std::deque<std::string> newline;
    
    Song();
    Song( std::string song_name );
    Song( std::string song_name, std::string song_author );
    ~Song();
    void fillName(std::string song_name);
    void fillAuthor(std::string song_author);
    void fillAlbum(std::string song_album);
    void fillTonalita(std::string song_tonalita);
    void fillFamiglia(std::string song_famiglia);
    void fillGruppo(std::string song_gruppo);
    void fillMomenti(std::string song_momenti);
    void fillIdentificatore();
    void fillIdentificatore(std::string song_identificatore);
    void fillDataRevisione( std::string song_data_revisione);
    void switch_find_name();
    void switch_find_author();
    void switch_find_tonalita();
    void switch_find_intro();
    
    std::string getName();
    std::string getAuthor();
    std::string getAlbum();
    std::string getTonalita();
    std::string getFamiglia();
    std::string getGruppo();
    std::string getMomenti();
    std::string getIndentificatore();
    std::string getDataRevisione();
    
    bool chk_find_name();
    bool chk_find_author();
    bool chk_find_tonalita();
    bool chk_find_intro();
    
    void read_from_file( std::ifstream &inFile);
    void analyze();
    
    std::string replaceChords( std::string line );

    
private:
    std::string name;
    std::string author;
    std::string album;
    std::string tonalita;
    std::string famiglia;
    std::string gruppo;
    std::string momenti;
    std::string identificatore;
    std::string data_revisione;
    bool notFindName;
    bool notFindAuthor;
    bool notFindTonalita;
    bool findIntro;
    
    std::string convertChords( std::string accordo );
    std::string convertChords_lowercase( std::string accordo );
    std::string removeAccented( std::string str );


};
#endif
