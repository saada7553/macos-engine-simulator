#ifndef S_AUDIO_FILE_H
#define S_AUDIO_FILE_H

typedef unsigned int SampleOffset; 

class sAudioFile {
public: 
    sAudioFile(); 
    ~sAudioFile(); 

    enum class AudioFormat {
        Wave, 
        Undefined, 
    }; 

    enum class Error {
        None, 
        CouldNotOpenFile, 
        InvalidFileFormat, 
        FileAlreadyOpen, 
        NoFileOpen, 
        ReadOutOfRange, 
        FileReadError, 
        CouldNotLockBuffer, 
        InvalidParam,
    }; 

    Error OpenFile(const char* fname); 
    Error CloseFile(); 
    Error InitializeInternalBuffer(SampleOffset samples, bool saveData = false); 
    Error FillBuffer(SampleOffset offset); 

    const void* GetBuffer() const { return m_buffer; }
    void DestroyInternalBuffer(); 
    SampleOffset GetSampleCount() const { return m_sampleCount; }


protected: 
    Error GenericRead(SampleOffset offset, SampleOffset size, void* buffer);
    unsigned int m_dataSegmentOffset; 
    char* m_buffer; 
    SampleOffset m_sampleCount; 
}; 

#endif