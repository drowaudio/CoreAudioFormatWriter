/*
  ==============================================================================

    CoreAudioFormat.cpp
    Created: 14 Jun 2012 6:07:10pm
    Author:  David Rowland

  ==============================================================================
*/

#include "CoreAudioFormat.h"
#include <AudioToolbox/AudioToolbox.h>

#if JUCE_MAC || JUCE_IOS
#define CoreAudioFormat CoreAudioFormatNew

//==============================================================================
namespace
{
    const char* const coreAudioFormatName = "CoreAudio supported file";

    StringArray findFileExtensionsForCoreAudioCodecs()
    {
        StringArray extensionsArray;
        CFMutableArrayRef extensions = CFArrayCreateMutable (0, 0, 0);
        UInt32 sizeOfArray = sizeof (CFMutableArrayRef);

        if (AudioFileGetGlobalInfo (kAudioFileGlobalInfo_AllExtensions, 0, 0, &sizeOfArray, &extensions) == noErr)
        {
            const CFIndex numValues = CFArrayGetCount (extensions);

            for (CFIndex i = 0; i < numValues; ++i)
                extensionsArray.add ("." + String::fromCFString ((CFStringRef) CFArrayGetValueAtIndex (extensions, i)));
        }

        CFRelease (extensions);
        return extensionsArray;
    }
    
    StringArray findWritableTypes()
    {
        StringArray extensionsArray;
        
        UInt32 size;
        UInt32* fileTypes = NULL;
        
        OSStatus status = AudioFileGetGlobalInfoSize (kAudioFileGlobalInfo_WritableTypes, 0, NULL, &size);
        if (status == noErr)
        {
            const int numFileFormats = size / sizeof (UInt32);
            fileTypes = new UInt32[numFileFormats];
            
            status = AudioFileGetGlobalInfo (kAudioFileGlobalInfo_WritableTypes, 0, NULL, &size, fileTypes);
            if (status == noErr)
            {
                for (int i = 0; i < numFileFormats; ++i)
                {
                    String typeId;
                    char* id = (char*) &fileTypes[i];
                    typeId << id[3] << id[2] << id[1] << id[0];
                    extensionsArray.add (typeId);
                }
            }
        }
        
        return extensionsArray;
    }
}

//==============================================================================
class CoreAudioReader : public AudioFormatReader
{
public:
    CoreAudioReader (InputStream* const inp)
        : AudioFormatReader (inp, TRANS (coreAudioFormatName)),
          ok (false), lastReadPosition (0)
    {
        usesFloatingPointData = true;
        bitsPerSample = 32;

        OSStatus status = AudioFileOpenWithCallbacks (this,
                                                      &readCallback,
                                                      0,        // write needs to be null to avoid permisisions errors
                                                      &getSizeCallback,
                                                      0,        // setSize needs to be null to avoid permisisions errors
                                                      0,        // AudioFileTypeID inFileTypeHint
                                                      &audioFileID);
        if (status == noErr)
        {
            status = ExtAudioFileWrapAudioFileID (audioFileID, false, &audioFileRef);

            if (status == noErr)
            {
                AudioStreamBasicDescription sourceAudioFormat;
                UInt32 audioStreamBasicDescriptionSize = sizeof (AudioStreamBasicDescription);
                ExtAudioFileGetProperty (audioFileRef,
                                         kExtAudioFileProperty_FileDataFormat,
                                         &audioStreamBasicDescriptionSize,
                                         &sourceAudioFormat);

                numChannels = sourceAudioFormat.mChannelsPerFrame;
                sampleRate  = sourceAudioFormat.mSampleRate;

                UInt32 sizeOfLengthProperty = sizeof (int64);
                ExtAudioFileGetProperty (audioFileRef,
                                         kExtAudioFileProperty_FileLengthFrames,
                                         &sizeOfLengthProperty,
                                         &lengthInSamples);

                destinationAudioFormat.mSampleRate       = sampleRate;
                destinationAudioFormat.mFormatID         = kAudioFormatLinearPCM;
                destinationAudioFormat.mFormatFlags      = kLinearPCMFormatFlagIsFloat | kLinearPCMFormatFlagIsNonInterleaved | kAudioFormatFlagsNativeEndian;
                destinationAudioFormat.mBitsPerChannel   = sizeof (float) * 8;
                destinationAudioFormat.mChannelsPerFrame = numChannels;
                destinationAudioFormat.mBytesPerFrame    = sizeof (float);
                destinationAudioFormat.mFramesPerPacket  = 1;
                destinationAudioFormat.mBytesPerPacket   = destinationAudioFormat.mFramesPerPacket * destinationAudioFormat.mBytesPerFrame;

                status = ExtAudioFileSetProperty (audioFileRef,
                                                  kExtAudioFileProperty_ClientDataFormat,
                                                  sizeof (AudioStreamBasicDescription),
                                                  &destinationAudioFormat);
                if (status == noErr)
                {
                    bufferList.malloc (1, sizeof (AudioBufferList) + numChannels * sizeof (AudioBuffer));
                    bufferList->mNumberBuffers = numChannels;
                    ok = true;
                }
            }
        }
    }

    ~CoreAudioReader()
    {
        ExtAudioFileDispose (audioFileRef);
        AudioFileClose (audioFileID);
    }

    //==============================================================================
    bool readSamples (int** destSamples, int numDestChannels, int startOffsetInDestBuffer,
                      int64 startSampleInFile, int numSamples)
    {
        jassert (destSamples != nullptr);
        const int64 samplesAvailable = lengthInSamples - startSampleInFile;

        if (samplesAvailable < numSamples)
        {
            for (int i = numDestChannels; --i >= 0;)
                if (destSamples[i] != nullptr)
                    zeromem (destSamples[i] + startOffsetInDestBuffer, sizeof (int) * (size_t) numSamples);

            numSamples = (int) samplesAvailable;
        }

        if (numSamples <= 0)
            return true;

        if (lastReadPosition != startSampleInFile)
        {
            OSStatus status = ExtAudioFileSeek (audioFileRef, startSampleInFile);
            if (status != noErr)
                return false;

            lastReadPosition = startSampleInFile;
        }

        while (numSamples > 0)
        {
            const int numThisTime = jmin (8192, numSamples);
            const size_t numBytes = sizeof (float) * (size_t) numThisTime;

            audioDataBlock.ensureSize (numBytes * numChannels, false);
            float* data = static_cast<float*> (audioDataBlock.getData());

            for (int j = (int) numChannels; --j >= 0;)
            {
                bufferList->mBuffers[j].mNumberChannels = 1;
                bufferList->mBuffers[j].mDataByteSize = (UInt32) numBytes;
                bufferList->mBuffers[j].mData = data;
                data += numThisTime;
            }

            UInt32 numFramesToRead = (UInt32) numThisTime;
            OSStatus status = ExtAudioFileRead (audioFileRef, &numFramesToRead, bufferList);
            if (status != noErr)
                return false;

            for (int i = numDestChannels; --i >= 0;)
            {
                if (destSamples[i] != nullptr)
                {
                    if (i < (int) numChannels)
                        memcpy (destSamples[i] + startOffsetInDestBuffer, bufferList->mBuffers[i].mData, numBytes);
                    else
                        zeromem (destSamples[i] + startOffsetInDestBuffer, numBytes);
                }
            }

            startOffsetInDestBuffer += numThisTime;
            numSamples -= numThisTime;
            lastReadPosition += numThisTime;
        }

        return true;
    }

    bool ok;

private:
    AudioFileID audioFileID;
    ExtAudioFileRef audioFileRef;
    AudioStreamBasicDescription destinationAudioFormat;
    MemoryBlock audioDataBlock;
    HeapBlock<AudioBufferList> bufferList;
    int64 lastReadPosition;

    static SInt64 getSizeCallback (void* inClientData)
    {
        return static_cast<CoreAudioReader*> (inClientData)->input->getTotalLength();
    }

    static OSStatus readCallback (void* inClientData,
                                  SInt64 inPosition,
                                  UInt32 requestCount,
                                  void* buffer,
                                  UInt32* actualCount)
    {
        CoreAudioReader* const reader = static_cast<CoreAudioReader*> (inClientData);

        reader->input->setPosition (inPosition);
        *actualCount = (UInt32) reader->input->read (buffer, (int) requestCount);

        return noErr;
    }

    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CoreAudioReader);
};

//==============================================================================
class CoreAudioWriter  : public AudioFormatWriter
{
public:
    CoreAudioWriter (OutputStream* const out, const double sampleRate_,
                     const unsigned int numChannels_, const unsigned int bits,
                     const StringPairArray& metadataValues)
        : AudioFormatWriter (out, TRANS (coreAudioFormatName), sampleRate_, numChannels_, bits),
          writeFailed (true),
          bytesWritten (0)
    {
        usesFloatingPointData = true;
        bitsPerSample = 32;
        
        // this appears to return all the types, even those specified by the docs as non-writable
        StringArray types (findWritableTypes());
        for (int i = 0; i < types.size(); ++i)
        {
            DBG (types[i]);
        }
        
        // set the input stream to the output stream's start
        FileOutputStream* fileCheck = dynamic_cast<FileOutputStream*> (out);
        if (fileCheck != nullptr)
            input = new FileInputStream (fileCheck->getFile());

        MemoryOutputStream* memoryCheck = dynamic_cast<MemoryOutputStream*> (out);
        if (memoryCheck != nullptr)
            input = new MemoryInputStream (memoryCheck->getData(), memoryCheck->getDataSize(), false);
        
        // destination format
        AudioStreamBasicDescription destinationAudioFormat;
        destinationAudioFormat.mSampleRate       = sampleRate;
        destinationAudioFormat.mFormatID         = kAudioFormatLinearPCM;
        destinationAudioFormat.mFormatFlags      = kLinearPCMFormatFlagIsSignedInteger | kLinearPCMFormatFlagIsPacked;
        destinationAudioFormat.mBitsPerChannel   = sizeof (int16) * 8;
        destinationAudioFormat.mChannelsPerFrame = numChannels;
        destinationAudioFormat.mBytesPerFrame    = sizeof (int16);
        destinationAudioFormat.mFramesPerPacket  = 1;
        destinationAudioFormat.mBytesPerPacket   = destinationAudioFormat.mFramesPerPacket * destinationAudioFormat.mBytesPerFrame;
        
        audioFileID = NULL;
        OSStatus status = AudioFileInitializeWithCallbacks (this, 
                                                            &readCallback,
                                                            &writeCallback,
                                                            &getSizeCallback,
                                                            &setSizeCallback,
                                                            kAudioFileWAVEType,
                                                            &destinationAudioFormat,
                                                            0,
                                                            &audioFileID);
        
        String statusCode;
        char* id = (char*) &status;
        statusCode << id[3] << id[2] << id[1] << id[0];
        DBG (statusCode);
//        OSStatus status = AudioFileOpenWithCallbacks (this,
//                                                      &readCallback,
//                                                      &writeCallback,
//                                                      &getSizeCallback,
//                                                      &setSizeCallback,
//                                                      kAudioFileAAC_ADTSType,        // AudioFileTypeID inFileTypeHint
//                                                      &audioFileID);
        jassert (audioFileID != NULL);
        
        if (status == noErr)
        {
            status = ExtAudioFileWrapAudioFileID (audioFileID, false, &audioFileRef);
            
            if (status == noErr)
            {
                // the format that we will supply to the writer
                AudioStreamBasicDescription sourceAudioFormat;
                sourceAudioFormat.mSampleRate       = sampleRate;
                sourceAudioFormat.mFormatID         = kAudioFormatLinearPCM;
                sourceAudioFormat.mFormatFlags      = kLinearPCMFormatFlagIsFloat | kLinearPCMFormatFlagIsNonInterleaved | kAudioFormatFlagsNativeEndian;
                sourceAudioFormat.mBitsPerChannel   = sizeof (float) * 8;
                sourceAudioFormat.mChannelsPerFrame = numChannels;
                sourceAudioFormat.mBytesPerFrame    = sizeof (float);
                sourceAudioFormat.mFramesPerPacket  = 1;
                sourceAudioFormat.mBytesPerPacket   = sourceAudioFormat.mFramesPerPacket * sourceAudioFormat.mBytesPerFrame;
                
                status = ExtAudioFileSetProperty (audioFileRef,
                                                  kExtAudioFileProperty_ClientDataFormat,
                                                  sizeof (AudioStreamBasicDescription),
                                                  &sourceAudioFormat);
                if (status == noErr)
                {
                    bufferList.malloc (1, sizeof (AudioBufferList) + numChannels * sizeof (AudioBuffer));
                    bufferList->mNumberBuffers = numChannels;
                    writeFailed = false;
                }
            }
        }
    }
    
    ~CoreAudioWriter()
    {
        ExtAudioFileDispose (audioFileRef);
        AudioFileClose (audioFileID);
    }
    
    //==============================================================================
    bool write (const int** data, int numSamples)
    {
        DBG ("write");
        jassert (data != nullptr && *data != nullptr); // the input must contain at least one channel!

        if (writeFailed)
            return false;
        
        const size_t numBytes = sizeof (float) * (size_t) numSamples;
        
        for (int j = (int) numChannels; --j >= 0;)
        {
            if (data[j] != nullptr)
            {
                bufferList->mBuffers[j].mNumberChannels = 1;
                bufferList->mBuffers[j].mDataByteSize = (UInt32) numBytes;
                bufferList->mBuffers[j].mData = (void*) data[j];
            }
        }
        
        UInt32 numFramesToWrite = (UInt32) numSamples;

        OSStatus status = ExtAudioFileWrite (audioFileRef, numFramesToWrite, bufferList);
        
        if (status == noErr)
            return true;
        
        writeFailed = true;

        return false;
    }
    
    bool writeFailed;

private:
    //==============================================================================
    AudioFileID audioFileID;
    ExtAudioFileRef audioFileRef;
    HeapBlock<AudioBufferList> bufferList;
    uint64 bytesWritten;
    ScopedPointer<InputStream> input;
    //int64 expectedSize;
    //MemoryBlock tempBlock;
    
    //==============================================================================
    static SInt64 getSizeCallback (void* inClientData)
    {
        CoreAudioWriter* const writer = static_cast<CoreAudioWriter*> (inClientData);
        const SInt64 size = writer->bytesWritten;//writer->input->getTotalLength();
        DBG ("getSizeCallback " << (int) size);
        return size;

//
//        return writer->tempBlock.getSize();//expectedSize;//bytesWritten;
    }
    
    static OSStatus readCallback (void* inClientData,
                                  SInt64 inPosition,
                                  UInt32 requestCount,
                                  void* buffer,
                                  UInt32* actualCount)
    {
//        DBG ("readCallback - pos: " << (int) inPosition << " bytes: " << (int) requestCount);
//        CoreAudioWriter* const writer = static_cast<CoreAudioWriter*> (inClientData);
//        memcpy (addBytesToPointer (writer->tempBlock.getData(), inPosition), buffer, requestCount);
//        *actualCount = requestCount;
        CoreAudioWriter* const writer = static_cast<CoreAudioWriter*> (inClientData);
        
        const bool seekSucceeded = writer->input->setPosition (inPosition);
        *actualCount = (UInt32) writer->input->read (buffer, (int) requestCount);

        DBG ("readCallback - pos: " << (int) inPosition << " bytes: " << (int) requestCount << " seek: " << seekSucceeded);
        DBG (*((char*) buffer));

        return noErr;
    }

    static OSStatus writeCallback (void* inClientData,
                                   SInt64 inPosition, 
                                   UInt32 requestCount, 
                                   const void* buffer, 
                                   UInt32* actualCount)
    {
        CoreAudioWriter* const writer = static_cast<CoreAudioWriter*> (inClientData);
        
        //const bool success = writer->output->write (addBytesToPointer (buffer, inPosition), requestCount);
        writer->output->setPosition (inPosition);
        const bool success = writer->output->write (buffer, requestCount);

        if (success)
        {
//            writer->tempBlock.ensureSize (writer->tempBlock.getSize() + requestCount + inPosition);
//            memcpy (addBytesToPointer (writer->tempBlock.getData(), inPosition), buffer, requestCount);

            writer->bytesWritten += requestCount;
            DBG ("writeCallback - pos: " << (int) inPosition << " bytes: " << (int) requestCount << " total: " << (int) writer->bytesWritten);
            *actualCount = requestCount;
            DBG (*((char*) buffer));
            return noErr;
        }
        else
        {
            DBG ("write error");
            *actualCount = 0;
            return success;
        }
    }
    
    static OSStatus setSizeCallback (void* inClientData,
                                     SInt64 inSize)
    {
        DBG ("setSizeCallback: " << (int) inSize);
        CoreAudioWriter* const writer = static_cast<CoreAudioWriter*> (inClientData);

        const int64 numBytesToPad = inSize - writer->bytesWritten;
        writer->bytesWritten += numBytesToPad;
        writer->output->writeRepeatedByte (0, numBytesToPad);
        
        return noErr;
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CoreAudioWriter);
};

//==============================================================================
CoreAudioFormat::CoreAudioFormat()
    : AudioFormat (TRANS (coreAudioFormatName), findFileExtensionsForCoreAudioCodecs())
{
}

CoreAudioFormat::~CoreAudioFormat() {}

Array<int> CoreAudioFormat::getPossibleSampleRates()    { return Array<int>(); }
Array<int> CoreAudioFormat::getPossibleBitDepths()      { return Array<int>(); }

bool CoreAudioFormat::canDoStereo()     { return true; }
bool CoreAudioFormat::canDoMono()       { return true; }

//==============================================================================
AudioFormatReader* CoreAudioFormat::createReaderFor (InputStream* sourceStream,
                                                     bool deleteStreamIfOpeningFails)
{
    ScopedPointer<CoreAudioReader> r (new CoreAudioReader (sourceStream));

    if (r->ok)
        return r.release();

    if (! deleteStreamIfOpeningFails)
        r->input = nullptr;

    return nullptr;
}

AudioFormatWriter* CoreAudioFormat::createWriterFor (OutputStream* streamToWriteTo,
                                                     double sampleRateToUse,
                                                     unsigned int numberOfChannels,
                                                     int bitsPerSample,
                                                     const StringPairArray& metadataValues,
                                                     int qualityOptionIndex)
{
    CoreAudioWriter* newWriter = new CoreAudioWriter (streamToWriteTo, sampleRateToUse, (int) numberOfChannels, bitsPerSample, metadataValues);
    if (newWriter != nullptr && ! newWriter->writeFailed)
        return newWriter;
    
    return nullptr;
}

#undef CoreAudioFormat
#endif