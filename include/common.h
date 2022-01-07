#include <stdint.h>

typedef uint32_t Q_STATUS;

#define ARRAY_COUNT(arr, array_size) (array_size / sizeof(*arr))

#define Q_SUCCESS					0
#define Q_ERROR						1
#define Q_SHADER_COMPILE_FAILED		2
#define Q_OUT_OF_MEMORY				3
#define Q_SHADER_LINK_FAILED		4
#define Q_IO_ERROR					5
#define Q_SHADER_UNIFORM_NONEXIST	6

#define TRUE		1
#define FALSE		0