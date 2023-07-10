
#include <observerCallbacks.h>

void callback1() { log_w("Callback 1 called!"); }

void callback2(const JsonObject& params)
{
    log_w("Callback 2 called!");
    // Access the parameters from the JsonObject
    if (params.containsKey("param1"))
    {
        String param = params["param1"];

        // int value = params["param1"].as<int>();
        log_w("Parameter: %s", param.c_str());
    }
}

StreamSubject streamSubject;

void myObserver::setupCallbacks()
{

    streamSubject.addObserver("key1", callback1);

    streamSubject.addObserver("key2", callback2);
}

void myObserver::loopCallbacks(const String &input, const JsonObject &params)
{
    streamSubject.readStream(input, params);
}