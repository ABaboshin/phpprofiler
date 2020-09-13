# A php profiler

Minimal php version 7.4.10 (./configure --with-curl)

```bash
# /usr/local/src/php-7.4.10/ext/ext_skel.php --ext phpprofiler

cd phpprofiler
phpize
./configure
make

php -d extension=modules/phpprofiler.so test.php

```

TODO:
 x intercept ZEND_DO_ICALL/ZEND_DO_UCALL/ZEND_DO_FCALL/ZEND_DO_FCALL_BY_NAME/ZEND_RETURN/ZEND_RETURN_BY_REF/ZEND_YIELD/ZEND_YIELD_FROM/ZEND_HANDLE_EXCEPTION
 - register interceptors on start
 - instrument internal function
 - interface IInterceptor (before/after/skip)
 - BaseClass BaseInterceptor
 - intercept curl
 - intercept pdo
 - intercept mysql
 - intercept incoming http
