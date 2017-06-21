#include <sys/types.h>

#define ENDIAN_BIG 0
#define ENDIAN_LITTLE 1

#define WAV_FMT_UNKNOWN    0
#define WAV_FMT_PCM        1
#define WAV_FMT_ALAW       6
#define WAV_FMT_MULAW      7

/* return code */
#define SND_OK                  0
#define SND_IO_ERROR            1
#define SND_ALLOC_ERROR         2
#define SND_FORMAT_ERROR        3
#define SND_FORMAT_UNSUPPORTED  4
#define SND_NULLPOINTER         5
#define SND_ILLEGAL_VALUE       6
#define SND_ERRCODE_FINAL       7  /* dummy */

typedef struct format_chunk {
  int fmt_id;              /* format ID */
  int channels;            /* number of channels */
  int samplerate;          /* sampling rate */
  int byterate;            /* bytes per second */
  int blocksize;           /* bytes per block */
  int bitpersample;        /* bits per sample */
  int extensionsize;       /* size of extension */
  void *extension;         /* extension data */
} FormatChunk;

typedef struct wavheader {
  size_t size;             /* file size */
  FormatChunk fmt;         /* format */
  size_t datasize;         /* data bytes */
} WavHeader;

typedef struct sounddata {
  WavHeader header;
  short ch;                /* channels - for convenience */
  int size;                /* number of samples */
  short *data[2];
} SoundData;

SoundData *createSoundData();
SoundData *readWavFile(char *filename, int *ret_val);
void freeSoundData(SoundData *s);
char *SndStrError(int error);
int writeWavFile(SoundData *s, char *filename);
int setWavProfile(SoundData *s, int rate, int ch, int bytepersample);
int allocSoundData(SoundData *s, int samples);
int copyWavProfile(SoundData *s, SoundData *src);
int copySoundData(SoundData *s, SoundData *src);
int setLength(SoundData *s, int samples);

