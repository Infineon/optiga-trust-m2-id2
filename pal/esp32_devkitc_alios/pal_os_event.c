/**
* \copyright
* MIT License
*
* Copyright (c) 2020 Infineon Technologies AG
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE
*
* \endcopyright
*
* \author Infineon Technologies AG
*
* \file pal_os_event.c
*
* \brief   This file implements the platform abstraction layer APIs for os event/scheduler.
*
* \ingroup  grPAL
*
* @{
*/

#include "optiga/pal/pal_os_event.h"
#include "espos_timer.h"
#include "espos_mutex.h"
#include "espos_queue.h"
#include "espos_task.h"
#include "aos/init.h"
#include "aos/kernel.h"

typedef struct callbacks {
	/// Callback function when timer elapses
	volatile register_callback clb;
	/// Pointer to store upper layer callback context (For example: Ifx i2c context)
	void * clb_ctx;
}pal_os_event_clbs_t;

static const char *TIMER_NAME = "scheduler_timer";

static espos_timer_t scheduler_timer;
static uint8_t create_state = 1;
static espos_mutex_t global_mutex;
static espos_queue_t global_queue;
/// @cond hidden

static pal_os_event_t pal_os_event_0 = {0};

static void queue_recv_task(void *p)
{
    register_callback callback;
    pal_os_event_clbs_t clbs;
    while(1)
    {
        if ( 0 == espos_queue_recv(global_queue, &clbs, ESPOS_NO_DELAY) )
        {
            callback = clbs.clb;
            clbs.clb = NULL;
            callback((void * )clbs.clb_ctx);
        }
    }   
}
static void timer_handler(espos_timer_t timer, void *arg)
{
    pal_os_event_trigger_registered_callback();
    espos_mutex_unlock(global_mutex);
}

void pal_os_event_start(pal_os_event_t * p_pal_os_event, register_callback callback, void * callback_args)
{
    if (FALSE == p_pal_os_event->is_event_triggered)
    {
        pal_os_event_register_callback_oneshot(p_pal_os_event,callback,callback_args,1000);
        p_pal_os_event->is_event_triggered = TRUE;
    }
}

void pal_os_event_stop(pal_os_event_t * p_pal_os_event)
{
    //lint --e{714} suppress "The API pal_os_event_stop is not exposed in header file but used as extern in 
    //optiga_cmd.c"
    p_pal_os_event->is_event_triggered = FALSE;
    espos_timer_stop(scheduler_timer);
}

pal_os_event_t * pal_os_event_create(register_callback callback, void * callback_args)
{
    if (( NULL != callback )&&( NULL != callback_args ))
    {
        pal_os_event_start(&pal_os_event_0,callback,callback_args);
    }
    return (&pal_os_event_0);
}

void pal_os_event_trigger_registered_callback(void)
{
    register_callback callback;
    pal_os_event_clbs_t clbs;
    uint8_t exit = 1;

    // !!!OPTIGA_LIB_PORTING_REQUIRED
    // The following steps related to TIMER must be taken care while porting to different platform
    //lint --e{534} suppress "Error handling is not required so return value is not checked"
    while (exit)
    {
        if (0 == espos_mutex_lock(global_mutex, ESPOS_MAX_DELAY))
        {
            espos_timer_stop(scheduler_timer);
            clbs.clb_ctx = pal_os_event_0.callback_ctx;
            clbs.clb = pal_os_event_0.callback_registered;

            espos_queue_send(global_queue, &clbs, ESPOS_MAX_DELAY);             
            exit = 0;
        }
    }
}
/// @endcond

void pal_os_event_register_callback_oneshot(pal_os_event_t * p_pal_os_event,
                                             register_callback callback,
                                             void * callback_args,
                                             uint32_t time_us)
{
    p_pal_os_event->callback_registered = callback;
    p_pal_os_event->callback_ctx = callback_args;
    uint32_t time_ms;
    espos_task_t new_task_handle;


    if(create_state)
    {
        // !!!OPTIGA_LIB_PORTING_REQUIRED
        // The following steps related to TIMER must be taken care while porting to different platform
        espos_mutex_create(&global_mutex, ESPOS_MUTEX_NORMAL);
        espos_queue_create(&global_queue, sizeof(pal_os_event_clbs_t), 10);
        aos_task_new("queue_recv_task", queue_recv_task, 0, 8192);
        espos_timer_create(&scheduler_timer, 
                            TIMER_NAME, 
                            timer_handler, 
                            NULL,
                            2,
                            ESPOS_TIMER_NO_AUTO_RUN);
        create_state = 0;
        
    }
    else
    {
        espos_timer_set_period(scheduler_timer, 2);
    }
    
    espos_timer_start(scheduler_timer);
}

//lint --e{818,715} suppress "As there is no implementation, pal_os_event is not used"
void pal_os_event_destroy(pal_os_event_t * pal_os_event)
{
    espos_timer_del(scheduler_timer);
}

/**
* @}
*/
