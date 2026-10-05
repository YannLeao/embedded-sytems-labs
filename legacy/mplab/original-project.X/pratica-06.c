#define F_CPU 16000000UL

#include <avr/io.h>
#include <Arduino.h>

int main(void)
{
  init();
  
  DDRL = 0xFF;
  PORTL = 0x00;

  while (1) {
    int potenciometro = analogRead(A0);

    int count = potenciometro / 4; // Reduce 10 to 8 bits
    PORTL = count;

    delay(200);
  }
}

