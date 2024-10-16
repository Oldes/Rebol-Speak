
//   ____  __   __        ______        __
//  / __ \/ /__/ /__ ___ /_  __/__ ____/ /
// / /_/ / / _  / -_|_-<_ / / / -_) __/ _ \
// \____/_/\_,_/\__/___(@)_/  \__/\__/_// /
//  ~~~ oldes.huhuman at gmail.com ~~~ /_/
//
// Project: Rebol/Speak extension
// SPDX-License-Identifier: MIT
// =========================================


#include <objc/objc.h>
#include <objc/message.h>
#include <Foundation/Foundation.h>
#include <AppKit/AppKit.h>

#include "speak-rebol-extension.h"


void list_voices(void) {
    @autoreleasepool {
        NSArray *voices = [NSSpeechSynthesizer availableVoices];
        puts("Available Voices:");
        int i = 1;
        for (NSString *voice in voices) {
            // Get the attributes of each voice
            NSDictionary *voiceAttributes = [NSSpeechSynthesizer attributesForVoice:voice];
            NSString *voiceName = [voiceAttributes objectForKey:NSVoiceName];
            NSString *voiceLanguage = [voiceAttributes objectForKey:NSVoiceLocaleIdentifier];
            printf("%i: %s %s\n", i, [voiceName UTF8String], [voiceLanguage UTF8String]);
            i++;
        }
    }
}
int speak(voice_t* voice, int no_wait) {
	const char *text = voice->text;
	NSSpeechSynthesizer *synthesizer = (NSSpeechSynthesizer*)voice->synth;

	@autoreleasepool {
		NSString *nsText = [NSString stringWithUTF8String:text];
		if (!synthesizer) {
			if (voice->number > 0) {
				NSArray *voices = [NSSpeechSynthesizer availableVoices];
				if (voice->number <= [voices count]) {
					NSString *nsVoiceIdentifier = voices[voice->number-1];
					synthesizer = [[NSSpeechSynthesizer alloc] initWithVoice:nsVoiceIdentifier];
				}
			}
			if (!synthesizer) {
				synthesizer = [[NSSpeechSynthesizer alloc] init];
			}
			voice->synth = synthesizer;
		}
		[synthesizer startSpeakingString:nsText];
	}
	return 0;
}

void release_voice(voice_t* voice) {
	//printf("releasing voice..");
	NSSpeechSynthesizer *synthesizer = (NSSpeechSynthesizer*)voice->synth;
	if (synthesizer) {
		[synthesizer stopSpeaking];
		[synthesizer release];
		voice->synth = NULL;
	}
	//puts("done");
}
