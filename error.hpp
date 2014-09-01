#ifndef __ERROR_HPP__
#define __ERROR_HPP__

#include <string>
#include <ostream>

#include "rpc.pb.h"

using ::rpc::EError;

class TError {
public:
    TError();
    TError(EError e, std::string description);
    TError(EError e, int eno, std::string description);

    // return true if non-successful
    operator bool() const;

    EError GetError() const;
    std::string GetErrorName() const;
    const std::string &GetMsg() const;

    static const TError& Success() {
        static TError e;
        return e;
    }

    friend std::ostream& operator<<(std::ostream& os, const TError& err) {
        os << err.Error << " (" << err.Description << ")";
        return os;
    }

private:
    EError Error;
    std::string Description;
};

#endif
