// Copyright (c) 2019, FlightAware LLC.
// All rights reserved.
// Licensed under the 2-clause BSD license; see the LICENSE file

#include "stats_writer.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <ctime>

#include "common.h"
#include "json.hpp"

using namespace flightaware::uat;
using namespace flightaware::stats;

void StatsWriter::Start() {
    std::cerr << "StatsWriter starting up" << std::endl;

    UpdateStats();
}

void StatsWriter::Stop() {
    std::cerr << "StatsWriter stopping" << std::endl;
}

void StatsWriter::UpdateStats() {
    std::cerr << "StatsWriter -- UpdateStats()" << std::endl;

    using json = nlohmann::json;
    json stats_json;

    auto now = now_millis();

    stats_json["Name"] = "stats.json";
    stats_json["updated"] = now / 1000.0;
    stats_json["messages"] = tracker_->TotalMessages();
    stats_json["unique_aircraft"] = tracker_->UniqueAircraft();
    stats_json["peak_signal"] = tracker_->PeakSignal();
    stats_json["single_message_aircraft"] = tracker_->SingleMessageAircraft();

    std::ofstream stats_file((dir_ / "stats.json").native());
    stats_file << stats_json << std::endl;

    auto self(shared_from_this());
    timer_.expires_from_now(interval_);
    timer_.async_wait(strand_.wrap([this, self](const boost::system::error_code &ec) {
        if (!ec) {
            UpdateStats();
        }
    }));
}
