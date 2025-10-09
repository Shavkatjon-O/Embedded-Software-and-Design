Serial_Communications — TCP Demo (SimulIDE)
==========================================

Purpose
-------
This README shows how to run the Serial_Communications project using the repository's TCP bridge (no admin required). SimulIDE will be configured to use a TCP socket (localhost:9002). The bridge listens on localhost:9001 for host-side clients and forwards data to SimulIDE on localhost:9002.

Steps
-----
1. Open a PowerShell terminal and change directory to the SimulIDE tools folder:

   ```powershell
   cd w:\soc3050code\tools\simulide
   ```

2. Configure the SimulIDE circuit file to use TCP (creates a backup of the circuit):

   ```powershell
   .\configure-simulide-tcp.ps1
   ```

3. Start the TCP bridge (no admin required). This will start listeners on ports 9001 and 9002:

   ```powershell
   .\windows-virtual-com.ps1 -Method tcpsockets -Start
   ```

   - Port 9001 is for host applications (students' serial monitor or test clients).
   - Port 9002 is expected by SimulIDE's SerialPort component.

4. Launch SimulIDE and open the project circuit. SimulIDE will use `localhost:9002` for the SerialPort.

5. From the `projects/Serial_Communications` folder you can run one of the provided test clients to interact with the simulated serial port.

PowerShell test client (simple interactive)
------------------------------------------
Run the provided `tcp_test_client.ps1` from PowerShell (pwsh). This script connects to localhost:9001 and provides a simple interactive prompt. Type text and press Enter to send; received data will print.

Python test client (recommended for automation)
-----------------------------------------------
Run the provided `tcp_test_client.py` (requires Python 3). It connects to localhost:9001, sends a handshake, and prints any replies.

Notes
-----
- If ports 9001 or 9002 are already in use, modify the scripts in `tools/simulide` to choose different ports (and update the clients accordingly).
- Windows firewall may prompt to allow the bridge script to open ports; allow it or update firewall rules for localhost loopback.
- This setup preserves the MCU USART behaviour inside SimulIDE; polling and ISR-based demos from `Main.c` work unchanged when the code runs inside the simulator.

Troubleshooting
---------------
- If you do not see any reply, ensure the TCP bridge is running and SimulIDE is connected to `localhost:9002`.
- Use `netstat -ano | Select-String "9001|9002"` to check whether listeners are active.
- For instructor-hosted labs, TCP is recommended since it does not require admin privileges.
