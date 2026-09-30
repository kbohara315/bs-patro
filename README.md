# bs-patro — Bikram Sambat (BS) Patro

Bikram Sambat date for the Omarchy top bar. A lightweight BS date pill
(not a full calendar): flag + BS day month year, both calendars in the
hover tooltip.

## Install as a plugin

```bash
omarchy plugin add https://github.com/kbohara315/bs-patro.git --enable
omarchy bar move kshitij.bs-patro --section center
```

One setting: `nepali` (off by default) switches to Nepali month/weekday
names with Devanagari digits. On a vertical bar the pill stacks
flag/day/year into the 28px slot automatically.

## Standalone C version

`bs-patro.c` is the original zero-dependency implementation (libc only,
~16KB) for `type: command` bar modules or terminals — the QML plugin
above is a faithful port of it, and `Model.js` is unit-tested
(`node Model.js`).

```bash
gcc -Os -s -o bs-patro bs-patro.c  # libc only, ~16KB
cp -f bs-patro ~/.config/omarchy/bar/scripts/bs-patro
```

- `bs-patro` — English month names, arabic digits
- `bs-patro --np` — Nepali month/weekday names, Devanagari digits
- `bs-patro --text-only` — plain text instead of JSON (for terminals)

BS month-length table vendored from nepali-date-converter (MIT,
subeshb1/Nepali-Date). Valid BS 2000–2090.

## Remove

```bash
omarchy plugin remove kshitij.bs-patro
```

## Dependencies

The plugin itself: none — pure QML/JS, everything Omarchy ships. The
standalone C version needs `gcc` to build (libc only).
