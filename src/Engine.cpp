#include "Engine.hpp"

volatile bool enableSwitch = 1;

Engine::Engine(char pGuitarString, int pStep, int pDir) : mGuitarString(pGuitarString), mSTEP(pStep), mDIR(pDir)
{
  pinMode(pDir, OUTPUT);
  pinMode(pStep, OUTPUT);
  this->mRevolution = 200;

  this->mTarget = 30;

  this->mPosition = 0;

  mPosPtr = &mPosition;
}

char Engine::getGuitarString()
{
  return this->mGuitarString;
}

int Engine::getTarget()
{
  return this->mTarget;
}

void Engine::reverseTarget()
{
  this->mTarget *= (-1);
}

int Engine::getPosition()
{
  return this->mPosition;
}

int *Engine::getPosPtr()
{
  return this->mPosPtr;
}

void Engine::oneStep(int pSignal)
{
  this->mPosition = 0;
  if (pSignal > 0)
  {
    digitalWrite(this->mDIR, HIGH);
    delayMicroseconds(1000);
    digitalWrite(this->mSTEP, LOW);
    delayMicroseconds(1000);
    digitalWrite(this->mSTEP, HIGH);
    delayMicroseconds(1000);
  }
  if (pSignal < 0)
  {

    digitalWrite(this->mDIR, LOW);
    delayMicroseconds(1000);
    digitalWrite(this->mSTEP, LOW);
    delayMicroseconds(1000);
    digitalWrite(this->mSTEP, HIGH);
    delayMicroseconds(1000);

  }
}

int Engine::getDir()
{
  return this->mDIR;
}

int Engine::getStep()
{
  return this->mSTEP;
}

void Engine::setDelaySpeed(int pSDSpeed)
{

  this->mDelaySpeed = pSDSpeed;
  //Serial.println(mDelaySpeed);

}

int Engine::getDelaySpeed()
{
  return this->mDelaySpeed;
}

