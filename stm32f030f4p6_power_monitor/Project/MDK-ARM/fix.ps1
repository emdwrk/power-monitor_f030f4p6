$path = (Resolve-Path '..\..\Drivers\BSP\ADC\adc.c').Path
$bytes = [System.IO.File]::ReadAllBytes($path)
$txt = [System.Text.Encoding]::UTF8.GetString($bytes)
$txt = $txt.Replace('uint8_t  i;', 'uint16_t i;')
[System.IO.File]::WriteAllText($path, $txt)
Write-Output 'FIXED'