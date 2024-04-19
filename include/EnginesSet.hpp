#ifndef ENGINESSET
#define ENGINESSET

/**
 * @file EngineSet.hpp
 * @brief Store all the Engines and control them to play a music.
 * @version 1.0
 * @date 2023-07-28
 * @details This archive contains the class 'EngineSet', used to create the magic guitar.
 It store all the Engines and control them using the SD card data to play a music.
 * @authors CPE - Consultoria e Projetos Elétricos
 */

#include "Engine.hpp"
#include "SDCard.hpp"
#include <vector>
#include <string>
#include <Arduino.h>

#define DELAY_SPEED 1800
#define STEP_NUMBER 13

class EnginesSet
{
public:

    /**
    * @brief Enum of Engine directions.
    */
    enum mDirection
    {
        UP = 0,
        DOWN = 1
    };

    /**
    * @brief Contructor of an EnginesSet.
    */
    EnginesSet();

    /**
    * @brief Inserts an Engine to the EnginesSet.
    * @param pGuitarString Guitar string of the Engine.
    * @param pStep STEP pin of the Engine.
    * @param pDir DIR pin of the Engine.
    */
    void insertMotor(char pGuitarString, int pStep, int pDir);

    // void boot(); // Lógica de fim de curso

    /**
    * @brief Add an Engine to mEnginesToPlay according the music.
    * @param pGuitarString Guitar string of the Engine.
    */
    void addToenginesToPlay(std::string pGuitarString); // adiciona os motores que irão girar de acordo com a corda presente na música

    /**
    * @brief Spin the Engines on mEnginesToPlay.
    */
    void playMany(); // roda o motor antes adicionado

    /**
    * @brief Play a music.
    * @param pStream Music stream.
    * @param pTune Music tune.
    */
    void parseFile(std::string pStream, int pTune); // toca a música

    /**
    * @brief Set all the Engines up or down.
    * @param pDirection Engine current direction.
    * @param pGuitarStrings Guitar strings that will be played.
    */
    void runThrough(mDirection pDirection, std::string pGuitarStrings); // faz subida ou descida em todos os motores

    void tune(int pTunePosition);

    /**
    * @brief Reset an Engine.
    * @param pGuitarReset Engine position in mEngines.
    * @param pSignal Direction to reset the Engine (up or down).
    */
    void playOneStep(int pGuitarReset, int pSignal); // reseta o motor selecionado

    /**
    * @brief Save the Engines' position on the SD card.
    * @param pSD SD Card.
    */
    void getEnginePos(SDCard pSd);

    /**
    * @brief Sets the opposition after a music.
    */
    void endMusic(); //Configura oposição após cada música

    /**
    * @brief Set the Engines speed.
    * @param pSD SD Card.
    */
    void setEngineSpeed(SDCard pSd); // configura velocidade a cada motor

    /**
    * @brief Set a compass suddivision.
    * @param pSD SD Card.
    */
    void setSubdivision(SDCard pSd); //determina o número de subdivisões de um compasso

    /**
    * @brief Set the delay between pulses.
    * @param pSD SD Card.
    */
    void setDelay(); //determina e retorna o delay entre um pulso e outro

    /**
    * @brief Set the mLastMillis variable to current reading of millis()
    */
    void setLastMillis();
  
    void setCurrentTarget(String pcurrentTargets);

    /**
    * @brief Spin a Engine a half of his target.
    * @param pGuitarStringReset Engine position in mEngines.
    */
    void runHalfTarget(int pGuitarStringReset);

    /**
    * @brief Vector of Engine positions.
    */
    std::vector<int> mEnginePos;

private:
    /**
    * @brief Vector of Engines.
    */
    std::vector<Engine *> mEngines;

    /**
    * @brief Vector of Engines to play a music.
    */
    std::vector<Engine *> mEnginesToPlay;

    // int mExitLoop;
    // int mButtonUpState;
    // int mButtonSelectState;
    // int mButtonDownState;

    /**
    * @brief Engines BPM speed.
    */
    float mBPMSpeed; 

    /**
    * @brief Music subdivision.
    */
    string mSubdivision;

    /**
    * @brief Global Engine Targets.
    */
    int mGlobalTargets;

    /**
    * @brief Delay in miliseconds.
    */
    uint32_t mDelayMillis;

    /**
    * @brief Delay in microseconds.
    */
    uint32_t mDelayMicro;

    /**
    * @brief millis() reading on last stroke.
    */
    unsigned long mLastMillis;
};

#endif
