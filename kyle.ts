-bash-4.2$ docker ps
CONTAINER ID        IMAGE                                      COMMAND                  CREATED             STATUS                  PORTS                                                                                                                                NAMES
037d5547f7a2        registry.vcs.sjrb.ad/nginx-proxy:1.1.246   "/bin/sh -c 'sh /e..."   19 hours ago        Up 19 hours             0.0.0.0:80->80/tcp, 0.0.0.0:136-139->136-139/tcp, 0.0.0.0:443->443/tcp, 0.0.0.0:445->445/tcp, 0.0.0.0:22000-22005->22000-22005/tcp   nginx-proxy
a6f28e180947        registry.vcs.sjrb.ad/vcs-auth:1.0.46       "/bin/sh -c 'node ..."   45 hours ago        Up 45 hours             0.0.0.0:5013->1337/tcp                                                                                                               vcs-auth
8a34154a05ad        registry.vcs.sjrb.ad/testacular:0.0.8      "/bin/sh -c 'node ..."   3 months ago        Up 3 months             7000/tcp, 0.0.0.0:5002->1337/tcp                                                                                                     testacular
e5ecad5314c9        registry.vcs.sjrb.ad/sis-monitor:1.2.21    "/bin/sh -c 'node ..."   7 months ago        Up 6 months (healthy)   5858/tcp, 0.0.0.0:5020->80/tcp                                                                                                       sis-monitor
-bash-4.2$ docker inspect testacular 

