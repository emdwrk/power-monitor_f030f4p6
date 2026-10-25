$path = '..\..\Drivers\BSP\LCD\lcd.c'
$c = Get-Content -Raw -Encoding UTF8 $path

$old = "        a += gput(0, 0, u8(&p), s, 0x0000);"
$new = @'
        uint32_t c = u8(&p);
        if (c == ' ') { a += (uint16_t)(s * 4); continue; }
        {
            uint16_t id = font_find(c);
            a += (uint16_t)((f_w[id] + 1) * s);
        }
'@

if ($c.Contains($old)) {
    $c = $c.Replace($old, $new)
    Set-Content -Encoding UTF8 -Path $path -NoNewline $c
    Write-Output 'OK'
} else {
    Write-Output "NOTFOUND. Checking bytes..."
    $idx = $c.IndexOf('gput(0, 0');
    if ($idx -ge 0) {
        Write-Output "Found gput(0,0 at offset $idx"
        $snippet = $c.Substring([Math]::Max(0,$idx-20), 100)
        Write-Output "Context: [$snippet]"
    }
}