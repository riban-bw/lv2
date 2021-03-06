# MultiChord

A LV2 MIDI plugin to trigger chords from single MIDI notes.

Each note of the octave can be individually configured to trigger up to a four note chord. Each note of the chord may be up to an octave below or above the trigger note. Notes from different octaves will trigger chords from the same relative octave. Unlike many other chord plugins, MutliChord allows you to define different chord types for each note, e.g. one note triggers a major chord while another triggers a minor chord. This plugin allows configuration of up to twelve different chords.

Each note of each chord may play at a different velocity relative to the trigger note's velocity, in a range from half the velocity to twice the velocity of the trigger note. For example, if the trigger note velocity is `100` and the scale for the chord is `0.5, 2, 1, 0.5` then the resulting chord will have note velocities: `50, 127, 100, 50`. Note that velocities will be constrained to the range between 1 and 127

Each of the 12 notes of an octave have the following parameters

Parameter | Description | Type | Minimum | Maximum
--------- | ----------- | ---- | ------- | -------
Offset 1 | 1st note of chord relative to trigger note | Integer (semitone) | -12 | +12
Offset 2 | 2nd note of chord relative to trigger note | Integer (semitone) | -12 | +12
Offset 3 | 3rd note of chord relative to trigger note | Integer (semitone) | -12 | +12
Offset 4 | 4th note of chord relative to trigger note | Integer (semitone) | -12 | +12
Velocity 1 | Velocity of 1st note relative to trigger note | Float | 0.5 | 2.0
Velocity 2 | Velocity of 2nd note relative to trigger note | Float | 0.5 | 2.0
Velocity 3 | Velocity of 3rd note relative to trigger note | Float | 0.5 | 2.0
Velocity 4 | Velocity of 4th note relative to trigger note | Float | 0.5 | 2.0

Setting a note's offset to 0 will disable the chord note unless it is the first note of the chord, e.g. Offset values 0,4,7,0 will trigger a major chord triad. Triggering this chord with 'C' will result in 'C', 'E' and 'G' being played.

The plugin provides three builtin presets as described in following table which shows an example of the notes in chord that will play if a 'C4' trigger note is played. The presets play all notes of chord with same velocity as trigger note is played.

Name | Description | Note 1 | Note 2 | Note 3 | Note 4
---- | ----------- | ------ | ------ | ------ | ------
Default | Effectively bypasses plugin by playing same notes as triggered | C4 | None | None | None
Major Triad | A three note major chord | C4 | E4 | G4 | None
Minor Triad | A three note minor chord | C4 | Eb4 | G4 | None
