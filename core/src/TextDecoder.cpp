/*
* Copyright 2016 Nu-book Inc.
* Copyright 2022 gitlost
* Copyright 2025 Axel Waggershauser
*/
// SPDX-License-Identifier: Apache-2.0

#include "TextDecoder.h"

#include "ZXAlgorithms.h"
#include "libzueci/zueci.h"

#include <cassert>
#include <stdexcept>

namespace ZXing {

std::string BytesToUtf8(ByteView bytes, ECI eci)
{
	constexpr unsigned int replacement = 0xFFFD;
	constexpr unsigned int flags = ZUECI_FLAG_SB_STRAIGHT_THRU | ZUECI_FLAG_SJIS_STRAIGHT_THRU;
	int utf8_len;

	if (eci == ECI::Unknown)
		eci = ECI::Binary;

	int error_number = zueci_dest_len_utf8(ToInt(eci), bytes.data(), bytes.size(), replacement, flags, &utf8_len);
	if (error_number >= ZUECI_ERROR)
		throw std::runtime_error("zueci_dest_len_utf8 failed");

	std::string utf8(utf8_len, 0);

	error_number = zueci_eci_to_utf8(ToInt(eci), bytes.data(), bytes.size(), replacement, flags,
									 reinterpret_cast<uint8_t*>(utf8.data()), &utf8_len);
	if (error_number >= ZUECI_ERROR)
		throw std::runtime_error("zueci_eci_to_utf8 failed");

	assert(Size(utf8) == utf8_len);

	return utf8;
}

} // ZXing
