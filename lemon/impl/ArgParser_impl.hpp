#pragma once

#include "ArgParser.h"
#include "Logging.h"

#include <exception>

/* Arg */
template <LMN::ArgType ARG_T, typename DATA_T>
LMN::Arg<ARG_T, DATA_T>::operator bool() const {
    return m_has;
}

template <LMN::ArgType ARG_T, typename DATA_T>
const DATA_T& LMN::Arg<ARG_T, DATA_T>::value() const {
    static_assert(ARG_T == ArgType::Value, "Value only supported for ArgType::Value");
    if (!m_val) {
        ERROR("No value found for agument '" << m_label << "' (no default)");
        throw std::logic_error("Missing argument");
    } 
    return m_has;
}

template <LMN::ArgType ARG_T, typename DATA_T>
const DATA_T* LMN::Arg<ARG_T, DATA_T>::begin() const {
    static_assert(ARG_T == ArgType::List, "List access only supported for ArgType::List");
    if (!m_list.first) {
        ERROR("No list found for agument '" << m_label << "' (no default)");
        throw std::logic_error("Missing argument");
    }
    return m_list.first;
}

template <LMN::ArgType ARG_T, typename DATA_T>
const DATA_T* LMN::Arg<ARG_T, DATA_T>::end() const {
    static_assert(ARG_T == ArgType::List, "List access only supported for ArgType::List");
    if (!m_list.first) {
        ERROR("No list found for agument '" << m_label << "' (no default)");
        throw std::logic_error("Missing argument");
    }
    return m_list.first + m_list.second;
}

template <LMN::ArgType ARG_T, typename DATA_T>
LMN::Arg<ARG_T, DATA_T>::~Arg() {
    if (m_val) {
        delete m_val;
    }
    if (m_list.first) {
        delete[] m_list.first;
    }
}

/* ArgDefiniton */

template <LMN::ArgType ARG_T, typename DATA_T>
LMN::ArgDefinition<ARG_T, DATA_T>::ArgDefinition(ArgParser* parser) 
    : m_parser(parser)
{}

template <LMN::ArgType ARG_T, typename DATA_T>
LMN::ArgDefinition<ARG_T, DATA_T> LMN::ArgDefinition<ARG_T, DATA_T>::flag(char flag) {
    if (m_has_flag) {
        throw std::invalid_argument("`flag()` called twice for same argument");
    }

    LMN::ArgDefinition<ARG_T, DATA_T> ret_arg = *this;
    ret_arg.m_flag = flag;
    ret_arg.m_has_flag = true;
    return ret_arg;
}

template <LMN::ArgType ARG_T, typename DATA_T>
LMN::ArgDefinition<ARG_T, DATA_T> LMN::ArgDefinition<ARG_T, DATA_T>::key(const char* key) {
    if (m_has_key) {
        throw std::invalid_argument("`key()` called twice for same argument");
    }

    LMN::ArgDefinition<ARG_T, DATA_T> ret_arg = *this;
    ret_arg.m_key = key;
    ret_arg.m_has_key = true;
    return ret_arg;
}

template <LMN::ArgType ARG_T, typename DATA_T>
LMN::ArgDefinition<ARG_T, DATA_T> LMN::ArgDefinition<ARG_T, DATA_T>::description(const char* desc) {
    if (m_has_desc) {
        throw std::invalid_argument("`description()` called twice for same argument");
    }

    LMN::ArgDefinition<ARG_T, DATA_T> ret_arg = *this;
    ret_arg.m_desc = desc;
    ret_arg.m_has_desc = true;
    return ret_arg;
}

template <LMN::ArgType ARG_T, typename DATA_T>
LMN::ArgDefinition<ARG_T, DATA_T> LMN::ArgDefinition<ARG_T, DATA_T>::required() {
    static_assert(ARG_T != ArgType::Indicator, "Cannot require indicator");
    if (m_required) {
        throw std::invalid_argument("`required()` called twice for same argument");
    }

    LMN::ArgDefinition<ARG_T, DATA_T> ret_arg = *this;
    ret_arg.m_required = true;
    return ret_arg;
}

template <LMN::ArgType ARG_T, typename DATA_T>
LMN::ArgDefinition<ARG_T, DATA_T> LMN::ArgDefinition<ARG_T, DATA_T>::defaultValue(DATA_T&& default_val) {
    static_assert(ARG_T == ArgType::Value, "Default value only supported for ArgType::Value");
    if (m_has_default_list) {
        throw std::invalid_argument("`defaultValue()` called twice for same argument");
    }

    LMN::ArgDefinition<ARG_T, DATA_T> ret_arg = *this;
    ret_arg.default_val = std::move(default_val);
    ret_arg.m_has_default_val = true;
    return ret_arg;
}

template <LMN::ArgType ARG_T, typename DATA_T>
LMN::ArgDefinition<ARG_T, DATA_T> LMN::ArgDefinition<ARG_T, DATA_T>::defaultList(std::initializer_list<DATA_T> l) {
    static_assert(ARG_T == ArgType::List, "Default list only supported for ArgType::List");
    if (m_has_default_list) {
        throw std::invalid_argument("`defaultList()` called twice for same argument");
    }

    LMN::ArgDefinition<ARG_T, DATA_T> ret_arg = *this;
    //ret_arg.default_val = std::move(default_val);
    ret_arg.m_has_default_val = true;
    return ret_arg;
}

template <LMN::ArgType ARG_T, typename DATA_T>
LMN::ArgDefinition<ARG_T, DATA_T>::operator LMN::Arg<ARG_T, DATA_T>() {
    if (!m_has_flag || !m_has_key) {
        throw std::invalid_argument("Must specify either a key or flag, did you call `flag()` or `key()`?");
    }

    // Add documentation
    ArgParser::Documentation doc;
    if (m_has_flag) {
        doc.flag = m_flag;
    }
    if (m_has_key) {
        doc.key = m_key;
    }
    if (m_has_desc) {
        doc.description = m_desc;
    }
    if (m_has_default_val) {
        doc.default_value = m_parser->from(*m_default_val);
    }
    if (m_has_default_list) {
        std::string default_list_str = "[";
        for (std::size_t i = 0; i < m_default_list.second; ++i) {
            default_list_str += m_parser->from(m_default_list.first[i]);
            if (i < m_default_list.second - 1) {
                default_list_str.push_back(',');
                default_list_str.push_back(' ');
            }
        }
        doc.default_list = default_list_str;
    }

    m_parser->addDocumentation(std::move(doc));

    // Use the parser to lookup the values
    Arg<ARG_T, DATA_T> arg;
    arg.m_label = m_parser->getLabel(m_has_flag ? &m_flag : nullptr, m_key);
}

/* ArgParser */

LMN::ArgParser::ArgParser(int argc, char** argv) 
    : m_argc(argc)
    , m_argv(argv)
    , m_checked(argc, false) 
    , m_help(false)
{
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--help" || arg == "-h") 
            m_help = true;
    }
    m_unique_keys.insert("help");
    m_unique_flags.insert('h');
}

template <ArgType ARG_T, typename DATA_T = void>
ArgDefinition<ARG_T, DATA_T> addDef() {
    return ArgDefinition<ARG_T, DATA_T>(this);
}

bool LMN::ArgParser::isValue(const char* first_char) const {
    return *first_char != '-';
}

std::string LMN::ArgParser::getFlag(char flag) const {
    if (isValue(&flag)) {
        throw std::invalid_argument("Dash character '-' is not a valid flag");
    }
    std::string s;
    s.push_back('-');
    s.push_back(flag);
    return s;
}

std::string LMN::ArgParser::getKey(const char* key) const {
    if (isValue(key)) {
        throw std::invalid_argument("Key definition must not start with dashes '-'");
    }
    return "--" + std::string(key);
}

void LMN::ArgParser::checkNewFlag(const char* flag) {
    if (!!flag && m_unique_flags.find(*flag) != m_unique_flags.end()) {
        ERROR("Duplicate flag: " << *flag);
        throw std::logic_error("Duplicate flag");
    } else if (!!flag) {
        m_unique_flags.insert(*flag);
    }
}

void LMN::ArgParser::checkNewKey(const char* key) {
    if (!!key && m_unique_keys.find(std::string(key)) != m_unique_keys.end()) {
        ERROR("Duplicate key: " << *key);
        throw std::logic_error("Duplicate key");
    } else if (!!key) {
        m_unique_keys.insert(std::string(key));
    }
}

std::string LMN::ArgParser::getLabel(const char* flag, const char* key) {
    if (!!key) {
        return getKey(key);
    } else if (!!flag) {
        return getFlag(*flag);
    } else {
        throw std::invalid_argument("Cannot get label if flag and key are unspecified");
    }
}

bool LMN::ArgParser::lookupIndicator(const char* flag, const char* key) {
    checkNewFlag(flag); 
    checkNewKey(key); 
    std::string flag_str = !!flag ? std::string() : getFlag(*flag);
    std::string key_str = !!key ? std::string() : getKey(key);

    if (m_help) {
        return false;
    }

    for (std::size_t i = 1; i < m_argc; ++i) {
        if (m_checked[i]) {
            continue;
        }

        std::string arg = m_argv[i];
        if ((flag && arg == flag_str) || (key && arg == key_str)) {
            m_checked[i] = true;
            return true;
        }
    }
    return false;
}

std::pair<const char*, bool> LMN::ArgParser::lookupCstrValue(const char* flag, const char* key) {
    checkNewFlag(flag); 
    checkNewKey(key); 
    std::string flag_str = !!flag ? std::string() : getFlag(*flag);
    std::string key_str = !!key ? std::string() : getKey(key);

    if (m_help) {
        return {nullptr, false};
    }

    for (std::size_t i = 1; i < m_argc; ++i) {
        if (m_checked[i]) {
            continue;
        }

        std::string arg = m_argv[i];
        if ((flag && arg == flag_str) || (key && arg == key_str)) {
            m_checked[i] = true;
            if ((i == m_argc - 1) || !isValue(m_argv[i + 1])) {
                ERROR("Missing value for '" << getLabel(flag, key) << "'");
                throw std::invalid_argument("Missing value");
            }
            if ((i < m_argc - 2) && !isValue(m_argv[i + 2])) {
                WARN("Found multiple values for '" << getLabel(flag, key) << "' when only one is expected. Ignoring extra values");
            }
            
            m_checked[i + 1] = true;
            return {m_argv[i + 1], true};
        }
    }
}

std::pair<std::list<const char*>, bool> LMN::ArgParser::lookupCstrList(const char* flag, const char* key) {
    checkNewFlag(flag); 
    checkNewKey(key); 
    std::string flag_str = !!flag ? std::string() : getFlag(*flag);
    std::string key_str = !!key ? std::string() : getKey(key);

    if (m_help) {
        return {std::list<const char*>(), false};
    }

    std::list<const char*> lst;

    for (std::size_t i = 1; i < m_argc; ++i) {
        if (m_checked[i]) {
            continue;
        }

        std::string arg = m_argv[i];
        if ((flag && arg == flag_str) || (key && arg == key_str)) {
            m_checked[i] = true;
            if ((i == m_argc - 1) || !isValue(m_argv[i + 1])) {
                ERROR("Missing value(s) for '" << getLabel(flag, key) << "'");
                throw std::invalid_argument("Missing value(s)");
            }
            
            for (std::size_t j = i + 1; j < m_argc; ++j) {
                if (!isValue(m_argv[j])) {
                    break;
                }
                m_checked[j] = true;
                lst.push_back(m_argv[j]);
            }
            return {lst, true};
        }
    }
}
