#pragma once

#include "esphome/components/climate_ir/climate_ir.h"
#include "esphome/components/remote_transmitter/remote_transmitter.h"

namespace esphome {
namespace mitsubishi_heavy {

enum MitsubishiModel {
  MODEL_ZJ,
  MODEL_ZM,
  MODEL_ZMP
};

class MitsubishiHeavyClimate : public climate_ir::ClimateIR {
 public:
  MitsubishiHeavyClimate()
      : climate_ir::ClimateIR(
            16.0f, 30.0f, 1.0f,  // Min, max, step
            true, true,          // supports_dry, supports_fan_only
            {
              climate::CLIMATE_FAN_LOW,
              climate::CLIMATE_FAN_MEDIUM,
              climate::CLIMATE_FAN_HIGH,
              climate::CLIMATE_FAN_AUTO
            },
            {
              climate::CLIMATE_SWING_VERTICAL,
              climate::CLIMATE_SWING_HORIZONTAL,
              climate::CLIMATE_SWING_BOTH
            },
            {
              climate::CLIMATE_PRESET_NONE,
              climate::CLIMATE_PRESET_SLEEP,
              climate::CLIMATE_PRESET_ECO
            }) {}

  void transmit_state() override;
  void set_model(MitsubishiModel model) { this->model_ = model; }

 protected:
  MitsubishiModel model_;
};

}  // namespace mitsubishi_heavy
}  // namespace esphome
