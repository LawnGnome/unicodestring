#ifndef USTRING_H_DEFINED
#define USTRING_H_DEFINED

#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include <unicode/unistr.h>

class ICUError : public std::runtime_error {
	public:
		ICUError(const std::ostringstream &ss) : std::runtime_error(ss.str()) {}
		ICUError(const std::string &what) : std::runtime_error(what) {}
};

class MalformedInput : public ICUError {
	public:
		MalformedInput(const std::ostringstream &ss) : ICUError(ss) {}
		MalformedInput(const std::string &what) : ICUError(what) {}
};

class NormalisationError : public ICUError {
	public:
		NormalisationError(const std::ostringstream &ss) : ICUError(ss) {}
		NormalisationError(const std::string &what) : ICUError(what) {}
};

class NotFoundError : public ICUError {
	public:
		NotFoundError(const std::ostringstream &ss) : ICUError(ss) {}
		NotFoundError(const std::string &what) : ICUError(what) {}
};

class UString {
	public:
		typedef std::map<UChar32, size_t> CodepointCount;

		UString();
		UString(const UString &us);
		UString(const char *buffer, size_t length, const char *charset);
		UString(const UChar32 *buffer, size_t length);

		UString &operator=(const UString &src);
		bool operator<(const UString &other) const;
		bool operator<=(const UString &other) const;
		bool operator>(const UString &other) const;
		bool operator>=(const UString &other) const;
		bool operator==(const UString &other) const;
		bool operator!=(const UString &other) const;

		UChar32 charAt(size_t offset) const;
		short compare(const UString &other) const;
		CodepointCount countCodepoints() const;
		size_t find(const UString &needle) const;
		size_t length() const { return data.size(); }
		UString replace(const UString &needle, const UString &replacement) const;
		void remove(size_t offset);
		UString reverse() const;
		void set(const char *buffer, size_t length, const char *charset);
		void set(const UChar32 *buffer, size_t length);
		void setCharAt(size_t offset, const UString &src);
		UString substring(size_t offset) const;
		UString substring(size_t offset, size_t length) const;
		UString toLower() const;
		UString toUpper() const;
		std::string toUTF8() const;

	private:
		typedef std::vector<UChar32> UStringData;
		UStringData data;

		void setData(const UnicodeString &us);
};

#endif
