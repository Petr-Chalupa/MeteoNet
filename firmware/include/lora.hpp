#pragma once

// full lifecycle for communicating master-slave when connected previously via wifi AP (MAC transfer)

namespace LoRa {
void init(void);

void deinit(void);

void register_RX(void);

void TX(void);
} // namespace LoRa
