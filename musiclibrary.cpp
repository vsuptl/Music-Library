/// TODO Update File Header
///
/// Assignment details and provided code are created and
/// owned by Adam T Koehler, PhD - Copyright 2023.
/// University of Illinois Chicago - CS 251 Spring 2023

#include <iostream>
#include <string>
#include <map>
#include <set>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cctype>
#include <cstdlib>

using namespace std;

// INSERTS GIVEN HELPERS CODE BELOW - DO NOT REMOVE
#include "helpers.cpp"
// INSERTS GIVEN HELPERS CODE ABOVE - DO NOT REMOVE

//
// NOTE TO STUDENTS:
// You may use the functions in the helpers file,
// but you may not modify the file or its contents in any way whatsoever.
//

//
// STUDENT CODE MAY GO BELOW THIS LINE
//

struct Album{//Struct containing 2 sets of artists names and songs
    set<string> artistNames;
    set<string> songs;
};

void loadData(string fileName, map<string, Album> &albums, set<string> &totalAlbums, set<string> &totalUniqueArtists);
void printAlbums(map<string, Album> &albums);
void stats(string remains, map<string, Album> &albums, set<string> &totalAlbums, set<string> &totalUniqueArtists);
void clear(map<string, Album> &albums, set<string> &totalAlbums, set<string> &totalUniqueArtists, int &totalSongs);
void exportCommand(string fileName, map<string, Album> &albums);
void search(string remains, map<string, Album> &albums, set<string> totalAlbums);
int getTotalSongs(map<string, Album> &albums);
void searchAblum(string searchWord, set<string> totalAlbums, set<string> &tempResult);
void searchArtist(string searchWord, map<string, Album> &albums, set<string> &tempResult);
void searchSong(string searchWord, map<string, Album> &albums, set<string> &tempResult);

int main()
{
    string userEntry;
    string command, remains;//command is help, clear, search, load, exit, export etc.
    map<string, Album> albums;//map where the key is album name and value is the struct
    set<string> totalAlbums;//set of all album names when data is loaded
    set<string> totalUniqueArtists; //set of all artist names when data is loaded
    int totalSongs = 0; 
    
    // TODO: Declare any necessary variables for the music libary

    // Display welcome message once per program execution
    cout << "Welcome to the Music Library App" << endl;
    cout << "--------------------------------" << endl;

    // Stay in menu until exit command
    do
    {
        cout << endl;
        cout << "Enter a command (help for help): " << endl;
        getline(cin, userEntry);
        cout << endl;

        // Split the user entry into two pieces
        splitFirstWord(userEntry, command, remains);
        tolower(command);

        // take an action, one per iteration, based on the command
        if (command == "help")
        {
            helpCommand();
        }
        else if (command == "clear")
        {
            clear(albums, totalAlbums, totalUniqueArtists, totalSongs);
        }
        else if (command == "export")
        {
            exportCommand(remains, albums);
        }
        else if (command == "load")
        {
            loadData(remains, albums, totalAlbums, totalUniqueArtists);
        }
        else if (command == "stats")
        {
            stats(remains, albums, totalAlbums, totalUniqueArtists);
        }          
        else if (command == "search")
        {
            search(remains, albums, totalAlbums);
        }

    }while(command != "exit");    

    cout << "Thank you for using the Music Library App" << endl;
    return 0;
}

//Takes in a filename, map, and 2 sets as parameters and they are passed by reference
//First opens the text file and data is put into assorted map and 2 sets. 
void loadData(string fileName, map<string, Album> &albums, set<string> &totalAlbums, set<string> &totalUniqueArtists){
    fstream inFS;
    string albumName;
    string line;
    Album temp;
    char checkfirstChar;

    if(fileName.empty()){//If there is no fileName as input, musicdatabase.txt is default
        fileName = "musicdatabase.txt";
    }
    
    inFS.open(fileName);//opens the file

    if(!inFS.is_open()){//outputs txt if file could not open
        cout << "Error: Could not open music library file - "<< fileName << endl;
    }
    else{
        while(getline(inFS, albumName)){//grabs the album name
            totalAlbums.insert(albumName);//inserts in in albumName set
            set<string> tempArtistName;
            set<string> tempSongs;
            getline(inFS, line);//grabs artist name or song name
            checkfirstChar = line[0];//If the first character is 0, then its a song
            while(checkfirstChar != '0'){//checks to see if the line is a artist until first character is 0.
                tempArtistName.insert(line);
                totalUniqueArtists.insert(line);
                getline(inFS, line);
                checkfirstChar = line[0];
            }
            while(line != albumName){//Runs until line is equal to album name
                tempSongs.insert(line);//song is inserted into set 
                getline(inFS, line);
            }
            temp.artistNames = tempArtistName;//set is put into struct
            temp.songs = tempSongs;//set is put into struct
            albums[albumName] = temp; //map is created for one album
        }

        inFS.close();//closes file 

    }


}

//Takes a string, map, and 2 sets as parameters 
//Prints out the total records, total unique artists, and total songs based on the data loaded
//There is an option to print out the current library
void stats(string remains, map<string, Album> &albums, set<string> &totalAlbums, set<string> &totalUniqueArtists){
    
    cout << "Overall Music Library Stats" << endl;
    cout << "===========================" << endl;
    cout << "Total Records: " << totalAlbums.size() << endl;//prints size of set of albums 
    cout << "Total Unique Artists: " << totalUniqueArtists.size() << endl;//prints size of set of artist names
    cout << "Total Songs: " << getTotalSongs(albums) << endl << endl;//prints total songs from map

    if(remains == "-d"){ //option where it prints current library based on data loaded
        cout << "Your Current Music Library Includes" << endl;
        cout << "===================================" << endl;
        printAlbums(albums);
    }
}

//takes in a map, 2 sets and int as parameters, they all are passed as reference
//clears out the map, 2 sets, and int is equal to 0
void clear(map<string, Album> &albums, set<string> &totalAlbums, set<string> &totalUniqueArtists, int &totalSongs){
    albums.clear();
    totalAlbums.clear();
    totalUniqueArtists.clear();
    totalSongs = 0;
}

//takes in string, and map as parameters
//takes the current library and exports it into a new txt file.
void exportCommand(string fileName, map<string, Album> &albums){
    ofstream outFS;

    if(fileName.empty()){//uses musicdatabase as defualt when fileName is empty 
        fileName = "musicdatabase.txt";
    }
    
    outFS.open(fileName);//opens the file name

    if(!outFS.is_open()){//prints out message if file could not open
        cout << "Error: Could not open music library file - "<< fileName << endl;
    }
    else{
        for(auto &e: albums){//iterates through each album
        outFS << e.first << endl;//exports into file
        for(auto &s: e.second.artistNames){//iterates through artist names
            outFS << s << endl;//exports into file
        }
        for(auto &l: e.second.songs){//iterates through song names
            outFS << l << endl;//exports into file
        }
        outFS << e.first << endl;
    }
    outFS.close();//closes file
    }

}

//takes in string, map, and a set as parameters
//takes in multiple search terms and outputs the result of album names from those
//search terms
void search(string remains, map<string, Album> &albums, set<string> totalAlbums){
    stringstream ss;
    string type;
    set<string> result;
    string searchWord;

    ss << remains;//remains is put into stringstream
    ss >> type;//grabs the type of search(album, artist, song)

    if(remains.empty()){//prints message if remains is empty
        cout << "Error: Search terms cannot be empty." << endl;
        cout << "No results found." << endl << endl;
    }
    else{
        while(ss >> searchWord){//iterates through all the search terms
            tolower(searchWord);//lowercases the search terms
            set<string> tempResult;
            if(type == "album"){
                if(searchWord[0] == '+'){//takes the intersection of sets of result and tempset
                    searchWord = searchWord.substr(1);
                    searchAblum(searchWord, totalAlbums, tempResult);
                    set<string> tempSet = result;
                    result.clear();//clears the result set 
                    set_intersection(tempSet.begin(), tempSet.end(), tempResult.begin(), tempResult.end(), inserter(result, result.begin()));
                }
                else if(searchWord[0] == '-'){//takes the difference of sets of result and tempset
                    searchWord = searchWord.substr(1);
                    searchAblum(searchWord, totalAlbums, tempResult);
                    set<string> tempSet = result;
                    result.clear();
                    set_difference(tempSet.begin(), tempSet.end(), tempResult.begin(), tempResult.end(), inserter(result, result.begin()));
                }
                else{//takes the union of sets of result and tempset
                    searchAblum(searchWord, totalAlbums, tempResult);
                    set<string> tempSet = result;
                    result.clear();
                    set_union(tempSet.begin(), tempSet.end(), tempResult.begin(), tempResult.end(), inserter(result, result.begin()));

                }

            }
            else if(type == "artist"){
                if(searchWord[0] == '+'){//takes the intersection of sets of result and tempset
                    searchArtist(searchWord, albums, tempResult);
                    set<string> tempSet = result;
                    result.clear();//clears the result set 
                    set_intersection(tempSet.begin(), tempSet.end(), tempResult.begin(), tempResult.end(), inserter(result, result.begin()));
                }
                else if(searchWord[0] == '-'){//takes the difference of sets of result and tempset
                    searchArtist(searchWord, albums, tempResult);
                    set<string> tempSet = result;
                    result.clear();
                    set_difference(tempSet.begin(), tempSet.end(), tempResult.begin(), tempResult.end(), inserter(result, result.begin()));
                }
                else{//takes the union of sets of result and tempset
                    searchArtist(searchWord, albums, tempResult);
                    set<string> tempSet = result;
                    result.clear();
                    set_union(tempSet.begin(), tempSet.end(), tempResult.begin(), tempResult.end(), inserter(result, result.begin()));
                }
            }
            else if(type == "song"){
                if(searchWord[0] == '+'){//takes the intersection of sets of result and tempset
                    searchSong(searchWord, albums, tempResult);
                    set<string> tempSet = result;
                    result.clear();
                    set_intersection(tempSet.begin(), tempSet.end(), tempResult.begin(), tempResult.end(), inserter(result, result.begin()));
                }
                else if(searchWord[0] == '-'){//takes the difference of sets of result and tempset
                    searchSong(searchWord, albums, tempResult);
                    set<string> tempSet = result;
                    result.clear();
                    set_difference(tempSet.begin(), tempSet.end(), tempResult.begin(), tempResult.end(), inserter(result, result.begin()));
                }
                else{//takes the union of sets of result and tempset
                    searchSong(searchWord, albums, tempResult);
                    set<string> tempSet = result;
                    result.clear();
                    set_union(tempSet.begin(), tempSet.end(), tempResult.begin(), tempResult.end(), inserter(result, result.begin()));
                }
                
            }
        }
    }
    if(result.size() > 0){//prints out the resulting set 
    cout << "Your search results exist in the following albums: " << endl;
        for(auto e: result){
            cout << e << endl;
        }
    cout << endl;
    }

}

//searches through set of album names and tries to find if it matches with the searchWord
void searchAblum(string searchWord, set<string> totalAlbums, set<string> &tempResult){
    if(searchWord[0] == '+' || searchWord[0] == '-'){//checks to see if it has - or + at the beginning of searchWord
        searchWord = searchWord.substr(1);//gets rid of the + or -
    }
    for(string e: totalAlbums){//iterates through set of album names
        string lowerString = e;
        tolower(lowerString);//lowercases the album name in the the set
        int index = lowerString.find(searchWord);
        if(index != string::npos){//if founded, then album name is inserted into temp set
            tempResult.insert(e);
        }
    }
}

//searches through map of albums to find if it matches with the searchWord of songs
void searchArtist(string searchWord, map<string, Album> &albums, set<string> &tempResult){
    if(searchWord[0] == '+' || searchWord[0] == '-'){//checks to see if it has - or + at the beginning of searchWord
        searchWord = searchWord.substr(1);
    }
    for(auto e: albums){//iterates through albums
        for(auto f: e.second.artistNames){//iterates through set of artist names
            string lowerString = f;
            tolower(lowerString);//lowercases the artist names
            int index = lowerString.find(searchWord);
            if(index != string::npos){//if founded, then song name is inserted into temp set
            tempResult.insert(e.first);
            }
        }   
    }
}

//searches through map of albums to find if it matches with the searchWord of songs
void searchSong(string searchWord, map<string, Album> &albums, set<string> &tempResult){
    if(searchWord[0] == '+' || searchWord[0] == '-'){//checks to see if it has - or + at the beginning of searchWord
        searchWord = searchWord.substr(1);
    }
    for(auto e: albums){//iterates through albums
        for(auto f: e.second.songs){//iterates through set of songs
            string lowerString = f;
            tolower(lowerString);//lowercases the song names
            int index = lowerString.find(searchWord);
            if(index != string::npos){//if founded, then song name is inserted into temp set
            tempResult.insert(e.first);
            }
        }   
    }
}

//prints out the albums with artist and song name
void printAlbums(map<string, Album> &albums){
    for(auto &e: albums){//iterates through map of albums
        cout << e.first << endl;//prints album name
        for(auto &s: e.second.artistNames){//iterates through artist names and prints
            cout << " " << s << endl;
        }
        for(auto &l: e.second.songs){//iterates through song names and prints
            cout << "   " << l << endl;
        }
    }
    cout << endl;
    
}

//returns the total songs in music library
int getTotalSongs(map<string, Album> &albums){
    int totalSongs = 0;
    for(auto &e: albums){//iterates through map of albums
        totalSongs += e.second.songs.size();//takes the size and adds to int
    }
    return totalSongs;
}