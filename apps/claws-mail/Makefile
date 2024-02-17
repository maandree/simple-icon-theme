.POSIX:

PREFIX         = /usr
ICONPREFIX     = $(PREFIX)/share/icons

THEME_NAME     = simple
THEME_DIR      = $(THEME_NAME)

APP_PREFIX     = /usr
APP_PACKAGE    = claws-mail

APP_THEME_NAME = $(THEME_NAME)
APP_THEME_DIR  = $(THEME_DIR)

ICONS =\
	address_book.svg\
	address_search.svg\
	address.svg\
	anonymous.svg\
	book.svg\
	cancel.svg\
	category.svg\
	checkbox_off.svg\
	checkbox_on.svg\
	check_spelling.svg\
	claws_mail_compose_logo.svg\
	claws_mail_icon_64.svg\
	claws_mail_icon.svg\
	claws_mail_logo_small.svg\
	claws_mail_logo.svg\
	clip_gpg_signed.svg\
	clipkey_gpg_signed.svg\
	clipkey.svg\
	clip.svg\
	close.svg\
	complete.svg\
	continue.svg\
	copied.svg\
	delete_btn.svg\
	deleted.svg\
	dir_close_hrm_mark.svg\
	dir_close_hrm.svg\
	dir_close_mark.svg\
	dir_close.svg\
	dir_noselect_close_mark.svg\
	dir_noselect_close.svg\
	dir_noselect_open.svg\
	dir_noselect.svg\
	dir_open_hrm_mark.svg\
	dir_open_hrm.svg\
	dir_open_mark.svg\
	dir_open.svg\
	dir_subs_close_hrm_mark.svg\
	dir_subs_close_hrm.svg\
	dir_subs_close_mark.svg\
	dir_subs_close.svg\
	dir_subs_open_hrm_mark.svg\
	dir_subs_open_hrm.svg\
	dir_subs_open_mark.svg\
	dir_subs_open.svg\
	doc_info.svg\
	down_arrow.svg\
	drafts_close_hrm_mark.svg\
	drafts_close_hrm.svg\
	drafts_close_mark.svg\
	drafts_close.svg\
	drafts_open_hrm_mark.svg\
	drafts_open_hrm.svg\
	drafts_open_mark.svg\
	drafts_open.svg\
	edit_extern.svg\
	empty.svg\
	error.svg\
	exec.svg\
	first_arrow.svg\
	forwarded.svg\
	gpg_signed.svg\
	group.svg\
	ham_btn.svg\
	ignorethread.svg\
	inbox_close_hrm_mark.svg\
	inbox_close_hrm.svg\
	inbox_close_mark.svg\
	inbox_close.svg\
	inbox_open_hrm_mark.svg\
	inbox_open_hrm.svg\
	inbox_open_mark.svg\
	inbox_open.svg\
	insert_file.svg\
	interface.svg\
	jpilot.svg\
	key_gpg_signed.svg\
	key.svg\
	last_arrow.svg\
	left_arrow.svg\
	locked.svg\
	mail_attach.svg\
	mail_compose.svg\
	mail_draft.svg\
	mail_forward.svg\
	mail_receive_all.svg\
	mail_receive.svg\
	mail_reply.svg\
	mail_reply_to_all.svg\
	mail_reply_to_author.svg\
	mail_reply_to_list.svg\
	mail_send_queue.svg\
	mail_send.svg\
	mark_allread.svg\
	mark_allunread.svg\
	mark_ignorethread.svg\
	mark_locked.svg\
	mark_mark.svg\
	mark_read.svg\
	mark.svg\
	mark_unlocked.svg\
	mark_unmark.svg\
	mark_unread.svg\
	mark_watchthread.svg\
	mime_audio.svg\
	mime_calendar.svg\
	mime_image.svg\
	mime_message.svg\
	mime_pdf.svg\
	mime_pgpsig.svg\
	mime_ps.svg\
	mime_text_enriched.svg\
	mime_text_html.svg\
	mime_text_patch.svg\
	mime_text_plain.svg\
	mime_unknown.svg\
	moved.svg\
	new_mark.svg\
	new.svg\
	notice_error.svg\
	notice_note.svg\
	notice_warn.svg\
	open_mail.svg\
	outbox_close_hrm_mark.svg\
	outbox_close_hrm.svg\
	outbox_close_mark.svg\
	outbox_close.svg\
	outbox_open_hrm_mark.svg\
	outbox_open_hrm.svg\
	outbox_open_mark.svg\
	outbox_open.svg\
	paste.svg\
	preferences.svg\
	printer_btn.svg\
	printer.svg\
	print.svg\
	privacy_emblem_encrypted.svg\
	privacy_emblem_failed.svg\
	privacy_emblem_passed.svg\
	privacy_emblem_signed.svg\
	privacy_emblem_warn.svg\
	privacy_expired.svg\
	privacy_failed.svg\
	privacy_passed.svg\
	privacy_signed.svg\
	privacy_unknown.svg\
	privacy_warn.svg\
	properties.svg\
	queue_close_hrm_mark.svg\
	queue_close_hrm.svg\
	queue_close_mark.svg\
	queue_close.svg\
	queue_open_hrm_mark.svg\
	queue_open_hrm.svg\
	queue_open_mark.svg\
	queue_open.svg\
	quicksearch.svg\
	read.svg\
	replied_and_forwarded.svg\
	replied.svg\
	right_arrow.svg\
	rotate_left.svg\
	rotate_right.svg\
	search.svg\
	spam_btn.svg\
	spam.svg\
	trash_btn.svg\
	trash_close_hrm_mark.svg\
	trash_close_hrm.svg\
	trash_close_mark.svg\
	trash_close.svg\
	trash_hrm_mark.svg\
	trash_hrm.svg\
	trash_mark.svg\
	trash_open_hrm_mark.svg\
	trash_open_hrm.svg\
	trash_open_mark.svg\
	trash_open.svg\
	trash.svg\
	tray_newmail_offline.svg\
	tray_newmail.svg\
	tray_newmarkedmail_offline.svg\
	tray_newmarkedmail.svg\
	tray_nomail_offline.svg\
	tray_nomail.svg\
	tray_unreadmail_offline.svg\
	tray_unreadmail.svg\
	tray_unreadmarkedmail_offline.svg\
	tray_unreadmarkedmail.svg\
	unread.svg\
	up_arrow.svg\
	vcard.svg\
	watchthread.svg\
	zoom_fit.svg\
	zoom_in.svg\
	zoom_out.svg\
	zoom_width.svg

all:
	@:

check:
	set -e; for f in $(ICONS); do test -f "$$f"; done
	set -e; for f in $(ICONS); do test -L "$$f"; done
	test -z "$$(printf '%s\n' $(ICONS) *.svg | sort | uniq -u)"
	test -z "$$(for f in $(ICONS); do readlink -- "$$f"; done | grep '^/')"

install:
	mkdir -p -- "$(DESTDIR)$(APP_PREFIX)/share/$(APP_PACKAGE)/themes"
	test ! -e "$(DESTDIR)$(APP_PREFIX)/share/$(APP_PACKAGE)/themes/$(APP_THEME_DIR)"
	../rellink "$(ICONPREFIX)/$(THEME_DIR)/apps/claws-mail" \
	           "$(APP_PREFIX)/share/$(APP_PACKAGE)/themes/$(APP_THEME_DIR)" \
		   "$(DESTDIR)"
	mkdir -p -- "$(DESTDIR)$(ICONPREFIX)/$(THEME_DIR)/apps/claws-mail"
	cp -P -- $(ICONS) "$(DESTDIR)$(ICONPREFIX)/$(THEME_DIR)/apps/claws-mail/"
	printf '%s\n' "$(APP_THEME_NAME)" "Mattias Andrée" > \
		"$(DESTDIR)$(ICONPREFIX)/$(THEME_DIR)/apps/claws-mail/.claws_themeinfo"

uninstall:
	-cd -- "$(DESTDIR)$(ICONPREFIX)/$(THEME_DIR)/apps/claws-mail" && rm -f -- $(ICONS)
	-unlink -- "$(DESTDIR)$(ICONPREFIX)/$(THEME_DIR)/apps/claws-mail/.claws_themeinfo"
	-rmdir -- "$(DESTDIR)$(ICONPREFIX)/$(THEME_DIR)/apps/claws-mail"
	-rmdir -- "$(DESTDIR)$(ICONPREFIX)/$(THEME_DIR)/apps"
	-rmdir -- "$(DESTDIR)$(ICONPREFIX)/$(THEME_DIR)"
	-unlink -- "$(DESTDIR)$(APP_PREFIX)/share/$(APP_PACKAGE)/themes/$(APP_THEME_DIR)"
	-rmdir -- "$(DESTDIR)$(APP_PREFIX)/share/$(APP_PACKAGE)/themes"
	-rmdir -- "$(DESTDIR)$(APP_PREFIX)/share/$(APP_PACKAGE)"

clean:
	@:

.PHONY: all check install uninstall clean
