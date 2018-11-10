This is original nlohmann's json library (v 3.4.0) with two code fragments added.

After headers inclusion at line 59:
```c++
    #define JSON_APPEND_HEADER
    #include "additional_functions.cpp"
    #undef JSON_APPEND_HEADER
```

And in end of json class at line 20155:
```c++
    #define JSON_APPEND_JSON
    #include "additional_functions.cpp"
    #undef JSON_APPEND_JSON
```

`additional_functions.cpp` is file with my functions that i added for resemblance with Qt's containers.
