# A php profiler

```bash
# /usr/local/src/php-7.4.10/ext/ext_skel.php --ext phpprofiler

cd phpprofiler
phpize
./configure
make

php -d extension=modules/phpprofiler.so test.php

```
