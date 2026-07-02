#pragma once

// connecting, reconnecting
// two modes: 1. uplinking data via mqtt and connected network; 2. AP for connecting/disconnecting slave sensor node OR configuring node

namespace WiFi {

void connect(void);

void disconnect(void);

bool is_connected();

void start_AP(void);

void stop_AP(void);

} // namespace WiFi
