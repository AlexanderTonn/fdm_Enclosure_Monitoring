#include "sensor_dht.hpp"


auto init() -> void
{
    // Initialize DHT sensor
    dht.begin();
}
