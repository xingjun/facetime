#include "util_log.h"
#include "ffmpeg.h"
#if 0
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavdevice/avdevice.h>
#endif

int main () {
    const char* camera_format = "video4linux2";
    const char* camera_device = "/dev/video0";
    av_register_all ();
    avdevice_register_all();

    AVFormatContext *fmtCtx = NULL;
    AVFormatParameters inputFmtParameter;
    AVCodecContext *decodeCtx;
    AVCodec *decoder;
 
/* 获取这个输入设备的处理器，以便进行处理 */
    AVInputFormat *inputFmt = av_find_input_format (camera_format); 
    if (inputFmt == NULL) {
        /* 找不到这个“输入设备”，错误处理  */
        ERROR ("can not find input deivce: %s", camera_format);
        return -1;
    }
 
/* 使用这个输入处理器“打开”一个文件。在Linux中，这自然是一视频设备的设备文件了 */
    memset ( &inputFmtParameter, 0, sizeof(inputFmtParameter));
    inputFmtParameter.width  = 320;
    inputFmtParameter.height = 240;
 
/*
 * 在这里我们使用AVFormatParameter 这个结构，向输入处理器传递参数
 * 这里向video4linux2 device传递“想要的”尺寸。设备会返回一个相近的能接受的尺寸。
 * 你大可以什么都不传，只传NULL从而使用默认参数。
 */
 
    if (av_open_input_file ( &fmtCtx, camera_device, inputFmt,
                             sizeof(inputFmtParameter),
                             &inputFmtParameter) < 0 ){
        /* 打开“输入设备”失败，错误处理  */
        ERROR ("can not open device: %s", camera_device);
        return -1;
    }

}
