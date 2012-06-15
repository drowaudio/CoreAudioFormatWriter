/*
  ==============================================================================

    CoreAudioFormat.h
    Created: 14 Jun 2012 6:07:10pm
    Author:  David Rowland

  ==============================================================================
*/

#ifndef __COREAUDIOFORMAT_H_B57C53A__
#define __COREAUDIOFORMAT_H_B57C53A__

#include "../JuceLibraryCode/JuceHeader.h"

#if JUCE_MAC || JUCE_IOS
#define CoreAudioFormat CoreAudioFormatNew

//==============================================================================
/**
    OSX and iOS only - This uses the AudioToolbox framework to read any audio
    format that the system has a codec for.

    This should be able to understand formats such as mp3, m4a, etc.

    @see AudioFormat
 */
class JUCE_API  CoreAudioFormat     : public AudioFormat
{
public:
    //==============================================================================
    /** Creates a format object. */
    CoreAudioFormat();

    /** Destructor. */
    ~CoreAudioFormat();

    //==============================================================================
    Array<int> getPossibleSampleRates();
    Array<int> getPossibleBitDepths();
    bool canDoStereo();
    bool canDoMono();

    //==============================================================================
    AudioFormatReader* createReaderFor (InputStream* sourceStream,
                                        bool deleteStreamIfOpeningFails);

    AudioFormatWriter* createWriterFor (OutputStream* streamToWriteTo,
                                        double sampleRateToUse,
                                        unsigned int numberOfChannels,
                                        int bitsPerSample,
                                        const StringPairArray& metadataValues,
                                        int qualityOptionIndex);

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CoreAudioFormat);
};

#undef CoreAudioFormat
#endif


#endif  // __COREAUDIOFORMAT_H_B57C53A__
