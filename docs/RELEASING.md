# Release identity

ZOOM patches reference effects by a 32-bit ID, while the pedal stores effect
files by an independent eight-character filename. Both must be globally unique
enough for community effects to coexist safely.

The current development identity is:

- ID: `0x01001000`
- Filename: `XXF1176`
- Name: `FET1176`

The `low16 = 0x1000` ID and `XX` prefix belong to the stomphacks playground.
They are intentionally reusable for private experiments and explicitly must not
be used for a distributed binary.

Before publishing a ZD2/ZIC download:

1. Request an author range and two-letter filename prefix through the
   [stomphacks project](https://github.com/thammer/stomphacks/issues).
2. Add the assigned author to the upstream `ids/effect-authors.json` registry.
3. Change the manifest to an assigned Dynamics ID with `low16 >= 0x2000`.
4. Change the flash filename to the assigned prefix plus at most six characters.
5. Rebuild from source and require a completely passing build report.
6. Hardware-test that exact release artifact under the safety protocol.
7. Publish the exact tested pair and their SHA-256 hashes—never a fresh rebuild.

Do not publish the current playground ZD2/ZIC pair.

