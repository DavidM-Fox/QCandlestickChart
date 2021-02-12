#ifndef STOCKER_API_H 
#define STOCKER_API_H

#include <string>
#include <curl/curl.h>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <stdio.h>

namespace stocker
{
    class quote
    {
        public:
            quote(std::string t_symbol);
            ~quote();

            // Setters
            void set_function(std::string t_function);
            void set_symbol(std::string t_symbol);
            void set_interval(std::string t_interval);
            
            // Getters
            std::string get_function();
            std::string get_symbol();
            std::string get_interval();

            void download(const std::string &t_function);
    
        private:

            bool build_url(const std::string &t_function);
            bool string_replace(std::string& str, const std::string from, const std::string to);
            
            std::string m_api_key;
            std::string m_symbol;
            std::string m_function;
            std::string m_interval;
            std::string m_fileName;
            std::string m_url;
    };
    

    // quote constructor:
    quote::quote(std::string t_symbol) : m_symbol(t_symbol)
    {
        std::string api_key;
        std::ifstream file("..\\..\\api.key");
        if (file.is_open())
        {
            std::getline(file, api_key);
            this->m_api_key = api_key;
            file.close();
        }
        else
        {
            std::cout << "Unable to open file";
        }
    }
    
    // quote deconstructor
    quote::~quote() { }

    // Class Setters
    void quote::set_function(std::string t_function) {this->m_function   = t_function;}
    void quote::set_symbol(std::string t_symbol)     {this->m_symbol     = t_symbol;}
    void quote::set_interval(std::string t_interval) {this->m_interval   = t_interval;}

    // Class Getters
    std::string quote::get_function()   {return this->m_function;}
    std::string quote::get_symbol()     {return this->m_symbol;}
    std::string quote::get_interval()   {return this->m_interval;}

    /*
    Downloads historical stock data for a set range.
    t_function can be "Today", "Daily", "Weekly", or "Monthly".
    Returns an std::string of the .csv file.
    */
    void quote::download(const std::string &t_function)
    {
        if(build_url(t_function))
        {
            std::string file = "..\\..\\stock_data\\" + t_function + "_" + get_symbol() + ".csv";
            const char *url  = this->m_url.c_str();
            CURL *curl;
            FILE *fp;
            CURLcode res;
            curl = curl_easy_init();
            if (curl)
            {
                fp = fopen(file.c_str(), "wb");
                curl_easy_setopt(curl, CURLOPT_URL, url);
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
                curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
                res = curl_easy_perform(curl);

                /* always cleanup */
                curl_easy_cleanup(curl);
                fclose(fp);
            }
        }
        else
        {
            std::cout << "Error: t_function is incorrect";
        }
    }
    
    // Builds download url from quote data
    bool quote::build_url(const std::string &t_function)
    {
        std::string url;
        if(t_function == "Today")
            url = "https://www.alphavantage.co/query?function=TIME_SERIES_INTRADAY&symbol={symbol}&interval=15min&apikey={api}&datatype=csv";
        else if(t_function == "Daily")
            url = "https://www.alphavantage.co/query?function=TIME_SERIES_DAILY&symbol={symbol}&apikey={api}&datatype=csv";
        else if(t_function == "Weekly")
            url = "https://www.alphavantage.co/query?function=TIME_SERIES_WEEKLY&symbol={symbol}&apikey={api}&datatype=csv";
        else if(t_function == "Monthly")
            url = "https://www.alphavantage.co/query?function=TIME_SERIES_MONTHLY&symbol={symbol}&apikey={api}&datatype=csv";
        else
            return false;
        
        string_replace(url, "{symbol}", this->m_symbol);
        string_replace(url, "{api}", this->m_api_key);
        this->m_url = url;

        return true;
    }

    // Replaces string "{from}" to "{to}"
    bool quote::string_replace(std::string& str, const std::string from, const std::string to)
    {
        size_t start = str.find(from);
        if(start == std::string::npos)
        {
            return false;
        }
        str.replace(start, from.length(), to);
        return true;
    }
}

#endif // STOCKER_API_H