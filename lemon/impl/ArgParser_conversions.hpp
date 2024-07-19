#pragma once

#include "ArgParser.h"

#include <string>

template <>
std::string LMN::ArgParser::to<std::string>(const std::string& str) {return str;}
template <>
std::string LMN::ArgParser::from<std::string>(const std::string& v) {return v;}

template <>
char LMN::ArgParser::to<char>(const std::string& str) { return str[0]; }
template <>
std::string LMN::ArgParser::from<char>(const char& v) {return std::to_string(v);}

template <>
int LMN::ArgParser::to<int>(const std::string& str) {return std::stoi(str, std::string::size_type());}
template <>
std::string LMN::ArgParser::from<int>(const int& v) {return std::to_string(v);}

template <>
uint32_t LMN::ArgParser::to<uint32_t>(const std::string& str) {return static_cast<uint32_t>(std::stoi(str, std::string::size_type()));}
template <>
std::string LMN::ArgParser::from<uint32_t>(const uint32_t& v) {return std::to_string(v);}

template <>
int64_t LMN::ArgParser::to<int64_t>(const std::string& str) {return std::stoi(str, std::string::size_type());}
template <>
std::string LMN::ArgParser::from<int64_t>(const int64_t& v) {return std::to_string(v);}

template <>
uint64_t LMN::ArgParser::to<uint64_t>(const std::string& str) {return static_cast<uint64_t>(std::stoi(str, std::string::size_type()));}
template <>
std::string LMN::ArgParser::from<uint64_t>(const uint64_t& v) {return std::to_string(v);}

template <>
float LMN::ArgParser::to<float>(const std::string& str) {
    std::string::size_type sz;
    return std::stof(str, &sz);
}
template <>
std::string LMN::ArgParser::from<float>(const float& v) {return std::to_string(v);}