#include <LiquidCrystal.h>
#include <MFRC522.h>
#include <SPI.h>
#include <Ethernet.h>

#define SS_PIN	48      // Pin 48 para el SS (SDA) del RC522
#define RST_PIN	49      // Pin 49 para el reset del RC522

// Configuración de red
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress server(138,197,206,194);  // IP del servidor en Digital Ocean (no DNS) 138.197.206.194
IPAddress ip(192, 168, 0, 25);

// Configuración electrónica
MFRC522 mfrc522(SS_PIN, RST_PIN); //Creamos el objeto para el RC522
const int rs = 26, en = 27, d4 = 22, d5 = 23, d6 = 24, d7 = 25;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Variables del programa
EthernetClient client;
byte ActualUID[4]; //almacenará el código del Tag leído
char nombre[16];

// Configuración Cerradura Inteligente
char idPuerta[] = "Prueba2";              // Este valor indica la puerta en la que está colocado el dispositivo

// Configuración motor
const int motorPin1 = 38; // 28BYJ48 In1
const int motorPin2 = 37; // 28BYJ48 In2
const int motorPin3 = 36; // 28BYJ48 In3
const int motorPin4 = 35; // 28BYJ48 In4

int motorSpeed = 1200;  //variable para fijar la velocidad
int stepCounter = 0;    // contador para los pasos
int stepsPerRev = 1000; // pasos para una vuelta completa

// Secuencia media fase
const int numSteps = 8;
const int stepsLookup[8] = {B1000, B1100, B0100, B0110, B0010, B0011, B0001, B1001};


void setup()
{
    inicializacion();
    conexionAInternet();
}


void loop()
{
    if (mfrc522.PICC_IsNewCardPresent()) // Revisamos si hay nuevas tarjetas  presentes
    {
        if (mfrc522.PICC_ReadCardSerial()) // Seleccionamos una tarjeta
        {
            client.print("GET /api/usuario/checkaccess/puertapublicidentification/");
            client.print(idPuerta);
            client.print("/llavepublicidentification/");
            for (byte i = 0; i < mfrc522.uid.size; i++)
            {
                if(mfrc522.uid.uidByte[i] < 0x10)
                {
                    Serial.print("0");
                    client.print("0");
                }
                Serial.print(mfrc522.uid.uidByte[i], HEX);
                client.print(mfrc522.uid.uidByte[i], HEX);
            }
            Serial.println();
            client.println();
            delay(1000);
            validarAcceso();
            Serial.println();
            client.stop();
            mfrc522.PICC_HaltA(); // Terminamos la lectura de la tarjeta  actual
            verificarEstadoConexion();
        }
    }
}

void inicializacion()
{
    Serial.begin(9600); // Iniciamos la comunicación  serial
	SPI.begin();        // Iniciamos el Bus SPI
    mfrc522.PCD_Init(); // Iniciamos  el MFRC522
    lcd.begin(16, 2);   // Inicializar el LCD
    pinMode(motorPin1, OUTPUT);
    pinMode(motorPin2, OUTPUT);
    pinMode(motorPin3, OUTPUT);
    pinMode(motorPin4, OUTPUT);
    imprimirMensajeLCDMonitorDosLineas("Bienvenido", "Bienvenido", 2, 0, 3, 1, 0);
}

void conexionAInternet()
{
    if (Ethernet.begin(mac) == 0) // inicia la librería Ethernet y la configuración de red
    {
        imprimirMensajeLCDMonitorDosLineas("Fallo Ethernet", "con DHCP", 0, 0, 0, 1, 0);
        Ethernet.begin(mac, ip);
    }
    delay(1000); // Esperar a que Ethernet se inicie
    imprimirMensajeLCDMonitorUnaLinea("Conectando...", 0, 0, 0);
    conexionAlServidor();
}

void conexionAlServidor()
{
    client.connect(server, 8080);
    verificarEstadoConexion();
}

void verificarEstadoConexion()
{
    while(!client.connected())
    {
        imprimirMensajeLCDMonitorUnaLinea("Conexion fallida", 0, 0, 20);
        imprimirMensajeLCDMonitorUnaLinea("Reconectando...", 0, 0, 20);
        client.connect(server, 8080);
    }
    if(client.connected())
    {
        imprimirMensajeLCDMonitorUnaLinea("Conexion exitosa", 0, 0, 20);
        imprimirMensajeLCDMonitorDosLineas("Direccion IP:", Ethernet.localIP(), 0, 0, 0, 1, 20);
    }
}

void validarAcceso()
{
    int cantCaracteresLeidos,cantCaracteresUsuario;
    cantCaracteresLeidos = cantCaracteresUsuario = 0;
    char ant = 'X';
    bool noTieneAcceso = 1;
    while (client.available())
    {
        char act = client.read();
        if (ant == 'O')
        {
            if (act == 'K')
            {
                noTieneAcceso = 0;
            }
        }
        if(noTieneAcceso == 0 && cantCaracteresLeidos>2)
        {
            if(act != '#')
            {
                nombre[cantCaracteresUsuario]=act;
                cantCaracteresUsuario++;
            }
        }
        ant = act;
        cantCaracteresLeidos++;
        Serial.print(act);
    }
    if (noTieneAcceso)
    {
        imprimirMensajeLCDMonitorUnaLinea("Acc. Denegado", 0, 0, 2000);
    }
    else
    {
        imprimirMensajeBienvenida(cantCaracteresUsuario);
        abrirYCerrarCerradura();
    }
}

void mensajeUsuarioDisplay (const char *usuario, bool acceso)
{
    if (acceso)
    {
        imprimirMensajeLCDMonitorDosLineas("Bienvenido:", usuario, 0, 0, 0, 1, 500);
    }
    else
    {
        imprimirMensajeLCDMonitorDosLineas("Acc. Denegado", "Acc. Denegado", 1, 0, 1, 1, 500);
    }
}

void imprimirMensajeLCDMonitorUnaLinea (const char *mensaje, uint8_t posHorizontal, uint8_t posVertical, unsigned long tEspera)
{
    lcd.clear();
    lcd.setCursor(posHorizontal, posVertical);
    lcd.print(mensaje);
    Serial.println(mensaje);
    delay(tEspera);
}

void imprimirMensajeLCDMonitorDosLineas (const char *mensaje1, const char * mensaje2, uint8_t posHorizontal1, uint8_t posVertical1, uint8_t posHorizontal2, uint8_t posVertical2, unsigned long tEspera)
{
    lcd.clear();
    lcd.setCursor(posHorizontal1, posVertical1);
    lcd.print(mensaje1);
    Serial.println(mensaje1);
    lcd.setCursor(posHorizontal2, posVertical2);
    lcd.print(mensaje2);
    Serial.println(mensaje2);
    delay(tEspera);
}

void imprimirMensajeBienvenida(int tope)
{
    int i=0;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Bienvenido:");
    Serial.println("Bienvenido:");
    lcd.setCursor(0,1);
    while(i<tope)
    {
        lcd.print(nombre[i]);
        Serial.print(nombre[i]);
        i++;
    }
    Serial.println();
    delay(1000);
}

void imprimirMensajeLCDMonitorDosLineas (const char *mensaje1, IPAddress mensaje2, uint8_t posHorizontal1, uint8_t posVertical1, uint8_t posHorizontal2, uint8_t posVertical2, unsigned long tEspera)
{
    lcd.clear();
    lcd.setCursor(posHorizontal1, posVertical1);
    lcd.print(mensaje1);
    Serial.println(mensaje1);
    lcd.setCursor(posHorizontal2, posVertical2);
    lcd.print(mensaje2);
    Serial.println(mensaje2);
    delay(tEspera);
}

void abrirYCerrarCerradura()
{
    for (int i = 0; i < stepsPerRev * 2; i++)
    {
        clockwise();
        delayMicroseconds(motorSpeed);
    }
    for (int i = 0; i < stepsPerRev * 2; i++)
    {
        anticlockwise();
        delayMicroseconds(motorSpeed);
    }
    delay(1000);
}

void clockwise()
{
    stepCounter++;
    if (stepCounter >= numSteps)
        stepCounter = 0;
    setOutput(stepCounter);
}

void anticlockwise()
{
    stepCounter--;
    if (stepCounter < 0)
        stepCounter = numSteps - 1;
    setOutput(stepCounter);
}

void setOutput(int step)
{
    digitalWrite(motorPin1, bitRead(stepsLookup[step], 0));
    digitalWrite(motorPin2, bitRead(stepsLookup[step], 1));
    digitalWrite(motorPin3, bitRead(stepsLookup[step], 2));
    digitalWrite(motorPin4, bitRead(stepsLookup[step], 3));
}
