#pragma once

#include "ArgParser.h"
#include "Logging.h"

#include <exception>

/* Arg */

template <typename DATA_T>
const DATA_T& LMN::ValueArg<DATA_T>::value() const {
    if (!m_val) {
        ERROR("No value or default found for agument '" << label << "'");
        throw std::logic_error("Missing argument");
    } 
    return *m_val;
}

template <typename DATA_T>
LMN::ValueArg<DATA_T>::~ValueArg() {}

template <typename DATA_T>
const DATA_T* LMN::ListArg<DATA_T>::begin() const {
    if (!m_list.first) {
        ERROR("No list or default found for agument '" << label << "'");
        throw std::logic_error("Missing argument");
    }
    return m_list.first;
}

template <typename DATA_T>
const DATA_T* LMN::ListArg<DATA_T>::end() const {
    if (!m_list.first) {
        ERROR("No list or default found for agument '" << label << "'");
        throw std::logic_error("Missing argument");
    }
    return m_list.first + m_list.second;
}

template <typename DATA_T>
LMN::ListArg<DATA_T>::~ListArg() {}

template <LMN::ArgType ARG_T, typename DATA_T, typename _BASE>
template <class ARG_DEF>
LMN::Arg<ARG_T, DATA_T, _BASE>::Arg(ARG_DEF& def) {
    std::unique_ptr<Arg<ARG_T, DATA_T, _BASE>> parsed_arg = def.parse();
    // Copy the parsed arg

    *this = *parsed_arg; 
}

template <LMN::ArgType ARG_T, typename DATA_T, typename _BASE>
LMN::Arg<ARG_T, DATA_T, _BASE>::operator bool() const {
    return m_has;
}

/* ArgDefiniton */

template <LMN::ArgType ARG_T, typename DATA_T>
LMN::ArgDefinition<ARG_T, DATA_T>& LMN::ValueDefinition<ARG_T, DATA_T>::defaultValue(DATA_T&& default_val) {
    static_assert(ARG_T == ArgType::Value, "Default value only supported for ArgType::Value");
    if (!!m_default_val) {
        throw std::invalid_argument("`defaultValue()` called twice for same argument");
    }
    m_default_val.reset(new DATA_T(std::move(default_val)));
    ArgDefinition<ARG_T, DATA_T>& beep = static_cast<ArgDefinition<ARG_T, DATA_T>&>(*this);
    return static_cast<ArgDefinition<ARG_T, DATA_T>&>(*this);
}

template <LMN::ArgType ARG_T, typename DATA_T>
LMN::ArgDefinition<ARG_T, DATA_T>& LMN::ListDefinition<ARG_T, DATA_T>::defaultList(std::initializer_list<DATA_T> l) {
    static_assert(ARG_T == ArgType::List, "Default list only supported for ArgType::List");
    if (m_has_default_list) {
        throw std::invalid_argument("`defaultList()` called twice for same argument");
    }
    m_default_list = l;
    return static_cast<ArgDefinition<ARG_T, DATA_T>&>(*this);
}


template <LMN::ArgType ARG_T, typename DATA_T, typename _BASE>
LMN::ArgDefinition<ARG_T, DATA_T, _BASE>::ArgDefinition(ArgParser* parser) 
    : m_parser(parser)
{}

template <LMN::ArgType ARG_T, typename DATA_T, typename _BASE>
LMN::ArgDefinition<ARG_T, DATA_T, _BASE>& LMN::ArgDefinition<ARG_T, DATA_T, _BASE>::flag(char flag) {
    if (m_has_flag) {
        throw std::invalid_argument("`flag()` called twice for same argument");
    }
    m_flag = flag;
    m_has_flag = true;
    return *this;
}

template <LMN::ArgType ARG_T, typename DATA_T, typename _BASE>
LMN::ArgDefinition<ARG_T, DATA_T, _BASE>& LMN::ArgDefinition<ARG_T, DATA_T, _BASE>::key(const char* key) {
    if (m_has_key) {
        throw std::invalid_argument("`key()` called twice for same argument");
    }
    m_key = key;
    m_has_key = true;
    return *this;
}

template <LMN::ArgType ARG_T, typename DATA_T, typename _BASE>
LMN::ArgDefinition<ARG_T, DATA_T, _BASE>& LMN::ArgDefinition<ARG_T, DATA_T, _BASE>::description(const char* desc) {
    if (m_has_desc) {
        throw std::invalid_argument("`description()` called twice for same argument");
    }
    m_desc = desc;
    m_has_desc = true;
    return *this;
}

template <LMN::ArgType ARG_T, typename DATA_T, typename _BASE>
LMN::ArgDefinition<ARG_T, DATA_T, _BASE>& LMN::ArgDefinition<ARG_T, DATA_T, _BASE>::required() {
    static_assert(ARG_T != ArgType::Indicator, "Cannot require indicator arg");
    if (m_required) {
        throw std::invalid_argument("`required()` called twice for same argument");
    }
    m_required = true;
    return *this;
}

template <LMN::ArgType ARG_T, typename DATA_T, typename _BASE>
LMN::ArgDefinition<ARG_T, DATA_T, _BASE>::~ArgDefinition() {}

template <LMN::ArgType ARG_T, typename DATA_T, typename _BASE>
std::unique_ptr<LMN::Arg<ARG_T, DATA_T>> LMN::ArgDefinition<ARG_T, DATA_T, _BASE>::parse() {
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
    if constexpr (ARG_T == ArgType::Value) {
        if (!!this->m_default_val) {
            doc.default_value = m_parser->from(*this->m_default_val);
        }
    }
    if constexpr (ARG_T == ArgType::List) {
        if (this->m_has_default_list) {
            std::string default_list_str = "[";
            for (std::size_t i = 0; i < this->m_default_list.second; ++i) {
                default_list_str += m_parser->from(this->m_default_list.first[i]);
                if (i < this->m_default_list.second - 1) {
                    default_list_str.push_back(',');
                    default_list_str.push_back(' ');
                }
            }
            doc.default_list = default_list_str;
        }
    }

    m_parser->addDocumentation(std::move(doc));

    // Use the parser to lookup the values
    std::unique_ptr<Arg<ARG_T, DATA_T>> arg(new Arg<ARG_T, DATA_T>());
    arg->label = m_parser->getLabel(m_has_flag ? &m_flag : nullptr, m_key);

    if constexpr (ARG_T == ArgType::Indicator) {
        arg->m_has = m_parser->lookupIndicator(&m_flag, m_key);
    }
    if constexpr (ARG_T == ArgType::Value) {
        auto[c_str_val, found] = m_parser->lookupCstrValue(&m_flag, m_key);
        if (found) {
            arg->m_val.reset(new DATA_T(m_parser->to<DATA_T>(std::string(c_str_val))));
            arg->m_has = true;
        } else {
            arg->m_val = this->m_default_val;
            arg->m_has = !!this->m_default_val;
        }
    }
    if constexpr (ARG_T == ArgType::List) {
        auto[c_str_val_list, found] = m_parser->lookupCstrList(&m_flag, m_key);
        if (found) {
            //arg->m_val = new DATA_T(m_parser->to<DATA_T>(std::string(c_str_val)));
            DEBUG("TODO");
            arg->m_has = true;
        } else {
            //arg->m_val = this->m_default_val;
            arg->m_has = this->m_has_default_val;
        }
    }

    return arg;
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

template <LMN::ArgType ARG_T, typename DATA_T = void>
LMN::ArgDefinition<ARG_T, DATA_T> LMN::ArgParser::addDef() {
    return ArgDefinition<ARG_T, DATA_T>(this);
}

bool LMN::ArgParser::enableHelp() {
    if (m_help) {
        PRINT("\n   [Help]\n");

        std::vector<std::string> key_and_flag_strs;
        key_and_flag_strs.reserve(m_docs.size());

        std::size_t max_length = 0;
        for (auto&[flag, key, _, __, ___] : m_docs) {
            std::string key_and_flag;
            if (!!flag && !!key) {
                key_and_flag = getKeyStr(key) + " or " + getFlagStr(flag);
            } else if (!!key) {
                key_and_flag = getKeyStr(key);
            } else if (!!flag) {
                key_and_flag = getFlagStr(flag);
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
        for (auto&[flag, key, desc, default_val, default_list] : m_docs) {
            std::string description_display_str(desc);
            description_display_str.push_back(' ');

            if (!default_val.empty()) description_display_str += " [Default value: " + default_val + "]";
            if (!default_list.empty()) description_display_str += " [Default values: " + default_list + "]";
            std::string key_and_flag_str_adj = key_and_flag_strs[ind++];
            key_and_flag_str_adj += std::string(max_length - key_and_flag_str_adj.size() + 1, ' ');
            PRINT_NAMED(key_and_flag_str_adj, description_display_str);
        }

        NEW_LINE;

        std::exit(0);
        return false;
    }

    for (uint32_t i=1; i<m_argc; ++i) {
        if (!m_checked[i]) {
            ERROR("Unrecognized argument '" << m_argv[i] << "'");
            throw std::invalid_argument("Unrecognized argument");
        }
    }
    return true;
}

bool LMN::ArgParser::isValue(const char* first_char) const {
    return *first_char != '-';
}

std::string LMN::ArgParser::getFlagStr(char flag) const {
    if (!isValue(&flag)) {
        throw std::invalid_argument("Dash character '-' is not a valid flag");
    }
    std::string s;
    s.push_back('-');
    s.push_back(flag);
    return s;
}

std::string LMN::ArgParser::getKeyStr(const char* key) const {
    if (!isValue(key)) {
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
    if (!!key && !!flag) {
        return getKeyStr(key) + " (" + getFlagStr(*flag) + ")";
    } else if (!!key) {
        return getKeyStr(key);
    } else if (!!flag) {
        return getFlagStr(*flag);
    } else {
        throw std::invalid_argument("Cannot get label if flag and key are unspecified");
    }
}

bool LMN::ArgParser::lookupIndicator(const char* flag, const char* key) {
    checkNewFlag(flag); 
    checkNewKey(key); 
    std::string flag_str = !!flag ? getFlagStr(*flag) : std::string();
    std::string key_str = !!key ? getKeyStr(key) : std::string();

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
    std::string flag_str = !!flag ? getFlagStr(*flag) : std::string();
    std::string key_str = !!key ? getKeyStr(key) : std::string();

    if (m_help) {
        return {nullptr, false};
    }

    for (std::size_t i = 1; i < m_argc; ++i) {
        if (m_checked[i]) {
            continue;
        }

        std::string arg = m_argv[i];
        if ((!!flag && arg == flag_str) || (!!key && arg == key_str)) {
            m_checked[i] = true;
            if ((i == m_argc - 1) || !isValue(m_argv[i + 1])) {
                ERROR("Missing value for '" << getLabel(flag, key) << "'");
                throw std::invalid_argument("Missing value");
            }
            if ((i < m_argc - 2) && isValue(m_argv[i + 2])) {
                WARN("Found multiple values for '" << getLabel(flag, key) << "' when only one is expected. Ignoring extra values");
                for (std::size_t j = i + 1; j < m_argc; ++j) {
                    if (!isValue(m_argv[j])) {
                        break;
                    }
                    m_checked[j] = true;
                }
            }
            
            m_checked[i + 1] = true;
            return {m_argv[i + 1], true};
        }
    }
    return {nullptr, false};
}

std::pair<std::list<const char*>, bool> LMN::ArgParser::lookupCstrList(const char* flag, const char* key) {
    checkNewFlag(flag); 
    checkNewKey(key); 
    std::string flag_str = !!flag ? getFlagStr(*flag) : std::string();
    std::string key_str = !!key ? getKeyStr(key) : std::string();

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
    return {lst, false};
}

void LMN::ArgParser::addDocumentation(Documentation&& documentation) {
    m_docs.push_back(std::move(documentation));
}
