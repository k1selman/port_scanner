#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <SFML/Network.hpp>
//try other libraries (if possible)?

static bool port_is_open(const std::string &address, int port)
{
    sf::TcpSocket socket;
    return (sf::TcpSocket().connect(address, port) == sf::Socket::Done);      //Fix this
}

static std::vector<std::string> split(const std::string &string, char delim = ' ', bool allowEmpty = false)
{
    std::vector<std::string> tokens;
    std::stringstream sstream(string);
    std::string token;

    while(std::getline(sstream, token, delim)){
        if(allowEmpty || token.size() > 0){
            tokens.push_back(token);
        }
    }

    return tokens;
}

static int string_to_int(const std::string &string)
{
    std::stringstream sstream(string);
    int i;
    sstream >> i;
    return i;
}

template <typename T>
static void swap(T &a, T &b)
{
    T c = a;
    a = b;
    b = c;
}


template <typename T>
static std::vector<T> range(T min, T max)
{
    if(min>max){
        swap(min,max);
    }

    if(min == max){
        return std::vector<T>(1,min);
    }

    std::vector<T> values;
    for(;min<=max; ++min){
        values.push_back(min);
    }

    return values;
}

static std::vector<int> ParsePortsList(const std::string &list)
{
    std::vector<int> ports;
    for(const std::string &token : split(list, ',')){
        std::vector<std::string> strRange = split(token, '-');
        switch(strRange.size()){
            case 0: ports.push_back(string_to_int(token));         break;
            case 1: ports.push_back(string_to_int(strRange[0]));   break;
            case 2:
            {
                int min = string_to_int(strRange[0]);
                int max = string_to_int(strRange[1]);
                for(int port: range(min,max)){
                    ports.push_back(port);
                }
                break;
            }
            default: break;
        }
    }

    return ports;
}



using namespace std;

int main()
{
    std::string address;
    std::string port_list;
    std::vector<int> ports;
    std::cout << "Address: " << std::flush;
    std::getline(std::cin, address);
    std::cout << "Port: " << std::flush;
    std::getline(std::cin, port_list);

    ports = ParsePortsList(port_list);
    std::cout << "Scanning for " << address << "...\n";

    for (int port : ports){
        std::cout << "Port " << port << ": ";
        if(port_is_open(address, port)){
            std::cout << "OPEN \n";
        }else{
            std::cout << "CLOSED \n";
        }
    }

    std::cout << std::flush;
    return 0;
}
