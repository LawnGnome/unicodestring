#include "ustring.h"

#include <algorithm>
#include <cassert>
#include <cstring>
#include <string>

#include <unicode/normlzr.h>
#include <unicode/ucnv.h>

#define THROW_UERROR(exception, prefix, err) \
	{ \
		std::ostringstream message; \
		message << (prefix) << u_errorName(err); \
		throw exception(message); \
	}

UnicodeString normalise(const UnicodeString &us) {
	UnicodeString norm;
	UErrorCode err = U_ZERO_ERROR;
	Normalizer::normalize(us, UNORM_NFC, 0, norm, err);

	if (U_FAILURE(err)) {
		THROW_UERROR(NormalisationError, "Error normalising string: ", err);
	}

	return norm;
}

UString::UString() {}

UString::UString(const UString &us) : data(us.data) {}

UString::UString(const char *buffer, size_t length, const char *charset) {
	set(buffer, length, charset);
}

UString::UString(const UChar32 *buffer, size_t length) {
	set(buffer, length);
}

UString &UString::operator=(const UString &src) {
	data = src.data;
	return *this;
}

bool UString::operator<(const UString &other) const {
	return (compare(other) == -1);
}

bool UString::operator<=(const UString &other) const {
	return (compare(other) != 1);
}

bool UString::operator>(const UString &other) const {
	return (compare(other) == 1);
}

bool UString::operator>=(const UString &other) const {
	return (compare(other) != -1);
}

bool UString::operator==(const UString &other) const {
	return (compare(other) == 0);
}

bool UString::operator!=(const UString &other) const {
	return (compare(other) != 0);
}

UChar32 UString::charAt(size_t offset) const {
	return data.at(offset);
}

short UString::compare(const UString &us) const {
	if (data > us.data) {
		return 1;
	} else if (data < us.data) {
		return -1;
	}

	assert(data == us.data);
	return 0;
}

UString::CodepointCount UString::countCodepoints() const {
	CodepointCount count;

	for (UStringData::const_iterator i = data.begin(); i != data.end(); i++) {
		++count[*i];
	}

	return count;
}

size_t UString::find(const UString &needle) const {
	UStringData::const_iterator pos = std::search(data.begin(), data.end(), needle.data.begin(), needle.data.end());

	if (pos == data.end()) {
		throw NotFoundError("Search string not found");
	}

	return pos - data.begin();
}

void UString::remove(size_t offset) {
	if (offset >= data.size()) {
		throw std::out_of_range("Offset out of range");
	}

	// You really wouldn't want to do this often.
	UStringData newData;

	newData.reserve(data.size() - 1);
	for (UStringData::const_iterator i = data.begin(); i != data.end(); i++) {
		if (((size_t) (i - data.begin())) != offset) {
			newData.push_back(*i);
		}
	}

	data = newData;
}

UString UString::replace(const UString &needle, const UString &replacement) const {
	UString replaced;
	UStringData::const_iterator it = data.begin(), prev = data.begin();

	while (data.end() != (it = std::search(prev, data.end(), needle.data.begin(), needle.data.end()))) {
		for (UStringData::const_iterator i = prev; i != it; i++) {
			replaced.data.push_back(*i);
		}
		for (UStringData::const_iterator i = replacement.data.begin(); i != replacement.data.end(); i++) {
			replaced.data.push_back(*i);
		}
		prev = it + needle.data.size();
	}

	for (UStringData::const_iterator i = prev; i != it; i++) {
		replaced.data.push_back(*i);
	}

	return replaced;
}

UString UString::reverse() const {
	UString reversed(*this);

	std::reverse(reversed.data.begin(), reversed.data.end());
	return reversed;
}

size_t UString::rfind(const UString &needle) const {
    UStringData::const_iterator pos = std::find_end(data.begin(), data.end(), needle.data.begin(), needle.data.end());

	if (pos == data.end()) {
		throw NotFoundError("Search string not found");
	}

	return pos - data.begin();
}

void UString::set(const char *buffer, size_t length, const char *charset) {
	assert(buffer != NULL);
	assert(charset != NULL);

	UnicodeString us(buffer, length, charset);

	if (us.isBogus()) {
		std::ostringstream message;
		message << "Invalid input string for charset "
			<< charset;
		throw MalformedInput(message);
	}

	setData(normalise(us));
}

void UString::set(const UChar32 *buffer, size_t length) {
	assert(buffer != NULL);

	setData(normalise(UnicodeString::fromUTF32(buffer, length)));
}

void UString::setCharAt(size_t offset, const UString &src) {
	if (data.size() <= offset) {
		throw std::out_of_range("Offset out of range");
	}

	try {
		data[offset] = src.charAt(0);
	} catch (std::out_of_range e) {
		throw std::invalid_argument("Source string must have at least one character");
	}

}

UString UString::substring(size_t offset) const {
	return substring(offset, data.size() - offset);
}

UString UString::substring(size_t offset, size_t length) const {
	if (offset >= data.size()) {
		throw std::out_of_range("Offset out of range");
	}

	if ((offset + length) > data.size()) {
		throw std::out_of_range("Length out of range");
	}

	UString us;
	us.data.reserve(length);
	for (UStringData::const_iterator i = data.begin() + offset; i != data.begin() + offset + length; i++) {
		us.data.push_back(*i);
	}

	return us;
}

std::string UString::toEncoding(const char *encoding) const {
	char *output;
	size_t outputSize;
	std::string result;
	UnicodeString us(UnicodeString::fromUTF32(data.data(), data.size()));
	UConverter *converter;
	UErrorCode err = U_ZERO_ERROR;

	if ((converter = ucnv_open(encoding, &err)) == NULL) {
		THROW_UERROR(ConversionError, "Error creating output converter: ", err);
	}

	outputSize = UCNV_GET_MAX_BYTES_FOR_STRING(us.length(), ucnv_getMaxCharSize(converter));
	output = new char[outputSize];

	outputSize = ucnv_fromUChars(converter, output, outputSize, us.getBuffer(), us.length(), &err);
	if (U_FAILURE(err)) {
		delete[] output;
		ucnv_close(converter);
		THROW_UERROR(ConversionError, "Error converting string: ", err);
	}

	result.assign(output, outputSize);

	delete[] output;
	ucnv_close(converter);

	return result;
}

UString UString::toLower() const {
	UString us;

	us.setData(UnicodeString::fromUTF32(data.data(), data.size()).toLower());
	return us;
}

UString UString::toUpper() const {
	UString us;

	us.setData(UnicodeString::fromUTF32(data.data(), data.size()).toUpper());
	return us;
}

void UString::setData(const UnicodeString &us) {
	int32_t bufferLength = us.countChar32();
	UChar32 *buffer = new UChar32[bufferLength];
	UErrorCode err = U_ZERO_ERROR;

	us.toUTF32(buffer, bufferLength, err);

	if (U_FAILURE(err)) {
		std::ostringstream message;
		delete[] buffer;
		message << "Error converting UnicodeString to UTF-32 buffer: "
			<< u_errorName(err);
		throw ICUError(message);
	}

	data.clear();
	// TODO: There must be a better way of doing this!
	data.reserve(bufferLength);
	for (int32_t i = 0; i < bufferLength; i++) {
		data.push_back(buffer[i]);
	}

	delete[] buffer;
}
