#include <assert.h>
#include <string.h>

#include "wlpinyin.h"

enum {
	RIME_SHIFT_MASK = 1 << 0,
	RIME_LOCK_MASK = 1 << 1,
	RIME_CONTROL_MASK = 1 << 2,
	RIME_MOD1_MASK = 1 << 3,
	RIME_MOD2_MASK = 1 << 4,
	RIME_MOD4_MASK = 1 << 6,
};

int main(void) {
	assert(im_popup_effective_scale(0) == 1);
	assert(im_popup_effective_scale(-2) == 1);
	assert(im_popup_effective_scale(2) == 2);

	assert(im_popup_scaled_size(0, 2) == 0);
	assert(im_popup_scaled_size(80, 1) == 80);
	assert(im_popup_scaled_size(80, 2) == 160);

	assert(im_text_or_empty(NULL)[0] == '\0');
	assert(im_text_or_empty("abc")[0] == 'a');
	assert(!im_preedit_has_text(NULL));
	assert(!im_preedit_has_text(""));
	assert(im_preedit_has_text("ni"));
	assert(im_key_should_bypass_rime(XKB_KEY_BackSpace, false));
	assert(im_key_should_bypass_rime(XKB_KEY_Delete, false));
	assert(im_key_should_bypass_rime(XKB_KEY_space, false));
	assert(im_key_should_bypass_rime(XKB_KEY_1, false));
	assert(im_key_should_bypass_rime(XKB_KEY_Return, false));
	assert(!im_key_should_bypass_rime(XKB_KEY_BackSpace, true));
	assert(!im_key_should_bypass_rime(XKB_KEY_a, false));
	assert(!im_key_should_bypass_rime(XKB_KEY_A, false));
	assert(strcmp(im_terminal_control_for_key(XKB_KEY_BackSpace), "\x7f") == 0);
	assert(im_terminal_control_for_key(XKB_KEY_Delete) == NULL);
	assert(im_terminal_control_for_key(XKB_KEY_a) == NULL);
	assert(im_should_commit_control_for_key(
			XKB_KEY_BackSpace, ZWP_TEXT_INPUT_V3_CONTENT_PURPOSE_TERMINAL));
	assert(!im_should_commit_control_for_key(
			XKB_KEY_BackSpace, ZWP_TEXT_INPUT_V3_CONTENT_PURPOSE_NORMAL));
	assert(!im_should_commit_control_for_key(
			XKB_KEY_Delete, ZWP_TEXT_INPUT_V3_CONTENT_PURPOSE_TERMINAL));
	assert(im_keycode_is_tracked(0));
	assert(im_keycode_is_tracked(WLPINYIN_KEYCODE_TRACK_MAX - 1));
	assert(!im_keycode_is_tracked(WLPINYIN_KEYCODE_TRACK_MAX));

	im_preedit_t preedit = {
			.text = strdup("ni"),
			.begin = 1,
			.end = 2,
	};
	im_preedit_clear(&preedit);
	assert(preedit.text == NULL);
	assert(preedit.begin == 0);
	assert(preedit.end == 0);

	im_context_t ctx = {
			.page_no = 2,
			.highlighted_index = 1,
			.page_size = 5,
	};
	im_context_clear(&ctx);
	assert(ctx.page_no == 0);
	assert(ctx.highlighted_index == 0);
	assert(ctx.page_size == 0);

	assert(im_rime_filter_mods(RIME_MOD2_MASK) == 0);
	assert(im_rime_filter_mods(RIME_LOCK_MASK | RIME_CONTROL_MASK) ==
				 RIME_CONTROL_MASK);
	assert(
			im_rime_filter_mods(RIME_SHIFT_MASK | RIME_MOD1_MASK | RIME_MOD4_MASK) ==
			(RIME_SHIFT_MASK | RIME_MOD1_MASK | RIME_MOD4_MASK));

	return 0;
}
