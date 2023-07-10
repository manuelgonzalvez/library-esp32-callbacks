#ifndef myobserver_h
#define myobserver_h

#include <Arduino.h>
#include "ArduinoJson.h"
#include <unordered_map>
#include <vector>
#include <functional>

class Observer
{
public:
    virtual void notify(const JsonObject& params) = 0;
};

class Subject
{
public:
    void addObserver(Observer* observer)
    {
        observers.push_back(observer);
    }

    void removeObserver(Observer* observer)
    {
        observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
    }

    void notifyObservers(const JsonObject& params)
    {
        for (auto observer : observers)
        {
            observer->notify(params);
        }
    }

protected:
    std::vector<Observer*> observers;

};

class StreamSubject : public Subject
{
public:
    void readStream(const String& input, const JsonObject& params)
    {
        String trimmedInput = input;
        trimmedInput.trim();
        log_v("Input: %s", trimmedInput.c_str());

        for (auto observer : observers)
        {
            if (observerMap.count(observer) > 0 && trimmedInput == observerMap[observer])
            {
                observer->notify(params);
            }
        }
    }

    void addObserver(const String& key, std::function<void(const JsonObject&)> callback)
    {
        auto observer = new CallbackObserver(callback);
        Subject::addObserver(observer);

        observerMap[observer] = key;
    }

    void addObserver(const String& key, std::function<void()> callback)
    {
        auto observer = new CallbackObserver([callback](const JsonObject&) { callback(); });
        Subject::addObserver(observer);

        observerMap[observer] = key;
    }

private:
    class CallbackObserver : public Observer
    {
    public:
        explicit CallbackObserver(std::function<void(const JsonObject&)> callback) : callback(callback) {}

        void notify(const JsonObject& params) override
        {
            callback(params);
        }

    private:
        std::function<void(const JsonObject&)> callback;
    };

    std::unordered_map<Observer*, String> observerMap;
};

class myObserver
{
public:
    static void setupCallbacks();
    static void loopCallbacks(const String& input, const JsonObject& params);
    // static void loopCallbacks(const String& input, const JsonObject& params, std::vector<JsonObject>& returnValues);
};

#endif
