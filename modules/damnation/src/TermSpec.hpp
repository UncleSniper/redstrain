#ifndef REDSTRAIN_MOD_DAMNATION_TERMSPEC_HPP
#define REDSTRAIN_MOD_DAMNATION_TERMSPEC_HPP

#include <cstddef>
#include <stdint.h>

#include "api.hpp"

namespace redengine {
namespace damnation {

	enum REDSTRAIN_DAMNATION_API TSFlag {
		TSF_AUTO_LEFT_MARGIN,
		TSF_AUTO_RIGHT_MARGIN,
		TSF_BACK_COLOR_ERASE,
		TSF_CAN_CHANGE,
		TSF_CEOL_STANDOUT_GLITCH,
		TSF_COL_ADDR_GLITCH,
		TSF_CPI_CHANGES_RES,
		TSF_CR_CANCELS_MICRO_MODE,
		TSF_DEST_TABS_MAGIC_SMSO,
		TSF_EAT_NEWLINE_GLITCH,
		TSF_ERASE_OVERSTRIKE,
		TSF_GENERIC_TYPE,
		TSF_HARD_COPY,
		TSF_HARD_CURSOR,
		TSF_HAS_META_KEY,
		TSF_HAS_PRINT_WHEEL,
		TSF_HAS_STATUS_LINE,
		TSF_HUE_LIGHNESS_SATURATION,
		TSF_INSERT_NULL_GLITCH,
		TSF_LPI_CHANGES_RES,
		TSF_MEMORY_ABOVE,
		TSF_MEMORY_BELOW,
		TSF_MOVE_INSERT_MODE,
		TSF_MOVE_STANDOUT_MODE,
		TSF_NEEDS_XON_XOFF,
		TSF_NO_ESC_CTLC,
		TSF_NO_PAD_CHAR,
		TSF_NON_DEST_SCROLL_REGION,
		TSF_NON_REV_RMCUP,
		TSF_OVER_STRIKE,
		TSF_PRTR_SILENT,
		TSF_ROW_ADDR_GLITCH,
		TSF_SEMI_AUTO_RIGHT_MARGIN,
		TSF_STATUS_LINE_ESC_OK,
		TSF_TILDE_GLITCH,
		TSF_TRANSPARENT_UNDERLINE,
		TSF_XON_XOFF,
		TSF_DPRCT_BACKSPACE_MOVE_LEFT,
		TSF_DPRCT_PERFORMS_HARDWARE_TABS,
		TSF_DPRCT_DO_NOT_USE_ASCII_CR,
		TSF_SCREEN_UNDERSTANDS_ANSI_SET_DEFAULT_COLOR,
		TSF_SCREEN_UNDERSTANDS_XTERM_SEQUENCES,
		TSF_SCREEN_UNDERSTANDS_ISO2022,
		TSF_DOES_NOT_SCROLL_FOR_SEQUENTIAL_OUTPUT,
		TSF__LAST
	};

	const unsigned TS_FLAG_DIMENSION = (static_cast<unsigned>(TSF__LAST) + 31u) / 32u;

	struct REDSTRAIN_DAMNATION_API TermSpec {

		const char* names;
		const char* description;

		uint32_t flags[TS_FLAG_DIMENSION];

		uint32_t columns, init_tabs, label_height, label_width, lines, lines_of_memory;
		uint32_t magic_cookie_glitch, max_attributes, max_colors, max_pairs, maximum_windows;
		uint32_t no_color_video, num_labels, padding_baud_rate, virtual_terminal;
		uint32_t width_status_line;
		uint32_t underline_magic_cookie_width, number_of_function_keys;

		const char* acs_chars;
		const char* back_tab;
		const char* bell;
		const char* carriage_return;
		const char* change_char_pitch;
		const char* change_line_pitch;
		const char* change_res_horz;
		const char* change_res_vert;
		const char* change_scroll_region;
		const char* char_padding;
		const char* clear_all_tabs;
		const char* clear_margins;
		const char* clear_screen;
		const char* clr_bol;
		const char* clr_eol;
		const char* clr_eos;
		const char* column_address;
		const char* command_character;
		const char* create_window;
		const char* cursor_address;
		const char* cursor_down;
		const char* cursor_home;
		const char* cursor_invisible;
		const char* cursor_left;
		const char* cursor_mem_address;
		const char* cursor_normal;
		const char* cursor_right;
		const char* cursor_to_ll;
		const char* cursor_up;
		const char* cursor_visible;
		const char* define_char;
		const char* delete_character;
		const char* delete_line;
		const char* dial_phone;
		const char* dis_status_line;
		const char* display_clock;
		const char* down_half_line;
		const char* ena_acs;
		const char* enter_alt_charset_mode;
		const char* enter_am_mode;
		const char* enter_blink_mode;
		const char* enter_bold_mode;
		const char* enter_ca_mode;
		const char* enter_delete_mode;
		const char* enter_dim_mode;
		const char* enter_doublewide_mode;
		const char* enter_draft_quality;
		const char* enter_insert_mode;
		const char* enter_italics_mode;
		const char* enter_leftward_mode;
		const char* enter_micro_mode;
		const char* enter_near_letter_quality;
		const char* enter_normal_quality;
		const char* enter_protected_mode;
		const char* enter_reverse_mode;
		const char* enter_secure_mode;
		const char* enter_shadow_mode;
		const char* enter_standout_mode;
		const char* enter_subscript_mode;
		const char* enter_superscript_mode;
		const char* enter_underline_mode;
		const char* enter_upward_mode;
		const char* enter_xon_mode;
		const char* erase_chars;
		const char* exit_alt_charset_mode;
		const char* exit_am_mode;
		const char* exit_attribute_mode;
		const char* exit_ca_mode;
		const char* exit_delete_mode;
		const char* exit_doublewide_mode;
		const char* exit_insert_mode;
		const char* exit_italics_mode;
		const char* exit_leftward_mode;
		const char* exit_micro_mode;
		const char* exit_shadow_mode;
		const char* exit_standout_mode;
		const char* exit_subscript_mode;
		const char* exit_superscript_mode;
		const char* exit_underline_mode;
		const char* exit_upward_mode;
		const char* exit_xon_mode;
		const char* fixed_pause;
		const char* flash_hook;
		const char* flash_screen;
		const char* form_feed;
		const char* from_status_line;
		const char* goto_window;
		const char* hangup;
		const char* init_1string;
		const char* init_2string;
		const char* init_3string;
		const char* init_file;
		const char* init_prog;
		const char* initialize_color;
		const char* initialize_pair;
		const char* insert_character;
		const char* insert_line;
		const char* insert_padding;
		const char* key_a1;
		const char* key_a3;
		const char* key_b2;
		const char* key_backspace;
		const char* key_beg;
		const char* key_btab;
		const char* key_c1;
		const char* key_c3;
		const char* key_cancel;
		const char* key_catab;
		const char* key_clear;
		const char* key_close;
		const char* key_command;
		const char* key_copy;
		const char* key_create;
		const char* key_ctab;
		const char* key_dc;
		const char* key_dl;
		const char* key_down;
		const char* key_eic;
		const char* key_end;
		const char* key_enter;
		const char* key_eol;
		const char* key_eos;
		const char* key_exit;
		const char* key_f0;
		const char* key_f1;
		const char* key_f10;
		const char* key_f11;
		const char* key_f12;
		const char* key_f13;
		const char* key_f14;
		const char* key_f15;
		const char* key_f16;
		const char* key_f17;
		const char* key_f18;
		const char* key_f19;
		const char* key_f2;
		const char* key_f20;
		const char* key_f21;
		const char* key_f22;
		const char* key_f23;
		const char* key_f24;
		const char* key_f25;
		const char* key_f26;
		const char* key_f27;
		const char* key_f28;
		const char* key_f29;
		const char* key_f3;
		const char* key_f30;
		const char* key_f31;
		const char* key_f32;
		const char* key_f33;
		const char* key_f34;
		const char* key_f35;
		const char* key_f36;
		const char* key_f37;
		const char* key_f38;
		const char* key_f39;
		const char* key_f4;
		const char* key_f40;
		const char* key_f41;
		const char* key_f42;
		const char* key_f43;
		const char* key_f44;
		const char* key_f45;
		const char* key_f46;
		const char* key_f47;
		const char* key_f48;
		const char* key_f49;
		const char* key_f5;
		const char* key_f50;
		const char* key_f51;
		const char* key_f52;
		const char* key_f53;
		const char* key_f54;
		const char* key_f55;
		const char* key_f56;
		const char* key_f57;
		const char* key_f58;
		const char* key_f59;
		const char* key_f6;
		const char* key_f60;
		const char* key_f61;
		const char* key_f62;
		const char* key_f63;
		const char* key_f7;
		const char* key_f8;
		const char* key_f9;
		const char* key_find;
		const char* key_help;
		const char* key_home;
		const char* key_ic;
		const char* key_il;
		const char* key_left;
		const char* key_ll;
		const char* key_mark;
		const char* key_message;
		const char* key_move;
		const char* key_next;
		const char* key_npage;
		const char* key_open;
		const char* key_options;
		const char* key_ppage;
		const char* key_previous;
		const char* key_print;
		const char* key_redo;
		const char* key_reference;
		const char* key_refresh;
		const char* key_replace;
		const char* key_restart;
		const char* key_resume;
		const char* key_right;
		const char* key_save;
		const char* key_sbeg;
		const char* key_scancel;
		const char* key_scommand;
		const char* key_scopy;
		const char* key_screate;
		const char* key_sdc;
		const char* key_sdl;
		const char* key_select;
		const char* key_send;
		const char* key_seol;
		const char* key_sexit;
		const char* key_sf;
		const char* key_sfind;
		const char* key_shelp;
		const char* key_shome;
		const char* key_sic;
		const char* key_sleft;
		const char* key_smessage;
		const char* key_smove;
		const char* key_snext;
		const char* key_soptions;
		const char* key_sprevious;
		const char* key_sprint;
		const char* key_sr;
		const char* key_sredo;
		const char* key_sreplace;
		const char* key_sright;
		const char* key_srsume;
		const char* key_ssave;
		const char* key_ssuspend;
		const char* key_stab;
		const char* key_sundo;
		const char* key_suspend;
		const char* key_undo;
		const char* key_up;
		const char* keypad_local;
		const char* keypad_xmit;
		const char* lab_f0;
		const char* lab_f1;
		const char* lab_f10;
		const char* lab_f2;
		const char* lab_f3;
		const char* lab_f4;
		const char* lab_f5;
		const char* lab_f6;
		const char* lab_f7;
		const char* lab_f8;
		const char* lab_f9;
		const char* label_format;
		const char* label_off;
		const char* label_on;
		const char* meta_off;
		const char* meta_on;
		const char* micro_column_address;
		const char* micro_down;
		const char* micro_left;
		const char* micro_right;
		const char* micro_row_address;
		const char* micro_up;
		const char* newline;
		const char* order_of_pins;
		const char* orig_colors;
		const char* orig_pair;
		const char* pad_char;
		const char* parm_dch;
		const char* parm_delete_line;
		const char* parm_down_cursor;
		const char* parm_down_micro;
		const char* parm_ich;
		const char* parm_index;
		const char* parm_insert_line;
		const char* parm_left_cursor;
		const char* parm_left_micro;
		const char* parm_right_cursor;
		const char* parm_right_micro;
		const char* parm_rindex;
		const char* parm_up_cursor;
		const char* parm_up_micro;
		const char* pkey_key;
		const char* pkey_local;
		const char* pkey_xmit;
		const char* plab_norm;
		const char* print_screen;
		const char* prtr_non;
		const char* prtr_off;
		const char* prtr_on;
		const char* pulse;
		const char* quick_dial;
		const char* remove_clock;
		const char* repeat_char;
		const char* req_for_input;
		const char* reset_1string;
		const char* reset_2string;
		const char* reset_3string;
		const char* reset_file;
		const char* restore_cursor;
		const char* row_address;
		const char* save_cursor;
		const char* scroll_forward;
		const char* scroll_reverse;
		const char* select_char_set;
		const char* set_attributes;
		const char* set_background;
		const char* set_bottom_margin;
		const char* set_bottom_margin_parm;
		const char* set_clock;
		const char* set_color_pair;
		const char* set_foreground;
		const char* set_left_margin;
		const char* set_left_margin_parm;
		const char* set_right_margin;
		const char* set_right_margin_parm;
		const char* set_tab;
		const char* set_top_margin;
		const char* set_top_margin_parm;
		const char* set_window;
		const char* start_bit_image;
		const char* start_char_set_def;
		const char* stop_bit_image;
		const char* stop_char_set_def;
		const char* subscript_characters;
		const char* superscript_characters;
		const char* tab;
		const char* these_cause_cr;
		const char* to_status_line;
		const char* tone;
		const char* underline_char;
		const char* up_half_line;
		const char* user0;
		const char* user1;
		const char* user2;
		const char* user3;
		const char* user4;
		const char* user5;
		const char* user6;
		const char* user7;
		const char* user8;
		const char* user9;
		const char* wait_tone;
		const char* xoff_character;
		const char* xon_character;
		const char* zero_motion;

		const char* set_a_background;
		const char* set_a_foreground;

		const char* reset_string;
		const char* cursor_down_or_scroll;
		const char* keypad_map;
		const char* screen_switch_charset;
		const char* screen_reset_charset;
		const char* get_mouse;
		const char* req_mouse_pos;

		bool hasFlag(TSFlag) const;

	};

	REDSTRAIN_DAMNATION_API const TermSpec* getTermSpecFor(const char*);
	REDSTRAIN_DAMNATION_API const TermSpec* getTermSpecForThisTerminal();

}}

#endif /* REDSTRAIN_MOD_DAMNATION_TERMSPEC_HPP */
