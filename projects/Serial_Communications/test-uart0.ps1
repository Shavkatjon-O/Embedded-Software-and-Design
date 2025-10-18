#!/usr/bin/env pwsh
<#
.SYNOPSIS
    Test UART0 in SimulIDE 1.1.0
    
.DESCRIPTION
    Quick test to see if UART0 works when UART1 is broken
#>

$ErrorActionPreference = "Stop"

Write-Host "`n========================================" -ForegroundColor Cyan
Write-Host "  UART0 Test for SimulIDE 1.1.0" -ForegroundColor Cyan
Write-Host "========================================`n" -ForegroundColor Cyan

$WorkspaceRoot = "W:\soc3050code"
$ProjectDir = Join-Path $WorkspaceRoot "projects\Serial_Communications"
$ToolchainDir = Join-Path $WorkspaceRoot "tools\avr-toolchain\bin"

# Backup original Main.c
Write-Host "[BACKUP] Backing up original Main.c..." -ForegroundColor Yellow
Copy-Item (Join-Path $ProjectDir "Main.c") (Join-Path $ProjectDir "Main_UART1_original.c.backup") -Force

# Replace with UART0 test
Write-Host "[SETUP] Copying UART0 test code..." -ForegroundColor Cyan
Copy-Item (Join-Path $ProjectDir "Main_UART0_test.c") (Join-Path $ProjectDir "Main.c") -Force

# Build
Write-Host "`n[BUILD] Compiling UART0 test..." -ForegroundColor Cyan
cd $ProjectDir

& "$ToolchainDir\avr-gcc.exe" -mmcu=atmega128 -DF_CPU=16000000UL -Os -o Main.elf Main.c
if ($LASTEXITCODE -ne 0) {
    Write-Host "[ERROR] Compilation failed!" -ForegroundColor Red
    # Restore original
    Move-Item (Join-Path $ProjectDir "Main_UART1_original.c.backup") (Join-Path $ProjectDir "Main.c") -Force
    exit 1
}

& "$ToolchainDir\avr-objcopy.exe" -O ihex Main.elf Main.hex
if ($LASTEXITCODE -ne 0) {
    Write-Host "[ERROR] HEX generation failed!" -ForegroundColor Red
    # Restore original
    Move-Item (Join-Path $ProjectDir "Main_UART1_original.c.backup") (Join-Path $ProjectDir "Main.c") -Force
    exit 1
}

Write-Host "[OK] UART0 test compiled successfully!" -ForegroundColor Green

# Launch SimulIDE 1.1.0
Write-Host "`n[LAUNCH] Starting SimulIDE 1.1.0 with UART0 configuration..." -ForegroundColor Cyan
Write-Host "[INFO] Circuit configured for:" -ForegroundColor Yellow
Write-Host "  - Mcu_Uart=0 (UART0)" -ForegroundColor White
Write-Host "  - Pins: PORTE0/PORTE1" -ForegroundColor White
Write-Host "" -ForegroundColor White
Write-Host "[TEST] Look for these messages in SerialTerm:" -ForegroundColor Yellow
Write-Host "  - 'UART0 TEST - SimulIDE 1.1.0'" -ForegroundColor White
Write-Host "  - 'If you see this message, UART0 TX works!'" -ForegroundColor White
Write-Host "  - 'Hello from UART0!'" -ForegroundColor White
Write-Host "  - Counter values" -ForegroundColor White
Write-Host ""

# Close any running SimulIDE
Stop-Process -Name "simulide" -Force -ErrorAction SilentlyContinue
Start-Sleep -Seconds 1

# Launch
& "$WorkspaceRoot\tools\simulide\cli-simulide.ps1" -ProjectDir $ProjectDir -BuildFirst $false

Write-Host "`n[REMINDER] After testing:" -ForegroundColor Yellow
Write-Host "  Run this script again to restore original code:" -ForegroundColor White
Write-Host "  .\restore-uart1.ps1" -ForegroundColor Cyan
Write-Host ""

exit 0
