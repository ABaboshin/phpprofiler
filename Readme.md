# A php profiler

```bash
# /usr/local/src/php-7.4.10/ext/ext_skel.php --ext phpprofiler

cd phpprofiler
phpize
./configure
make

php -d extension=modules/phpprofiler.so test.php

```

TODO:
 - intercept ucall/fcall/fcall_by_name
 - register interceptors on start
 - interface IInterceptor (before/after/skip)
 - BaseClass BaseInterceptor
 - intercept curl
 - intercept pdo
 - intercept mysql
 - intercept incoming http
