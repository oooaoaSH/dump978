// -*- c++ -*-

// Copyright (c) 2019, FlightAware LLC.
// All rights reserved.
// Licensed under the 2-clause BSD license; see the LICENSE file

#ifndef STATS_WRITER_H
#define STATS_WRITER_H

#include <chrono>
#include <memory>

#include <boost/asio/io_service.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/asio/strand.hpp>
#include <boost/filesystem.hpp>

#include "track.h"
#include "uat_message.h"

namespace flightaware::stats {
    class StatsWriter : public std::enable_shared_from_this<StatsWriter> {
      public:
        typedef std::shared_ptr<StatsWriter> Pointer;

        static Pointer Create(boost::asio::io_service &service, flightaware::uat::Tracker::Pointer tracker, const boost::filesystem::path &dir, std::chrono::milliseconds interval) { return Pointer(new StatsWriter(service, tracker, dir, interval)); }
        
        void Start();
        void Stop();

    private:
        StatsWriter(boost::asio::io_service &service, flightaware::uat::Tracker::Pointer tracker, const boost::filesystem::path &dir, std::chrono::milliseconds interval) : service_(service), strand_(service), timer_(service), tracker_(tracker), dir_(dir), interval_(interval) {}

        void UpdateStats();

        boost::asio::io_service &service_;
        boost::asio::io_service::strand strand_;
        boost::asio::steady_timer timer_;
        flightaware::uat::Tracker::Pointer tracker_;
        boost::filesystem::path dir_;        
        std::chrono::milliseconds interval_;

    };
} // namespace flightaware::stats

#endif