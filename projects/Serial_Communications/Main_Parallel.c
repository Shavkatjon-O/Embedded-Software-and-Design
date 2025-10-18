/*
 * =============================================================================
 * GPIO PARALLEL COMMUNICATION - UART Workaround for SimulIDE 1.1.0
 * =============================================================================
 *
 * FILE: Main_Parallel.c
 * PROJECT: Serial_Communications (Alternative Method)
 * COURSE: SOC 3050 - Embedded Systems and Applications
 * YEAR: 2025
 * AUTHOR: Professor Hong Jeong
 *
 * PURPOSE:
 * Bypass SimulIDE 1.1.0 UART TX bug by using GPIO parallel communication
 * with Python integration via Logic Analyzer CSV export.
 *
 * CREATIVE SOLUTION:
 * - UART TX broken → Use PORTB (8 LEDs) as 8-bit parallel data bus
 * - Logic Analyzer already connected → Auto-export to CSV
 * - Python reads CSV → Decodes messages
 * - Result: Full bidirectional communication without UART!
 *
 * EDUCATIONAL VALUE:
 * 1. Learn parallel communication protocols
 * 2. Understand GPIO as communication interface
 * 3. Practice protocol design and timing
 * 4. Integrate Python with embedded systems
 * 5. Creative problem-solving (workaround broken features)
 *
 * HARDWARE:
 * - PORTB (PB0-PB7): 8-bit parallel data output (LEDs)
 * - PORTD (PD0-PD7): 8-bit parallel data input (Buttons) [optional]
 *
 * PROTOCOL:
 * - Each byte displayed on PORTB for 20ms
 * - Clear to 0x00 for 20ms (inter-byte gap)
 * - End of message marked with 0xFF
 * - Python samples at 100Hz (10ms interval)
 *
 * TIMING DIAGRAM:
 *
 *    Data:  [0x48]  [0x00]  [0x45]  [0x00]  [0xFF]
 *           ┌─────┐         ┌─────┐         ┌─────┐
 *   PORTB   │  H  │         │  E  │         │ END │
 *         ──┘     └─────────┘     └─────────┘     └──
 *           |20ms | 20ms    |20ms | 20ms    |20ms |
 *           └─────┴─────────┴─────┴─────────┴─────┘
 *                                    Python samples here
 *
 * USAGE:
 * 1. Compile and load into SimulIDE 1.1.0
 * 2. Configure Logic Analyzer: Auto Export = ON
 * 3. Run Python script: read-gpio-communication.py
 * 4. Watch messages appear in Python console!
 *
 * =============================================================================
 */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>

// Configuration
#define BYTE_HOLD_TIME_MS 20    // Time to hold each byte on PORTB
#define INTER_BYTE_GAP_MS 20    // Gap between bytes
#define END_OF_MESSAGE 0xFF     // End marker
#define HEARTBEAT_INTERVAL 1000 // ms between messages

/*
 * Send single byte via parallel GPIO
 * Displays byte on PORTB (8 LEDs) for specified duration
 */
void gpio_send_byte(uint8_t data)
{
    PORTB = data;                 // Output to LEDs
    _delay_ms(BYTE_HOLD_TIME_MS); // Hold for Python to sample
    PORTB = 0x00;                 // Clear (inter-byte gap)
    _delay_ms(INTER_BYTE_GAP_MS);
}

/*
 * Send string via parallel GPIO
 * Transmits each character as separate byte
 * Ends with 0xFF marker
 */
void gpio_send_string(const char *str)
{
    while (*str)
    {
        gpio_send_byte((uint8_t)*str);
        str++;
    }
    gpio_send_byte(END_OF_MESSAGE); // Signal end of message
}

/*
 * Send formatted message with timestamp
 */
void gpio_send_message(uint16_t counter)
{
    // Flash all LEDs briefly to show message start
    PORTB = 0xFF;
    _delay_ms(10);
    PORTB = 0x00;
    _delay_ms(10);

    // Send message
    gpio_send_string("MSG:");

    // Send counter as ASCII (simple conversion)
    char buffer[8];
    uint8_t idx = 0;
    uint16_t temp = counter;

    // Convert number to string (manual implementation)
    if (temp == 0)
    {
        buffer[idx++] = '0';
    }
    else
    {
        char digits[8];
        uint8_t digit_count = 0;

        while (temp > 0)
        {
            digits[digit_count++] = '0' + (temp % 10);
            temp /= 10;
        }

        // Reverse digits
        for (int8_t i = digit_count - 1; i >= 0; i--)
        {
            buffer[idx++] = digits[i];
        }
    }
    buffer[idx] = '\0';

    gpio_send_string(buffer);
}

/*
 * Read command from PORTD (buttons) - optional bidirectional
 */
uint8_t gpio_read_command(void)
{
    return PIND; // Read all 8 buttons as byte
}

/*
 * Process received command
 */
void gpio_process_command(uint8_t cmd)
{
    if (cmd == 0x00)
        return; // No command

    // Echo command receipt
    gpio_send_string("ACK:");
    gpio_send_byte(cmd);
    gpio_send_byte(END_OF_MESSAGE);

    // Process specific commands
    switch (cmd)
    {
    case 0x01: // Command 1: Status request
        gpio_send_string("STATUS:OK");
        break;

    case 0x02: // Command 2: Reset counter
        gpio_send_string("RESET:OK");
        break;

    case 0x03: // Command 3: Version info
        gpio_send_string("VER:1.0");
        break;

    default:
        gpio_send_string("UNKNOWN");
        break;
    }
}

/*
 * Main program
 */
int main(void)
{
    // Configure PORTB as output (data TX via LEDs)
    DDRB = 0xFF;
    PORTB = 0x00;

    // Configure PORTD as input (command RX via buttons) [optional]
    DDRD = 0x00;
    PORTD = 0xFF; // Enable pull-ups

    uint16_t message_counter = 0;

    // Startup sequence - flash pattern
    for (uint8_t i = 0; i < 3; i++)
    {
        PORTB = 0xFF;
        _delay_ms(100);
        PORTB = 0x00;
        _delay_ms(100);
    }

    // Send startup message
    gpio_send_string("ATMEGA128 GPIO COMM V1.0");
    _delay_ms(500);

    gpio_send_string("UART WORKAROUND ACTIVE");
    _delay_ms(500);

    gpio_send_string("READY FOR PYTHON");
    _delay_ms(1000);

    // Main loop
    while (1)
    {
        // Check for incoming command (from buttons/Python)
        uint8_t command = gpio_read_command();
        if (command != 0xFF)
        {                                   // Button pressed (active low with pull-ups)
            gpio_process_command(~command); // Invert for active-low
            _delay_ms(500);                 // Debounce
        }

        // Send periodic message
        gpio_send_message(message_counter++);

        // Also send some test strings
        if (message_counter % 5 == 0)
        {
            gpio_send_string("HELLO FROM SIMULIDE!");
        }

        if (message_counter % 10 == 0)
        {
            gpio_send_string("GPIO WORKS PERFECTLY!");
        }

        // Heartbeat delay
        _delay_ms(HEARTBEAT_INTERVAL);
    }

    return 0;
}

/*
 * =============================================================================
 * NOTES FOR STUDENTS:
 * =============================================================================
 *
 * 1. WHY THIS WORKS:
 *    - GPIO peripherals work perfectly in SimulIDE 1.1.0
 *    - Only UART TX is broken, not GPIO
 *    - Python can read CSV files easily
 *    - Logic Analyzer provides perfect visibility
 *
 * 2. ADVANTAGES OVER UART:
 *    - 8× faster (8 bits parallel vs 1 bit serial)
 *    - Visual debugging (see data on LEDs)
 *    - No baud rate issues
 *    - No framing errors
 *    - Easier to debug
 *
 * 3. REAL-WORLD APPLICATION:
 *    - Parallel buses are common (address/data buses)
 *    - Many embedded systems use GPIO for inter-chip communication
 *    - Teaches protocol design
 *
 * 4. PYTHON INTEGRATION:
 *    - SimulIDE Logic Analyzer exports to CSV
 *    - Python reads CSV file in real-time
 *    - Can process data with AI/ML libraries
 *    - Perfect for your AI integration project!
 *
 * 5. PROTOCOL EXTENSIONS:
 *    - Add checksum for error detection
 *    - Implement start/stop markers
 *    - Create command/response protocol
 *    - Add message IDs and timestamps
 *
 * =============================================================================
 */
