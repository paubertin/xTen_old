#include "logger.h"

#include "../xTenStd.h"
#include "criticalsection.h"
#include <time.h>

namespace xten {

#pragma region Constants, Statics, Globals

	static const char* ERRORLOG_FILENAME = "error.log";

#ifdef NDEBUG
	const unsigned char ERRORFLAG_DEFAULT =		LOGFLAG_WRITE_TO_DEBUGGER;
	const unsigned char WARNINGFLAG_DEFAULT =	LOGFLAG_WRITE_TO_DEBUGGER;
	const unsigned char INFOFLAG_DEFAULT =		LOGFLAG_WRITE_TO_DEBUGGER;
#elif _DEBUG
	const unsigned char ERRORFLAG_DEFAULT =		(LOGFLAG_WRITE_TO_DEBUGGER | LOGFLAG_WRITE_TO_LOG_FILE | LOGFLAG_WRITE_TO_CONSOLE);
	const unsigned char WARNINGFLAG_DEFAULT =	(LOGFLAG_WRITE_TO_DEBUGGER | LOGFLAG_WRITE_TO_LOG_FILE | LOGFLAG_WRITE_TO_CONSOLE);
	const unsigned char INFOFLAG_DEFAULT =		(LOGFLAG_WRITE_TO_DEBUGGER | LOGFLAG_WRITE_TO_LOG_FILE | LOGFLAG_WRITE_TO_CONSOLE);
#else
	const unsigned char ERRORFLAG_DEFAULT =		0;
	const unsigned char WARNINGFLAG_DEFAULT =	0;
	const unsigned char INFOFLAG_DEFAULT =		0;
#endif

	class LogMgr;
	static LogMgr* s_LogMgr = nullptr;

#pragma endregion

#pragma region LogMgr class

	class LogMgr
	{
	public:
		enum ErrorDialogResult
		{
			LOGMGR_ERROR_ABORT,
			LOGMGR_ERROR_RETRY,
			LOGMGR_ERROR_IGNORE
		};

		typedef std::map<std::string, unsigned char> Tags;
		typedef std::list<Logger::ErrorMessenger*> ErrorMessengerList;

		Tags m_Tags;
		ErrorMessengerList m_ErrorMessengers;

		// thread safety
		CriticalSection m_TagCriticalSection;
		CriticalSection m_MessengerCriticalSection;
	public:
		// construction
		LogMgr();
		~LogMgr();
		void init(const char* loggingConfigFilename);

		// logs
		void log(const std::string& tag, const std::string& message, const char* funcName, const char* sourceFile, unsigned int lineNum);
		void setDisplayFLags(const std::string& tag, unsigned char flags);

		// error messengers
		void addErrorMessenger(Logger::ErrorMessenger* pMessenger);
		LogMgr::ErrorDialogResult error(const std::string & errorMessage, bool isFatal, const char* funcName, const char* sourceFile, unsigned int lineNum);

		void writeEndLine();
	private:
		// helpers
		void outputFinalBufferToLogs(const std::string& finalBuffer, const std::string& tag, unsigned char flags);
		void writeToLogFile(const std::string& data);
		void writeToConsole(const std::string& tag, const std::string& data);
		void getOutputBuffer(std::string& outOutPutBuffer, const std::string& tag, const std::string& message, const char* funcName, const char* sourceFile, unsigned int lineNum);
	};

#pragma endregion

#pragma region LogMgr class definition

	LogMgr::LogMgr()
	{
		setDisplayFLags("ERROR", ERRORFLAG_DEFAULT);
		setDisplayFLags("WARNING", WARNINGFLAG_DEFAULT);
		setDisplayFLags("INFO", INFOFLAG_DEFAULT);
	}

	LogMgr::~LogMgr()
	{
		m_MessengerCriticalSection.lock();
		for (auto it = m_ErrorMessengers.begin(); it != m_ErrorMessengers.end(); ++it)
		{
			Logger::ErrorMessenger* pMessenger = (*it);
			XDEL(pMessenger);
		}
		m_ErrorMessengers.clear();
		m_MessengerCriticalSection.unlock();
	}

	void LogMgr::writeEndLine()
	{
		std::string endString;
		endString = "######### ---------------------- #########";
		endString += "\n";
		endString += "\n";
		writeToLogFile(endString);
	}

	void LogMgr::init(const char* loggingConfigFilename)
	{
		std::string iniString;
		iniString = "######### ------ XTEN LOG ------ #########";
		iniString += "\n";
		writeToLogFile(iniString);

		if (loggingConfigFilename)
		{
			TiXmlDocument loggingConfigFile(loggingConfigFilename);
			if (loggingConfigFile.LoadFile())
			{
				TiXmlElement* pRoot = loggingConfigFile.RootElement();
				if (!pRoot)
					return;
			
				for (TiXmlElement* pNode = pRoot->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement())
				{
					unsigned char flags = 0;
					std::string tag(pNode->Attribute("tag"));
			
					int debugger = 0;
					pNode->Attribute("debugger", &debugger);
					if (debugger)
						flags |= LOGFLAG_WRITE_TO_DEBUGGER;
			
					int logFile = 0;
					pNode->Attribute("file", &logFile);
					if (logFile)
						flags |= LOGFLAG_WRITE_TO_LOG_FILE;

					int console = 0;
					pNode->Attribute("console", &console);
					if (console)
						flags |= LOGFLAG_WRITE_TO_CONSOLE;
			
					setDisplayFLags(tag, flags);
				}
			}
		}
	}

	void LogMgr::log(const std::string& tag, const std::string& message, const char* funcName, const char* sourceFile, unsigned int lineNum)
	{
		m_TagCriticalSection.lock();
		Tags::iterator findIt = m_Tags.find(tag);
		if (findIt != m_Tags.end())
		{
			m_TagCriticalSection.unlock();

			std::string buffer;
			getOutputBuffer(buffer, tag, message, funcName, sourceFile, lineNum);
			outputFinalBufferToLogs(buffer, tag, findIt->second);
		}
		else
		{
			m_TagCriticalSection.unlock();
		}
	}

	void LogMgr::setDisplayFLags(const std::string& tag, unsigned char flags)
	{
		m_TagCriticalSection.lock();
		if (flags != 0)
		{
			Tags::iterator findIt = m_Tags.find(tag);
			if (findIt == m_Tags.end())
				m_Tags.insert(std::make_pair(tag, flags));
			else
				findIt->second = flags;
		}
		else
		{
			m_Tags.erase(tag);
		}
		m_TagCriticalSection.unlock();
	}

	void LogMgr::addErrorMessenger(Logger::ErrorMessenger* pMessenger)
	{
		m_MessengerCriticalSection.lock();
		m_ErrorMessengers.push_back(pMessenger);
		m_MessengerCriticalSection.unlock();
	}

	LogMgr::ErrorDialogResult LogMgr::error(const std::string & errorMessage, bool isFatal, const char* funcName, const char* sourceFile, unsigned int lineNum)
	{
		std::string tag = ((isFatal) ? ("FATAL") : ("ERROR"));

		std::string buffer;
		getOutputBuffer(buffer, tag, errorMessage, funcName, sourceFile, lineNum);

		m_TagCriticalSection.lock();
		Tags::iterator findIt = m_Tags.find(tag);
		if (findIt != m_Tags.end())
			outputFinalBufferToLogs(buffer, tag, findIt->second);
		m_TagCriticalSection.unlock();

		int result = ::MessageBoxA(NULL, buffer.c_str(), tag.c_str(), MB_ABORTRETRYIGNORE | MB_ICONERROR | MB_DEFBUTTON3);

		switch (result)
		{
		case IDIGNORE: return LogMgr::LOGMGR_ERROR_IGNORE;
		case IDABORT: __debugbreak();  return LogMgr::LOGMGR_ERROR_RETRY;
		case IDRETRY: return LogMgr::LOGMGR_ERROR_RETRY;
		default: return LogMgr::LOGMGR_ERROR_RETRY;
		}
	}

	// IMPORTANT: The two places this function is called from wrap the code in the tag critical section (m_pTagCriticalSection), 
	// so that makes this call thread safe.  If you call this from anywhere else, make sure you wrap it in that critical section.
	//------------------------------------------------------------------------------------------------------------------------------------
	void LogMgr::outputFinalBufferToLogs(const std::string& finalBuffer, const std::string& tag, unsigned char flags)
	{
		if ((flags && LOGFLAG_WRITE_TO_LOG_FILE) > 0)
			writeToLogFile(finalBuffer);
		if ((flags && LOGFLAG_WRITE_TO_DEBUGGER) > 0)
			::OutputDebugStringA(finalBuffer.c_str());
		if ((flags && LOGFLAG_WRITE_TO_CONSOLE) > 0)
			writeToConsole(tag, finalBuffer);
	}

	void LogMgr::writeToConsole(const std::string& tag, const std::string& data)
	{
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		
		if (equals(tag,"FATAL"))
			SetConsoleTextAttribute(hConsole, BACKGROUND_RED | BACKGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		else if (equals(tag,"ERROR"))
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
		else if (equals(tag,"WARNING"))
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);

		printf("%s", data.c_str());
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
	}

	void LogMgr::writeToLogFile(const std::string& data)
	{
		FILE* pLogFile = nullptr;
		fopen_s(&pLogFile, ERRORLOG_FILENAME, "a+");
		if (!pLogFile)
			return;
		fprintf_s(pLogFile, data.c_str());

		fclose(pLogFile);
	}

	void LogMgr::getOutputBuffer(std::string& outOutPutBuffer, const std::string& tag, const std::string& message, const char* funcName, const char* sourceFile, unsigned int lineNum)
	{
		char date[20];
		struct tm *sTm;
		time_t now = time(0);
		sTm = gmtime(&now);
		//strftime(date, sizeof(date), "%Y-%m-%d %H:%M:%S", sTm);
		strftime(date, sizeof(date), "%H:%M:%S", sTm);
		std::string strdate(date);
		outOutPutBuffer = strdate + " ";

		if (!tag.empty())
			outOutPutBuffer += "[" + tag + "] " + message;
		else
			outOutPutBuffer += message;

		if (funcName != nullptr)
		{
			outOutPutBuffer += "\nFunction: ";
			outOutPutBuffer += funcName;
		}

		if (sourceFile != nullptr)
		{
			outOutPutBuffer += "\n";
			outOutPutBuffer += sourceFile;
		}

		if (lineNum != 0)
		{
			outOutPutBuffer += "\nLine: ";
			char lineNumBuffer[11];
			memset(lineNumBuffer, 0, sizeof(char));
			outOutPutBuffer += _itoa(lineNum, lineNumBuffer, 10);
		}

		outOutPutBuffer += "\n";
	}

#pragma endregion

#pragma region ErrorMessenger definition

	Logger::ErrorMessenger::ErrorMessenger()
	{
		s_LogMgr->addErrorMessenger(this);
		m_Enabled = true;
	}

	void Logger::ErrorMessenger::show(const std::string & errorMessage, bool isFatal, const char* funcName, const char* sourceFile, unsigned int lineNum)
	{
		if (m_Enabled)
		{
			if (s_LogMgr->error(errorMessage, isFatal, funcName, sourceFile, lineNum) == LogMgr::LOGMGR_ERROR_IGNORE)
				m_Enabled = false;
		}
	}

#pragma endregion

#pragma region C Interface

	namespace Logger {

		void Init(const char* loggingConfigFilename)
		{
			if (!s_LogMgr)
			{
				s_LogMgr = XNEW LogMgr;
				s_LogMgr->init(loggingConfigFilename);
			}
		}

		void Destroy()
		{
			s_LogMgr->writeEndLine();
			XDEL(s_LogMgr);
		}

		void Log(const std::string& tag, const std::string& message, const char* funcName, const char* sourceFile, unsigned int lineNum)
		{
			XTEN_ASSERT(s_LogMgr);
			s_LogMgr->log(tag, message, funcName, sourceFile, lineNum);
		}

		void SetDisplayFLags(const std::string& tag, unsigned char flags)
		{
			XTEN_ASSERT(s_LogMgr);
			s_LogMgr->setDisplayFLags(tag, flags);
		}

	}

#pragma endregion

}