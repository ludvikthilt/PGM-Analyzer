#include <iostream> //opérations de flux d'entrée et de sortie standard
#include <fstream>  //opérations de fichier
#include <string>   //manipulation des chaînes de caractères 
#include <sstream>  //manipulation des flux de chaînes de caractères en mémoire
#ifdef _WIN32
    #include <windows.h> // pause système Inclure pour Windows
    #else 
    #include <unistd.h> // pause système Inclure pour Unix/Linux
#endif 
#include "pgm.hpp"  //fichier en-tête

using namespace std;


    //==========================================================//
   // Fonction de type int pour gérer les entrées utilisateur  //
  // arg : aucun , valeur de retour: loadingfile(int)         //
 //==========================================================//
int getLoadingChoice() {
    int loadingFile;
    while(true) {
        if (cin >> loadingFile) { 
            if(loadingFile == 1 || loadingFile == 2) {
                return loadingFile;
            
            }
        cerr << RESET BOLD "entrer une valeur correcte (1 ou 2)" GREEN  << endl;
        cin.clear();
        cin.ignore(10000, '\n');
        }
    }
}
    
    //============================================================//
   //Fonction de type bool pour vérifier l'existence du fichier  //
  // arg : const string filePath , valeur de retour: bool       //
 //============================================================//
bool checkFileExists(const string& filePath) {
    ifstream file(filePath, ios::binary);
    return !file.fail();
}


    //======================================================================================//
   //Fonction de type string pour recupérer le chemin d'acccès                             //
  // arg : const string initialPath , bool isDirectPath valeur de retour: string filePath //
 //======================================================================================//
string getValidFilePath(const string& initialPath, bool isDirectPath) {
    string filePath = initialPath;
    while (!checkFileExists(filePath)) {
        clear();
        cerr << RESET BLINK RED "fichier non trouvé " RESET  << endl;
        cout<<endl;
        if (isDirectPath) {
            cout<<endl;
            cout << BOLD "Entrez le chemin d'accès du fichier"<<endl;
            cout<<endl;
            cout<< "sous windows"<<BLUE " D:/input.pgm "<< RESET ";" << BOLD " Sous ubuntu wsl "<< BLUE "/mnt/d/input.pgm"<<RESET<<"  :  " WHITEBACK BLACK;
        } else {
            cout<<endl;
            cout << BOLD "Donnez le nom de votre fichier image (sans extension)   :  " RESET  WHITEBACK BLACK;
        }
        //cin.ignore();
        getline(cin, filePath);
        if (!isDirectPath) {
            filePath += ".pgm";
        }
    }
    //clear();
    cout<<endl;
    cerr <<RESET GREEN BLINK "fichier trouvé" RESET<< endl;
    return filePath;
}
  
  
    //===========================================================================================//
   //Fonction de type PGMImage* pour recuper les info de l'image (en-tête)                      //
  // arg : const string filePath , bool isDirectPath valeur de retour: string filePath         //
 //===========================================================================================//
PGMImage* readPGMHeader(const string& filePath) {
    ifstream file(filePath, ios::binary);
    PGMImage* image /*(pointeur sur une instance de la structure)*/= new PGMImage(); //allocation de la memoire pour une instance de la structure (initiée à l'aide du constructeur)
    string line;
    int nbLine = 0;

    while (getline(file, line) && nbLine < 4) {
        switch (nbLine) {
            case 0:
                image->fileType = line;
                break;
            case 1://verification si la ligne est un commentaire
                if (line[0] != '#') {
                    istringstream iss(line);
                    iss >> image->width >> image->height;
                    nbLine++;
                }
                break;
            case 2:
                if (image->width == 0 && image->height == 0) {
                    istringstream iss(line);
                    iss >> image->width >> image->height;
                } else {
                    istringstream iss(line);
                    iss >> image->maxValue;
                }
                break;
            case 3:
                istringstream iss(line);
                iss >> image->maxValue;
                break;
        }
        nbLine++;
    }
    
    file.close();
    return image;
}


    //===========================================================================================//
   //Procédure pour recuper les info de l'image ( les pixels en tableau 2D)                     //
  // arg : PGMImage* image, const string filePath                                              //
 //===========================================================================================//
void readPixelData(PGMImage* image, const string& filePath) {
    ifstream file(filePath, ios::binary);
    string line;
    
    // Sauter le header
    for(int i = 0; i < 4; i++) {
        getline(file, line);
    }

    // Allouer la mémoire
    image->pixels = new unsigned short*[image->height];
    for (int i = 0; i < image->height; i++) {
        image->pixels[i] = new unsigned short[image->width];
    }

    if (image->fileType == "P5" || image->fileType == "p5") {
        // Pour P5 (binaire)
        unsigned char* buffer = new unsigned char[image->width];
        for (int i = 0; i < image->height; i++) {
            file.read(reinterpret_cast<char*>(buffer), image->width);
            for(int j = 0; j < image->width; j++) {
                image->pixels[i][j] = static_cast<unsigned short>(buffer[j]);
            }
        }
        delete[] buffer;
    } else {
        // Pour P2 (ASCII)
        unsigned short pixel;
        for (int i = 0; i < image->height; i++) {
            for(int j = 0; j < image->width; j++) {
                file >> pixel;
                image->pixels[i][j] = pixel;
            }
        }
    }
    file.close();
}


    //===========================================================================================//
   //Procédure pour ecrire les info de l'image modifiée dans le fichierSortie                   //
  // arg : PGMImage* image, const string outputFileName                                        //
 //===========================================================================================//
void writePGM(const PGMImage* image, const string& outputFileName) {
    ofstream outputFile(outputFileName, ios::binary);
    if(!outputFile.is_open()) {
        cerr << "erreur lors du chargement du fichier sortie" << endl;
        return;
    }

    // Écrire l'en-tête
    outputFile << image->fileType << '\n';
    outputFile << "#mod by thilt" << '\n';
    outputFile << image->width << " " << image->height << '\n';
    outputFile << image->maxValue << '\n';

    if (image->fileType == "P5" || image->fileType == "p5") {
        // Pour P5 (binaire)
        unsigned char* buffer = new unsigned char[image->width];
        for (int i = 0; i < image->height; i++) {
            for (int j = 0; j < image->width; j++) {
                buffer[j] = static_cast<unsigned char>(image->pixels[i][j]);
            }
            outputFile.write(reinterpret_cast<char*>(buffer), image->width);
        }
        delete[] buffer;
    } else {
        // Pour P2 (ASCII)
        for (int i = 0; i < image->height; i++) {
            for (int j = 0; j < image->width; j++) {
                outputFile << image->pixels[i][j] << " ";
            }
            outputFile << '\n';
        }
    }
    cerr << BOLD "creation de l'image modifiée" RESET << endl;
    stop();
    clear();
    outputFile.close();
}


    //===========================================================================================//
   //Procédure pour modifier les informations stocker dans le tableau 2D                        //
  // arg : PGMImage* image                            (pointeur sur l'instance de la structure)//
 //===========================================================================================//
void invertPixels(PGMImage* image) {
    for (int i = 0; i < image->height; i++) {
        for (int j = 0; j < image->width; j++) {
            image->pixels[i][j] = image->maxValue - image->pixels[i][j];
        }
    }
}


    //===========================================================================================//
   //Procédure pour liber la mémoire utiliser par l'instance de la structure(tableau et entête) //
  // arg : PGMImage* image                           (pointeur sur l'instance de la structure) //
 //===========================================================================================//
void cleanupPGMImage(PGMImage* image) {
    if (image) {
        for(int i = 0; i < image->height; i++) {
            delete[] image->pixels[i];
        }
        delete[] image->pixels;
        delete image;
    }
}


    //===========================================================//
   //Procédure pour effacer le contenu affiché sur le terminal  //
  //===========================================================//
void clear(){
    #ifdef _WIN32
        system("CLS");
    #else
        system("clear");
    #endif
}
    //===================================================================//
   //Procédure pour mettre en pause le contenu affiché sur le terminal  //
  //===================================================================//
void stop(){
    #ifdef _WIN32
       Sleep(2000);
    #else
        usleep(2000000);
    #endif
}
