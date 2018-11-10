## File structure
- apis
  - bots
    - TelegramBot
  - handlers
    - VkHandler
  - TelegramApi
  - VkApi
- interfaces
  - Bot - QThread
  - Model
  - Task
  - User - Abstraction for storing user ids
- models
  - Attachment - Photos, Videos, Music, etc.
  - Error
  - Post
  - Posts
  - Status
- queue
  - QueueHandler - Interface
  - QueueManager
  - QueueTask
- tasks
  - TaskManager - create and manage tasks
  - vk
    - PostsVkTask - Fetch last posts from user subscriptions
- tests - Temporary storage for test code, before i learn any testing framework
- utils
  - concurrent
    - UFuture - report about finished task and return value from another thread
  - USingleton
  - Utils
- vendor
  - cotire - Unity builds (All cpp files compiled as one)
  - tgbot-cpp - Telegram Bot API library
  - verdigris - Replacement for MOC (Needed for unity builds)
- Runner - keeper of pointers to Queue, Bots and Api's
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
            "group_id": "post_id"
        }
    },
    "tasks": {
        "last_launched": {
            "task.vk.posts": timestamp
        }
    },
    "vk": {
        "group_names": {
            "group_id": "group_name"
        },
        "groups": {
            "TUser": [
                "group name 1",
                "group name 2",
                "group name 3"
            ]
        },
        "tokens": {
            "TUser": "access token"
        }
    }
}
```


## Models

`User` is universal user_id storage. Each bot should have separate constructor and add prefix to id, for methods like `isTelegram` to work. Model method `sendTo(User)` decides to which `Export` send itself, and will add task for it.


## TODO

Handle errors, and logs (write to temp file, send to "admin" user)


## Conventions

Any method that writes to Storage should call save after last write or at end of function


#### Singletons

- Runner
- Storage


## VK

Code for stored procedure `execute.fetchLastPostFrom`.
Can take only 20 groups


```javascript
var POSTS_COUNT = 10;

var group_ids = Args.group_ids.split(",");
var post_ids = Args.post_ids.split(",");

var ig = 1, groups_count = group_ids.length;
var r = [];

while (ig < groups_count) {
    var group_id = group_ids[ig];
    var posts;
    var ip = 0;
    var not_break = true;

    if (group_id.substr(0,1) == "-") {
        posts = API.wall.get({count: POSTS_COUNT, filter: "owner", v: "5.85", owner_id: group_id}).items;
    } else {
        posts = API.wall.get({count: POSTS_COUNT, filter: "owner", v: "5.85", domain: group_id}).items;
    }

    var posts_count = posts.length;

    while (ip < posts_count) {
        var post = posts[ip];
        if (post.id == post_ids[ig - 1]) {
            not_break = false;
        }

        if (not_break && post.is_pinned != 1) {
            post.domain = group_id;
            r.push(post);
        }

        ip = ip + 1;
    }

    ig = ig + 1;
};

return r;

```

