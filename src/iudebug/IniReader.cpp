
/* inih -- simple .INI file parser

inih is released under the New BSD license (see LICENSE.txt). Go to the project
home page for more info:

https://github.com/benhoyt/inih

*/

// Adapted by rtk0c for the IuDebug project

#include "IniReader.h"

#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <cstring>
#include <algorithm>

#define MAX_SECTION 50
#define MAX_NAME 50

/* Strip whitespace chars off end of given string, in place. Return s. */
inline static char* rstrip(char* s)
{
    char* p = s + strlen(s);
    while (p > s && isspace((unsigned char)(*--p)))
        *p = '\0';
    return s;
}

/* Return pointer to first non-whitespace char in given string. */
inline static char* lskip(const char* s)
{
    while (*s && isspace((unsigned char)(*s)))
        s++;
    return (char*)s;
}

/* Return pointer to first char (of chars) or inline comment in given string,
   or pointer to null at end of string if neither found. Inline comment must
   be prefixed by a whitespace character to register as a comment. */
inline static char* find_chars_or_comment(const char* s, const char* chars)
{
#if INI_ALLOW_INLINE_COMMENTS
    int was_space = 0;
    while (*s && (!chars || !strchr(chars, *s)) &&
           !(was_space && strchr(INI_INLINE_COMMENT_PREFIXES, *s))) {
        was_space = isspace((unsigned char)(*s));
        s++;
    }
#else
    while (*s && (!chars || !strchr(chars, *s))) {
        s++;
    }
#endif
    return (char*)s;
}

/* Version of strncpy that ensures dest (size bytes) is null-terminated. */
inline static char* strncpy0(char* dest, const char* src, size_t size)
{
    strncpy(dest, src, size);
    dest[size - 1] = '\0';
    return dest;
}

/* See documentation in header file. */
int ini_parse_stream(ini_reader reader, void* stream, ini_handler handler,
                     void* user)
{
    /* Uses a fair bit of stack (use heap instead if you need to) */
#if INI_USE_STACK
    char line[INI_MAX_LINE];
#else
    char* line;
#endif
    char section[MAX_SECTION] = "";
    char prev_name[MAX_NAME] = "";
    
    char* start;
    char* end;
    char* name;
    char* value;
    int lineno = 0;
    int error = 0;
    
#if !INI_USE_STACK
    line = (char*)malloc(INI_MAX_LINE);
    if (!line) {
        return -2;
    }
#endif
    
    /* Scan through stream line by line */
    while (reader(line, INI_MAX_LINE, stream) != NULL) {
        lineno++;
        
        start = line;
#if INI_ALLOW_BOM
        if (lineno == 1 && (unsigned char)start[0] == 0xEF &&
            (unsigned char)start[1] == 0xBB &&
            (unsigned char)start[2] == 0xBF) {
            start += 3;
        }
#endif
        start = lskip(rstrip(start));
        
        if (*start == ';' || *start == '#') {
            /* Per Python configparser, allow both ; and # comments at the
               start of a line */
        }
#if INI_ALLOW_MULTILINE
        else if (*prev_name && *start && start > line) {
            
#if INI_ALLOW_INLINE_COMMENTS
            end = find_chars_or_comment(start, NULL);
            if (*end)
                *end = '\0';
            rstrip(start);
#endif
            
            /* Non-blank line with leading whitespace, treat as continuation
               of previous name's value (as per Python configparser). */
            if (!handler(user, section, prev_name, start) && !error)
                error = lineno;
        }
#endif
        else if (*start == '[') {
            /* A "[section]" line */
            end = find_chars_or_comment(start + 1, "]");
            if (*end == ']') {
                *end = '\0';
                strncpy0(section, start + 1, sizeof(section));
                *prev_name = '\0';
            }
            else if (!error) {
                /* No ']' found on section line */
                error = lineno;
            }
        }
        else if (*start) {
            /* Not a comment, must be a name[=:]value pair */
            end = find_chars_or_comment(start, "=:");
            if (*end == '=' || *end == ':') {
                *end = '\0';
                name = rstrip(start);
                value = lskip(end + 1);
#if INI_ALLOW_INLINE_COMMENTS
                end = find_chars_or_comment(value, NULL);
                if (*end)
                    *end = '\0';
#endif
                rstrip(value);
                
                /* Valid name[=:]value pair found, call handler */
                strncpy0(prev_name, name, sizeof(prev_name));
                if (!handler(user, section, name, value) && !error)
                    error = lineno;
            }
            else if (!error) {
                /* No '=' or ':' found on name[=:]value line */
                error = lineno;
            }
        }
        
#if INI_STOP_ON_FIRST_ERROR
        if (error)
            break;
#endif
    }
    
#if !INI_USE_STACK
    free(line);
#endif
    
    return error;
}

/* See documentation in header file. */
int ini_parse_file(FILE* file, ini_handler handler, void* user)
{
    return ini_parse_stream((ini_reader)fgets, file, handler, user);
}

/* See documentation in header file. */
int ini_parse(const char* filename, ini_handler handler, void* user)
{
    FILE* file;
    int error;
    
    file = fopen(filename, "r");
    if (!file)
        return -1;
    error = ini_parse_file(file, handler, user);
    fclose(file);
    return error;
}

//~ C++ wrapper

int IniReader_ValueHandler(void* user, const char* sectionName, const char* name, const char* value)
{
    auto& self = *(IniReader*)user;
    auto& sectionStorage = self.values[sectionName];
    // NOTE: this creates a std::string for name even if the entry exists already; we don't really care since IniReader is intended to be used on small config files only
    sectionStorage[name] = value;
    return 1;
}

int IniReader::ParseFile(const char* filename)
{
    return ini_parse(filename, IniReader_ValueHandler, this);
}

int IniReader::ParseFile(FILE* file)
{
    return ini_parse_file(file, IniReader_ValueHandler, this);
}

const std::string* IniReader::GetString(std::string_view section, std::string_view name) const
{
    auto iter = values.find(section);
    if (iter == values.end()) return nullptr;
    auto iter2 = iter->second.find(name);
    if (iter2 == iter->second.end()) return nullptr;
    return &iter2->second;
}

Optional<int64_t> IniReader::GetInteger(std::string_view section, std::string_view name) const
{
    auto iter = values.find(section);
    if (iter == values.end()) return {};
    auto iter2 = iter->second.find(name);
    if (iter2 == iter->second.end()) return {};
    
    auto& valstr = iter2->second;
    const char* value = valstr.c_str();
    
    char* end;
    // This parses "1234" (decimal) and also "0x4D2" (hex)
    long n = strtol(value, &end, 0);
    if (end > value)
        return n;
    else
        return {};
}

Optional<double> IniReader::GetReal(std::string_view section, std::string_view name) const
{
    auto iter = values.find(section);
    if (iter == values.end()) return {};
    auto iter2 = iter->second.find(name);
    if (iter2 == iter->second.end()) return {};
    
    auto& valstr = iter2->second;
    const char* value = valstr.c_str();
    
    char* end;
    double n = strtod(value, &end);
    if (end > value)
        return n;
    else
        return {};
}

Optional<float> IniReader::GetFloat(std::string_view section, std::string_view name) const
{
    auto iter = values.find(section);
    if (iter == values.end()) return {};
    auto iter2 = iter->second.find(name);
    if (iter2 == iter->second.end()) return {};
    
    auto& valstr = iter2->second;
    const char* value = valstr.c_str();
    
    char* end;
    float n = strtof(value, &end);
    if (end > value)
        return n;
    else
        return {};
}

Optional<bool> IniReader::GetBoolean(std::string_view section, std::string_view name) const
{
    auto iter = values.find(section);
    if (iter == values.end()) return {};
    auto iter2 = iter->second.find(name);
    if (iter2 == iter->second.end()) return {};
    
    auto& valstr = iter2->second;
    
    if (valstr == "true")
        return true;
    else if (valstr == "false")
        return false;
    else
        return {};
}
