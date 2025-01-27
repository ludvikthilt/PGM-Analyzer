#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include "pgm.hpp"

using namespace std ;


int LoadingFile = 0;
std::string FilePath , FileName , OutputFileName;
std::string FileType, line ;
int FileWidth = 0, FileHeight = 0 ,MaxValue, NbLine = 0;
//int** P2pixels = new int*[FileHeight];
//int P2pixel;
unsigned short** pixels = new unsigned short*[FileHeight];
unsigned short pixel;
//unsigned short** Midpixels = new unsigned short*[FileHeight];




     //==================================//
    //      ordre d'éxécution : 1       // 
   //  LECTURE DE L'IMAGE PGM(ReadPGM) //
  //     procédure sans arguments     // 
 //==================================//
void FileReading(){
    cout<<"BIENVENUE DANS L'ANALYSEUR PGM by ThiLT"<<endl;
    cout<<"Selon votre convenance vous devez"<<endl;
    cout<<"1 - Placer l'image à traiter dans le répertoire de l'application\n2 - Entrer le chemin d'accès de l'image  :  ";
    ChoiceErrorHandler();
      //-------------------------------------------------------------------------------//
     // gestion des fichiers : création du chemin d'accès pour la lecture du fichier  //
    //-------------------------------------------------------------------------------//  
   
    if (LoadingFile == 1){
        cout <<"merci de placer donc votre image dans le répertoire" <<endl; 
        cout << "donnez le nom de votre fichier image (sans extension)   :  ";
        cin.ignore();
        getline(cin,FileName);
        FileName =  FileName + ".pgm";
        FilePath = FileName;
        cout<<FilePath<<endl;
        FileNameErrorHandler();
        
    }else{
        cout<<"Entrez le chemin d'accès du fichier \n (sous windows D:/input.pgm \n sous ubuntu wsl /mnt/d/input.pgm )  :  ";
        cin.ignore();
        getline(cin,FilePath);
        cout<<FilePath<<endl;
        FilePathErrorHandler();
        
    }
    FileScrapping();
}
     //==================================//
    //      ordre d'éxécution : 2       // 
   // ECRITURE DE L'IMAGE PGM(WritePGM)//
  //     procédure sans arguments     // 
 //==================================//
 void FileWriting(){
    InversionPixel();
    OutputFileName = FileName + "_modify.pgm";
    ofstream OutputFile(OutputFileName, ios::out|ios::binary);
    if(OutputFile.is_open()){
        //ecriture dans le fichier
        OutputFile << FileType <<endl;
        OutputFile <<"#mod by thilt"<<endl;
        OutputFile << FileWidth <<" "<<FileHeight<<endl;
        OutputFile << MaxValue <<endl;
        if (FileType == "P5" || FileType == "p5"){
            for (unsigned int i = 0; i < FileHeight; i++){
                for (unsigned int j = 0; j < FileWidth; j++){
                    //unsigned char pixelVal = static_cast<unsigned short>(clamp(pixels[i][j], 0, MaxValue));
                    OutputFile.write(reinterpret_cast<char*>(pixels[i]),FileWidth*sizeof(unsigned short));  
                }
            }
        }else if(FileType == "P2" || FileType == "p2"){
            for (unsigned int i = 0; i < FileHeight; i++) {
                for (unsigned int j = 0; j < FileWidth; j++) {
                    OutputFile << static_cast<int>(pixels[i][j]) <<"  ";
                }
                OutputFile<<endl;
            }
        }
        cerr<<"creation du fichier de sortie"<<endl;
    }else if(!OutputFile.is_open()){
        cerr<<"erreur lors du chargement du fichier sortie"<<endl;
    }

    
}
         //-------------------------------------------------------------------------------------------//
        // WritePGM : opération à effectuer                                                          //
       //-------------------------------------------------------------------------------------------//  
void InversionPixel(){
    for (unsigned int i = 0; i < FileHeight; i++) {
        for (unsigned int j = 0; j < FileWidth; j++) {
            pixels[i][j] = MaxValue - pixels[i][j];
        }
    }
} 
         //-------------------------------------------------------------------------------------------//
        // ReadPGM : selection du fichier : gestion du choix de l'option de selection et des erreurs //
       //-------------------------------------------------------------------------------------------//  
void ChoiceErrorHandler(){
    while(true){

        if (cin >> LoadingFile){ 
            if(LoadingFile == 1 || LoadingFile == 2){
                break;
            }else{
                cerr<<"entrer une valeur correcte (1 ou 2)"<<endl;
            }
        }else{
            cerr<<"entrer une valeur correcte (1 ou 2)"<<endl;
            cin.clear();
            cin.ignore(10000, '\n');
        }
    }
}
     //-----------------------------------------------------------------------------------------------//
    // ReadPGM : gestion des fichiers : ouverture du fichier et gestion d'erreur lors de l'ouverture //
   //-----------------------------------------------------------------------------------------------//  
void FileNameErrorHandler(){
    ifstream File(FilePath, ios::binary);
    while (File.fail()){
            cerr << "fichier non trouvé " <<endl; 
            cout <<"merci de placer donc votre image dans le répertoire (relancer le programme si nécessaire)" <<endl; 
            cout << "donnez le nom de votre fichier image (sans extension)   :  ";
            cin.ignore();
            getline(cin,FileName);
            FileName =  FileName + ".pgm";
            FilePath = FileName;
            cout<<FilePath<<endl;
            ifstream File(FilePath, ios::binary);
    } 
    if(!File.fail()){cerr<<"fichier trouvé "<<endl;}
}
         //-----------------------------------------------------------------------------------------------//
        // ReadPGM : gestion des fichiers : ouverture du fichier et gestion d'erreur lors de l'ouverture //
       //-----------------------------------------------------------------------------------------------// 
void FilePathErrorHandler(){
    ifstream File(FilePath, ios::binary);
    while (File.fail()){
            cerr << "fichier non trouvé " <<endl;
            cout<<"Entrez le chemin d'accès du fichier \n (sous windows D:/input.pgm \n sous ubuntu wsl /mnt/d/input.pgm )  :  ";
            cin.ignore();
            getline(cin,FilePath);
            cout<<FilePath<<endl;
            ifstream File(FilePath, ios::binary);
    }
    if(!File.fail()){cerr<<"fichier trouvé "<<endl;}
}
     //-------------------------------------------------------//
    // ReadPGM : gestion des fichiers : scrapping            //
   //-------------------------------------------------------//
void FileScrapping(){

      //-------------------------------------------------------//
     //  gestion des fichiers : scrapping : en-tète           //
    //-------------------------------------------------------// 

    ifstream File(FilePath, ios::binary);
    while( getline(File, line) && NbLine < 4 ){
        switch (NbLine){
            case 0 :
                FileType = line ;
                break;
            case 1 :
                if(line[0] == '#'){
                    break;
                }else{
                    istringstream iss(line);
                    iss >> FileWidth >> FileHeight ;
                    NbLine++ ;
                    break;
                }
            case 2 :
                if(FileWidth == 0 && FileHeight == 0){
                    istringstream iss(line);
                    iss >> FileWidth >> FileHeight ;
                }else{
                    istringstream iss(line);
                    iss >> MaxValue ;
                }
                break ;
            case 3 :
                istringstream iss(line);
                iss >> MaxValue ;
                break ;
        }
        NbLine++;
    }
    cout << FileType<<endl ;
    cout << FileWidth<<endl; 
    cout << FileHeight<<endl ; 
    cout << MaxValue<<endl ;
      //---------------------------------------------//
     // gestion des fichiers : scrapping : pixel    //
    //---------------------------------------------//
    
        for (int i = 0; i < FileHeight; i++){
            pixels[i] = new unsigned short[FileWidth];
        }
        //remplissage tableau 2D
        for (int i = 0; i < FileHeight; i++){
            for(int j = 0; j < FileWidth; j++){
                if(pixels [i][j] == 0){
                    File >> pixel ;
                    pixels [i][j] = pixel ;
                }
            }
        }
        //affichage de la matrice
        /*for (int i = 0; i < FileHeight; i++){
            for(int j = 0; j < FileWidth; j++){
                cout << pixels [i][j] << "   ";
            }
            cout<<endl;
        }*/
    
    File.close();
    //DynamicClear();
}

void DynamicClear(){
    for( int i = 0; i < FileHeight; i++){
        delete[] pixels[i];

    }
    delete[] pixels;
}