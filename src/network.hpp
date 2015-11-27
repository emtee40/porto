#pragma once

#include <memory>
#include <string>
#include <mutex>

#include "common.hpp"
#include "util/netlink.hpp"
#include "util/locks.hpp"

class TNetwork;

class TQdisc : public TNonCopyable {
    const uint32_t Handle;
    const uint32_t DefClass;

public:
    TQdisc(uint32_t handle, uint32_t defClass) : Handle(handle), DefClass(defClass) { }

    TError Create(const TNlLink &link);
    TError Remove(const TNlLink &link);
    uint32_t GetHandle() { return Handle; }
};

class TNetwork : public std::enable_shared_from_this<TNetwork>,
                 public TNonCopyable,
                 public TLockable {
    std::shared_ptr<TNl> Nl;
    std::vector<std::shared_ptr<TNlLink>> Links;
    std::shared_ptr<TQdisc> Qdisc;

    std::vector<std::pair<std::string, int>> ifaces;
    struct nl_sock *rtnl;

    TError PrepareLink(TNlLink &link);

public:
    TError UpdateInterfaces();

    TNetwork();
    ~TNetwork();
    TError Connect();
    TError Prepare();
    TError Update();
    // OpenLinks doesn't lock TNetwork
    TError OpenLinks(std::vector<std::shared_ptr<TNlLink>> &links);
    TError Destroy();

    std::vector<std::shared_ptr<TNlLink>> GetLinks() { return Links; }
    std::shared_ptr<TQdisc> GetQdisc() { return Qdisc; }

    TError GetTrafficCounters(int minor, ETclassStat stat,
                              std::map<std::string, uint64_t> &result);

    TError UpdateTrafficClasses(int parent, int minor,
            std::map<std::string, uint64_t> &Prio,
            std::map<std::string, uint64_t> &Rate,
            std::map<std::string, uint64_t> &Ceil);
    TError RemoveTrafficClasses(int minor);

    TError AddTrafficClass(int ifIndex, uint32_t parent, uint32_t handle,
                           uint64_t prio, uint64_t rate, uint64_t ceil);
    TError DelTrafficClass(int ifIndex, uint32_t handle);

    static TError NetlinkError(int error, const std::string description);
    static void DumpNetlinkObject(const std::string &prefix, void *obj);
};