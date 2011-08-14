/*
 *  Sound.cpp
 *  hex-game
 *
 *  Created by Micke Prag on 2011-09-09.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "Sound.h"
#include "AVFoundation/AVAudioPlayer.h"
#include <map>

class Sound::PrivateImplData {
public:
    std::map<std::string, AVAudioPlayer *> sounds;
};

void Sound::initImpl() {
    pd = new PrivateImplData;
}

void Sound::destroyImpl() {
    for(std::map<std::string, AVAudioPlayer *>::iterator it = pd->sounds.begin(); it != pd->sounds.end(); ++it) {
        [(*it).second release];
    }
    delete pd;
}

void Sound::initFile(const std::string &name) {
    NSString * path;
    AVAudioPlayer * snd;
    NSError *err;
    std::string filename = name + ".caf";

    path = [[[NSBundle mainBundle] resourcePath] stringByAppendingPathComponent: [NSString stringWithCString:filename.c_str() encoding:[NSString defaultCStringEncoding]]];

    if ([[NSFileManager defaultManager] fileExistsAtPath:path]) {
        NSURL * url = [NSURL fileURLWithPath:path];
        snd = [[AVAudioPlayer alloc] initWithContentsOfURL:url error:&err];
        if (!snd) {
            NSLog(@"Sound named '%s' had error %@", filename.c_str(), [err localizedDescription]);
        } else {
            [snd prepareToPlay];
            pd->sounds[name] = snd;
        }
    }
}

void Sound::playImpl(const std::string &filename) {
    if (pd->sounds.find(filename) != pd->sounds.end()) {
        [pd->sounds[filename] play];
    }
}
