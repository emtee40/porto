#include <sstream>

#include "mount.hpp"
#include "file.hpp"
#include "registry.hpp"
#include "util/string.hpp"

using namespace std;

TError TMount::Mount() {
    int ret = mount(device.c_str(), mountpoint.c_str(), vfstype.c_str(),
                    mountflags, CommaSeparatedList(flags).c_str());

    if (ret)
        return TError(EError::Unknown);

    return TError::Success();
}

TError TMount::Umount() {
    int ret = umount(mountpoint.c_str());

    if (ret)
        return TError(EError::Unknown);

    return TError::Success();
}

// from single /proc/self/mounts line, like:
// /dev/sda1 /boot ext4 rw,seclabel,relatime,data=ordered 0 0
TMount::TMount(const string &mounts_line) {
    istringstream ss(mounts_line);
    string flag_string, t;
    ss >> device >> mountpoint >> vfstype >> flag_string;

    for (auto i = flag_string.begin(); i != flag_string.end(); i++) {
        if (*i == ',' && !t.empty()) {
            flags.insert(t);
            t.clear();
        } else
            t += *i;
    }

    if (!t.empty())
        flags.insert(t);
}

TMountSnapshot::TMountSnapshot() {
    TFile f("/proc/self/mounts");

    vector<string> lines;
    f.AsLines(lines);
    for (auto line : lines)
        mounts.insert(make_shared<TMount>(line));
}

set<shared_ptr<TMount> > const& TMountSnapshot::Mounts() {
    return mounts;
}

ostream& operator<<(ostream& os, const TMountSnapshot& ms) {
    for (auto m : ms.mounts)
        os << *m << endl;

    return os;
}
