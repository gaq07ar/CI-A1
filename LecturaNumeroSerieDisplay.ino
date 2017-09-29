#include <LiquidCrystal.h>
#include <MFRC522.h>
#include <SPI.h>


#define RST_PIN	3    //Pin 3 para el reset del RC522
#define SS_PIN	10   //Pin 10 para el SS (SDA) del RC522



MFRC522 mfrc522(SS_PIN, RST_PIN); //Creamos el objeto para el RC522
const int rs = 8, en = 9, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


void setup()
{
	Serial.begin(9600); // Iniciamos la comunicación  serial
	SPI.begin();        // Iniciamos el Bus SPI
    mfrc522.PCD_Init(); // Iniciamos  el MFRC522
    lcd.begin(16, 2);   // Inicializar el LCD
    Serial.println("Inicio de lectura");  // Vuelca el texto al monitor serial
    lcd.setCursor(2,0);
    lcd.print("Bienvenido");       // Muestra el texto en el display
    lcd.setCursor(3,1);
    lcd.print("Bienvenido");
    delay(1500);
    lcd.clear();
}

byte ActualUID[4]; //almacenará el código del Tag leído
const int cantUsuarios = 2;
byte baseUsuarios[cantUsuarios][5] =
{
    {0x25, 0xB9, 0x9A, 0x2C, 01}, // Código usuario 1
    {0xC3, 0x0F, 0x62, 0x00, 10}  // Código usuario 2
};

void loop()
{
    if ( mfrc522.PICC_IsNewCardPresent()) // Revisamos si hay nuevas tarjetas  presentes
    {  
        if ( mfrc522.PICC_ReadCardSerial()) // Seleccionamos una tarjeta
        {
            for (byte i = 0; i < mfrc522.uid.size; i++)
            {
                Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
                Serial.print(mfrc522.uid.uidByte[i], HEX);   
                ActualUID[i] = mfrc522.uid.uidByte[i];
            }
            busquedaUsuario(ActualUID);
            Serial.println();
            mfrc522.PICC_HaltA(); // Terminamos la lectura de la tarjeta  actual         
        }      
	}	
}


void busquedaUsuario(byte arrayEnt[]) // Procedimiento para comprobar si existe el usuario
{
    bool usAcc = false;
    byte numAcc;
    for(int i = 0; i < cantUsuarios; i++)
    { 
        if(arrayEnt[0] == baseUsuarios[i][0])
        {
            if(arrayEnt[1] == baseUsuarios[i][1])
            {
                if(arrayEnt[2] == baseUsuarios[i][2])
                {
                    if(arrayEnt[3] == baseUsuarios[i][3])
                    {
                        usAcc = true;
                        numAcc = baseUsuarios[i][4];
                    }
                }
            }
        }
    }  
    if(usAcc)
    {
        identificarUsuario(numAcc);
    }
    else
    {
        mensajeUsuarioDisplay("Acc. Denegado.", usAcc);
    }
}

void identificarUsuario(byte entrada)
{
    switch(entrada)
    {
        case 01:
            mensajeUsuarioDisplay("Gabriel. Q", true);
            break;
        case 10:
            mensajeUsuarioDisplay("Mariano. D.", true);
            break;
        default:
            break;
    }    
}

void mensajeUsuarioDisplay (char usuario[], bool acceso)
{
    if (acceso)
    {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Bienvenido:");
        lcd.setCursor(0,1);
        lcd.print(usuario);
        delay(1000);
        lcd.clear();
    }
    else
    {
        lcd.clear();
        lcd.setCursor(1,0);
        lcd.print("Acc. Denegado");
        lcd.setCursor(1,1);
        lcd.print("Acc. Denegado");
        delay(800);
        lcd.clear();
    }
}
