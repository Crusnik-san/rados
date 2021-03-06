/*
 * This file is a part of RadOs project
 * Copyright (c) 2013, Radoslaw Biernacki <radoslaw.biernacki@gmail.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1) Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2) Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3) No personal names or organizations' names associated with the 'RadOs'
 *    project may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE RADOS PROJECT AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * /file Test OS port (step 1)
 * /ingroup tests
 *
 * This is first basic test to check the port.
 * Test if task_proc is called and if it can block on semaphore, test if idle
 * procedure will be called (because of task block)
 * Test in following services are implemented correctly:
 * - task (stack and context) initialization is performed correctly
 * - arch_context_switch implemented correctly (at least called 2 times
 *   init_idle->task1->idle)
 * /{
 */

#include "os.h"
#include "os_test.h"

static os_task_t task1;
static OS_TASKSTACK task1_stack[OS_STACK_MINSIZE];
static os_sem_t sem1;
static volatile int task1_started = 0;

void test_idle(void)
{
   test_result(task1_started ? 0 : -1);
}

int task1_proc(void *OS_UNUSED(param))
{
   int ret;

   task1_started = 1;

   ret = os_sem_down(&sem1, OS_TIMEOUT_INFINITE);
   test_debug("fail: od_sem_down ret code %d", ret);
   test_result(-1);

   return 0;
}

void test_init(void)
{
   os_sem_create(&sem1, 0);
   os_task_create(&task1, 1, task1_stack, sizeof(task1_stack), task1_proc, NULL);
}

int main(void)
{
   os_init();
   test_setupmain(OS_PROGMEM_STR("Test1"));
   test_init();
   os_start(test_idle);

   return 0;
}

/** /} */

