# A small php profiler

Minimal php version 7.4.10 (./configure --with-curl)

```bash
# /usr/local/src/php-7.4.10/ext/ext_skel.php --ext phpprofiler

cd phpprofiler
phpize
./configure
make

PHPPROFILER_CONFIGURATION=/mnt/d/playground/phpprofiler/phpprofiler/interceptions/loader.php php -d extension=modules/phpprofiler.so test.php

```

A small php profiler with ability to intercept ZEND_DO_UCALL/ZEND_DO_FCALL/ZEND_DO_FCALL_BY_NAME/ZEND_RETURN/ZEND_RETURN_BY_REF.

Out of scope:
 - exception handling
 - instrument internal methods
