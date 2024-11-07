#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

int tiempoVerdeRojo = 4000;  // 4 segundos
int tiempoRojoVerde = 5000;  // 5 segundos
int tiempoAmbar = 500;       // 500 ms
const int bpulsar = 2;
int boton;

const byte Filas = 1;  // Una fila
const byte Columnas = 4;  // Cuatro columnas
byte Pines_Filas[] = {13};  // Solo un pin para la fila (Pin 13)
byte Pines_Columnas[] = {A0, A1, A2, A3};  // Pines para las columnas (A0, A1, A2, A3)
char Teclas[Filas][Columnas] = {
  {'A', '3', '2', '1'} // Teclas de la primera fila
};

Keypad Teclado1 = Keypad(makeKeymap(Teclas), Pines_Filas, Pines_Columnas, Filas, Columnas);
LiquidCrystal_I2C lcd(0x20, 16, 2);

// Pines de los semáforos
int semaforo1[] = {3, 4, 5};   // Verde, Ámbar, Rojo
int semaforo2[] = {6, 7, 8};   // Verde, Ámbar, Rojo
int semaforo3[] = {9, 10, 11}; // Verde, Ámbar, Rojo

void configurarSemaforo(int semaforo[]) {
  for (int i = 0; i < 3; i++) {
    pinMode(semaforo[i], OUTPUT);
    digitalWrite(semaforo[i], LOW);
  }
}

// Función para encender las luces de un semáforo según la secuencia
void encenderSecuencia(int semaforo[], int pin1, int pin2, int tiempo, String semaforoID) {
  mostrarMensaje("Semaforo " + semaforoID);
  digitalWrite(semaforo[pin1], HIGH);
  digitalWrite(semaforo[pin2], HIGH);
  delay(tiempo);
  digitalWrite(semaforo[pin1], LOW);

  unsigned long tiempoInicio = millis();
  while (millis() - tiempoInicio < tiempo) {
    boton = digitalRead(bpulsar);
    if (boton == 1) {
      apagarLcd();
    } else {
      encenderLcd();
      mostrarMensaje("Semaforo " + semaforoID);
    }
  }
}

// Mostrar mensaje en el LCD
void mostrarMensaje(String mensaje) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(mensaje);
}

// Apagar todas las luces
void apagarLuces() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(semaforo1[i], LOW);
    digitalWrite(semaforo2[i], LOW);
    digitalWrite(semaforo3[i], LOW);
  }
  mostrarMensaje("Luces apagadas");
  delay(2500);
}

void setup() {
  encenderLcd();
  Serial.begin(9600);

  configurarSemaforo(semaforo1);
  configurarSemaforo(semaforo2);
  configurarSemaforo(semaforo3);
}

void apagarLcd() {
  lcd.clear();
  lcd.noBacklight();
}

void encenderLcd() {
  lcd.init();
  lcd.backlight();
}

void loop() {
  char tecla = Teclado1.getKey();
  mostrarMensaje("Elija una opcion");
  while (true){
    tecla = Teclado1.getKey();
    if (tecla){
      Serial.println(tecla);
      break;
    }
  }

  switch (tecla) {
    case '1':
      encenderSecuencia(semaforo1, 0, 2, tiempoVerdeRojo, "1");
      encenderSecuencia(semaforo1, 1, 2, tiempoAmbar, "1");
      break;
    case '2':
      encenderSecuencia(semaforo2, 2, 0, tiempoRojoVerde, "2");
      encenderSecuencia(semaforo2, 1, 0, tiempoAmbar, "2");
      break;
    case '3':
      encenderSecuencia(semaforo3, 1, 2, tiempoAmbar, "3");
      break;
    case 'A':
      encenderSecuencia(semaforo1, 0, 2, tiempoVerdeRojo, "1");
      encenderSecuencia(semaforo1, 1, 2, tiempoAmbar, "1");
      mostrarMensaje("Semaforo 2");
      encenderSecuencia(semaforo2, 2, 0, tiempoRojoVerde, "2");
      encenderSecuencia(semaforo2, 1, 0, tiempoAmbar, "2");
      mostrarMensaje("Semaforo 3");
      encenderSecuencia(semaforo3, 1, 2, tiempoAmbar, "3");
      break;
    }
    apagarLuces();
  
}
