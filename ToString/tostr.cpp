#include "tostr.h"

#define TOSTR_BUFFER_LEN    100

#define INT_TO_STR          0
#define UNSIGNED_TO_STR     1
#define DOBULE_TO_STR       2
#define HEX_TO_STR          3
#define BIN_TO_STR          4
#define LONG_TO_STR         5
#define LONG_LONG_TO_STR    6

Hex::Hex(long long a_value) :
    m_value(a_value),
    m_leadingBytes(0)
{
}
Hex::Hex(long long a_value, unsigned a_leadingZeroes) :
    m_value(a_value),
    m_leadingBytes(a_leadingZeroes)
{
}

Hex::~Hex() {
}

Bin::Bin(long long a_value) :
    m_value(a_value),
    m_leadingZeroes(0)
{
}
Bin::Bin(long long a_value, unsigned a_leadingZeroes) :
    m_value(a_value),
    m_leadingZeroes(a_leadingZeroes)
{
}

Bin::~Bin() {
}


ToStr::ToStr() {
}

ToStr::ToStr(ToStr & in) {
}

ToStr::ToStr(int in) :
    m_int(in),
    m_unsigned(0),
    m_double(0.0),
    m_long(0),
    m_longlong(0),
    m_which(INT_TO_STR),
    m_leadingZeroes(0)
{
}

ToStr::ToStr(long in) :
    m_int(0),
    m_unsigned(0),
    m_double(0.0),
    m_long(in),
    m_longlong(0),
    m_which(LONG_TO_STR),
    m_leadingZeroes(0)
{
}

ToStr::ToStr(long long in) :
    m_int(0),
    m_unsigned(0),
    m_double(0.0),
    m_long(0),
    m_longlong(in),
    m_which(LONG_LONG_TO_STR),
    m_leadingZeroes(0)
{
}

ToStr::ToStr(unsigned in) :
    m_int(0),
    m_unsigned(in),
    m_double(0.0),
    m_long(0),
    m_longlong(0),
    m_which(UNSIGNED_TO_STR),
    m_leadingZeroes(0)
{
}

ToStr::ToStr(double in) :
    m_int(0),
    m_unsigned(0),
    m_double(in),
    m_long(0),
    m_longlong(0),
    m_which(DOBULE_TO_STR),
    m_leadingZeroes(0)
{
}

ToStr::ToStr(Hex in) :
    m_int(0),
    m_unsigned(0),
    m_double(0),
    m_long(0),
    m_longlong(in.m_value),
    m_which(HEX_TO_STR),
    m_leadingZeroes(in.m_leadingBytes)
{
}

ToStr::ToStr(Bin in) :
    m_int(0),
    m_unsigned(0),
    m_double(0),
    m_long(0),
    m_longlong(in.m_value),
    m_which(BIN_TO_STR),
    m_leadingZeroes(in.m_leadingZeroes)
{
}

ToStr::~ToStr() {
}

void ToStr::convert(std::wstring & in) const {
    wchar_t buffer[TOSTR_BUFFER_LEN] = { 0 };
    in = L"0";

    switch (m_which) {
    case INT_TO_STR:
        if (S_OK == StringCbPrintfW(buffer, TOSTR_BUFFER_LEN, L"%d", m_int)) {
            in = std::wstring(buffer);
        }
        break;
    case UNSIGNED_TO_STR:
        if (S_OK == StringCbPrintfW(buffer, TOSTR_BUFFER_LEN, L"%u", m_unsigned)) {
            in = std::wstring(buffer);
        }
        break;
    case DOBULE_TO_STR:
        if (S_OK == StringCbPrintfW(buffer, TOSTR_BUFFER_LEN, L"%lf", m_double)) {
            in = std::wstring(buffer);
        }
        break;
    case HEX_TO_STR:
        if (S_OK == StringCbPrintfW(buffer, TOSTR_BUFFER_LEN, L"%I64X", m_longlong)) {
            in = std::wstring(buffer);

            unsigned elements = in.length();

            //in this context, m_leadingZeroes mean leading bytes
            if (m_leadingZeroes > elements) {
                std::wstring    leadingZeroes;
                unsigned        max = 0;

                max = m_leadingZeroes - elements;

                for (unsigned i = 0; i < max; i++) {
                    leadingZeroes.append(L"0");
                }
                in = leadingZeroes + in;
            }
        }
        break;
    case BIN_TO_STR:
        {
            bool fLogicalOneOccurred = false;
            std::wstring output;

            for (unsigned i = 0; i < 64; i++) {
                if (m_longlong & (0x8000000000000000ull >> i)) {
                    output += L"1";

                    fLogicalOneOccurred = true;
                }
                else {
                    // dont print '0' before first '1', unless leading zeroes are on
                    if (fLogicalOneOccurred || (m_leadingZeroes >= 64 - i)) {
                        output += L"0";
                    }
                }
            }

            in = output;
        }
        break;
    case LONG_TO_STR:
        if (S_OK == StringCbPrintfW(buffer, TOSTR_BUFFER_LEN, L"%li", m_long)) {
            in = std::wstring(buffer);
        }
        break;
    case LONG_LONG_TO_STR:
        if (S_OK == StringCbPrintfW(buffer, TOSTR_BUFFER_LEN, L"%I64li", m_longlong)) {
            in = std::wstring(buffer);
        }
        break;
    }
}

void ToStr::convert(std::string & in) const
{
    char buffer[TOSTR_BUFFER_LEN] = { 0 };
    in = "0";

    switch (m_which) {
    case INT_TO_STR:
        if (S_OK == StringCbPrintfA(buffer, TOSTR_BUFFER_LEN, "%d", m_int)) {
            in = std::string(buffer);
        }
        break;
    case UNSIGNED_TO_STR:
        if (S_OK == StringCbPrintfA(buffer, TOSTR_BUFFER_LEN, "%u", m_unsigned)) {
            in = std::string(buffer);
        }
        break;
    case DOBULE_TO_STR:
        if (S_OK == StringCbPrintfA(buffer, TOSTR_BUFFER_LEN, "%f", m_double)) {
            in = std::string(buffer);
        }
        break;
    case HEX_TO_STR:
        if (S_OK == StringCbPrintfA(buffer, TOSTR_BUFFER_LEN, "%I64X", m_longlong)) {
            in = std::string(buffer);

            unsigned elements = in.length();

            //in this context, m_leadingZeroes mean leading bytes
            if (m_leadingZeroes > elements) {
                std::string    leadingZeroes;
                unsigned        max = 0;

                max = m_leadingZeroes - elements;

                for (unsigned i = 0; i < max; i++) {
                    leadingZeroes.append("0");
                }
                in = leadingZeroes + in;
            }
        }
        break;
    case BIN_TO_STR:
        {
            bool fLogicalOneOccurred = false;
            std::string output;

            for (unsigned i = 0; i < 64; i++) {
                if (m_longlong & (0x8000000000000000ull >> i)) {
                    output += "1";

                    fLogicalOneOccurred = true;
                }
                else
                {
                    // dont print '0' before first '1', unless leading zeroes are on
                    if (fLogicalOneOccurred || (m_leadingZeroes >= 64 - i)) {
                        output += "0";
                    }
                }
            }

            in = output;
        }
        break;
        break;
    case LONG_TO_STR:
        if (S_OK == StringCbPrintfA(buffer, TOSTR_BUFFER_LEN, "%li", m_long)) {
            in = std::string(buffer);
        }
        break;
    case LONG_LONG_TO_STR:
        if (S_OK == StringCbPrintfA(buffer, TOSTR_BUFFER_LEN, "%I64li", m_longlong)) {
            in = std::string(buffer);
        }
        break;
    }
}

// friend
std::wstring operator+(const std::wstring & a_lval, const ToStr & a_rval) {
    std::wstring lval = a_lval;
    std::wstring rval = a_rval;

    lval += rval;

    return lval;
}

 std::string operator+(const std::string & a_lval, const ToStr & a_rval) {
    std::string lval = a_lval;
    std::string rval = a_rval;

    lval += rval;

    return lval;
}
