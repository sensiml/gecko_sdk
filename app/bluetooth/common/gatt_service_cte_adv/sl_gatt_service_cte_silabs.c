/***************************************************************************//**
 * @file
 * @brief Constant Tone Extension GATT Service (Silabs proprietary)
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
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

#include <stdbool.h>
#include "sl_bluetooth.h"
#include "app_assert.h"
#include "sli_gatt_service_cte.h"
#include "sli_gatt_service_cte_adv.h"
#include "sl_gatt_service_cte_silabs_config.h"

// The advertising set handle allocated from Bluetooth stack.
static uint8_t advertising_set_handle = 0xff;

// Advertising init status.
static bool advertising_initialized;

/**************************************************************************//**
 * Initialize advertisement package according to CTE specifications.
 *****************************************************************************/
void adv_cte_init(void)
{
  sl_status_t sc;
  uint8_t data[3] = { 0x02, 0x01, 0x06 };

  // Set default values.
  advertising_initialized = false;
  adv_cte_min_len = ADV_CTE_MIN_LEN_DEFAULT;
  adv_cte_min_tx_count = ADV_CTE_MIN_TX_COUNT_DEFAULT;
  adv_cte_phy = ADV_CTE_PHY_DEFAULT;

  // Create an advertising set.
  sc = sl_bt_advertiser_create_set(&advertising_set_handle);
  app_assert_status(sc);

  // Set advertising interval.
  sc = sl_bt_advertiser_set_timing(
    advertising_set_handle,
    SL_GATT_SERVICE_CTE_SILABS_ADV_INTERVAL, // min. adv. interval
    SL_GATT_SERVICE_CTE_SILABS_ADV_INTERVAL, // max. adv. interval
    0,   // adv. duration
    0);  // max. num. adv. events
  app_assert_status(sc);

  // Set custom advertising data.
  sc = sl_bt_advertiser_set_data(advertising_set_handle,
                                 0,
                                 sizeof(data),
                                 data);
  app_assert_status(sc);

  // Turn off legacy PDU flag.
  sc = sl_bt_advertiser_clear_configuration(
    advertising_set_handle,
    ADV_CONFIG_USE_LEGACY_PDUS_BIT_MASK);
  app_assert_status(sc);

  // Start advertising with CTE.
  sc = adv_cte_start();
  app_assert_status(sc);

  advertising_initialized = true;
}

/**************************************************************************//**
 * Start/restart advertising with the preset parameters.
 *****************************************************************************/
sl_status_t adv_cte_start(void)
{
  sl_status_t sc = SL_STATUS_OK;

  // Stop advertising.
  if (advertising_initialized) {
    sc = sl_bt_advertiser_stop(advertising_set_handle);
  }

  // Set PHY.
  if (sc == SL_STATUS_OK) {
    sc = sl_bt_advertiser_set_phy(
      advertising_set_handle,
      sl_bt_gap_1m_phy,
      ADV_CTE_PHY_CONVERT(adv_cte_phy));
  }

  // Start general advertising and disable connections.
  if (sc == SL_STATUS_OK) {
    sc = sl_bt_advertiser_start(
      advertising_set_handle,
      sl_bt_advertiser_user_data,
      sl_bt_advertiser_non_connectable);
  }

  // Add CTE to extended advertisements.
  if (sc == SL_STATUS_OK) {
    sc = sl_bt_cte_transmitter_enable_silabs_cte(
      advertising_set_handle,
      adv_cte_min_len,
      SLI_CTE_TYPE_AOA,
      adv_cte_min_tx_count,
      SLI_CTE_SWITCHING_PATTERN_LENGTH,
      SLI_CTE_SWITCHING_PATTERN);
  }

  return sc;
}
