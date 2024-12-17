`pthread_testcancel` - 在线程中创建一个取消点（cancellation point），使得线程有机会响应取消请求。

```c
#include <pthread.h>

void pthread_testcancel(void);
```

描述
调用 `pthread_testcancel()` 在调用线程内创建一个 cancellation point （取消点），以便线程响应取消请求。

如果使用 `pthread_setcancelstate()` 禁用可取消性，或者没有待处理的取消请求，则调用 `pthread_testcancel()` 无效。

返回值
函数不返回值。如果调用线程因调用该函数而被取消，则该函数不返回。

```c
#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

#define handle_error_en(en, msg) \
    do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

static void *thread_func(void *ignored_argument)
{
    int s;
    /* 禁用 cancellation 一会儿，不会立刻响应取消请求
    */
    s = pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
    if (s != 0) {
        handle_error_en(s, "pthread_setcancelstate");
    }
    printf("thread_func(): started; cancellation disabled\n");
    sleep(5);
    printf("thread_func(): about to enable cancellation\n");

    s = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    if (s != 0)
        handle_error_en(s, "pthread_setcancelstate");

    /* sleep() is a cancellation point */
    sleep(1000);     /* Should get canceled while we sleep */
    /* Should never get here */
    printf("thread_func(): not canceled!\n");
    return NULL;
}

int main(void)
{
    pthread_t thr;
    void *res;
    int s;

    /* Start a thread and then send it a cancellation request */
    s = pthread_create(&thr, NULL, &thread_func, NULL);
    if (s != 0) {
        handle_error_en(s, "pthread_create");
    }

    sleep(2);   /* Give thread a chance to get started */
    printf("main(): sending cancellation request\n");
    s = pthread_cancel(thr);
    if (s != 0) {
        handle_error_en(s, "pthread_cancel");
    }

    /* Join with thread to see what its exit status was */
    s = pthread_join(thr, &res);
    if (s != 0) {
        handle_error_en(s, "pthread_join");
    }

    if (res != PTHREAD_CANCELED) {
        printf("main(): thread was canceled\n");
    } else {
        printf("main(): thread wasn't canceled (shouldn't happen!)\n");
    }
    exit(EXIT_SUCCESS);
}
```