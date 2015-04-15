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
operating systems, you should ensure that /dev/urandom is readable,
or customize random generation in `fill_buff`.
The call `srp_random_seed` has been removed.

The call `srp_user_new` has a new parameter, `username_for_verifier`,
allowing to use different usernames for verifier and srp login.

Added option for `srp_create_salted_verification_key` call to specify
a salt.

We ship with OpenSSL's implementation of the SHA256 hash algorithm.
Support for other hash algoritms was dropped (but re-introducing is
fairly easy, just copy from an OpenSSL source distribution).

Usage Example
-------------

For an usage example, see `test_srp.c`
