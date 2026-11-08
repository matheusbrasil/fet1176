# Release identity

ZOOM patches reference effects by a 32-bit ID, while the pedal stores effect
files by an independent filename of at most eight characters. Both values must
be unique among every effect installed on a pedal.

FETCOMP uses:

- ID: `0x0100A000`
- Filename: `MBFETCMP`
- Display name: `FETCOMP`
- Author prefix: `MB`
- Assigned low-16 block: `0xA000–0xA03F`

`0x01` is the Dynamics category on the MS-60B+. `0xA000` is the first effect
coordinate in the range Thomas Hammer assigned to `matheusbrasil`.

## Why this ID

The stomphacks community scheme allocates a non-overlapping low-16 range and a
two-letter filename prefix to each author. Thomas Hammer assigned
`0xA000–0xA03F` to the `matheusbrasil` author nickname. Combining its first
coordinate with the MS-60B+ Dynamics group produces the full ID
`0x0100A000`. The eight-character flash filename is `MBFETCMP`: the `MB`
namespace plus the six-character abbreviation `FETCMP`.

Never reuse `0x0100A000` for another effect. New FETCOMP versions keep this ID;
a genuinely different effect uses another coordinate in the assigned range.

## Local stomphacks registry entry

The stomphacks builder validates author ranges. Add this object to the
`authors` array in `ids/effect-authors.json` before building:

```json
{
  "nickname": "matheusbrasil",
  "name": "Matheus Brasil",
  "low16": "0xa000-0xa03f",
  "prefix": "MB",
  "status": "assigned",
  "url": "https://github.com/matheusbrasil/fet1176"
}
```

## Release checklist

1. Build `source/manifest.json` with a clean stomphacks checkout plus the
   registry entry above.
2. Require a completely passing build report.
3. Confirm ID `0x0100A000`, filename `MBFETCMP`, target `0x00A0`, and the
   expected parameter table in the decoded container.
4. Hardware-test that exact artifact under the protocol in `SAFETY.md`.
5. Publish the exact tested ZD2/ZIC pair and their SHA-256 hashes—never an
   untested fresh rebuild.

