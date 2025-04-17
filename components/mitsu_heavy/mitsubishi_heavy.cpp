#include "mitsubishi_heavy.h"
#include "esphome/core/log.h"
#include "ir_sender_esphome.h"
#include <MitsubishiHeavyHeatpumpIR.h>
#include <algorithm>  // For std::clamp

namespace esphome {
namespace mitsubishi_heavy {

static const char *TAG = "mitsubishi_heavy.climate";

void MitsubishiHeavyClimate::transmit_state() {
  uint8_t powerModeCmd;
  uint8_t operatingModeCmd;
  uint8_t temperatureCmd;
  uint8_t fanSpeedCmd;

  uint8_t swingVCmd = VDIR_SWING;
  uint8_t swingHCmd = HDIR_SWING;

  if (this->fan_mode.has_value()) {
    switch (this->fan_mode.value()) {
      case climate::CLIMATE_FAN_LOW:
        fanSpeedCmd = FAN_2;
        break;
      case climate::CLIMATE_FAN_MEDIUM:
        fanSpeedCmd = FAN_3;
        break;
      case climate::CLIMATE_FAN_HIGH:
        fanSpeedCmd = FAN_4;
        break;
      case climate::CLIMATE_FAN_AUTO:
      default:
        fanSpeedCmd = FAN_AUTO;
        break;
    }
  } else {
    fanSpeedCmd = FAN_AUTO;
  }

  switch (this->mode) {
    case climate::CLIMATE_MODE_COOL:
      powerModeCmd = POWER_ON;
      operatingModeCmd = MODE_COOL;
      break;
    case climate::CLIMATE_MODE_HEAT:
      powerModeCmd = POWER_ON;
      operatingModeCmd = MODE_HEAT;
      break;
    case climate::CLIMATE_MODE_AUTO:
      powerModeCmd = POWER_ON;
      operatingModeCmd = MODE_AUTO;
      break;
    case climate::CLIMATE_MODE_FAN_ONLY:
      powerModeCmd = POWER_ON;
      operatingModeCmd = MODE_FAN;
      break;
    case climate::CLIMATE_MODE_DRY:
      powerModeCmd = POWER_ON;
      operatingModeCmd = MODE_DRY;
      break;
    case climate::CLIMATE_MODE_OFF:
    default:
      powerModeCmd = POWER_OFF;
      operatingModeCmd = MODE_AUTO;
      break;
  }

  float clamped_temp = std::clamp(this->target_temperature,
                                  static_cast<float>(MITSUBISHI_TEMP_MIN),
                                  static_cast<float>(MITSUBISHI_TEMP_MAX));
  temperatureCmd = static_cast<uint8_t>(clamped_temp);

  auto *tx = dynamic_cast<remote_transmitter::RemoteTransmitterComponent *>(this->transmitter_);
  if (tx == nullptr) {
    ESP_LOGE(TAG, "Invalid transmitter type");
    return;
  }
  IRSenderESPHome espSender(0, tx);

  MitsubishiHeavyHeatpumpIR *heatpumpIR;
  switch (model_) {
    case MODEL_ZJ:
      heatpumpIR = new MitsubishiHeavyZJHeatpumpIR();
      break;
    case MODEL_ZM:
      heatpumpIR = new MitsubishiHeavyZMHeatpumpIR();
      break;
    case MODEL_ZMP:
      heatpumpIR = new MitsubishiHeavyZMPHeatpumpIR();
      break;
    default:
      ESP_LOGE(TAG, "Invalid model");
      return;
  }

  heatpumpIR->send(espSender, powerModeCmd, operatingModeCmd, fanSpeedCmd, temperatureCmd,
                   swingVCmd, swingHCmd, false, false, false);

  delete heatpumpIR;  // Clean up heap memory to avoid leaks
}

}  // namespace mitsubishi_heavy
}  // namespace esphome
