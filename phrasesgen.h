#ifndef _INCLUDE_PHRASESGEN_H_
#define _INCLUDE_PHRASESGEN_H_

#include "smsdk_ext.h"
#include "ILanguageFileParser.h"

#include <string>
#include <unordered_set>
#include <fstream>
#include <atomic>
#include <chrono>
#include <vector>
#include <algorithm>
#include <filesystem>

class CPhrasesGenerator :
	public ILanguageFileParserListener,
	public IThread
{
public:
	CPhrasesGenerator():
		m_bEnglishFile(false),
		m_pszLangCode(NULL),
		m_nSection(Section_None),
		m_nParsed(0),
		m_pThread(NULL),
		m_bReqTerm(false)
	{
	}

public:
	void Generate();
	void SDK_OnUnload();
	void LoadWhitelist();

private:
	enum Section_t
	{
		Section_None,
		Section_Settings,
		Section_Tokens,
		Section_Unknown,
	};

	std::vector<std::string> m_vecLangWhitelist;

public: // ILanguageFileParserListener
	ParseAction_t State_EnteredSection(const char* pszKey) override;
	ParseAction_t State_KeyValue(const char* pszKey, const char* pszValue) override;
	void State_Ended(bool halted, bool failed) override;

private:
	bool m_bEnglishFile;
	const char* m_pszLangCode;
	std::ofstream m_Out;

	Section_t m_nSection;
	size_t m_nParsed;

	IThreadHandle* m_pThread;
	std::atomic<bool> m_bReqTerm;
	std::chrono::high_resolution_clock::time_point m_tmBegin;

	void ParseTokensFromFile(const char* pszBase, const char* pszLanguage);

public: // IThread
	/**
	 * @brief Called when the thread runs (in its own thread).
	 *
	 * @param pHandle		Pointer to the thread's handle.
	 */
	void RunThread(IThreadHandle* pHandle) override;

	/**
	 * @brief Called when the thread terminates.  This occurs inside the thread as well.
	 *
	 * @param pHandle		Pointer to the thread's handle.
	 * @param cancel		True if the thread did not finish, false otherwise.
	 */
	void OnTerminate(IThreadHandle* pHandle, bool cancel) override;
};

#endif // _INCLUDE_PHRASESGEN_H_
