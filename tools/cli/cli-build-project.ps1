param([string]$ProjectDir)

# HARDWARE BUILD: Standard compilation with ELPM instructions for real hardware
Write-Host "Analyzing: $ProjectDir"

# PORTABLE SYSTEM - Use only included tools, no external dependencies
$WorkspaceRoot = Split-Path (Split-Path $PSScriptRoot -Parent) -Parent

# Portable AVR tools (REQUIRED - no external fallbacks)
$portableAvrGcc = Join-Path $WorkspaceRoot "tools\avr-toolchain\bin\avr-gcc.exe"
$portableAvrObjcopy = Join-Path $WorkspaceRoot "tools\avr-toolchain\bin\avr-objcopy.exe"

if (Test-Path $portableAvrGcc) {
    $avrGccExe = $portableAvrGcc
    $avrObjcopyExe = $portableAvrObjcopy
    Write-Host "✅ Using portable AVR toolchain" -ForegroundColor Green
}
else {
    Write-Host "❌ PORTABLE SYSTEM ERROR: AVR toolchain not found!" -ForegroundColor Red
    Write-Host "Expected location: $portableAvrGcc" -ForegroundColor Yellow
    Write-Host "This system requires NO external installations - all tools should be included." -ForegroundColor Yellow
    Write-Host "Please ensure the tools/avr-toolchain directory is complete." -ForegroundColor Yellow
    exit 1
}

if ($ProjectDir -like "*\projects\*") {
    $name = Split-Path $ProjectDir -Leaf
    Write-Host "Building: $name"
    Set-Location $ProjectDir
    
    # Standard AVR compiler flags (compatible with avr/io.h)
    $McStudioFlags = "-funsigned-char -funsigned-bitfields -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -mrelax"
    $ExactMcStudioFlags = "-O3"  # EXACT from MC Studio: "Optimize most (-O3)" - NOT -Os!
    $CommonFlags = "-mmcu=atmega128 -DF_CPU=16000000UL -DBAUD=9600 -DDEBUG -Wall -I. -I../../shared_libs"
    
    # UNIFIED BUILD: Generate standard hex file compatible with both hardware and SimulIDE
    
    # Special handling for projects with educational interrupt programming
    if ($name -eq "Serial_interrupt") {
        Write-Host "Building Serial_interrupt for HARDWARE (with ELPM)..." -ForegroundColor Yellow
        & $avrGccExe $CommonFlags.Split(' ') $McStudioFlags.Split(' ') $ExactMcStudioFlags.Split(' ') Main.c ../../shared_libs/_port.c ../../shared_libs/_uart.c ../../shared_libs/_interrupt_manager.c -lm -o Main.elf
    }
    elseif ($name -eq "Serial_Communications") {
        Write-Host "Building Serial_Communications for HARDWARE (with ELPM)..." -ForegroundColor Cyan
        & $avrGccExe $CommonFlags.Split(' ') $McStudioFlags.Split(' ') $ExactMcStudioFlags.Split(' ') Main.c ../../shared_libs/_port.c -lm -o Main.elf
    }
    elseif ($name -eq "Graphics_Display") {
        Write-Host "Building Graphics_Display with GLCD library..." -ForegroundColor Magenta
        # Graphics Display project uses GLCD library
        # Add flag for newer SimulIDE versions (1.1.0+) that need different timing
        $SimulIDENewFlag = "-DSIMULIDE_NEW_VERSION"
        & $avrGccExe $CommonFlags.Split(' ') $McStudioFlags.Split(' ') $ExactMcStudioFlags.Split(' ') $SimulIDENewFlag Main.c `
            ../../shared_libs/_glcd.c `
            ../../shared_libs/_port.c `
            ../../shared_libs/_init.c `
            -lm -o Main.elf
    }
    elseif ($name -eq "Interrupt_Basic") {
        Write-Host "Building Interrupt_Basic for HARDWARE (with ELPM)..." -ForegroundColor Yellow
        & $avrGccExe $CommonFlags.Split(' ') $McStudioFlags.Split(' ') $ExactMcStudioFlags.Split(' ') Main.c ../../shared_libs/_port.c ../../shared_libs/_init.c -lm -o Main.elf
    }
    elseif ($name -eq "Port_Basic") {
        Write-Host "Building Port_Basic for HARDWARE (with ELPM)..." -ForegroundColor Cyan
        & $avrGccExe $CommonFlags.Split(' ') $McStudioFlags.Split(' ') $ExactMcStudioFlags.Split(' ') Main.c -lm -o Main.elf
    }
    else {
        # Standard build for hardware (with ELPM) for all projects
        Write-Host "Building $name for HARDWARE (with ELPM)..." -ForegroundColor Green
        & $avrGccExe $CommonFlags.Split(' ') $McStudioFlags.Split(' ') $ExactMcStudioFlags.Split(' ') Main.c ../../shared_libs/_port.c ../../shared_libs/_init.c ../../shared_libs/_uart.c ../../shared_libs/_glcd.c ../../shared_libs/_timer2.c ../../shared_libs/_adc.c ../../shared_libs/_buzzer.c ../../shared_libs/_eeprom.c -lm -o Main.elf
    }
    
    if ($LASTEXITCODE -eq 0) {
        & $avrObjcopyExe -O ihex -R .eeprom Main.elf Main.hex
        Write-Host "Build completed!" -ForegroundColor Green
        Write-Host "Files: Main.elf, Main.hex" -ForegroundColor Gray
    }
    else {
        Write-Host "Build failed!" -ForegroundColor Red
    }
}
else {
    Write-Host "Error: Not a project folder" -ForegroundColor Red
}
