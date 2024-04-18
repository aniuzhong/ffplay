#ifndef FFPLAY_FRAME_QUEUE_H
#define FFPLAY_FRAME_QUEUE_H

#include <SDL.h>
#include <SDL_thread.h>

#include <libavcodec/avcodec.h>

#define VIDEO_PICTURE_QUEUE_SIZE 3
#define SUBPICTURE_QUEUE_SIZE 16
#define SAMPLE_QUEUE_SIZE 9
#define FRAME_QUEUE_SIZE FFMAX(SAMPLE_QUEUE_SIZE, FFMAX(VIDEO_PICTURE_QUEUE_SIZE, SUBPICTURE_QUEUE_SIZE))

typedef struct PacketQueue PacketQueue;

/* Common struct for handling all types of decoded data and allocated render buffers. */
typedef struct Frame {
    AVFrame *frame;
    AVSubtitle sub;
    int serial;
    double pts;           /* presentation timestamp for the frame */
    double duration;      /* estimated duration of the frame */
    int64_t pos;          /* byte position of the frame in the input file */
    int width;
    int height;
    int format;
    AVRational sar;
    int uploaded;
    int flip_v;
} Frame;

typedef struct FrameQueue {
    Frame queue[FRAME_QUEUE_SIZE];
    int rindex;
    int windex;
    int size;
    int max_size;
    int keep_last;
    int rindex_shown;
    SDL_mutex *mutex;
    SDL_cond *cond;
    PacketQueue *pktq;
} FrameQueue;

typedef struct FrameData {
    int64_t pkt_pos;
} FrameData;

void    frame_queue_unref_item(Frame *vp);
int     frame_queue_init(FrameQueue *f, PacketQueue *pktq, int max_size, int keep_last);
void    frame_queue_destroy(FrameQueue *f);
void    frame_queue_signal(FrameQueue *f);
Frame  *frame_queue_peek(FrameQueue *f);
Frame  *frame_queue_peek_next(FrameQueue *f);
Frame  *frame_queue_peek_last(FrameQueue *f);
Frame  *frame_queue_peek_writable(FrameQueue *f);
Frame  *frame_queue_peek_readable(FrameQueue *f);
void    frame_queue_push(FrameQueue *f);
void    frame_queue_next(FrameQueue *f);
int     frame_queue_nb_remaining(FrameQueue *f);
int64_t frame_queue_last_pos(FrameQueue *f);

#endif // FFPLAY_FRAME_QUEUE_H