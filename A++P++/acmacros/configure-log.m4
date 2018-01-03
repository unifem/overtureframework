AC_DEFUN(BTNG_AC_LOG,[echo "configure:__oline__:" $1 >&AC_FD_CC])

AC_DEFUN(BTNG_AC_LOG_VAR,[
dnl arg1 is list of variables to log.
dnl arg2 (optional) is a label.
dnl ifelse($2,,define(btng_log_label),define(btng_log_label,$2: ))
define([btng_log_label],ifelse($2,,,[$2: ]))
btng_log_vars="$1"
for btng_log_vars_index in $btng_log_vars ; do
  BTNG_AC_LOG("btng_log_label$btng_log_vars_index is '`eval echo \\\"\$\{$btng_log_vars_index\}\\\"`'")
done
undefine([btng_log_label])
])
