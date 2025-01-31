/***************************************************************************/ /**
 * @file
 * @brief CPC API implementation.
 *******************************************************************************
 * # License
 * <b>Copyright 2019 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 ******************************************************************************/

#include "sli_cpc.h"
#include "em_core.h"

static sl_slist_node_t *process_queue;

/***************************************************************************//**
 * Push function in dispatch queue along with the data to be passed when
 * dispatched.
 ******************************************************************************/
void sli_cpc_dispatcher_push(sl_cpc_dispatcher_handle_t *handle,
                             sl_cpc_dispatcher_fnct_t fnct,
                             void *data)
{
  handle->fnct = fnct;
  handle->data = data;
  CORE_ATOMIC_SECTION(sl_slist_push_back(&process_queue, &handle->node); );

  sli_cpc_signal_event(SL_CPC_SIGNAL_SYSTEM);
}

/***************************************************************************//**
 * Remove function from dispatch queue along with the data to be passed when
 * dispatched.
 ******************************************************************************/
void sli_cpc_dispatcher_cancel(sl_cpc_dispatcher_handle_t *handle)
{
  CORE_ATOMIC_SECTION(sl_slist_remove(&process_queue, &handle->node); );
}

/***************************************************************************//**
 * Process the dispatch queue.
 ******************************************************************************/
void sli_cpc_dispatcher_process(void)
{
  while (true) {
    sl_slist_node_t *node;
    sl_cpc_dispatcher_handle_t *handle;
    CORE_ATOMIC_SECTION(node = sl_slist_pop(&process_queue); );
    if (node == NULL) {
      break;
    }
    handle = SL_SLIST_ENTRY(node, sl_cpc_dispatcher_handle_t, node);

    handle->fnct(handle->data);
  }
}
