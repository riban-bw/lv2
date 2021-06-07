/*
 * DISTRHO Plugin Framework (DPF)
 * Copyright (C) 2012-2018 Filipe Coelho <falktx@falktx.com>
 *
 * Permission to use, copy, modify, and/or distribute this software for any purpose with
 * or without fee is hereby granted, provided that the above copyright notice and this
 * permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD
 * TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN
 * NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER
 * IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "DistrhoPlugin.hpp"
#define MAX_NOTES 4 // Maximum notes in a chord

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------------------------------------------

// Some constant strings used in naming elements
String m_saNoteNames[] = {String("C"),String("C#"),String("D"),String("D#"),String("E"),String("F"),String("F#"),String("G"),String("G#"),String("A"),String("A#"),String("B")};

/**
  Plugin that creats different chords for each note of an octave played
 */
class MultiChord : public Plugin
{
    public:
        MultiChord()
            : Plugin(
                24 * MAX_NOTES // Quantity of parameters
                ,3 // Quantity of internal presets (enable DISTRHO_PLUGIN_WANT_PROGRAMS)
                ,0 // Quantity of internal states
            )
        {
        }

    protected:
       /* --------------------------------------------------------------------------------------------------------
        * Information */

       /**
          Get the plugin label.
          This label is a short restricted name consisting of only _, a-z, A-Z and 0-9 characters.
        */
        const char* getLabel() const override
        {
            return "MultiChord";
        }

       /**
          Get an extensive comment/description about the plugin.
        */
        const char* getDescription() const override
        {
            return "Plugin that creates different chords for each note of the octave";
        }

       /**
          Get the plugin author/maker.
        */
        const char* getMaker() const override
        {
            return "riban";
        }

       /**
          Get the plugin homepage.
        */
        const char* getHomePage() const override
        {
            return "https://github.com/riban-bw/lv2";
        }

       /**
          Get the plugin license name (a single line of text).
          For commercial plugins this should return some short copyright information.
        */
        const char* getLicense() const override
        {
            return "ISC";
        }

       /**
          Get the plugin version, in hexadecimal.
        */
        uint32_t getVersion() const override
        {
            return d_version(1, 0, 0);
        }


       /**
          Get the plugin unique Id.
          This value is used by LADSPA, DSSI and VST plugin formats.
        */
        int64_t getUniqueId() const override
        {
            int64_t nValue = ('r' << 24) | ('i' << 16) | ('b' << 8) | ('a' << 0);
            return (nValue << 32) | ('n' << 24) | 1;
        }

       /* --------------------------------------------------------------------------------------------------------
        * Init and Internal data*/
        void initParameter(uint32_t index, Parameter& parameter) override
        {
            if(index > 24 * MAX_NOTES)
                return;
            uint32_t nNote = index % MAX_NOTES;
            uint32_t nChord = (index / MAX_NOTES) % 12;
            String sName;
            if(index < 12 * MAX_NOTES)
            {
                // Note map
                sName = String("Offset ") + m_saNoteNames[nChord] + String(nNote + 1);
                parameter.hints = kParameterIsAutomable | kParameterIsInteger;
                parameter.ranges.min = -12.0f;
                parameter.ranges.max = 12.0f;
                parameter.ranges.def = 0.0f;
                parameter.enumValues.count = 25;
                parameter.enumValues.restrictedMode = true;
                ParameterEnumerationValue* const values = new ParameterEnumerationValue[25];
                for(int i = -12; i < 13; ++i)
                {
                    String sPrefix("");
                    if(i < 0)
                        sPrefix = "-";
                    else if(i > 0)
                        sPrefix = "+";
                    values[i + 12].label = sPrefix + m_saNoteNames[(nChord + 12 + i) % 12];
                    values[i + 12].value = i;
                }
                parameter.enumValues.values = values;
                m_fParamValues[index] = 0.0f;
            }
            else
            {
                // Velocity map
                sName = String("Velocity ") + m_saNoteNames[nChord] + String(nNote + 1);
                parameter.hints = kParameterIsAutomable;
                parameter.ranges.min = 0.5f;
                parameter.ranges.max = 2.0f;
                parameter.ranges.def = 1.0f;
                m_fParamValues[index] = 1.0f;
            }
            parameter.name = sName;
            parameter.symbol = sName.replace('#', 's').replace(' ', '_').toLower();
        }

        void initProgramName(uint32_t index, String& programName)
        {
            switch(index)
            {
                case 0:
                    programName = "Default";
                    break;
                case 1:
                    programName = "Major triad";
                    break;
                case 2:
                    programName = "Minor triad";
                    break;
            }
        }

        float getParameterValue(uint32_t index) const override
        {
            if(index < 24 * MAX_NOTES)
                return m_fParamValues[index];
            return 0.0f;
        }

        void setParameterValue(uint32_t index, float value) override
        {
            if(index < 24 * MAX_NOTES)
                m_fParamValues[index] = value;
        }

        /**
         Load a program.
        */
        void loadProgram(uint32_t index)
        {
            switch(index)
            {
                case 0: // Default
                case 1: // Major triad
                case 2: // Minor triad
                    for(int i = 0; i < MAX_NOTES * 12; ++i)
                    {
                        m_fParamValues[i] = 0.0f;
                        m_fParamValues[i + MAX_NOTES * 12] = 1.0f;
                    }
                    if(index == 0)
                        return;
                    for(int i = 0; i < MAX_NOTES * 12; i+=MAX_NOTES)
                    {
                        m_fParamValues[i] = 0.0f;
                        m_fParamValues[i+1] = (index==1)?4.0f:3.0f;
                        m_fParamValues[i+2] = 7.0f;
                    }
                    break;
            }
        }

       /* --------------------------------------------------------------------------------------------------------
        * Audio/MIDI Processing */

       /**
          Run/process function for plugins with MIDI input.
          In this case we just pass-through all MIDI events.
        */
        void run(const float**, float**, uint32_t, const MidiEvent* midiEvents, uint32_t midiEventCount) override
        {
            int note, velocity;
            for (uint32_t j = 0; j < midiEventCount; ++j)
            {
                if(midiEvents[j].kDataSize > 2 && (midiEvents[j].data[0] & 0xE0) == 0x80)
                {
                    uint8_t base_note;
                    // Note on or note off
                    for(int i=0; i < MAX_NOTES; ++i)
                    {
                        // Iterate through each note of chord
                        base_note = midiEvents[j].data[1] % 12;
                        int nParam = base_note * MAX_NOTES + i;
                        int offset = m_fParamValues[nParam];
                        if(i > 0 && offset == (int)m_fParamValues[base_note * MAX_NOTES])
                            continue; // Skip if note not configured (same as root note)
                        note = midiEvents[j].data[1] + offset;
                        if(note > 127 || note < 0)
                            continue; // Transposed note is out of range
                        velocity = m_fParamValues[nParam + 12 * MAX_NOTES] * midiEvents[j].data[2];
                        if (velocity < 1)
                            continue; // Don't play extremely quite notes
                        if(velocity > 127)
                            velocity = 127;
                        MidiEvent chordEvent;
                        memcpy(&chordEvent, &midiEvents[j], sizeof(MidiEvent));
                        chordEvent.data[1] = note;
                        chordEvent.data[2] = velocity;
                        writeMidiEvent(chordEvent);
                    }
                }
                else
                    writeMidiEvent(midiEvents[j]);
            }
        }

        // -------------------------------------------------------------------------------------------------------

        private:
            float m_fParamValues[24 * MAX_NOTES];

       /**
          Set our plugin class as non-copyable and add a leak detector just in case.
        */
        DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MultiChord)
};

/* ------------------------------------------------------------------------------------------------------------
 * Plugin entry point, called by DPF to create a new plugin instance. */

Plugin* createPlugin()
{
    return new MultiChord();
}

// -----------------------------------------------------------------------------------------------------------

END_NAMESPACE_DISTRHO
