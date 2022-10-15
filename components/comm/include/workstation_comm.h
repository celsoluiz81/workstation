#include <stdio.h>

typedef enum{
    UART_TEST_MESSAGE,
    UART_CLAMP_STATE_MESSAGE,
    UART_STATE_SEND_MSG
} workstation_uart_state_t;

void WorkstationUartSetup(void);
void WorkstationUartInit(void);
void WorkstationUartMain(void);
