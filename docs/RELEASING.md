# Release identity

ZOOM patches reference effects by a 32-bit ID, while the pedal stores effect
files by an independent filename of at most eight characters. Both values must
be unique among every effect installed on a pedal.

FET1176 uses:

- ID: `0x01004000`
- Filename: `MBF1176`
- Display name: `FET1176`
- Author prefix: `MB`
- Self-reserved low-16 block: `0x4000–0x43FF`

`0x01` is the Dynamics category on the MS-60B+. `0x4000` is the first effect
coordinate in the project's `MB` block.

## Why this ID

There is no official ZOOM allocation mechanism for community effects. The
stomphacks project proposes per-author ranges at `low16 >= 0x2000` and records
known authors in a community ledger. That document explicitly calls the scheme
a proposal rather than a settled standard.

At the time this identity was selected, the ledger assigned `0x2000–0x23FF`
to Waveformer and `0x3000–0x33FF` to Leemuzhko. FET1176 therefore self-reserves
the next non-overlapping aligned block, `0x4000–0x43FF`, for Matheus Brasil and
uses the filename prefix `MB`. Searches of the referenced public custom-effect
projects and the local MS-60B+ stock catalog found no collision for
`0x01004000` or `MBF1176`.

This is a project reservation, not an official assignment. A future community
registry conflict would require coordination. Never reuse `0x01004000` for a
different effect: released patches retain this ID permanently across FET1176
updates.

## Local stomphacks registry entry

The stomphacks builder validates author ranges. Add this object to the
`authors` array in `ids/effect-authors.json` before building:

```json
{
  "nickname": "matheusbrasil",
  "name": "Matheus Brasil",
  "low16": "0x4000-0x43ff",
  "prefix": "MB",
  "status": "self-reserved",
  "url": "https://github.com/matheusbrasil/fet1176"
}
```

## Release checklist

1. Build `source/manifest.json` with a clean stomphacks checkout plus the
   registry entry above.
2. Require a completely passing build report.
3. Confirm ID `0x01004000`, filename `MBF1176`, target `0x00A0`, and the
   expected parameter table in the decoded container.
4. Hardware-test that exact artifact under the protocol in `SAFETY.md`.
5. Publish the exact tested ZD2/ZIC pair and their SHA-256 hashes—never an
   untested fresh rebuild.

