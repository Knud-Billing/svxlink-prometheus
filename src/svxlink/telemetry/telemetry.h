#ifndef TELEMETRY_H
#define TELEMETRY_H

#include <prometheus/exposer.h>
#include <prometheus/registry.h>
#include <prometheus/counter.h>
#include <prometheus/summary.h>
#include <chrono>

using namespace prometheus;
using namespace std::chrono;


class Telemetry {

    protected:

    Telemetry();
    
    ~Telemetry();

    Exposer exposer{"127.0.0.1:9091"};
    std::shared_ptr<Registry> registry;

    milliseconds tx_ms;

    Family<Counter> *tx_counter;
    Counter *tx_on_counter;
    Counter *tx_off_counter;
    
    Family<Summary> *tx_length;
    Summary *tx_length_total;


    milliseconds echolink_ms;

    Family<Counter> *echolink_counter;
    Counter *echolink_connect_counter;
    Counter *echolink_disconnect_counter;

    Family<Summary> *echolink_length;
    Summary *echolink_length_total;


    milliseconds parrot_ms;

    Family<Counter> *parrot_counter;
    Counter *parrot_activate_counter;
    Counter *parrot_deactivate_counter;

    Family<Summary> *parrot_length;
    Summary *parrot_length_total;

    
    milliseconds squelch_ms;

    Family<Counter> *squelch_counter;
    Counter *squelch_open_counter;
    Counter *squelch_close_counter;

    Family<Summary> *squelch_length;
    Summary *squelch_length_total;

    
    milliseconds qso_ms;

    Family<Counter> *qso_counter;
    Counter *qso_start_counter;
    Counter *qso_stop_counter;

    Family<Summary> *qso_length;
    Summary *qso_length_total;


    public:

    // Not cloneable
    Telemetry(Telemetry &other) = delete;
    // Not assignable
    void operator=(const Telemetry &) = delete;

    static Telemetry& getInstance();

    void tx_on();
    void tx_off();

    void parrot_activate();
    void parrot_deactivate();

    void echolink_connect();
    void echolink_disconnect();

    void squelch_open();
    void squelch_close();    
    
    void qso_start();
    void qso_stop();
};

#endif
