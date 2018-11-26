
// Configure UART0 pins
    SYSCTL_RCGCUART_R |= SYSCTL_RCGCUART_R0;         // turn-on UART0, leave other uarts in same status
    GPIO_PORTA_DEN_R |= 3;                           // default, added for clarity
    GPIO_PORTA_AFSEL_R |= 3;                         // default, added for clarity
    GPIO_PORTA_PCTL_R = GPIO_PCTL_PA1_U0TX | GPIO_PCTL_PA0_U0RX;

    // Configure UART0 to 115200 baud, 8N1 format (must be 3 clocks from clock enable and config writes)
    UART0_CTL_R = 0;                                 // turn-off UART0 to allow safe programming
    UART0_CC_R = UART_CC_CS_SYSCLK;                  // use system clock (40 MHz)
    UART0_IBRD_R = 21;                               // r = 40 MHz / (Nx115.2kHz), set floor(r)=21, where N=16
    UART0_FBRD_R = 45;                               // round(fract(r)*64)=45
    UART0_LCRH_R = UART_LCRH_WLEN_8 | UART_LCRH_FEN; // configure for 8N1 w/ 16-level FIFO
    UART0_CTL_R = UART_CTL_TXE | UART_CTL_RXE | UART_CTL_UARTEN; // enable TX, RX, and module

    
// Configure UART1 pins
    SYSCTL_RCGCUART_R |= SYSCTL_RCGCUART_R1;         // turn-on UART1, leave other uarts in same status
    GPIO_PORTB_DEN_R |= 3;                           // default, added for clarity
    GPIO_PORTB_AFSEL_R |= 3;                         // default, added for clarity
    GPIO_PORTB_PCTL_R = GPIO_PCTL_PB1_U0TX | GPIO_PCTL_PB0_U0RX;

    // Configure UART1 to 115200 baud, 8N1 format (must be 3 clocks from clock enable and config writes)
    UART1_CTL_R = 0;                                 // turn-off UART1 to allow safe programming
    UART1_CC_R = UART_CC_CS_SYSCLK;                  // use system clock (40 MHz)
    UART1_IBRD_R = 21;                               // r = 40 MHz / (Nx115.2kHz), set floor(r)=21, where N=16
    UART1_FBRD_R = 45;                               // round(fract(r)*64)=45
    UART1_LCRH_R = UART_LCRH_WLEN_8 | UART_LCRH_FEN; // configure for 8N1 w/ 16-level FIFO
    UART1_CTL_R = UART_CTL_TXE | UART_CTL_RXE | UART_CTL_UARTEN; // enable TX, RX, and module


// Configure UART2 pins
    SYSCTL_RCGCUART_R |= SYSCTL_RCGCUART_R2;         // turn-on UART2, leave other uarts in same status
    GPIO_PORTD_DEN_R |= 0xC0;                           // default, added for clarity
    GPIO_PORTD_AFSEL_R |= 0xC0;                         // default, added for clarity
    GPIO_PORTD_PCTL_R = GPIO_PCTL_PD7_U0TX | GPIO_PCTL_PD6_U0RX;

    // Configure UART2 to 115200 baud, 8N1 format (must be 3 clocks from clock enable and config writes)
    UART2_CTL_R = 0;                                 // turn-off UART2 to allow safe programming
    UART2_CC_R = UART_CC_CS_SYSCLK;                  // use system clock (40 MHz)
    UART2_IBRD_R = 21;                               // r = 40 MHz / (Nx115.2kHz), set floor(r)=21, where N=16
    UART2_FBRD_R = 45;                               // round(fract(r)*64)=45
    UART2_LCRH_R = UART_LCRH_WLEN_8 | UART_LCRH_FEN; // configure for 8N1 w/ 16-level FIFO
    UART2_CTL_R = UART_CTL_TXE | UART_CTL_RXE | UART_CTL_UARTEN; // enable TX, RX, and module

//(16-47)
    UART0_IM_R = UART_IM_RXIM;                       // turn-on RX interrupt
    NVIC_EN0_R |= 1 << (INT_UART0-16);               // turn-on interrupt 21 (UART0)
(48-79)
    UART2_IM_R = UART_IM_RXIM;                       // turn-on RX interrupt
    NVIC_EN1_R |= 1 << (INT_UART2-48);               // turn-on interrupt 49 (UART0)

// other pin assignments
#define UART_LCRH_SPS           0x00000080  // UART Stick Parity Select
#define UART_LCRH_WLEN_M        0x00000060  // UART Word Length
#define UART_LCRH_WLEN_5        0x00000000  // 5 bits (default)
#define UART_LCRH_WLEN_6        0x00000020  // 6 bits
#define UART_LCRH_WLEN_7        0x00000040  // 7 bits
#define UART_LCRH_WLEN_8        0x00000060  // 8 bits
#define UART_LCRH_FEN           0x00000010  // UART Enable FIFOs
#define UART_LCRH_STP2          0x00000008  // UART Two Stop Bits Select
#define UART_LCRH_EPS           0x00000004  // UART Even Parity Select
#define UART_LCRH_PEN           0x00000002  // UART Parity Enable
#define UART_LCRH_BRK           0x00000001  // UART Send Break 

// Interrupt assignments
#define INT_UART0               21          // UART0
#define INT_UART1               22          // UART1(16-47)
#define INT_UART2               49          // UART2
#define INT_UART3               75          // UART3(48-79)
#define INT_UART4               76          // UART4
#define INT_UART5               77          // UART5
#define INT_UART6               78          // UART6
#define INT_UART7               79          // UART7

F(cycle) = (F/ (16 * Baud_Rate))
