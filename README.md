[![rebol-speak](https://github.com/user-attachments/assets/351c5178-62d7-4e69-b1aa-1e1b4571d67b)](https://github.com/Oldes/Rebol-Speak)

[![Rebol-Speak CI](https://github.com/Oldes/Rebol-Speak/actions/workflows/main.yml/badge.svg)](https://github.com/Oldes/Rebol-Speak/actions/workflows/main.yml)
[![Gitter](https://badges.gitter.im/rebol3/community.svg)](https://app.gitter.im/#/room/#Rebol3:gitter.im)
[![Zulip](https://img.shields.io/badge/zulip-join_chat-brightgreen.svg)](https://rebol.zulipchat.com/)

# Rebol/Speak

Speak extension for [Rebol3](https://github.com/Oldes/Rebol3) (version 3.14.1 and higher)

## Usage
```rebol
import speak
say "Hello world!"

```

## Extension commands:


#### `list-voices`
Print a list of available system voices

#### `say` `:text`
Converts text to speech and plays it
* `text` `[string!]` The text to be spoken
* `/as`
* `voice` `[integer! handle!]` Specify the voice or handle to use for speech synthesis
* `/no-wait` Do not block execution while the speech is playing

