//Nesse arquivo é definida a classe que modela o dispositivo joystick

#ifndef JOYSTICK_
#define JOYSTICK_
#define DEAD_ZONE 25 // O valor mínimo de inclinação do joystick até que ele retorne o primeiro valor

//Essa classe modela um dispositivo real do tipo joystick. Ela lê as entradas analógicas passadas pelo dispositivo e faz a conversões necessárias para repassar a inclinação dele e o pressionar do botão.

class Joystick
{
    private:
        int pin_X, pin_Y, button; //pinos do joystick (funciona como dois potenciômetros e um botão)
        int X, Y; //Inclinação do joystick
        int pinNumber;

    public:
        Joystick(int cpin_X, int cpin_Y, int cbutton); 
        Joystick(int cpin_X, int cbutton);

        /*Os métodos abaixo retornam as inclinações dos joysticks, em porcentagem valores de -100 a 100,
        onde -100 o Joystick está totalmente inclinado pra esquerda (quando na direção X) ou para cima (quando na direção Y)
        e 100 indica que o Joystick está totalmente inclinado para a direita (quando na direção X) ou para baixo, (quando
        na direção Y). Zero indica que o Joystick está centralizado, a menos da deadzone */
        
        bool get_button(); //retorna o acionamento do botão sob o joystick

        int get_X();
        int get_Y();

        bool Y_asButtonUp();
        bool Y_asButtonDown();
        bool X_asButtonDown();
        bool X_asButtonUp();

};


#endif