# lv2

A selection of LV2 plugins for Zynthian (and other LV2 hosts).

## Multi-Chord

Multi-chord is a MIDI processor. It will create a chord from each MIDI note received. Each incoming note C,C#,D,D#,E,F,F#,G,G#,A,A#,B can trigger any chord, individually configurable, e.g. incoming MIDI 'C' may trigger a C major chord whilst incoming MIDI 'C#' could trigger A minor. Each chord may have up to 3 notes. Any note configured the same as the first will not sound, e.g. setting C1='C', C2='E', C3='C' will play C + E when a MIDI 'C' is received.

The default is to pass all MIDI events uneffected. Some presets are included for some common chord patters, e.g. major triad, minor triad, etc.

Each note for each chord may be selected from an octave either side of the root (played) note. Chord patterns repeat for each octave, e.g. incoming MIDI 'C' will trigger the same chord but in different registers when played across the keyboard.
