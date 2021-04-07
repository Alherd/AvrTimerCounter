#include <stdlib.h> 
#include <string.h> 
#include "FreeRTOS.h" 
#include "task.h" 
#include <stdbool.h> 
#include<avr/io.h> 
#include<avr/interrupt.h> 
 
#define S1 PC5 
#define S2 PC4 
 
void changeTask(); 
void displayTask(); 
void pollButtonsTask(); 
void runTaskForPeriod(int buttonNumber, int period, int display_interval); 
 
char m1b[] = { 
 0b00100100, 
 0b00001010, 
 0b00010100,
 0b00000011 
}; 
 
char m2b[] = { 
 0b00000011, 
 0b00010100, 
 0b00001010, 
 0b00100100 
}; 
 
xTaskHandle displayTaskHandle = NULL; 
 
int currentSymbolIndex = 0; 
int pB = 0; 
int state = 0; 
 
int display_interval = 100; 
static volatile unsigned long msCounter = 0; 
unsigned long display_last = 0; 
 
char pushedButton = 0; 
 
ISR(TIMER0_COMPA_vect) 
{ 
  msCounter++; 
} 
 
void setupTimer0() 
{ 
    TCCR0A = 0; 
    TCCR0B = 0;  
 
    OCR0B = 113; 
    TCCR0B |= (1 << WGM02);
 
    TCCR0B |= (1 << CS00)|(1 << CS01); 
    TIMSK0 |= (1 << OCIE0A);
    sei();  
} 
 
int main() { 
    setupTimer0(); 
     
    DDRB = 0b11111111; 
 
    xTaskCreate(changeTask, NULL, configMINIMAL_STACK_SIZE, NULL, 3, NULL); 
    xTaskCreate(pollButtonsTask, NULL, configMINIMAL_STACK_SIZE, NULL, 4, NULL); 
    xTaskCreate(displayTask, NULL, configMINIMAL_STACK_SIZE, NULL, 4, & displayTaskHandle); 
 
    vTaskStartScheduler(); 
 
    return 0; 
} 
 
void displayTask() { 
  portTickType xLastWakeTime; 
  xLastWakeTime = xTaskGetTickCount(); 
 
    while (true) { 
        PORTB = pB; 
        vTaskDelayUntil( &xLastWakeTime, (1 / portTICK_RATE_MS ) ); 
    } 
} 
 
void changeTask() { 
  portTickType xLastWakeTime; 
  xLastWakeTime = xTaskGetTickCount(); 
    while (true) { 
if (pushedButton == 1) { 
            if (currentSymbolIndex == 4) currentSymbolIndex = 0; 
            pB = m1b[currentSymbolIndex]; 
            currentSymbolIndex++; 
        } else if (pushedButton == 2) { 
            if (currentSymbolIndex == 4) currentSymbolIndex = 0; 
            pB = m2b[currentSymbolIndex]; 
            currentSymbolIndex++; 
        } 
        vTaskDelayUntil( &xLastWakeTime, (display_interval / portTICK_RATE_MS ) ); 
    } 
} 
 
void pollButtonsTask() { 
    while (true) { 
        if ((PINC & (1 << S1)) == 0) 
        { 
            runTaskForPeriod(1, 3000, 200); 
        }  
        else if((PINC & (1 << S2)) == 0) 
  { 
            runTaskForPeriod(2, 6000, 800); 
  }  
        else { 
            PORTB = 0b00000000; 
 
            pushedButton = 0; 
            currentSymbolIndex = 0; 
            display_interval = 1000; 
 
            vTaskSuspend(displayTaskHandle); 
   state = 0; 
            display_last = msCounter; 
        } 
    } 
} 
 
void runTaskForPeriod(int buttonNumber, int period, int display_interval_button){ 
    if (state == 1 && (msCounter - display_last >= period)) { 
                display_last = msCounter; 
                pushedButton = buttonNumber; 
                display_interval = display_interval_button; 
                vTaskSuspend(displayTaskHandle); 
            } else if (state == 0 && (msCounter - display_last >= 0)) { 
                state = 1; 
                display_last = msCounter; 
                pushedButton = buttonNumber; 
                display_interval = display_interval_button; 
                vTaskResume(displayTaskHandle); 
            } 
}
