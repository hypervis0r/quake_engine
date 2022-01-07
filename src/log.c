#include "log.h"

Q_STATUS QLogInfo(char* msg, ...)
{
	va_list va;
	va_start(va, msg);

	printf("[+] INFO: ");

	vprintf(msg, va);

	return Q_SUCCESS;
}

Q_STATUS QLogWarn(char* msg, ...)
{
	va_list va;
	va_start(va, msg);

	printf("[/] WARN: ");

	vprintf(msg, va);

	return Q_SUCCESS;
}

Q_STATUS QFatalErrorEx(uint32_t status, char* msg, ...)
{
	va_list va;
	va_start(va, msg);

	printf("[-] FATAL ERROR: ");

	vfprintf(stderr, msg, va);

	exit(status);

	return Q_ERROR;
}