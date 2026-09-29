// bs-patro Model.js — pure Bikram Sambat conversion, no Qt or Quickshell
// imports, so it stays unit-testable under plain node:
//   node bs-patro/Model.js
//
// Faithful port of bs-patro.c: same epoch anchor (BS 2000-01-01 ==
// AD 1943-04-14, a Wednesday), same table walk, same output strings.

function BsPatro() {
    // BS month lengths, BS 2000-2090. Vendored from nepali-date-converter
    // (MIT, subeshb1/Nepali-Date): row 0 == BS 2000, BS 2000-01-01 == AD 1943-04-14.
    var BS_TABLE = [
        [30,32,31,32,31,30,30,30,29,30,29,31],
        [31,31,32,31,31,31,30,29,30,29,30,30],
        [31,31,32,32,31,30,30,29,30,29,30,30],
        [31,32,31,32,31,30,30,30,29,29,30,31],
        [30,32,31,32,31,30,30,30,29,30,29,31],
        [31,31,32,31,31,31,30,29,30,29,30,30],
        [31,31,32,32,31,30,30,29,30,29,30,30],
        [31,32,31,32,31,30,30,30,29,29,30,31],
        [31,31,31,32,31,31,29,30,30,29,29,31],
        [31,31,32,31,31,31,30,29,30,29,30,30],
        [31,31,32,32,31,30,30,29,30,29,30,30],
        [31,32,31,32,31,30,30,30,29,29,30,31],
        [31,31,31,32,31,31,29,30,30,29,30,30],
        [31,31,32,31,31,31,30,29,30,29,30,30],
        [31,31,32,32,31,30,30,29,30,29,30,30],
        [31,32,31,32,31,30,30,30,29,29,30,31],
        [31,31,31,32,31,31,29,30,30,29,30,30],
        [31,31,32,31,31,31,30,29,30,29,30,30],
        [31,32,31,32,31,30,30,29,30,29,30,30],
        [31,32,31,32,31,30,30,30,29,30,29,31],
        [31,31,31,32,31,31,30,29,30,29,30,30],
        [31,31,32,31,31,31,30,29,30,29,30,30],
        [31,32,31,32,31,30,30,30,29,29,30,30],
        [31,32,31,32,31,30,30,30,29,30,29,31],
        [31,31,31,32,31,31,30,29,30,29,30,30],
        [31,31,32,31,31,31,30,29,30,29,30,30],
        [31,32,31,32,31,30,30,30,29,29,30,31],
        [30,32,31,32,31,30,30,30,29,30,29,31],
        [31,31,32,31,31,31,30,29,30,29,30,30],
        [31,31,32,31,32,30,30,29,30,29,30,30],
        [31,32,31,32,31,30,30,30,29,29,30,31],
        [30,32,31,32,31,30,30,30,29,30,29,31],
        [31,31,32,31,31,31,30,29,30,29,30,30],
        [31,31,32,32,31,30,30,29,30,29,30,30],
        [31,32,31,32,31,30,30,30,29,29,30,31],
        [30,32,31,32,31,31,29,30,30,29,29,31],
        [31,31,32,31,31,31,30,29,30,29,30,30],
        [31,31,32,32,31,30,30,29,30,29,30,30],
        [31,32,31,32,31,30,30,30,29,29,30,31],
        [31,31,31,32,31,31,29,30,30,29,30,30],
        [31,31,32,31,31,31,30,29,30,29,30,30],
        [31,31,32,32,31,30,30,29,30,29,30,30],
        [31,32,31,32,31,30,30,30,29,29,30,31],
        [31,31,31,32,31,31,29,30,30,29,30,30],
        [31,31,32,31,31,31,30,29,30,29,30,30],
        [31,32,31,32,31,30,30,29,30,29,30,30],
        [31,32,31,32,31,30,30,30,29,29,30,31],
        [31,31,31,32,31,31,30,29,30,29,30,30],
        [31,31,32,31,31,31,30,29,30,29,30,30],
        [31,32,31,32,31,30,30,30,29,29,30,30],
        [31,32,31,32,31,30,30,30,29,30,29,31],
        [31,31,31,32,31,31,30,29,30,29,30,30],
        [31,31,32,31,31,31,30,29,30,29,30,30],
        [31,32,31,32,31,30,30,30,29,29,30,30],
        [31,32,31,32,31,30,30,30,29,30,29,31],
        [31,31,32,31,31,31,30,29,30,29,30,30],
        [31,31,32,31,32,30,30,29,30,29,30,30],
        [31,32,31,32,31,30,30,30,29,29,30,31],
        [30,32,31,32,31,30,30,30,29,30,29,31],
        [31,31,32,31,31,31,30,29,30,29,30,30],
        [31,31,32,32,31,30,30,29,30,29,30,30],
        [31,32,31,32,31,30,30,30,29,29,30,31],
        [30,32,31,32,31,31,29,30,29,30,29,31],
        [31,31,32,31,31,31,30,29,30,29,30,30],
        [31,31,32,32,31,30,30,29,30,29,30,30],
        [31,32,31,32,31,30,30,30,29,29,30,31],
        [31,31,31,32,31,31,29,30,30,29,29,31],
        [31,31,32,31,31,31,30,29,30,29,30,30],
        [31,31,32,32,31,30,30,29,30,29,30,30],
        [31,32,31,32,31,30,30,30,29,29,30,31],
        [31,31,31,32,31,31,29,30,30,29,30,30],
        [31,31,32,31,31,31,30,29,30,29,30,30],
        [31,32,31,32,31,30,30,29,30,29,30,30],
        [31,32,31,32,31,30,30,30,29,29,30,31],
        [31,31,31,32,31,31,30,29,30,29,30,30],
        [31,31,32,31,31,31,30,29,30,29,30,30],
        [31,32,31,32,31,30,30,30,29,29,30,30],
        [31,32,31,32,31,30,30,30,29,30,29,31],
        [31,31,31,32,31,31,30,29,30,29,30,30],
        [31,31,32,31,31,31,30,29,30,29,30,30],
        [31,32,31,32,31,30,30,30,29,29,30,30],
        [31,32,31,32,31,30,30,30,29,30,29,31],
        [31,31,32,31,31,31,30,29,30,29,30,30],
        [31,31,32,31,31,31,30,29,30,29,30,30],
        [31,32,31,32,31,30,30,30,29,29,30,31],
        [30,32,31,32,31,30,30,30,29,30,29,31],
        [31,31,32,31,31,31,30,29,30,29,30,30],
        [31,31,32,31,31,31,30,30,29,30,30,30],
        [30,31,32,32,30,31,30,30,29,30,30,30],
        [30,32,31,32,31,30,30,30,29,30,30,30],
        [30,32,31,32,31,30,30,30,29,30,30,30],
    ];

    var MONTHS_EN = ["Baisakh", "Jestha", "Asar", "Shrawan", "Bhadra", "Ashwin", "Kartik", "Mangsir", "Poush", "Magh", "Falgun", "Chaitra"];
    var MONTHS_NP = ["\u092c\u0948\u0936\u093e\u0916", "\u091c\u0947\u0920", "\u0905\u0938\u093e\u0930", "\u0936\u094d\u0930\u093e\u0935\u0923", "\u092d\u093e\u0926\u094d\u0930", "\u0906\u0936\u094d\u0935\u093f\u0928", "\u0915\u093e\u0930\u094d\u0924\u093f\u0915", "\u092e\u0902\u0938\u093f\u0930", "\u092a\u094c\u0937", "\u092e\u093e\u0918", "\u092b\u093e\u0932\u094d\u0917\u0941\u0923", "\u091a\u0948\u0924\u094d\u0930"];
    var WEEKDAYS_EN = ["Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"];
    var WEEKDAYS_NP = ["\u0938\u094b\u092e\u092c\u093e\u0930", "\u092e\u0902\u0917\u0932\u092c\u093e\u0930", "\u092c\u0941\u0927\u092c\u093e\u0930", "\u092c\u093f\u0939\u093f\u092c\u093e\u0930", "\u0936\u0941\u0915\u094d\u0930\u092c\u093e\u0930", "\u0936\u0928\u093f\u092c\u093e\u0930", "\u0906\u0907\u0924\u092c\u093e\u0930"];
    var GMONTHS_EN = ["January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"];
    var GMONTHS_ABBR = ["Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"];
    var DEVANAGARI_ZERO = 0x0966;
    var FLAG = "\uD83C\uDDF3\uD83C\uDDF5";

    // Days since civil 1970-01-01 (H. Hinnant). Timezone-free.
    function daysFromCivil(y, m, d) {
        y -= m <= 2 ? 1 : 0;
        var era = Math.floor((y >= 0 ? y : y - 399) / 400);
        var yoe = y - era * 400;
        var doy = Math.floor((153 * (m + (m > 2 ? -3 : 9)) + 2) / 5) + d - 1;
        var doe = yoe * 365 + Math.floor(yoe / 4) - Math.floor(yoe / 100) + doy;
        return era * 146097 + doe - 719468;
    }

    function devanagari(n) {
        return String(n).replace(/[0-9]/g, function(ch) {
            return String.fromCharCode(DEVANAGARI_ZERO + Number(ch));
        });
    }

    // Gregorian Y/M/D (M 1-12) -> { ok, bsY, bsM (0-11), bsD, wd (Monday==0) }.
    function convert(gy, gm, gd) {
        var delta = daysFromCivil(gy, gm, gd) - daysFromCivil(1943, 4, 14);
        var wd = (((2 + delta) % 7) + 7) % 7;
        if (delta < 0) return { ok: false, wd: wd };
        var passed = delta + 1, bsY = -1, bsM = -1, bsD = 0;
        for (var yi = 0; yi < 91 && bsY < 0; yi++) {
            var yearDays = 0, mi;
            for (mi = 0; mi < 12; mi++) yearDays += BS_TABLE[yi][mi];
            if (passed <= yearDays) {
                for (mi = 0; mi < 12; mi++) {
                    if (passed <= BS_TABLE[yi][mi]) {
                        bsY = 2000 + yi; bsM = mi; bsD = passed;
                        break;
                    }
                    passed -= BS_TABLE[yi][mi];
                }
            } else {
                passed -= yearDays;
            }
        }
        if (bsY < 0) return { ok: false, wd: wd };
        return { ok: true, bsY: bsY, bsM: bsM, bsD: bsD, wd: wd };
    }

    // Full pill + tooltip strings, mirroring bs-patro.c exactly.
    function render(gy, gm, gd, useNp, vertical) {
        var c = convert(gy, gm, gd);
        var gfull = WEEKDAYS_EN[(new Date(gy, gm - 1, gd).getDay() + 6) % 7]
            + ", " + gd + " " + GMONTHS_EN[gm - 1] + " " + gy;
        var num = useNp ? devanagari : String;
        if (!c.ok) {
            var text = vertical
                ? [FLAG, String(gd), String(gy)]
                : [FLAG + " " + gd + " " + GMONTHS_ABBR[gm - 1] + " " + gy];
            return { text: text, tooltip: "BS date out of range (2000-2090)", ok: false };
        }
        var months = useNp ? MONTHS_NP : MONTHS_EN;
        var text = vertical
            ? [FLAG, num(c.bsD), num(c.bsY)]
            : [FLAG + " " + num(c.bsD) + " " + months[c.bsM] + " " + num(c.bsY)];
        var tip;
        if (useNp) {
            tip = "\u0935\u093f.\u0938\u0902.: " + WEEKDAYS_NP[c.wd] + ", "
                + devanagari(c.bsD) + " " + MONTHS_NP[c.bsM] + " " + devanagari(c.bsY)
                + "\nAD: " + gfull;
        } else {
            tip = "Bikram Sambat: " + WEEKDAYS_EN[c.wd] + ", "
                + c.bsD + " " + MONTHS_EN[c.bsM] + " " + c.bsY
                + "\nGregorian: " + gfull;
        }
        if (vertical) {
            tip = FLAG + " " + c.bsD + " " + MONTHS_EN[c.bsM] + " " + c.bsY + "\n" + tip;
        }
        return { text: text, tooltip: tip, ok: true };
    }

    return { convert: convert, render: render, devanagari: devanagari, daysFromCivil: daysFromCivil };
}

// Self-test: node bs-patro/Model.js. Anchors verified against bs-patro.c:
// epoch BS 2000-01-01 == AD 1943-04-14 (Wed), plus "today" parity.
var BsPatroModel = BsPatro();
if (typeof module !== "undefined" && require.main === module) {
    var assert = require("assert");
    var e = BsPatroModel.convert(1943, 4, 14);
    assert.deepStrictEqual([e.ok, e.bsY, e.bsM, e.bsD, e.wd], [true, 2000, 0, 1, 2]);
    var t = BsPatroModel.convert(2026, 9, 30);
    assert.deepStrictEqual([t.ok, t.bsY, t.bsM, t.bsD, t.wd], [true, 2083, 5, 14, 2]);
    assert.strictEqual(BsPatroModel.devanagari(2083), "\u0968\u0966\u096e\u0969");
    var r = BsPatroModel.render(2026, 9, 30, false, false);
    assert.strictEqual(r.text[0], "\uD83C\uDDF3\uD83C\uDDF5 14 Ashwin 2083");
    var rv = BsPatroModel.render(2026, 9, 30, false, true);
    assert.deepStrictEqual(rv.text, ["\uD83C\uDDF3\uD83C\uDDF5", "14", "2083"]);
    var rn = BsPatroModel.render(2026, 9, 30, true, false);
    assert.strictEqual(rn.text[0], "\uD83C\uDDF3\uD83C\uDDF5 \u0967\u096a \u0906\u0936\u094d\u0935\u093f\u0928 \u0968\u0966\u096e\u0969");
    var oob = BsPatroModel.render(1940, 1, 1, false, false);
    assert.strictEqual(oob.ok, false);
    assert.strictEqual(oob.tooltip, "BS date out of range (2000-2090)");
    var last = BsPatroModel.convert(2034, 4, 13);
    assert.deepStrictEqual([last.ok, last.bsY, last.bsM, last.bsD], [true, 2090, 11, 30]);
    var after = BsPatroModel.convert(2034, 4, 14);
    assert.strictEqual(after.ok, false);
    console.log("bs-patro Model.js: all tests passed");
}
