#ifndef LOG_SYSTEM
#define LOG_SYSTEM
#include <time.h>
#include <stdio.h>
#include <pthread.h>
#define CONFIG_FILE "log.cfg"
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#define MAX_SIZE_STACK 10000000
#define TIME_BETWEEN_LOGS 100 // time in ms
pthread_t log_pthread;

#define DEBUG_TRACE(msg, ...)                                             \
    do                                                                    \
    {                                                                     \
        log_add(DEBUG, __LINE__, __FILE__, __func__, msg, ##__VA_ARGS__); \
    } while (0)

#define INFO_TRACE(msg, ...)                                             \
    do                                                                   \
    {                                                                    \
        log_add(INFO, __LINE__, __FILE__, __func__, msg, ##__VA_ARGS__); \
    } while (0)

#define WARNING_TRACE(msg, ...)                                             \
    do                                                                      \
    {                                                                       \
        log_add(WARNING, __LINE__, __FILE__, __func__, msg, ##__VA_ARGS__); \
    } while (0)

#define ERROR_TRACE(msg, ...)                                             \
    do                                                                    \
    {                                                                     \
        log_add(ERROR, __LINE__, __FILE__, __func__, msg, ##__VA_ARGS__); \
    } while (0)

enum log_level
{
    ERROR = 0,
    WARNING,
    INFO,
    DEBUG,
    NB_LEVEL
};

/** Adds a log in the system
 */
int log_add(int level, int line, char* file, const char* function, char *format, ...);

typedef struct
{
    int stack_size;
    int write_on_file;
    int write_on_socket;
    const char *output_file;
    int smooth_end;
    int level;
} configuration;

struct log_ctx
{
    struct log *stack_log;
    configuration *config;
    int socket;
    FILE *fp;
    int nb_logs_in_stack;
    clock_t begin;
    volatile int end;
};

struct log
{
    int level;
    double time;
    int line;
    char *file;
    const char *func;
    char *data;
};

/** Initialize the log system
 */
void log_init();

/** Main loop of the log system.
 */
void *log_thread(void);

/** Wait for the stack to finish then end the process and free the structures
 */
void log_end();

/** Set a new log level
 */
void set_log_level(int level);

/** Get current log level
 */
int get_log_level();
#endif
