# RepostBot

Telegram (or maybe other messengers later) bot for reposting messages/posts from various sites.


## Current project

- Telegram subscribe bot (Export)
- VK wall posts import (Import)


## Sites that will maybe supported in future

- Matrix(Riot) subscribe bot
- VK subscribe bot

- Telegram channel posts import
- Bash.im, ITHappens.me, Zadolba.li
- RSS feeds
- xkcd.com
- rtut.io
- pikabu.ru


## File structure
- exports
  - ETelegram - Telegram bot
- imports
  - IVk - VK API
- interfaces
  - Exporter
  - Importer
  - Model
  - Task
  - Threadable
  - User - Abstraction for storing user ids
- models
  - Error
  - Post
  - Status
- queue
  - QueueHandler - Interface
  - QueueManager
  - QueueTask
- tasks
  - TaskManager - create and manage tasks
  - vk
    - PostsVkTask - Fetch last posts from users subscriptions
- tests - Temporary storage for test code, before i learn any testing framework
- utils
  - concurrent
    - UFuture - report about finished task and return value from another thread
  - USingleton
  - Utils
- vendor
  - tgbot-cpp - Telegram Bot API library
  - cotire - Unity builds
  - verdigris - Replacement for MOC (Needed for unity builds)
- Runner - keeper of threads,
- Storage - json storage
- vars - secure variables: tokens, etc.
- vars.h.example - template for secure variables
- main


## Storage

I think permanent storage of posts isn't needed, so i will store them in memory, and delete after send is finished.

```json
{
    "task.vk.posts": {
        "last_ids": {
            "<group_id>": "<last_post_id>"
        }
    },
    "tasks": {
        "last_launched": {
            "task.vk.posts": <last_successfull_launch_timestamp>
        }
    },
    "vk.groups": {
        "<user_id>": [
            "<group_name_1>",
            "<group_name_2>",
            "<group_name_3>"
        ]
    }
}
```


## Tasks - Users

`User` is universal user_id storage. Each bot should have separate constructor and add prefix to id, for methods like `isTelegram` to work. Model method `sendTo(User)` decides to which `Export` send itself, and will add task for it.


## TODO

Vk:
  - fetch not last but all unread (except for new groups), maybe move to vk.execute
Rename and move vk and telegram to `api` folder (Terms Exporter and Importer aren't suitable)
  - separate to `bots` and `handlers` folders


# Conventions

Any method that writes to Storage should call save after last write or at end of function


#### Singletons

- Runner
- Storage
