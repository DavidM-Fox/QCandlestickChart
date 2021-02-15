#ifndef AVAPI_H
#define AVAPI_H
#include "rapidcsv.h"
#include <curl/curl.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

namespace avapi {

typedef std::pair<std::time_t, std::vector<float>> time_pair;
typedef std::vector<time_pair> time_series;

// avapi::Quote helper functions
void downloadCsv(const std::string &t_url, const std::string &file_name);
bool stringReplace(std::string &str, const std::string from,
                   const std::string to);
std::string readFirstLineFromFile(const std::string &file_name);
std::time_t dateTimeToUnixTimestamp(std::string input);
std::time_t dateToUnixTimestamp(std::string input);
void printSeries(const time_series &series);
void printPair(const time_pair &pair);

// Alpha Vantage API url placeholders
extern std::string url_base;
extern std::string url_symbol_api;

// enum for getTimeSeries()
enum function { DAILY, WEEKLY, MONTHLY };

// Main class of avapi
class Quote {
public:
    Quote(std::string symbol, std::string api_key);
    ~Quote();

    time_series getIntradaySeries(const std::string &interval = "15min");
    time_series getTimeSeries(const function &func, const size_t &last_n = 30);
    time_pair getGlobalQuote();

    std::string m_symbol;
    std::string m_api_key;

private:
    time_series parseIntradayCsv(const std::string &file_name);

    time_series parseTimeSeriesCsv(const std::string &file_name,
                                   const size_t &last_n);
};

} // namespace avapi
#endif // AVAPI_H