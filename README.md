# RepostBot

Telegram (or maybe other messengers later) bot for reposting messages/posts from various sites.


## Current project

- Telegram subscribe bot (Export)
- VK group posts import (Import)


## File structure
- interfaces
  - Threadable
  - Exporter
  - Importer
  - Model
  - User - Abstraction for storing user ids
- exports
  - ETelegram - Telegram bot
- imports
  - IVK - Get group posts
- models
  - Error
  - Post
- utils
  - USingleton
- vendor
  - tgbot-cpp - Telegram Bot API library
- Storage - json storage
- vars - secure variables: tokens, etc.
- vars.h.example - template for secure variables
- Runner - keeper of threads
- main


## Storage

I think permanent storage of posts isn't needed, so i will store them in memory, and delete after send if finished.

```json
{
  "import.vk": { - Import/Export variables
    "token": "<token>"
  },
  "export.telegram": {
  },
  "vk.groups": {
    "<user_uid>": [
      "<group_id 1>",
      "<group_id 2>",
      "<group_id 3>"
    ]
  }
}
```


## TODO

Vk thread code (fetch last post from all subscribed groups)
Save Storage on write


# Conventions

In any method with writing to Storage should call save after last write or at end of function


#### Singletons

- Runner
- Storage
