$path = '..\..\Drivers\BSP\LCD\lcd.c'
$c = Get-Content -Raw -Encoding UTF8 $path
$start = $c.IndexOf('void LCD_ShowIUW(float I, float U, float W,')
$end   = $c.IndexOf('/* ================= low-level hardware layer ================= */')
if ($start -lt 0) { Write-Output 'MARK START NOT FOUND'; exit 1 }
if ($end   -lt 0) { Write-Output 'MARK END NOT FOUND'; exit 1 }

$newfn = @'
void LCD_ShowIUW(float I, float U, float W,
                 float Imax, float Umax, float Wavg)
{
    static uint8_t  first   = 1;
    static uint16_t fcnt    = 0;
    static uint8_t  prev_ci = 0xFF;
    static uint16_t last_val_w[3];
    static uint16_t last_sub_w[3];

    const uint16_t card_w = LCD_W - 12;
    const uint16_t card_x = 6;
    const uint16_t card_h = 90;
    const uint16_t bar_w  = 5;
    const uint16_t cy[3]  = { 18, 118, 218 };

    /* wider cycling palettes per channel (8 colours) */
    static const uint16_t av[8] = { ACCENT_V, 0xFD00, ACCENT_V2, ORANGE,  LIME,  0x07E0, 0x04FF, 0xFFFF };
    static const uint16_t aa[8] = { ACCENT_A, 0x04FF, ACCENT_A2, 0x06BF, 0x20E0, 0xFC60, 0x04DF, 0xFFE0 };
    static const uint16_t aw[8] = { ACCENT_W, 0x97C0, ACCENT_W2, LIME,   0x07E0, 0xFD80, 0x06BF, 0xFFFF };
    const uint16_t *accent[3] = { av, aa, aw };
    uint8_t ci = (uint8_t)((fcnt++ / 6) & 0x07);

    if (first)
    {
        uint16_t i;
        LCD_FillGradBuf(0, 0, LCD_W, LCD_H, BG_DARK, BG_MID, 8);

        LCD_Fill(0, 0, LCD_W, 18, HEADER_BG);
        {
            const char *title = "\xE5\x8A\x9F\xE7\x8E\x87\xE8\xAE\xA1";
            uint16_t tw = gadvance(title, 2);
            gputs((LCD_W - tw) / 2, 3, title, 2, WHITE);
        }

        for (i = 0; i < 3; i++)
        {
            const char *label = (i == 0) ? "\xE7\x94\xB5\xE5\x8E\x8B"
                             : (i == 1) ? "\xE7\x94\xB5\xE6\xB5\x81"
                                       : "\xE5\x8A\x9F\xE7\x8E\x87";
            const char *unit = (i == 0) ? "V" : (i == 1) ? "A" : "W";
            uint16_t y  = cy[i];
            uint16_t ac = accent[i][0];

            LCD_Fill(card_x + 3, (uint16_t)(y + 3), (uint16_t)(card_x + card_w + 3),
                     (uint16_t)(y + card_h + 3), CARD_SHAD);
            LCD_Fill(card_x, y, (uint16_t)(card_x + card_w), (uint16_t)(y + card_h), CARD_BG);
            LCD_Fill(card_x, y, (uint16_t)(card_x + card_w), (uint16_t)(y + 1), ac);
            LCD_Fill(card_x, (uint16_t)(y + 1), (uint16_t)(card_x + bar_w),
                     (uint16_t)(y + card_h), ac);

            gputs((uint16_t)(card_x + bar_w + 5), (uint16_t)(y + 3), label, 1, TEXT_DIM);
            {
                uint16_t uw = gadvance(unit, 1);
                gputs((uint16_t)(card_x + card_w - uw - 4), (uint16_t)(y + 3), unit, 1, TEXT_DIM);
            }
        }
        first = 0;
    }

    if (ci != prev_ci)
    {
        uint8_t i;
        prev_ci = ci;
        for (i = 0; i < 3; i++)
        {
            uint16_t y  = cy[i];
            uint16_t ac = accent[i][ci];
            LCD_Fill(card_x, (uint16_t)(y + 1), (uint16_t)(card_x + bar_w),
                     (uint16_t)(y + card_h), ac);
            LCD_Fill(card_x, y, (uint16_t)(card_x + card_w), (uint16_t)(y + 1), ac);
        }
    }

    {
        uint8_t i;
        for (i = 0; i < 3; i++)
        {
            float    v   = (i == 0) ? U : (i == 1) ? I : W;
            uint16_t y   = cy[i];
            uint16_t ac  = accent[i][ci];
            char     buf[12];
            uint16_t nw;
            uint8_t  n = 0;
            long     ip;
            long     ipart, fpart;
            char     st[8];
            uint8_t  sn = 0;

            ip = (long)(v * 100.0f + (v >= 0 ? 0.5f : -0.5f));
            if (ip < 0) { buf[n++] = '-'; ip = -ip; }
            ipart = ip / 100;
            fpart = ip % 100;
            do { st[sn++] = (char)('0' + (ipart % 10)); ipart /= 10; } while (ipart > 0);
            while (sn) buf[n++] = st[--sn];
            buf[n++] = '.';
            buf[n++] = (char)('0' + (fpart / 10));
            buf[n++] = (char)('0' + (fpart % 10));
            buf[n] = '\0';
            nw = gadvance(buf, 2);

            {
                uint16_t old_w = last_val_w[i];
                uint16_t max_w = old_w > nw ? old_w : nw;
                if (max_w)
                {
                    uint16_t clr_x = (uint16_t)(card_x + bar_w + (card_w - bar_w - max_w) / 2);
                    LCD_Fill(clr_x, (uint16_t)(y + 30), (uint16_t)(clr_x + max_w),
                             (uint16_t)(y + 64), CARD_BG);
                }
                last_val_w[i] = nw;
            }

            {
                uint16_t txt_w = nw;
                uint16_t x = (uint16_t)(card_x + bar_w + (card_w - bar_w - txt_w) / 2);
                gputs((uint16_t)(x + 2), (uint16_t)(y + 32), buf, 2, CARD_SHAD);
                gputs(x, (uint16_t)(y + 30), buf, 2, ac);
            }

            {
                const char *stag = (i == 2) ? "AVG " : "MAX ";
                float     sv    = (i == 2) ? Wavg : (i == 0) ? Umax : Imax;
                char      sunit = (i == 0) ? 'V' : (i == 1) ? 'A' : 'W';
                char     sbuf[16];
                uint8_t  k = 0;
                long     sip, sipart, sfpart;
                char     stmp[8];
                uint8_t  stn = 0;
                uint16_t sub_w, clear_w;
                uint16_t sx = (uint16_t)(card_x + bar_w + 4);
                uint16_t sy = (uint16_t)(y + 67);

                while (*stag) sbuf[k++] = *stag++;
                sip = (long)(sv * 100.0f + (sv >= 0 ? 0.5f : -0.5f));
                if (sip < 0) { sbuf[k++] = '-'; sip = -sip; }
                sipart = sip / 100;
                sfpart = sip % 100;
                do { stmp[stn++] = (char)('0' + (sipart % 10)); sipart /= 10; } while (sipart > 0);
                while (stn) sbuf[k++] = stmp[--stn];
                sbuf[k++] = '.';
                sbuf[k++] = (char)('0' + (sfpart / 10));
                sbuf[k++] = (char)('0' + (sfpart % 10));
                sbuf[k++] = sunit;
                sbuf[k] = '\0';

                sub_w = gadvance(sbuf, 1);
                clear_w = (uint16_t)(last_sub_w[i] > sub_w ? last_sub_w[i] : sub_w);
                last_sub_w[i] = sub_w;
                LCD_Fill(sx, (uint16_t)(sy - 2), (uint16_t)(sx + clear_w + 2),
                         (uint16_t)(sy + 13), CARD_BG);
                gputs(sx, sy, sbuf, 1, TEXT_MID);
            }
        }
    }
}


'@

$c = $c.Substring(0, $start) + $newfn + $c.Substring($end)
Set-Content -Encoding UTF8 -Path $path -NoNewline $c
Write-Output 'REPLACE OK'