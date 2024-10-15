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
#include <stdio.h>
#include <stdlib.h> // malloc
#ifdef TO_WINDOWS
#include <windows.h>
#endif

extern void list_voices(void);
extern void release_voice(void*);
extern void speak(voice_t* voice, int no_wait);

#define COMMAND int

#define FRM_IS_HANDLE(n, t)   (RXA_TYPE(frm,n) == RXT_HANDLE && RXA_HANDLE_TYPE(frm, n) == t && IS_USED_HOB(RXA_HANDLE_CONTEXT(frm, n)))
#define ARG_Is_Integer(n)     (RXA_TYPE(frm,n) == RXT_INTEGER)
#define ARG_Double(n)         RXA_DEC64(frm,n)
#define ARG_Float(n)          (float)RXA_DEC64(frm,n)
#define ARG_Int32(n)          RXA_INT32(frm,n)
#define ARG_Handle_Series(n)  RXA_HANDLE_CONTEXT(frm, n)->series;

#define RETURN_HANDLE(hob)                   \
	RXA_HANDLE(frm, 1)       = hob;          \
	RXA_HANDLE_TYPE(frm, 1)  = hob->sym;     \
	RXA_HANDLE_FLAGS(frm, 1) = hob->flags;   \
	RXA_TYPE(frm, 1) = RXT_HANDLE;           \
	return RXR_VALUE

#define APPEND_STRING(str, ...) \
	len = snprintf(NULL,0,__VA_ARGS__);\
	if (len > (int)(SERIES_REST(str)-SERIES_LEN(str))) {\
		RL_EXPAND_SERIES(str, SERIES_TAIL(str), len);\
		SERIES_TAIL(str) -= len;\
	}\
	len = snprintf( \
		SERIES_TEXT(str)+SERIES_TAIL(str),\
		SERIES_REST(str)-SERIES_TAIL(str),\
		__VA_ARGS__\
	);\
	SERIES_TAIL(str) += len;

#define RETURN_ERROR(err)  do {RXA_SERIES(frm, 1) = err; return RXR_ERROR;} while(0)


int Common_mold(REBHOB *hob, REBSER *str) {
	int len;
	if (!str) return 0;
	SERIES_TAIL(str) = 0;
	APPEND_STRING(str, "0#%lx", (unsigned long)(uintptr_t)hob->data);
	return len;
}


int VoiceHandle_free(void* hndl) {
	if (!hndl) return 0;
	REBHOB *hob = (REBHOB *)hndl;
	voice_t *voice = (voice_t*)hob->data;
	
	//debug_print("Release voice %lx %lx\n", (unsigned long)(uintptr_t)hob->data, (unsigned long)(uintptr_t)voice->text);
	
	release_voice(voice->pVoice);
	if (voice->text) free(voice->text);
	return 0;
}
int VoiceHandle_get_path(REBHOB *hob, REBCNT word, REBCNT *type, RXIARG *arg) {
//	ma_engine* engine = (ma_engine*)hob->data;
	word = RL_FIND_WORD(arg_words, word);
	switch (word) {
//	case W_ARG_RESOURCES:
//		*type = RXT_BLOCK;
//		arg->series = hob->series;
//		arg->index = 0;
//		break;
	default:
		return PE_BAD_SELECT;	
	}
	return PE_USE;
}
int VoiceHandle_set_path(REBHOB *hob, REBCNT word, REBCNT *type, RXIARG *arg) {
//	ma_engine* engine = (ma_engine*)hob->data;
	word = RL_FIND_WORD(arg_words, word);
	switch (word) {
//	case W_ARG_VOLUME:
//		switch (*type) {
//		case RXT_DECIMAL:
//		case RXT_PERCENT:
//			ma_engine_set_volume(engine, arg->dec64);
//			break;
//		case RXT_INTEGER:
//			ma_engine_set_volume(engine, (float)arg->int64);
//			break;
//		default: 
//			return PE_BAD_SET_TYPE;
//		}
//		break;
	default:
		return PE_BAD_SET;	
	}
	return PE_OK;
}


COMMAND cmd_init_words(RXIFRM *frm, void *ctx) {
	arg_words  = RL_MAP_WORDS(RXA_SERIES(frm,1));
	type_words = RL_MAP_WORDS(RXA_SERIES(frm,2));

	// custom initialization may be done here...

	return RXR_TRUE;
}

COMMAND cmd_list_voices(RXIFRM *frm, void *ctx) {
	list_voices();
	return RXR_UNSET;
}

COMMAND cmd_say(RXIFRM *frm, void *ctx) {
	REBSER *ser =  RXA_SERIES(frm,1);
	REBLEN index = RXA_INDEX(frm,1);
	REBLEN tail  = ser->tail;
	if (index > tail) index = tail;
	REBLEN len = tail - index;
	REBHOB* hob;
	REBOOL no_wait = RXA_REF(frm,4);

	if (FRM_IS_HANDLE(3, Handle_VoiceHandle)) {
		hob = RXA_HANDLE_CONTEXT(frm,3);
	} else {
		hob = RL_MAKE_HANDLE_CONTEXT(Handle_VoiceHandle);
	}
	if (hob == NULL) return RXR_FALSE;
	voice_t *voice = (voice_t*)hob->data;

	if (ARG_Is_Integer(3)) {
		voice->number = RXA_INT32(frm,3);		
	}

#ifdef TO_WINDOWS
	// Windows API requires a wide char string!
	size_t bytes = (len + 1) * 2;
	voice->text = realloc(voice->text, bytes);
	if (voice->text == NULL) return RXR_FALSE;

	if (BYTE_SIZE(ser)) {
		MultiByteToWideChar(CP_UTF8, 0, (LPCCH)(ser->data + index), len, voice->text, len);
	} else {
		memcpy(voice->text, ser->data + index * 2, len * 2);
	}
	REBUNI *uni = (REBUNI*) voice->text;
	uni[len] = 0;
#endif

	speak(voice, no_wait);

	RETURN_HANDLE(hob);
}

