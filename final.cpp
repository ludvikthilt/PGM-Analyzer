#include <iostream> //opérations de flux d'entrée et de sortie standard
#include <fstream>  //opérations de fichier
#include <string>   //manipulation des chaînes de caractères 
#include <sstream>  //manipulation des flux de chaînes de caractères en mémoire
#include "pgm.hpp"  //fichier en-tête

using namespace std;



int main() {
    //affichage
    clear();
    cout << BOLD YELLOW "\t\tBIENVENUE DANS L'ANALYSEUR D'IMAGE PGM" RESET<< "\t\tby ThiLT"<< endl;
    cout<<endl;
    cout<<endl;
    cout << BOLD  "Selon votre convenance vous pouvez"<< endl;
    cout << "1 - Placer l'image à traiter dans le répertoire de l'application"<<endl;
    cout << "2 - Entrer le chemin d'accès de l'image  :  "<<endl;
    cout<<endl;
    cout <<"Entrez votre choix (1 ou 2)  :  "  RESET GREEN;
    //gestion du choix et des erreurs lors de l'entrée
    int loadingChoice = getLoadingChoice();
    string filePath;
    
    //gestion de l'entrée de l'emplacement des images et des emplacements invalides
    if (loadingChoice == 1) {
        clear();
        cout << RESET BOLD"\t\tMerci de placer votre image dans le répertoire"  << endl;
        cout<<endl;
        cout << "Donnez le nom de votre fichier image (sans extension)   :  " RESET  WHITEBACK BLACK;
        string fileName;
        cin.ignore();
        getline(cin, fileName);
        filePath = getValidFilePath(fileName + ".pgm", false);
    } else {
        cout << RESET BOLD"Entrez le chemin d'accès du fichier"<<endl;
        cout<< "sous windows"<<BLUE " D:/input.pgm "<< RESET ";" << BOLD " Sous ubuntu wsl "<< BLUE "/mnt/d/input.pgm"<<RESET<<"  :  " WHITEBACK BLACK;
        cin.ignore();
        getline(cin, filePath);
        filePath = getValidFilePath(filePath, true);
    }

    // Lecture et extraction des infos de l'image
    PGMImage* image = readPGMHeader(filePath);
    readPixelData(image, filePath);

    // Inversion des pixels
    invertPixels(image);

    // Écriture du fichier de sortie
    size_t dotPos = filePath.find_last_of(".");
    string outputFileName = filePath.substr(0, dotPos) + "_modify.pgm";
    writePGM(image, outputFileName);

    // Libération de la mémoire
    cleanupPGMImage(image);

    return 0;
}
