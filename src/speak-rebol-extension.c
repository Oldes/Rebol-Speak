//   ____  __   __        ______        __
//  / __ \/ /__/ /__ ___ /_  __/__ ____/ /
// / /_/ / / _  / -_|_-<_ / / / -_) __/ _ \
// \____/_/\_,_/\__/___(@)_/  \__/\__/_// /
//  ~~~ oldes.huhuman at gmail.com ~~~ /_/
//
// Project: Rebol/Speak extension
// SPDX-License-Identifier: MIT
// =========================================

#include "speak-rebol-extension.h"

RL_LIB *RL; // Link back to reb-lib from embedded extensions

//==== Globals ===============================================================//
extern MyCommandPointer Command[];
REBCNT Handle_VoiceHandle;

u32* arg_words;
u32* type_words;
//============================================================================//

static const char* init_block = EXT_INIT_CODE;

int Common_mold(REBHOB *hob, REBSER *ser);

int VoiceHandle_free(void* hndl);
int VoiceHandle_get_path(REBHOB *hob, REBCNT word, REBCNT *type, RXIARG *arg);
int VoiceHandle_set_path(REBHOB *hob, REBCNT word, REBCNT *type, RXIARG *arg);

extern int init_speak(void);

RXIEXT const char *RX_Init(int opts, RL_LIB *lib) {
	RL = lib;
	REBYTE ver[8];
	RL_VERSION(ver);
	debug_print(
		"RXinit speak-extension; Rebol v%i.%i.%i\n",
		ver[1], ver[2], ver[3]);

	if (MIN_REBOL_VERSION > VERSION(ver[1], ver[2], ver[3])) {
		debug_print(
			"Needs at least Rebol v%i.%i.%i!\n",
			 MIN_REBOL_VER, MIN_REBOL_REV, MIN_REBOL_UPD);
		return 0;
	}
	if (!CHECK_STRUCT_ALIGN) {
		trace("CHECK_STRUCT_ALIGN failed!");
		return 0;
	}	

	REBHSP spec;
	spec.mold = Common_mold;

	spec.size      = sizeof(voice_t); // It is MY_, not MA_! 
	spec.flags     = HANDLE_REQUIRES_HOB_ON_FREE;
	spec.free      = VoiceHandle_free;
	spec.get_path  = VoiceHandle_get_path;
	spec.set_path  = VoiceHandle_set_path;
	Handle_VoiceHandle = RL_REGISTER_HANDLE_SPEC((REBYTE*)"voice-handle", &spec);

	return init_block;
}

RXIEXT int RX_Call(int cmd, RXIFRM *frm, void *ctx) {
	return Command[cmd](frm, ctx);
}
