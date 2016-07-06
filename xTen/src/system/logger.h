#pragma once

#include <string>

// display flags
const unsigned char LOGFLAG_WRITE_TO_LOG_FILE = 1 << 0;
const unsigned char LOGFLAG_WRITE_TO_DEBUGGER = 1 << 1;
const unsigned char LOGFLAG_WRITE_TO_CONSOLE  = 1 << 2;

namespace xten { namespace Logger {

	class ErrorMessenger
	{
	private:
		bool m_Enabled;
	public:
		ErrorMessenger();
		void show(const std::string & errorMessage, bool isFatal, const char* funcName, const char* sourceFile, unsigned int lineNum);
	};

	void Init(const char* loggingConfigFilename);
	void Destroy();

	void Log(const std::string& tag, const std::string& message, const char* funcName, const char* sourceFile, unsigned int lineNum);
	void SetDisplayFLags(const std::string& tag, unsigned char flags);

}

#ifdef _DEBUG
#define XTEN_ASSERT(expression)															\
do																						\
{																						\
	if (!(expression))																	\
	{																					\
		static Logger::ErrorMessenger* pErrorMessenger = XNEW Logger::ErrorMessenger;	\
		pErrorMessenger->show(#expression, false, __FUNCTION__, __FILE__, __LINE__);	\
	}																					\
} while(0)


#define XTEN_LOG(tag, str)																\
do                                                                                      \
{																						\
	std::string s((str));																\
	Logger::Log(tag, s, NULL, NULL, 0);													\
} while (0)

#define XTEN_INFO(str)																	\
do                                                                                      \
{																						\
	std::string s((str));																\
	Logger::Log("INFO", s, NULL, NULL, 0);												\
} while (0)

#define XTEN_WARN(str)																	\
do                                                                                      \
{																						\
	std::string s((str));																\
	Logger::Log("WARNING", s, __FUNCTION__, __FILE__, __LINE__);						\
} while (0)

#define XTEN_ERROR(str)																	\
do                                                                                      \
{																						\
	static Logger::ErrorMessenger* pErrorMessenger = XNEW Logger::ErrorMessenger;	    \
	std::string s((str));																\
	pErrorMessenger->show(s, false, __FUNCTION__, __FILE__, __LINE__);					\
} while (0)

#else
#define XTEN_ASSERT(expression) do { (void)sizeof(expression); } while(0)
#define XTEN_LOG(tag, str) do { (void)sizeof(tag); (void)sizeof(str); } while(0)
#define XTEN_INFO(str) do { (void)sizeof(str); } while(0)
#define XTEN_WARN(str) do { (void)sizeof(str); } while(0)
#define XTEN_ERROR(str) do { (void)sizeof(str); } while(0)
#endif
}
