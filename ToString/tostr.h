#include <Strsafe.h>
#include <string>

class ToStr;

class Hex {
    friend class            ToStr;
private:
    unsigned long long      m_value;
    unsigned                m_leadingBytes;
public:
                            Hex(long long);
                            Hex(long long, unsigned);
                            ~Hex();
};

class Bin {
    friend class            ToStr;
private:
    unsigned long long      m_value;
    unsigned                m_leadingZeroes;
public:
                            Bin(long long);
                            Bin(long long, unsigned);
                            ~Bin();
};


class ToStr {
private:
    int                     m_int;
    unsigned                m_unsigned;
    double                  m_double;
    long                    m_long;
    long long               m_longlong;

    unsigned                m_which;
    unsigned                m_leadingZeroes;

    inline void             convert(std::wstring &) const;
    inline void             convert(std::string &) const;

                            ToStr();
                            ToStr(ToStr &);
public:
                            ToStr(int);
                            ToStr(long);
                            ToStr(long long);
                            ToStr(unsigned);
                            ToStr(double);
                            ToStr(Hex);
                            ToStr(Bin);
                            ~ToStr();

    // ASCII string
    inline operator         std::string() const;
    inline std::string      operator+(const std::string &);
    friend std::string      operator+(const std::string &, const ToStr &);

    // UNICODE strings
    inline operator         std::wstring() const;
    inline std::wstring     operator+(const std::wstring &);
    friend std::wstring     operator+(const std::wstring &, const ToStr &);
};

// ASCII string
inline ToStr::operator std::string() const {
    std::string ret;
    convert(ret);
    return ret;
}

inline std::string ToStr::operator+(const std::string & a_arg) {
    std::string ret;
    convert(ret);
    return ret + a_arg;
}

// UNICODE strings
inline ToStr::operator std::wstring() const {
    std::wstring ret;
    convert(ret);
    return ret;
}

inline std::wstring ToStr::operator+(const std::wstring & a_arg) {
    std::wstring ret;
    convert(ret);
    return ret + a_arg;
}

// ASCII string
//class StringA : public std::string {
//private:
//public:
//    StringA() : std::string() {}
//    StringA(char * p) : std::string(p) {}
//    StringA(const char * p) : std::string(p) {}
//    StringA(std::string & in) : std::string(in) {}
//    StringA(ToStr & in) : std::string(in) {}
//    ~StringA(){}
//
//    inline operator const char * () const {
//        return this->c_str();
//    }
//
//    inline StringA & operator= (std::string & in) {
//        this->clear();
//        this->append(in);
//        return *this;
//    }
//
//    inline StringA & operator= (ToStr & in) {
//        this->clear();
//        this->append(in);
//        return *this;
//    }
//};
//
//// UNICODE string
//class StringW : public std::wstring {
//private:
//public:
//    StringW() : std::wstring() {}
//    StringW(wchar_t * p) : std::wstring(p) {}
//    StringW(const wchar_t * p) : std::wstring(p) {}
//    StringW(std::wstring & in) : std::wstring(in) {}
//    StringW(ToStr & in) : std::wstring(in) {}
//    ~StringW(){}
//
//    inline operator const wchar_t * () const {
//        return this->c_str();
//    }
//
//    inline StringW & operator= (std::wstring & in) {
//        this->clear();
//        this->append(in);
//        return *this;
//    }
//
//    inline StringW & operator= (ToStr & in) {
//        this->clear();
//        this->append(in);
//        return *this;
//    }
//};
