<#
Simple TCP test client for Serial_Communications demo
Connects to localhost:9001 and allows interactive send/receive
#>

param(
    [string]$HostName = '127.0.0.1',
    [int]$Port = 9001
)

Write-Host "Connecting to $HostName:$Port..."

$tcp = New-Object System.Net.Sockets.TcpClient
$tcp.Connect($HostName,$Port)
$stream = $tcp.GetStream()
$reader = New-Object System.IO.StreamReader($stream)
$writer = New-Object System.IO.StreamWriter($stream)
$writer.AutoFlush = $true

Write-Host "Connected. Type messages to send. Type '\\quit' to exit." -ForegroundColor Green

# Start background job to read incoming data
$script:stopFlag = $false
$null = Start-Job -ScriptBlock {
    param($stream)
    $reader = New-Object System.IO.StreamReader($stream)
    while (-not $script:stopFlag) {
        try {
            Start-Sleep -Milliseconds 100
            if ($stream.DataAvailable) {
                $s = $reader.ReadLine()
                if ($s -ne $null) { Write-Host "<< $s" -ForegroundColor Cyan }
            }
        } catch {
            # ignore read errors
        }
    }
} -ArgumentList $stream

while ($true) {
    $line = Read-Host -Prompt "send"
    if ($line -eq "\\quit") { break }
    $writer.WriteLine($line)
}

# Cleanup
$script:stopFlag = $true
Stop-Job -State Running | Remove-Job -Force
$reader.Close(); $writer.Close(); $tcp.Close()
Write-Host "Disconnected." -ForegroundColor Yellow
