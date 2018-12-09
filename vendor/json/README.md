This is original nlohmann's json library (v 3.4.0) with two code fragments added.

After headers inclusion at line 59:
```c++
    #define JSON_APPEND_HEADER
    #include "additional_functions.cpp"
    #undef JSON_APPEND_HEADER
```

In end of json class at line 18596:
```c++
    #define JSON_APPEND_JSON
    #include "additional_functions.cpp"
    #undef JSON_APPEND_JSON
```

And in end of file at line 18692:
```c++
    #define JSON_APPEND_CONVERSIONS
    #include "additional_functions.cpp"
    #undef JSON_APPEND_CONVERSIONS
```

`additional_functions.cpp` is file with my functions that i added for resemblance with Qt's containers.
