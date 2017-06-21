#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wavio.h"

static char* ErrMessage[] = {
  "OK",
  "I/O error",
  "Memory allocation failed",
  "Illegal format",
  "Unsupported format",
  "Null pointer exception",
  "Illegal value specified",
};

char *SndStrError(int err)
{
  if (err >= 0 && err < SND_ERRCODE_FINAL)
    return ErrMessage[err];
  return "Unknown error code";
}

#if 0
static int read_2byte_be(FILE *f)
{
  char buf[2];
  fread(buf,sizeof(char),2,f);
  return ((buf[0]&0xff)<<8)|(buf[1]&0xff);
}

static int read_4byte_be(FILE *f)
{
  char buf[4];
  fread(buf,sizeof(char),4,f);
  return ((buf[0]&0xff)<<24)|((buf[1]&0xff)<<16)|((buf[2]&0xff)<<8)|(buf[3]&0xff);
}
#endif

static int read_2byte_le(FILE *f)
{
  char buf[2];
  fread(buf,sizeof(char),2,f);
  return ((buf[1]&0xff)<<8)|(buf[0]&0xff);
}

static int read_4byte_le(FILE *f)
{
  char buf[4];
  fread(buf,sizeof(char),4,f);
  return ((buf[3]&0xff)<<24)|((buf[2]&0xff)<<16)|((buf[1]&0xff)<<8)|(buf[0]&0xff);
}

static void write_2byte_le(int x, FILE *f)
{
  char buf[2];
  buf[0] = (x & 0xff);
  buf[1] = ((x >> 8) & 0xff);
  fwrite(buf,sizeof(char),2,f);
}

static void write_4byte_le(int x, FILE *f)
{
  char buf[4];
  buf[0] = (x & 0xff);
  buf[1] = ((x >> 8) & 0xff);
  buf[2] = ((x >> 16) & 0xff);
  buf[3] = ((x >> 24) & 0xff);
  fwrite(buf,sizeof(char),4,f);
}
  

/*
 * checks if the file contains the specified characters
 */
static int check_format(char *fmtstring, FILE *f)
{
  int i,len;
  char c;
  len = strlen(fmtstring);
  for (i = 0; i < len; i++) {
    c = fgetc(f);
    if (ferror(f))
      return SND_IO_ERROR;
    if (feof(f) || c != fmtstring[i])
      return SND_FORMAT_ERROR;
  }
  return SND_OK;
}

static int
readFormatChunk(FormatChunk *ch, FILE *f)
{
  int chunksize;
  int ret;
  int pos;
  if ((ret = check_format("fmt ",f)) != SND_OK)
    return ret;
  chunksize = read_4byte_le(f);
  ch->fmt_id       = read_2byte_le(f);
  ch->channels     = read_2byte_le(f);
  ch->samplerate   = read_4byte_le(f);
  ch->byterate     = read_4byte_le(f);
  ch->blocksize    = read_2byte_le(f);
  ch->bitpersample = read_2byte_le(f);
  if (feof(f))
    return SND_FORMAT_ERROR;
  if (ferror(f))
    return SND_IO_ERROR;

  pos = ftell(f);
  if (check_format("data",f) == SND_OK)
    return SND_OK;
  fseek(f,pos,SEEK_SET);

  ch->extensionsize= read_2byte_le(f);
  if (ch->extensionsize == 0)
    ch->extension = NULL;
  else {
    ch->extension = malloc(ch->extensionsize);
    if (ch->extension == NULL)
      return SND_ALLOC_ERROR;
    if (fread(ch->extension, sizeof(char),ch->extensionsize,f) != 
	ch->extensionsize)
      return SND_FORMAT_ERROR;
  }
  if (check_format("data",f) == SND_OK)
    return SND_OK;
  return SND_FORMAT_ERROR;
}
  

/*
 * read wav header. 
 */
static int 
readWavHeader(WavHeader *head, FILE *f)
{
  int ret;

  if ((ret = check_format("RIFF",f)) != SND_OK)
    return ret;
  head->size = read_4byte_le(f);
  if ((ret = check_format("WAVE",f)) != SND_OK)
    return ret;
  if ((ret = readFormatChunk(&head->fmt, f)) != SND_OK)
    return ret;
  head->datasize = read_4byte_le(f);
  return SND_OK;
}

/*
 * free sounddata
 */
void
freeSoundData(SoundData *s)
{
  int i;
  for (i = 0; i < s->ch; i++) {
    if (s->data[i] != NULL)
      free(s->data[i]);
    if (s->header.fmt.extensionsize > 0)
      free(s->header.fmt.extension);
    free(s);
  }
}
  

SoundData *
createSoundData()
{
  SoundData *s = (SoundData*)malloc(sizeof(SoundData));
  if (s == NULL) {
    return NULL;
  }
  memset(s,0,sizeof(SoundData));
  return s;
}  

/*
 * read .wav file and returns data.
 * returns NULL upon failure.
 */

SoundData *
readWavFile(char *filename, int *ret_val)
{
  FILE *f;
  int ret;
  int samplesize;
  int i,ch;

  f = fopen(filename,"rb");
  if (f == NULL) {
    if (ret_val) *ret_val = SND_IO_ERROR;
    return NULL;
  }
  SoundData *s = createSoundData();
  if (s == NULL) {
    if (ret_val) *ret_val = SND_ALLOC_ERROR;
    fclose(f);
    return NULL;
  }
  if ((ret = readWavHeader(&s->header,f)) != SND_OK) {
    freeSoundData(s);
    fclose(f);
    if (ret_val) *ret_val = ret;
    return NULL;
  }

  /* set parameters */
  s->ch = s->header.fmt.channels;
  if (s->ch != 1 && s->ch != 2) {
    if (ret_val) *ret_val = SND_FORMAT_UNSUPPORTED;
    fclose(f);
    freeSoundData(s);
    return NULL;
  }
  s->size = s->header.datasize/s->header.fmt.blocksize;
  samplesize = s->header.fmt.blocksize/s->ch;
  for (ch = 0; ch < s->ch; ch++) {
    s->data[ch] = malloc(sizeof(short)*s->size);
    if (s->data[ch] == NULL) {
      if (ret_val) *ret_val = SND_ALLOC_ERROR;
      fclose(f);
      freeSoundData(s);
      return NULL;
    }
  }
  if (samplesize == 1) {
    /* 1byte/sample */
    for (i = 0; i < s->size; i++) {
      for (ch = 0; ch < s->ch; ch++) {
	s->data[ch][i] = (int)((unsigned char)fgetc(f)) - 128;
	if (feof(f) || ferror(f)) {
	  if (ret_val) *ret_val = SND_IO_ERROR;
	  s->size = i;
	  fclose(f);
	  return s;
	}
      }
    }
  } else if (samplesize == 2) {
    /* 2byte/sample */
    for (i = 0; i < s->size; i++) {
      for (ch = 0; ch < s->ch; ch++) {
	s->data[ch][i] = read_2byte_le(f);
	if (feof(f) || ferror(f)) {
	  if (ret_val) *ret_val = SND_IO_ERROR;
	  s->size = i;
	  fclose(f);
	  return s;
	}
      }
    }
  } else {
    if (ret_val) *ret_val = SND_FORMAT_UNSUPPORTED;
    fclose(f);
    freeSoundData(s);
    return NULL;
  }
  return s;
}

int
writeWavFile(SoundData *s, char *filename)
{
  FILE *f;
  int i,ch;
  int samplesize = s->header.fmt.blocksize/s->ch;

  f = fopen(filename,"wb");
  if (f == NULL)
    return SND_IO_ERROR;
  fputs("RIFF",f);
  write_4byte_le(s->header.size,f);
  fputs("WAVEfmt ",f);
  write_4byte_le(16,f);
  write_2byte_le(s->header.fmt.fmt_id,f);
  write_2byte_le(s->header.fmt.channels,f);
  write_4byte_le(s->header.fmt.samplerate,f);
  write_4byte_le(s->header.fmt.byterate,f);
  write_2byte_le(s->header.fmt.blocksize,f);
  write_2byte_le(s->header.fmt.bitpersample,f);
  fputs("data",f);
  write_4byte_le(s->header.datasize,f);
  for (i = 0; i < s->size; i++) {
    for (ch = 0; ch < s->ch; ch++) {
      if (samplesize == 1)
	fputc(s->data[ch][i]+128,f);
      else /* samplesize == 2 */
	write_2byte_le(s->data[ch][i],f);
    }
  }
  fclose(f);
  return SND_OK;
}

int
setWavProfile(SoundData *s, int rate, int ch, int bytepersample)
{
  if (ch != 1 && ch != 2)
    return SND_FORMAT_UNSUPPORTED;
  if (bytepersample != 1 && bytepersample != 2)
    return SND_FORMAT_UNSUPPORTED;
  s->header.fmt.fmt_id = WAV_FMT_PCM;
  s->header.fmt.channels = ch;
  s->header.fmt.samplerate = rate;
  s->header.fmt.byterate = rate*bytepersample;
  s->header.fmt.blocksize = ch*bytepersample;
  s->header.fmt.bitpersample = 8*bytepersample;

  s->ch = ch;
  return SND_OK;
}

int
allocSoundData(SoundData *s, int samples)
{
  int ch;
  if (s->header.fmt.fmt_id == WAV_FMT_UNKNOWN) {
    /* not initialized */
    return SND_FORMAT_ERROR;
  }
  for (ch = 0; ch < s->ch; ch++) {
    s->data[ch] = malloc(sizeof(short)*samples);
    if (s->data[ch] == NULL) {
      if (ch == 1)
	free(s->data[0]);
      return SND_ALLOC_ERROR;
    }
    memset(s->data[ch],0,sizeof(short)*samples);
  }
  s->size = samples;
  s->header.datasize = samples*s->header.fmt.blocksize;
  s->header.size = s->header.datasize+36;
  return SND_OK;
}

int
copyWavProfile(SoundData *s, SoundData *src)
{
  if (s == NULL)
    return SND_NULLPOINTER;
  s->ch = src->ch;
  s->header = src->header;
  s->header.size = s->header.datasize = s->size = 0;
  return SND_OK;
}

int 
copySoundData(SoundData *s, SoundData *src)
{
  int ret,ch;
  if ((ret = copyWavProfile(s,src)) != SND_OK)
    return ret;
  allocSoundData(s,src->size);
  for (ch = 0; ch < s->ch; ch++)
    memcpy(s->data[ch],src->data[ch],s->size*sizeof(short));
  return SND_OK;
}

int
setLength(SoundData *s, int samples)
{
  if (s->size < samples)
    return SND_ILLEGAL_VALUE;
  s->size = samples;
  s->header.datasize = samples*s->header.fmt.blocksize;
  s->header.size = s->header.datasize+36;
  return SND_OK;
}

  
