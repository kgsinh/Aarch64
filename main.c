#define PERIPHERAL_BASE 0x3F000000
#define GPIO_BASE       (PERIPHERAL_BASE + 0x200000)
#define UART0_BASE      (PERIPHERAL_BASE + 0x201000)

// GPIO Function Select Registers
#define GPFSEL1         (*(volatile unsigned int*)(GPIO_BASE + 0x04))

// GPIO Pin Output Set/Clear Registers
#define GPSET0          (*(volatile unsigned int*)(GPIO_BASE + 0x1C))
#define GPCLR0          (*(volatile unsigned int*)(GPIO_BASE + 0x28))

// UART Registers
#define UART0_DR        (*(volatile unsigned int*)(UART0_BASE + 0x00))
#define UART0_FR        (*(volatile unsigned int*)(UART0_BASE + 0x18))
#define UART0_IBRD      (*(volatile unsigned int*)(UART0_BASE + 0x24))
#define UART0_FBRD      (*(volatile unsigned int*)(UART0_BASE + 0x28))
#define UART0_LCRH      (*(volatile unsigned int*)(UART0_BASE + 0x2c))
#define UART0_CR        (*(volatile unsigned int*)(UART0_BASE + 0x30))
#define UART0_ICR       (*(volatile unsigned int*)(UART0_BASE + 0x44))

// LED GPIO Pin
#define LED_PIN         13

// Function declarations
void uart_init(void);
void uart_putc(unsigned char c);
void uart_puts(char *s);
void gpio_init(void);
void led_on(void);
void led_off(void);
void delay(unsigned int count);

void main(void) 
{
    uart_init();
    gpio_init();
    uart_puts("Welcome to PyjamaCafe!\r\n");
    uart_puts("Hello from Pi 3 B+ Aarch64 Bare Metal!\r\n");
    uart_puts("Starting LED blink on GPIO13...\r\n");
    
    while(1)
    {
        led_on();
        uart_puts("LED ON\r\n");
        delay(500000);
        
        led_off();
        uart_puts("LED OFF\r\n");
        delay(500000);
    }
}

void uart_init(void) 
{
    UART0_CR = 0;
    
    unsigned int r = GPFSEL1;
    r &= ~((7 << 12) | (7 << 15)); 
    r |= (4 << 12) | (4 << 15);    // Set GPIO 14/15 to ALT0 for UART
    GPFSEL1 = r;
    
    UART0_ICR = 0x7F0;
    UART0_IBRD = 26;               // 115200 baud @ 48MHz
    UART0_FBRD = 3;
    UART0_LCRH = 0x70; 
    UART0_CR = 0x301; 
}

void gpio_init(void)
{
    unsigned int r = GPFSEL1;
    
    // GPIO13 is in GPFSEL1 register
    // GPIO13 uses bits [11:9] (3 bits per pin)
    // Pin offset in GPFSEL1: 13 - 10 = 3, so bits [9:11]
    r &= ~(7 << 9);     // Clear bits 9-11 (clear GPIO13 function)
    r |= (1 << 9);      // Set to 001 = Output
    
    GPFSEL1 = r;
    
    // Ensure LED starts OFF
    led_off();
}

void led_on(void)
{
    GPSET0 = (1 << LED_PIN);    // Set GPIO13 high
}

void led_off(void)
{
    GPCLR0 = (1 << LED_PIN);    // Clear GPIO13 low
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

void delay(unsigned int count)
{
    while(count--)
    {
        asm volatile("nop");
    }
}