To Run:
--------------------
Server:
$make
$./server <port>

Clients:
$./client <serverip> <serverport>

Features:
--------------------
User authentication
User authentication is quite robust. It locks out entrants after repeated failed password attempts, logs out users who go offline without sending logout notices and assigns every used a Unique Session ID (USID) which enables absolutely every server transaction to be authenticated against good credentials and known location.

Message exchange
$message <username> <message>
Sends a message to the specified user. Only they receive it, only they see it.

Multiple client support
Should support many dozens (hundreds, thousands?) of users.

Heartbeat (alternative implementation)
In servercode/authentication.c is the heartbeat function. Rather than having the client ping the server every x seconds, a user is "checked in" whenever they send a message to the server. Furthermore, *all* user messages to the server trigger a review of the logged in users list. Any user who has not checked in for longer than the x seconds specified is sent a ping--failure to respond results in that user being formally logged out and all other connected users notified. A
proper ping response updates the user's lastcheckin time and keeps them logged in.

Offline messaging
Messages sent using $message are stored on the server if the specified recipient is offline and delivered in chronological order upon their logging in next, after which the stored messages are purged.

Broadcasts
$broadcast <message>
Works as specified. Sends a message to all logged-in users.

Display current users
$online
Works as specified. Sends and displays a list of logged-in users to the requester.

Logout
$logout
Alerts the server that the user is logging out, triggering the requisite server operations (de-authenticating the user and alerting other users to the logout) before shutting the client down.

Graceful exit (client side)
Sending a SIGINT on the client side initiates the logout function described above. The server has no graceful exit.

P2P features (all)
$getaddress <username>
$private <username> <message>
All P2P features are implemented, but calling getaddress <user> does not result in any feedback unless the requested user is offline. Otherwise, rest assured that it happened and send private messages with abandon. You'll be told if you're trying to message somebody you can't.

Blacklist (not implemented)

Bonus features (not implemented)

General Code Comments:
--------------------
Some of candlechat's server logic is quite hacky (requesthandler) and its main protocol (candlemsg) isn't exceptionally well designed. But much of the backend network infrastructure is quite robust (servertools, wireio, authenticate) and allows for reliable message packing, unpacking, sending and receiving. It's a rickety house on a very solid foundation.

Design:
--------------------
As much as was possible in the given time, Candlechat aspires to provide a server interface immune to the bad behavior of its clients. The server assumes as a given that malicious users will deliberately try to confuse and crash it. It is robust in authentication but presently vulnerable to an attack which could peg the server's memory by forcing it to store unlimited pending messages.

The design of Candlechat's most important protocol, the "candlemessage," is poorly thought out and ill managed. Excellent infrastructure assures they are sent and received reliably, but figuring out conventions for their use proved difficult. The design was "simplified" midway through the project, but the simplified design was implemented within the same struct and infrastructure as the original (to save time) and hence remained rather clunky.

The network infrastructure tucks away as much as possible the socket interface to expose an API more relevant to Candlechat directly. Candleexchange() provides the fundamental connectionless messaging function, while sendcandlemsg() allows recipients to choose how to respond to or acknowledge a message before closing the connection. The candleexchange() function supports a single transaction over one connection: open connection, call, response (optional), close connection.
