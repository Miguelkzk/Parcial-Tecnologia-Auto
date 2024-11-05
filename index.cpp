#include <Wire.h>
#include <LiquidCrystal_I2C.h>

int tiempoVerdeRojo = 4000;  // 4 segundos
int tiempoRojoVerde = 5000;  // 5 segundos
int tiempoAmbar = 500;       // 500 ms
LiquidCrystal_I2C lcd(0x20, 16, 2);

// Pines de los semáforos
int semaforo1[] = {3, 4, 5};   // Verde, Ámbar, Rojo
int semaforo2[] = {6, 7, 8};   // Verde, Ámbar, Rojo
int semaforo3[] = {9, 10, 11}; // Verde, Ámbar, Rojo

int switchPin = 2;    // Pin del switch
int valorSwitch = 0;  // Valor del switch

// Configura los pines como salida y apaga al inicio
void configurarSemaforo(int semaforo[]) {
  for (int i = 0; i < 3; i++) {
    pinMode(semaforo[i], OUTPUT);
    digitalWrite(semaforo[i], LOW);
  }
}

// Función para encender las luces de un semáforo según la secuencia
void encenderSecuencia(int semaforo[], int pin1, int pin2, int tiempo) {
  digitalWrite(semaforo[pin1], HIGH);  // Enciende el primer pin
  digitalWrite(semaforo[pin2], HIGH);  // Enciende el segundo pin
  unsigned long tiempoInicio = millis(); // Captura el tiempo de inicio

  // Espera el tiempo de la secuencia sin bloquear
  while (millis() - tiempoInicio < tiempo) {
    valorSwitch = digitalRead(switchPin);  // Lee el valor del switch
    if (valorSwitch == 0) {  // Si el switch está presionado, apaga las luces
      apagarLuces();
      return;  // Sale de la función y detiene la secuencia
    }
  }

  digitalWrite(semaforo[pin1], LOW);  // Apaga el primer pin
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
  delay(2500);  // Espera un poco para mostrar el mensaje
}

void setup() {
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);  // Inicia el monitor serie

  pinMode(switchPin, INPUT);  // Configura el pin del switch como entrada

  configurarSemaforo(semaforo1);
  configurarSemaforo(semaforo2);
  configurarSemaforo(semaforo3);
}

void loop() {
  valorSwitch = digitalRead(switchPin);  // Lee el valor del switch

  if (valorSwitch == 1) {  // Si el switch está activado
    mostrarMensaje("Semaforo 1");
    encenderSecuencia(semaforo1, 0, 2, tiempoVerdeRojo);  // Enciende el verde y el rojo
    encenderSecuencia(semaforo1, 1, 2, tiempoAmbar);     // Enciende el ámbar

    mostrarMensaje("Semaforo 2");
    encenderSecuencia(semaforo2, 2, 0, tiempoRojoVerde); // Enciende el rojo y el verde
    encenderSecuencia(semaforo2, 1, 0, tiempoAmbar);     // Enciende el ámbar

    mostrarMensaje("Semaforo 3");
    encenderSecuencia(semaforo3, 1, 2, tiempoAmbar);     // Solo el ámbar
  } else {  // Si el switch está desactivado, apaga todas las luces inmediatamente
    apagarLuces();
  }
}
