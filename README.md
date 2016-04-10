csrp-gmp
========

csrp-gmp is a minimal C implementation of the [Secure Remote Password
protocol](http://srp.stanford.edu/), originally written by Tom Cocagne
to depend on OpenSSL, ported to LibGMP by est31.
The project is intended for direct inclusion into utilizing programs.
It's only dependency is LibGMP.

SRP Overview
------------

SRP is a cryptographically strong authentication
protocol for password-based, mutual authentication over an insecure
network connection.

Unlike other common challenge-response autentication protocols, such
as Kereros and SSL, SRP does not rely on an external infrastructure
of trusted key servers or certificate management. Instead, SRP server
applications use verification keys derived from each user's password
to determine the authenticity of a network connection.

SRP provides mutual-authentication in that successful authentication
requires both sides of the connection to have knowledge of the
user's password. If the client side lacks the user's password or the
server side lacks the proper verification key, the authentication will
fail.

Unlike SSL, SRP does not directly encrypt all data flowing through
the authenticated connection. However, successful authentication does
result in a cryptographically strong shared key that can be used
for symmetric-key encryption.

Porter's notes
--------------

Compared with [csrp](https://github.com/cocagne/csrp), some things
have changed for the outside.
As LibGMP doesn't ship with a cryptographically strong PRNG, strong
PRNGs provided (and seeded) by the OS are used instead. On non-Windows
operating systems, we try to read the file `/dev/urandom`, if this fails,
we use a (poorly) seeded pcgrandom instead. The call `srp_random_seed`
has been removed.

The call `srp_user_new` has a new parameter, `username_for_verifier`,
allowing to use different usernames for verifier and srp login.
Also, `srp_user_start_authentication` and `srp_verifier_new` have new
parameters to specify `a` and `b` values.

Also, some cleanups were done regarding types: lengths are now `size_t`,
and the `const` qualifier has been removed at many places where its
usage was misleading.

Added option for `srp_create_salted_verification_key` call to specify
a salt. You can now specify `bytes_b` for `srp_verifier_new`,
`bytes_a` for `srp_user_start_authentication`, allowing full control
over the login process, which is good for unit tests.
`csrp-gmp` itself uses this additional flexibility to automatically test
for [RFC 5054](https://tools.ietf.org/html/rfc5054) compatibility inside
`test_srp.c`.

`RFC 5054` compatibility especially means that compatibility to csrp's
master branch has been dropped. `csrp-gmp` is compatible to the
[rfc5054_compat](https://github.com/cocagne/csrp/tree/rfc5054_compat)
branch of `csrp` though.

We ship with OpenSSL's implementation of the SHA256 and SHA-1 hash
algorithms. Support for other hash algoritms was dropped (but
re-introducing is fairly easy, just copy from an OpenSSL source
distribution).

Development
-----------

### Clang format

You can invoke clang-format like:
```
clang-format-3.7 -i *.c *.h
```

You should do this before doing any commit to the code.
Best is you put it into a commit hook, like when you execute
these bash instructions:
```bash
cat << "EOF" > .git/hooks/pre-commit
#!/bin/bash
for f in `git diff-index --cached --name-only HEAD | grep -E "\.(c|h)$"` ; do
	clang-format-3.7 -i "${f}"
	git add "${f}"
done
EOF
chmod +x .git/hooks/pre-commit
```

### Quick compile helper

If you want to compile this and get running fast, this is the command
you can use on a recent Ubuntu (~15.04) box on a 64 bit arch:

```
cc -L/usr/lib/x86_64-linux-gnu/ -I/usr/include/ -lgmp -I. srp.c test_srp.c -Isha sha/sha*.c -lgmp -o srp-test
```
For other distributions/operating systems, you might have to adjust the paths.

Usage Example
-------------

For an usage example, see `test_srp.c`
