#include "../inc/avapi.h"

namespace avapi {

extern std::string url_base = "https://www.alphavantage.co/query?function=";
extern std::string url_symbol_api =
    "&symbol={symbol}&apikey={api}&datatype=csv";

// Quote constructor
Quote::Quote(std::string symbol, std::string api_key)
    : m_symbol(symbol), m_api_key(api_key)
{
    stringReplace(url_symbol_api, "{symbol}", symbol);
    stringReplace(url_symbol_api, "{api}", api_key);
}

// Quote deconstructor
Quote::~Quote() {}

// Get an intraday time series which each time_pair being:
//
// time_t unix_timestamp, vector([open, high, low, close, volume])
time_series Quote::getIntradaySeries(const std::string &interval)
{
    std::string url =
        url_base + "TIME_SERIES_INTRADAY&interval={interval}" + url_symbol_api;

    stringReplace(url, "{interval}", interval);

    std::string file_name =
        "..\\..\\data\\intraday_" + interval + "_" + this->m_symbol + ".csv";

    downloadCsv(url, file_name);

    return parseIntradayCsv(file_name);
}

// Get a "func" time_series from last_n with each time_pair being:
//
// time_t unix_timestamp, vector([open, high, low, close, volume])
//
// func: avapi::DAILY avapi::WEEKLY avapi::MONTHLY
// last_n: last n days, weeks, or months
time_series Quote::getTimeSeries(const function &func, const size_t &last_n)
{
    std::string url, file_name;
    switch (func) {
    case function::DAILY:
        url = url_base + "TIME_SERIES_DAILY" + url_symbol_api;
        file_name = "..\\..\\data\\daily_" + this->m_symbol + ".csv";
        downloadCsv(url, file_name);
        return parseTimeSeriesCsv(file_name, last_n);

    case function::WEEKLY:
        url = url_base + "TIME_SERIES_WEEKLY" + url_symbol_api;
        file_name = "..\\..\\data\\weekly_" + this->m_symbol + ".csv";
        downloadCsv(url, file_name);
        return parseTimeSeriesCsv(file_name, last_n);

    case function::MONTHLY:
        url = url_base + "TIME_SERIES_MONTHLY" + url_symbol_api;
        file_name = "..\\..\\data\\monthly_" + this->m_symbol + ".csv";
        downloadCsv(url, file_name);
        return parseTimeSeriesCsv(file_name, last_n);

    default:
        std::cout << "Error: Incorrect time_series function passed."
                  << std::endl;
        time_series fail;
        return fail;
    }
}

// Get time_pair for latest global quote:
//
// time_t timestamp,
// vector([open,high,low,price,volume,prevClose,change,change%])
time_pair Quote::getGlobalQuote()
{
    std::string url = url_base + "GLOBAL_QUOTE" + url_symbol_api;
    std::string file_name =
        "..\\..\\data\\global_quote_" + this->m_symbol + ".csv";
    downloadCsv(url, file_name);

    rapidcsv::Document doc(file_name, rapidcsv::LabelParams(0, 0));

    std::vector<float> data = doc.GetRow<float>(this->m_symbol);
    data.erase(data.begin() + 5);

    time_pair global_quote = std::make_pair(
        dateToUnixTimestamp(doc.GetColumn<std::string>("latestDay")[0]), data);

    return global_quote;
}

// Construct time_series from intraday data csv
time_series Quote::parseIntradayCsv(const std::string &file_name)
{
    rapidcsv::Document doc(file_name);

    std::vector<std::string> date_time = doc.GetColumn<std::string>(0);
    int n_data = date_time.size();

    std::vector<float> open = doc.GetColumn<float>(1);
    std::vector<float> high = doc.GetColumn<float>(2);
    std::vector<float> low = doc.GetColumn<float>(3);
    std::vector<float> close = doc.GetColumn<float>(4);
    std::vector<float> volume = doc.GetColumn<float>(5);

    time_series series;

    for (int i = 0; i < n_data; ++i) {

        std::vector<float> data;

        data.push_back(open[i]);
        data.push_back(high[i]);
        data.push_back(low[i]);
        data.push_back(close[i]);
        data.push_back(volume[i]);

        series.push_back(
            std::make_pair(dateTimeToUnixTimestamp(date_time[i]), data));
    }
    return series;
}

// Construct time_series from a data csv (non intraday)
time_series Quote::parseTimeSeriesCsv(const std::string &file_name,
                                      const size_t &last_n)
{
    rapidcsv::Document doc(file_name);

    std::vector<std::string> date = doc.GetColumn<std::string>(0);
    size_t n_data = date.size();

    if (last_n > n_data) {
        std::cout << "Error: Not enough data in in file for last_n series"
                  << '\n';
        time_series fail;
        return fail;
    }

    std::vector<float> open = doc.GetColumn<float>(1);
    std::vector<float> high = doc.GetColumn<float>(2);
    std::vector<float> low = doc.GetColumn<float>(3);
    std::vector<float> close = doc.GetColumn<float>(4);
    std::vector<float> volume = doc.GetColumn<float>(5);

    time_series series;

    for (int i = 0; i < last_n; ++i) {

        std::vector<float> data;

        data.push_back(open[i]);
        data.push_back(high[i]);
        data.push_back(low[i]);
        data.push_back(close[i]);
        data.push_back(volume[i]);

        series.push_back(std::make_pair(dateToUnixTimestamp(date[i]), data));
    }
    return series;
}

// Helper Functions -----------------------

// Curls .csv data file using "t_url" and saves it to .csv "file_name"
void downloadCsv(const std::string &t_url, const std::string &file_name)
{
    const char *url = t_url.c_str();
    const char *file = file_name.c_str();
    CURL *curl;
    FILE *fp;
    CURLcode res;
    curl = curl_easy_init();
    if (curl) {
        fp = fopen(file, "wb");
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        res = curl_easy_perform(curl);

        /* always cleanup */
        curl_easy_cleanup(curl);
        fclose(fp);
    }
}

// Replaces string "from" to "to" within "str"
bool stringReplace(std::string &str, const std::string from,
                   const std::string to)
{
    size_t start = str.find(from);
    if (start == std::string::npos) {
        return false;
    }
    str.replace(start, from.length(), to);
    return true;
}

// Reads the first line from provided file, used mainly for reading an api key
// .txt file
std::string readFirstLineFromFile(const std::string &file_name)
{
    std::string api_key;
    std::ifstream file(file_name);
    if (file.is_open()) {
        std::getline(file, api_key);
        file.close();
    }
    else {
        std::cout << "Unable to open file: " << file_name;
    }
    return api_key;
}

// Converts date + time string "%Y-%m-%d %H:%M:%S" to Unix Timestamp (seconds
// since epoch)
std::time_t dateTimeToUnixTimestamp(std::string input)
{
    std::tm t{};
    std::istringstream ss(input);
    ss >> std::get_time(&t, "%Y-%m-%d %H:%M:%S");
    return mktime(&t);
}

// Converts date string "%Y-%m-%d" to Unix Timestamp (seconds since epoch)
std::time_t dateToUnixTimestamp(std::string input)
{
    std::tm t{};
    std::istringstream ss(input);
    ss >> std::get_time(&t, "%Y-%m-%d");
    return mktime(&t);
}

// Prints the data within time_series to console
void printSeries(const time_series &series)
{
    for (auto &pair : series) {
        std::cout << pair.first << ':';
        for (auto &val : pair.second) {
            std::cout << ' ' << val;
        }
        std::cout << '\n';
    }
}

// Prints the data within time_pair to console
void printPair(const time_pair &pair)
{
    std::cout << pair.first << ':';
    for (auto &val : pair.second) {
        std::cout << ' ' << val;
    }
    std::cout << '\n';
}
} // namespace avapi