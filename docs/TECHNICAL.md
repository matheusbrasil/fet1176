# Technical notes

## Signal path

FETCOMP processes stereo blocks in place at 44.1 kHz using the MS Plus C674x
effect ABI. The detector receives the mono sum of the two channels, passes it
through a compact two-biquad detector voicing model and a user-controlled
second-order high-pass filter, then feeds a peak envelope follower.

Gain reduction uses a variable threshold, ratio-dependent slope, and a
quadratic soft knee. The resulting dB reduction is converted to linear gain and
applied equally to both channels. Make-up gain affects the compressed path; the
`DRY` signal is added afterward in parallel.

## Parameter mappings

For normalized knob position `n = value / 100`:

- Input threshold: `T = -60n dB`
- Output gain: `-30 + 60n dB`
- Attack: `2 + 78n ms`
- Release: `50 + 1050n ms`
- Detector HPF: `20 + 480n Hz`, Q approximately 0.707
- Ratios: 4:1, 8:1, 12:1, 20:1

The detector HPF target is smoothed at block rate and reaches approximately 99%
of a knob movement in 30 ms, avoiding the stepping/clicking that a coefficient
jump can produce.

## Implementation constraints

- Leaf DSP callback: no external calls
- No stack frame or callee-saved register use
- No standard-library math or hardware divide dependency
- Per-instance state only; no mutable statics
- Initialization guard prevents uninitialized coefficients from reaching audio
- Every output sample is checked and bounded before it is written to the bus
- Approximate `log2` and `exp2` routines avoid unavailable runtime helpers

The build verifier reports the kernel at approximately 3,116 bytes in `.text`
and validates the supported relocation set, parameter tables, defaults,
container CRC, target, and ZIC structure.

## Icon theme

The icon contains native 72×97 and 102×128 monochrome frames. Its final ZIC
descriptor sets `invert = 1`, the stock dark-panel theme flag that makes the
pedal render white controls over the black panel. This behavior was confirmed
on the MS-60B+.

## Validation

Hardware observations for the unchanged DSP core in version 0.22 include:

- Exact bypass at `IN 0`, `OUT 50`, `DRY 0`
- Smooth IN, OUT, attack, release, dry, and HPF response
- Detector HPF restores low-string weight as its value rises
- Strong slap/pluck peaks held roughly 10 dB below bypass at the default setting
- Correct selector labels for all four ratios
- Correct dark-panel/white-control display treatment
- Independent byte-identical readback of both ZD2 and ZIC after installation

