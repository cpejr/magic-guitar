#ifndef SDCARD
#define SDCARD

/**
 * @file SDCard.hpp
 * @brief Control the SD Card.
 * @version 1.0
 * @date 2023-07-28
 * @details This archive contains the class 'SDCard', used to control the SD Card that stores the musics and data.
 * @authors CPE - Consultoria e Projetos Elétricos
 */

#include <vector>
#include <SD.h>
#include <string>
#include <regex>
using namespace std;

class SDCard
{
public:

    /**
    * @brief Constructor of a SDCard.
    */
    SDCard();

    /**
    * @brief Returns the vector of files names.
    */
    vector<string> getList();

    /**
    * @brief Create the vector mFilesNames.
    */
    void Makelist();

    /**
    * @brief Prints the position of an archive - used for tests.
    * @param pFileName File name.
    */
    void printPosition(std::string pFileName);

    /**
    * @brief Saves the Engines' position on the SD card.
    * @param pNumFile Number of the file.
    * @param pNumStroke Number of the stroke.
    */
    string readFile(int pNumFile, char pNumStroke);

    /**
    * @brief Saves the Engines' position on the SD card.
    * @param pEnginePos Vector of Engines' position.
    */
    void writeInFile(vector<int> pEnginePos);

    /**
    * @brief Returns the current targets.
    */
    String getCurrentTarget();

    /**
    * @brief Music speed.
    */
    int SD_SPEED;

    /**
    * @brief Music subdivision.
    */
    string SD_SUBDIVISION;

    /**
    * @brief Name of a music author.
    */
    string authorName;

private:
    /**
    * @brief Vector of files names.
    */
    vector<string> mFilesNames;
};

#endif