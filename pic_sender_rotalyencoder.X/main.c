#include "mcc_generated_files/mcc.h"

static void UART1_WriteSignedInt32(int32_t value)
{
    char buffer[12];
    uint32_t magnitude;
    uint8_t index = 0;

    if (value < 0)
    {
        UART1_Write('-');
        magnitude = (uint32_t)(-(value + 1)) + 1U;
    }
    else
    {
        magnitude = (uint32_t)value;
    }

    do
    {
        buffer[index++] = (char)('0' + (magnitude % 10U));
        magnitude /= 10U;
    } while (magnitude > 0U);

    while (index > 0)
    {
        UART1_Write((uint8_t)buffer[--index]);
    }
}

static uint8_t Encoder_ReadState(void)
{
    return (uint8_t)((ENCODER_A_GetValue() << 1) | ENCODER_B_GetValue());
}

static int8_t Encoder_GetStep(uint8_t previousState, uint8_t currentState)
{
    static const int8_t transitionTable[16] = {
        0,  1, -1,  0,
       -1,  0,  0,  1,
        1,  0,  0, -1,
        0, -1,  1,  0
    };

    return transitionTable[(uint8_t)((previousState << 2) | currentState)];
}

static void Encoder_SendValue(int32_t value)
{
    UART1_WriteString("ENC,");
    UART1_WriteSignedInt32(value);
    UART1_WriteString("\r\n");
}

/*
                         Main application
 */
void main(void)
{
    int32_t encoderValue = 0;
    uint8_t previousEncoderState;

    // Initialize the device
    SYSTEM_Initialize();

    // If using interrupts in PIC18 High/Low Priority Mode you need to enable the Global High and Low Interrupts
    // If using interrupts in PIC Mid-Range Compatibility Mode you need to enable the Global Interrupts
    // Use the following macros to:

    // Enable the Global Interrupts
    //INTERRUPT_GlobalInterruptEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();

    previousEncoderState = Encoder_ReadState();
    Encoder_SendValue(encoderValue);

    while (1)
    {
        uint8_t currentEncoderState = Encoder_ReadState();
        int8_t encoderStep = Encoder_GetStep(previousEncoderState, currentEncoderState);

        if (encoderStep != 0)
        {
            encoderValue += encoderStep;
            previousEncoderState = currentEncoderState;
            Encoder_SendValue(encoderValue);
        }
        else if (currentEncoderState != previousEncoderState)
        {
            previousEncoderState = currentEncoderState;
        }

        __delay_ms(1);
    }
}
/**
 End of File
*/
