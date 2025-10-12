/*
 * ============================================================================
 * ATMEGA128 POLLING vs INTERRUPT EDUCATIONAL DEMO
 * ============================================================================
 * Purpose: Comprehensive lecture material for polling vs interrupt concepts
 * Target: ATmega128 microcontroller @ 16MHz
 *
 * ============================================================================
 * HARDWARE SETUP
 * ============================================================================
 * - PORTB: LEDs (active low - 0=ON, 1=OFF)
 * - PORTD: Buttons (active low with pull-ups - 0=pressed, 1=idle)
 * - GLCD: Real-time status display and educational feedback
 *
 * Button Functions:
 * - PD1: Polling demo (LED3 follows button state)
 * - PD0: Interrupt demo (LED0 toggles on press via INT0)
 *
 * ============================================================================
 * ATMEGA128 EXTERNAL INTERRUPT VECTORS TABLE
 * ============================================================================
 * Vector | Name    | Pin | Description                | Address
 * -------|---------|-----|----------------------------|--------
 *   2    | INT0    | PD0 | External Interrupt 0       | 0x0004
 *   3    | INT1    | PD1 | External Interrupt 1       | 0x0006
 *   4    | INT2    | PD2 | External Interrupt 2       | 0x0008
 *   5    | INT3    | PD3 | External Interrupt 3       | 0x000A
 *   6    | INT4    | PE4 | External Interrupt 4       | 0x000C
 *   7    | INT5    | PE5 | External Interrupt 5       | 0x000E
 *   8    | INT6    | PE6 | External Interrupt 6       | 0x0010
 *   9    | INT7    | PE7 | External Interrupt 7       | 0x0012
 *
 * ============================================================================
 * EXTERNAL INTERRUPT PIN MAPPING
 * ============================================================================
 * Interrupt | Pin | Port | Bit | Control Bits | Trigger Options
 * ----------|-----|------|-----|--------------|------------------
 * INT0      | PD0 | D    | 0   | ISC01,ISC00  | Low, Change, Fall, Rise
 * INT1      | PD1 | D    | 1   | ISC11,ISC10  | Low, Change, Fall, Rise
 * INT2      | PD2 | D    | 2   | ISC21,ISC20  | Low, Change, Fall, Rise
 * INT3      | PD3 | D    | 3   | ISC31,ISC30  | Low, Change, Fall, Rise
 * INT4      | PE4 | E    | 4   | ISC41,ISC40  | Low, Change, Fall, Rise
 * INT5      | PE5 | E    | 5   | ISC51,ISC50  | Low, Change, Fall, Rise
 * INT6      | PE6 | E    | 6   | ISC61,ISC60  | Low, Change, Fall, Rise
 * INT7      | PE7 | E    | 7   | ISC71,ISC70  | Low, Change, Fall, Rise
 *
 * ============================================================================
 * INTERRUPT CONTROL REGISTERS
 * ============================================================================
 *
 * EICRA (External Interrupt Control Register A) - Address: 0x6A
 * ┌─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┐
 * │ISC31│ISC30│ISC21│ISC20│ISC11│ISC10│ISC01│ISC00│
 * └─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┘
 *   Bit7  Bit6  Bit5  Bit4  Bit3  Bit2  Bit1  Bit0
 *
 * EICRB (External Interrupt Control Register B) - Address: 0x5A
 * ┌─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┐
 * │ISC71│ISC70│ISC61│ISC60│ISC51│ISC50│ISC41│ISC40│
 * └─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┘
 *   Bit7  Bit6  Bit5  Bit4  Bit3  Bit2  Bit1  Bit0
 *
 * EIMSK (External Interrupt Mask Register) - Address: 0x59
 * ┌─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┐
 * │INT7 │INT6 │INT5 │INT4 │INT3 │INT2 │INT1 │INT0 │
 * └─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┘
 *   Bit7  Bit6  Bit5  Bit4  Bit3  Bit2  Bit1  Bit0
 *
 * EIFR (External Interrupt Flag Register) - Address: 0x58
 * ┌─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┐
 * │INTF7│INTF6│INTF5│INTF4│INTF3│INTF2│INTF1│INTF0│
 * └─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┘
 *   Bit7  Bit6  Bit5  Bit4  Bit3  Bit2  Bit1  Bit0
 *
 * ============================================================================
 * INTERRUPT SENSE CONTROL (ISC) BIT COMBINATIONS
 * ============================================================================
 * ISCn1 | ISCn0 | Description
 * ------|-------|----------------------------------------------------------
 *   0   |   0   | Low level triggers interrupt
 *   0   |   1   | Any logical change triggers interrupt
 *   1   |   0   | Falling edge triggers interrupt (HIGH to LOW)
 *   1   |   1   | Rising edge triggers interrupt (LOW to HIGH)
 *
 * Example: For INT0 (our demo)
 * - EICRA |= (1<<ISC01); EICRA &= ~(1<<ISC00);  // Falling edge
 * - EIMSK |= (1<<INT0);                          // Enable INT0
 * - sei();                                       // Global interrupt enable
 *
 * ============================================================================
 * POLLING vs INTERRUPT COMPARISON TABLE
 * ============================================================================
 * Aspect           | POLLING                   | INTERRUPT
 * -----------------|---------------------------|---------------------------
 * Response Time    | Depends on loop time      | Immediate (few cycles)
 * CPU Usage        | Continuous checking       | Event-driven (efficient)
 * Code Complexity  | Simple linear flow        | Requires ISR management
 * Predictability   | Deterministic timing      | Asynchronous events
 * Power Consumption| Higher (continuous loop)  | Lower (CPU can sleep)
 * Real-time Response| May miss short events    | Guaranteed response
 * Multiple Events  | Sequential checking       | Priority-based handling
 * Debugging        | Easy to trace execution   | Complex (ISR context)
 * Implementation   | Straightforward           | Requires careful design
 * Best Use Cases   | Simple, periodic checks   | Critical, time-sensitive
 *
 * ============================================================================
 * INTERRUPT PROGRAMMING STEPS (ATmega128)
 * ============================================================================
 * 1. Configure the pin as input (DDRx &= ~(1<<pin))
 * 2. Enable internal pull-up if needed (PORTx |= (1<<pin))
 * 3. Set interrupt sense control bits in EICRA/EICRB
 * 4. Enable specific interrupt in EIMSK
 * 5. Enable global interrupts (sei())
 * 6. Implement ISR(INTx_vect) function
 * 7. Keep ISR short and fast
 * 8. Use volatile for shared variables
 *
 * ============================================================================
 * EDUCATIONAL OBJECTIVES
 * ============================================================================
 * 1. Understand fundamental differences between polling and interrupts
 * 2. Learn ATmega128 external interrupt system
 * 3. Practice interrupt configuration and ISR programming
 * 4. Observe real-time system behavior and timing
 * 5. Master active-low hardware interfacing
 * 6. Understand register manipulation and bit operations
 * 7. Learn proper volatile variable usage in ISR context
 *
 * ============================================================================
 * DEMO SELECTION: Uncomment ONE function in main()
 * ============================================================================
 */

#include "config.h"

// ============================================================================
// INTERRUPT SERVICE ROUTINE (ISR) IMPLEMENTATION
// ============================================================================
// Simple state shared with ISR (MUST be volatile for ISR communication)
volatile uint8_t int0_flag = 0;

// INT0 interrupt service routine: Executes when PD0 falling edge detected
// ISR DESIGN PRINCIPLES:
// 1. Keep ISR SHORT and FAST (avoid delays, heavy processing)
// 2. Use volatile for variables shared between ISR and main
// 3. Avoid calling complex functions from ISR
// 4. Set flags for main loop processing when possible
ISR(INT0_vect)
{
    int0_flag = 1; // Set flag for main loop processing
    led_toggle(0); // Immediate visual feedback (simple operation)
    // NOTE: More complex processing should be done in main loop after flag check
}

static void ui_draw_header(void)
{
    lcd_clear();
    lcd_string(0, 0, "Polling vs Interrupt");
    lcd_string(1, 0, "PD1: Polling (LED3)");
    lcd_string(2, 0, "PD0: INT0  (LED0)");
}

static void setup_io_and_display(void)
{
    // Initialize ports and GLCD
    init_devices(); // uses Port_init() and lcd_init() only

    // Buttons are already inputs with pull-ups in Port_init(), but ensure PD0/PD1
    DDRD &= ~((1 << PD0) | (1 << PD1));
    PORTD |= (1 << PD0) | (1 << PD1);

    ui_draw_header();

    // Ensure LED port default OFF AFTER all initialization
    led_all_off();
}

static void setup_ext_interrupt(void)
{
    // ========================================================================
    // EXTERNAL INTERRUPT CONFIGURATION - STEP BY STEP
    // ========================================================================

    // STEP 1: Configure interrupt sense control for INT0 (PD0)
    // We want falling edge trigger (button press: HIGH -> LOW)
    // ISC01=1, ISC00=0 for falling edge (see truth table above)
    EICRA |= (1 << ISC01);  // Set ISC01 bit in EICRA register
    EICRA &= ~(1 << ISC00); // Clear ISC00 bit in EICRA register

    // STEP 2: Enable INT0 interrupt in interrupt mask register
    // This allows INT0 to actually generate interrupts
    EIMSK |= (1 << INT0); // Set INT0 bit in EIMSK register

    // STEP 3: Enable global interrupts
    // This is the master enable for ALL interrupts in the system
    sei(); // Set global interrupt enable flag (I-bit in SREG)

    // ========================================================================
    // WHAT HAPPENS WHEN PD0 BUTTON IS PRESSED:
    // 1. PD0 goes from HIGH to LOW (falling edge)
    // 2. Hardware detects edge based on EICRA settings
    // 3. If EIMSK[INT0]=1 and global interrupts enabled, interrupt fires
    // 4. CPU saves context and jumps to ISR(INT0_vect)
    // 5. ISR executes our code (toggle LED, set flag)
    // 6. CPU restores context and returns to main program
    // ========================================================================
}

// ============================================================================
// DEMO 1: POLLING METHOD - CONTINUOUS CHECKING
// ============================================================================
// This demo shows the traditional polling approach where the CPU continuously
// checks the button state in a loop. The button controls LED3 directly.
//
// POLLING CHARACTERISTICS:
// - CPU is always busy checking input state
// - Response time depends on loop execution time
// - Simple to understand and implement
// - May miss very short button presses
// - Higher power consumption (CPU never sleeps)
static void demo_polling(void)
{
    setup_io_and_display();

    lcd_string(4, 0, "Mode: POLLING");
    lcd_string(5, 0, "Press PD1 button");

    // MAIN POLLING LOOP: This runs continuously, checking button state
    while (1)
    {
        // Read current button state from PIND register
        uint8_t pind_value = PIND;

        // POLLING LOGIC: Check button state and respond immediately
        // This happens every loop iteration - very frequent checking
        if (pind_value & (1 << PD1)) // Button not pressed (HIGH)
        {
            led_off(3); // LED3 OFF - button released
        }
        else // Button pressed (LOW)
        {
            led_on(3); // LED3 ON - button pressed
        }

        // REAL-TIME STATUS DISPLAY for educational purposes
        // Shows actual register values so students can see binary patterns
        lcd_xy(6, 0);
        lcd_string(6, 0, "PORTB:");
        lcd_xy(6, 7);
        GLCD_3DigitDecimal(PORTB); // Shows LED states in decimal

        lcd_xy(7, 0);
        lcd_string(7, 0, "PIND: ");
        lcd_xy(7, 7);
        GLCD_3DigitDecimal(pind_value); // Shows button states in decimal

        _delay_ms(50); // Small delay for display refresh rate
        // NOTE: This delay affects polling response time!
        // Shorter delay = faster response but more CPU usage
        // Longer delay = slower response but less CPU usage
    }
}

// ============================================================================
// DEMO 2: INTERRUPT METHOD - EVENT-DRIVEN RESPONSE
// ============================================================================
// This demo shows the interrupt-driven approach where the CPU responds to
// button presses automatically via hardware interrupts. Button toggles LED0.
//
// INTERRUPT CHARACTERISTICS:
// - CPU responds immediately to events (few clock cycles)
// - CPU can do other tasks or sleep between events
// - More complex to implement and debug
// - Guaranteed response to all events (won't miss short presses)
// - Lower power consumption (CPU can sleep)
// - Asynchronous - events can happen anytime
static void demo_interrupt(void)
{
    setup_io_and_display(); // Initialize hardware
    setup_ext_interrupt();  // Configure INT0 interrupt system

    lcd_string(4, 0, "Mode: INTERRUPT");
    lcd_string(5, 0, "Press PD0 button");

    // MAIN LOOP: CPU can do other tasks while waiting for interrupts
    // This loop is NOT checking the button - that's handled by hardware!
    while (1)
    {
        // CHECK ISR FLAG: See if interrupt occurred
        // This is the communication method between ISR and main program
        if (int0_flag)
        {
            int0_flag = 0;                         // Clear flag (acknowledge)
            lcd_string(5, 0, "INT0: Triggered! "); // Visual feedback
            _delay_ms(500);                        // Show message briefly
            lcd_string(5, 0, "Press PD0 button");  // Reset message
        }

        // REAL-TIME STATUS DISPLAY
        // Shows port registers even though main loop isn't checking buttons
        uint8_t pind_value = PIND;

        lcd_xy(6, 0);
        lcd_string(6, 0, "PORTB:");
        lcd_xy(6, 7);
        GLCD_3DigitDecimal(PORTB); // Shows LED states (LED0 toggles via ISR)

        lcd_xy(7, 0);
        lcd_string(7, 0, "PIND: ");
        lcd_xy(7, 7);
        GLCD_3DigitDecimal(pind_value); // Shows button states

        _delay_ms(100); // Moderate refresh rate
        // NOTE: Even with this delay, button response is immediate!
        // That's because ISR handles button press, not this main loop
    }
}

int main(void)
{
    // ========================================================================
    // EDUCATIONAL DEMO SELECTION - ATMEGA128 INTERRUPT LEARNING
    // ========================================================================
    // Choose ONE demo by uncommenting the appropriate function call below.

    demo_polling(); // DEMO 1: Polling method (check PD1 continuously)
    // demo_interrupt(); // DEMO 2: Interrupt method (respond to PD0 events)

    // ========================================================================
    // EXPERIMENT SUGGESTIONS FOR STUDENTS
    // ========================================================================
    /*
     * 1. Polling Response Time:
     *    - Change _delay_ms(50) to different values
     *    - Observe how it affects button response
     *
     * 2. Interrupt Edge Types:
     *    - Modify EICRA settings for rising edge (ISC01=1, ISC00=1)
     *    - Try "any change" mode (ISC01=0, ISC00=1)
     *
     * 3. Register Value Analysis:
     *    - Watch PORTB/PIND values change with hardware states
     *    - Calculate binary equivalents of decimal values shown
     */

    return 0;
}