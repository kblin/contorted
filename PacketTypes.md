# Introduction #

Dplay uses a plethora of different packet types to carry information.
Here is an overview over the packets that I could figure out so far.

Note that the packets I am talking about here do not include the actual updates the game sends, those will be sent via UDP using application-specific encoding of the data. (Well, unless it's a type 0x0016 packet. Getting an idea why this project is called contorted?)


# List of packets #

The following packets have been identified, grouped by their packet ID (see DplayHeaders for details on that.):

  * DplayHeaders. (Not a real packet, but deserves an extra page.)


  * Type `0x0001`: LobbyInformation
  * Type `0x0002`: LobbyBroadcast
  * Type `0x0004`: GameInformationRequest
  * Type `0x0005`: IdRequest
  * Type `0x0007`: IdReply
  * Type `0x0008`: PlayerInformation
  * Type `0x000b`: ConnectionShutdown
  * Type `0x000d`: BeginTransmission
  * Type `0x000e`: EndTransmission
  * Type `0x000f`: GameSettingsChanged
  * Type `0x0013`: JoinGame
  * Type `0x0015`: MessageContainer
  * Type `0x0016`: DataSend
  * Type `0x0017`: DataAck
  * Type `0x001a`: -unknown-
  * Type `0x0029`: GameInformation
  * Type `0x002e`: NewPlayerNotification
  * Type `0x002f`: NewPlayerConfirmation
  * Type `0x0038`: PlayerInformationNewPlayer