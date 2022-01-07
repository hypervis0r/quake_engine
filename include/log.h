#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "common.h"

#define QFatalError( x, ... ) QFatalErrorEx(Q_ERROR, x, __VA_ARGS__)

Q_STATUS QLogInfo(char* msg, ...);
Q_STATUS QLogWarn(char* msg, ...);
Q_STATUS QFatalErrorEx(uint32_t status, char* msg, ...);