#pragma once

#include <string>
#include <vector>
#include <exception>
#include <set>
#include <list>

#include "Logging.h"

namespace LMN {

enum class ArgType {
    Indicator, 
    Value, 
    List, 
    File //TODO
};


// Forward dec
template <ArgType ARG_T, typename DATA_T>
class ArgDefinition;

class ArgParser;

template <ArgType ARG_T, typename DATA_T = void>
class Arg {
    public:
        Arg() = default;

        /// @brief Check if the argument was passed
        operator bool() const;

        /// @brief Get the value of a Value type argument
        /// @return Value
        const DATA_T& value() const;

        /// @brief Access List type argument
        /// @return Pointer to start of list
        const DATA_T* begin() const;

        /// @brief Access List type argument
        /// @return Pointer to end of list
        const DATA_T* end() const;

        ~Arg();

        friend class ArgDefinition<ARG_T, DATA_T>;
    private:
        std::string m_label;

        bool m_has = false;

        const DATA_T* m_val = nullptr;
        std::pair<const DATA_T*, std::size_t> m_list = {nullptr, 0};
};

template <ArgType ARG_T, typename DATA_T = void>
class ArgDefinition {
    public:
        ArgDefinition(ArgParser* parser);

        /// @brief Add flag definition
        /// @param flag Flag value
        ArgDefinition<ARG_T, DATA_T> flag(char flag);

        /// @brief Add key definition
        /// @param key Key value
        ArgDefinition<ARG_T, DATA_T> key(const char* key);

        /// @brief Add argument description
        /// @param desc Description string. Must be string literal or live until enableHelp() is called
        ArgDefinition<ARG_T, DATA_T> description(const char* desc);

        /// @brief Assert that an agrument is required
        ArgDefinition<ARG_T, DATA_T> required();

        /// @brief Supply a default value. Only enabled for Value type
        /// @param default_val Default value
        ArgDefinition<ARG_T, DATA_T> defaultValue(DATA_T&& default_val);

        /// @brief Supply a default list. Only enabled for List type
        /// @param default_val Default value
        ArgDefinition<ARG_T, DATA_T> defaultList(std::initializer_list<DATA_T> l);

        /// @brief Parse the argument
        operator Arg<ARG_T, DATA_T>();

        ~ArgDefinition();
    private:
        ArgParser* m_parser;

        bool m_has_flag = false;
        bool m_has_key = false;
        bool m_has_desc = false;
        bool m_required = false;
        bool m_has_default_val = false;
        bool m_has_default_list = false;

        bool m_parsed = false;

        char m_flag;
        const char* m_key = nullptr;;
        const char* m_desc = nullptr;

        const DATA_T* m_default_val = nullptr;
        std::pair<const DATA_T*, std::size_t> m_default_list = {nullptr, 0};
};

class ArgParser {
    public:
        ArgParser(int argc, char** argv);

        template <ArgType ARG_T, typename DATA_T>
        friend class Argument;
        
        template <typename T>
        T to(const std::string& str);

        template <typename T>
        std::string from(const T& v);

        template <ArgType ARG_T, typename DATA_T = void>
        ArgDefinition<ARG_T, DATA_T> addDef();

    private:
        struct Documentation {
            //Documentation() = default;
            //Documentation(const std::string& flag_, const std::string& key_, const std::string& default_value_, const std::string& description_)
            //    : flag(flag_)
            //    , key(key_)
            //    , default_value(default_value_)
            //    , description(description_)
            //{}

            char flag;
            const char* key;
            const char* description;
            std::string default_value;
            std::string default_list;
        };

    private:
        bool isValue(const char* first_char) const;
        inline std::string getFlag(char flag) const;
        inline std::string getKey(const char* key) const;
        std::string getLabel(const char* flag, const char* key);
        bool lookupIndicator(const char* flag, const char* key);
        std::pair<const char*, bool> lookupCstrValue(const char* flag, const char* key);
        std::pair<std::list<const char*>, bool> lookupCstrList(const char* flag, const char* key);

        void addDocumentation(Documentation&& documentation);

        void checkNewFlag(const char* flag);
        void checkNewKey(const char* key);


    private:
        int m_argc;
        char** m_argv;

        std::vector<bool> m_checked;
        std::list<Documentation> m_docs;

        std::set<std::string> m_unique_keys;
        std::set<char> m_unique_flags;
        bool m_help;
};

}

// parser.parse<Indicator, Value, List, File>().flag().key().defaultValue().description()





namespace BRY {

    template <typename T>
    class DataArgument {
        public:
            void set(const T& arg) {m_arg = arg;}
            void set(T&& arg) {m_arg = std::move(arg);}
            const T& get() const {
                return m_arg;
            }
        private:
            T m_arg;
    };

    class IndicatorArgument {};

    template <typename T, typename BASE = std::conditional<!std::is_same<T, void>::value, DataArgument<T>, IndicatorArgument>::type>
    class Argument : public BASE {
        public:
            Argument(bool valid) : m_valid(valid) {}
            //operator bool() const {return m_valid;} // this makes it easy to forget to use get()
            inline bool has() const {return m_valid;}
        private:
            bool m_valid;
    };

    template <typename T>
    class ArrayArgument {
        public:
            ArrayArgument(const std::vector<T>& args, bool valid) : m_args(args), m_valid(valid) {}
            const std::vector<T>& get() const {return m_args;}
            operator bool() const {return m_valid;}
        private:
            std::vector<T> m_args;
            bool m_valid;
    };

    class ArgParser {
        public:
            ArgParser(int argc, char** argv) 
                : m_argc(argc)
                , m_argv(argv)
                , m_checked(argc, false) 
            {
                for (int i = 1; i < argc; ++i) {
                    std::string arg = argv[i];
                    if (arg == "--help" || arg == "-h") 
                        m_help = true;
                }
                m_unique_keys.insert("help");
                m_unique_flags.insert('h');
            }

            // Parse the argument given a key, flag, default value, and description. Note that using type T = char
            // may not use a character flag without default value
            template <typename T>
            Argument<T> parse(const std::string& key, char flag, const T& default_value, const std::string& description) {
                static_assert(!std::is_same<T, void>::value, "Do not specify default value with indicator");
                return _parse<T>(&flag, &key, &default_value, description);
            }

            template <typename T>
            Argument<T> parse(const std::string& key, const T& default_value, const std::string& description) {
                static_assert(!std::is_same<T, void>::value, "Do not specify default value with indicator");
                return _parse<T>(nullptr, &key, &default_value, description);
            }

            template <typename T>
            Argument<T> parse(char flag, const T& default_value, const std::string& description) {
                static_assert(!std::is_same<T, void>::value, "Do not specify default value with indicator");
                return _parse<T>(&flag, nullptr, &default_value, description);
            }

            template <typename T = void, typename _ENABLE = std::enable_if<!std::is_same<T, char>::value>::type>
            Argument<T> parse(const std::string& key, char flag, const std::string& description) {
                //static_assert(!std::is_same)
                return _parse<T>(&flag, &key, nullptr, description);
            }

            template <typename T = void>
            Argument<T> parse(const std::string& key, const std::string& description) {
                return _parse<T>(nullptr, &key, nullptr, description);
            }

            template <typename T = void>
            Argument<T> parse(char flag, const std::string& description) {
                return _parse<T>(&flag, nullptr, nullptr, description);
            }

            // Include this to enable the help function and check for unrecognized arguments
            bool enableHelp() {
                if (m_help) {
                    PRINT("\n   [Help]\n");

                    std::vector<std::string> key_and_flag_strs;
                    key_and_flag_strs.reserve(m_descriptions.size());

                    std::size_t max_length = 0;
                    for (auto&[flag, key, _, __] : m_descriptions) {
                        std::string key_and_flag;
                        if (!flag.empty() && !key.empty()) {
                            key_and_flag = key + " or " + flag;
                        } else if (!key.empty()) {
                            key_and_flag = key;
                        } else if (!flag.empty()) {
                            key_and_flag = flag;
                        } else {
                            ASSERT(false, "Empty key and flag");
                        }

                        if (key_and_flag.size() > max_length)
                            max_length = key_and_flag.size();

                        key_and_flag_strs.push_back(std::move(key_and_flag));
                    }

                    std::string help_key_and_flag = "--help or -h";
                    help_key_and_flag += std::string(max_length - help_key_and_flag.size() + 1, ' ');
                    PRINT_NAMED(help_key_and_flag, "Display this message");

                    std::size_t ind = 0;
                    for (auto&[flag, key, def, desc] : m_descriptions) {
                        std::string description = desc + " ";

                        if (!def.empty()) description += " [Default value: " + def + "]";
                        std::string key_and_flag_str_adj = key_and_flag_strs[ind++];
                        key_and_flag_str_adj += std::string(max_length - key_and_flag_str_adj.size() + 1, ' ');
                        PRINT_NAMED(key_and_flag_str_adj, description);
                    }

                    NEW_LINE;

                    std::exit(0);
                    return false;
                }

                for (uint32_t i=1; i<m_argc; ++i) {
                    if (!m_checked[i]) {
                        ERROR("Unrecognized arg '" << m_argv[i] << "'");
                        std::exit(1);
                        return false;
                    }
                }
                return true;
            }

        private:
            struct Documentation {
                Documentation(const std::string& flag_, const std::string& key_, const std::string& default_value_, const std::string& description_)
                    : flag(flag_)
                    , key(key_)
                    , default_value(default_value_)
                    , description(description_)
                {}

                std::string flag;
                std::string key;
                std::string default_value;
                std::string description;
            };
        private:
            inline std::string getFlag(char flag) const {
                return "-" + std::string{flag};
            }
            inline std::string getKey(const std::string& key) const {return "--" + key;}

            template <typename T>
            T to(const std::string& str);

            template <typename T>
            std::string from(const T& v);

            bool isValue(const std::string& arg) const {
                return arg.find("-") != 0u;
            }
        
            template <typename T>
            Argument<T> _parse(const char* flag, const std::string* key, const T* default_value, const std::string& description) {
                // Determines if the argument is just an indicator, or holding a value in the next slot
                constexpr bool indicator = std::is_same<T, void>::value;

                if (!key && !flag) {
                    throw std::invalid_argument("Both key and flag cannot be unspecified");
                }

                if (key && m_unique_keys.contains(*key)) {
                    ASSERT(false, "Duplicate key: " << *key);
                } else if (key) {
                    m_unique_keys.insert(*key);
                }

                if (flag && m_unique_flags.contains(*flag)) {
                    ASSERT(false, "Duplicate flag: " << *flag);
                } else if (flag) {
                    m_unique_flags.insert(*flag);
                }

                // Flag
                std::string flag_str = std::string();
                if (flag) {
                    flag_str = getFlag(*flag);
                }

                // Key
                std::string key_str = std::string();
                if (key) {
                    ASSERT(isValue(key_str), "Do not use dashes '--' when specifying a key label");
                    key_str = getKey(*key);
                }

                // Default
                std::string default_str = std::string();
                if constexpr (!indicator) {
                    if (default_value)
                        default_str = from<T>(*default_value);
                }

                // Add the necessary descriptions
                m_descriptions.emplace_back(flag_str, key_str, default_str, description);

                if (m_help)
                    return Argument<T>(true);

                for (uint32_t i=1; i<m_argc; ++i) {
                    if (m_checked[i]) continue;

                    std::string arg = m_argv[i];
                    if ((flag && arg == flag_str) || (key && arg == key_str)) {
                        m_checked[i] = true;
                        if constexpr (indicator) {
                            return Argument<T>(true);
                        } else {
                            ASSERT(i < m_argc - 1 && isValue(m_argv[i + 1]), "Parse key: '" << key_str << "' expected a value (use type T = void for indicator arguments)");
                            m_checked[i + 1] = true;
                            T val = to<T>(m_argv[i + 1]);
                            Argument<T> ret(true);
                            ret.set(std::move(val));
                            return ret;
                        }
                    }
                }
                if constexpr (indicator) {
                    return Argument<T>(false);
                } else {
                    if (default_value) {
                        Argument<T> ret(true);
                        ret.set(*default_value);
                        return ret;
                    } else {
                        return Argument<T>(false);
                    }
                }
            }

        private:
            std::set<std::string> m_unique_keys;
            std::set<char> m_unique_flags;

            std::vector<bool> m_checked;
            int m_argc;
            char** m_argv;
            std::vector<Documentation> m_descriptions;
            bool m_help = false;
    };

    template <>
    std::string ArgParser::to<std::string>(const std::string& str) {return str;}
    template <>
    std::string ArgParser::from<std::string>(const std::string& v) {return v;}

    template <>
    char ArgParser::to<char>(const std::string& str) { return str[0]; }
    template <>
    std::string ArgParser::from<char>(const char& v) {return std::to_string(v);}

    template <>
    int ArgParser::to<int>(const std::string& str) {return std::stoi(str, std::string::size_type());}
    template <>
    std::string ArgParser::from<int>(const int& v) {return std::to_string(v);}

    template <>
    uint32_t ArgParser::to<uint32_t>(const std::string& str) {return static_cast<uint32_t>(std::stoi(str, std::string::size_type()));}
    template <>
    std::string ArgParser::from<uint32_t>(const uint32_t& v) {return std::to_string(v);}

    template <>
    int64_t ArgParser::to<int64_t>(const std::string& str) {return std::stoi(str, std::string::size_type());}
    template <>
    std::string ArgParser::from<int64_t>(const int64_t& v) {return std::to_string(v);}

    template <>
    uint64_t ArgParser::to<uint64_t>(const std::string& str) {return static_cast<uint64_t>(std::stoi(str, std::string::size_type()));}
    template <>
    std::string ArgParser::from<uint64_t>(const uint64_t& v) {return std::to_string(v);}

    template <>
    float ArgParser::to<float>(const std::string& str) {
        std::string::size_type sz;
        return std::stof(str, &sz);
    }
    template <>
    std::string ArgParser::from<float>(const float& v) {return std::to_string(v);}


}

#include "impl/ArgParser_conversions.hpp"
#include "impl/ArgParser_impl.hpp"