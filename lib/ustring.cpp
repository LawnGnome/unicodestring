#include "ustring.h"

#include <algorithm>
#include <cassert>
#include <cstring>
#include <string>

#include <unicode/normlzr.h>

UString::UString() {}

UString::UString(const UString &us) : data(us.data) {}

UString::UString(const char *buffer, size_t length, const char *charset) {
	assert(buffer != NULL);
	assert(charset != NULL);

	UnicodeString us(buffer, length, charset);

	if (us.isBogus()) {
		std::ostringstream message;
		message << "Invalid input string for charset "
			<< charset;
		throw MalformedInput(message);
	}

	UnicodeString norm;
	UErrorCode err = U_ZERO_ERROR;
	Normalizer::normalize(us, UNORM_NFC, 0, norm, err);

	if (U_FAILURE(err)) {
		std::ostringstream message;
		message << "Error normalising string: "
			<< u_errorName(err);
		throw NormalisationError(message);
	}

	setData(norm);
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

void UString::reverse() {
	std::reverse(data.begin(), data.end());
}

void UString::setCharAt(size_t offset, UChar32 ch) {
	if (offset >= data.size()) {
		throw std::out_of_range("Offset is out of range");
	}

	data[offset] = ch;
}

void UString::setCharAt(size_t offset, const UString &src) {
	try {
		setCharAt(offset, src.charAt(0));
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
	std::copy(data.begin() + offset, data.begin() + offset + length, us.data.begin());

	return us;
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

std::string UString::toUTF8() const {
	/* This is a pretty naive implementation: we can and should cache this
	 * within the object until the string actually changes. */
	std::string dest;
	StringByteSink<std::string> sink(&dest);

	UnicodeString::fromUTF32(data.data(), data.size()).toUTF8(sink);
	return dest;
}

void UString::setData(const UnicodeString &us) {
	int32_t bufferLength = us.countChar32();
	UChar32 *buffer = new UChar32[bufferLength];
	UErrorCode err = U_ZERO_ERROR;

	us.toUTF32(buffer, bufferLength, err);

	if (U_FAILURE(err)) {
		std::ostringstream message;
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
}
