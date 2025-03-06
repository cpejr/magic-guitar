#ifndef ENGINE
#define ENGINE

extern volatile bool enableSwitch;

/**
 * @file Engine.hpp
 * @brief Control and store a step-enginer.
 * @version 1.0
 * @date 2023-07-28
 * @details This archive contains the class 'Engine', used to control individually and store a step-enginer of the Magic Guitar.
 * @authors CPE - Consultoria e Projetos Elétricos
 */

#include "SDCard.hpp"

class Engine
{
public:
    
    /**
    * @brief Contructor of an Engine.
    * @param pGuitarString Guitar string of the Engine.
    * @param pStep STEP pin of the Engine.
    * @param pDir DIR pin of the Engine.
    */
    Engine(char pGuitarString, int pStep, int pDir);

    /**
    * @brief Return the Engine guitar string.
    */
    char getGuitarString();

    /**
    * @brief Return the Engine target.
    */
    int getTarget();

    /**
    * @brief Return the Engine DIR pin.
    */
    int getDir();

    /**
    * @brief Return the Engine STEP pin.
    */
    int getStep();

    /**
    * @brief Reverse the engine target.
    */
    void reverseTarget();

    /**
    * @brief Return the Engine position.
    */
    int getPosition();

    /**
    * @brief Return the position pointer.
    */
    int *getPosPtr();

    /**
    * @brief Spin the engine one step - used for reset it.
    */
    void oneStep(int pSignal); // utilizado para resetar o motor

    /**
    * @brief Set the Engine speed.
    * @param pSDSpeed New Engine speed.
    */
    void setSpeed(int pSDSpeed); // utilizado para determinar o valor da velocidade

    /**
    * @brief Set the Engine delay speed.
    * @param pSDSpeed New Engine delay speed.
    */
    void setDelaySpeed(int pSDSpeed);

    /**
    * @brief Return the Engine delay speed.
    */
    int getDelaySpeed();

    /**
    * @brief Next Engine position.
    */
    int mTarget;        // próxima posição

    /**
    * @brief Engine current position.
    */
    int mPosition;      // posição atual

private:
    /**
    * @brief Engine guitar string.
    */ 
    char mGuitarString; // corda

    /**
    * @brief Pointer to the engine position.
    */ 
    int *mPosPtr; // ponteiro para gravar a posição

    /**
    * @brief Speed of the engine in RPM.
    */ 
    int mSpeedRPM;

    /**
    * @brief Delay speed of the engine.
    */ 
    int mDelaySpeed;

    /**
    * @brief DIR pin of the engine driver.
    */ 
    int mDIR; // pino do drive

    /**
    * @brief STEP pin of the engine driver.
    */ 
    int mSTEP; // pino do drive

    /**
    * @brief Steps needed for a full revolution.
    */ 
    int mRevolution; // quantidade de passos necessários para dar uma volta completa
};

#endif