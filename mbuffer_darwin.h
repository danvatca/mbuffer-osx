#ifndef MBUFFER_DARWIN_H
#define MBUFFER_DARWIN_H

#include <mach/clock.h>
#include <mach/mach.h>
#include <sys/time.h>
#include <semaphore.h>


/*
 * semaphores on OS X only work in named mode, ie, sem_open, but not
 * sem_init. The sem_wait and sem_port take the literal sem value, not
 * a ptr to it.
 */

struct timespec ts;

static inline int mac_sem_init(sem_t *sem, int pshared, int value)
{
    char *fname = strdup("/tmp/.mbuffer.XXXXXX");
    mktemp(fname);
    *sem = sem_open(fname, O_CREAT, 0600, value);
    unlink(fname);
    free(fname);
    return 0;
}

static inline int mac_sem_destroy(sem_t *sem)
{
    sem_close(sem);
    return 0;
}

static inline int mac_sem_wait(sem_t *sem)
{
    return sem_wait(*sem);
}

static inline int mac_sem_post(sem_t *sem)
{
    return sem_post(*sem);
}

static inline int mac_sem_getvalue(sem_t *sem, int *value)
{
    *value = 0;
    return 0;
}


/* Other Linux porting wrappers */

#define CLOCK_REALTIME 0

#define sem_init mac_sem_init
#define sem_destroy mac_sem_destroy
#define sem_wait mac_sem_wait
#define sem_post mac_sem_post
#define sem_getvalue mac_sem_getvalue

#endif
