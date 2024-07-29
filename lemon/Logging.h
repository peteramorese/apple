#pragma once

#include "Options.h"

#include <iostream>

#ifdef LMN_LOG_COLOR
    #define LMN_LOG_WHITE(msg) "\033[0;37m" << msg << "\033[0m"
    #define LMN_LOG_BWHITE(msg) "\033[1;37m" << msg << "\033[0m"
    #define LMN_LOG_GREEN(msg) "\033[0;32m" << msg << "\033[0m"
    #define LMN_LOG_BGREEN(msg) "\033[1;32m" << msg << "\033[0m"
    #define LMN_LOG_YELLOW(msg) "\033[0;33m" << msg << "\033[0m"
    #define LMN_LOG_BYELLOW(msg) "\033[1;33m" << msg << "\033[0m"
    #define LMN_LOG_RED(msg) "\033[0;31m" << msg << "\033[0m"
    #define LMN_LOG_BRED(msg) "\033[1;31m" << msg << "\033[0m"
    #define LMN_LOG_CYAN(msg) "\033[0;36m" << msg << "\033[0m"
    #define LMN_LOG_BCYAN(msg) "\033[1;36m" << msg << "\033[0m"
    #define LMN_LOG_CYAN(msg) "\033[0;36m" << msg << "\033[0m"
    #define LMN_LOG_BCYAN(msg) "\033[1;36m" << msg << "\033[0m"
#else
    #define LMN_LOG_WHITE(msg) msg
    #define LMN_LOG_BWHITE(msg) msg
    #define LMN_LOG_GREEN(msg) msg
    #define LMN_LOG_BGREEN(msg) msg
    #define LMN_LOG_YELLOW(msg) msg
    #define LMN_LOG_BYELLOW(msg) msg
    #define LMN_LOG_RED(msg) msg
    #define LMN_LOG_BRED(msg) msg
    #define LMN_LOG_CYAN(msg) msg
    #define LMN_LOG_BCYAN(msg) msg
    #define LMN_LOG_CYAN(msg) msg
    #define LMN_LOG_BCYAN(msg) msg
#endif

#ifdef LMN_EXCLUDE_LOGS
    #define LOG(msg) 
    #define PRINT(msg) 
    #define PRINT_VEC2(msg, vec2)
    #define PRINT_VEC3(msg, vec3)
    #define PRINT_NAMED(name, msg) 
    #define DEBUG(msg)
    #define INFO(msg) 
    #define INFO_SMLN(msg) 
    // Keep error message
    #define ERROR(msg) std::cerr << "\033[1;31m >[ERR] ERROR ("<< __func__ << "): \033[1;31m" << msg << "\033[0m \n"
    #define WARN(msg) 
    #define NEW_LINE 
#else
    namespace lemon {

    class Logger {
        public:        
            static void setLogLevel(uint32_t lvl);
            
            /* TODO */
            //static void storeLogsAtLevel(uint32_t lvl);

        private:
            static uint32_t s_log_lvl;
    };

    uint32_t Logger::s_log_lvl = 0;

    }

    #define REGISTER_LOG_LVL(lvl) Logger::setLogLevel(lvl);
    /* TODO */
    #define LOG(level, msg) std::cout << "\r\033[1;36m [LOG]\033[0;37m " << msg << "\033[0m \n"

    #define PRINT(msg) std::cout << "\033[0;37m" << msg << "\033[0m \n"
    #define PRINT_VEC2(msg, vec2) std::cout << "\033[0;37m" << msg << " (" << vec2[0] << ", " << vec2[1] << ")\033[0m \n"
    #define PRINT_VEC3(msg, vec3) std::cout << "\033[0;37m" << msg << " (" << vec3[0] << ", " << vec3[1] << ", " << vec3[2] << ")\033[0m \n"
    #define PRINT_NAMED(name, msg) std::cout << "\r "<< LMN_LOG_BGREEN(name) << ": " << LMN_LOG_WHITE(msg) << "\n"
    #define DEBUG(msg) std::cout << LMN_LOG_BCYAN("\r[DBG] ("<< __func__ << "): ") << LMN_LOG_WHITE(msg) << "\n"
    #define INFO(msg) std::cout << LMN_LOG_BWHITE("\r[IFO]: ") << LMN_LOG_WHITE(msg) << " \n"
    #define INFO_SMLN(msg) std::cout << "\r" << LMN_LOG_BWHITE("\r[IFO]: ") << LMN_LOG_WHITE(msg) << std::flush
    #define ERROR(msg) std::cerr << LMN_LOG_BRED("\r[ERR] ERROR ("<< __func__ << "): ") << LMN_LOG_WHITE(msg) << "\n"
    #define WARN(msg) std::cout << LMN_LOG_BYELLOW("\r[WRN] WARNING ("<< __func__ << "): ") << LMN_LOG_WHITE(msg) << "\n"
    #define NEW_LINE std::cout<<"\n"
#endif

#ifdef LMN_ASSERTS
    #define ASSERT(condition, msg) {if (!(condition)) {ERROR("[Assert fail] " << msg); exit(1);}}
#endif

#ifdef LMN_DEBUG_TOOLS
    #define PAUSE std::cout << "<paused>" << "\n"; \
            std::cin.get()
#endif

