#include "commands.h"
#include "ui_commands.h"
#include "servermanager.h"
#include "argwindowmine.h"

#define EXE_LAMBDA QString reslt; \
                   auto exe([this, &reslt](int32_t ind, bool showR