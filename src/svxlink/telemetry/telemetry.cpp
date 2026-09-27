#include "telemetry.h"

#include <prometheus/exposer.h>
#include <prometheus/registry.h>
#include <prometheus/counter.h>
#include <prometheus/summary.h>

#include <chrono>
#include <iostream>


using namespace std::chrono;
using namespace prometheus;


Telemetry& Telemetry::getInstance()
{
    static Telemetry telemetry;
    return telemetry;
}


Telemetry::Telemetry() 
{
    // exposer = Exposer{"127.0.0.1:9091"};

    registry = std::make_shared<Registry>();

    tx_counter = &BuildCounter()
                        .Name("tx_total")
                        .Help("Number of transmissions")
                        .Register(*registry);
    tx_on_counter = &tx_counter->Add({{"transition", "on"}});
    tx_off_counter = &tx_counter->Add({{"transition", "off"}});

    tx_length = &BuildSummary()
                   .Name("tx_length_total")
                   .Help("Transmitter active")
                   .Register(*registry);
    tx_length_total = &tx_length->Add({{"tx", "length"}}, Summary::Quantiles());



    echolink_counter = &BuildCounter()
                        .Name("echolink_total")
                        .Help("Number of Echolink connections")
                        .Register(*registry);
    echolink_connect_counter = &echolink_counter->Add({{"transition", "connect"}});
    echolink_disconnect_counter = &echolink_counter->Add({{"transition", "disconnect"}});

    echolink_length = &BuildSummary()
                   .Name("echolink_length_total")
                   .Help("Echolink active")
                   .Register(*registry);
    echolink_length_total = &echolink_length->Add({{"echolink", "length"}}, Summary::Quantiles());



    // parrot_counter = &BuildCounter()
    //                     .Name("parrot_total")
    //                     .Help("Number of parrot activations")
    //                     .Register(*registry);
    // parrot_activate_counter = &parrot_counter->Add({{"transition", "activate"}});
    // parrot_deactivate_counter = &parrot_counter->Add({{"transition", "deactivate"}});

    // parrot_length = &BuildSummary()
    //                .Name("parrot_length_total")
    //                .Help("Parrot active")
    //                .Register(*registry);
    // parrot_length_total = &parrot_length->Add({{"parrot", "length"}}, Summary::Quantiles());


    qso_counter = &BuildCounter()
                        .Name("qso_total")
                        .Help("Number of QSOs")
                        .Register(*registry);
    qso_start_counter = &qso_counter->Add({{"qso", "start"}});
    qso_stop_counter = &qso_counter->Add({{"qso", "stop"}});

    qso_length = &BuildSummary()
                   .Name("qso_length_total")
                   .Help("QSO length")
                   .Register(*registry);
    qso_length_total = &qso_length->Add({{"qso", "length"}}, Summary::Quantiles());


    squelch_counter = &BuildCounter()
                        .Name("squelch_total")
                        .Help("Number of squelch events")
                        .Register(*registry);
    squelch_open_counter = &squelch_counter->Add({{"squelch", "open"}});
    squelch_close_counter = &squelch_counter->Add({{"squelch", "close"}});

    squelch_length = &BuildSummary()
                   .Name("squelch_length_total")
                   .Help("Squelch active")
                   .Register(*registry);
    squelch_length_total = &squelch_length->Add({{"squelch", "length"}}, Summary::Quantiles());


    exposer.RegisterCollectable(registry);

}

Telemetry::~Telemetry() {

}

void Telemetry::tx_on() {
    tx_ms = duration_cast< milliseconds >(system_clock::now().time_since_epoch());
    tx_on_counter->Increment();
}

void Telemetry::tx_off() {
    milliseconds now = duration_cast< milliseconds >(system_clock::now().time_since_epoch());
    tx_off_counter->Increment();
    double time = (now.count() - tx_ms.count())/1000.0;
    tx_length_total->Observe(time);
    std::cout << "Tx was on for " << time << " seconds" << std::endl;
}


void Telemetry::echolink_connect() {
    echolink_ms = duration_cast< milliseconds >(system_clock::now().time_since_epoch());
    echolink_connect_counter->Increment();
}

void Telemetry::echolink_disconnect() {
    milliseconds now = duration_cast< milliseconds >(system_clock::now().time_since_epoch());
    echolink_disconnect_counter->Increment();
    echolink_length_total->Observe((now.count() - echolink_ms.count())/1000.0);
}


void Telemetry::parrot_activate() {
    parrot_ms = duration_cast< milliseconds >(system_clock::now().time_since_epoch());
    parrot_activate_counter->Increment();
}

void Telemetry::parrot_deactivate() {
    milliseconds now = duration_cast< milliseconds >(system_clock::now().time_since_epoch());
    parrot_deactivate_counter->Increment();
    parrot_length_total->Observe((now.count() - parrot_ms.count())/1000.0);
}

void Telemetry::squelch_open() {
    squelch_ms = duration_cast< milliseconds >(system_clock::now().time_since_epoch());
    squelch_open_counter->Increment();
}

void Telemetry::squelch_close() {
    milliseconds now = duration_cast< milliseconds >(system_clock::now().time_since_epoch());
    squelch_close_counter->Increment();
    double time = (now.count() - squelch_ms.count())/1000.0;
    squelch_length_total->Observe(time);
    std::cout << "Squelch was open for " << time << " seconds" << std::endl;
}


void Telemetry::qso_start() {
    qso_ms = duration_cast< milliseconds >(system_clock::now().time_since_epoch());
    qso_start_counter->Increment();
}

void Telemetry::qso_stop() {
    milliseconds now = duration_cast< milliseconds >(system_clock::now().time_since_epoch());
    qso_stop_counter->Increment();
    qso_length_total->Observe((now.count() - qso_ms.count())/1000.0);
}