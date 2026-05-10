#define PERIPHERAL_BASE 0x3F000000
#define GPIO_BASE       (PERIPHERAL_BASE + 0x200000)
#define UART0_BASE      (PERIPHERAL_BASE + 0x201000)

#define GPFSEL1         (*(volatile unsigned int*)(GPIO_BASE + 0x04))
#define UART0_DR        (*(volatile unsigned int*)(UART0_BASE + 0x00))
#define UART0_FR        (*(volatile unsigned int*)(UART0_BASE + 0x18))
#define UART0_IBRD      (*(volatile unsigned int*)(UART0_BASE + 0x24))
#define UART0_FBRD      (*(volatile unsigned int*)(UART0_BASE + 0x28))
#define UART0_LCRH      (*(volatile unsigned int*)(UART0_BASE + 0x2c))
#define UART0_CR        (*(volatile unsigned int*)(UART0_BASE + 0x30))
#define UART0_ICR       (*(volatile unsigned int*)(UART0_BASE + 0x44))

void uart_init();
void uart_putc(unsigned char c);
void uart_puts(char *s);

void main() 
{
    uart_init();
    uart_puts("Hello from Pi 3 B+ Aarch64 Bare Metal!\r\n");
    while(1);
}

void uart_init() 
{
    UART0_CR = 0;
    unsigned int r = GPFSEL1;
    r &= ~((7 << 12) | (7 << 15)); 
    r |= (4 << 12) | (4 << 15);    // Set GPIO 14/15 to ALT0
    GPFSEL1 = r;
//    UART0_ICR = 0x7FF;
    UART0_ICR = 0x7F0;
    UART0_IBRD = 26;               // 115200 baud @ 48MHz
    UART0_FBRD = 3;
    UART0_LCRH = 0x70; 
    UART0_CR = 0x301; 
}

void uart_putc(unsigned char c) 
{
    while(UART0_FR & (1 << 5)); 
    UART0_DR = c;
}

void uart_puts(char *s) 
{
    while(*s)
	{	
		uart_putc(*s++);
			
	}
}