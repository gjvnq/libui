// 9 april 2015
#include "uipriv_unix.h"

char *uiUnixStrdupText(const char *t)
{
	return g_strdup(t);
}

void uiFreeText(char *t)
{
	g_free(t);
}

void uiFreeTextArray(char **array)
{
	for (int i=0; array[i] != NULL; i++) {
		g_free(array[i]);
	}
}

char *uiJoinStrArray(const char **array, const char *joiner)
{
	// Calculate needed memory
	size_t mem=0;
	for (size_t i=0; array[i] != NULL; i++) {
		mem += strlen(array[i]) + strlen(joiner);
	}
	// Create our answer
	char *msg = malloc((mem+2) * sizeof(char));
	size_t k = 0;
	for (size_t i=0; array[i] != NULL; i++) {
		for (size_t j=0; j < strlen(array[i]); j++) {
			msg[k] = array[i][j];
			k++;
		}
		for (size_t j=0; j < strlen(joiner); j++) {
			msg[k] = joiner[j];
			k++;
		}
	}
	msg[k] = 0;
	return msg;
}

int uiprivStricmp(const char *a, const char *b)
{
	return strcasecmp(a, b);
}
