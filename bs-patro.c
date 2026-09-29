// bs-patro: Bikram Sambat (BS) Patro — Bikram Sambat date for the Omarchy top bar.
//
// Prints Waybar-style JSON consumed by an Omarchy `type: command` bar module:
//     {"text":"...","tooltip":"..."}
// Text is the Nepal flag + BS date, e.g. "🇳🇵 11 Ashwin 2083".
// On a vertical bar (position left/right in ~/.config/omarchy/shell.json)
// the 28px slot fits only short lines, so the month is dropped from display:
//     🇳🇵\n11\n2083    (flag, day, year — month lives in the hover tooltip)
// Tooltip carries both calendars for hover.
//
// Usage:
//     bs-patro            # English month names, arabic digits
//     bs-patro --np       # Nepali month/weekday names, Devanagari digits
//     bs-patro --text-only  # plain text instead of JSON (for terminals)
//
// BS month-length table vendored from bs-patro-converter (MIT,
// subeshb1/Nepali-Date): BS 2000-01-01 == AD 1943-04-14. Valid BS 2000-2090.
//
// Build: gcc -Os -s -o bs-patro bs-patro.c  (libc only, ~16KB)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// bs_table[year-2000][month] = days in month. Index 0 == BS 2000.
static const unsigned char bs_table[91][12] = {
    {30,32,31,32,31,30,30,30,29,30,29,31},
    {31,31,32,31,31,31,30,29,30,29,30,30},
    {31,31,32,32,31,30,30,29,30,29,30,30},
    {31,32,31,32,31,30,30,30,29,29,30,31},
    {30,32,31,32,31,30,30,30,29,30,29,31},
    {31,31,32,31,31,31,30,29,30,29,30,30},
    {31,31,32,32,31,30,30,29,30,29,30,30},
    {31,32,31,32,31,30,30,30,29,29,30,31},
    {31,31,31,32,31,31,29,30,30,29,29,31},
    {31,31,32,31,31,31,30,29,30,29,30,30},
    {31,31,32,32,31,30,30,29,30,29,30,30},
    {31,32,31,32,31,30,30,30,29,29,30,31},
    {31,31,31,32,31,31,29,30,30,29,30,30},
    {31,31,32,31,31,31,30,29,30,29,30,30},
    {31,31,32,32,31,30,30,29,30,29,30,30},
    {31,32,31,32,31,30,30,30,29,29,30,31},
    {31,31,31,32,31,31,29,30,30,29,30,30},
    {31,31,32,31,31,31,30,29,30,29,30,30},
    {31,32,31,32,31,30,30,29,30,29,30,30},
    {31,32,31,32,31,30,30,30,29,30,29,31},
    {31,31,31,32,31,31,30,29,30,29,30,30},
    {31,31,32,31,31,31,30,29,30,29,30,30},
    {31,32,31,32,31,30,30,30,29,29,30,30},
    {31,32,31,32,31,30,30,30,29,30,29,31},
    {31,31,31,32,31,31,30,29,30,29,30,30},
    {31,31,32,31,31,31,30,29,30,29,30,30},
    {31,32,31,32,31,30,30,30,29,29,30,31},
    {30,32,31,32,31,30,30,30,29,30,29,31},
    {31,31,32,31,31,31,30,29,30,29,30,30},
    {31,31,32,31,32,30,30,29,30,29,30,30},
    {31,32,31,32,31,30,30,30,29,29,30,31},
    {30,32,31,32,31,30,30,30,29,30,29,31},
    {31,31,32,31,31,31,30,29,30,29,30,30},
    {31,31,32,32,31,30,30,29,30,29,30,30},
    {31,32,31,32,31,30,30,30,29,29,30,31},
    {30,32,31,32,31,31,29,30,30,29,29,31},
    {31,31,32,31,31,31,30,29,30,29,30,30},
    {31,31,32,32,31,30,30,29,30,29,30,30},
    {31,32,31,32,31,30,30,30,29,29,30,31},
    {31,31,31,32,31,31,29,30,30,29,30,30},
    {31,31,32,31,31,31,30,29,30,29,30,30},
    {31,31,32,32,31,30,30,29,30,29,30,30},
    {31,32,31,32,31,30,30,30,29,29,30,31},
    {31,31,31,32,31,31,29,30,30,29,30,30},
    {31,31,32,31,31,31,30,29,30,29,30,30},
    {31,32,31,32,31,30,30,29,30,29,30,30},
    {31,32,31,32,31,30,30,30,29,29,30,31},
    {31,31,31,32,31,31,30,29,30,29,30,30},
    {31,31,32,31,31,31,30,29,30,29,30,30},
    {31,32,31,32,31,30,30,30,29,29,30,30},
    {31,32,31,32,31,30,30,30,29,30,29,31},
    {31,31,31,32,31,31,30,29,30,29,30,30},
    {31,31,32,31,31,31,30,29,30,29,30,30},
    {31,32,31,32,31,30,30,30,29,29,30,30},
    {31,32,31,32,31,30,30,30,29,30,29,31},
    {31,31,32,31,31,31,30,29,30,29,30,30},
    {31,31,32,31,32,30,30,29,30,29,30,30},
    {31,32,31,32,31,30,30,30,29,29,30,31},
    {30,32,31,32,31,30,30,30,29,30,29,31},
    {31,31,32,31,31,31,30,29,30,29,30,30},
    {31,31,32,32,31,30,30,29,30,29,30,30},
    {31,32,31,32,31,30,30,30,29,29,30,31},
    {30,32,31,32,31,31,29,30,29,30,29,31},
    {31,31,32,31,31,31,30,29,30,29,30,30},
    {31,31,32,32,31,30,30,29,30,29,30,30},
    {31,32,31,32,31,30,30,30,29,29,30,31},
    {31,31,31,32,31,31,29,30,30,29,29,31},
    {31,31,32,31,31,31,30,29,30,29,30,30},
    {31,31,32,32,31,30,30,29,30,29,30,30},
    {31,32,31,32,31,30,30,30,29,29,30,31},
    {31,31,31,32,31,31,29,30,30,29,30,30},
    {31,31,32,31,31,31,30,29,30,29,30,30},
    {31,32,31,32,31,30,30,29,30,29,30,30},
    {31,32,31,32,31,30,30,30,29,29,30,31},
    {31,31,31,32,31,31,30,29,30,29,30,30},
    {31,31,32,31,31,31,30,29,30,29,30,30},
    {31,32,31,32,31,30,30,30,29,29,30,30},
    {31,32,31,32,31,30,30,30,29,30,29,31},
    {31,31,31,32,31,31,30,29,30,29,30,30},
    {31,31,32,31,31,31,30,29,30,29,30,30},
    {31,32,31,32,31,30,30,30,29,29,30,30},
    {31,32,31,32,31,30,30,30,29,30,29,31},
    {31,31,32,31,31,31,30,29,30,29,30,30},
    {31,31,32,31,31,31,30,29,30,29,30,30},
    {31,32,31,32,31,30,30,30,29,29,30,31},
    {30,32,31,32,31,30,30,30,29,30,29,31},
    {31,31,32,31,31,31,30,29,30,29,30,30},
    {31,31,32,31,31,31,30,30,29,30,30,30},
    {30,31,32,32,30,31,30,30,29,30,30,30},
    {30,32,31,32,31,30,30,30,29,30,30,30},
    {30,32,31,32,31,30,30,30,29,30,30,30},
};

static const char *months_en[12] = {
    "Baisakh","Jestha","Asar","Shrawan","Bhadra","Ashwin",
    "Kartik","Mangsir","Poush","Magh","Falgun","Chaitra",
};
static const char *months_np[12] = {
    "बैशाख","जेठ","असार","श्रावण","भाद्र","आश्विन",
    "कार्तिक","मंसिर","पौष","माघ","फाल्गुण","चैत्र",
};
static const char *weekdays_en[7] = {
    "Monday","Tuesday","Wednesday","Thursday","Friday","Saturday","Sunday",
};
static const char *weekdays_np[7] = {
    "सोमबार","मंगलबार","बुधबार","बिहिबार","शुक्रबार","शनिबार","आइतबार",
};
static const char *gmonths_en[12] = {
    "January","February","March","April","May","June",
    "July","August","September","October","November","December",
};
static const char *gmonths_abbr[12] = {
    "Jan","Feb","Mar","Apr","May","Jun",
    "Jul","Aug","Sep","Oct","Nov","Dec",
};

// Days since civil 1970-01-01 (H. Hinnant's algorithm). Timezone-free,
// so no DST/localtime pitfalls in the date arithmetic.
static long days_from_civil(long y, unsigned m, unsigned d) {
    y -= m <= 2;
    const long era = (y >= 0 ? y : y - 399) / 400;
    const unsigned yoe = (unsigned)(y - era * 400);
    const unsigned doy = (153 * (m + (m > 2 ? -3 : 9)) + 2) / 5 + d - 1;
    const unsigned doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;
    return era * 146097 + (long)doe - 719468;
}

// Append src to dst (bounded). Returns 0 on truncation.
static int append(char *dst, size_t cap, size_t *len, const char *src) {
    size_t n = strlen(src);
    if (*len + n + 1 > cap) return 0;
    memcpy(dst + *len, src, n + 1);
    *len += n;
    return 1;
}

// Append int as decimal, or Devanagari digits when np != 0.
// U+0966 + d encodes as E0 A5 (A6+d) in UTF-8.
static int append_num(char *dst, size_t cap, size_t *len, int v, int np) {
    char buf[32];
    if (!np) {
        snprintf(buf, sizeof buf, "%d", v);
        return append(dst, cap, len, buf);
    }
    char *p = buf;
    char tmp[16];
    int n = snprintf(tmp, sizeof tmp, "%d", v);
    for (int i = 0; i < n; i++) {
        *p++ = (char)0xE0;
        *p++ = (char)0xA5;
        *p++ = (char)(0xA6 + (tmp[i] - '0'));
    }
    *p = '\0';
    return append(dst, cap, len, buf);
}

// Minimal JSON string escaper: handles " \ control chars + \n.
static void json_print(const char *s) {
    putchar('"');
    for (; *s; s++) {
        unsigned char c = (unsigned char)*s;
        switch (c) {
        case '"': fputs("\\\"", stdout); break;
        case '\\': fputs("\\\\", stdout); break;
        case '\n': fputs("\\n", stdout); break;
        default:
            if (c < 0x20) printf("\\u%04x", c);
            else putchar(c);
        }
    }
    putchar('"');
}

// 1 when the Omarchy bar is vertical (position left/right), else 0.
// Read from ~/.config/omarchy/shell.json with a minimal "position" scan —
// no JSON parser needed for a single known key. Missing/unparseable file
// means horizontal, matching the bar's own default.
static int bar_is_vertical(void) {
    const char *home = getenv("HOME");
    if (!home || !*home) return 0;
    char path[512];
    snprintf(path, sizeof path, "%s/.config/omarchy/shell.json", home);
    FILE *f = fopen(path, "r");
    if (!f) return 0;
    char buf[8192];
    size_t n = fread(buf, 1, sizeof buf - 1, f);
    fclose(f);
    buf[n] = '\0';

    const char *p = strstr(buf, "\"position\"");
    if (!p) return 0;
    p = strchr(p, ':');
    if (!p) return 0;
    p++;
    while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r') p++;
    if (*p != '"') return 0;
    p++;
    if (!strncmp(p, "left\"", 5) || !strncmp(p, "right\"", 6)) return 1;
    return 0;
}

int main(int argc, char **argv) {
    int use_np = 0, text_only = 0;
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "--np") || !strcmp(argv[i], "-np")) use_np = 1;
        else if (!strcmp(argv[i], "--text-only")) text_only = 1;
    }

    time_t now_t = time(NULL);
    struct tm now = *localtime(&now_t);
    int gy = now.tm_year + 1900, gm = (unsigned)now.tm_mon + 1, gd = (unsigned)now.tm_mday;

    // A vertical bar slot is sizeVertical wide (28px) with no text wrapping,
    // and the flag emoji renders double-height, so only very short lines fit.
    // Vertical: "🇳🇵\n13\n2083" (flag, day, year) — month moves to the hover
    // tooltip, which already carries the full date. Horizontal unchanged.
    int vert = bar_is_vertical();
    const char *sep = vert ? "\n" : " ";

    // 1943-04-14 (Wednesday) == BS 2000-01-01. tm_wday: Sunday==0 -> Monday==0.
    long delta = days_from_civil(gy, (unsigned)gm, (unsigned)gd)
               - days_from_civil(1943, 4, 14);
    int wd = (int)((2 + delta) % 7); // Monday==0 anchor: 1943-04-14 was Wednesday
    if (wd < 0) wd += 7;

    char text[256] = {0}, tip[512] = {0};
    size_t tl = 0, pl = 0;

    if (delta < 0) {
        goto out_of_range;
    } else {
        long passed = delta + 1; // day 1 == epoch
        int bs_y = -1, bs_m = -1;
        long bs_d = 0;
        for (int yi = 0; yi < 91 && bs_y < 0; yi++) {
            long year_days = 0;
            for (int mi = 0; mi < 12; mi++) year_days += bs_table[yi][mi];
            if (passed <= year_days) {
                for (int mi = 0; mi < 12; mi++) {
                    if (passed <= bs_table[yi][mi]) {
                        bs_y = 2000 + yi; bs_m = mi; bs_d = passed;
                        break;
                    }
                    passed -= bs_table[yi][mi];
                }
            } else {
                passed -= year_days;
            }
        }
        if (bs_y < 0) goto out_of_range;

        char gfull[96];
        snprintf(gfull, sizeof gfull, "%s, %d %s %d",
                 weekdays_en[(now.tm_wday + 6) % 7], gd, gmonths_en[now.tm_mon], gy);

        append(text, sizeof text, &tl, "🇳🇵");
        append(text, sizeof text, &tl, sep);
        append_num(text, sizeof text, &tl, (int)bs_d, use_np);
        if (!vert) {
            append(text, sizeof text, &tl, sep);
            append(text, sizeof text, &tl, use_np ? months_np[bs_m] : months_en[bs_m]);
        }
        append(text, sizeof text, &tl, sep);
        append_num(text, sizeof text, &tl, bs_y, use_np);

        // Vertical's compact text drops the month, so the tooltip leads with
        // the full compact date line before the two-calendar lines below.
        if (vert) {
            char full[96];
            snprintf(full, sizeof full, "🇳🇵 %ld %s %d",
                     bs_d, months_en[bs_m], bs_y);
            append(tip, sizeof tip, &pl, full);
            append(tip, sizeof tip, &pl, "\n");
        }

        if (use_np) {
            char nb[64] = {0}, ny[64] = {0};
            size_t nl = 0, yl = 0;
            append_num(nb, sizeof nb, &nl, (int)bs_d, 1);
            append_num(ny, sizeof ny, &yl, bs_y, 1);
            append(tip, sizeof tip, &pl, "वि.सं.: ");
            append(tip, sizeof tip, &pl, weekdays_np[wd]);
            append(tip, sizeof tip, &pl, ", ");
            append(tip, sizeof tip, &pl, nb);
            append(tip, sizeof tip, &pl, " ");
            append(tip, sizeof tip, &pl, months_np[bs_m]);
            append(tip, sizeof tip, &pl, " ");
            append(tip, sizeof tip, &pl, ny);
            append(tip, sizeof tip, &pl, "\nAD: ");
            append(tip, sizeof tip, &pl, gfull);
        } else {
            char bsline[128];
            snprintf(bsline, sizeof bsline, "Bikram Sambat: %s, %ld %s %d",
                     weekdays_en[wd], bs_d, months_en[bs_m], bs_y);
            append(tip, sizeof tip, &pl, bsline);
            append(tip, sizeof tip, &pl, "\nGregorian: ");
            append(tip, sizeof tip, &pl, gfull);
        }
        goto emit;
    }

out_of_range: {
    char day[16], year[16];
    snprintf(day, sizeof day, "%d", gd);
    snprintf(year, sizeof year, "%d", gy);
    append(text, sizeof text, &tl, "🇳🇵");
    append(text, sizeof text, &tl, sep);
    append(text, sizeof text, &tl, day);
    if (!vert) {
        append(text, sizeof text, &tl, sep);
        append(text, sizeof text, &tl, gmonths_abbr[now.tm_mon]);
    }
    append(text, sizeof text, &tl, sep);
    append(text, sizeof text, &tl, year);
    append(tip, sizeof tip, &pl, "BS date out of range (2000-2090)");
}

emit:
    if (text_only) {
        puts(text);
        return 0;
    }
    fputs("{\"text\":", stdout);
    json_print(text);
    fputs(",\"tooltip\":", stdout);
    json_print(tip);
    puts("}");
    return 0;
}
