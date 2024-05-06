#ifndef FFPLAY_VIDEO_STATE
#define FFPLAY_VIDEO_STATE

#include <libavformat/avformat.h>

#include "clock.h"
#include "packet_queue.h"
#include "frame_queue.h"
#include "decoder.h"

typedef struct VideoState {
    char *filename;
    SDL_Thread *read_tid;
    int abort_request;
    SDL_cond *continue_read_thread;
    AVFormatContext *ic;
    int eof;
    int audio_stream;
    int video_stream;
    int subtitle_stream;
    AVStream *audio_st;
    AVStream *video_st;
    AVStream *subtitle_st;
    Clock audclk;
    Clock vidclk;
    Clock extclk;
    FrameQueue sampq;
    FrameQueue pictq;
    FrameQueue subpq;
    PacketQueue audioq;
    PacketQueue videoq;
    PacketQueue subtitleq;
    Decoder auddec;
    Decoder viddec;
    Decoder subdec;
} VideoState;

VideoState    *stream_open(const char *filename);
void           stream_close(VideoState *is);
int            stream_component_open(VideoState *is, int stream_index);
void           stream_component_close(VideoState *is, int stream_index);

#endif // FFPLAY_VIDEO_STATE