#include "switch.h"

uint16 switch_debounce_OnTimer;
uint16 switch_debounce_OffTimer;
uint16 switch_timer;
uint8 switch_status;
uint8 switch_counter;
void Switch_Read(void)
{
    static uint8 switch_statusOld;
    uint8 stSwitchloc;
    stSwitchloc = HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_13);
    if(1u == stSwitchloc)
    {
        switch_debounce_OnTimer++;
        if(switch_debounce_OnTimer>=50)
        {
            switch_debounce_OnTimer = 0;
            switch_status = 1;
        }
    }
    else
    {
        switch_debounce_OffTimer++;
        if(switch_debounce_OffTimer>=50)
        {
            switch_debounce_OffTimer = 0;
            switch_status = 0;
        }
    }

    if((0 == switch_status)&&(1u == switch_statusOld))
    {
        switch_counter++;
        if(switch_counter>5)
        {
            switch_counter = 1;
        }
    }
    switch_statusOld = switch_status;

    if(1u == switch_status)
    {
        switch_timer++;
        if(switch_timer>=3000)
        {
            switch_timer = 3000;
            switch_counter = 0;
        }
    }
    else
    {
        switch_timer = 0;
    }
}

void PWM_Output(void)
{
    switch(switch_counter)
    {
        case 1:
            TIM3->CCR1 = PWM_DUTY_20;
        break;
        case 2:
            TIM3->CCR1 = PWM_DUTY_40;
        break;
        case 3:
            TIM3->CCR1 = PWM_DUTY_60;
        break;
        case 4:
            TIM3->CCR1 = PWM_DUTY_80;
        break; 
        case 5:
            TIM3->CCR1 = PWM_DUTY_100;
        break;
        default :
            TIM3->CCR1 = PWM_DUTY_0;
        break;
    }
}