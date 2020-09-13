PHP_ARG_ENABLE([phpprofiler],
  [whether to enable phpprofiler support],
  [AS_HELP_STRING([--enable-phpprofiler],
    [Enable phpprofiler support])],
  [no])

if test "$PHP_PHPPROFILER" != "no"; then
  AC_DEFINE(HAVE_PHPPROFILER, 1, [ Have phpprofiler support ])
  PHP_NEW_EXTENSION(phpprofiler, phpprofiler.c engine.c interceptor.c callStack.c, $ext_shared)
fi
