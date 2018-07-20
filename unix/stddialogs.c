// 26 june 2015
#include "uipriv_unix.h"

// LONGTERM figure out why, and describe, that this is the desired behavior
// LONGTERM also point out that font and color buttons also work like this

#define windowWindow(w) (GTK_WINDOW(uiControlHandle(uiControl(w))))

static char **filedialog_adv(GtkWindow *parent, GtkFileChooserAction mode, int multiple, const char *human_filter_msg, const char *patterns[], const gchar *confirm)
{
	GtkWidget *fcd;
	GtkFileChooser *fc;
	GtkFileFilter *filter;
	gint response;
	guint i, len;
	GSList *list;
	char **filenames;

	filter = gtk_file_filter_new();
	gtk_file_filter_set_name(filter, human_filter_msg);
	if (patterns != NULL) {
		for (int i = 0; patterns[i] != NULL; i++) {
			gtk_file_filter_add_pattern(filter, patterns[i]);
		}
	}

	fcd = gtk_file_chooser_dialog_new(NULL, parent, mode,
		"_Cancel", GTK_RESPONSE_CANCEL,
		confirm, GTK_RESPONSE_ACCEPT,
		NULL);
	fc = GTK_FILE_CHOOSER(fcd);
	gtk_file_chooser_set_local_only(fc, FALSE);
	gtk_file_chooser_set_select_multiple(fc, multiple);
	gtk_file_chooser_set_show_hidden(fc, TRUE);
	gtk_file_chooser_set_do_overwrite_confirmation(fc, TRUE);
	gtk_file_chooser_set_create_folders(fc, TRUE);
	if (patterns != NULL) {
		gtk_file_chooser_set_filter(fc, filter);
	}
	response = gtk_dialog_run(GTK_DIALOG(fcd));
	if (response != GTK_RESPONSE_ACCEPT) {
		gtk_widget_destroy(fcd);
		return NULL;
	}
	// filenames = uiUnixStrdupText(gtk_file_chooser_get_filenames(fc));
	list = gtk_file_chooser_get_filenames(fc);
	len = g_slist_length(list);
	filenames = malloc((len+1) * sizeof(const char *));
	if (filenames != NULL) {
		for (i=0; i < len; i++) {
			filenames[i] = uiUnixStrdupText(g_slist_nth_data(list, i));
		}
	}
	filenames[i] = 0;
	g_slist_free(list);
	gtk_widget_destroy(fcd);
	return filenames;
}

static char *filedialog(GtkWindow *parent, GtkFileChooserAction mode, const gchar *confirm)
{
	return  filedialog_adv(parent, mode, FALSE, NULL, NULL, confirm)[0];
}

char *uiOpenFile(uiWindow *parent)
{
	return filedialog(windowWindow(parent), GTK_FILE_CHOOSER_ACTION_OPEN, "_Open");
}

char **uiOpenFileAdv(uiWindow *parent, int multiple, const char *human_filter_msg, const char *patterns[])
{
	return filedialog_adv(windowWindow(parent), GTK_FILE_CHOOSER_ACTION_OPEN, multiple, human_filter_msg, patterns, "_Open");
}

char *uiSaveFile(uiWindow *parent)
{
	return filedialog(windowWindow(parent), GTK_FILE_CHOOSER_ACTION_SAVE, "_Save");
}

static void msgbox(GtkWindow *parent, const char *title, const char *description, GtkMessageType type, GtkButtonsType buttons)
{
	GtkWidget *md;

	md = gtk_message_dialog_new(parent, GTK_DIALOG_MODAL,
		type, buttons,
		"%s", title);
	gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(md), "%s", description);
	gtk_dialog_run(GTK_DIALOG(md));
	gtk_widget_destroy(md);
}

void uiMsgBox(uiWindow *parent, const char *title, const char *description)
{
	msgbox(windowWindow(parent), title, description, GTK_MESSAGE_OTHER, GTK_BUTTONS_OK);
}

void uiMsgBoxError(uiWindow *parent, const char *title, const char *description)
{
	msgbox(windowWindow(parent), title, description, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK);
}
