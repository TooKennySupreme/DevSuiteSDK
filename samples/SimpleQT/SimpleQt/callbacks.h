#include "apbase.h"

//   This handles the PROMPT= ini command, a multiple-choice question
int APBASE_DECL MultipleChoice_Callback(void *pContext,
    const char *szMessage,
    const char *szChoices);

//   This handles error messages
int APBASE_DECL ErrorMessage_Callback(void *pContext,
    const char *szMessage,
    unsigned int mbType);
