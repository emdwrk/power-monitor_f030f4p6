$path = '..\..\Drivers\BSP\LCD\lcd.c'
$c = Get-Content -Raw -Encoding UTF8 $path

$old = 'while (*p)
        uint32_t c = u8(&p);
        if (c == '"'"' '"'"') { a += (uint16_t)(s * 4); continue; }
        {
            uint16_t id = font_find(c);
            a += (uint16_t)((f_w[id] + 1) * s);
        }'

$new = 'while (*p)
    {
        uint32_t c = u8(&p);
        if (c == '"'"' '"'"') { a += (uint16_t)(s * 4); continue; }
        {
            uint16_t id = font_find(c);
            a += (uint16_t)((f_w[id] + 1) * s);
        }
    }'

if ($c.Contains($old)) {
    $c = $c.Replace($old, $new)
    Set-Content -Encoding UTF8 -Path $path -NoNewline $c
    Write-Output 'OK'
} else {
    Write-Output 'FAIL'
    # Try alternative - just add the brace
    $lines = $c -split "`r`n"
    for ($i = 0; $i -lt $lines.Count; $i++) {
        if ($lines[$i] -match 'while \(\*p\)' -and $i+1 -lt $lines.Count) {
            $next = $lines[$i+1]
            Write-Output "Line $i : $($lines[$i])"
            Write-Output "Line $($i+1) : $next"
        }
    }
}