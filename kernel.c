/* kernel.c - Main kernel with null process */
#include "types.h"
#include "serial.h"
#include "string.h"
#include "memory.h"

#define MAX_INPUT 128

void kmain(void) {
    char input[MAX_INPUT];
    int pos = 0;
    
    /* Initialize hardware */
    serial_init();

    memory_init();

serial_puts("\nFree safety test:\n");

void *a = kmalloc(256);
void *b = kmalloc(256);

kfree(a);
kfree(a);

serial_puts("Double free: survived\n");

kfree((void *)0x12345678);

serial_puts("Invalid free: survived\n");

void *c = kmalloc(256);

serial_puts(c ? "Allocation after invalid frees: SUCCESS\n"
              : "Allocation after invalid frees: FAILED\n");
        
    /* Print welcome message */
    serial_puts("\n");
    serial_puts("========================================\n");
    serial_puts("    kacchiOS - Minimal Baremetal OS\n");
    serial_puts("========================================\n");
    serial_puts("Hello from kacchiOS!\n");
    serial_puts("Running null process...\n\n");
    
    /* Main loop - the "null process" */
    while (1) {
        serial_puts("kacchiOS> ");
        pos = 0;
        
        /* Read input line */
        while (1) {
            char c = serial_getc();
            
            /* Handle Enter key */
            if (c == '\r' || c == '\n') {
                input[pos] = '\0';
                serial_puts("\n");
                break;
            }
            /* Handle Backspace */
            else if ((c == '\b' || c == 0x7F) && pos > 0) {
                pos--;
                serial_puts("\b \b");  /* Erase character on screen */
            }
            /* Handle normal characters */
            else if (c >= 32 && c < 127 && pos < MAX_INPUT - 1) {
                input[pos++] = c;
                serial_putc(c);  /* Echo character */
            }
        }
        
        /* Echo back the input */
        if (pos > 0) {
            serial_puts("You typed: ");
            serial_puts(input);
            serial_puts("\n");
        }
    }
    
    /* Should never reach here */
    for (;;) {
        __asm__ volatile ("hlt");
    }
}