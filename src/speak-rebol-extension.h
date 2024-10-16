//   ____  __   __        ______        __
//  / __ \/ /__/ /__ ___ /_  __/__ ____/ /
// / /_/ / / _  / -_|_-<_ / / / -_) __/ _ \
// \____/_/\_,_/\__/___(@)_/  \__/\__/_// /
//  ~~~ oldes.huhuman at gmail.com ~~~ /_/
//
// Project: Rebol/Speak extension
// SPDX-License-Identifier: MIT
// =========================================
// NOTE: auto-generated file, do not modify!
//

#include "rebol-extension.h"

#define SERIES_TEXT(s)   ((char*)SERIES_DATA(s))

#define MIN_REBOL_VER 3
#define MIN_REBOL_REV 14
#define MIN_REBOL_UPD 1
#define VERSION(a, b, c) (a << 16) + (b << 8) + c
#define MIN_REBOL_VERSION VERSION(MIN_REBOL_VER, MIN_REBOL_REV, MIN_REBOL_UPD)


extern REBCNT Handle_VoiceHandle;

typedef struct voice_t {
	void *synth;
	void *text;
	int  number;
} voice_t;

extern u32* arg_words;
extern u32* type_words;

enum ext_commands {
	CMD_INIT_WORDS,
	CMD_LIST_VOICES,
	CMD_SAY,
};


int cmd_init_words(RXIFRM *frm, void *ctx);
int cmd_list_voices(RXIFRM *frm, void *ctx);
int cmd_say(RXIFRM *frm, void *ctx);

enum ma_arg_words {W_ARG_0
};
enum ma_type_words {W_TYPE_0
};

typedef int (*MyCommandPointer)(RXIFRM *frm, void *ctx);

#define EXT_INIT_CODE \
	"REBOL [Title: {Rebol Speak Extension} Type: module Version: 0.0.1 needs: 3.14.1 exports: [say]]\n"\
	"init-words: command [args [block!] type [block!]]\n"\
	"list-voices: command [\"Print a list of available system voices\"]\n"\
	"say: command [\"Converts text to speech and plays it\" text [string!] \"The text to be spoken\" /as voice [integer! handle!] {Specify the voice or handle to use for speech synthesis} /no-wait \"Do not block execution while the speech is playing\"]\n"\
	"init-words [][]\n"\
	"protect/hide 'init-words\n"\
	"\n"

#ifdef  USE_TRACES
#include <stdio.h>
#define debug_print(fmt, ...) do { printf(fmt, __VA_ARGS__); } while (0)
#define trace(str) puts(str)
#else
#define debug_print(fmt, ...)
#define trace(str) 
#endif

