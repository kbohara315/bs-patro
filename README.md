# bs-patro — Bikram Sambat (BS) Patro

Bikram Sambat date for the Omarchy top bar. A lightweight BS date pill
(not a full calendar). Prints Waybar-style JSON
consumed by a `type: command` bar module (`{"text":"...","tooltip":"..."}`).

- `bs-patro` — English month names, arabic digits
- `bs-patro --np` — Nepali month/weekday names, Devanagari digits
- `bs-patro --text-only` — plain text instead of JSON (for terminals)

## Build

```bash
gcc -Os -s -o bs-patro bs-patro.c  # libc only, ~16KB
```

## Deploy

The bar module in `~/.config/omarchy/shell.json` runs it from a fixed path,
so copy the built binary there (source of truth stays here):

```bash
gcc -Os -s -o bs-patro bs-patro.c
cp -f bs-patro ~/.config/omarchy/bar/scripts/bs-patro
```

BS month-length table vendored from bs-patro-converter (MIT,
subeshb1/Nepali-Date). Valid BS 2000–2090.
