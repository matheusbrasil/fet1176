# Build report: FET1176 (0x01001000)

Built by tools/zd2_make_effect.py from manifest.json; scaffold: CLEAN-ROOM (tools/zd2_cleanroom.py), container synthesized from scratch (tools/zd2_from_scratch.py).

- entry: e_entry = Dll_FET1176 @ 0xa60; audio fn Fx_SFX_FET1176 @ 0x0 (text)
- relocs: 24, all ABS32/ABS_L16/ABS_H16
- sections: ['.const', '.text']; .text 3744 B
- param table: 9 entries match the manifest + ABI doc; end marker on last param
- selector labels: RATIO = 4:1/8:1/12:1/20:1 (w9 -> Aw_P<k>_str, 8-byte stride, NUL-terminated)
- imageinfo: {24,30,->pic} + zero tail; pic blank (92 B, generic geometry)
- _Coe: 16 words; defaults match manifest (default/max as float32); unassigned slots zero
- .text: all 12 generated functions carry their landmark instructions (full dump: XXF1176.text.dis)
- provenance: generated blob + from-scratch container; no stock file enters the build
- kernel: ~3116 B in .text (upper size bound); SAFE-DSP ok (leaf, no stack, no callee-saved regs)
- icon: 2 fresh frames (drawn here), custom renderer fet1176_icon.py, panel theme dark/white-controls, structure invariants OK
- container ICON: the drawn ZIC frame placed
- container INFO dspload set to 30.00 raw = 11.1% of the 270 budget (container default kept otherwise)
- blob stripped (verified): 9284 -> 7285 B (-21.5%), removed .strtab, .symtab
- container: id 0x01001000, target 0x00a0, name 'FET1176', 7-param PRME/PRMJ, TXE1/TXJ1 set, CRC 0x2378e7d4, envelope invariants OK

## Parameters

| # | name | range | default | coeff slot | handler |
|---|---|---|---|---|---|
| 2 | IN | 0..100 | 75 | [5] | direct |
| 3 | OUT | 0..100 | 55 | [6] | direct |
| 4 | DRY | 0..100 | 30 | [7] | direct |
| 5 | RATIO | 0..3 | 0 | [8] | direct |
| 6 | ATACK | 0..100 | 70 | [9] | direct |
| 7 | RELSE | 0..100 | 25 | [10] | direct |
| 8 | HPF | 0..100 | 10 | [11] | direct |

## Files

- `build/XXF1176.ZD2` + `build/XXF1176.ZIC`: the upload pair
- `build/XXF1176.audio.dis`: C kernel disassembly (review)
- `build/XXF1176.text.dis`: full generated .text (review: every function in it is generated)

## Reminders

- CLEAN-ROOM build: the code blob contains NO Zoom-authored code, and the container, icon, text and params are yours (the container is synthesized from scratch; the three undecoded header fields ship zeroed, hardware-proven). MIT-shareable ONCE hardware-validated. See docs/ip-and-licensing.md.
- Upload under the safety protocol only (SAFETY.md): safe_connect first, live patch only, never save, readback compare.
- dspload declared 30.00 raw = 11.1% on-device; the pedal TRUSTS this for patch-composition gating (270-raw budget).
