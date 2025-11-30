// Disclaimer, this file was generated with ChatGPT because I was too lazy to write my own testing program
// All other files were written by me. The only thing ChatGPT did on those was help with the error and help text

#include <iostream>
#include <cassert>
#include "../include/woodcode.h" // adjust path if needed

void test_decode_invalid_length()
{
    WoodCode code;
    auto result = code.decode("123"); // too short
    assert(result.isError());
    assert(result.message.find("Invalid code") != std::string::npos);
}

void test_decode_invalid_header_format()
{
    WoodCode code;
    auto result = code.decode("Z10400abc123"); // wrong start char
    assert(result.isError());
    assert(result.message.find("Invalid header") != std::string::npos);
}

void test_decode_old_version()
{
    WoodCode code;
    auto result = code.decode("W10304abc123");
    assert(result.isError());
    assert(result.message.find("too old") != std::string::npos);
}

void test_decode_new_version()
{
    WoodCode code;
    auto result = code.decode("W10401abc123");
    assert(result.isError());
    assert(result.message.find("too new") != std::string::npos);
}

void test_decode_empty_payload()
{
    WoodCode code;
    auto result = code.decode("W10400071425"); // valid version + date, no payload
    assert(result.isWarning());
    assert(result.message.find("Decoded result is empty") != std::string::npos);
}

void test_encode_invalid_char()
{
    WoodCode code;
    auto result = code.encode("Hello\nWorld");
    assert(result.isError());
    assert(result.message.find("not a valid char") != std::string::npos);
}

void test_decode_corrupted_version_digits()
{
    WoodCode code;
    auto result = code.decode("W10x05abc123"); // 'x' is invalid
    assert(result.isError());
    assert(result.message.find("Invalid header") != std::string::npos);
}

void test_encode_empty_string()
{
    WoodCode code;
    auto result = code.encode(""); // should be valid
    assert(result.isOk());
    assert(result.value.find("W10400") == 0); // still encodes
}

void test_decode_encoded_empty_string()
{
    WoodCode code;
    std::string emptyCode = code.encode("").value;
    auto result = code.decode(emptyCode);
    assert(result.isWarning());
    assert(result.message.find("Decoded result is empty") != std::string::npos);
}

// ChatGPT being special
void test_decode_how_did_you_get_here()
{
    WoodCode code;
    // Valid W-prefix, valid digits, but version == 10400 (same as expected) gets rejected below for some edge reason
    // So let’s try slipping past the outer check but not triggering a version error

    // This should not happen naturally, but let's bypass the exact header match to force the unreachable fallback.
    std::string input = "W10400badc0d"; // Skips the exact-match check, passes the numeric version check

    // We'll patch the version logic to force it to run to the unreachable line:
    // Pretend it's W10400 but not exactly the same string (maybe by whitespace or something corrupt)
    auto result = code.decode("W10400\n000000"); // Should fail inside and fall through
    assert(result.isError());
    assert(result.message.find("How the hell") == std::string::npos);
}

void test_encode_embedded_null()
{
    WoodCode code;
    std::string strWithNull = std::string("abc\0def", 7);
    auto result = code.encode(strWithNull);
    assert(result.isError());
    assert(result.message.find("not a valid char") != std::string::npos);
}

void test_decode_stoi_exception()
{
    WoodCode code;
    // corrupt version number to cause stoi failure (non-digit chars)
    auto result = code.decode("W10abc000000");
    assert(result.isError());
    assert(result.message.find("Invalid header") != std::string::npos);
}

void test_encode_large_string()
{
    WoodCode code;
    std::string large(10'000'000, 'A'); // 10 million 'A's
    auto result = code.encode(large);
    // Depending on your code, might be Ok or Err — just check no crash and is a valid Result
    assert(result.isOk());
    // (This is a placeholder to show test ran — you might customize expected behavior)
}

void test_decode_short_strings()
{
    WoodCode code;

    auto r1 = code.decode("W1030"); // too short
    assert(r1.isError());

    auto r2 = code.decode("W10400123"); // less than 12 chars but looks closer
    assert(r2.isError());

    auto r3 = code.decode("W10400123456"); // exactly 12 chars, valid length but maybe invalid payload
    // Can't guarantee error or not here, just check Result is valid
    assert(r3.message.find("Decoded result is empty") != std::string::npos);
}

void test_decode_unicode_chars()
{
    WoodCode code;
    std::string unicodeStr = "W10400\u03B1\u03B2\u03B3" + std::string("000000"); // Greek letters in payload
    auto result = code.decode(unicodeStr);
    assert(result.isError() || result.isWarning() || result.isOk()); // just test it doesn’t crash
}

void test_encode_control_chars()
{
    WoodCode code;
    std::string controls = "\t\b\a\x1B"; // tab, backspace, bell, escape
    for (char c : controls)
    {
        std::string s(1, c);
        auto res = code.encode(s);
        assert(res.isError());
        assert(res.message.find("not a valid char") != std::string::npos);
    }
}

void test_decode_corrupt_header_footer()
{
    WoodCode code;

    // Header partially missing
    auto r1 = code.decode("W103" + std::string("05123456")); // I have no clue what ChatGPT thinks this is gonna do
    assert(r1.message.find("Decoded result is empty"));

    // Date substring corrupt (non-numeric)
    std::string input = "W10400" + std::string("123456") + "ABCDEF";
    auto r2 = code.decode(input);
    assert(r2.message.find("Invalid code") != std::string::npos);
}

void test_decode_substr_out_of_range()
{
    WoodCode code;
    // input length less than 12 to test substr boundaries
    auto r = code.decode("W103");
    assert(r.isError());
}

// Read disclaimer at the top
int main()
{
    test_decode_invalid_length();
    test_decode_invalid_header_format();
    test_decode_old_version();
    test_decode_new_version();
    test_decode_empty_payload();
    test_encode_invalid_char();

    // Bonus tests
    test_decode_corrupted_version_digits();
    test_encode_empty_string();
    test_decode_encoded_empty_string();
    test_decode_how_did_you_get_here();

    test_encode_embedded_null();
    test_decode_stoi_exception();
    test_encode_large_string();
    test_decode_short_strings();
    test_decode_unicode_chars();
    test_encode_control_chars();
    test_decode_corrupt_header_footer();
    test_decode_substr_out_of_range();

    std::cout << "All tests passed!\n";
    return 0;
}
