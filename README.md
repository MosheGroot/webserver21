# Webserv project



## Goal:

Write own implementation of web server like nginx one with next capabilities:
  * custom configs
  * multiple servers handling
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
| HTTP + requests processing + default pages    | Config + CGI + Kvast website             | Client-server connections, multiplexing and etc.              |




## Testing

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
