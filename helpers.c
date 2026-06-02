#include <limits.h>
#include <stdlib.h>
#include <string.h>

#include "wlpinyin.h"

enum {
	RIME_LOCK_MASK = 1 << 1,
	RIME_MOD2_MASK = 1 << 4,
};

int im_popup_effective_scale(int scale) {
	return scale > 0 ? scale : 1;
}

int im_popup_scaled_size(int logical_size, int scale) {
	if (logical_size <= 0)
		return 0;

	scale = im_popup_effective_scale(scale);
	if (logical_size > INT_MAX / scale)
		return INT_MAX;

	return logical_size * scale;
}

const char *im_text_or_empty(const char *text) {
	return text ? text : "";
}

void im_preedit_clear(im_preedit_t *preedit) {
	if (!preedit)
		return;

	free(preedit->text);
	preedit->text = NULL;
	preedit->begin = 0;
	preedit->end = 0;
}

void im_context_clear(im_context_t *ctx) {
	if (!ctx)
		return;

	ctx->page_no = 0;
	ctx->highlighted_index = 0;
	ctx->page_size = 0;
}

bool im_preedit_has_text(const char *text) {
	return text && text[0] != '\0';
}

static bool im_keysym_is_ascii_alpha(xkb_keysym_t keysym) {
	return (keysym >= XKB_KEY_a && keysym <= XKB_KEY_z) ||
				 (keysym >= XKB_KEY_A && keysym <= XKB_KEY_Z);
}

bool im_key_should_bypass_rime(xkb_keysym_t keysym, bool composing) {
	if (composing)
		return false;

	return !im_keysym_is_ascii_alpha(keysym);
}

bool im_keycode_is_tracked(uint32_t keycode) {
	return keycode < WLPINYIN_KEYCODE_TRACK_MAX;
}

const char *im_terminal_control_for_key(xkb_keysym_t keysym) {
	if (keysym == XKB_KEY_BackSpace)
		return "\x7f";

	return NULL;
}

bool im_should_commit_control_for_key(xkb_keysym_t keysym,
																			uint32_t content_purpose) {
	return content_purpose == ZWP_TEXT_INPUT_V3_CONTENT_PURPOSE_TERMINAL &&
				 im_terminal_control_for_key(keysym) != NULL;
}

xkb_mod_mask_t im_rime_filter_mods(xkb_mod_mask_t mods) {
	return mods & ~(RIME_LOCK_MASK | RIME_MOD2_MASK);
}
