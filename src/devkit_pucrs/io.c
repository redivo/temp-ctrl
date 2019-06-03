/**************************************************************************************************/
/**
 * \brief  In and Output interfaces
 */
/**************************************************************************************************/

#include "io.h"
#include "timer.h"
#include "devkit_pucrs/lpc23xx.h"

/**************************************************************************************************/

#define FORM_FEED 0x0C

#define LCD_E	8
#define LCD_RS	4

#define LCD_MAX_COLUMNS 16
#define LCD_MAX_LINES 2

#define LED_MAX 8
#define LED0_BIT 0
#define LED1_BIT 1
#define LED2_BIT 2
#define LED3_BIT 3
#define LED4_BIT 4
#define LED5_BIT 5
#define LED6_BIT 6
#define LED7_BIT 7

#define LED_COLLER LED2_BIT
#define LED_HOT_HYSTERESIS LED3_BIT

#define LED_COLD_HYSTERESIS LED6_BIT
#define LED_HEATER LED5_BIT

#define LED_IDLE LED4_BIT

#define COLLER_PIN_RELAY 14
#define HEATER_PIN_RELAY 12

#define SW_STEP_DOWN_BIT 4
#define SW_STEP_UP_BIT 8

/**************************************************************************************************/

/**
 * \brief  Send command to LCD display
 * \param  cmd  Command to be sent
 */
static void lcd_command(uint8_t cmd)
{
    FIO3PIN0 = cmd;
    FIO2CLR = LCD_RS;	/* RS=0: comando */
    FIO2SET = LCD_E;
    FIO2CLR = LCD_E;
    ms_sleep(20);
}

/**************************************************************************************************/

/**
 * \brief  Send a character to LCD display
 * \param  c  Character to be sent
 */
static void lcd_char(char c)
{
    FIO3PIN0 = c;
    FIO2SET = LCD_RS;	/* RS=1: caractere */
    FIO2SET = LCD_E;
    FIO2CLR = LCD_E;
    ms_sleep(8);
}

/**************************************************************************************************/

/**
 * \brief  Initialize LCD display
 */
static void lcdInit()
{
    FIO3DIR |= 0xff;	/* Dados do LCD como saidas */
    FIO2DIR  |= LCD_E | LCD_RS;	/* EN e RS do LCD como saidas */
    FIO2CLR = LCD_E;
    ms_sleep(20);
    lcd_command(0x38);	/* Configura LCD para 2 linhas */
    lcd_command(1);		/* Limpa display */
    lcd_command(0x0c);	/* Apaga cursor */
}

/**************************************************************************************************/

/**
 * \brief  Initialize LEDinterface
 */
void ledInit()
{
    /* Set led ports to output */
    //FIO4DIR |= 1 << LED0_BIT;
    FIO3DIR |= 1 << 26; // Led 0 is linked to 3.26 too
    FIO4DIR |= 1 << LED1_BIT;
    FIO4DIR |= 1 << LED2_BIT;
    FIO4DIR |= 1 << LED3_BIT;
    FIO4DIR |= 1 << LED4_BIT;
    FIO4DIR |= 1 << LED5_BIT;
    FIO4DIR |= 1 << LED6_BIT;
    FIO4DIR |= 1 << LED7_BIT;

    /* Initially all leds must be turned on to verify they're right */
    FIO3PIN &= ~(1 << 26);
    FIO4PIN &= ~(1 << LED1_BIT);
    FIO4PIN &= ~(1 << LED2_BIT);
    FIO4PIN &= ~(1 << LED3_BIT);
    FIO4PIN &= ~(1 << LED4_BIT);
    FIO4PIN &= ~(1 << LED5_BIT);
    FIO4PIN &= ~(1 << LED6_BIT);
    FIO4PIN &= ~(1 << LED7_BIT);

    /* After 1 second, turn them off */
    ms_sleep(1000);
    FIO3PIN |= 1 << 26;
    FIO4PIN |= (1 << 1)
             + (1 << LED2_BIT)
             + (1 << LED3_BIT)
             + (1 << LED4_BIT)
             + (1 << LED5_BIT)
             + (1 << LED6_BIT)
             + (1 << LED7_BIT);
}

/**************************************************************************************************/

void swInit()
{
    FIO2DIR &= ~((1 << SW_STEP_DOWN_BIT) | (1 << SW_STEP_UP_BIT));
}

/**************************************************************************************************/

void pinInit()
{
    FIO4DIR |= 1 << COLLER_PIN_RELAY;
    FIO4DIR |= 1 << HEATER_PIN_RELAY;
}

/**************************************************************************************************/

void ioInit()
{
    lcdInit();
    ledInit();
    swInit();
    pinInit();
}

/**************************************************************************************************/

error_t print(char *txt)
{
    static uint8_t column = 0;
    static uint8_t line = 0;

    /* Iterate over received text and print it on LCD display */
    while (*txt) {
        switch (*txt) {
            /* Handle new line */
            case '\n':
                /* Check boundaries */
                if ((line + 1) >= LCD_MAX_LINES) {
                    return ERR_MAX_SIZE_EXCEEDED;
                }

                if (line == 0) {
                    lcd_command(0xC0 + column);
                }

                line++;
                break;

            /* Handle return */
            case '\r':
                if (line == 0) {
                    lcd_command(0x80);
                } else {
                    lcd_command(0xC0);
                }

                column = 0;
                break;

            /* FormFeed return to the begin of the LCD display */
            case '\f':
                lcd_command(0x80);
                column = 0;
                line = 0;
                break;

            default:
                /* Check boundaries */
                if (column >= LCD_MAX_COLUMNS) {
                    return ERR_MAX_SIZE_EXCEEDED;
                }

                lcd_char(*txt);
                column++;
        }

        txt++;
    }

    return ERR_NONE;
}

/**************************************************************************************************/

void clearScreen()
{
    // Clear screen and go back to the begin of the scree
    lcd_command(0x01);
    print("\f");
}

/**************************************************************************************************/

userInput_t userInput()
{
    // Step down pressed
    if ((FIO2PIN & (1 << SW_STEP_DOWN_BIT)) == 0) {
        return USER_INPUT_TARGET_TEMP_STEP_DOWN;
    }

    // Step up pressed
    else if ((FIO2PIN & (1 << SW_STEP_UP_BIT)) == 0) {
        return USER_INPUT_TARGET_TEMP_STEP_UP;
    }

    // Nothing pressed
    return USER_INPUT_NONE;
}

/**************************************************************************************************/

void setCoolerState(coolerState_t state)
{
    switch (state) {
        case COOLER_ON:
            FIO4PIN |= (1 << COLLER_PIN_RELAY);
            coolerStateWarning(true);
            break;

        case COOLER_OFF:
            FIO4PIN &= ~(1 << COLLER_PIN_RELAY);
            coolerStateWarning(false);
            break;
    }
}

/**************************************************************************************************/

void setHeaterState(heaterState_t state)
{
    switch (state) {
        case HEATER_ON:
            FIO4PIN |= (1 << HEATER_PIN_RELAY);
            heaterStateWarning(true);
            break;

        case HEATER_OFF:
            FIO4PIN &= ~(1 << HEATER_PIN_RELAY);
            heaterStateWarning(false);
            break;
    }
}

/**************************************************************************************************/

void hysteresisTimeHotWarning(bool display)
{
    ms_sleep(100);
    if (display) {
        FIO4PIN &= ~(1 << LED_HOT_HYSTERESIS);
    }
    else {
        FIO4PIN |= (1 << LED_HOT_HYSTERESIS);
    }
}

/**************************************************************************************************/

void hysteresisTimeColdWarning(bool display)
{
    ms_sleep(100);
    if (display) {
        FIO4PIN &= ~(1 << LED_COLD_HYSTERESIS);
    }
    else {
        FIO4PIN |= (1 << LED_COLD_HYSTERESIS);
    }
}

/**************************************************************************************************/

void coolerStateWarning(bool isOn)
{
    if (isOn) {
        ms_sleep(100);
        FIO4PIN |= (1 << LED_IDLE);
        ms_sleep(100);
        FIO4PIN &= ~(1 << LED_COLLER);
    }
    else {
        ms_sleep(100);
        FIO4PIN |= (1 << LED_COLLER);
        ms_sleep(100);
        FIO4PIN &= ~(1 << LED_IDLE);
    }
}

/**************************************************************************************************/

void heaterStateWarning(bool isOn)
{
    if (isOn) {
        ms_sleep(100);
        FIO4PIN |= (1 << LED_IDLE);
        ms_sleep(100);
        FIO4PIN &= ~(1 << LED_HEATER);
    }
    else {
        ms_sleep(100);
        FIO4PIN |= (1 << LED_HEATER);
        ms_sleep(100);
        FIO4PIN &= ~(1 << LED_IDLE);
    }
}

/**************************************************************************************************/

void debug(char *msg)
{
    // TODO
}

/**************************************************************************************************/
