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
    int paused;
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
    int width;
    int height;
    enum ShowMode {
        SHOW_MODE_NONE = -1, SHOW_MODE_VIDEO = 0, SHOW_MODE_WAVES, SHOW_MODE_RDFT, SHOW_MODE_NB
    } show_mode;

    // Additional
    SDL_Renderer *renderer;
} VideoState;

VideoState    *stream_open(const char *filename);
void           stream_close(VideoState *is);
int            stream_component_open(VideoState *is, int stream_index);
void           stream_component_close(VideoState *is, int stream_index);
int            read_thread(void *arg);
int            audio_thread(void *arg);
int            video_thread(void *arg);
int            subtitle_thread(void *arg);
int            get_video_frame(VideoState *is, AVFrame *frame);

#endif // FFPLAY_VIDEO_STATE
