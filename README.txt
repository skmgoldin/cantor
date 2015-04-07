Cantor is a reference/testing client which will be kept compliant with v0.*
releases of the Chant chat protocol.

Quick Start
--------------------
$ make
$ ./client <ip> <port>

Features:
--------------------
Broadcasts
$ broadcast <message>
Sends a message to all logged-in users.

Private message exchange
$ message <username> <message>
Sends a message to the specified user. Only they receive it, only they see it.

Display current users
$ online
Sends and displays a list of logged-in users to the requester.

Logout
$ logout
Alerts the server that the user is logging out, triggering the requisite server
operations (de-authenticating the user and alerting other users to the logout)
before shutting the client down.

About Chant
--------------------
Aspirations:
Chant is an experimental chat protocol for secure, connectionless communication.
Wherever possible, Chant assumes that its clients are bad people dead set on
cheating, confusing, compromising and annhilating the server they are
communicating with. A compliant Chant server should be relatively secure by
virtue of its compliance alone.

Reality:
Chant is quite immature. Presently the protocol does provide connectionless
communication, but enforces zero security.


