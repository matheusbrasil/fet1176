# Pedal safety

Custom ZD2 effects are experimental. A broken effect saved into the patch that
loads at startup can make an MS Plus pedal repeatedly crash during boot. A file
transfer interrupted while the pedal rewrites its effect storage can also leave
the pedal in an unsafe state.

The authoritative and maintained procedure is the
[stomphacks safety guide](https://github.com/thammer/stomphacks/blob/main/SAFETY.md).
Read it completely before installing FETCOMP.

At minimum:

1. Make a complete backup of effects, icons, effect list, and patches.
2. Confirm the pedal's **Autosave setting is OFF** and require the pedal's own
   acknowledgement rather than assuming the command succeeded.
3. Use the guarded `tools-pedal/pedal_diy.py` installer from stomphacks.
4. Keep experimental effects only in the current unsaved scratch patch.
   **Never save a patch containing an experimental effect.**
5. Before replacing an installed version, remove the effect from the current
   chain, uninstall the old file, and independently confirm the filename is
   absent.
6. Install the exact validated ZD2/ZIC pair, read both files back from the
   pedal, and compare them byte-for-byte with what was uploaded.
7. One program may access the pedal's MIDI/file interface at a time.
8. If any transfer hangs or fails, **do not power-cycle the pedal**. Keep it
   powered and follow the stomphacks rescue procedure first.

The source being open and the build passing tests do not remove these risks.

