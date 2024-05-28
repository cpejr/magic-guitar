#include <SD.h>
#include <SPI.h>
#include "FS.h"
#include <Arduino.h>
#include <vector>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_ST7735.h>
#include <esp_ipc.h>

#include "SDCard.hpp"
#include "Engine.hpp"
#include "EnginesSet.hpp"

using namespace std;

vector<string> musicNames;
int musicNumber;
int j;

// Variáveis do display TFT
#define TFT_MOSI 23 //também representado por SDA
#define TFT_SCLK 18
#define TFT_DC 2 // também representado por A0
#define TFT_CS 15
#define TFT_RST 4
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

int delayButtons = 250;
#define buttonUp 16
#define buttonSelect 17
#define buttonDown 35

int exitLoop = 0;

int GLOBAL_SPEED = 90;
const int GLOBAL_STEPS = 200;
const int GLOBAL_TARGET = 30;

int buttonUpState;
int buttonSelectState;
int buttonDownState;
int *enginePos;
int lastStroke = 0;

// bool haltReadingButtons = false;

void readingButtons()
{
  buttonUpState = digitalRead(buttonUp);
  buttonSelectState = digitalRead(buttonSelect);
  buttonDownState = digitalRead(buttonDown);
}

EnginesSet guitar;

SDCard sdCard;
int targetScreen = 1;
string defStroke = "";
void taskStroke(void *parameter)
{
  readingButtons();
  guitar.parseFile(defStroke,0);
  lastStroke = 0;
  // vTaskDelete(NULL);
}

bool breakLoopFlag = false;

// References the current songLoop. Used to delete it from outside the task
TaskHandle_t loopHandle = NULL; 

/**
 * @brief Loop that runs on another core and plays the song
 */
void songLoop(void* parameter)
{
  // Plays each string once before the song starts
  guitar.standardBeat();

  guitar.setLastMillis();

  while(1)
  {
    guitar.parseFile(defStroke, 0);  

    readingButtons();
    if (breakLoopFlag) break;
  }

  lastStroke = 0;
  vTaskDelete(NULL);
}

void settings(int *targetScreen)
{
  tft.fillScreen(ST7735_BLACK);
  int goBack = 0;
  int posSettings = 2;
  while (goBack == 0)
  {
    readingButtons();
    if (posSettings == 2)
    {
      tft.setCursor(0, 0);
      tft.setTextSize(3);
      tft.println("Menu:");
      tft.setTextSize(2);
      tft.println("");
      tft.write(16);
      tft.println("Musicas");
      tft.println(" Afinar");
      tft.println(" Resetar");
    }
    if (posSettings == 3)
    {
      tft.setCursor(0, 0);
      tft.setTextSize(3);
      tft.println("Menu:");
      tft.setTextSize(2);
      tft.println("");
      tft.println(" Musicas");
      tft.write(16);
      tft.println("Afinar");
      tft.println(" Resetar");
    }
    if (posSettings == 4)
    {
      tft.setCursor(0, 0);
      tft.setTextSize(3);
      tft.println("Menu:");
      tft.setTextSize(2);
      tft.println("");
      tft.println(" Musicas");
      tft.println(" Afinar");
      tft.write(16);
      tft.println("Resetar");
    }

    if (buttonUpState == 0 && posSettings != 2)
    {
      posSettings--;
      tft.fillRect(0, 24, 10, 225, ST7735_BLACK);
      delay(delayButtons);
    }
    if (buttonDownState == 0 && posSettings != 4)
    {
      posSettings++;
      tft.fillRect(0, 24, 10, 225, ST7735_BLACK);
      delay(delayButtons);
    }
    if (buttonSelectState == 0)
    {
      goBack = 1;
      delay(delayButtons);
    }
  }
  *targetScreen = posSettings;
}
void strokes(string firstStroke, string secondStroke, string thirdStroke, int nStrokes)
{
  tft.fillScreen(ST7735_BLACK);
  int returnPlaying = 0;
  int playingPos = 1;
  guitar.setDelay();

  while (returnPlaying == 0)
  {
    readingButtons();
    if (playingPos == 1)
    {
      tft.setCursor(35, 0);//80,0
      tft.setTextSize(2);
      tft.println("BATIDA 1");
      tft.setCursor(0, 55);//0,100
      tft.write(27);
      tft.setCursor(148, 55);//300,100
      tft.write(26);
      tft.setTextSize(2);
      tft.setCursor(40, 100);//100,160
      tft.write(16);
      tft.println("Voltar");
      defStroke = firstStroke;
      if (lastStroke == 0)
      {
        lastStroke = 1;
        breakLoopFlag = false;
        xTaskCreatePinnedToCore(songLoop, "songLoop", 1000, NULL, 1, &loopHandle, 0);
        
        // taskStroke(NULL);
        
        // esp_ipc_call(PRO_CPU_NUM, taskStroke, NULL);
      }
    }
    if (playingPos == 2)
    {
      tft.setCursor(35, 0);
      tft.setTextSize(2);
      tft.println("BATIDA 2");
      tft.setCursor(0, 55);
      tft.write(27);
      tft.setCursor(148, 55);
      tft.write(26);
      tft.setTextSize(2);
      tft.setCursor(40, 100);
      tft.write(16);
      tft.println("Voltar");
      defStroke = secondStroke;
      if (lastStroke == 0)
      {
        lastStroke = 1;
        breakLoopFlag = false;
        xTaskCreatePinnedToCore(songLoop, "songLoop", 1000, NULL, 1, &loopHandle, 0);
        
        // taskStroke(NULL);

        // esp_ipc_call(PRO_CPU_NUM, taskStroke, NULL);
      }
    }
    if (playingPos == 3)
    {
      //tft.setCursor(0, 0);
      tft.setCursor(35, 0);
      tft.setTextSize(2);
      tft.println("BATIDA 3");
      tft.setCursor(0, 55);
      tft.write(27);
      tft.setCursor(148, 55);
      tft.write(26);
      tft.setTextSize(2);
      tft.setCursor(40, 100);
      tft.write(16);
      tft.println("Voltar");
      defStroke = thirdStroke;
      if (lastStroke == 0)
      {
        lastStroke = 1;
        breakLoopFlag = false;
        xTaskCreatePinnedToCore(songLoop, "songLoop", 1000, NULL, 1, &loopHandle, 0);

        // taskStroke(NULL);
        
        // esp_ipc_call(PRO_CPU_NUM, taskStroke, NULL);
      }
    }

    guitar.getEnginePos(sdCard);

    if (buttonUpState == 0)
    {
      tft.fillRect(114, 0, 20, 20, ST7735_BLACK);
      if (nStrokes == 3)
      {
        if (playingPos != 1)
        {
          playingPos--;
        }
        else
        {
          playingPos = 3;
        }
      }
      if (nStrokes == 2)
      {
        if (playingPos != 1)
        {
          playingPos--;
        }
        else
        {
          playingPos = 2;
        }
      }
      if (nStrokes == 1)
      {
        if (playingPos != 1)
        {
          playingPos--;
        }
        else
        {
          playingPos = 1;
        }
      }
      delay(delayButtons);
    }
    if (buttonDownState == 0)
    {
      tft.fillRect(114, 0, 20, 20, ST7735_BLACK);
      if (nStrokes == 3)
      {
        if (playingPos != 3)
        {
          playingPos++;
        }
        else
        {
          playingPos = 1;
        }
      }
      if (nStrokes == 2)
      {
        if (playingPos != 2)
        {
          playingPos++;
        }
        else
        {
          playingPos = 1;
        }
      }
      if (nStrokes == 1)
      {
        if (playingPos != 1)
        {
          playingPos++;
        }
        else
        {
          playingPos = 1;
        }
      }
      delay(delayButtons);
    }
    if (buttonSelectState == 0)
    {
      if (loopHandle != NULL)
      {
        vTaskDelete(loopHandle);
        lastStroke = 0;
      }

      breakLoopFlag = true;

      returnPlaying = 1;
      delay(delayButtons);
    }
  }
  tft.fillScreen(ST7735_BLACK);

}

void music(int music)
{
  // Declarando as variáveis locais
  tft.fillScreen(ST7735_BLACK);
  string firstStroke = sdCard.readFile(music, '1');
  string secondStroke = sdCard.readFile(music, '2');
  string thirdStroke = sdCard.readFile(music, '3');

  //funcao do SD que seta a velocidade SDCard.SD_SPEED
  guitar.setEngineSpeed(sdCard);
  //função do EnginesSet que seta o número de subdivisões SD_SUBDIVISION
  guitar.setSubdivision(sdCard);
  
  int nStrokes = 3;
  if (secondStroke.length() == 0 && thirdStroke.length() == 0)
  {
    nStrokes = 1;
  }
  if (secondStroke.length() != 0 && thirdStroke.length() == 0)
  {
    nStrokes = 2;
  }

  int goBack = 0;
  int goBackMusica = 0;
  int menuPos = 1;
  while (goBackMusica == 0)

  {
    while (goBack == 0)
    {
      if(lastStroke == 0)
      {
        guitar.endMusic();
      }

      readingButtons();
      if (menuPos == 1)
      {
        tft.setCursor(0, 0);
        tft.setTextSize(2);
        tft.print(musicNames[music - 1].c_str());
        tft.println(":");
        tft.setTextSize(2);
        tft.println("");
        tft.print("-");
        tft.println(sdCard.authorName.c_str());
        tft.setCursor(0, 80);
        tft.write(16);
        tft.println("Tocar");
        tft.println(" Voltar");
      }
      if (menuPos == 2)
      {
        tft.setCursor(0, 0);
        tft.setTextSize(2);
        tft.print(musicNames[music - 1].c_str());
        tft.println(":");
        tft.setTextSize(2);
        tft.println("");
        tft.print("-");
        tft.println(sdCard.authorName.c_str());
        tft.setCursor(0, 80);
        tft.println(" Tocar");
        tft.write(16);
        tft.println("Voltar");
      }

      if (buttonUpState == 0 && menuPos != 1)
      {
        menuPos--;
        tft.fillRect(0, 80, 10, 150, ST7735_BLACK);
        delay(delayButtons);
      }
      if (buttonDownState == 0 && menuPos != 2)
      {
        menuPos++;
        tft.fillRect(0, 80, 10, 150, ST7735_BLACK);
        delay(delayButtons);
      }
      if (buttonSelectState == 0)
      {
        goBack = 1;
        tft.fillScreen(ST7735_BLACK);
        delay(delayButtons);
      }
    }
    if (menuPos == 1)
    {
      strokes(firstStroke, secondStroke, thirdStroke, nStrokes);
      goBack = 0;
    }
    if (menuPos == 2)
    {
      goBackMusica = 1;
    }
  }
}

void menu(int *targetScreen)
{
  // Declarando as variáveis locais
  tft.fillScreen(ST7735_BLACK);
  int goBackCursor = 0;
  int selectedMusic = 0;
  int goBack = 0;
  int menuPos = 1;
  int screenNumber = musicNumber / 10;
  int lastMusicScreenNumber = musicNumber % 10;
  int presentScreen = 0;
  if (lastMusicScreenNumber != 0)
  {
    screenNumber += 1;
  }
  int menuPosMax = musicNumber + screenNumber;
  while (goBack == 0)
  {
    readingButtons();
    if (presentScreen == 0)
    {
      if (musicNumber < 10)
      {
        tft.setCursor(0, 0);
        tft.setTextSize(2);
        tft.println("Musicas:");
        tft.setTextSize(1);
        tft.println("");
        for (j = 1; j < musicNumber + 1; j++)
        {
          if (menuPos == j)
          {
            tft.write(16);
            tft.print(j);
            tft.print("-");
            tft.println(musicNames[j - 1].c_str());
            selectedMusic = j;
          }
          else
          {
            tft.print(" ");
            tft.print(j);
            tft.print("-");
            tft.println(musicNames[j - 1].c_str());
          }
        }
        if (menuPos == menuPosMax)
        {
          tft.write(16);
          tft.print("Voltar ");
          goBackCursor = 1;
        }
        else
        {
          tft.print(" Voltar ");
          goBackCursor = 0;
        }
      }
      if (musicNumber > 9)
      {
        tft.setCursor(0, 0);
        tft.setTextSize(2);
        tft.println("Musicas:");
        tft.setTextSize(1);
        tft.println("");
        for (j = 1; j < 11; j++)
        {
          if (menuPos == j)
          {
            tft.write(16);
            tft.print(j);
            tft.print("-");
            tft.println(musicNames[j - 1].c_str());
            selectedMusic = j;
          }
          else
          {
            tft.print(" ");
            tft.print(j);
            tft.print("-");
            tft.println(musicNames[j - 1].c_str());
          }
        }
        if (menuPos % 11 == 0)
        {
          tft.write(16);
          tft.print("Voltar ");
          goBackCursor = 1;
        }
        else
        {
          tft.print(" Voltar ");
          goBackCursor = 0;
        }
      }
      delay(10);
    }
    if (presentScreen > 0 && presentScreen < screenNumber - 1)
    {
      tft.setCursor(0, 0);
      tft.setTextSize(2);
      tft.println("Musicas:");
      tft.setTextSize(1);
      tft.println("");
      for (j = (10 * presentScreen) + 1; j < (10 * (presentScreen + 1)) + 1; j++)
      {
        if ((menuPos - presentScreen) == j)
        {
          tft.write(16);
          tft.print(j);
          tft.print("-");
          tft.println(musicNames[j - 1].c_str());
          selectedMusic = j;
        }
        else
        {
          tft.print(" ");
          tft.print(j);
          tft.print("-");
          tft.println(musicNames[j - 1].c_str());
        }
      }
      if (menuPos % 11 == 0)
      {
        tft.write(16);
        tft.print("Voltar ");
        goBackCursor = 1;
      }
      else
      {
        tft.print(" Voltar ");
        goBackCursor = 0;
      }
    }
    if (presentScreen == screenNumber - 1 && screenNumber != 1)
    {
      tft.setCursor(0, 0);
      tft.setTextSize(2);
      tft.println("Musicas:");
      tft.setTextSize(1);
      tft.println("");
      for (j = (10 * presentScreen) + 1; j < musicNumber + 1; j++)
      {
        if ((menuPos - presentScreen) == j)
        {
          tft.write(16);
          tft.print(j);
          tft.print("-");
          tft.println(musicNames[j - 1].c_str());
          selectedMusic = j;
        }
        else
        {
          tft.print(" ");
          tft.print(j);
          tft.print("-");
          tft.println(musicNames[j - 1].c_str());
        }
      }
      if (menuPos == menuPosMax)
      {
        tft.write(16);
        tft.print("Voltar ");
        goBackCursor = 1;
      }
      else
      {
        tft.print(" Voltar ");
        goBackCursor = 0;
      }
    }
    delay(10);

    if (buttonUpState == 0 && menuPos != 1)
    {
      tft.fillRect(0, 20, 5, 100, ST7735_BLACK);
      //tft.fillRect(0, 24, 10, 225, ST7735_BLACK);
      menuPos--;
      if (menuPos % 11 == 0)
      {
        presentScreen--;
        tft.fillScreen(ST7735_BLACK);
      }
      delay(delayButtons);
    }
    if (buttonDownState == 0 && menuPos != menuPosMax)
    {
      tft.fillRect(0, 20, 5, 100, ST7735_BLACK);
      //tft.fillRect(0, 24, 10, 225, ST7735_BLACK);
      if (menuPos % 11 == 0)
      {
        presentScreen++;
        tft.fillScreen(ST7735_BLACK);
      }
      menuPos++;
      delay(delayButtons);
    }
    if (buttonSelectState == 0)
    {
      if (goBackCursor == 0)
      {
        music(selectedMusic);
        delay(delayButtons);
      }
      else
      {
        *targetScreen = 1;
        goBack = 1;
        delay(delayButtons);
      }
    }
  }
}

int tunePos = 0;
void taskTune(void *parameter) // Tem que estar nessa posição, depois de setEngine e antes de afinar
{
  guitar.tune(tunePos);
  vTaskDelete(NULL);
}
void afinar(int *targetScreen)
{
  tft.fillScreen(ST7735_BLACK);
  int goBack = 0;
  tunePos = 0;
  while (goBack == 0)
  {
    readingButtons();
    if (tunePos == 0)
    {
      tft.setCursor(0, 0);
      tft.setTextSize(2);
      tft.println("Selecione:");
      tft.setTextSize(1);
      tft.println("");
      tft.write(16);
      tft.println("Corda E");
      tft.println(" Corda A");
      tft.println(" Corda D");
      tft.println(" Corda G");
      tft.println(" Corda B");
      tft.println(" Corda e");
      tft.println(" Voltar");
    }
    if (tunePos == 1)
    {
      tft.setCursor(0, 0);
      tft.setTextSize(2);
      tft.println("Selecione:");
      tft.setTextSize(1);
      tft.println("");
      tft.println(" Corda E");
      tft.write(16);
      tft.println("Corda A");
      tft.println(" Corda D");
      tft.println(" Corda G");
      tft.println(" Corda B");
      tft.println(" Corda e");
      tft.println(" Voltar");
    }
    if (tunePos == 2)
    {
      tft.setCursor(0, 0);
      tft.setTextSize(2);
      tft.println("Selecione:");
      tft.setTextSize(1);
      tft.println("");
      tft.println(" Corda E");
      tft.println(" Corda A");
      tft.write(16);
      tft.println("Corda D");
      tft.println(" Corda G");
      tft.println(" Corda B");
      tft.println(" Corda e");
      tft.println(" Voltar");
    }
    if (tunePos == 3)
    {
      tft.setCursor(0, 0);
      tft.setTextSize(2);
      tft.println("Selecione:");
      tft.setTextSize(1);
      tft.println("");
      tft.println(" Corda E");
      tft.println(" Corda A");
      tft.println(" Corda D");
      tft.write(16);
      tft.println("Corda G");
      tft.println(" Corda B");
      tft.println(" Corda e");
      tft.println(" Voltar");
    }
    if (tunePos == 4)
    {
      tft.setCursor(0, 0);
      tft.setTextSize(2);
      tft.println("Selecione:");
      tft.setTextSize(1);
      tft.println("");
      tft.println(" Corda E");
      tft.println(" Corda A");
      tft.println(" Corda D");
      tft.println(" Corda G");
      tft.write(16);
      tft.println("Corda B");
      tft.println(" Corda e");
      tft.println(" Voltar");
    }
    if (tunePos == 5)
    {
      tft.setCursor(0, 0);
      tft.setTextSize(2);
      tft.println("Selecione:");
      tft.setTextSize(1);
      tft.println("");
      tft.println(" Corda E");
      tft.println(" Corda A");
      tft.println(" Corda D");
      tft.println(" Corda G");
      tft.println(" Corda B");
      tft.write(16);
      tft.println("Corda e");
      tft.println(" Voltar");
    }
    if (tunePos == 6)
    {
      tft.setCursor(0, 0);
      tft.setTextSize(2);
      tft.println("Selecione:");
      tft.setTextSize(1);
      tft.println("");
      tft.println(" Corda E");
      tft.println(" Corda A");
      tft.println(" Corda D");
      tft.println(" Corda G");
      tft.println(" Corda B");
      tft.println(" Corda e");
      tft.write(16);
      tft.println("Voltar");
    }
    delay(10);

    if (buttonUpState == 0 && tunePos != 0)
    {
      tunePos--;
      tft.fillRect(0, 20, 5, 100, ST7735_BLACK);
      delay(delayButtons);
    }
    if (buttonDownState == 0 && tunePos != 6)
    {
      tunePos++;
      tft.fillRect(0, 20, 5, 100, ST7735_BLACK);
      delay(delayButtons);
    }
    if (buttonSelectState == 0)
    {
      if (tunePos != 6)
      {
        xTaskCreatePinnedToCore(taskTune, "taskTune", 1000, NULL, 1, NULL, 0);
      }
      else
      {
        guitar.endMusic();
        tft.fillScreen(ST7735_BLACK);
        goBack = 1;
        *targetScreen = 1;
      }
      delay(delayButtons);
    }
    guitar.getEnginePos(sdCard);
  }
 }

void adjustEngine(int *resetPos)
{
  int guitarString = *resetPos;
  string guitarStrings = "EADGBe";
  char guitarStringChr = guitarStrings[guitarString];
  tft.fillScreen(ST7735_BLACK);
  int goBack = 0;
  int adjustPos = 0;
  while (goBack == 0)
  {
    readingButtons();
    if (adjustPos == 0)
    {
      tft.setCursor(0, 0);
      tft.setTextSize(2);
      tft.print("Corda ");
      tft.print(guitarStringChr);
      tft.println(":");
      tft.setTextSize(2);
      tft.println("");
      tft.write(16);
      tft.println("Subir");
      tft.println(" Descer");
      tft.println(" Pronto");
    }
    if (adjustPos == 1)
    {
      tft.setCursor(0, 0);
      tft.setTextSize(2);
      tft.print("Corda ");
      tft.print(guitarStringChr);
      tft.println(":");
      tft.setTextSize(2);
      tft.println("");
      tft.println(" Subir");
      tft.write(16);
      tft.println("Descer");
      tft.println(" Pronto");
    }
    if (adjustPos == 2)
    {
      tft.setCursor(0, 0);
      tft.setTextSize(2);
      tft.print("Corda ");
      tft.print(guitarStringChr);
      tft.println(":");
      tft.setTextSize(2);
      tft.println("");
      tft.println(" Subir");
      tft.println(" Descer");
      tft.write(16);
      tft.println("Pronto");
    }

    if (buttonUpState == 0 && adjustPos != 0)
    {
      adjustPos--;
      tft.fillRect(0, 24, 10, 225, ST7735_BLACK);
      delay(delayButtons);
    }
    if (buttonDownState == 0 && adjustPos != 2)
    {
      adjustPos++;
      tft.fillRect(0, 24, 10, 225, ST7735_BLACK);
      delay(delayButtons);
    }
    if (buttonSelectState == 0)
    {
      if (adjustPos == 0)
      {
        guitar.playOneStep(guitarString, 1);
        guitar.getEnginePos(sdCard);
      }
      if (adjustPos == 1)
      {
        guitar.playOneStep(guitarString, -1);
        guitar.getEnginePos(sdCard);
      }
      if (adjustPos == 2)
      {
        guitar.runHalfTarget(guitarString);
        tft.fillScreen(ST7735_BLACK);
        goBack = 1;
        *resetPos = 1;
        delay(delayButtons);
      }
    }
  }
}

void resetEngines(int *targetScreen)
{
  tft.fillScreen(ST7735_BLACK);
  int goBack = 0;
  int resetPos = 0;
  while (goBack == 0)
  {
    readingButtons();
    if (resetPos == 0)
    {
      tft.setCursor(0, 0);
      tft.setTextSize(2);
      tft.println("Selecione:");
      tft.setTextSize(1);
      tft.println("");
      tft.write(16);
      tft.println("Corda E");
      tft.println(" Corda A");
      tft.println(" Corda D");
      tft.println(" Corda G");
      tft.println(" Corda B");
      tft.println(" Corda e");
      tft.println(" Voltar");
    }
    if (resetPos == 1)
    {
      tft.setCursor(0, 0);
      tft.setTextSize(2);
      tft.println("Selecione:");
      tft.setTextSize(1);
      tft.println("");
      tft.println(" Corda E");
      tft.write(16);
      tft.println("Corda A");
      tft.println(" Corda D");
      tft.println(" Corda G");
      tft.println(" Corda B");
      tft.println(" Corda e");
      tft.println(" Voltar");
    }
    if (resetPos == 2)
    {
      tft.setCursor(0, 0);
      tft.setTextSize(2);
      tft.println("Selecione:");
      tft.setTextSize(1);
      tft.println("");
      tft.println(" Corda E");
      tft.println(" Corda A");
      tft.write(16);
      tft.println("Corda D");
      tft.println(" Corda G");
      tft.println(" Corda B");
      tft.println(" Corda e");
      tft.println(" Voltar");
    }
    if (resetPos == 3)
    {
      tft.setCursor(0, 0);
      tft.setTextSize(2);
      tft.println("Selecione:");
      tft.setTextSize(1);
      tft.println("");
      tft.println(" Corda E");
      tft.println(" Corda A");
      tft.println(" Corda D");
      tft.write(16);
      tft.println("Corda G");
      tft.println(" Corda B");
      tft.println(" Corda e");
      tft.println(" Voltar");
    }
    if (resetPos == 4)
    {
      tft.setCursor(0, 0);
      tft.setTextSize(2);
      tft.println("Selecione:");
      tft.setTextSize(1);
      tft.println("");
      tft.println(" Corda E");
      tft.println(" Corda A");
      tft.println(" Corda D");
      tft.println(" Corda G");
      tft.write(16);
      tft.println("Corda B");
      tft.println(" Corda e");
      tft.println(" Voltar");
    }
    if (resetPos == 5)
    {
      tft.setCursor(0, 0);
      tft.setTextSize(2);
      tft.println("Selecione:");
      tft.setTextSize(1);
      tft.println("");
      tft.println(" Corda E");
      tft.println(" Corda A");
      tft.println(" Corda D");
      tft.println(" Corda G");
      tft.println(" Corda B");
      tft.write(16);
      tft.println("Corda e");
      tft.println(" Voltar");
    }
    if (resetPos == 6)
    {
      tft.setCursor(0, 0);
      tft.setTextSize(2);
      tft.println("Selecione:");
      tft.setTextSize(1);
      tft.println("");
      tft.println(" Corda E");
      tft.println(" Corda A");
      tft.println(" Corda D");
      tft.println(" Corda G");
      tft.println(" Corda B");
      tft.println(" Corda e");
      tft.write(16);
      tft.println("Voltar");
    }
    delay(10);

    if (buttonUpState == 0 && resetPos != 0)
    {
      resetPos--;
      tft.fillRect(0, 20, 5, 100, ST7735_BLACK);
      delay(delayButtons);
    }
    if (buttonDownState == 0 && resetPos != 6)
    {
      resetPos++;
      tft.fillRect(0, 20, 5, 100, ST7735_BLACK);
      delay(delayButtons);
    }
    if (buttonSelectState == 0)
    {
      if (resetPos != 6)
      {
        adjustEngine(&resetPos);
      }
      else
      {
        tft.fillScreen(ST7735_BLACK);
        goBack = 1;
        *targetScreen = 1;
      }
      delay(delayButtons);
    }
  }
}

void setup()
{
  //Serial.begin(115200);

  //                    step, dir)
  guitar.insertMotor('E', 3, 21);
  guitar.insertMotor('A', 26, 25); 
  guitar.insertMotor('D', 13, 12);
  guitar.insertMotor('G', 33 , 32);
  guitar.insertMotor('B', 14, 27);
  guitar.insertMotor('e', 22, 1 );


  pinMode(buttonUp, INPUT_PULLUP);
  pinMode(buttonSelect, INPUT_PULLUP);
  pinMode(buttonDown, INPUT_PULLUP);


  musicNames = sdCard.getList();
  musicNumber = musicNames.size();

  String currentTargets = "";
  // currentTargets = sdCard.getCurrentTarget();
  // guitar.setCurrentTarget(currentTargets);

  /*guitar.runHalfTarget(0);
  guitar.runHalfTarget(1);
  guitar.runHalfTarget(2);
  guitar.runHalfTarget(3);
  guitar.runHalfTarget(4);
  guitar.runHalfTarget(5);*/
  //Serial.println("Inicializado display...");

  // Teste para a inicialização do TFT
  tft.initR(INITR_BLACKTAB);
  tft.setRotation(-1);

  // Inicializar a fonte com tamanho padrão
  tft.setTextSize(2);              // Tamanho do texto
  tft.setTextColor(ST7735_WHITE); // Cor do texto

  // Limpa o display
  tft.fillScreen(ST7735_BLACK);

  readingButtons();
  }

void loop()
{
  switch (targetScreen)
  {
  case 1:
  {
    settings(&targetScreen);
    break;
  }
  case 2:
  {
    menu(&targetScreen);
    break;
  }
  case 3:
  {
    afinar(&targetScreen);
    break;
  }
  case 4:
  {
    resetEngines(&targetScreen);
    break;
  }
  }

}
