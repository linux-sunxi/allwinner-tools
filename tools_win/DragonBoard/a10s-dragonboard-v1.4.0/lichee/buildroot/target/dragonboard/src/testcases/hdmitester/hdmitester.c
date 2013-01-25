/*
 * \file        hdmitester.c
 * \brief       
 *
 * \version     1.0.0
 * \date        2012年06月20日
 * \author      James Deng <csjamesdeng@allwinnertech.com>
 *
 * Copyright (c) 2012 Allwinner Technology. All Rights Reserved.
 *
 */

#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/unistd.h>
#include <sys/mman.h>
#include <pthread.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#include <asoundlib.h>

#include "drv_display_sun4i.h"
#include "dragonboard_inc.h"

#define COLOR_WHITE                     0xffffffff
#define COLOR_YELLOW                    0xffffff00
#define COLOR_GREEN                     0xff00ff00
#define COLOR_CYAN                      0xff00ffff
#define COLOR_MAGENTA                   0xffff00ff
#define COLOR_RED                       0xffff0000
#define COLOR_BLUE                      0xff0000ff
#define COLOR_BLACK                     0xff000000

static unsigned int colorbar[8] = 
{
    COLOR_WHITE,
    COLOR_YELLOW,
    COLOR_CYAN,
    COLOR_GREEN,
    COLOR_MAGENTA,
    COLOR_RED,
    COLOR_BLUE,
    COLOR_BLACK
};

struct hdmi_output_type
{
    int mode;
    int width;
    int height;
    char name[16];
};

#define TYPE_COUNT                      11

static struct hdmi_output_type type_list[TYPE_COUNT] = {
    {DISP_TV_MOD_480I,       720,  480,  "480I"},       /* 0 */
    {DISP_TV_MOD_576I,       720,  576,  "576I"},       /* 1 */ 
    {DISP_TV_MOD_480P,       720,  480,  "480P"},       /* 2 */
    {DISP_TV_MOD_576P,       720,  576,  "576P"},       /* 3 */
    {DISP_TV_MOD_720P_50HZ,  1280, 720,  "720P 50HZ"},  /* 4 */
    {DISP_TV_MOD_720P_60HZ,  1280, 720,  "720P 60HZ"},  /* 5 */
    {DISP_TV_MOD_1080I_50HZ, 1920, 1080, "1080I 50HZ"}, /* 6 */
    {DISP_TV_MOD_1080I_60HZ, 1920, 1080, "1080I 60HZ"}, /* 7 */
    {DISP_TV_MOD_1080P_24HZ, 1920, 1080, "1080P 24HZ"}, /* 8 */
    {DISP_TV_MOD_1080P_50HZ, 1920, 1080, "1080P 50HZ"}, /* 9 */
    {DISP_TV_MOD_1080P_60HZ, 1920, 1080, "1080P 60HZ"}  /* 10 */
};

static int disp;
static int output_mode;
static int screen_width = 0;
static int screen_height = 0;
static unsigned char *buff = NULL;
static int fb;

static int sound_play_stop;

#define BUF_LEN                         4096
char *buf[BUF_LEN];

static int init_layer(void)
{
    unsigned int args[4];
    __disp_fb_create_para_t fb_para;
    int ret;
    int i, j;
    unsigned int *p;

    memset(&fb_para, 0, sizeof(__disp_fb_create_para_t));
    fb_para.fb_mode = FB_MODE_SCREEN1;
    fb_para.mode = DISP_LAYER_WORK_MODE_NORMAL;
    fb_para.buffer_num = 2;
    fb_para.width = screen_width;
    fb_para.height = screen_height;

    args[0] = 1;
    args[1] = (unsigned int)&fb_para;
    ret = ioctl(disp, DISP_CMD_FB_REQUEST, args);
    if (ret < 0) {
        db_error("request framebuffer failed\n");
        return -1;
    }

    fb = open("/dev/fb1", O_RDWR);
    buff = mmap(NULL, screen_width * screen_height * 4, PROT_READ|PROT_WRITE, MAP_SHARED, fb, 0);
    for (i = 0; i < screen_height; i++)
    {
        p = (unsigned int *)(buff + screen_width * i * 4);
        for (j = 0; j < screen_width; j++)
        {
            int idx = (j << 3) / screen_width;
            *p++ = colorbar[idx];
        }
    }
    close(fb);

    return 0;
}

static void exit_layer(void)
{
    unsigned int args[4];

    munmap(buf, screen_width * screen_height * 4);

    args[0] = 1;
    ioctl(disp, DISP_CMD_FB_RELEASE, args);
}

static int detect_output_mode(void)
{
    unsigned int args[4];
    int ret;
    int support_mode;

    args[0] = 1;
    args[1] = DISP_TV_MOD_1080P_50HZ;
    ret = ioctl(disp, DISP_CMD_HDMI_SUPPORT_MODE, args);
    if (ret == 1) {
        db_msg("hdmitester: your device support 1080p 50Hz\n");
        output_mode = DISP_TV_MOD_1080P_50HZ;
        screen_width  = 1920;
        screen_height = 1080;
    }
    else {
        args[0] = 1;
        args[1] = DISP_TV_MOD_720P_50HZ;
        ret = ioctl(disp, DISP_CMD_HDMI_SUPPORT_MODE, args);
        if (ret == 1) {
            db_msg("hdmitester: your device support 720p 50Hz\n");
            output_mode = DISP_TV_MOD_720P_50HZ;
            screen_width  = 1280;
            screen_height = 720;
        }
        else {
            db_msg("hdmitester: your device do not support neither 1080p nor 720p (50Hz)\n");
            if (script_fetch("hdmi", "support_mode", &support_mode, 1)) {
                support_mode = 2;
                db_msg("hdmitester: can't fetch user config mode, use default mode: %s\n", 
                        type_list[support_mode].name);
            }
            else if (support_mode < 0 || support_mode >= TYPE_COUNT) {
                support_mode = 2;
                db_msg("hdmitester: user config mode invalid, use default mode: %s\n", 
                        type_list[support_mode].name);
            }
            db_msg("hdmitester: use user config mode: %s\n", type_list[support_mode].name);
            args[0] = 1;
            args[1] = type_list[support_mode].mode;
            ret = ioctl(disp, DISP_CMD_HDMI_SUPPORT_MODE, args);
            if (ret == 1) {
                db_msg("hdmitester: you device support %s\n", type_list[support_mode].name);
                output_mode = type_list[support_mode].mode;
                screen_width  = type_list[support_mode].width;
                screen_height = type_list[support_mode].height;
            }
            else {
                db_msg("hdmitester: you device do not support %s\n", 
                        type_list[support_mode].name);
                return -1;
            }
        }
    }

    return 0;
}

static int xrun_recovery(snd_pcm_t *handle, int err)
{
    if (err == -EPIPE) {
        err = snd_pcm_prepare(handle);
    }

    if (err < 0) {
        db_warn("Can't recovery from underrun, prepare failed: %s\n", snd_strerror(err));
    }
    else if (err == -ESTRPIPE) {
        while ((err = snd_pcm_resume(handle)) == -EAGAIN) {
            sleep(1);

            if (err < 0) {
                err = snd_pcm_prepare(handle);
            }
            if (err < 0) {
                db_warn("Can't recovery from suspend, prepare failed: %s\n", snd_strerror(err));
            }
        }

        return 0;
    }

    return err;
}

static void *sound_play(void *args)
{
    char path[256];
    int samplerate;
    int err;
    snd_pcm_t *playback_handle;
    snd_pcm_hw_params_t *hw_params;
    FILE *fp;

    db_msg("prepare play sound...\n");
    if (script_fetch("hdmi", "sound_file", (int *)path, sizeof(path) / 4)) {
        db_warn("unknown sound file, use default\n");
        strcpy(path, "/dragonboard/data/test48000.pcm");
    }
    if (script_fetch("hdmi", "samplerate", &samplerate, 1)) {
        db_warn("unknown samplerate, use default #48000\n");
        samplerate = 48000;
    }
    db_msg("samplerate #%d\n", samplerate);

    err = snd_pcm_open(&playback_handle, "hw:1,0", SND_PCM_STREAM_PLAYBACK, 0);
    if (err < 0) {
        db_error("cannot open audio device (%s)\n", snd_strerror(err));
        pthread_exit((void *)-1);
    }

    err = snd_pcm_hw_params_malloc(&hw_params);
    if (err < 0) {
        db_error("cannot allocate hardware parameter structure (%s)\n", snd_strerror(err));
        pthread_exit((void *)-1);
    }

    err = snd_pcm_hw_params_any(playback_handle, hw_params);
    if (err < 0) {
        db_error("cannot initialize hardware parameter structure (%s)\n", snd_strerror(err));
        pthread_exit((void *)-1);
    }

    err = snd_pcm_hw_params_set_access(playback_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED);
    if (err < 0) {
        db_error("cannot allocate hardware parameter structure (%s)\n", snd_strerror(err));
        pthread_exit((void *)-1);
    }

    err = snd_pcm_hw_params_set_format(playback_handle, hw_params, SND_PCM_FORMAT_S16_LE);
    if (err < 0) {
        db_error("cannot allocate hardware parameter structure (%s)\n", snd_strerror(err));
        pthread_exit((void *)-1);
    }

    err = snd_pcm_hw_params_set_rate(playback_handle, hw_params, samplerate, 0);
    if (err < 0) {
        db_error("cannot set sample rate (%s)\n", snd_strerror(err));
        pthread_exit((void *)-1);
    }

    err = snd_pcm_hw_params_set_channels(playback_handle, hw_params, 2);
    if (err < 0) {
        db_error("cannot set channel count (%s), err = %d\n", snd_strerror(err), err);
        pthread_exit((void *)-1);
    }

    err = snd_pcm_hw_params(playback_handle, hw_params);
    if (err < 0) {
        db_error("cannot set parameters (%s)\n", snd_strerror(err));
        pthread_exit((void *)-1);
    }

    snd_pcm_hw_params_free(hw_params);

    db_msg("open test pcm file: %s\n", path);
    fp = fopen(path, "r");
    if (fp == NULL) {
        db_error("cannot open test pcm file(%s)\n", strerror(errno));
        pthread_exit((void *)-1);
    }

    db_msg("play it...\n");
    while (1) {
        while (!feof(fp)) {
            if (sound_play_stop) {
                goto out;
            }

            err = fread(buf, 1, BUF_LEN, fp);
            if (err < 0) {
                db_warn("read test pcm failed(%s)\n", strerror(errno));
            }

            err = snd_pcm_writei(playback_handle, buf, BUF_LEN/4);
            if (err < 0) {
                err = xrun_recovery(playback_handle, err);
                if (err < 0) {
                    db_warn("write error: %s\n", snd_strerror(err));
                }
            }

            if (err == -EBADFD) {
                db_warn("PCM is not in the right state (SND_PCM_STATE_PREPARED or SND_PCM_STATE_RUNNING)\n");
            }
            if (err == -EPIPE) {
                db_warn("an underrun occurred\n");
            }
            if (err == -ESTRPIPE) {
                db_warn("a suspend event occurred (stream is suspended and waiting for an application recovery)\n");
            }

            if (feof(fp)) {
                fseek(fp, 0L, SEEK_SET);
            }
        }
    }

out:
    db_msg("play end...\n");
    fclose(fp);
    snd_pcm_close(playback_handle);
    pthread_exit(0);
}

int main(int argc, char *argv[])
{
    unsigned int args[4];
    int status = 0;
    int retry = 0;
    int flags = 0;
    int ret;
    pthread_t tid;
    __disp_output_type_t disp_output_type;
    int mic_activated;

    INIT_CMD_PIPE();

    init_script(atoi(argv[2]));

    disp = open("/dev/disp", O_RDWR);
    if (disp == -1) {
        db_error("hdmitester: open /dev/disp failed(%s)\n", strerror(errno));
        goto err;
    }

    args[0] = 0;
    disp_output_type = (__disp_output_type_t)ioctl(disp, DISP_CMD_GET_OUTPUT_TYPE,(void*)args);
     if(script_fetch("mic", "activated", &mic_activated,1)){
        mic_activated=0;  
     }       

    /* test main loop */
    while (1) {
        if(disp_output_type==DISP_OUTPUT_TYPE_LCD)
        args[0] = 1;
        else
        args[0] = 0;
        
        ret = ioctl(disp, DISP_CMD_HDMI_GET_HPD_STATUS, args);
        if (ret == 1) {
            flags = 0;

            if (status == 1) {
                sleep(1);
                continue;
            }

            /* try three times before go on...
             * it will take 3 second.
             */
            if (retry < 3) {
                retry++;
                sleep(1);
                continue;
            }
            
            if(disp_output_type==DISP_OUTPUT_TYPE_LCD){
                   /* detect and set output mode */
                  ret = detect_output_mode();
                  if (ret < 0) {
                       goto err;
                 }

               args[0] = 1;
               args[1] = output_mode;
                   ret = ioctl(disp, DISP_CMD_HDMI_SET_MODE, args);
                if (ret < 0) {
                    db_error("hdmitester: set hdmi output mode failed(%d)\n", ret);
                  goto err;
                 }

               /* init layer */
               ret = init_layer();
                if (ret < 0) {
                      db_error("hdmitester: init layer failed\n");
                       goto err;
                }

               /* set hdmi on */
               args[0] = 1;
               ret = ioctl(disp, DISP_CMD_HDMI_ON, args);
               if (ret < 0) {
                     db_error("hdmitester: set hdmi on failed(%d)\n", ret);
                     exit_layer();
                    goto err;
                 }

                /* create sound play thread */
               sound_play_stop = 0;
                ret = pthread_create(&tid, NULL, sound_play, NULL);
               if (ret != 0) {
                    db_error("hdmitester: create sound play thread failed\n");
                   exit_layer();
                      args[0] = 1;
                    ioctl(disp, DISP_CMD_HDMI_OFF, args);
                    goto err;
                   }
                 
            }else
            if((disp_output_type==DISP_OUTPUT_TYPE_HDMI)&&(!mic_activated)){
               /* create sound play thread */
               sound_play_stop = 0;
                ret = pthread_create(&tid, NULL, sound_play, NULL);
               if (ret != 0) {
                    db_error("hdmitester: create sound play thread failed\n");
                    exit_layer();
                     args[0] = 1;
                    ioctl(disp, DISP_CMD_HDMI_OFF, args);
                    goto err;
                   }

            }
            status = 1;
             SEND_CMD_PIPE_OK();
        }
        else {
            void *retval;

            /* reset retry to 0 */
            retry = 0;

            if (status == 0) {
                sleep(1);
                continue;
            }

            if (flags < 3) {
                flags++;
                sleep(1);
                continue;
            }

            status = 0;
            if(disp_output_type==DISP_OUTPUT_TYPE_LCD){
            /* end sound play thread */
            sound_play_stop = 1;
            db_msg("hdmitester: waiting for sound play thread finish...\n");
            if (pthread_join(tid, &retval)) {
                db_error("hdmitester: can't join with sound play thread\n");
            }
            db_msg("hdmitester: sound play thread exit code #%d\n", (int)retval);

            exit_layer();
            args[0] = 1;
            ioctl(disp, DISP_CMD_HDMI_OFF, args);
            }
            else
            if((disp_output_type==DISP_OUTPUT_TYPE_HDMI)&&(!mic_activated)){
                 /* end sound play thread */
              sound_play_stop = 1;
             db_msg("hdmitester: waiting for sound play thread finish...\n");
             if (pthread_join(tid, &retval)) {
                db_error("hdmitester: can't join with sound play thread\n");
              }
            db_msg("hdmitester: sound play thread exit code #%d\n", (int)retval);
            }
        }

        /* sleep 1 second */
        sleep(1);
    }

err:
    SEND_CMD_PIPE_FAIL();
    close(disp);
    deinit_script();
    return -1;
}
