#ifndef __PGM_HPP__
    #define __PGM_HPP__

    //embelissement du texte sur le terminal
    #define BOLD "\033[1m"
    #define UNDERLINE "\033[4m"
    #define BLINK "\033[5m"

    //couleur de texte
    #define RED "\033[31m"
    #define GREEN "\033[32m"
    #define YELLOW "\033[33m"
    #define BLACK "\033[30m"
    #define BLUE "\033[34m"
    

    //couleur de background
    #define WHITEBACK "\033[47m"
    
    #define RESET "\033[0m"


    #include <string>
    #include <fstream>
    #include <iostream>
    #include <sstream>

    struct PGMImage {
        std::string fileType;
        int width;
        int height;
        int maxValue;
        unsigned short** pixels;

        PGMImage() : 
            fileType(""),
            width(0),
            height(0),
            maxValue(0),
            pixels(nullptr)
        {}

        ~PGMImage() {
            std::cout<< RESET YELLOW BLINK"merci d'avoir utiliser notre programme" RESET<<std::endl;
        }
    };

    int getLoadingChoice();

    bool checkFileExists(const std::string& filePath);

    std::string getValidFilePath(const std::string& initialPath, bool isDirectPath);

    PGMImage* readPGMHeader(const std::string& filePath);

    void readPixelData(PGMImage* image, const std::string& filePath);

    void invertPixels(PGMImage* image);

    void writePGM(const PGMImage* image, const std::string& outputFileName);

    void cleanupPGMImage(PGMImage* image);

    void clear();

    void stop();

#endif