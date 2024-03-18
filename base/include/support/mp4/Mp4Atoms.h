/*------------------------------------------------------------------------------------------------------------*
 *                                                                                                            *
 * Copyright      2021 MiTAC International Corp.                                                              *
 *                                                                                                            *
 *------------------------------------------------------------------------------------------------------------*
 * PROJECT     :  Common Framework                                                                            *
 * BINARY NAME :  libBase                                                                                     *
 * FILE NAME   :  support/mp4/Mp4Atoms.h                                                                      *
 * CREATED BY  :  Huah Tu <huah.tu@mic.com.tw>                                                                *
 * CREATED DATE:  06/23/21 (MM/DD/YY)                                                                         *
 * DESCRIPTION :  Structures/classes for MP4 atoms.                                                           *
 *------------------------------------------------------------------------------------------------------------*/

#ifndef _SUPPORT_MP4_MP4_ATOMS_H
#define _SUPPORT_MP4_MP4_ATOMS_H

// libBase include
#include <container/List.h>

// 1st Level
#define MP4_TAG_ftyp    0x70797466
#define MP4_TAG_mdat    0x7461646D
#define MP4_TAG_moov    0x766F6F6D
#define MP4_TAG_free    0x65657266
// 2nd level
#define MP4_TAG_mvhd    0x6468766D
#define MP4_TAG_trak    0x6B617274
// 3rd level
#define MP4_TAG_tkhd    0x64686B74
#define MP4_TAG_mdia    0x6169646D
// 4th level
#define MP4_TAG_mdhd    0x6468646D
#define MP4_TAG_hdlr    0x726C6468
#define MP4_TAG_minf    0x666E696D
// 5th level
#define MP4_TAG_stbl    0x6C627473
// 6th level
#define MP4_TAG_stsd    0x64737473
#define MP4_TAG_stts    0x73747473
#define MP4_TAG_stss    0x73737473
#define MP4_TAG_stsz    0x7A737473
#define MP4_TAG_stsc    0x63737473
#define MP4_TAG_stco    0x6F637473
#define MP4_TAG_co64    0x34366F63

#define MP4_TRACK_TYPE_VIDEO    0x65646976
#define MP4_TRACK_TYPE_SOUND    0x6E756F73
#define MP4_TRACK_TYPE_METADATA 0x6174656D

#define MP4_TRACK_FORMAT_AVC1   0x31637661
#define MP4_TRACK_FORMAT_SOWT   0x74776F73
#define MP4_TRACK_FORMAT_METT   0x7474656D
#define MP4_TRACK_FORMAT_MP4A   0x6134706D

class Mp4Context;
class Mp4MoovAtom;
class Mp4TrakAtom;
class Mp4MdiaAtom;
class Mp4MinfAtom;
class Mp4StblAtom;
class Mp4SttsAtom;
class Mp4StssAtom;
class Mp4StszAtom;
class Mp4StscAtom;
class Mp4StcoAtom;

struct Mp4AtomHeader
{
    int size;
    int tag;
};

class Mp4Atom
{
  public:
    Mp4Context *context;
    Mp4Atom *parent;
    List<Mp4Atom *> children;
    int level;
    int offset;
    Mp4AtomHeader header;
    bool byExtendAtomSize = false;
    char *payload = 0;

    virtual ~Mp4Atom();

    bool is(int tag);

    // 1. If buf is 0, payload will be prepared and used.
    // 2. For mdat, MIO_ERR_NOT_SUPPROTED will be returned.
    int loadPayload(void *buf = 0);
    // if buf is 0, payload will be used, and if payload is 0 too, MIO_ERR_INCORRECT_STATUS is returned.
    int updatePayload(void *buf = 0);
    // Will load children's payloads if children.size() > 0.  Otherwise, loadPayload(0) is called.
    int loadChildrenPayloads(void);
    // Will save data (header and payload) hierarchically.
    int save();
    int save(int _fd);
    int updateInt(int offsetInAtom, int data);
    int updateSize();

    Mp4Atom *lookForChildAtom(int tag);
    // 1. It will sync sizes and offets hierarchically.
    // 2. Special atoms will override this function to sync table back to payload.  For this case, pre-condition
    //    will be checked.
    // 3. Negative value will be returned for any error.  If succeed, new atom size is returned.
    virtual int syncData(int offset);

    // 1. Return 0 if failed or not support.
    // 2. Default behaviour is to copy payload if no children, that is, payload should be valid; or call
    //    children's duplicate() recursively.
    // 3. loadChildrenPayloads() should be called explicitly before duplicate().
    virtual Mp4Atom *duplicate(Mp4Context *context, Mp4Atom *parent);

    // Not yet added to children, it should be done by clients.
    Mp4Atom *newChildFreeAtom(int offset, int size);

  protected:
    Mp4Atom(Mp4Context *context, Mp4Atom *parent, int offset);
    Mp4Atom(Mp4Context *context, Mp4Atom *parent, int offset, Mp4AtomHeader &header);

    // Default do nothing and return MIO_GENERAL_OK.
    virtual int init(bool silent);
    virtual Mp4Atom *newChildAtom(int offset, Mp4AtomHeader &header);

    int scanChildAtoms(bool silent);
    // 1. payload should not be 0, otherwise, only header.size is changed to newSize.
    // 2. If newSize is bigger than header.size, new payload is allocated, and original payload content will be
    //    copied to new payload.
    void resize(int newSize);

    bool duplicateChildAtoms(Mp4Context *context, Mp4Atom *another);
    void duplicatePayloadTo(Mp4Atom *another);

    int fd(void);

    void logAtom();
    static void logAtom(int level, Mp4AtomHeader &header, int offset);

  private:
    // Private copy constructor is declared but not defined to prevent accident copy.
    Mp4Atom(const Mp4Atom &);
    // Private assignment operator is declared but not defined to prevent accident assignment.
    Mp4Atom &operator=(const Mp4Atom &);

    int _save(int _fd);

    friend class List<Mp4Atom *>;
    friend class Mp4Context;
};

class Mp4RootAtom : public Mp4Atom
{
  public:
    Mp4Atom *locateMDatAtom();
    Mp4MoovAtom *locateMoovAtom();
    Mp4TrakAtom *locateTrackAtom(int trackType);
    Mp4TrakAtom *locateVideoTrackAtom();
    Mp4TrakAtom *locateAudioTrackAtom();
    Mp4TrakAtom *locateMDataTrackAtom();

  protected:
    virtual ~Mp4RootAtom();

    virtual int init(bool silent);
    virtual Mp4Atom* newChildAtom(int offset, Mp4AtomHeader &header);

  private:
    Mp4RootAtom(Mp4Context *context, int fileSize);

    int calculateNewFileSize();

    friend class Mp4Context;
};

class Mp4MoovAtom : public Mp4Atom
{
  public:
    int totalTracks;

    virtual ~Mp4MoovAtom();

    Mp4Atom *locateMvhdAtom(void);
    Mp4TrakAtom *locateTrackAtom(int trackType);
    Mp4TrakAtom *locateVideoTrackAtom();
    Mp4TrakAtom *locateAudioTrackAtom();
    Mp4TrakAtom *locateMDataTrackAtom();

    virtual Mp4Atom *duplicate(Mp4Context *context, Mp4Atom *parent);

  protected:
    virtual int init(bool silent);
    virtual Mp4Atom* newChildAtom(int offset, Mp4AtomHeader &header);

  private:
    Mp4MoovAtom(Mp4Context *context, Mp4Atom *parent, int offset, Mp4AtomHeader &header);

    friend class Mp4RootAtom;
};

class Mp4TrakAtom : public Mp4Atom
{
  public:
    // Legal value should be the big endian of "vide", "soun", and maybe "meta".
    int trackType = 0;
    // Legal values include the big endian of "avc1", "sowt", and maybe "mett".
    int trackFormat = 0;
    int timeScale = 0;

    Mp4Atom *locateTkhdAtom(void);
    Mp4MdiaAtom *locateMdiaAtom(void);
    Mp4Atom *locateMdhdAtom(void);
    Mp4MinfAtom *locateMinfAtom(void);
    Mp4StblAtom *locateStblAtom(void);
    Mp4SttsAtom *locateSttsAtom(void);
    Mp4StszAtom *locateStszAtom(void);
    Mp4StscAtom *locateStscAtom(void);

    virtual Mp4Atom *duplicate(Mp4Context *context, Mp4Atom *parent);

  protected:
    virtual ~Mp4TrakAtom();

    virtual int init(bool silent);
    virtual Mp4Atom* newChildAtom(int offset, Mp4AtomHeader &header);

  private:
    Mp4TrakAtom(Mp4Context *context, Mp4Atom *parent, int offset, Mp4AtomHeader &header);

    friend class Mp4MoovAtom;
};

class Mp4MdiaAtom : public Mp4Atom
{
  public:
    Mp4Atom *locateMdhdAtom(void);
    Mp4MinfAtom *locateMinfAtom(void);
    Mp4StblAtom *locateStblAtom(void);
    Mp4SttsAtom *locateSttsAtom(void);
    Mp4StszAtom *locateStszAtom(void);
    Mp4StscAtom *locateStscAtom(void);

    virtual Mp4Atom *duplicate(Mp4Context *context, Mp4Atom *parent);

  protected:
    virtual ~Mp4MdiaAtom();

    virtual int init(bool silent);
    virtual Mp4Atom* newChildAtom(int offset, Mp4AtomHeader &header);

  private:
    Mp4MdiaAtom(Mp4Context *context, Mp4Atom *parent, int offset, Mp4AtomHeader &header);

    friend class Mp4TrakAtom;
};

class Mp4MinfAtom : public Mp4Atom
{
  public:
    Mp4StblAtom *locateStblAtom(void);
    Mp4SttsAtom *locateSttsAtom(void);
    Mp4StszAtom *locateStszAtom(void);
    Mp4StscAtom *locateStscAtom(void);

    virtual Mp4Atom *duplicate(Mp4Context *context, Mp4Atom *parent);

  protected:
    virtual ~Mp4MinfAtom();

    virtual int init(bool silent);
    virtual Mp4Atom* newChildAtom(int offset, Mp4AtomHeader &header);

  private:
    Mp4MinfAtom(Mp4Context *context, Mp4Atom *parent, int offset, Mp4AtomHeader &header);

    friend class Mp4MdiaAtom;
};

class Mp4StblAtom : public Mp4Atom
{
  public:
    struct Chunk
    {
        int groupNdx;
        int offset;
        int totalBytes;
        int startSampleNdx;
        int samples;
    };

    // The following data members are only valid after loadTables().
    int totalChunks = 0;
    Chunk *chunks = 0;
    int totalSamples = 0;
    int dataEnd = 0;

    Mp4SttsAtom *locateSttsAtom(void);
    Mp4StssAtom *locateStssAtom(void);
    Mp4StszAtom *locateStszAtom(void);
    Mp4StscAtom *locateStscAtom(void);
    Mp4StcoAtom *locateStcoAtom(void);

    // Load all stts/stsz/stsc/stco tables for further processing.
    int loadTables(bool stssToo, bool silent = false);

    virtual Mp4Atom *duplicate(Mp4Context *context, Mp4Atom *parent);

  protected:
    virtual ~Mp4StblAtom();

    virtual int init(bool silent);
    virtual Mp4Atom* newChildAtom(int offset, Mp4AtomHeader &header);


  private:
    Mp4StblAtom(Mp4Context *context, Mp4Atom *parent, int offset, Mp4AtomHeader &header);

    friend class Mp4MinfAtom;
};

class Mp4SttsAtom : public Mp4Atom
{
  public:
    struct SampleTime
    {
        int count;
        int duration;
    };

    int totalEntries = 0;
    // Only valid after loadSampleTimeTable().
    int totalSamples = 0;
    SampleTime *sampleTimeTable = 0;

    // If usrTable is 0, sampleTimeTable is prepared and used.
    int loadSampleTimeTable(SampleTime *usrTable = 0);
    // totalEntries/sampleTimeTable should ready.  Otherwise, 0 is returned,
    int countSamples(void);
    // sampleTimeTable should be prepared and loaded already.
    int cutSamples(int newTotalSamples);

    virtual int syncData(int offset);

    virtual Mp4Atom *duplicate(Mp4Context *context, Mp4Atom *parent);

  protected:
    virtual ~Mp4SttsAtom();

    virtual int init(bool silent);

  private:
    Mp4SttsAtom(Mp4Context *context, Mp4Atom *parent, int offset, Mp4AtomHeader &header);

    friend class Mp4StblAtom;
};

class Mp4StssAtom : public Mp4Atom
{
  public:
    int totalKeySamples;
    // Only valid after loadKeySampleIndices().
    int lastKeySampleNdx = 0;
    int *keySampleIndices = 0;

    // If buf is 0, keySampleIndices is prepared and used.
    int loadKeySampleIndices(int *buf = 0, bool silent = false);

    virtual int syncData(int offset);

    virtual Mp4Atom *duplicate(Mp4Context *context, Mp4Atom *parent);

  protected:
    virtual ~Mp4StssAtom();

    virtual int init(bool silent);

  private:
    Mp4StssAtom(Mp4Context *context, Mp4Atom *parent, int offset, Mp4AtomHeader &header);

    friend class Mp4StblAtom;
};

class Mp4StszAtom : public Mp4Atom
{
  public:
    int uniformSampleBytes;
    int totalSamples;
    int *sampleBytesTable = 0;

    // If buf is 0, sampleBytesTable is prepared and used.
    int loadSampleBytesTable(int *buf = 0);
    // 1. If uniformSampleBytes is 0, loadSampleBytesTable() is required befor using this function.  Otherwise,
    //    0 is returned.
    // 2. ndx is 0 based.
    int iterateSampleBytes(int ndx, int count);
    int cutSamples(int newTotalSamples);

    virtual int syncData(int offset);

    virtual Mp4Atom *duplicate(Mp4Context *context, Mp4Atom *parent);

  protected:
    virtual ~Mp4StszAtom();

    virtual int init(bool silent);

  private:
    Mp4StszAtom(Mp4Context *context, Mp4Atom *parent, int offset, Mp4AtomHeader &header);

    friend class Mp4StblAtom;
};

class Mp4StscAtom : public Mp4Atom
{
  public:
    struct ChunkSamples
    {
        int chunkNdx;
        int samples;
        // Currently, only 1 is supported.
        int descNdx;
        // How may chunks to use this entry.  For example, chunkNdx is 5, samples is 22, chunkCnt is 3, it means
        // that chunk 5, 6, 7 all contains 22 samples.
        int chunkCnt;
    };

    int totalEntries;
    // Only valid after loadChunkSamplesTable().
    int totalChunks;
    ChunkSamples *chunkSamplesTable = 0;

    // 1. If usrTable is 0, chunkSamplesTable is prepared and used.
    // 2. refTotalSamples is used to calculate last chunkCnt and check data consistence.
    // 3. Because there are may checks inside this function, it is better to log error inside.
    int loadChunkSamplesTable(int refTotalSamples, ChunkSamples *usrTable = 0, bool silent = false);
    int cutSamples(int newTotalSamples);

    virtual int syncData(int offset);

    virtual Mp4Atom *duplicate(Mp4Context *context, Mp4Atom *parent);

  protected:
    virtual ~Mp4StscAtom();

    virtual int init(bool silent);

  private:
    Mp4StscAtom(Mp4Context *context, Mp4Atom *parent, int offset, Mp4AtomHeader &header);

    friend class Mp4StblAtom;
};

class Mp4StcoAtom : public Mp4Atom
{
  public:
    int totalChunks;
    int *offsets = 0;

    // If buf is 0, offsets is prepared and used.
    virtual int loadOffsets(int *buf = 0);

    virtual int syncData(int offset);

    virtual Mp4Atom *duplicate(Mp4Context *context, Mp4Atom *parent);

  protected:
    Mp4StcoAtom(Mp4Context *context, Mp4Atom *parent, int offset, Mp4AtomHeader &header);
    virtual ~Mp4StcoAtom();

    virtual int init(bool silent);

    friend class Mp4StblAtom;
};

class Mp4Co64Atom : public Mp4StcoAtom
{
  public:
    virtual int syncData(int offset);
    virtual Mp4Atom *duplicate(Mp4Context *context, Mp4Atom *parent);

    // If buf is 0, offsets is prepared and used.
    virtual int loadOffsets(int *buf = 0);

  protected:
    virtual ~Mp4Co64Atom();

  private:
    Mp4Co64Atom(Mp4Context *context, Mp4Atom *parent, int offset, Mp4AtomHeader &header);

    virtual int init(bool silent);

    friend class Mp4StblAtom;
};

#endif//_SUPPORT_MP4_MP4_ATOMS_H
