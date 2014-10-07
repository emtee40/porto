#ifndef __SUBSYSTEM_HPP__
#define __SUBSYSTEM_HPP__

#include <ostream>
#include <string>
#include <memory>
#include <map>

#include "cgroup.hpp"

class TCgroup;

class TSubsystem : public std::enable_shared_from_this<TSubsystem> {
    std::string Name;
    std::shared_ptr<TCgroup> RootCgroup;

    NO_COPY_CONSTRUCT(TSubsystem);

protected:
    TSubsystem(const std::string &name) : Name(name) {}

public:
    static std::shared_ptr<TSubsystem> Get(const std::string &name);
    const std::string& GetName() const;
    std::shared_ptr<TCgroup> GetRootCgroup(std::shared_ptr<TMount> mount=nullptr);
};

class TMemorySubsystem : public TSubsystem {
public:
    TMemorySubsystem() : TSubsystem("memory") {}
    TError Usage(std::shared_ptr<TCgroup> &cg, uint64_t &value) const;
    TError Statistics(std::shared_ptr<TCgroup> &cg, const std::string &name, uint64_t &val) const;
    TError UseHierarchy(TCgroup &cg) const;
};

class TFreezerSubsystem : public TSubsystem {
public:
    TFreezerSubsystem() : TSubsystem("freezer") {}

    TError WaitState(TCgroup &cg, const std::string &state) const;
    TError Freeze(TCgroup &cg) const;
    TError Unfreeze(TCgroup &cg) const;
};

class TCpuSubsystem : public TSubsystem {
public:
    TCpuSubsystem() : TSubsystem("cpu") {}
};

class TCpuacctSubsystem : public TSubsystem {
public:
    TCpuacctSubsystem() : TSubsystem("cpuacct") {}
    TError Usage(std::shared_ptr<TCgroup> &cg, uint64_t &value) const;
};

class TNetclsSubsystem : public TSubsystem {
public:
    TNetclsSubsystem() : TSubsystem("net_cls") {}
};

struct BlkioStat {
    std::string Device;
    uint64_t Read;
    uint64_t Write;
    uint64_t Sync;
    uint64_t Async;
};

class TBlkioSubsystem : public TSubsystem {
    TError GetStatLine(const std::vector<std::string> &lines,
                       const size_t i,
                       const std::string &name,
                       uint64_t &val) const;
    TError GetDevice(const std::string &majmin,
                     std::string &device) const;
public:
    TBlkioSubsystem() : TSubsystem("blkio") {}
    TError Statistics(std::shared_ptr<TCgroup> &cg, const std::string &file, std::vector<BlkioStat> &stat) const;
};

extern std::shared_ptr<TMemorySubsystem> memorySubsystem;
extern std::shared_ptr<TFreezerSubsystem> freezerSubsystem;
extern std::shared_ptr<TCpuSubsystem> cpuSubsystem;
extern std::shared_ptr<TCpuacctSubsystem> cpuacctSubsystem;
extern std::shared_ptr<TNetclsSubsystem> netclsSubsystem;
extern std::shared_ptr<TBlkioSubsystem> blkioSubsystem;

#endif
