#pragma once

#include "esphome/components/remote_base/remote_base.h"
#include "esphome/components/remote_transmitter/remote_transmitter.h"
#include <IRSender.h> // arduino-heatpump library


namespace esphome {
namespace mitsubishi_heavy {

class IRSenderESPHome : public IRSender
{
  public:
    IRSenderESPHome(uint8_t pin, remote_transmitter::RemoteTransmitterComponent *transmitter) : IRSender(pin), _transmit(transmitter->transmit()) {};
    void setFrequency(int frequency);
    void space(int spaceLength);
    void mark(int markLength);

  protected:
    remote_transmitter::RemoteTransmitterComponent::TransmitCall _transmit;
};


}  // namespace mitsubishi_heavy
}  // namespace esphome

