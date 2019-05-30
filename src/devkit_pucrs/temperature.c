/**************************************************************************************************/
/**
 * \brief  Temperature-related functions
 */
/**************************************************************************************************/

#include "lpc23xx.h"
#include <string.h>
#include "temperature.h"
#include "io.h"
#include "timer.h"
#include "sts_cfg.h"

/**************************************************************************************************/

#define I2C_CONSET_EN_BIT 6
#define I2C_CONSET_STA_BIT 5
#define I2C_CONSET_STO_BIT 4
#define I2C_CONSET_SI_BIT 3
#define I2C_CONSET_AA_BIT 2

#define LM75_ONBOARD_ADDR 0x48
#define LM75_OFFBORD_ADDR 0x4F

#define LM75_ADDR LM75_OFFBORD_ADDR
//#define LM75_ADDR LM75_ONBOARD_ADDR

#define LM75_TEMP_REG 0x00

#define PCONP_I2C0 (1 << 7)
#define PINSEL_I2C0_SDA PINSEL1
#define PINSEL_I2C0_SCL PINSEL1

#define I2C_CON_START 0x20
#define I2C_CON_STOP 0x10
#define I2C_CON_INTERRUPT 0x08
#define I2C_CON_ACK 0x04
#define I2C_CON_NACK 0x04

#define I2C_STAT_START_BIT_SENT 0x08

#define I2C_STAT_WRITE_ACK_RCVD 0x18
#define I2C_STAT_WRITE_DATA_ACK_RCVD 0x28
#define I2C_STAT_WRITE_ACK_NOT_RCVD_AFTER_SLV_ADDR 0x20
#define I2C_STAT_WRITE_ACK_NOT_RCVD_AFTER_BYTE 0x30

#define I2C_STAT_READ_ACK_RCVD 0x40
#define I2C_STAT_DATA_RCVD 0x50
#define I2C_STAT_READ_NOT_ACK_RCVD_AFTER_SLV_ADDR 0x48
#define I2C_STAT_READ_NACK_RCVD 0x58

#define I2C_READ 1
#define I2C_WRITE 0

#define WRITE_I2C_ADDR(_addr) (_addr << 1)
#define READ_I2C_ADDR(_addr) ((_addr << 1) + 1)

#define TEMPERATURE_DEBOUNCE 2

/**************************************************************************************************/

typedef union {
    struct {
        unsigned char reserved1:2;	//!< Reserved
        unsigned char assert_ack:1;	//!< Assert ack flag.
        unsigned char interrupt:1;	//!< I²C interrupt flag.
        unsigned char stop:1;		//!< STOP flag.
        unsigned char start:1;		//!< START flag.
        unsigned char enable:1;		//!< I²C interface enable.
        unsigned char reserved2:1;	//!< Reserved
    } bits;
    unsigned char reg;
} i2c_control_t;

/**************************************************************************************************/

typedef enum {
    IDLE,
    RUNNING,
} i2c_state_t;

/**
 * \brief  Run I2C operation in master mode
 * \param  operation  Operation to be done
 * \param  devAddr    Slave device address
 * \param  data       Pointer to store the data, when it's a read operation and to data be read,
                      when it's a write operation
 * \param  dataSize   Size allocated to data
 */
static i2c_state_t i2c_operation_fsm(uint8_t operation, uint8_t devAddr, uint8_t *data,
                                     uint8_t dataSize)
{
    static i2c_state_t state = IDLE;
    static uint8_t iterations;

    /* If IDLE, so start I2C */
    if (state == IDLE) {
        iterations = 0;
        I20CONSET = I2C_CON_START;
        state = RUNNING;
        return state;
    }

    // Verify if is something new
    if((I20CONSET & I2C_CON_INTERRUPT) == 0) {
        return state;
    }

    switch (I20STAT) {
        case I2C_STAT_START_BIT_SENT:
            if (operation == I2C_READ) {
                I20DAT = READ_I2C_ADDR(devAddr); // Inform what device i'm communicating
            }
            else {
                I20DAT = WRITE_I2C_ADDR(devAddr); // Inform what device i'm communicating
            }

            I20CONCLR = I2C_CON_START;
            I20CONCLR = I2C_CON_INTERRUPT;
            break;

        case I2C_STAT_WRITE_ACK_RCVD:
            I20DAT = data[iterations];
            iterations++;

            I20CONCLR = I2C_CON_INTERRUPT;
            break;

        case I2C_STAT_WRITE_DATA_ACK_RCVD:
            /* If the iterations dont's reach dataSize, continue sending data.
             * Otherwise end the connection.
             */
            if (iterations < dataSize) {
                I20DAT = data[iterations];
                iterations++;
                I20CONCLR = I2C_CON_INTERRUPT;
            }
            else {
                I20CONSET = I2C_CON_STOP;
                I20CONCLR = I2C_CON_INTERRUPT;
                state = IDLE;
                break;
            }

            break;

        case I2C_STAT_READ_ACK_RCVD:
            I20CONSET = I2C_CON_ACK;
            I20CONCLR = I2C_CON_INTERRUPT;
            break;

        case I2C_STAT_DATA_RCVD:
            data[iterations] = I20DAT;
            iterations++;

            /* If the iterations reach dataSize end connection */
            if (iterations >= dataSize) {
                I20CONCLR = I2C_CON_ACK;
            }

            I20CONCLR = I2C_CON_INTERRUPT;

            break;

        case I2C_STAT_READ_NACK_RCVD:
            I20CONSET = I2C_CON_STOP;
            I20CONCLR = I2C_CON_INTERRUPT;
            state = IDLE;
            break;

        case I2C_STAT_WRITE_ACK_NOT_RCVD_AFTER_SLV_ADDR:
        case I2C_STAT_WRITE_ACK_NOT_RCVD_AFTER_BYTE:
        case I2C_STAT_READ_NOT_ACK_RCVD_AFTER_SLV_ADDR:
        default:
            I20CONSET = I2C_CON_STOP;
            I20CONCLR = I2C_CON_INTERRUPT;
            state = IDLE;
            break;
    }

    return state;
}

/**************************************************************************************************/

typedef enum {
    INITIALIZING,
    INFORMING_REGISTER_ADDR,
    REGISTER_ADDR_INFORMED,
    READING_TEMP,
} reading_phase_t;

float readTemperature()
{
    static reading_phase_t phase = INITIALIZING;
    uint8_t reg = LM75_TEMP_REG;
    static uint8_t temp[2];

    switch (phase) {
        case INITIALIZING:
        case INFORMING_REGISTER_ADDR:
            if (i2c_operation_fsm(I2C_WRITE, LM75_ADDR, &reg, 1) == IDLE) {
                phase = REGISTER_ADDR_INFORMED;
            } else {
                phase = INFORMING_REGISTER_ADDR;
            }
            break;

        case REGISTER_ADDR_INFORMED:
            memset(temp, 0, 2);
            phase = READING_TEMP;
            break;

        case READING_TEMP:
            if (i2c_operation_fsm(I2C_READ, LM75_ADDR, temp, 2) == IDLE) {
                ms_sleep(100);
                float temperature = two_complement_to_float(temp[0], temp[1], 3);
                phase = INFORMING_REGISTER_ADDR;
                return temperature;
            }
            break;

        default:
            break;
    }

    return Status.realTemp;
}

/**************************************************************************************************/

void tempInit()
{
    i2c_control_t i2c_control;

    /******************************/
    /* Initialize I2C 0 as master */
    /******************************/
    memset(&i2c_control, 0, sizeof(i2c_control));

    /* Turn on power */
    PCONP |= PCONP_I2C0;

    /* Selec SCL and SDA mode to respective pins */
    PINSEL_I2C0_SDA |= (1 << 22);
    PINSEL_I2C0_SCL |= (1 << 24);

    /* Configure I²C interface */
    i2c_control.bits.enable = 1;
    I20CONCLR = ~i2c_control.reg;
    I20CONSET = i2c_control.reg;

    I20SCLH = 100;
    I20SCLL = 100;

    ms_sleep(500);
}

/**************************************************************************************************/
