#!/usr/bin/python

import porto
from test_common import *

os.chmod("/dev/ram0", 0666)

AsAlice()

c = porto.Connection()

a = c.Run("a", wait=60, command="dd if=/dev/urandom of=/dev/null count=1", devices="/dev/urandom rw")
ExpectEq(a["exit_code"], "0")
a.Destroy()

a = c.Run("a", wait=60, command="dd if=/dev/urandom of=/dev/null count=1", devices="/dev/urandom -")
ExpectNe(a["exit_code"], "0")
a.Destroy()

a = c.Run("a", wait=60, command="dd if=/dev/ram0 of=/dev/null count=1")
ExpectNe(a["exit_code"], "0")
a.Destroy()

a = c.Run("a", wait=60, command="dd if=/dev/ram0 of=/dev/null count=1", devices="/dev/urandom rw")
ExpectNe(a["exit_code"], "0")
a.Destroy()

# disable controller

a = c.Run("a", wait=60, command="dd if=/dev/ram0 of=/dev/null count=1", **{"controllers[devices]": "false"})
ExpectEq(a["exit_code"], "0")
a.Destroy()

# allow access

a = c.Run("a", wait=60, command="dd if=/dev/ram0 of=/dev/null count=1", devices="/dev/ram0 rw")
ExpectEq(a["exit_code"], "0")
a.Destroy()

# in chroot

a = c.Run("a", wait=60, command="dd if=/dev/ram0 of=/dev/null count=1", root_volume={"layers": ["ubuntu-precise"]})
ExpectNe(a["exit_code"], "0")
a.Destroy()

a = c.Run("a", wait=60, command="dd if=/dev/ram0 of=/dev/null count=1", devices="/dev/ram0 m", root_volume={"layers": ["ubuntu-precise"]})
ExpectNe(a["exit_code"], "0")
a.Destroy()

a = c.Run("a", wait=60, command="dd if=/dev/ram0 of=/dev/null count=1", devices="/dev/ram0 rw", root_volume={"layers": ["ubuntu-precise"]})
ExpectEq(a["exit_code"], "0")
a.Destroy()

s = c.Run("s")

m = c.Run("s/m", root_volume={"layers": ["ubuntu-precise"]})

a = c.Run("s/m/a", wait=60, command="dd if=/dev/ram0 of=/dev/null count=1")
ExpectNe(a["exit_code"], "0")
a.Destroy()

s["devices"] = "/dev/ram0 rw"

a = c.Run("s/a", wait=60, command="dd if=/dev/ram0 of=/dev/null count=1", root_volume={"layers": ["ubuntu-precise"]})
ExpectEq(a["exit_code"], "0")
a.Destroy()

a = c.Run("s/m/a", wait=60, command="dd if=/dev/ram0 of=/dev/null count=1")
ExpectNe(a["exit_code"], "0")
a.Destroy()

m["devices"] = "/dev/ram0 rw"

a = c.Run("s/m/a", wait=60, command="dd if=/dev/ram0 of=/dev/null count=1")
ExpectEq(a["exit_code"], "0")
a.Destroy()

a = c.Run("s/m/a", wait=60, command="dd if=/dev/ram0 of=/dev/null count=1", devices="/dev/ram0 -")
ExpectNe(a["exit_code"], "0")
a.Destroy()

m["devices"] = "/dev/ram0 m"

a = c.Run("s/m/a", wait=60, command="dd if=/dev/ram0 of=/dev/null count=1")
ExpectNe(a["exit_code"], "0")
a.Destroy()

m["devices"] = ""

a = c.Run("s/m/a", wait=60, command="dd if=/dev/ram0 of=/dev/null count=1")
ExpectNe(a["exit_code"], "0")
a.Destroy()

m.Destroy()
s.Destroy()

m = c.Run("m", devices="/dev/ram0 rw")
b = c.Run("m/b", root_volume={"layers": ["ubuntu-precise"]}, devices="/dev/ram0 -")
b["devices"] = "/dev/ram0 rw"
a = c.Run("m/b/a", wait=60, command="dd if=/dev/ram0 of=/dev/null count=1")
ExpectEq(a["exit_code"], "0")
a.Destroy()
b.Destroy()
m.Destroy()

AsRoot()

a = c.Run("a", weak=False, **{"controllers[devices]": "true"})
b = c.Run("a/b", weak=False, **{"controllers[devices]": "true"})
ReloadPortod()
assert b["state"] == "meta"
b.Destroy()
a.Destroy()

os.chmod("/dev/ram0", 0660)
