#include "SDCard.hpp"

SDCard::SDCard()
{
  //SD_SPEED = 90;
  SD.begin(5);
  Makelist();
}

vector<string> SDCard::getList()
{
  return this->mFilesNames;
}

void SDCard::Makelist()
{
  File root = SD.open("/musicas");
  while (true)
  {
    File entry = root.openNextFile();
    if (!entry)
    {
      break;
    }
    std::regex reg("\\.txt$");
    std::string noTxt = std::regex_replace(entry.name(), reg, "");
    this->mFilesNames.push_back(noTxt);
  }
}

void SDCard::printPosition(std::string pFileName)
{
  const char *c = pFileName.c_str();
  File textFile = SD.open(c);
  if (textFile)
  {
    //Serial.print("Conteudo do arquivo de posição: \n");
    // Comentado para testar Integração 4.0
    // while (textFile.available())
    // {
    //   Serial.write(textFile.read());
    // }
    // textFile.close();
  }
  else
  {
    //Serial.println("error opening file");
  }
}

string SDCard::readFile(int pNumFile, char pNumStroke)
{
  File text = SD.open("/musicas");
  string verify = "";
  string file = "";
  int goBack = 0;
  for (int i = 0; i < pNumFile; i++)
  {
    File textFile = text.openNextFile();
    if (i == pNumFile - 1)
    {
      if (textFile)
      {
        while (textFile.available())
        {
          char ch = textFile.read();
          if (ch == pNumStroke)
          {
            ch = textFile.read();
            if (ch == '<')
            {
              while (goBack != 1)
              {
                ch = textFile.read();
                if (ch == '>')
                {
                  goBack = 1;
                }
                else
                {
                  file += ch;
                }
              }
            }
          }
          else if (ch == 'V')
          {
            string num = "";
            int vel;
            ch = textFile.read();
            if (ch == '<') {
              for (int i = 0; i < 3; i++)
              {
                ch = textFile.read();
                num += ch;
              }
              vel = stoi(num);
              SD_SPEED = vel;

            }
            else if (ch == '>') {
              break;
            }
          }
          else if (ch == 'N') {
            authorName = "";
            ch = textFile.read();
            if (ch == '<') {
              while(ch != '>')
              {
                ch = textFile.read();
                if(ch != '>'){
                authorName += ch;
                }
              }
            }
          }
          else if (ch == 'S')
          {
            string sub = "";
            int numSubdivision;
            ch = textFile.read();
            if (ch == '<') {
              while(ch != '>')
              {
                ch = textFile.read();
                if(ch != '>'){
                sub += ch;
                }
              }
            }
            //numSubdivision = stoi(sub);
            //SD_SUBDIVISION = numSubdivision;
            SD_SUBDIVISION = sub;
          }
          else
          {
            verify = "";
          }
        }
        return file;
        textFile.close();
      }
    }
  }
}

void SDCard::writeInFile(vector<int> pEnginePos)
{
  // const char * fileNamePtr = fileName.c_str();
/*
  File textFile = SD.open("/final_position.txt", FILE_WRITE);

  if (textFile)
  {
    //Serial.print("\n Gravando a posição dos motores... \n");
    for (auto &i : pEnginePos)
    {
      textFile.println(i);
    }
    // textFile.println(enginePos);
    textFile.close();
  }
  */
}

String SDCard::getCurrentTarget()
{
  String fileContent = "";
  File textFile = SD.open("/final_position.txt");
  while (textFile.available())
  {
    char ch = textFile.read();
    fileContent += ch;
  }
  // Serial.println(fileContent);
  return fileContent;
}