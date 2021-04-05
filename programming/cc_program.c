
#define F_CPU 1000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>

#define beep_delay 100


void setup(void) {
  cli();
  // Disable Watchdog
  MCUSR &= ~(1<<WDRF);
  WDTCR |= (1<<WDCE) | (1<<WDE);
  WDTCR = 0;

  // Set Pin Modes
  DDRB |= (1<<PB0)|(1<<PB1)|(1<<PB3);   // Output Mode

  // Power Save Options
  PRR = 0b00000000;  // Power Save Options
  ACSR = (1<<ACD);   // Disable Analog Comparator
  
  CLKPR = (1<<CLKPCE);
  CLKPR = (0<<CLKPS2)|(1<<CLKPS1)|(1<<CLKPS0);
  //CLKPR = (1<<CLKPS2);
  
  sei();  
  //set_sleep_mode(SLEEP_MODE_IDLE);
}

void beep(void) {
  int i;
  for(i=0;i<4;i++) {
    PINB |= 1<<PB0|1<<PB1;
    _delay_ms(beep_delay);
  }
  for(i=0;i<4;i++) {
    PINB |= 1<<PB0;
    _delay_ms(beep_delay);
  }
  PORTB |= 1<<PB0|1<<PB1;
  _delay_ms(beep_delay*4);
  PORTB &= ~(1<<PB0|1<<PB1);
  _delay_ms(beep_delay*4); 
}

void init_pwm(void) {
  TCCR0A = (1<<COM0A1)|(1<<COM0B1)|(1<<WGM01)|(1<<WGM00);
  TCCR0B = 0<<CS02|1<<CS01|0<<CS00;
  OCR0A = 80;
  OCR0B = 80;
}

void init_adc(void) {
  ADCSRA |= 1<<ADEN|1<<ADIE|1<<ADPS2|1<<ADPS1|1<<ADPS0;

  //Enable ADC Freerun
  ADCSRA |= 1<<ADATE;
  
  ADMUX = 0<<REFS2|0<<REFS1|0<<ADLAR|1<<MUX0;
  ADCSRA |= (1<<ADSC);
}


ISR(ADC_vect) {
    if ( ADCH < 1 ) {
      OCR0B = 160;
    } else {
      OCR0B = 10;
    }    
}



int main(void) {
  setup();
  beep();
  init_pwm();
  init_adc();
 
  OCR0A=0;
  while (1) {
    while(++OCR0A) { _delay_ms(10); }
    while(--OCR0A) { _delay_ms(10); }
  }
}

