/*******************************************************************************
   Título:      TCC  Engenharia Mecatrônica.

   Descrição:   C L P   ---  I H M

   Processador: ARDUINO MEGA ADK  
   
   Geração Multifolhas


   Desenvolvido: MRPRO Tecnologia e Automação

   Autor: Tecnologo. Marcelo Rodrigues     Crea-SP 5070505617

   Data de Inicio: 11/05/2025       Versão:   2025.1.0

        

 
*******************************************************************************/  
/* ============================================================================  
                                                              
                                       _                      
                                      / \                     
                                     |oo >                    
                                     _\=/_                    
                    ___         #   /  _  \   #               
                   /<> \         \\//|/.\|\\//                
                 _|_____|_        \/  \_/  \/                 
                | | === | |          |\ /|                    
                |_|  0  |_|          \_ _/                    
                 ||  0  ||           | | |                    
                 ||__*__||           | | |                    
                |* \___/ *|          []|[]                    
                /=\ /=\ /=\          | | |                    
________________[_]_[_]_[_]_________/_]_[_\___________________________________
                                                                             
============================================================================== */

/* 
                 Comunicação I2C
             Cabo Verde SDA  Pino 20
             Cabo Azul  SCL  Pino 21 

*/

// ========================================================================================================
// --- Bibliotecas Auxiliares ---

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <Adafruit_GFX.h>
#include <Adafruit_BusIO_Register.h>
#include <TCA9548A.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_SSD1306.h>

// =============================================================================================================
// --- Mapeamento de Hardware ---
#define  col_1   33                  //coluna 1 do teclado
#define  col_2   34                  //coluna 2 do teclado
#define  col_3   35                 //coluna 3 do teclado
#define  col_4   36                 //coluna 4 do teclado
#define  row_A   38                  //linha A  do teclado
#define  row_B   39                 //linha B  do teclado
#define  row_C   40                 //linha C  do teclado
#define  row_D   41                 //linha D  do teclado
#define  row_E  37                 //linha E  do teclado


#define OLED_RESET 3


/**********************************************************************************************************/
                                        /* Arquivos de inclusão */





LiquidCrystal_I2C lcd(0x27,20,4);            // Set the LCD address to 0x27 for a 16 chars and 2 line display
Adafruit_SSD1306 display(OLED_RESET);        // Objeto para o OLED
TCA9548A I2CMux;                             // Address can be passed into the constructor



//==============================================================
//  -----Diretivas TCA9548A
#define i2clcd        0
#define i2coled       1





#define usalcd    I2CMux.openChannel(i2clcd)
#define usaoled   I2CMux.openChannel(i2coled)



// =============================================================================================================
// --- Protótipo das Funções ---
void readKeyboard();                  //Função para leitura do teclado
void store(char value);               //Função para armazenar o número digitado no teclado matricial
void numero();                        //Função para imprimir o número digitado na tela do LCD


// =============================================================================================================
// --- Variáveis Globais ---
char control = 0x01;                  //variável de controle de teclado
char counter = 0x00;                  //variável auxiliar de contagem
int  number_teclado  = 0x00;          //variável para armazenar o número pressionado no teclado


// =============================================================================================================
// --- Interrupção ---
ISR(TIMER2_OVF_vect)
{
    TCNT2=100;          // Reinicializa o registrador do Timer2
    counter++;          // incrementa counter
    
    if(counter == 0x05) // counter igual a D'5'?
    {                   // sim...
    
       counter = 0x00;  //reinicia counter
       
       readKeyboard();  //lê teclado     

            
    
    } //end if counter
}


// =============================================================================================================
// --- Configurações Iniciais ---
void setup()
{
                                   Wire.begin();
                           Serial.begin(115200);
                             I2CMux.begin(Wire);
                 
            


                                         usalcd;
                                     delay(100);
                                     lcd.init();
                                lcd.backlight();       
                         I2CMux.closeChannel(0);
       
                                         usalcd;
                                    lcd.clear(); 
                             lcd.setCursor(2,1);
                  lcd.print("MRPRO TECNOLOGIA");
                         I2CMux.closeChannel(0);





                                         usaoled;
       display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
                          display.clearDisplay();
                          I2CMux.closeChannel(1);




                                         usaoled;
                          display.clearDisplay();
                          display.setTextSize(1);
                     display.setTextColor(WHITE);
                         display.setCursor(30,0);
                     display.print("Tecnologia");
                                      delay(100);
                               display.display();
                          display.setTextSize(2);
                        display.setCursor(30,10);
                          display.print("MRPRO");
                                      delay(100);
                               display.display();  
                          I2CMux.closeChannel(1);



           pinMode(row_A, OUTPUT);
           pinMode(row_B, OUTPUT);
           pinMode(row_C, OUTPUT);
           pinMode(row_D, OUTPUT);
           pinMode(row_E, OUTPUT);
       
            pinMode(col_1, INPUT);                                //Saída para varredura da coluna 1
            pinMode(col_2, INPUT);
            pinMode(col_3, INPUT);
            pinMode(col_4, INPUT);
     
     
     digitalWrite(col_1, HIGH);                             //Inicializa coluna 1 em HIGH
     digitalWrite(col_2, HIGH);                             //Inicializa coluna 2 em HIGH
     digitalWrite(col_3, HIGH);                             //Inicializa coluna 3 em HIGH
     digitalWrite(col_4, HIGH);                             //Inicializa coluna 4 em HIGH

     
     TCCR2A = 0x00;   //Timer operando em modo normal
     TCCR2B = 0x07;   //Prescaler 1:1024
     TCNT2  = 100;    //10 ms overflow again
     TIMSK2 = 0x01;   //Habilita interrupção do Timer2
 
} //end setup


// =============================================================================================================
// --- Loop Infinito ---
void loop()
{
  

   
    
} //end loop



   // Estouro = Timer2_cont x prescaler x ciclo de máquina
   
   // Ciclo de máquina = 1/Fosc = 1/16E6 = 62,5ns = 62,5E-9s
   
   // Estouro = (256 - 100) x 1024 x 62,5E-9 = 9,98ms 

   // Varredura = Estouro x Counter = 9,98ms x 5 = 49,9ms


// =============================================================================================================
// --- Desenvolvimento das Funções ---
void readKeyboard()                       //Função para leitura do teclado
{

   if(digitalRead(col_1) && control == 0x01)       //Coluna 1 em nível high? Control igual 1?
   {                                               //Sim...
         control = 0x02;                           //control igual a 2
         digitalWrite(col_1,  LOW);                //apenas coluna 1 em nível baixo
         digitalWrite(col_2, HIGH);
         digitalWrite(col_3, HIGH);
         digitalWrite(col_4, HIGH);
      
      // -- Testa qual tecla foi pressionada e armazena o valor --
         if     (!digitalRead(row_A))  store(11);    // Digita na tela do lcd "F1"
         else if(!digitalRead(row_B))  store(1);
         else if(!digitalRead(row_C))  store(4);
         else if(!digitalRead(row_D))  store(7);
         else if(!digitalRead(row_E))  store(12);    // Digita na tela do lcd "<-"  
   } //end if col_1
   
   else if(digitalRead(col_2) && control == 0x02)   //Coluna 2 em nível high? Control igual 2?
   {                                                //Sim...
         control = 0x03;                            //control igual a 3
         digitalWrite(col_1, HIGH);
         digitalWrite(col_2,  LOW);                 //apenas coluna 2 em nível baixo
         digitalWrite(col_3, HIGH);
         digitalWrite(col_4, HIGH);
         
      // -- Testa qual tecla foi pressionada e armazena o valor --
         if     (!digitalRead(row_A))  store(21);       // Digita da tela do lcd "F2"
         else if(!digitalRead(row_B))  store(2);
         else if(!digitalRead(row_C))  store(5);
         else if(!digitalRead(row_D))  store(8);
         else if(!digitalRead(row_E))  store(0);

   } //end if col_2
   
   else if(digitalRead(col_3) && control == 0x03)   //Coluna 3 em nível high? Control igual 3?
   {                                                //Sim...
         control = 0x04;                            //control igual a 4     
         digitalWrite(col_1, HIGH);                 //
         digitalWrite(col_2, HIGH);
         digitalWrite(col_3,  LOW);                 //apenas coluna 3 em nível baixo
         digitalWrite(col_4, HIGH);
         
      // -- Testa qual tecla foi pressionada e armazena o valor --
         if     (!digitalRead(row_A))  store(31);       // Digita na tela do lcd "#"
         else if(!digitalRead(row_B))  store(3);
         else if(!digitalRead(row_C))  store(6);
         else if(!digitalRead(row_D))  store(9);
         else if(!digitalRead(row_E))  store(32);      // Digita na tela do lcd "->"

   } //end if col_3

   else if(digitalRead(col_4) && control == 0x04)   //Coluna 3 em nível high? Control igual 4?
   {                                                //Sim...
         control = 0x01;                            //control igual a 1     
         digitalWrite(col_1, HIGH);                 //
         digitalWrite(col_2, HIGH);
         digitalWrite(col_3, HIGH);                 
         digitalWrite(col_4,  LOW);                 //apenas coluna 4 em nível baixo
         
      // -- Testa qual tecla foi pressionada e armazena o valor --
         if     (!digitalRead(row_A))  store(41);           // Digita na tela do lcd "*"
         else if(!digitalRead(row_B))  store(42);           // Digita na tela do lcd "^"
         else if(!digitalRead(row_C))  store(43);           // Digita na tela do lcd "?"
         else if(!digitalRead(row_D))  store(44);           // Digita na tela do lcd "Esc"
         else if(!digitalRead(row_E))  store(45);           // Digita na tela do lcd "Ent"

   } //end if col_4
  
  
} //end readKeyboard



void store(char value1)                   //Função para armazenar o valor digitado no teclado
{
   char i;                                //variável de iterações
        
   TIMSK2 = 0x00;                         //Desabilita interrupção do Timer2
   
   
  
   delay(350);
  
        
   number_teclado = value1;                       //atualiza number

   numero();

   TIMSK2 = 0x01;   //Habilita interrupção do Timer2
   
} //end store


void numero()                                           //Função para imprimir o número digitado
{
  
                                                                      usalcd;
                                                          lcd.setCursor(0,3);
                                            lcd.print("Voce pressionou:   ");   
                                         
                                         
                                                                                    
                                   if     (number_teclado == 11) lcd.print("F1");
                                   else if(number_teclado == 12) lcd.print("<-");
                                   else if(number_teclado == 21) lcd.print("F2");
                                   else if(number_teclado == 31) lcd.print("#");
                                   else if(number_teclado == 32) lcd.print("->");
                                   else if(number_teclado == 41) lcd.print("*");
                                   else if(number_teclado == 42) lcd.print("ST");
                                   else if(number_teclado == 43) lcd.print("DT");
                                   else if(number_teclado == 44) lcd.print("ESC");
                                   else if(number_teclado == 65) lcd.print("ENT");
                                                  else lcd.print(number_teclado);                //Mostra número  
                                                                      delay(200);
                                                          I2CMux.closeChannel(0);  
   
 
} //end numero


 















