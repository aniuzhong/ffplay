#ifndef FFPLAY_CLOCK_H
#define FFPLAY_CLOCK_H

/* external clock speed adjustment constants for realtime sources based on buffer fullness */
#define EXTERNAL_CLOCK_SPEED_MIN  0.900
#define EXTERNAL_CLOCK_SPEED_MAX  1.010
#define EXTERNAL_CLOCK_SPEED_STEP 0.001

enum {
    AV_SYNC_AUDIO_MASTER,   /* default choice */
    AV_SYNC_VIDEO_MASTER,
    AV_SYNC_EXTERNAL_CLOCK, /* synchronize to an external clock */
};

typedef struct Clock {
    double pts;           /* clock base */
    double pts_drift;     /* clock base minus time at which we updated the clock */
    double last_updated;
    double speed;
    int serial;           /* clock is based on a packet with this serial */
    int paused;
    int *queue_serial;    /* pointer to the current packet queue serial, used for obsolete clock detection */
} Clock;

void   init_clock(Clock *c, int *queue_serial);
double get_clock(Clock *c);
void   set_clock(Clock *c, double pts, int serial);
void   set_clock_at(Clock *c, double pts, int serial, double time);
void   set_clock_speed(Clock *c, double speed);
void   sync_clock_to_slave(Clock *c, Clock *slave);

#endif // FFPLAY_CLOCK_H