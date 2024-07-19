#pragma once

#include <string>
#include <vector>
#include <exception>
#include <set>
#include <list>
#include <memory>

#include "Logging.h"

namespace LMN {

enum class ArgType {
    Indicator, 
    Value, 
    List, 
    File //TODO
};


// Conditional inheriter
template <ArgType ARG_T, class INDICATOR_T, class VALUE_T, class LIST_T, class FILE_T>
struct conditional_base;

template <class INDICATOR_T, class VALUE_T, class LIST_T, class FILE_T>
struct conditional_base<ArgType::Indicator, INDICATOR_T, VALUE_T, LIST_T, FILE_T> {using type = INDICATOR_T;};
template <class INDICATOR_T, class VALUE_T, class LIST_T, class FILE_T>
struct conditional_base<ArgType::Value, INDICATOR_T, VALUE_T, LIST_T, FILE_T> {using type = VALUE_T;};
template <class INDICATOR_T, class VALUE_T, class LIST_T, class FILE_T>
struct conditional_base<ArgType::List, INDICATOR_T, VALUE_T, LIST_T, FILE_T> {using type = LIST_T;};
template <class INDICATOR_T, class VALUE_T, class LIST_T, class FILE_T>
struct conditional_base<ArgType::File, INDICATOR_T, VALUE_T, LIST_T, FILE_T> {using type = FILE_T;};

// Forward dec
class ArgParser;

class IndicatorDefinition;

template <ArgType ARG_T, typename DATA_T>
class ValueDefinition;

template <ArgType ARG_T, typename DATA_T>
class ListDefinition;

class FileDefinition;

template <ArgType ARG_T, typename DATA_T = void, typename _BASE = typename conditional_base<ARG_T, IndicatorDefinition, ValueDefinition<ARG_T, DATA_T>, ListDefinition<ARG_T, DATA_T>, FileDefinition>::type>
class ArgDefinition;

/* Arg classes */

struct ArgBase {
    std::string label;
};

class IndicatorArg : public ArgBase {};

template <typename DATA_T>
class ValueArg : public ArgBase {
    public:
        /// @brief Get the value of a Value type argument
        /// @return Value
        const DATA_T& value() const;

        ~ValueArg();

        template <ArgType ARG_T, typename _DATA_T, typename _BASE>
        friend class ArgDefinition;
    protected:
        std::shared_ptr<DATA_T> m_val = nullptr;
};

template <typename DATA_T>
class ListArg : public ArgBase {
    public:
        /// @brief Access List type argument
        /// @return Pointer to start of list
        const DATA_T* begin() const;

        /// @brief Access List type argument
        /// @return Pointer to end of list
        const DATA_T* end() const;

        ~ListArg();

        template <ArgType ARG_T, typename _DATA_T, typename _BASE>
        friend class ArgDefinition;
    protected:
        std::vector<DATA_T> m_list;
};

/* TODO */
class FileArg {};

template <ArgType ARG_T, typename DATA_T = void, typename _BASE = typename conditional_base<ARG_T, IndicatorArg, ValueArg<DATA_T>, ListArg<DATA_T>, FileArg>::type>
class Arg : public _BASE {
    public:
        Arg() = default;

        template <class ARG_DEF>
        Arg(ARG_DEF& def);

        Arg& operator=(const Arg&) = default;

        /// @brief Check if the argument was passed
        operator bool() const;

        template <ArgType _ARG_T, typename _DATA_T, typename __BASE>
        friend class ArgDefinition;
    private:
        bool m_has = false;

};

/* Arg Definitions */

class IndicatorDefinition {};

template <ArgType ARG_T, typename DATA_T>
class ValueDefinition {
    public:
        /// @brief Supply a default value. Only enabled for Value type
        /// @param default_val Default value
        [[nodiscard]] ArgDefinition<ARG_T, DATA_T>& defaultValue(DATA_T&& default_val);

    public:
        std::shared_ptr<DATA_T> m_default_val = nullptr;
};

template <ArgType ARG_T, typename DATA_T>
class ListDefinition {
    public:
        /// @brief Supply a default list. Only enabled for List type
        /// @param default_val Default value
        [[nodiscard]] ArgDefinition<ARG_T, DATA_T>& defaultList(std::initializer_list<DATA_T> l);

    public:
        std::vector<DATA_T> m_default_list;
};

class FileDefinition {};

template <ArgType ARG_T, typename DATA_T, typename _BASE>
class ArgDefinition : public _BASE {
    public:
        ArgDefinition(ArgParser* parser);

        /// @brief Add flag definition
        /// @param flag Flag value
        [[nodiscard]] ArgDefinition<ARG_T, DATA_T, _BASE>& flag(char flag);

        /// @brief Add key definition
        /// @param key Key value
        [[nodiscard]] ArgDefinition<ARG_T, DATA_T, _BASE>& key(const char* key);

        /// @brief Add argument description
        /// @param desc Description string. Must be string literal or live until enableHelp() is called
        [[nodiscard]] ArgDefinition<ARG_T, DATA_T, _BASE>& description(const char* desc);

        /// @brief Assert that an agrument is required
        [[nodiscard]] ArgDefinition<ARG_T, DATA_T, _BASE>& required();

        ///// @brief Parse the argument
        //operator Arg<ARG_T, DATA_T>();

        ~ArgDefinition();

        friend class Arg<ARG_T, DATA_T>;
    private:
        std::unique_ptr<Arg<ARG_T, DATA_T>> parse();

    private:
        ArgParser* m_parser;

        bool m_has_flag = false;
        bool m_has_key = false;
        bool m_has_desc = false;
        bool m_required = false;

        bool m_parsed = false;

        char m_flag;
        const char* m_key = nullptr;;
        const char* m_desc = nullptr;

};

class ArgParser {
    public:
        ArgParser(int argc, char** argv);

        template <typename T>
        T to(const std::string& str);

        template <typename T>
        std::string from(const T& v);

        template <ArgType ARG_T, typename DATA_T = void>
        ArgDefinition<ARG_T, DATA_T> addDef();

        bool enableHelp();

        template <ArgType ARG_T, typename DATA_T, typename _BASE>
        friend class ArgDefinition;
    private:
        struct Documentation {
            char flag = '\0';
            const char* key = nullptr;
            const char* description = nullptr;
            std::string default_value = std::string();
            std::string default_list = std::string();
        };

    private:
        bool isValue(const char* first_char) const;
        inline std::string getFlagStr(char flag) const;
        inline std::string getKeyStr(const char* key) const;
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

#include "impl/ArgParser_conversions.hpp"
#include "impl/ArgParser_impl.hpp"