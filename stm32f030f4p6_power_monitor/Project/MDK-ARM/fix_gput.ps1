$path = '..\..\Drivers\BSP\LCD\lcd.c'
$c = Get-Content -Raw -Encoding UTF8 $path

$old = @'
    else
    {
        for (gy = 0; gy < h; gy++)
        {
            int16_t py = ty + gy;
            if (py < 0) continue;
            for (gx = 0; gx < w; gx++)
            {
                if (!font_px(id, gx, gy)) continue;
                LCD_Fill((uint16_t)(x + (f_ox[id] + (int16_t)gx) * s),
                         (uint16_t)(py * s),
                         (uint16_t)(x + (f_ox[id] + (int16_t)gx + 1) * s),
                         (uint16_t)((py + 1) * s), color);
            }
        }
    }
'@

$new = @'
    else
    {
        int16_t oy = (int16_t)f_oy[id] * (int16_t)s;
        for (gy = 0; gy < h; gy++)
        {
            int16_t py = (int16_t)y + oy + (int16_t)gy * (int16_t)s;
            if (py < 0) continue;
            if (py > (int16_t)(LCD_H - s)) break;
            for (gx = 0; gx < w; gx++)
            {
                if (!font_px(id, gx, gy)) continue;
                LCD_Fill((uint16_t)(x + (f_ox[id] + (int16_t)gx) * s),
                         (uint16_t)py,
                         (uint16_t)(x + (f_ox[id] + (int16_t)gx + 1) * s - 1),
                         (uint16_t)(py + s - 1), color);
            }
        }
    }
'@

if ($c.Contains($old)) {
    $c = $c.Replace($old, $new)
    Set-Content -Encoding UTF8 -Path $path -NoNewline $c
    Write-Output 'PASS'
} else {
    Write-Output 'PATTERN NOT FOUND - checking line endings'
    $lines = $old -split "`n"
    foreach ($l in $lines) {
        if (-not $c.Contains($l.TrimEnd("`r"))) {
            Write-Output "MISSING: $l"
        }
    }
}