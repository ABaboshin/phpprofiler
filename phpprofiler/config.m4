PHP_ARG_ENABLE([phpprofiler],
  [whether to enable phpprofiler support],
  [AS_HELP_STRING([--enable-phpprofiler],
    [Enable phpprofiler support])],
  [no])

if test "$PHP_PHPPROFILER" != "no"; then
  AC_DEFINE(HAVE_PHPPROFILER, 1, [ Have phpprofiler support ])
  PHP_REQUIRE_CXX()
  PHP_NEW_EXTENSION(phpprofiler, phpprofiler.c hooks.cpp zend_handlers.cpp, $ext_shared)
fi
