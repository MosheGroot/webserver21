# Webserv project



## Goal:

Write own implementation of web server like nginx one with next capabilities:
  * custom configs
  * multiple servers handling at one or different `ip:port` pairs
  * multiple client-server connections
  * simple CGI
  * multiple routes in server, error pages, redirects and max client body size paramets
  * `GET`, `POST` and `DELETE` methods with `HTTP/1.1` version of requests

Language: `C++98`




## Result

* Structured code with object-oriented principles splitted by 5 parts:
  + Config
  + Core
  + Http
  + CGI
  + Utils
* Custom configs with multiple servers with next parameters:
  + ip and port
  + server_name
  + max client body size 
  + locations:
    + uri
    + root
    + index
    + autoindex
    + allowed methods
    + cgi
    + redirect
* Multiple servers at one or different ip:port socket
* Multiplexing with low-level `select` function for non-blocking sockets problem-solving
* `GET`, `POST` and `DELETE` methods
* Autoindex pages
* Execution of simple CGI bash scripts or binaries
* Simple thread-unsafety logger to console and file

## Contributors


| <img src="https://avatars.githubusercontent.com/u/44144647?v=4" width="25px"> [PP189B][PP189B_profile]  | <img src="https://avatars.githubusercontent.com/u/53272893?v=4" width="25px"> [Ulians-S][Uliana-S_profile]  | <img src="https://avatars.githubusercontent.com/u/77484081?v=4" width="25px"> [AliceFromWardenfell][AliceFromWardenfell_profile] |
|---------------------------------------|------------------------------------------|---------------------------------------------------------------|
| HTTP + requests processing + default pages    | Config + CGI + [Kvast website](https://github.com/Uliana-S/kvast_htmlacademy)     | Client-server connections, multiplexing and etc.              |




## Testing

### Current configuration

* [Kvast website](https://github.com/Uliana-S/kvast_htmlacademy) at `http://kvast.webserv.local`
![image](https://user-images.githubusercontent.com/44144647/157631309-1955e75e-6820-4dcc-b2c4-3b0b54b4a013.png)

* Test website at `webserv.local:8888`
![image](https://user-images.githubusercontent.com/44144647/157630839-35b3fb01-0b3f-4a89-9284-80afc16c1333.png)

* Default website at `default.webserv.local:8888`
![image](https://user-images.githubusercontent.com/44144647/157630648-2fc203ae-2b3e-4b3f-a52a-274df46ec3e4.png)

###### (P.S. you need to configure domains in `hosts` file of your system for testing)

### Configurating

Use this template for adding new server:
```
server
    ip   <...>
    port <...>
    server_name   <name1> <name2> <name3>
    max_body_size <number in bytes>
    location <uri>
        root      <...>
        index     <...>
        cgi       <...>
        redirect  <...>
        method    <m1> <m2> <m3>
        autoindex <on/off>
    location <uri>
        ...
```

### Build and run

```
make

./webserver [path_to_config]
```


[PP189B_profile]: https://github.com/PP189B
[Uliana-S_profile]: https://github.com/Uliana-S
[AliceFromWardenfell_profile]: https://github.com/AliceFromWardenfell
