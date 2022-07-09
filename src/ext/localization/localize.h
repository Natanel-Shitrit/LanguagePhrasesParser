#ifndef _INCLUDE_LOCALIZE_H_
#define _INCLUDE_LOCALIZE_H_

#include <iostream>
#include <filesystem>
#include <fstream>
#include <map>

#include "smsdk_ext.h"

class CLocalize
{
public:
    // LangToken = { Key, Value }
    typedef std::pair<std::u16string, std::u16string> LangToken;
    typedef std::map<std::u16string, std::u16string> LangTokens;

    // Language = { Name, Tokens }
    typedef std::pair<std::u16string, CLocalize::LangTokens> Language;
    typedef std::map<std::u16string, CLocalize::LangTokens> Languages;

private:
    enum class LexTokenType
    {
        None,
        Value,
        // Condition, 
        Comment,
        BraceOpen,
        BraceClose,
        End

        /// TODO: Implement conditionals.
    };

    struct LexToken
    {
        LexTokenType type;
        std::u16string value;

        LexToken(LexTokenType type = LexTokenType::None, std::u16string value = u"")
            : type(type), value(std::move(value))
        {}
    };

    LexToken LexNext();
    void Parse();
    void Clear()
    {
        m_Content = {};
        m_CurrentLanguage = {};
        m_LexPos = 0;
    }

public:
    CLocalize() {}
    ~CLocalize() {}

    void Parse(std::u16string_view content)
    {
        // Save content.
        m_Content = std::move(content);
        Parse();
        Clear();
    }

    void ParseFile(std::filesystem::path filePath);
    void ParseGameLocalizationFile(std::string_view language);
    void ParseGameLocalizationFiles(std::vector<std::string_view> languages);

private: // Parser variables.
    std::u16string_view m_Content {};
    std::u16string m_CurrentLanguage {};
    size_t m_LexPos { 0 };

public:
    Languages m_Languages;
};

template <typename T>
static void StringToLower(std::basic_string<T> *str)
{
    std::transform(str->begin(), str->end(), str->begin(),
        [](unsigned char c){ return std::tolower(c); });
}

#endif // _INCLUDE_LOCALIZE_H_
