# strong hold

## Overview

Serverside of stronghold panda application.

## Protocol Messages

Every string is encoded as `UTF16BE` and prefixed by a 2 byte length field `uint16_t`. Depending on the concrete message, it is possible to use length fields of differing sizes.

### Header

 - 4 byte magic: `0xBADC0DED`
 - 1 byte version: `0x01`
 - 2 byte total length: `uint16_t`
 - 1 byte message id: `uint8_t`

### Messages

#### Hello `0xA0`
Sent to server after connection was established, gets answered with welcome message.

 - 2 byte length: `uint8_t`
 - `length` byte `UTF16BE` encoded user name

#### Welcome `0xF0`
Sent to connecting client after hello message was received.
 - 4 byte client id: `uint32_t`

#### Connect `0xF1`
Sent by server to inform all clients about new connection.
- 4 byte client id: `uint32_t`
- 2 byte length: `uint8_t`
- `length` byte `UTF16BE` encoded user name

#### Disconnect `0xC0`
Sent by server to inform all clients about disconnect. A friendly client can also send this message to the server.
- 4 byte client id: `uint32_t`

#### Key Event  `0xC1`
Sent by server or concrete client to inform other parties about performed actions.
- 4 byte client id: `uint32_t`
- 1 byte key: `uint8_t`
- 1 byte key event: `uint8_t`
- 4 byte timestamp: `uint64_t` (ms since epoch)

Possible keys and their encodings are in the table below.

| key  | left | right | up | down | jump |
|------|------|-------|----|------|------|
| code | 1    | 2     | 4  | 8    | 16   |

Every key can be in one of the following states. 

| event | pressed | typed | released |
|-------|---------|----------|-------|
| code  | 1       | 2        | 4     |

Pressing and releasing a key on the keyboard results in the generating the following key events (in order):

 - KEY_PRESSED
 - KEY_TYPED (is only generated if a valid Unicode character could be generated.)
 - KEY_RELEASED
 
Due to reduction of network bandwith the key event KEY_TYPED is not sent, but implicitly derived in between
KEY_PRESSED and KEY_RELEASED events.

#### Reconciliation `0xF2`
Sent by server to all clients in order to reconsile player positions.
- 4 byte client id: `uint32_t`
