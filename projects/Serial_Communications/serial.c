/*
 * =============================================================================
 * TCP/IP SERIAL COMMUNICATION INTERFACE
 * =============================================================================
 *
 * FILE: serial.c
 * PROJECT: Serial_Communications
 * COURSE: SOC 3050 - Embedded Systems and Applications
 * YEAR: 2025
 * AUTHOR: Professor Hong Jeong
 *
 * PURPOSE:
 * Unified serial communication interface that works with both:
 * 1. Hardware UART (direct COM port communication)
 * 2. SimulIDE TCP socket (virtual communication via TCP/IP)
 *
 * EDUCATIONAL OBJECTIVES:
 * 1. Learn protocol abstraction (same interface, different transport)
 * 2. Understand TCP/IP vs UART communication differences
 * 3. Implement transparent communication layer
 * 4. Practice conditional compilation for different targets
 *
 * USAGE:
 * - For Hardware: Uses standard UART registers and interrupts
 * - For SimulIDE: Uses TCP socket communication on port 9002
 * - Same API functions work for both modes
 *
 * =============================================================================
 */

#include "config.h"
#include <string.h>
#include <stdio.h>

// Communication mode selection
#ifdef SIMULIDE_TCP_MODE
#define COMM_MODE_TCP 1
#define TCP_PORT 9002
#define TCP_BUFFER_SIZE 256
#else
#define COMM_MODE_UART 1
#endif

// =============================================================================
// COMMUNICATION BUFFER MANAGEMENT
// =============================================================================

#define SERIAL_BUFFER_SIZE 128
#define SERIAL_RX_BUFFER_SIZE 64
#define SERIAL_TX_BUFFER_SIZE 64

// Circular buffer structure
typedef struct
{
    volatile uint8_t buffer[SERIAL_BUFFER_SIZE];
    volatile uint8_t head;
    volatile uint8_t tail;
    volatile uint8_t count;
} circular_buffer_t;

// Global communication buffers
static circular_buffer_t rx_buffer = {0};
static circular_buffer_t tx_buffer = {0};

// Communication status
static volatile uint8_t comm_initialized = 0;
static volatile uint8_t tcp_connected = 0;

// =============================================================================
// BUFFER UTILITY FUNCTIONS
// =============================================================================

/**
 * Add data to circular buffer
 * Returns 1 if successful, 0 if buffer full
 */
static uint8_t buffer_put(circular_buffer_t *buf, uint8_t data)
{
    if (buf->count >= SERIAL_BUFFER_SIZE)
    {
        return 0; // Buffer full
    }

    buf->buffer[buf->head] = data;
    buf->head = (buf->head + 1) % SERIAL_BUFFER_SIZE;
    buf->count++;
    return 1;
}

/**
 * Get data from circular buffer
 * Returns 1 if data available, 0 if buffer empty
 */
static uint8_t buffer_get(circular_buffer_t *buf, uint8_t *data)
{
    if (buf->count == 0)
    {
        return 0; // Buffer empty
    }

    *data = buf->buffer[buf->tail];
    buf->tail = (buf->tail + 1) % SERIAL_BUFFER_SIZE;
    buf->count--;
    return 1;
}

/**
 * Check if buffer has data available
 */
static uint8_t buffer_available(circular_buffer_t *buf)
{
    return buf->count;
}

// =============================================================================
// HARDWARE UART IMPLEMENTATION
// =============================================================================

#ifdef COMM_MODE_UART

/**
 * Initialize UART1 for hardware communication
 * Baud rate: 9600, 8N1
 */
void serial_init_uart(void)
{
    // Set baud rate to 9600 (16MHz crystal)
    // UBRR = (F_CPU / (16 * BAUD)) - 1
    uint16_t ubrr = ((F_CPU / (16UL * 9600)) - 1);
    UBRR1H = (uint8_t)(ubrr >> 8);
    UBRR1L = (uint8_t)ubrr;

    // Enable receiver, transmitter and RX interrupt
    UCSR1B = (1 << RXEN1) | (1 << TXEN1) | (1 << RXCIE1);

    // Set frame format: 8 data bits, 1 stop bit, no parity
    // Note: ATmega128 doesn't have URSEL for UCSR1C (only UCSR0C has it)
    UCSR1C = (1 << UCSZ11) | (1 << UCSZ10);

    // Enable global interrupts
    sei();

    comm_initialized = 1;
}

/**
 * UART RX Complete Interrupt Service Routine
 * Automatically called when data received
 */
ISR(USART1_RX_vect)
{
    uint8_t received_byte = UDR1;
    buffer_put(&rx_buffer, received_byte);
}

/**
 * UART TX Complete Interrupt Service Routine
 * Automatically called when ready to send next byte
 */
ISR(USART1_UDRE_vect)
{
    uint8_t data;
    if (buffer_get(&tx_buffer, &data))
    {
        UDR1 = data; // Send next byte
    }
    else
    {
        // No more data to send, disable TX interrupt
        UCSR1B &= ~(1 << UDRIE1);
    }
}

/**
 * Send single byte via UART
 */
void serial_send_byte_uart(uint8_t data)
{
    buffer_put(&tx_buffer, data);

    // Enable TX interrupt to start transmission
    UCSR1B |= (1 << UDRIE1);
}

/**
 * Check if UART data is available
 */
uint8_t serial_available_uart(void)
{
    return buffer_available(&rx_buffer);
}

/**
 * Read byte from UART
 */
uint8_t serial_read_byte_uart(void)
{
    uint8_t data;
    if (buffer_get(&rx_buffer, &data))
    {
        return data;
    }
    return 0;
}

#endif // COMM_MODE_UART

// =============================================================================
// TCP/IP SOCKET IMPLEMENTATION (SimulIDE)
// =============================================================================

#ifdef COMM_MODE_TCP

// TCP simulation variables (for educational purposes)
static volatile uint8_t tcp_rx_data[TCP_BUFFER_SIZE];
static volatile uint8_t tcp_tx_data[TCP_BUFFER_SIZE];
static volatile uint16_t tcp_rx_count = 0;
static volatile uint16_t tcp_tx_count = 0;
static volatile uint16_t tcp_rx_index = 0;
static volatile uint16_t tcp_tx_index = 0;

/**
 * Initialize TCP communication
 * In real implementation, this would establish socket connection
 */
void serial_init_tcp(void)
{
    // Initialize TCP buffers
    tcp_rx_count = 0;
    tcp_tx_count = 0;
    tcp_rx_index = 0;
    tcp_tx_index = 0;

    // In SimulIDE, TCP connection is automatically handled
    // This is a simulation of the connection process
    tcp_connected = 1;
    comm_initialized = 1;

    // For educational purposes, simulate some initial data
    const char *welcome_msg = "TCP Serial Bridge Ready\\r\\n";
    strcpy((char *)tcp_rx_data, welcome_msg);
    tcp_rx_count = strlen(welcome_msg);
}

/**
 * Simulate TCP data reception
 * In real implementation, this would read from TCP socket
 */
void serial_tcp_receive_simulation(const char *data)
{
    uint16_t len = strlen(data);
    if (len < TCP_BUFFER_SIZE - tcp_rx_count)
    {
        memcpy((void *)(tcp_rx_data + tcp_rx_count), data, len);
        tcp_rx_count += len;
    }
}

/**
 * Send byte via TCP
 * In real implementation, this would write to TCP socket
 */
void serial_send_byte_tcp(uint8_t data)
{
    if (tcp_tx_count < TCP_BUFFER_SIZE)
    {
        tcp_tx_data[tcp_tx_count++] = data;
    }
}

/**
 * Check if TCP data is available
 */
uint8_t serial_available_tcp(void)
{
    return (tcp_rx_index < tcp_rx_count) ? 1 : 0;
}

/**
 * Read byte from TCP
 */
uint8_t serial_read_byte_tcp(void)
{
    if (tcp_rx_index < tcp_rx_count)
    {
        return tcp_rx_data[tcp_rx_index++];
    }
    return 0;
}

/**
 * Get transmitted data (for debugging/monitoring)
 */
const char *serial_get_tcp_output(void)
{
    tcp_tx_data[tcp_tx_count] = '\\0'; // Null terminate
    return (const char *)tcp_tx_data;
}

/**
 * Clear TCP output buffer
 */
void serial_clear_tcp_output(void)
{
    tcp_tx_count = 0;
    tcp_tx_index = 0;
}

#endif // COMM_MODE_TCP

// =============================================================================
// UNIFIED SERIAL INTERFACE
// =============================================================================

/**
 * Initialize serial communication
 * Automatically selects UART or TCP based on compilation mode
 */
void serial_init(void)
{
#ifdef COMM_MODE_UART
    serial_init_uart();
#elif defined(COMM_MODE_TCP)
    serial_init_tcp();
#endif
}

/**
 * Send single byte (unified interface)
 */
void serial_send_byte(uint8_t data)
{
#ifdef COMM_MODE_UART
    serial_send_byte_uart(data);
#elif defined(COMM_MODE_TCP)
    serial_send_byte_tcp(data);
#endif
}

/**
 * Send string (unified interface)
 */
void serial_send_string(const char *str)
{
    while (*str)
    {
        serial_send_byte(*str++);
    }
}

/**
 * Send string with newline
 */
void serial_send_line(const char *str)
{
    serial_send_string(str);
    serial_send_string("\\r\\n");
}

/**
 * Check if data is available (unified interface)
 */
uint8_t serial_available(void)
{
#ifdef COMM_MODE_UART
    return serial_available_uart();
#elif defined(COMM_MODE_TCP)
    return serial_available_tcp();
#else
    return 0;
#endif
}

/**
 * Read single byte (unified interface)
 */
uint8_t serial_read_byte(void)
{
#ifdef COMM_MODE_UART
    return serial_read_byte_uart();
#elif defined(COMM_MODE_TCP)
    return serial_read_byte_tcp();
#else
    return 0;
#endif
}

/**
 * Read string until newline or timeout
 */
uint8_t serial_read_line(char *buffer, uint8_t max_length)
{
    uint8_t index = 0;
    uint8_t data;

    while (index < max_length - 1)
    {
        if (serial_available())
        {
            data = serial_read_byte();

            // Handle carriage return and line feed
            if (data == '\r' || data == '\n')
            {
                break;
            }

            buffer[index++] = data;
        }
        // In real implementation, add timeout handling here
    }

    buffer[index] = '\0'; // Null terminate
    return index;         // Return number of characters read
}

/**
 * Get communication status
 */
uint8_t serial_is_initialized(void)
{
    return comm_initialized;
}

/**
 * Simple echo test function
 */
void serial_echo_test(void)
{
    serial_send_line("=== Serial Echo Test ===");
    serial_send_line("Type characters to echo them back");
    serial_send_line("Press 'q' to quit");

    while (1)
    {
        if (serial_available())
        {
            uint8_t received = serial_read_byte();

            // Echo back the character
            serial_send_byte(received);

            // Quit on 'q'
            if (received == 'q' || received == 'Q')
            {
                serial_send_line("\\nEcho test terminated.");
                break;
            }
        }
    }
}

/**
 * Demonstration function for TCP vs UART comparison
 */
void serial_demo_communication_modes(void)
{
    serial_send_line("=== Communication Mode Demo ===");

#ifdef COMM_MODE_UART
    serial_send_line("Mode: Hardware UART");
    serial_send_line("Features: Real hardware interrupts, true serial communication");
    serial_send_line("Connection: Physical COM port");
#elif defined(COMM_MODE_TCP)
    serial_send_line("Mode: TCP/IP Socket (SimulIDE)");
    serial_send_line("Features: Virtual communication, network protocol");
    serial_send_line("Connection: TCP socket on port 9002");
#endif

    serial_send_line("Ready for communication!");
}
