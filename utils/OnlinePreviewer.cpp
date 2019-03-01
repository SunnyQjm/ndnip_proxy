//
// Created by mingj on 19-3-1.
//

#include "OnlinePreviewer.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <vlc/vlc.h>
#include <iostream>

void OnlinePreviewer::preview(const std::string& filePath) {
    std::cout << "filePath: " << filePath << std::endl;
    libvlc_instance_t *inst;
    libvlc_media_player_t *mp;
    libvlc_media_t *m;

    /* Load the VLC engine */
    inst = libvlc_new(0, nullptr);

    /* Create a new item */
    // 读取一个网络视频位置
    //m = libvlc_media_new_location (inst, "http://mycool.movie.com/test.mov");
    // 读取一个本地视频文件
    m = libvlc_media_new_path(inst, filePath.c_str());

    /* Create a media player playing environement */
    mp = libvlc_media_player_new_from_media(m);

    /* No need to keep the media now */
    libvlc_media_release(m);

#if 0
    /* This is a non working code that show how to hooks into a window,
    * if we have a window around */
    libvlc_media_player_set_xwindow (mp, xid);
    /* or on windows */
    libvlc_media_player_set_hwnd (mp, hwnd);
    /* or on mac os */
    libvlc_media_player_set_nsobject (mp, view);
#endif

    /* play the media_player */
    libvlc_media_player_play(mp);

    sleep(10); /* Let it play a bit */
//
//    /* Stop playing */
//    libvlc_media_player_stop(mp);
//
//    /* Free the media_player */
//    libvlc_media_player_release(mp);
//
//    libvlc_release(inst);
}
