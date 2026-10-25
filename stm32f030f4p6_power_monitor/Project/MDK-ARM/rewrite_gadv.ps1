$path = '..\..\Drivers\BSP\LCD\lcd.c'
$c = Get-Content -Raw -Encoding UTF8 $path

$start = $c.IndexOf('static uint16_t gadvance')
$end   = $c.IndexOf('static uint16_t gputs', $start)

if ($start -lt 0 -or $end -lt 0) {
    Write-Output "MARKERS NOT FOUND start=$start end=$end"
    exit 1
}

$newfn = @'
static uint16_t gadvance(const char *str, uint8_t s)
{
    const char *p = str;
    uint16_t a = 0;
    while (*p)
    {
        uint32_t c = u8(&p);
        if (c == ' ') { a += (uint16_t)(s * 4); continue; }
        {
            uint16_t id = font_find(c);
            a += (uint16_t)((f_w[id] + 1) * s);
        }
    }
    return a;
}

'@

$c = $c.Substring(0, $start) + $newfn + $c.Substring($end)
Set-Content -Encoding UTF8 -Path $path -NoNewline $c
Write-Output 'OK'